// Multi-Lingual System Plugin
// Converted from AMX Mod X multilingual.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_CFG } from './constants';

import { BasePlugin } from './baseplugin';
import * as utils from './utils';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import localization from './localization';

const cvar = nodemodCore.cvar;

// Default languages (used as fallback if dynamic detection fails)
const DEFAULT_LANGUAGES = ['en', 'de', 'es', 'fr', 'pl', 'pt', 'ru'];

// Default language names (used as fallback)
const DEFAULT_LANGUAGE_NAMES: { [key: string]: string } = {
    'en': 'English',
    'de': 'Deutsch',
    'es': 'Español',
    'fr': 'Français',
    'pl': 'Polski',
    'pt': 'Português',
    'ru': 'Русский'
};

// Player menu state
interface PlayerLangState {
    personalLangIndex: number;
    serverLangIndex: number;
}

class MultiLingual extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Multi-Lingual System',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Language selection system'
    };

    // Dynamically detected available languages
    private availableLanguages: string[] = [];
    private languageNames: { [key: string]: string } = {};

    // Player states
    private playerStates: Map<number, PlayerLangState> = new Map();

    // Server language index
    private serverLangIndex: number = 0;

    // Pending info tasks
    private pendingTasks: Map<number, ReturnType<typeof setTimeout>> = new Map();

    // CVARs
    private amxLanguage: any;
    private amxClientLanguages: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Load common and languages dictionaries
        localization.loadDictionary('common');
        localization.loadDictionary('languages');

        // Detect available languages dynamically
        this.detectAvailableLanguages();

        // Register CVARs
        this.registerCvar('amx_language', 'en', nodemod.FCVAR.SERVER, 'Server default language');
        this.registerCvar('amx_client_languages', '1', nodemod.FCVAR.SERVER, 'Enable client language selection');

        this.amxLanguage = cvar.wrap('amx_language');
        this.amxClientLanguages = cvar.wrap('amx_client_languages');

        // Load server language from storage
        this.loadServerLanguage();

        // Register commands
        this.registerCommands();

        // Register for client connect/disconnect
        this.registerEvents();
    }

    /**
     * Dynamically detect available languages from the localization system.
     * Uses the 'languages' dictionary to find language codes and their display names.
     */
    private detectAvailableLanguages() {
        // Get available languages from the 'languages' dictionary
        const detected = localization.getAvailableLanguages('languages');

        if (detected.length > 0) {
            this.availableLanguages = detected.sort();

            // Get language names from the LANG_NAME key in each language
            for (const lang of this.availableLanguages) {
                // Use the localization system to get the language name
                // Pass null to use server language, but we want the specific language's entry
                const name = this.getLanguageDisplayName(lang);
                this.languageNames[lang] = name;
            }

        } else {
            // Fallback to defaults
            this.availableLanguages = [...DEFAULT_LANGUAGES];
            this.languageNames = { ...DEFAULT_LANGUAGE_NAMES };
        }
    }

    /**
     * Get the display name for a language code.
     * Looks up LANG_NAME in the languages dictionary for that specific language.
     * This matches the original AMX behavior of showing language names in their native form.
     */
    private getLanguageDisplayName(langCode: string): string {
        // Try to get LANG_NAME from the languages dictionary for this specific language
        // This gets the name in its native form (e.g., "English" for en, "Deutsch" for de)
        const name = localization.getLangForCode(langCode, 'languages', 'LANG_NAME');

        // If we got something other than the key back, use it
        if (name && name !== 'LANG_NAME') {
            return name;
        }

        // Fall back to hardcoded default if available
        if (DEFAULT_LANGUAGE_NAMES[langCode]) {
            return DEFAULT_LANGUAGE_NAMES[langCode];
        }

        // Otherwise use the language code itself
        return langCode.toUpperCase();
    }

    private loadServerLanguage() {
        // Try to load from a simple state file
        const stateFile = path.join(utils.getConfigsDir(), 'server_language.txt');
        let lang = 'en';

        try {
            if (fs.existsSync(stateFile)) {
                lang = fs.readFileSync(stateFile, 'utf-8').trim() || 'en';
            }
        } catch (e) {
            // Default to English
        }

        // Validate language
        if (!this.availableLanguages.includes(lang)) {
            lang = 'en';
        }

        this.serverLangIndex = this.availableLanguages.indexOf(lang);
        cvar.setString('amx_language', lang);
        localization.setDefaultLang(lang);
    }

    private saveServerLanguage(lang: string) {
        const stateFile = path.join(utils.getConfigsDir(), 'server_language.txt');
        try {
            fs.writeFileSync(stateFile, lang);
        } catch (e) {
            console.error('[MultiLingual] Error saving server language:', e);
        }
    }

    private registerCommands() {
        // Admin command to set server language
        this.registerCommand('amx_setlang', ADMIN_CFG, '<language> - set server language', (entity, args) => {
            this.cmdSetLang(entity, args);
        });

        // Client command for language menu (anyone can use - flag 0)
        this.registerCommand('amx_langmenu', 0, '- displays language selection menu', (entity, args) => {
            this.cmdLangMenu(entity);
        });
    }

    private registerEvents() {
        // Client connect - show info message
        nodemodCore.events.on('dllClientPutInServer', (entity: nodemod.Entity) => {
            if (!entity) return;

            const clientLangs = this.amxClientLanguages?.int || 1;
            if (!clientLangs) return;

            // Skip bots
            if (utils.isBot(entity)) return;

            const playerIndex = nodemod.eng.indexOfEdict(entity);

            // Show info after 10 seconds
            const taskId = setTimeout(() => {
                this.dispInfo(entity);
                this.pendingTasks.delete(playerIndex);
            }, 10000);

            this.pendingTasks.set(playerIndex, taskId);
        });

        // Client disconnect - clean up
        nodemodCore.events.on('dllClientDisconnect', (entity: nodemod.Entity) => {
            if (!entity) return;

            const playerIndex = nodemod.eng.indexOfEdict(entity);

            // Cancel pending task
            const taskId = this.pendingTasks.get(playerIndex);
            if (taskId) {
                clearTimeout(taskId);
                this.pendingTasks.delete(playerIndex);
            }

            // Clean up state
            this.playerStates.delete(playerIndex);

            // Clear client language
            localization.clearClientLang(entity);
        });
    }

    private dispInfo(entity: nodemod.Entity) {
        const clientLangs = this.amxClientLanguages?.int || 1;
        if (!clientLangs) return;

        this.sendChat(entity, this.getLang(entity, 'TYPE_LANGMENU'));
    }

    private cmdSetLang(entity: nodemod.Entity | null, args: string[]) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_SETLANG'));
            return;
        }

        const lang = args[0].toLowerCase();

        if (!this.availableLanguages.includes(lang)) {
            this.sendConsole(entity, this.getLang(entity, 'LANG_NOT_EXISTS'));
            return;
        }

        this.serverLangIndex = this.availableLanguages.indexOf(lang);
        this.saveServerLanguage(lang);
        cvar.setString('amx_language', lang);
        localization.setDefaultLang(lang);

        const langName = this.languageNames[lang] || lang;
        this.sendConsole(entity, this.getLang(entity, 'SET_LANG_SERVER', langName));
    }

    private cmdLangMenu(entity: nodemod.Entity | null) {
        if (!entity) return;

        const clientLangs = this.amxClientLanguages?.int || 1;
        if (!clientLangs) {
            this.sendConsole(entity, this.getLang(null, 'LANG_MENU_DISABLED'));
            return;
        }

        const playerIndex = nodemod.eng.indexOfEdict(entity);

        // Get current player language
        const currentLang = localization.getClientLang(entity);
        const playerLangIndex = this.availableLanguages.indexOf(currentLang);

        // Initialize state
        this.playerStates.set(playerIndex, {
            personalLangIndex: playerLangIndex >= 0 ? playerLangIndex : 0,
            serverLangIndex: this.serverLangIndex
        });

        this.showLangMenu(entity);
    }

    private getPlayerState(entity: nodemod.Entity): PlayerLangState {
        const playerIndex = nodemod.eng.indexOfEdict(entity);
        let state = this.playerStates.get(playerIndex);
        if (!state) {
            state = {
                personalLangIndex: 0,
                serverLangIndex: this.serverLangIndex
            };
            this.playerStates.set(playerIndex, state);
        }
        return state;
    }

    private showLangMenu(entity: nodemod.Entity) {
        const clientLangs = this.amxClientLanguages?.int || 1;
        if (!clientLangs) return;

        const state = this.getPlayerState(entity);
        const isAdmin = adminSystem.hasAccess(entity, ADMIN_CFG);

        const personalLang = this.availableLanguages[state.personalLangIndex];
        const personalLangName = this.languageNames[personalLang] || personalLang;

        const items: any[] = [];

        // Option 1: Personal language (cycles through languages)
        items.push({
            name: `${this.getLang(entity, 'PERSO_LANG')}: ${personalLangName}`,
            handler: (client: nodemod.Entity) => {
                this.cyclePersonalLang(client);
            }
        });

        // Option 2: Server language (admin only, cycles through languages)
        if (isAdmin) {
            const serverLang = this.availableLanguages[state.serverLangIndex];
            const serverLangName = this.languageNames[serverLang] || serverLang;

            items.push({
                name: `${this.getLang(entity, 'SERVER_LANG')}: ${serverLangName}`,
                handler: (client: nodemod.Entity) => {
                    this.cycleServerLang(client);
                }
            });
        }

        // Option 3 (or 2 for non-admin): Save
        items.push({
            name: this.getLang(entity, 'SAVE_LANG'),
            handler: (client: nodemod.Entity) => {
                this.saveLangSettings(client);
            }
        });

        nodemodCore.menu.show({
            title: this.getLang(entity, 'LANG_MENU'),
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private cyclePersonalLang(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);

        state.personalLangIndex = (state.personalLangIndex + 1) % this.availableLanguages.length;

        // Refresh menu
        setTimeout(() => {
            this.showLangMenu(entity);
        }, 0);
    }

    private cycleServerLang(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);

        state.serverLangIndex = (state.serverLangIndex + 1) % this.availableLanguages.length;

        // Refresh menu
        setTimeout(() => {
            this.showLangMenu(entity);
        }, 0);
    }

    private saveLangSettings(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);
        const isAdmin = adminSystem.hasAccess(entity, ADMIN_CFG);

        const newPersonalLang = this.availableLanguages[state.personalLangIndex];
        const currentPersonalLang = localization.getClientLang(entity);

        // Save personal language if changed
        if (newPersonalLang !== currentPersonalLang) {
            localization.setClientLang(entity, newPersonalLang);

            const langName = this.languageNames[newPersonalLang] || newPersonalLang;
            this.sendChat(entity, this.getLang(entity, 'SET_LANG_USER', langName));

            // Also set client info
            try {
                nodemod.eng.setClientKeyValue(
                    nodemod.eng.indexOfEdict(entity),
                    entity,
                    'lang',
                    newPersonalLang
                );
            } catch (e) {
                // May not be supported
            }
        }

        // Save server language if admin and changed
        if (isAdmin) {
            const newServerLang = this.availableLanguages[state.serverLangIndex];
            const currentServerLang = this.availableLanguages[this.serverLangIndex];

            if (newServerLang !== currentServerLang) {
                this.serverLangIndex = state.serverLangIndex;
                this.saveServerLanguage(newServerLang);
                cvar.setString('amx_language', newServerLang);
                localization.setDefaultLang(newServerLang);

                const langName = this.languageNames[newServerLang] || newServerLang;
                this.sendChat(entity, this.getLang(entity, 'SET_LANG_SERVER', langName));
            }
        }
    }
}



export default MultiLingual;
