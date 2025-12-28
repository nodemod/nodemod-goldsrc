// Plugin Menu Plugin
// Converted from AMX Mod X pluginmenu.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import * as utils from './utils';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader, LoadedPlugin } from './pluginloader';
import {helpRegistry, cvarRegistry } from './helpregistry';
import {
    ADMIN_CVAR, ADMIN_MENU, ADMIN_RCON, ADMIN_PASSWORD, ADMIN_USER, ADMIN_ALL, ADMIN_ADMIN
} from './constants';

const cvar = nodemodCore.cvar;

// Per-player state
interface PlayerState {
    currentCvar: string;
    currentPlugin: string;
    currentPage: number;
    currentCommand: string;
    explicitPlugin: string;
}

class PluginMenu extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Plugin Menu',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Plugin cvar and command menus'
    };

    // Per-player state
    private playerState: Map<number, PlayerState> = new Map();

    constructor(pluginName: string) {
        super(pluginName);

        // Register commands
        this.registerCommands();

        // Add to menu front after a short delay
        setTimeout(() => this.addToMenuFront(), 100);
    }

    private getPlayerState(entity: nodemod.Entity): PlayerState {
        const id = nodemod.eng.indexOfEdict(entity);
        if (!this.playerState.has(id)) {
            this.playerState.set(id, {
                currentCvar: '',
                currentPlugin: '',
                currentPage: 0,
                currentCommand: '',
                explicitPlugin: ''
            });
        }
        return this.playerState.get(id)!;
    }

    private registerCommands() {
        // Plugin Cvar Menu
        this.registerCommand('amx_plugincvarmenu', ADMIN_CVAR, '[plugin] - displays plugin cvar menu', (entity, args) => {
            if (entity) this.cmdCvarMenu(entity, args);
        });

        // Plugin Command Menu
        this.registerCommand('amx_plugincmdmenu', ADMIN_MENU, '[plugin] - displays plugin command menu', (entity, args) => {
            if (entity) this.cmdCommandMenu(entity, args);
        });

        // Messagemode handlers for cvar and command input
        this.registerCommand('amx_setcvarvalue', 0, '', (entity, args) => {
            if (entity) this.handleCvarInput(entity, args);
        });

        this.registerCommand('amx_execcommand', 0, '', (entity, args) => {
            if (entity) this.handleCommandInput(entity, args);
        });
    }

    private handleCvarInput(entity: nodemod.Entity, args: string[]) {
        const state = this.getPlayerState(entity);

        if (!state.currentCvar) {
            return;
        }

        const input = args.join(' ').trim();

        // Check for cancel
        if (input.toLowerCase() === '!cancel') {
            this.sendChat(entity, this.getLang(entity, 'CVAR_NOT_CHANGED'));
            state.currentCvar = '';
            setTimeout(() => this.displayCvarMenu(entity, state.currentPlugin), 0);
            return;
        }

        // Set the cvar value
        const cvarName = state.currentCvar;
        cvar.setString(cvarName, input);

        const adminName = entity.netname || 'Admin';
        this.sendChat(entity, this.getLang(entity, 'CVAR_CHANGED', cvarName, input));
        this.showActivity(entity, this.getLang(null, 'SET_CVAR_TO', adminName, cvarName, input));

        // Dual logging: AMXX internal format + HL engine format
        const userId = nodemod.eng.getPlayerUserId(entity);
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        this.logAmx(`Cmd: "${adminName}<${userId}><${authId}><>" set cvar (name "${cvarName}") (value "${input}")`);
        this.logMessage(entity, 'amx_cvar', { cvar: cvarName, value: input });

        state.currentCvar = '';
        setTimeout(() => this.displayCvarMenu(entity, state.currentPlugin), 0);
    }

    private handleCommandInput(entity: nodemod.Entity, args: string[]) {
        const state = this.getPlayerState(entity);

        if (!state.currentCommand) {
            return;
        }

        const input = args.join(' ').trim();

        // Check for cancel
        if (input.toLowerCase() === '!cancel') {
            this.sendChat(entity, this.getLang(entity, 'CMD_NOT_EXECUTED'));
            state.currentCommand = '';
            setTimeout(() => this.displayCommandMenu(entity, state.currentPlugin), 0);
            return;
        }

        // Execute the command with parameters
        const fullCommand = `${state.currentCommand} ${input}`;
        nodemod.eng.clientCommand(entity, `${fullCommand}\n`);

        this.sendChat(entity, this.getLang(entity, 'CMD_EXECUTED', state.currentCommand));

        state.currentCommand = '';
        setTimeout(() => this.displayCommandMenu(entity, state.currentPlugin), 0);
    }

    private addToMenuFront() {
        try {
            // Try to add to menufront if available
            const menufront = require('./menufront').default;
            if (menufront && menufront.addMenuLang) {
                menufront.addMenuLang('PLUGIN_CVARS', 'amx_plugincvarmenu', ADMIN_CVAR, 'pluginmenu.amxx');
                menufront.addMenuLang('PLUGIN_CMDS', 'amx_plugincmdmenu', ADMIN_MENU, 'pluginmenu.amxx');
            }
        } catch (e) {
            // Menufront not available
        }
    }

    // ==================== CVAR MENU ====================

    private cmdCvarMenu(entity: nodemod.Entity, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CVAR)) return;

        const state = this.getPlayerState(entity);

        // Check if a specific plugin was provided
        if (args.length > 0) {
            const pluginName = args[0];
            const loadedPlugin = this.findPlugin(pluginName);

            if (!loadedPlugin) {
                this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', pluginName));
                return;
            }

            state.explicitPlugin = loadedPlugin.pluginName;
            state.currentPlugin = loadedPlugin.pluginName;
            state.currentPage = 0;
            this.displayCvarMenu(entity, loadedPlugin.pluginName);
        } else {
            state.explicitPlugin = '';
            this.displayPluginCvarList(entity);
        }
    }

    private displayPluginCvarList(entity: nodemod.Entity) {
        const plugins = pluginLoader.getPlugins();
        const items: any[] = [];

        for (const loadedPlugin of plugins) {
            if (loadedPlugin.status !== 'running') continue;

            const cvarCount = this.getPluginCvarCount(loadedPlugin.pluginName);
            if (cvarCount > 0) {
                items.push({
                    name: `${loadedPlugin.plugin.metadata.name} - ${cvarCount}`,
                    handler: (client: nodemod.Entity) => {
                        const state = this.getPlayerState(client);
                        state.currentPlugin = loadedPlugin.pluginName;
                        state.currentPage = 0;
                        setTimeout(() => this.displayCvarMenu(client, loadedPlugin.pluginName), 0);
                    }
                });
            }
        }

        if (items.length === 0) {
            items.push({
                name: this.getLang(entity, 'NO_PLUGINS'),
                disabled: true
            });
        }

        nodemodCore.menu.show({
            entity,
            title: this.getLang(entity, 'PLUGIN_CVAR_MENU'),
            items,
            formatters: utils.coloredMenuFormatters
        });
    }

    private displayCvarMenu(entity: nodemod.Entity, pluginFilename: string) {
        const loadedPlugin = pluginLoader.getPlugin(pluginFilename);
        if (!loadedPlugin) return;

        const cvars = this.getPluginCvars(pluginFilename);
        const items: any[] = [];

        for (const cvarInfo of cvars) {
            const canModify = this.canModifyCvar(entity, cvarInfo.name);
            const displayValue = this.getCvarDisplayValue(entity, cvarInfo.name);

            items.push({
                name: `${cvarInfo.name} = ${displayValue}`,
                disabled: !canModify,
                handler: (client: nodemod.Entity) => {
                    this.promptCvarChange(client, cvarInfo.name);
                }
            });
        }

        if (items.length === 0) {
            items.push({
                name: this.getLang(entity, 'NO_CVARS'),
                disabled: true
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${loadedPlugin.plugin.metadata.name} ${this.getLang(entity, 'CVARS')}:`,
            items,
            onExit: (client: nodemod.Entity) => {
                const state = this.getPlayerState(client);
                if (!state.explicitPlugin) {
                    setTimeout(() => this.displayPluginCvarList(client), 0);
                }
            },
            formatters: utils.coloredMenuFormatters
        });
    }

    private promptCvarChange(entity: nodemod.Entity, cvarName: string) {
        const state = this.getPlayerState(entity);
        state.currentCvar = cvarName;

        const displayValue = this.getCvarDisplayValue(entity, cvarName);

        // Show a menu with current value and options
        const items: any[] = [
            {
                name: this.getLang(entity, 'CURRENT_VALUE', displayValue),
                disabled: true
            },
            {
                name: this.getLang(entity, 'ENTER_NEW_VALUE'),
                handler: (client: nodemod.Entity) => {
                    this.sendChat(client, this.getLang(client, 'TYPE_NEW_VALUE', cvarName));
                    // Use messagemode to get input
                    nodemod.eng.clientCommand(client, 'messagemode amx_setcvarvalue\n');
                }
            },
            {
                name: this.getLang(entity, 'TOGGLE_VALUE'),
                handler: (client: nodemod.Entity) => {
                    this.toggleCvar(client, cvarName);
                }
            }
        ];

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'MODIFY_CVAR')}: ${cvarName}`,
            items,
            onExit: (client: nodemod.Entity) => {
                setTimeout(() => this.displayCvarMenu(client, state.currentPlugin), 0);
            },
            formatters: utils.coloredMenuFormatters
        });
    }

    private toggleCvar(entity: nodemod.Entity, cvarName: string) {
        const currentValue = cvar.getInt(cvarName) || 0;
        const newValue = currentValue === 0 ? 1 : 0;

        cvar.setInt(cvarName, newValue);

        const adminName = entity.netname || 'Admin';
        this.sendChat(entity, this.getLang(entity, 'CVAR_CHANGED', cvarName, newValue.toString()));
        this.showActivity(entity, this.getLang(null, 'SET_CVAR_TO', adminName, cvarName, newValue.toString()));

        // Dual logging: AMXX internal format + HL engine format
        const userId = nodemod.eng.getPlayerUserId(entity);
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        this.logAmx(`Cmd: "${adminName}<${userId}><${authId}><>" set cvar (name "${cvarName}") (value "${newValue}")`);
        this.logMessage(entity, 'amx_cvar', { cvar: cvarName, value: newValue.toString() });

        // Return to cvar menu
        const state = this.getPlayerState(entity);
        setTimeout(() => this.displayCvarMenu(entity, state.currentPlugin), 0);
    }

    // Protected CVARs that should have their values masked
    private static readonly PROTECTED_CVARS = [
        'sv_password',
        'rcon_password',
        'amx_password_field',
        'amx_sql_pass',
        'mysql_pass'
    ];

    /**
     * Check if a CVAR's value should be masked for this user
     */
    private isCvarProtected(entity: nodemod.Entity, cvarName: string): boolean {
        const lowerName = cvarName.toLowerCase();

        // Check if it's a protected cvar
        if (!PluginMenu.PROTECTED_CVARS.some(p => lowerName.includes(p))) {
            return false;
        }

        // RCON users can see all values
        if (adminSystem.hasAccess(entity, ADMIN_RCON)) {
            return false;
        }

        // Password users can see sv_password
        if (lowerName === 'sv_password' && adminSystem.hasAccess(entity, ADMIN_PASSWORD)) {
            return false;
        }

        // Otherwise, mask it
        return true;
    }

    /**
     * Get the display value for a CVAR, masking protected ones
     */
    private getCvarDisplayValue(entity: nodemod.Entity, cvarName: string): string {
        if (this.isCvarProtected(entity, cvarName)) {
            return '*** PROTECTED ***';
        }

        const value = cvar.getString(cvarName) || '';
        return value.length > 20 ? value.substring(0, 17) + '...' : value;
    }

    private canModifyCvar(entity: nodemod.Entity, cvarName: string): boolean {
        // RCON users can modify anything
        if (adminSystem.hasAccess(entity, ADMIN_RCON)) {
            return true;
        }

        // Password users can modify sv_password
        if (cvarName.toLowerCase() === 'sv_password' && adminSystem.hasAccess(entity, ADMIN_PASSWORD)) {
            return true;
        }

        // Protected cvars need RCON access to modify
        if (this.isCvarProtected(entity, cvarName)) {
            return false;
        }

        // For now, allow if user has ADMIN_CVAR access
        return true;
    }

    private getPluginCvarCount(pluginFilename: string): number {
        return cvarRegistry.getCvarsByPlugin(pluginFilename).length;
    }

    private getPluginCvars(pluginFilename: string): { name: string; plugin: string }[] {
        const cvars = cvarRegistry.getCvarsByPlugin(pluginFilename);
        return cvars.map(name => ({ name, plugin: pluginFilename }));
    }

    // ==================== COMMAND MENU ====================

    private cmdCommandMenu(entity: nodemod.Entity, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_MENU)) return;

        const state = this.getPlayerState(entity);

        // Check if a specific plugin was provided
        if (args.length > 0) {
            const pluginName = args[0];
            const loadedPlugin = this.findPlugin(pluginName);

            if (!loadedPlugin) {
                this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', pluginName));
                return;
            }

            state.explicitPlugin = loadedPlugin.pluginName;
            state.currentPlugin = loadedPlugin.pluginName;
            state.currentPage = 0;
            this.displayCommandMenu(entity, loadedPlugin.pluginName);
        } else {
            state.explicitPlugin = '';
            this.displayPluginCommandList(entity);
        }
    }

    private displayPluginCommandList(entity: nodemod.Entity) {
        const plugins = pluginLoader.getPlugins();
        const items: any[] = [];

        for (const loadedPlugin of plugins) {
            if (loadedPlugin.status !== 'running') continue;

            const cmdCount = this.getPluginCommandCount(loadedPlugin.pluginName);
            if (cmdCount > 0) {
                items.push({
                    name: `${loadedPlugin.plugin.metadata.name} - ${cmdCount}`,
                    handler: (client: nodemod.Entity) => {
                        const state = this.getPlayerState(client);
                        state.currentPlugin = loadedPlugin.pluginName;
                        state.currentPage = 0;
                        setTimeout(() => this.displayCommandMenu(client, loadedPlugin.pluginName), 0);
                    }
                });
            }
        }

        if (items.length === 0) {
            items.push({
                name: this.getLang(entity, 'NO_PLUGINS'),
                disabled: true
            });
        }

        nodemodCore.menu.show({
            entity,
            title: this.getLang(entity, 'PLUGIN_CMD_MENU'),
            items,
            formatters: utils.coloredMenuFormatters
        });
    }

    private displayCommandMenu(entity: nodemod.Entity, pluginFilename: string) {
        const loadedPlugin = pluginLoader.getPlugin(pluginFilename);
        if (!loadedPlugin) return;

        const commands = this.getPluginCommands(entity, pluginFilename);
        const items: any[] = [];

        for (const cmd of commands) {
            const hasAccess = this.canExecuteCommand(entity, cmd.access);

            items.push({
                name: cmd.command,
                disabled: !hasAccess,
                handler: (client: nodemod.Entity) => {
                    this.displayCommandOptions(client, cmd.command, cmd.description);
                }
            });
        }

        if (items.length === 0) {
            items.push({
                name: this.getLang(entity, 'NO_CMDS'),
                disabled: true
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${loadedPlugin.plugin.metadata.name} ${this.getLang(entity, 'COMMANDS')}:`,
            items,
            onExit: (client: nodemod.Entity) => {
                const state = this.getPlayerState(client);
                if (!state.explicitPlugin) {
                    setTimeout(() => this.displayPluginCommandList(client), 0);
                }
            },
            formatters: utils.coloredMenuFormatters
        });
    }

    private displayCommandOptions(entity: nodemod.Entity, command: string, description: string) {
        const state = this.getPlayerState(entity);
        state.currentCommand = command;

        const items: any[] = [
            {
                name: this.getLang(entity, 'EXEC_WITH_PARAMS'),
                handler: (client: nodemod.Entity) => {
                    this.sendChat(client, this.getLang(client, 'TYPE_PARAMS', command));
                    nodemod.eng.clientCommand(client, 'messagemode amx_execcommand\n');
                }
            },
            {
                name: this.getLang(entity, 'EXEC_NO_PARAMS'),
                handler: (client: nodemod.Entity) => {
                    nodemod.eng.clientCommand(client, `${command}\n`);
                    this.sendChat(client, this.getLang(client, 'CMD_EXECUTED', command));
                    setTimeout(() => this.displayCommandMenu(client, state.currentPlugin), 0);
                }
            }
        ];

        let title = command;
        if (description) {
            title = `${command}\n${description}`;
        }

        nodemodCore.menu.show({
            entity,
            title,
            items,
            onExit: (client: nodemod.Entity) => {
                setTimeout(() => this.displayCommandMenu(client, state.currentPlugin), 0);
            },
            formatters: utils.coloredMenuFormatters
        });
    }

    private canExecuteCommand(entity: nodemod.Entity, access: number): boolean {
        if (access === ADMIN_USER || access === ADMIN_ALL || access === 0) {
            return true;
        }
        if (access === ADMIN_ADMIN && adminSystem.hasAccess(entity, ADMIN_ADMIN)) {
            return true;
        }
        return adminSystem.hasAccess(entity, access);
    }

    private getPluginCommandCount(pluginFilename: string): number {
        return helpRegistry.getCommandsByPlugin(pluginFilename).length;
    }

    private getPluginCommands(entity: nodemod.Entity | null, pluginFilename: string): { command: string; access: number; description: string; plugin: string }[] {
        const commands = helpRegistry.getCommandsByPlugin(pluginFilename);

        // Filter out say commands and map to expected format
        return commands
            .filter(cmd => !cmd.name.startsWith('say'))
            .map(cmd => ({
                command: cmd.name,
                access: cmd.flags,
                description: cmd.description,
                plugin: pluginFilename
            }));
    }

    private findPlugin(name: string): LoadedPlugin | undefined {
        const plugins = pluginLoader.getPlugins();

        // Try exact match on filename
        for (const loadedPlugin of plugins) {
            if (loadedPlugin.pluginName.toLowerCase() === name.toLowerCase()) {
                return loadedPlugin;
            }
        }

        // Try match on name
        for (const loadedPlugin of plugins) {
            if (loadedPlugin.plugin.metadata.name.toLowerCase() === name.toLowerCase()) {
                return loadedPlugin;
            }
        }

        // Try partial match
        for (const loadedPlugin of plugins) {
            if (loadedPlugin.pluginName.toLowerCase().includes(name.toLowerCase()) ||
                loadedPlugin.plugin.metadata.name.toLowerCase().includes(name.toLowerCase())) {
                return loadedPlugin;
            }
        }

        return undefined;
    }
}



export default PluginMenu;
