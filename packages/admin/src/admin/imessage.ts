// Info Messages Plugin
// Converted from AMX Mod X imessage.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO
//
// State Persistence Difference from SMA:
// The original AMXX uses localinfo (server-side key-value store) to track the
// current message index across map changes. This persists only while the server
// is running.
//
// NodeMod uses file-based state persistence (imessage_state.txt), which persists
// the message index across server restarts as well. This is intentionally more
// durable to maintain message rotation continuity across server maintenance.

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

// HUD position and timing
const X_POS = -1.0;  // Center
const Y_POS = 0.20;  // Near top
const HOLD_TIME = 12.0;

// Message entry with RGB color
interface MessageEntry {
    message: string;
    r: number;
    g: number;
    b: number;
}

class InfoMessages extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Info. Messages',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Displays rotating info messages'
    };

    private messages: MessageEntry[] = [];
    private currentIndex: number = 0;
    private timerId: ReturnType<typeof setTimeout> | null = null;
    private amxFreqImessage: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Load dictionaries for localization
        const localization = require('./localization').default;
        localization.loadDictionary('imessage');
        localization.loadDictionary('common');

        // Register CVAR
        this.registerCvar('amx_freq_imessage', '10', nodemod.FCVAR.SERVER, 'Frequency of info messages in seconds (0 = disabled)');
        this.amxFreqImessage = cvar.wrap('amx_freq_imessage');

        // Register server command
        nodemodCore.cmd.registerServer('amx_imessage', (args) => {
            this.setMessage(args);
        });

        // Load messages from imessage.ini
        this.loadFromFile();

        // Load persisted message index
        this.loadIndex();

        // Schedule initial timer if messages were loaded and frequency > 0
        if (this.messages.length > 0) {
            const freq = this.amxFreqImessage?.float || 10;
            if (freq > 0) {
                this.scheduleNextMessage(freq);
            }
        }
    }

    private getStateFile(): string {
        return path.join(utils.getConfigsDir(), 'imessage_state.txt');
    }

    /**
     * Load the current message index from state file
     */
    private loadIndex(): void {
        const stateFile = this.getStateFile();
        try {
            if (fs.existsSync(stateFile)) {
                const content = fs.readFileSync(stateFile, 'utf-8').trim();
                this.currentIndex = parseInt(content) || 0;
            }
        } catch (e) {
            this.currentIndex = 0;
        }
    }

    /**
     * Save the current message index to state file
     */
    private saveIndex(): void {
        const stateFile = this.getStateFile();
        try {
            fs.writeFileSync(stateFile, this.currentIndex.toString());
        } catch (e) {
            // Ignore save errors
        }
    }

    /**
     * Load messages from imessage.ini file
     * Format: "message" RRRGGGBBB
     */
    private loadFromFile(): void {
        const filePath = path.join(utils.getConfigsDir(), 'imessage.ini');

        if (!fs.existsSync(filePath)) {
            return;
        }

        try {
            const content = fs.readFileSync(filePath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';') || trimmed.startsWith('//')) continue;

                // Parse: "message" RRRGGGBBB
                const parts = utils.parseCommand(trimmed);
                if (parts.length >= 1) {
                    this.setMessage(parts);
                }
            }

            if (this.messages.length > 0) {
                console.log(this.getLang(null, 'LOADED_MESSAGES', this.messages.length.toString()));
            }
        } catch (e) {
            console.error('[InfoMessages] Error loading imessage.ini:', e);
        }
    }

    private setMessage(args: string[]) {
        // Stop existing timer
        if (this.timerId) {
            clearTimeout(this.timerId);
            this.timerId = null;
        }

        if (args.length < 1 || !args[0]) {
            console.log(this.getLang(null, 'USAGE_IMESSAGE'));
            return;
        }

        let message = args[0];
        const colorStr = args[1] || '255255255';  // Default to white if no color

        // Replace \n with actual newlines
        message = message.replace(/\\n/g, '\n');

        // Parse RRRGGGBBB color format
        const r = parseInt(colorStr.substring(0, 3)) || 255;
        const g = parseInt(colorStr.substring(3, 6)) || 255;
        const b = parseInt(colorStr.substring(6, 9)) || 255;

        // Add message
        this.messages.push({ message, r, g, b });

        // Start timer if frequency > 0
        const freq = this.amxFreqImessage?.float || 10;
        if (freq > 0) {
            this.scheduleNextMessage(freq);
        }
    }

    private scheduleNextMessage(delaySeconds: number) {
        this.timerId = setTimeout(() => {
            this.showInfoMessage();
        }, delaySeconds * 1000);
    }

    private showInfoMessage() {
        if (this.messages.length === 0) {
            return;
        }

        if (this.currentIndex >= this.messages.length) {
            this.currentIndex = 0;
        }

        const entry = this.messages[this.currentIndex];
        let message = entry.message;

        // Replace %hostname% placeholder
        const hostname = cvar.getString('hostname') || 'Server';
        message = message.replace(/%hostname%/g, hostname);

        // Show HUD message to all players
        const options = {
            channel: 4,
            x: X_POS === -1 ? -8192 : Math.floor(X_POS * 8192),
            y: Math.floor(Y_POS * 8192),
            r1: entry.r,
            g1: entry.g,
            b1: entry.b,
            a1: 255,
            fadeinTime: Math.floor(2.0 * 256),
            fadeoutTime: Math.floor(2.0 * 256),
            holdTime: Math.floor(HOLD_TIME * 256)
        };

        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            nodemodCore.util.showHudText(player, message, options);
            nodemod.eng.clientPrintf(player, nodemod.PRINT_TYPE.print_console, `${message}\n`);
        }

        this.currentIndex++;

        // Persist the index for next plugin load
        this.saveIndex();

        // Schedule next message
        const freq = this.amxFreqImessage?.float || 10;
        if (freq > 0) {
            this.scheduleNextMessage(freq);
        }
    }
}



export default InfoMessages;
