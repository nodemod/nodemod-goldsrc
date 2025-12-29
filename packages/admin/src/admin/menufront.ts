// Menus Front-End Plugin
// Converted from AMX Mod X menufront.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import * as utils from './utils';
import {
    ADMIN_MENU, ADMIN_KICK, ADMIN_BAN, ADMIN_SLAY, ADMIN_MAP,
    ADMIN_VOTE, ADMIN_CVAR, ADMIN_CFG, ADMIN_LEVEL_A, readFlags
} from './constants';

import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';

const cvar = nodemodCore.cvar;

// Menu item entry
interface MenuEntry {
    body: string;           // Display text or lang key
    isLangKey: boolean;     // Whether body is a localization key
    command: string;        // Command to execute when selected
    access: number;         // Required access flags
    pluginName: string;     // Plugin that owns this menu item
}

class MenuFront extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Menus Front-End',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Central menu hub for admin menus'
    };

    // Admin menu items
    private adminMenus: MenuEntry[] = [];

    // Client menu items
    private clientMenus: MenuEntry[] = [];

    constructor(pluginName: string) {
        super(pluginName);

        // Load common dictionary
        const localization = require('./localization').default;
        localization.loadDictionary('common');

        // Register commands
        this.registerCommands();

        // Add default menus after a short delay to ensure other plugins are loaded
        setTimeout(() => {
            this.addDefaultMenus();
            this.loadCustomMenuItems();
        }, 100);

    }

    private registerCommands() {
        // Admin menu command
        this.registerCommand('amxmodmenu', ADMIN_MENU, '- displays admin menus', (entity, args) => {
            this.cmdAdminMenu(entity);
        });

        // Client menu command (no access required - access checked per item)
        this.registerCommand('amx_menu', 0, '- displays client menus', (entity, args) => {
            this.cmdClientMenu(entity);
        });

        // Server commands to add menu items
        nodemodCore.cmd.registerServer('amx_addmenuitem', (args) => {
            this.addMenuItemCmd(args, false);
        });

        nodemodCore.cmd.registerServer('amx_addclientmenuitem', (args) => {
            this.addMenuItemCmd(args, true);
        });
    }

    // ========================================================================
    // Public API for other plugins to add menu items
    // ========================================================================

    /**
     * Add an admin menu item with literal text
     */
    public addMenu(body: string, command: string, access: number, pluginName: string): void {
        this.adminMenus.push({
            body,
            isLangKey: false,
            command,
            access,
            pluginName
        });
    }

    /**
     * Add an admin menu item with localization key
     */
    public addMenuLang(langKey: string, command: string, access: number, pluginName: string): void {
        this.adminMenus.push({
            body: langKey,
            isLangKey: true,
            command,
            access,
            pluginName
        });
    }

    /**
     * Add a client menu item with literal text
     */
    public addClientMenu(body: string, command: string, access: number, pluginName: string): void {
        this.clientMenus.push({
            body,
            isLangKey: false,
            command,
            access,
            pluginName
        });
    }

    /**
     * Add a client menu item with localization key
     */
    public addClientMenuLang(langKey: string, command: string, access: number, pluginName: string): void {
        this.clientMenus.push({
            body: langKey,
            isLangKey: true,
            command,
            access,
            pluginName
        });
    }

    // ========================================================================
    // Default Menu Items
    // ========================================================================

    private addDefaultMenus() {
        // Player management menus
        this.addMenuLang('KICK_PLAYER', 'amx_kickmenu', ADMIN_KICK, 'Players Menu');
        this.addMenuLang('BAN_PLAYER', 'amx_banmenu', ADMIN_BAN, 'Players Menu');
        this.addMenuLang('SLAP_SLAY', 'amx_slapmenu', ADMIN_SLAY, 'Players Menu');
        this.addMenuLang('TEAM_PLAYER', 'amx_teammenu', ADMIN_LEVEL_A, 'Players Menu');

        // Map menus
        this.addMenuLang('CHANGEL', 'amx_mapmenu', ADMIN_MAP, 'Maps Menu');
        this.addMenuLang('VOTE_MAPS', 'amx_votemapmenu', ADMIN_VOTE, 'Maps Menu');

        // Command menus
        this.addMenuLang('SPECH_STUFF', 'amx_speechmenu', ADMIN_MENU, 'Commands Menu');
        this.addMenuLang('CLIENT_COM', 'amx_clcmdmenu', ADMIN_LEVEL_A, 'Players Menu');
        this.addMenuLang('SERVER_COM', 'amx_cmdmenu', ADMIN_MENU, 'Commands Menu');
        this.addMenuLang('CVARS_SET', 'amx_cvarmenu', ADMIN_CVAR, 'Commands Menu');
        this.addMenuLang('CONFIG', 'amx_cfgmenu', ADMIN_MENU, 'Commands Menu');

        // Configuration menus
        this.addMenuLang('LANG_SET', 'amx_langmenu', ADMIN_CFG, 'Multi-Lingual System');
        this.addMenuLang('STATS_SET', 'amx_statscfgmenu', ADMIN_CFG, 'Stats Configuration');
        this.addMenuLang('PAUSE_PLUG', 'amx_pausecfgmenu', ADMIN_CFG, 'Pause Plugins');
        this.addMenuLang('RES_WEAP', 'amx_restmenu', ADMIN_CFG, 'Restrict Weapons');
        this.addMenuLang('TELE_PLAYER', 'amx_teleportmenu', ADMIN_CFG, 'Teleport Menu');
    }

    private loadCustomMenuItems() {
        // Load custom menu items from custommenuitems.cfg
        const configPath = path.join(process.cwd(), '..', 'configs', 'custommenuitems.cfg');

        if (!fs.existsSync(configPath)) {
            return;
        }

        try {
            const content = fs.readFileSync(configPath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();

                // Skip empty lines and comments
                if (!trimmed || trimmed.startsWith('//') || trimmed.startsWith(';')) {
                    continue;
                }

                // Parse: amx_addmenuitem "text" "command" "flags" "plugin"
                // or: amx_addclientmenuitem "text" "command" "flags" "plugin"
                if (trimmed.startsWith('amx_addmenuitem ')) {
                    const args = this.parseQuotedArgs(trimmed.substring(16));
                    this.addMenuItemCmd(args, false);
                } else if (trimmed.startsWith('amx_addclientmenuitem ')) {
                    const args = this.parseQuotedArgs(trimmed.substring(22));
                    this.addMenuItemCmd(args, true);
                }
            }
        } catch (e) {
            console.error('[MenuFront] Error loading custommenuitems.cfg:', e);
        }
    }

    private parseQuotedArgs(line: string): string[] {
        const args: string[] = [];
        let current = '';
        let inQuotes = false;

        for (let i = 0; i < line.length; i++) {
            const char = line[i];
            if (char === '"') {
                inQuotes = !inQuotes;
            } else if ((char === ' ' || char === '\t') && !inQuotes) {
                if (current) {
                    args.push(current);
                    current = '';
                }
            } else {
                current += char;
            }
        }
        if (current) {
            args.push(current);
        }
        return args;
    }

    private addMenuItemCmd(args: string[], isClientMenu: boolean) {
        if (args.length < 4) {
            console.log('[MenuFront] Usage: amx_addmenuitem "text" "command" "flags" "plugin"');
            return;
        }

        const body = args[0];
        const command = args[1];
        const access = readFlags(args[2]);
        const pluginName = args[3];

        if (isClientMenu) {
            this.addClientMenu(body, command, access, pluginName);
        } else {
            this.addMenu(body, command, access, pluginName);
        }
    }

    // ========================================================================
    // Admin Menu (amxmodmenu)
    // ========================================================================

    private cmdAdminMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_MENU)) return;

        this.displayAdminMenu(entity);
    }

    private displayAdminMenu(entity: nodemod.Entity) {
        const title = 'NodeMod Admin Menu';

        // Build items - only show accessible items from loaded plugins
        const items: any[] = [];

        for (const menu of this.adminMenus) {
            // Check access
            const hasAccess = adminSystem.hasAccess(entity, menu.access);

            // Check if plugin is loaded
            const pluginLoaded = this.isPluginLoaded(menu.pluginName);

            // Get display text
            const displayText = menu.isLangKey
                ? this.getLangWithFallback(entity, menu.body)
                : menu.body;

            if (hasAccess && pluginLoaded) {
                items.push({
                    name: displayText,
                    handler: (client: nodemod.Entity) => {
                        // Execute the menu command
                        nodemod.eng.clientCommand(client, `${menu.command}\n`);
                    }
                });
            } else {
                // Show disabled
                items.push({
                    name: displayText,
                    disabled: true
                });
            }
        }

        if (items.length === 0) {
            this.sendChat(entity, this.getLang(entity, 'NO_MENU_ITEMS'));
            return;
        }

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

    // ========================================================================
    // Client Menu (amx_menu)
    // ========================================================================

    private cmdClientMenu(entity: nodemod.Entity | null) {
        if (!entity) return;

        this.displayClientMenu(entity);
    }

    private displayClientMenu(entity: nodemod.Entity) {
        const title = 'NodeMod Client Menu';

        // Build items - only show accessible items from loaded plugins
        const items: any[] = [];

        for (const menu of this.clientMenus) {
            // Check access
            const hasAccess = menu.access === 0 || adminSystem.hasAccess(entity, menu.access);

            // Check if plugin is loaded
            const pluginLoaded = this.isPluginLoaded(menu.pluginName);

            // Get display text
            const displayText = menu.isLangKey
                ? this.getLangWithFallback(entity, menu.body)
                : menu.body;

            if (hasAccess && pluginLoaded) {
                items.push({
                    name: displayText,
                    handler: (client: nodemod.Entity) => {
                        nodemod.eng.clientCommand(client, `${menu.command}\n`);
                    }
                });
            } else {
                items.push({
                    name: displayText,
                    disabled: true
                });
            }
        }

        if (items.length === 0) {
            this.sendChat(entity, this.getLang(entity, 'NO_MENU_ITEMS'));
            return;
        }

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

    // ========================================================================
    // Helpers
    // ========================================================================

    private isPluginLoaded(pluginName: string): boolean {
        // First check by exact plugin filename
        if (pluginLoader.isLoaded(pluginName)) {
            return true;
        }

        // Then search by plugin metadata name (for display names like "Players Menu")
        const plugins = pluginLoader.getPlugins();
        for (const loadedPlugin of plugins) {
            if (loadedPlugin.status === 'running' &&
                loadedPlugin.plugin.metadata.name.toLowerCase() === pluginName.toLowerCase()) {
                return true;
            }
        }

        return false;
    }
}



// Export for other plugins to add menu items
export default MenuFront;
