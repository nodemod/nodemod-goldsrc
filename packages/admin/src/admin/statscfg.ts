// Stats Configuration Plugin
// Converted from AMX Mod X statscfg.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO
//
// NodeMod enhancements over original SMA:
// - Provides a global StatsRegistry for plugins to register configurable options
// - Supports dynamic registration/unregistration of stats variables
// - Uses getter/setter functions instead of direct CVAR manipulation
//
// Other plugins can register stats options via:
//   import { statsRegistry } from './statscfg';
//   statsRegistry.register('ST_SHOW_KILLER', 'ShowKiller', () => value, (v) => value = v);
//
// Commands:
//   amx_statscfgmenu - Opens the configuration menu
//   amx_statscfg on/off <pattern> - Enable/disable matching options
//   amx_statscfg save/load - Save/load settings
//   amx_statscfg list [start] - List all options

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { ADMIN_CFG } from './constants';

import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

// Maximum number of stats options
const MAX_MENU_DATA = 72;

// Stats entry interface
interface StatsEntry {
    name: string;           // Display name (e.g., "ST_SHOW_KILLER")
    variable: string;       // Variable name (e.g., "ShowKiller")
    getValue: () => number; // Getter function
    setValue: (val: number) => void; // Setter function
}

// Registry for stats variables
class StatsRegistry {
    private entries: StatsEntry[] = [];

    /**
     * Register a stats variable
     * @param name Display name for the setting
     * @param variable Variable identifier
     * @param getValue Function to get current value
     * @param setValue Function to set value
     */
    register(name: string, variable: string, getValue: () => number, setValue: (val: number) => void): void {
        // Check if already registered
        const existing = this.entries.find(e => e.variable === variable);
        if (existing) {
            existing.name = name;
            existing.getValue = getValue;
            existing.setValue = setValue;
            return;
        }

        if (this.entries.length >= MAX_MENU_DATA) {
            console.log('[StatsCfg] Warning: Maximum stats entries reached');
            return;
        }

        this.entries.push({ name, variable, getValue, setValue });
    }

    /**
     * Unregister a stats variable
     */
    unregister(variable: string): void {
        const index = this.entries.findIndex(e => e.variable === variable);
        if (index !== -1) {
            this.entries.splice(index, 1);
        }
    }

    /**
     * Get all entries
     */
    getEntries(): StatsEntry[] {
        return this.entries;
    }

    /**
     * Get entry by variable name
     */
    getEntry(variable: string): StatsEntry | undefined {
        return this.entries.find(e => e.variable === variable);
    }

    /**
     * Find entries matching a pattern
     */
    findEntries(pattern: string): StatsEntry[] {
        const lowerPattern = pattern.toLowerCase();
        return this.entries.filter(e =>
            e.variable.toLowerCase().includes(lowerPattern)
        );
    }

    /**
     * Get entry count
     */
    getCount(): number {
        return this.entries.length;
    }
}

// Global stats registry
export const statsRegistry = new StatsRegistry();

// Items per page for menu pagination
// Note: Manual pagination is needed because the "Save Configuration" option must appear
// on EVERY page, which the core menu pagination can't handle automatically.
const ITEMS_PER_PAGE = 6;

