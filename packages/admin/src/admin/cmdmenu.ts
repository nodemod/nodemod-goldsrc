// Commands Menu Plugin
// Converted from AMX Mod X cmdmenu.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_MENU, ADMIN_CVAR, readFlags } from './constants';

import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

// Command flags
const CMD_FLAG_SERVER = 1;     // Execute as server command
const CMD_FLAG_CLIENT = 2;     // Execute as client command
const CMD_FLAG_BROADCAST = 4;  // Execute as client command to all
const CMD_FLAG_KEEPMENU = 8;   // Keep menu open after execution

// Menu layer configuration
interface MenuLayer {
    name: string;      // Localization key
    command: string;   // Console command
    configFile: string; // Config file name
    helpText: string;  // Help text
}

const MENU_LAYERS: MenuLayer[] = [
    { name: 'CMD_MENU', command: 'amx_cmdmenu', configFile: 'cmds.ini', helpText: '- displays commands menu' },
    { name: 'CONF_MENU', command: 'amx_cfgmenu', configFile: 'configs.ini', helpText: '- displays configs menu' },
    { name: 'SPE_MENU', command: 'amx_speechmenu', configFile: 'speech.ini', helpText: '- displays speech menu' }
];

// Command entry from config
interface CommandEntry {
    name: string;
    cmd: string;
    flags: number;      // CMD_FLAG_* flags
    access: number;     // ADMIN_* access flags
    layer: number;      // Which menu layer this belongs to
}

// CVAR entry from config
interface CvarEntry {
    name: string;
    values: string[];
    access: number;
}

// Player menu state
interface PlayerMenuState {
    layer: number;
    selections: number[];  // Indices into commands/cvars array for accessible items
    isCvarMenu: boolean;
}

