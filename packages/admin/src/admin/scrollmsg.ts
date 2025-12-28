// Scrolling Message Plugin
// Converted from AMX Mod X scrollmsg.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO
//
// Features:
// - Smooth left-to-right scrolling animation on the HUD
// - Configurable message text and repeat frequency
// - %hostname% placeholder support
// - Uses TE_TEXTMESSAGE for cross-platform HUD display
//
// Usage:
//   amx_scrollmsg "<message>" [frequency_seconds]
//
// Example:
//   amx_scrollmsg "Welcome to %hostname%!" 60

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';

const cvar = nodemodCore.cvar;

// Constants
const SPEED = 0.3;
const SCROLLMSG_SIZE = 512;

class ScrollMsg extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Scrolling Message',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Scrolling HUD messages'
    };

    // Message state
    private startPos: number = 0;
    private endPos: number = 0;
    private scrollMsg: string = '';
    private displayMsg: string = '';
    private xPos: number = 0.65;
    private length: number = 0;
    private frequency: number = 0;

    // Timers
    private scrollTimer: NodeJS.Timeout | null = null;
    private initTimer: NodeJS.Timeout | null = null;

    constructor(pluginName: string) {
        super(pluginName);

        // Register server command
        this.registerCommands();
    }

    private registerCommands() {
        // Server command to set the scrolling message
        nodemodCore.cmd.registerServer('amx_scrollmsg', (args: string[]) => {
            this.setMessage(args);
        });
    }

    /**
     * Show one frame of the scrolling message
     */
    private showMsg() {
        // Extract the portion of the message to display
        this.displayMsg = this.scrollMsg.substring(this.startPos, this.endPos);

        // Expand the end position if not at full length
        if (this.endPos < this.length) {
            this.endPos++;
        }

        // Move the message to the left
        if (this.xPos > 0.35) {
            this.xPos -= 0.0063;
        } else {
            this.startPos++;
            this.xPos = 0.35;
        }

        // Display HUD message to all players
        this.showHudMessage(this.displayMsg, this.xPos);
    }

    /**
     * Show a HUD message to all players
     *
     * HUD parameter scaling (AMXX TE_TEXTMESSAGE format):
     * - x, y: float 0.0-1.0 * 8192 (for center use -8192)
     * - times: seconds * 256 (1/256th second units)
     */
    private showHudMessage(message: string, xPos: number) {
        const players = adminSystem.getPlayers({ excludeBots: false });

        for (const player of players) {
            try {
                // Use HUD message if available
                if (nodemodCore.util && nodemodCore.util.showHudText) {
                    nodemodCore.util.showHudText(player, message, {
                        x: Math.floor(xPos * 8192),  // xPos ranges 0.35-0.65 for scroll effect
                        y: Math.floor(0.90 * 8192),  // Near bottom of screen
                        r1: 200,
                        g1: 100,
                        b1: 0,
                        a1: 255,
                        fadeinTime: 0,   // No fade - instant appear
                        fadeoutTime: 0,  // No fade - instant disappear
                        holdTime: Math.floor(SPEED * 2 * 256),  // Twice the interval for overlap
                        channel: 2
                    });
                } else {
                    // Fallback to center print
                    nodemod.eng.clientPrintf(player, nodemod.PRINT_TYPE.print_center, message);
                }
            } catch (e) {
                // Ignore errors for individual players
            }
        }
    }

    /**
     * Initialize a new scroll cycle
     */
    private msgInit() {
        this.endPos = 1;
        this.startPos = 0;
        this.xPos = 0.65;

        // Replace %hostname% placeholder
        const hostname = cvar.getString('hostname') || 'Server';
        let processedMsg = this.scrollMsg.replace(/%hostname%/g, hostname);

        // Store processed message
        this.scrollMsg = processedMsg;
        this.length = this.scrollMsg.length;

        // Clear any existing scroll timer
        if (this.scrollTimer) {
            clearInterval(this.scrollTimer);
        }

        // Calculate how many scroll frames we need
        const totalFrames = this.length + 48;
        let frameCount = 0;

        // Start scrolling
        this.scrollTimer = setInterval(() => {
            if (frameCount >= totalFrames) {
                if (this.scrollTimer) {
                    clearInterval(this.scrollTimer);
                    this.scrollTimer = null;
                }
                return;
            }

            this.showMsg();
            frameCount++;
        }, SPEED * 1000);
    }

    /**
     * Set the scrolling message (server command handler)
     */
    private setMessage(args: string[]) {
        // Remove current messaging
        this.stopScrolling();

        if (args.length < 1) {
            console.log('[ScrollMsg] Usage: amx_scrollmsg <message> [frequency_seconds]');
            return;
        }

        // Get message
        this.scrollMsg = args[0] || '';
        this.length = this.scrollMsg.length;

        // Get frequency
        const freqStr = args[1] || '0';
        this.frequency = parseInt(freqStr) || 0;

        if (this.frequency > 0) {
            // Calculate minimum frequency based on message length
            const minimal = Math.round((this.length + 48) * (SPEED + 0.1));

            if (this.frequency < minimal) {
                console.log(this.getLang(null, 'MIN_FREQ', minimal.toString()));
                this.frequency = minimal;
            }

            const minutes = Math.floor(this.frequency / 60);
            const seconds = this.frequency % 60;
            console.log(this.getLang(null, 'MSG_FREQ', minutes.toString(), seconds.toString()));

            // Start the repeating task
            this.msgInit();

            this.initTimer = setInterval(() => {
                this.msgInit();
            }, this.frequency * 1000);
        } else {
            console.log(this.getLang(null, 'MSG_DISABLED'));
        }
    }

    /**
     * Stop all scrolling
     */
    private stopScrolling() {
        if (this.scrollTimer) {
            clearInterval(this.scrollTimer);
            this.scrollTimer = null;
        }
        if (this.initTimer) {
            clearInterval(this.initTimer);
            this.initTimer = null;
        }
    }

    /**
     * Plugin unload handler
     */
    onUnload() {
        this.stopScrolling();
    }
}



export default ScrollMsg;
