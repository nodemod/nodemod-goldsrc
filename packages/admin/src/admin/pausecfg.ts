// Pause Plugins Plugin
// Converted from AMX Mod X pausecfg.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import * as utils from './utils';
import { ADMIN_CFG } from './constants';

import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader, LoadedPlugin } from './pluginloader';

const cvar = nodemodCore.cvar;

// System plugins that cannot be paused
const SYSTEM_PLUGINS = [
    'Admin Base',
    'Admin Base (SQL)',
    'Pause Plugins',
    'TimeLeft',
    'NextMap',
    'Slots Reservation'
];

class PauseCfg extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Pause Plugins',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Pause/unpause plugin management'
    };

    // Additional system plugin names (added via amx_pausecfg add)
    private systemPlugins: Set<string> = new Set(SYSTEM_PLUGINS);

    // Paused plugins (filename -> true)
    private pausedPlugins: Set<string> = new Set();

    // Stopped plugins (permanently stopped until restart)
    private stoppedPlugins: Set<string> = new Set();

    // Track if settings modified
    private modified: boolean = false;

    constructor(pluginName: string) {
        super(pluginName);

        // Load common dictionary
        const localization = require('./localization').default;
        localization.loadDictionary('common');
        localization.loadDictionary('admincmd');

        // Load saved settings
        this.loadSettings();

        // Register commands
        this.registerCommands();

    }

    private getSaveFile(): string {
        return path.join(utils.getConfigsDir(), 'pausecfg.ini');
    }

    private registerCommands() {
        // Main command with subcommands
        this.registerCommand('amx_pausecfg', ADMIN_CFG, '<command> - pause/unpause plugin management', (entity, args) => {
            this.cmdPauseCfg(entity, args);
        });

        // Menu command
        this.registerCommand('amx_pausecfgmenu', ADMIN_CFG, '- pause/unpause plugins with menu', (entity, args) => {
            this.cmdMenu(entity);
        });

        // Direct on/off commands
        this.registerCommand('amx_off', ADMIN_CFG, '- pause all non-system plugins', (entity, args) => {
            this.pausePlugins(entity);
        });

        this.registerCommand('amx_on', ADMIN_CFG, '- unpause all paused plugins', (entity, args) => {
            this.unpausePlugins(entity);
        });
    }

    // ========================================================================
    // Plugin Status Helpers
    // ========================================================================

    private isSystemPlugin(pluginName: string): boolean {
        return this.systemPlugins.has(pluginName);
    }

    private isPaused(filename: string): boolean {
        return this.pausedPlugins.has(filename);
    }

    /**
     * Get plugin status with all SMA-compatible status codes:
     * - 'running' (r) - Plugin is running normally
     * - 'debug' (d) - Plugin is running in debug mode
     * - 'stopped' (s) - Plugin has been stopped
     * - 'paused' (p) - Plugin is paused
     * - 'bad_load' (b) - Plugin failed to load properly
     * - 'locked' - System plugin that cannot be paused
     */
    private getPluginStatus(plugin: LoadedPlugin): string {
        // Check for system/locked plugins first
        if (this.isSystemPlugin(plugin.plugin.metadata.name)) {
            return 'locked';
        }
        // Check for error/bad load status
        if (plugin.status === 'error') return 'bad_load';
        // Check for stopped status
        if (plugin.status === 'stopped') return 'stopped';
        if (this.stoppedPlugins.has(plugin.pluginName)) return 'stopped';
        // Check for paused status
        if (this.isPaused(plugin.pluginName)) return 'paused';
        // Check for debug mode (if plugin has debug flag)
        if ((plugin as any).debug) return 'debug';
        // Default to running
        return 'running';
    }

    /**
     * Get single-character status code for list display (matches SMA format)
     */
    private getStatusCode(status: string): string {
        switch (status) {
            case 'running': return 'r';
            case 'debug': return 'd';
            case 'stopped': return 's';
            case 'paused': return 'p';
            case 'bad_load': return 'b';
            case 'locked': return 'L';
            default: return '?';
        }
    }

    private getStatusDisplay(entity: nodemod.Entity | null, status: string): string {
        switch (status) {
            case 'running': return this.getLangWithFallback(entity, 'ON');
            case 'debug': return 'DEBUG';
            case 'paused': return this.getLangWithFallback(entity, 'OFF');
            case 'stopped': return this.getLang(entity, 'STOPPED');
            case 'bad_load': return 'BAD LOAD';
            case 'locked': return this.getLang(entity, 'LOCKED');
            default: return '?';
        }
    }

    // ========================================================================
    // Pause/Unpause Functions
    // ========================================================================

    private pausePlugin(filename: string): boolean {
        const plugin = pluginLoader.getPlugin(filename);
        if (!plugin) return false;

        if (this.isSystemPlugin(plugin.plugin.metadata.name)) {
            return false;
        }

        if (this.isPaused(filename)) {
            return false;
        }

        this.pausedPlugins.add(filename);
        this.modified = true;
        return true;
    }

    private unpausePlugin(filename: string): boolean {
        if (!this.isPaused(filename)) {
            return false;
        }

        this.pausedPlugins.delete(filename);
        this.modified = true;
        return true;
    }

    private pausePlugins(entity: nodemod.Entity | null) {
        let count = 0;

        for (const plugin of pluginLoader.getPlugins()) {
            if (this.isSystemPlugin(plugin.plugin.metadata.name)) continue;
            if (plugin.status !== 'running') continue;
            if (this.isPaused(plugin.pluginName)) continue;

            if (this.pausePlugin(plugin.pluginName)) {
                count++;
            }
        }

        const msg = count === 1
            ? this.getLang(entity, 'PAUSED_PLUGIN', count)
            : this.getLang(entity, 'PAUSED_PLUGINS', count);
        this.sendConsole(entity, msg);
    }

    private unpausePlugins(entity: nodemod.Entity | null) {
        let count = 0;

        for (const plugin of pluginLoader.getPlugins()) {
            if (this.isSystemPlugin(plugin.plugin.metadata.name)) continue;
            if (!this.isPaused(plugin.pluginName)) continue;

            if (this.unpausePlugin(plugin.pluginName)) {
                count++;
            }
        }

        const msg = count === 1
            ? this.getLang(entity, 'UNPAUSED_PLUGIN', count)
            : this.getLang(entity, 'UNPAUSED_PLUGINS', count);
        this.sendConsole(entity, msg);
    }

    // ========================================================================
    // Settings Persistence
    // ========================================================================

    private saveSettings(): boolean {
        const filename = this.getSaveFile();

        try {
            let content = ';Generated by Pause Plugins Plugin. Do not modify!\n';
            content += ';Title Filename\n';

            for (const plugin of pluginLoader.getPlugins()) {
                if (this.isPaused(plugin.pluginName)) {
                    content += `"${plugin.plugin.metadata.name}" ;${plugin.pluginName}\n`;
                }
            }

            fs.writeFileSync(filename, content);
            this.modified = false;
            return true;
        } catch (e) {
            console.error('[PauseCfg] Error saving settings:', e);
            return false;
        }
    }

    private loadSettings(): boolean {
        const filename = this.getSaveFile();

        if (!fs.existsSync(filename)) {
            return false;
        }

        try {
            const content = fs.readFileSync(filename, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';')) continue;

                // Parse "Plugin Title" ;filename.amxx
                const match = trimmed.match(/^"([^"]+)"/);
                if (match) {
                    const pluginTitle = match[1];
                    // Find plugin by title
                    for (const plugin of pluginLoader.getPlugins()) {
                        if (plugin.plugin.metadata.name === pluginTitle) {
                            this.pausedPlugins.add(plugin.pluginName);
                            break;
                        }
                    }
                }
            }

            return true;
        } catch (e) {
            console.error('[PauseCfg] Error loading settings:', e);
            return false;
        }
    }

    private clearSettings(): boolean {
        const filename = this.getSaveFile();

        try {
            if (fs.existsSync(filename)) {
                fs.unlinkSync(filename);
                return true;
            }
            return false;
        } catch (e) {
            return false;
        }
    }

    // ========================================================================
    // Menu
    // ========================================================================

    private cmdMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        this.displayMenu(entity);
    }

    private displayMenu(entity: nodemod.Entity) {
        const plugins = pluginLoader.getPlugins();
        const title = this.getLang(entity, 'PAUSE_UNPAUSE');

        const items: any[] = [];

        // Add plugin items with status-based formatting
        for (const plugin of plugins) {
            const status = this.getPluginStatus(plugin);
            const statusDisplay = this.getStatusDisplay(entity, status);
            const canToggle = status === 'running' || status === 'paused' || status === 'debug';

            // Format name with status indicator
            // Use different prefixes to indicate status visually
            let displayName: string;
            switch (status) {
                case 'running':
                case 'debug':
                    displayName = `${plugin.plugin.metadata.name} [${statusDisplay}]`;
                    break;
                case 'paused':
                    displayName = `${plugin.plugin.metadata.name} [${statusDisplay}]`;
                    break;
                case 'locked':
                    displayName = `* ${plugin.plugin.metadata.name} [${statusDisplay}]`;
                    break;
                case 'stopped':
                case 'bad_load':
                    displayName = `! ${plugin.plugin.metadata.name} [${statusDisplay}]`;
                    break;
                default:
                    displayName = `${plugin.plugin.metadata.name} [${statusDisplay}]`;
            }

            if (canToggle) {
                items.push({
                    name: displayName,
                    handler: (client: nodemod.Entity) => {
                        this.togglePlugin(client, plugin.pluginName);
                    }
                });
            } else {
                items.push({
                    name: displayName,
                    disabled: true
                });
            }
        }

        // Add Clear and Save options at the end
        items.push({
            name: `--- ${this.getLang(entity, 'CLEAR_PAUSED')} ---`,
            handler: (client: nodemod.Entity) => {
                this.menuClearSettings(client);
            }
        });

        items.push({
            name: `--- ${this.getLang(entity, 'SAVE_PAUSED')} ${this.modified ? '*' : ''} ---`,
            handler: (client: nodemod.Entity) => {
                this.menuSaveSettings(client);
            }
        });

        nodemodCore.menu.show({
            title,
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private togglePlugin(entity: nodemod.Entity, filename: string) {
        if (this.isPaused(filename)) {
            this.unpausePlugin(filename);
        } else {
            this.pausePlugin(filename);
        }

        // Refresh menu
        setTimeout(() => {
            this.displayMenu(entity);
        }, 0);
    }

    private menuClearSettings(entity: nodemod.Entity) {
        if (this.clearSettings()) {
            this.pausedPlugins.clear();
            this.sendChat(entity, this.getLang(entity, 'PAUSE_CONF_CLEARED'));
        } else {
            this.sendChat(entity, this.getLang(entity, 'PAUSE_ALR_CLEARED'));
        }

        setTimeout(() => {
            this.displayMenu(entity);
        }, 0);
    }

    private menuSaveSettings(entity: nodemod.Entity) {
        if (this.saveSettings()) {
            this.sendChat(entity, this.getLang(entity, 'PAUSE_CONF_SAVED'));
        } else {
            this.sendChat(entity, this.getLang(entity, 'PAUSE_SAVE_FAILED'));
        }

        setTimeout(() => {
            this.displayMenu(entity);
        }, 0);
    }

    // ========================================================================
    // Console Command
    // ========================================================================

    private cmdPauseCfg(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        if (args.length === 0) {
            this.showUsage(entity);
            return;
        }

        const cmd = args[0].toLowerCase();

        switch (cmd) {
            case 'add':
                if (args.length > 1) {
                    const pluginName = args.slice(1).join(' ');
                    this.addSystemPlugin(entity, pluginName);
                }
                break;

            case 'off':
                this.pausePlugins(entity);
                break;

            case 'on':
                this.unpausePlugins(entity);
                break;

            case 'save':
                if (this.saveSettings()) {
                    this.sendConsole(entity, this.getLang(entity, 'PAUSE_CONF_SAVED'));
                } else {
                    this.sendConsole(entity, this.getLang(entity, 'PAUSE_SAVE_FAILED'));
                }
                break;

            case 'clear':
                if (this.clearSettings()) {
                    this.pausedPlugins.clear();
                    this.sendConsole(entity, this.getLang(entity, 'PAUSE_CONF_CLEARED'));
                } else {
                    this.sendConsole(entity, this.getLang(entity, 'PAUSE_ALR_CLEARED'));
                }
                break;

            case 'pause':
                if (args.length > 1) {
                    this.pauseByFile(entity, args[1]);
                }
                break;

            case 'enable':
                if (args.length > 1) {
                    this.enableByFile(entity, args[1]);
                }
                break;

            case 'stop':
                if (args.length > 1) {
                    this.stopByFile(entity, args[1]);
                }
                break;

            case 'list':
                const start = args.length > 1 ? parseInt(args[1]) || 1 : 1;
                this.listPlugins(entity, start);
                break;

            default:
                this.showUsage(entity);
                break;
        }
    }

    private addSystemPlugin(entity: nodemod.Entity | null, pluginName: string) {
        // Find plugin by name
        for (const plugin of pluginLoader.getPlugins()) {
            if (plugin.plugin.metadata.name.toLowerCase() === pluginName.toLowerCase()) {
                this.systemPlugins.add(plugin.plugin.metadata.name);
                this.sendConsole(entity, this.getLang(entity, 'MARKED_SYSTEM', plugin.plugin.metadata.name));
                return;
            }
        }
        this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', pluginName));
    }

    private pauseByFile(entity: nodemod.Entity | null, filename: string) {
        // Find plugin by partial filename match
        for (const plugin of pluginLoader.getPlugins()) {
            if (plugin.pluginName.toLowerCase().includes(filename.toLowerCase())) {
                if (this.isSystemPlugin(plugin.plugin.metadata.name)) {
                    this.sendConsole(entity, this.getLang(entity, 'CANT_PAUSE_SYSTEM', plugin.plugin.metadata.name));
                    return;
                }

                if (this.pausePlugin(plugin.pluginName)) {
                    this.sendConsole(entity, `${this.getLang(entity, 'PAUSE_PLUGIN_MATCH', plugin.pluginName)} ${this.getLang(entity, 'PAUSED')}`);
                } else {
                    this.sendConsole(entity, this.getLang(entity, 'ALREADY_PAUSED'));
                }
                return;
            }
        }
        this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', filename));
    }

    private enableByFile(entity: nodemod.Entity | null, filename: string) {
        // Find plugin by partial filename match
        for (const plugin of pluginLoader.getPlugins()) {
            if (plugin.pluginName.toLowerCase().includes(filename.toLowerCase())) {
                if (this.unpausePlugin(plugin.pluginName)) {
                    this.sendConsole(entity, `${this.getLang(entity, 'PAUSE_PLUGIN_MATCH', plugin.pluginName)} ${this.getLang(entity, 'UNPAUSED')}`);
                } else {
                    this.sendConsole(entity, this.getLang(entity, 'CANT_UNPAUSE_PLUGIN', plugin.pluginName));
                }
                return;
            }
        }
        this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', filename));
    }

    private stopByFile(entity: nodemod.Entity | null, filename: string) {
        // Find plugin by partial filename match
        for (const plugin of pluginLoader.getPlugins()) {
            if (plugin.pluginName.toLowerCase().includes(filename.toLowerCase())) {
                if (this.isSystemPlugin(plugin.plugin.metadata.name)) {
                    this.sendConsole(entity, this.getLang(entity, 'CANT_STOP_SYSTEM', plugin.plugin.metadata.name));
                    return;
                }

                // Add to stopped plugins (permanent until restart)
                this.stoppedPlugins.add(plugin.pluginName);
                // Also remove from paused if it was there
                this.pausedPlugins.delete(plugin.pluginName);
                this.modified = true;

                this.sendConsole(entity, `${this.getLang(entity, 'PAUSE_PLUGIN_MATCH', plugin.pluginName)} ${this.getLang(entity, 'STOPPED')}`);
                return;
            }
        }
        this.sendConsole(entity, this.getLang(entity, 'PAUSE_COULDNT_FIND', filename));
    }

    private listPlugins(entity: nodemod.Entity | null, start: number) {
        const plugins = pluginLoader.getPlugins();
        const total = plugins.length;

        start = Math.max(1, Math.min(start, total)) - 1;
        const end = Math.min(start + 10, total);

        let running = 0;

        this.sendConsole(entity, '');
        this.sendConsole(entity, `----- ${this.getLang(entity, 'PAUSE_LOADED')} -----`);
        // Status codes: r=running, d=debug, s=stopped, p=paused, b=bad_load, L=locked
        const lName = this.getLang(entity, 'NAME').padEnd(18);
        const lVersion = this.getLang(entity, 'VERSION').padEnd(8);
        const lAuthor = this.getLang(entity, 'AUTHOR').padEnd(17);
        const lFile = this.getLang(entity, 'FILE').padEnd(16);
        const lSt = this.getLang(entity, 'ST');
        this.sendConsole(entity, `       ${lName} ${lVersion} ${lAuthor} ${lFile} ${lSt}`);

        for (let i = start; i < end; i++) {
            const plugin = plugins[i];
            const status = this.getPluginStatus(plugin);
            if (status === 'running' || status === 'debug') running++;

            const name = plugin.plugin.metadata.name.substring(0, 17).padEnd(18);
            const version = plugin.plugin.metadata.version.substring(0, 7).padEnd(8);
            const author = plugin.plugin.metadata.author.substring(0, 16).padEnd(17);
            const file = plugin.pluginName.substring(0, 15).padEnd(16);
            const statusCode = this.getStatusCode(status);

            this.sendConsole(entity, ` [${String(i + 1).padStart(3)}] ${name} ${version} ${author} ${file} ${statusCode}`);
        }

        this.sendConsole(entity, `----- ${this.getLang(entity, 'PAUSE_ENTRIES', start + 1, end, total, running)} -----`);
        this.sendConsole(entity, `  ${this.getLang(entity, 'STATUS_LEGEND')}`);

        if (end < total) {
            this.sendConsole(entity, `----- ${this.getLang(entity, 'PAUSE_USE_MORE', end + 1)} -----`);
        }
    }

    private showUsage(entity: nodemod.Entity | null) {
        this.sendConsole(entity, this.getLang(entity, 'PAUSE_USAGE'));
        this.sendConsole(entity, `${this.getLang(entity, 'PAUSE_COMMANDS')}:`);
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_OFF'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_ON'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_PAUSE'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_ENABLE'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_STOP'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_SAVE'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_CLEAR'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_LIST'));
        this.sendConsole(entity, this.getLang(entity, 'COM_PAUSE_ADD'));
    }

    // ========================================================================
    // Public API
    // ========================================================================

    /**
     * Check if a plugin is currently paused
     */
    public isPluginPaused(filename: string): boolean {
        return this.isPaused(filename);
    }

    /**
     * Get all paused plugin filenames
     */
    public getPausedPlugins(): string[] {
        return Array.from(this.pausedPlugins);
    }
}



export default PauseCfg;