class CommandsMenu extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Commands Menu',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Configurable commands and cvars menus'
    };

    private commands: CommandEntry[] = [];
    private commandCounts: number[] = [0, 0, 0];  // Count per layer
    private cvars: CvarEntry[] = [];
    private playerStates: Map<number, PlayerMenuState> = new Map();

    constructor(pluginName: string) {
        super(pluginName);

        // Also load common dictionary for shared strings
        this.loadCommonDictionary();

        // Load config files
        this.loadConfigs();

        // Register commands
        this.registerCommands();
    }

    private loadCommonDictionary() {
        // Load common.txt for shared strings like BACK, EXIT, MORE
        // BasePlugin already loaded cmdmenu dictionary
        const localization = require('./localization').default;
        localization.loadDictionary('common');
    }

    private loadConfigs() {
        const configsDir = utils.getConfigsDir();

        // Load command menus
        for (let layer = 0; layer < MENU_LAYERS.length; layer++) {
            const configPath = path.join(configsDir, MENU_LAYERS[layer].configFile);
            this.loadCommandSettings(configPath, layer);
        }

        // Precache sounds from speech.ini
        this.precacheSpeechSounds();

        // Load cvars menu
        const cvarsPath = path.join(configsDir, 'cvars.ini');
        this.loadCvarSettings(cvarsPath);
    }

    /**
     * Precache sounds from speech.ini entries.
     * Parses commands like "spk 'vox/sound'" and "mp3 play 'path/file.mp3'"
     * Matches original AMX behavior: checks file_exists before precaching.
     */
    private precacheSpeechSounds() {
        // Find commands from speech.ini (layer 2)
        const speechLayer = 2;
        const speechCommands = this.commands.filter(cmd => cmd.layer === speechLayer);

        // Game directory (parent of plugins dir)
        const gameDir = path.join(process.cwd(), '..');

        let precachedCount = 0;

        for (const cmd of speechCommands) {
            const soundPath = this.extractSoundPath(cmd.cmd);
            if (soundPath) {
                const isMp3 = soundPath.toLowerCase().endsWith('.mp3');

                // Check if it's a custom sound (not VOX/FVOX/default HL sounds)
                const isVoxSound = soundPath.toLowerCase().startsWith('vox/') ||
                                   soundPath.toLowerCase().startsWith('fvox/') ||
                                   soundPath.toLowerCase().startsWith('barney/') ||
                                   soundPath.toLowerCase().startsWith('hgrunt/');

                if (isMp3 || !isVoxSound) {
                    // Build full path for file_exists check
                    // wav files are in $moddir/sound/, mp3 files are in $moddir/
                    let fullPath: string;
                    if (isMp3) {
                        fullPath = path.join(gameDir, soundPath);
                    } else {
                        fullPath = path.join(gameDir, 'sound', soundPath);
                    }

                    // Only precache if file exists (matches original AMX behavior)
                    if (fs.existsSync(fullPath)) {
                        if (isMp3) {
                            nodemodCore.resource.precacheGeneric(soundPath);
                        } else {
                            nodemodCore.resource.precacheSound(soundPath);
                        }
                        precachedCount++;
                    }
                }
            }
        }

    }

    /**
     * Extract sound path from a command string.
     * Handles formats like:
     * - spk 'vox/sound path'
     * - mp3 play 'path/file.mp3'
     * - mp3 loop 'path/file.mp3'
     */
    private extractSoundPath(cmd: string): string | null {
        // Match spk 'path' or spk "path"
        let match = cmd.match(/spk\s+['"](.*?)['"]/i);
        if (match) {
            let soundPath = match[1].replace(/\\'/g, '');
            // Strip leading slash if present
            if (soundPath.startsWith('/')) {
                soundPath = soundPath.substring(1);
            }
            return soundPath;
        }

        // Match mp3 play 'path' or mp3 loop 'path'
        match = cmd.match(/mp3\s+(?:play|loop)\s+['"](.*?)['"]/i);
        if (match) {
            let soundPath = match[1].replace(/\\'/g, '');
            if (soundPath.startsWith('/')) {
                soundPath = soundPath.substring(1);
            }
            // Ensure .mp3 extension
            if (!soundPath.toLowerCase().endsWith('.mp3')) {
                soundPath += '.mp3';
            }
            return soundPath;
        }

        return null;
    }

    private loadCommandSettings(filePath: string, layer: number) {
        if (!fs.existsSync(filePath)) {
            return;
        }

        try {
            const content = fs.readFileSync(filePath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';') || trimmed.startsWith('//')) {
                    continue;
                }

                // Parse: "Name" "command" "flags" "access"
                const parts = this.parseQuotedLine(trimmed);
                if (parts.length >= 4) {
                    const entry: CommandEntry = {
                        name: parts[0],
                        cmd: parts[1].replace(/\\'/g, '"'),  // Replace \' with " (AMX escape sequence)
                        flags: readFlags(parts[2]),
                        access: readFlags(parts[3]),
                        layer
                    };
                    this.commands.push(entry);
                    this.commandCounts[layer]++;
                }
            }

        } catch (e) {
            console.error(`[CmdMenu] Error loading ${filePath}:`, e);
        }
    }

    private loadCvarSettings(filePath: string) {
        if (!fs.existsSync(filePath)) {
            return;
        }

        try {
            const content = fs.readFileSync(filePath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';') || trimmed.startsWith('//')) {
                    continue;
                }

                // Parse: cvarname value1 value2 value3... access_flags
                const parts = this.parseQuotedLine(trimmed);
                if (parts.length >= 3) {
                    // Last part is access flags, rest (except first) are values
                    const accessFlags = parts[parts.length - 1];
                    const values = parts.slice(1, -1).map(v => v.replace(/\\'/g, '"'));

                    if (values.length >= 2) {
                        const entry: CvarEntry = {
                            name: parts[0],
                            values,
                            access: readFlags(accessFlags)
                        };
                        this.cvars.push(entry);
                    }
                }
            }

        } catch (e) {
            console.error(`[CmdMenu] Error loading ${filePath}:`, e);
        }
    }

    private parseQuotedLine(line: string): string[] {
        const parts: string[] = [];
        let current = '';
        let inQuotes = false;

        for (let i = 0; i < line.length; i++) {
            const char = line[i];
            if (char === '"') {
                inQuotes = !inQuotes;
            } else if ((char === ' ' || char === '\t') && !inQuotes) {
                if (current) {
                    parts.push(current);
                    current = '';
                }
            } else {
                current += char;
            }
        }
        if (current) {
            parts.push(current);
        }
        return parts;
    }

    private registerCommands() {
        // Register command menu commands
        for (let layer = 0; layer < MENU_LAYERS.length; layer++) {
            const menuLayer = MENU_LAYERS[layer];
            this.registerCommand(menuLayer.command, ADMIN_MENU, menuLayer.helpText, (entity, args) => {
                this.cmdCommandMenu(entity, layer);
            });
        }

        // Register cvar menu command
        this.registerCommand('amx_cvarmenu', ADMIN_CVAR, '- displays cvars menu', (entity, args) => {
            this.cmdCvarMenu(entity);
        });
    }

    // Use getLangWithFallback from BasePlugin for cmdmenu/common dictionary lookup

    private getPlayerState(entity: nodemod.Entity): PlayerMenuState {
        const index = nodemod.eng.indexOfEdict(entity);
        let state = this.playerStates.get(index);
        if (!state) {
            state = {
                layer: 0,
                selections: [],
                isCvarMenu: false
            };
            this.playerStates.set(index, state);
        }
        return state;
    }

    // ========================================================================
    // Command Menu
    // ========================================================================

    private cmdCommandMenu(entity: nodemod.Entity | null, layer: number) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_MENU)) return;

        const state = this.getPlayerState(entity);
        state.layer = layer;
        state.isCvarMenu = false;
        state.selections = [];

        // Build list of accessible commands for this layer
        const startIndex = this.getLayerStartIndex(layer);
        const count = this.commandCounts[layer];

        for (let i = 0; i < count; i++) {
            const cmd = this.commands[startIndex + i];
            if (adminSystem.hasAccess(entity, cmd.access)) {
                state.selections.push(startIndex + i);
            }
        }

        this.displayCommandMenu(entity);
    }

    private getLayerStartIndex(layer: number): number {
        let start = 0;
        for (let i = 0; i < layer; i++) {
            start += this.commandCounts[i];
        }
        return start;
    }

    private displayCommandMenu(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);
        const menuTitle = this.getLangWithFallback(entity, MENU_LAYERS[state.layer].name);

        // Build all items - core menu handles pagination
        const items: any[] = [];
        for (const cmdIndex of state.selections) {
            const cmd = this.commands[cmdIndex];

            // Check if it's a separator (command starts with -)
            if (cmd.cmd.startsWith('-')) {
                items.push({
                    name: cmd.name,
                    disabled: true
                });
            } else {
                items.push({
                    name: cmd.name,
                    handler: (client: nodemod.Entity) => {
                        this.executeCommand(client, cmdIndex);
                    }
                });
            }
        }

        nodemodCore.menu.show({
            title: menuTitle,
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private executeCommand(entity: nodemod.Entity, cmdIndex: number) {
        const cmd = this.commands[cmdIndex];
        const state = this.getPlayerState(entity);

        if (cmd.flags & CMD_FLAG_SERVER) {
            // Execute as server command
            nodemod.eng.serverCommand(`${cmd.cmd}\n`);
        } else if (cmd.flags & CMD_FLAG_CLIENT) {
            // Execute as client command
            nodemod.eng.clientCommand(entity, `${cmd.cmd}\n`);
        } else if (cmd.flags & CMD_FLAG_BROADCAST) {
            // Execute as client command to all players
            for (const player of adminSystem.getPlayers()) {
                nodemod.eng.clientCommand(player, `${cmd.cmd}\n`);
            }
        }

        // Keep menu open if flag is set
        if (cmd.flags & CMD_FLAG_KEEPMENU) {
            // Delay to allow handler cleanup to complete first
            setTimeout(() => {
                this.displayCommandMenu(entity);
            }, 0);
        }
    }

    // ========================================================================
    // CVAR Menu
    // ========================================================================

    private cmdCvarMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_CVAR)) return;

        const state = this.getPlayerState(entity);
        state.isCvarMenu = true;
        state.selections = [];

        // Build list of accessible cvars
        for (let i = 0; i < this.cvars.length; i++) {
            if (adminSystem.hasAccess(entity, this.cvars[i].access)) {
                state.selections.push(i);
            }
        }

        this.displayCvarMenu(entity);
    }

    private displayCvarMenu(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);

        // Build all items - core menu handles pagination
        const items: any[] = [];
        for (const cvarIndex of state.selections) {
            const cvarEntry = this.cvars[cvarIndex];
            const currentValue = cvar.getString(cvarEntry.name) || '';

            items.push({
                name: `${cvarEntry.name}: ${currentValue}`,
                handler: (client: nodemod.Entity) => {
                    this.cycleCvar(client, cvarIndex);
                }
            });
        }

        nodemodCore.menu.show({
            title: this.getLangWithFallback(entity, 'CVAR_MENU'),
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private cycleCvar(entity: nodemod.Entity, cvarIndex: number) {
        const cvarEntry = this.cvars[cvarIndex];
        const currentValue = cvar.getString(cvarEntry.name) || '';

        // Find current value index and cycle to next
        let currentIdx = cvarEntry.values.indexOf(currentValue);
        if (currentIdx === -1) {
            currentIdx = 0;
        } else {
            currentIdx = (currentIdx + 1) % cvarEntry.values.length;
        }

        // Set new value
        cvar.setString(cvarEntry.name, cvarEntry.values[currentIdx]);

        // Refresh menu after handler cleanup completes
        setTimeout(() => {
            this.displayCvarMenu(entity);
        }, 0);
    }
}



export default CommandsMenu;