class StatsCfg extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Stats Configuration',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Configuration menu for stats plugins'
    };

    // File path for saving/loading
    private configFile: string;

    // Modified flag
    private modified: boolean = false;

    // Per-player menu position
    private menuPosition: Map<number, number> = new Map();

    constructor(pluginName: string) {
        super(pluginName);

        // Set config file path
        this.configFile = path.join(utils.getConfigsDir(), 'stats.ini');

        // Register commands
        this.registerCommands();

        // Load settings
        this.loadSettings();
    }

    private registerCommands() {
        // Menu command
        this.registerCommand('amx_statscfgmenu', ADMIN_CFG, '- displays stats configuration menu', (entity, args) => {
            this.cmdCfgMenu(entity);
        });

        // Console command
        this.registerCommand('amx_statscfg', ADMIN_CFG, '- displays help for stats configuration', (entity, args) => {
            this.cmdCfg(entity, args);
        });
    }

    // ==================== CONSOLE COMMAND ====================

    private cmdCfg(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        const cmd = args[0]?.toLowerCase() || '';

        if (cmd === 'on' || cmd === 'off') {
            // Enable/disable matching variables
            if (args.length < 2) {
                this.showUsage(entity);
                return;
            }

            const pattern = args[1];
            const enable = cmd === 'on';
            const matches = statsRegistry.findEntries(pattern);

            if (matches.length === 0) {
                this.sendConsole(entity, this.getLang(entity, 'NO_OPTION', pattern));
                return;
            }

            this.modified = true;
            for (const entry of matches) {
                entry.setValue(enable ? 1 : 0);
                const status = enable
                    ? this.getLang(entity, 'STATS_ENABLED')
                    : this.getLang(entity, 'STATS_DISABLED');
                this.sendConsole(entity, `${status}: ${this.getEntryDisplayName(entity, entry)}`);
            }
            this.sendConsole(entity, this.getLang(entity, 'TOTAL_NUM', matches.length.toString()));

        } else if (cmd === 'save') {
            // Save settings
            if (this.saveSettings()) {
                this.modified = false;
                this.sendConsole(entity, this.getLang(entity, 'STATS_CONF_SAVED'));
            } else {
                this.sendConsole(entity, this.getLang(entity, 'STATS_CONF_FAILED'));
            }

        } else if (cmd === 'load') {
            // Load settings
            if (this.loadSettings()) {
                this.modified = false;
                this.sendConsole(entity, this.getLang(entity, 'STATS_CONF_LOADED'));
            } else {
                this.sendConsole(entity, this.getLang(entity, 'STATS_CONF_FAIL_LOAD'));
            }

        } else if (cmd === 'list') {
            // List all settings
            let start = parseInt(args[1]) || 1;
            start = Math.max(1, start) - 1;

            const entries = statsRegistry.getEntries();
            const total = entries.length;

            if (start >= total) {
                start = Math.max(0, total - 1);
            }

            const end = Math.min(start + 10, total);

            // Header
            const lName = this.getLangWithFallback(entity, 'NAME');
            const lVariable = this.getLangWithFallback(entity, 'VARIABLE');
            const lStatus = this.getLangWithFallback(entity, 'STATUS');

            this.sendConsole(entity, '');
            this.sendConsole(entity, `----- ${this.getLang(entity, 'STATS_CONF')}: -----`);
            this.sendConsole(entity, `     ${lName.padEnd(29)}   ${lVariable.padEnd(24)}   ${lStatus}`);

            if (total > 0) {
                for (let i = start; i < end; i++) {
                    const entry = entries[i];
                    const displayName = this.getEntryDisplayName(entity, entry);
                    const status = entry.getValue() ? this.getLangWithFallback(entity, 'ON') : this.getLangWithFallback(entity, 'OFF');
                    this.sendConsole(entity, `${(i + 1).toString().padStart(3)}: ${displayName.padEnd(29)}   ${entry.variable.padEnd(24)}   ${status}`);
                }
            }

            this.sendConsole(entity, this.getLang(entity, 'STATS_ENTRIES_OF', (start + 1).toString(), end.toString(), total.toString()));

            if (end < total) {
                this.sendConsole(entity, this.getLang(entity, 'STATS_USE_MORE', (end + 1).toString()));
            } else {
                this.sendConsole(entity, this.getLang(entity, 'STATS_USE_BEGIN'));
            }

        } else if (cmd === 'add' && args.length >= 3) {
            // Add a new entry (for backwards compatibility)
            // In practice, plugins should use statsRegistry.register() directly
            const name = args[1];
            const variable = args[2];

            // Create a simple CVAR-based entry
            const cvarName = `stats_${variable}`;
            const cvar = nodemodCore.cvar;
            this.registerCvar(cvarName, '0', nodemod.FCVAR.SERVER, `Stats: ${name}`);
            const wrapper = cvar.wrap(cvarName);

            statsRegistry.register(
                name,
                variable,
                () => wrapper?.int || 0,
                (val) => { if (wrapper) cvar.setString(cvarName, val.toString()); }
            );

            this.sendConsole(entity, this.getLang(entity, 'STATS_ENTRY_ADDED', name, variable));

        } else {
            this.showUsage(entity);
        }
    }

    private showUsage(entity: nodemod.Entity | null) {
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_USAGE'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_COM'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_ON'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_OFF'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_SAVE'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_LOAD'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_LIST'));
        this.sendConsole(entity, this.getLang(entity, 'COM_STATS_ADD'));
    }

    // ==================== MENU ====================

    private cmdCfgMenu(entity: nodemod.Entity | null) {
        if (!entity) {
            console.log('[StatsCfg] Stats config menu is only available for players');
            return;
        }
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        // Reset to first page
        const entityId = nodemod.eng.indexOfEdict(entity);
        this.menuPosition.set(entityId, 0);

        this.displayCfgMenu(entity, 0);
    }

    private displayCfgMenu(entity: nodemod.Entity, pos: number = 0) {
        if (pos < 0) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        const entries = statsRegistry.getEntries();
        const total = entries.length;

        // Handle empty list
        if (total === 0) {
            const items: any[] = [{
                name: this.getLang(entity, 'NO_STATS'),
                disabled: true,
                handler: () => { }
            }];

            nodemodCore.menu.show({
                entity,
                title: this.getLang(entity, 'STATS_CONF'),
                items,
                formatters: utils.coloredMenuFormatters
            });
            return;
        }

        // Calculate pagination
        const start = pos * ITEMS_PER_PAGE;
        if (start >= total && total > 0) {
            this.menuPosition.set(entityId, 0);
            this.displayCfgMenu(entity, 0);
            return;
        }

        const end = Math.min(start + ITEMS_PER_PAGE, total);
        const totalPages = Math.ceil(total / ITEMS_PER_PAGE) || 1;
        const currentPage = pos + 1;

        const items: any[] = [];

        // Stats items for this page
        for (let i = start; i < end; i++) {
            const entry = entries[i];
            const displayName = this.getEntryDisplayName(entity, entry);
            const status = entry.getValue()
                ? this.getLangWithFallback(entity, 'ON')
                : this.getLangWithFallback(entity, 'OFF');

            items.push({
                name: `${displayName} [${status}]`,
                handler: (client: nodemod.Entity) => {
                    // Toggle the setting
                    this.modified = true;
                    entry.setValue(1 - entry.getValue());
                    const clientId = nodemod.eng.indexOfEdict(client);
                    setTimeout(() => this.displayCfgMenu(client, this.menuPosition.get(clientId) || 0), 0);
                }
            });
        }

        // Save configuration option
        const saveLabel = this.getLangWithFallback(entity, 'SAVE_CONF') + (this.modified ? ' *' : '');
        items.push({
            name: saveLabel,
            handler: (client: nodemod.Entity) => {
                if (this.saveSettings()) {
                    this.modified = false;
                    this.sendChat(client, '* ' + this.getLang(client, 'STATS_CONF_SAVED'));
                } else {
                    this.sendChat(client, '* ' + this.getLang(client, 'STATS_CONF_FAILED'));
                }
                const clientId = nodemod.eng.indexOfEdict(client);
                setTimeout(() => this.displayCfgMenu(client, this.menuPosition.get(clientId) || 0), 0);
            }
        });

        // Navigation: More (next page)
        if (end < total) {
            items.push({
                name: this.getLangWithFallback(entity, 'MORE') + '...',
                handler: (client: nodemod.Entity) => {
                    const clientId = nodemod.eng.indexOfEdict(client);
                    const newPos = (this.menuPosition.get(clientId) || 0) + 1;
                    this.menuPosition.set(clientId, newPos);
                    setTimeout(() => this.displayCfgMenu(client, newPos), 0);
                }
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'STATS_CONF')} ${currentPage}/${totalPages}`,
            items,
            onExit: pos > 0 ? (client: nodemod.Entity) => {
                // Go back to previous page
                const clientId = nodemod.eng.indexOfEdict(client);
                const newPos = (this.menuPosition.get(clientId) || 1) - 1;
                this.menuPosition.set(clientId, newPos);
                setTimeout(() => this.displayCfgMenu(client, newPos), 0);
            } : undefined,
            formatters: utils.coloredMenuFormatters
        });
    }

    // ==================== SETTINGS FILE ====================

    private saveSettings(): boolean {
        try {
            const entries = statsRegistry.getEntries();
            const lines: string[] = [
                ';Generated by Stats Configuration Plugin. Do not modify!',
                ';Variable  Description'
            ];

            for (const entry of entries) {
                if (entry.getValue()) {
                    // Get display name - use localization if it starts with ST_
                    let description: string;
                    if (entry.name.startsWith('ST_')) {
                        description = this.getLang(null, entry.name);
                    } else {
                        description = entry.name;
                    }
                    lines.push(`${entry.variable.padEnd(24)} ;${description}`);
                }
            }

            fs.writeFileSync(this.configFile, lines.join('\n') + '\n');
            return true;
        } catch (e) {
            console.error('[StatsCfg] Error saving settings:', e);
            return false;
        }
    }

    private loadSettings(): boolean {
        if (!fs.existsSync(this.configFile)) {
            return false;
        }

        try {
            const content = fs.readFileSync(this.configFile, 'utf-8');
            const lines = content.split('\n');

            // First, disable all entries
            for (const entry of statsRegistry.getEntries()) {
                entry.setValue(0);
            }

            // Then enable entries listed in the file
            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';')) continue;

                // Extract variable name (first word)
                const parts = utils.parseCommand(trimmed);
                if (parts.length > 0) {
                    const varName = parts[0];
                    const entry = statsRegistry.getEntry(varName);
                    if (entry) {
                        entry.setValue(1);
                    }
                }
            }

            return true;
        } catch (e) {
            console.error('[StatsCfg] Error loading settings:', e);
            return false;
        }
    }

    // ==================== UTILITIES ====================

    /**
     * Get display name for an entry, with localization if it starts with ST_
     */
    private getEntryDisplayName(entity: nodemod.Entity | null, entry: StatsEntry): string {
        if (entry.name.startsWith('ST_')) {
            return this.getLang(entity, entry.name);
        }
        return entry.name;
    }

    /**
     * Plugin load - register commands during precache phase (like original)
     */
    onLoad() {
        // Settings are loaded in constructor
    }
}



export default StatsCfg;
