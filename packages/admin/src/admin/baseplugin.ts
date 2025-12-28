// Base Plugin Class
// Provides common functionality for admin plugins to reduce code duplication
//
// Usage:
//   class MyPlugin extends BasePlugin implements Plugin {
//       readonly metadata: PluginMetadata = { ... };
//       constructor(pluginName: string) {
//           super(pluginName);
//           // ... plugin initialization
//       }
//   }
//
// The pluginName is passed by the plugin loader from plugins.ini and is used
// for localization lookups and command/CVAR registration.

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_ADMIN } from './constants';
import { registerCommand as helpRegisterCommand, cvarRegistry } from './helpregistry';
import localization from './localization';
import * as utils from './utils';
import type { PluginMetadata } from './pluginloader';

const cvar = nodemodCore.cvar;

/**
 * Base class for admin plugins providing common utilities.
 * Extend this class to get access to shared functionality.
 */
export abstract class BasePlugin {
    /** Plugin name from plugins.ini - used for localization and registration */
    protected readonly pluginName: string;

    /** Cached CVAR wrapper for amx_show_activity */
    private amxShowActivity: any;

    /**
     * Plugin metadata - must be implemented by subclasses.
     */
    abstract readonly metadata: PluginMetadata;

    /**
     * @param pluginName Plugin name from plugins.ini (e.g., 'adminchat', 'plmenu')
     */
    constructor(pluginName: string) {
        this.pluginName = pluginName;

        // Get amx_show_activity CVAR (registered by admin base)
        this.amxShowActivity = cvar.wrap('amx_show_activity');

        // Load localization dictionary for this plugin
        localization.loadDictionary(pluginName);
    }

    // ========================================================================
    // Command & CVAR Registration
    // ========================================================================

    /**
     * Register a command with automatic plugin tracking.
     */
    protected registerCommand(
        name: string,
        flags: number,
        description: string,
        callback: (entity: nodemod.Entity | null, args: string[]) => void
    ): void {
        helpRegisterCommand(name, flags, description, callback, this.pluginName);
    }

    /**
     * Register a CVAR with automatic plugin tracking.
     * @param name CVAR name
     * @param defaultValue Default value
     * @param flags CVAR flags
     * @param description CVAR description
     * @returns Wrapped CVAR for easy access
     */
    protected registerCvar(
        name: string,
        defaultValue: string,
        flags: number = 0,
        description: string = ''
    ): any {
        if (!cvar.exists(name)) {
            cvar.register(name, defaultValue, flags, description);
        }
        cvarRegistry.register(name, this.pluginName);
        return cvar.wrap(name);
    }

    // ========================================================================
    // Localization
    // ========================================================================

    /**
     * Get localized string for the current plugin
     * @param entity Target entity for language detection, or null
     * @param key Localization key
     * @param args Format arguments
     */
    protected getLang(entity: nodemod.Entity | null, key: string, ...args: any[]): string {
        return localization.getLang(entity, this.pluginName, key, ...args);
    }

    /**
     * Get localized string, falling back to common dictionary
     * @param entity Target entity for language detection, or null
     * @param key Localization key
     * @param args Format arguments
     */
    protected getLangWithFallback(entity: nodemod.Entity | null, key: string, ...args: any[]): string {
        let result = localization.getLang(entity, this.pluginName, key, ...args);
        if (result === key) {
            result = localization.getLang(entity, 'common', key, ...args);
        }
        return result;
    }

    // ========================================================================
    // Console/Chat Output
    // ========================================================================

    /**
     * Send message to entity's console, or server console if null
     */
    protected sendConsole(entity: nodemod.Entity | null, message: string): void {
        utils.sendMessage(entity, message);
    }

    /**
     * Send chat message to target entity, or all players if null
     */
    protected sendChat(target: nodemod.Entity | null, message: string): void {
        utils.sendChatToTarget(message, target, () => adminSystem.getPlayers());
    }

    /**
     * Send chat message to all players with a specific access flag
     */
    protected sendChatToAccess(message: string, accessFlag: number): void {
        for (const player of adminSystem.getPlayers()) {
            if (adminSystem.hasAccess(player, accessFlag)) {
                nodemodCore.util.sendChat(message, player);
            }
        }
    }

    // ========================================================================
    // Show Activity
    // ========================================================================

    /**
     * Get show activity options for use with utils.showActivity
     */
    protected getShowActivityOptions(): utils.ShowActivityOptions {
        return {
            getShowActivityLevel: () => this.amxShowActivity?.int || 2,
            hasAdminAccess: (e) => adminSystem.hasAccess(e, ADMIN_ADMIN)
        };
    }

