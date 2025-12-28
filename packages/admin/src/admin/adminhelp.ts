// Admin Help Plugin
// Converted from AMX Mod X adminhelp.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by tcquest78

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_ADMIN, ADMIN_USER } from './constants';
import { helpRegistry } from './helpregistry';
import localization from './localization';
import * as utils from './utils';
import { Plugin, PluginMetadata } from './pluginloader';
import { BasePlugin } from './baseplugin';

const cvar = nodemodCore.cvar;
const HELP_AMOUNT = 10; // Commands per page
const DISPLAY_MSG = true; // Show message on join

// ============================================================================
// Admin Help Plugin
// ============================================================================

class AdminHelp extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Help',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Help command showing available admin commands'
    };

    private pendingMessages: Map<number, ReturnType<typeof setTimeout>> = new Map();

    // CVARs
    private mpTimelimit: any;
    private amxNextmap: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Get CVAR wrappers
        this.mpTimelimit = cvar.wrap('mp_timelimit');
        this.amxNextmap = cvar.wrap('amx_nextmap');

        // Load localization
        localization.loadDictionary(this.pluginName);

        // Register amx_help command (flags=0 means everyone can use it)
        this.registerCommand('amx_help', 0, '<page> [nr of cmds (only for server)] - displays this help', (entity, args) => {
            this.cmdHelp(entity, args);
        });

        // Setup event handlers for join message
        if (DISPLAY_MSG) {
            this.setupEventHandlers();
        }
    }

    private setupEventHandlers() {
        nodemod.on('dllClientPutInServer', (entity: nodemod.Entity) => {
            if (utils.isBot(entity)) return;

            const index = nodemod.eng.indexOfEdict(entity);

            // Show help message after 15 seconds
            const timeout = setTimeout(() => {
                this.displayInfo(entity);
                this.pendingMessages.delete(index);
            }, 15000);

            this.pendingMessages.set(index, timeout);
        });

        nodemod.on('dllClientDisconnect', (entity: nodemod.Entity) => {
            const index = nodemod.eng.indexOfEdict(entity);
            const timeout = this.pendingMessages.get(index);
            if (timeout) {
                clearTimeout(timeout);
                this.pendingMessages.delete(index);
            }
        });
    }




    /**
     * amx_help [page] [amount] - Display available commands
     */
    private cmdHelp(entity: nodemod.Entity | null, args: string[]) {
        let userFlags = entity ? adminSystem.getUserFlags(entity) : 0xFFFFFFFF;

        // HACK: ADMIN_ADMIN is never set as user's actual flags
        // If user is an admin (has flags > 0 and not just ADMIN_USER), add ADMIN_ADMIN
        if (userFlags > 0 && !(userFlags & ADMIN_USER)) {
            userFlags |= ADMIN_ADMIN;
        }

        // Parse page number
        let start = args.length > 0 ? parseInt(args[0]) || 1 : 1;

        // Parse amount (server only)
        let helpAmount = HELP_AMOUNT;
        if (!entity && args.length >= 2) {
            helpAmount = parseInt(args[1]) || HELP_AMOUNT;
        }

        // Adjust to 0-based index
        start--;
        if (start < 0) start = 0;

        const totalCommands = helpRegistry.getCommandCount(userFlags);

        if (totalCommands === 0) {
            this.sendConsole(entity, this.getLang(entity, 'NO_COMMANDS'));
            return;
        }

        if (start >= totalCommands) {
            start = totalCommands - 1;
        }

        // Header
        this.sendConsole(entity, '');
        this.sendConsole(entity, `----- ${this.getLang(entity, 'HELP_COMS')} -----`);

        // Get commands for this page
        const commands = helpRegistry.getCommandPage(userFlags, start, helpAmount);
        const end = start + commands.length;

        // Display commands
        for (let i = 0; i < commands.length; i++) {
            const cmd = commands[i];
            const num = String(start + i + 1).padStart(3);
            this.sendConsole(entity, `${num}: ${cmd.name} ${cmd.description}`);
        }

        // Footer
        this.sendConsole(entity, `----- ${this.getLang(entity, 'HELP_ENTRIES', start + 1, end, totalCommands)} -----`);

        if (end < totalCommands) {
            this.sendConsole(entity, `----- ${this.getLang(entity, 'HELP_USE_MORE', 'amx_help', end + 1)} -----`);
        } else {
            this.sendConsole(entity, `----- ${this.getLang(entity, 'HELP_USE_BEGIN', 'amx_help')} -----`);
        }
    }

    /**
     * Display join info message
     */
    private displayInfo(entity: nodemod.Entity) {
        // Check if entity is still valid/connected
        if (!entity || !entity.netname) return;

        // Send help hint
        this.sendChat(entity, this.getLang(entity, 'TYPE_HELP'));

        // Get next map
        const nextMap = this.amxNextmap.value || '';
        const timeLimit = this.mpTimelimit.float || 0;

        if (timeLimit > 0) {
            const timeLeft = this.getTimeLeft();

            if (timeLeft > 0) {
                const minutes = Math.floor(timeLeft / 60);
                const seconds = String(timeLeft % 60).padStart(2, '0');
                this.sendChat(entity, this.getLang(entity, 'TIME_INFO_1', minutes, seconds, nextMap));
            } else {
                this.sendChat(entity, this.getLang(entity, 'TIME_INFO_2', nextMap));
            }
        }
    }

    /**
     * Get time left in seconds
     * Uses shared utility function for consistency across plugins
     */
    private getTimeLeft(): number {
        return utils.getTimeLeft();
    }
}



export default AdminHelp;
