// Anti Flood Plugin
// Converted from AMX Mod X antiflood.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

// ============================================================================
// Anti Flood Plugin
// ============================================================================

class AntiFlood extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Anti Flood',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Prevents chat flooding'
    };

    // Flood tracking per player (index -> next allowed time)
    private flooding: { [index: number]: number } = {};
    // Flood counter per player (index -> count 0-3)
    private floodCount: { [index: number]: number } = {};

    // CVAR wrapper
    private amxFloodTime: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Register CVARs using BasePlugin helper (auto-tracks for pluginmenu)
        this.amxFloodTime = this.registerCvar(
            'amx_flood_time',
            '0.75',
            nodemod.FCVAR.SERVER,
            'Minimum time between chat messages (0 = disabled)'
        );

        // Hook say commands
        this.hookSayCommands();

        // Handle player disconnect (cleanup)
        nodemod.on('dllClientDisconnect', (entity: nodemod.Entity) => {
            this.onClientDisconnect(entity);
        });
    }

    private hookSayCommands() {
        nodemod.on('dllClientCommand', (entity: nodemod.Entity, text: string) => {
            const args = utils.parseCommand(text);
            if (args.length === 0) return;

            const cmd = args[0].toLowerCase();

            if (cmd === 'say' || cmd === 'say_team') {
                if (this.checkFlood(entity)) {
                    nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
                }
            }
        });
    }

    /**
     * Check if player is flooding
     * Returns true if message should be blocked
     */
    private checkFlood(entity: nodemod.Entity): boolean {
        const maxChat = this.amxFloodTime?.float || 0;

        // If flood protection is disabled, allow message
        if (!maxChat) {
            return false;
        }

        const index = nodemod.eng.indexOfEdict(entity);
        const now = utils.getGameTime();

        // Initialize if needed
        if (this.flooding[index] === undefined) {
            this.flooding[index] = 0;
            this.floodCount[index] = 0;
        }

        // Check if player is chatting too fast
        if (this.flooding[index] > now) {
            // Player is flooding
            if (this.floodCount[index] >= 3) {
                // Too many flood attempts - block and add penalty
                const message = this.getLang(entity, 'STOP_FLOOD');
                nodemod.eng.clientPrintf(entity, nodemod.PRINT_TYPE.print_console, `** ${message} **\n`);

                // Add extra penalty time
                this.flooding[index] = now + maxChat + 3.0;
                return true;
            }

            // Increment flood counter
            this.floodCount[index]++;
        } else if (this.floodCount[index] > 0) {
            // Enough time passed - decrement counter
            this.floodCount[index]--;
        }

        // Update next allowed time
        this.flooding[index] = now + maxChat;

        return false;
    }

    private onClientDisconnect(entity: nodemod.Entity) {
        const index = nodemod.eng.indexOfEdict(entity);
        delete this.flooding[index];
        delete this.floodCount[index];
    }
}



export default AntiFlood;