    /**
     * Show admin activity to all players based on amx_show_activity setting
     * @param adminEntity The admin performing the action (null for console)
     * @param message The activity message
     */
    protected showActivity(adminEntity: nodemod.Entity | null, message: string): void {
        const adminName = utils.getAdminName(adminEntity);
        const options = this.getShowActivityOptions();

        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            utils.showActivity(player, adminName, message, options);
        }
    }

    /**
     * Show admin activity using two lang keys - one for activity level 1 (no name),
     * one for activity level 2+ (with name substituted via %s).
     * Equivalent to AMXX show_activity_key().
     *
     * @param adminEntity The admin performing the action
     * @param langKey1 Lang key for activity level 1 (e.g., "ADMIN: action %s")
     * @param langKey2 Lang key for activity level 2+ (e.g., "ADMIN %s: action %s")
     * @param args Additional arguments for the lang strings (e.g., map name)
     */
    protected showActivityKey(
        adminEntity: nodemod.Entity | null,
        langKey1: string,
        langKey2: string,
        ...args: any[]
    ): void {
        const adminName = utils.getAdminName(adminEntity);
        const activityLevel = this.amxShowActivity?.int || 2;

        if (activityLevel === 0) {
            return; // Show nothing
        }

        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            if (utils.isBot(player)) continue;

            let message: string;
            if (activityLevel === 1) {
                // Activity level 1: show without admin name
                message = this.getLang(player, langKey1, ...args);
            } else {
                // Activity level 2+: show with admin name as first arg
                message = this.getLang(player, langKey2, adminName, ...args);
            }

            this.sendChat(player, message);
        }
    }

    // ========================================================================
    // Utility Methods
    // ========================================================================

    /**
     * Get current game time in seconds
     */
    protected getGameTime(): number {
        return utils.getGameTime();
    }

    /**
     * Get player name with fallback to 'Unknown'
     */
    protected getPlayerName(entity: nodemod.Entity): string {
        return utils.getPlayerName(entity);
    }

    /**
     * Get admin name - returns 'CONSOLE' for null entity
     */
    protected getAdminName(entity: nodemod.Entity | null): string {
        return utils.getAdminName(entity);
    }

    /**
     * Parse command string into arguments
     */
    protected parseCommand(text: string): string[] {
        return utils.parseCommand(text);
    }

    /**
     * Log an AMXX-style message
     */
    protected logAmx(message: string): void {
        console.log(`[AMXX] ${message}`);
    }

    // ========================================================================
    // Player Information Extraction
    // ========================================================================

    /**
     * Player information structure for logging and display
     */
    protected extractPlayerInfo(entity: nodemod.Entity | null): {
        name: string;
        authId: string;
        userId: number;
        team: string;
        teamShort: string;
        /** Formatted string for HL log format: "name<userid><authid><team>" */
        logFormat: string;
    } {
        if (!entity) {
            return {
                name: 'CONSOLE',
                authId: 'CONSOLE',
                userId: 0,
                team: '',
                teamShort: '',
                logFormat: '"Console<0><CONSOLE><>"'
            };
        }

        const name = entity.netname || 'Unknown';
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        const userId = nodemod.eng.getPlayerUserId(entity);
        const team = utils.getPlayerTeamName(entity);
        const teamShort = utils.getPlayerTeamShort(entity);

        return {
            name,
            authId,
            userId,
            team,
            teamShort,
            logFormat: `"${name}<${userId}><${authId}><${team}>"`
        };
    }

    // ========================================================================
    // Admin Action Logging
    // ========================================================================

    /**
     * Log an admin action in HL engine format.
     * Consolidates the dual-format logging pattern used across admin commands.
     *
     * This logs in the format: L MM/DD/YYYY - HH:MM:SS: "admin<id><auth><team>" action "target<id><auth><team>" (params...)
     *
     * @param admin The admin performing the action (null for server console)
     * @param action The action description (e.g., "kick", "ban", "slay")
     * @param target Optional target player
     * @param params Additional key-value pairs to log
     */
    protected logAdminAction(
        admin: nodemod.Entity | null,
        action: string,
        target?: nodemod.Entity | null,
        params: { [key: string]: string } = {}
    ): void {
        const adminInfo = this.extractPlayerInfo(admin);

        let logLine = `L ${this.getLogTimestamp()}: ${adminInfo.logFormat} ${action}`;

        if (target) {
            const targetInfo = this.extractPlayerInfo(target);
            logLine += ` ${targetInfo.logFormat}`;
        }

        // Build parameters string
        for (const [key, value] of Object.entries(params)) {
            logLine += ` (${key} "${value}")`;
        }

        console.log(logLine);
    }

    /**
     * Log a message in HL engine format for external log parsing tools.
     * This is the format used by log analysis tools like HLSW, etc.
     *
     * Format: "<name><userid><authid><team>" triggered "<action>" (key "value")...
     *
     * @param entity The player entity (or null for server)
     * @param action The action name (e.g., "amx_tsay", "amx_chat")
     * @param params Additional key-value pairs to log
     */
    protected logMessage(
        entity: nodemod.Entity | null,
        action: string,
        params: { [key: string]: string } = {}
    ): void {
        const playerInfo = this.extractPlayerInfo(entity);

        // Build parameters string
        let paramsStr = '';
        for (const [key, value] of Object.entries(params)) {
            paramsStr += ` (${key} "${value}")`;
        }

        console.log(`L ${this.getLogTimestamp()}: ${playerInfo.logFormat} triggered "${action}"${paramsStr}`);
    }

    /**
     * Get timestamp in HL log format: MM/DD/YYYY - HH:MM:SS
     */
    private getLogTimestamp(): string {
        const now = new Date();
        const month = String(now.getMonth() + 1).padStart(2, '0');
        const day = String(now.getDate()).padStart(2, '0');
        const year = now.getFullYear();
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');
        const seconds = String(now.getSeconds()).padStart(2, '0');
        return `${month}/${day}/${year} - ${hours}:${minutes}:${seconds}`;
    }
}

export default BasePlugin;
