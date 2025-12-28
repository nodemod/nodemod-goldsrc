// Admin System Utilities
// Equivalent to amxmisc.inc stock functions from AMX Mod X
// These are shared across all admin plugins

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import {
    ADMIN_USER,
    ADMIN_IMMUNITY,
    ADMIN_ALL,
    ADMIN_ADMIN,
    CMDTARGET_OBEY_IMMUNITY,
    CMDTARGET_ALLOW_SELF,
    CMDTARGET_ONLY_ALIVE,
    CMDTARGET_NO_BOTS
} from './constants';

// ============================================================================
// Player State Utilities
// ============================================================================

/**
 * Check if entity is a bot
 * Uses FL.FAKECLIENT flag from entity flags
 */
export function isBot(entity: nodemod.Entity | null): boolean {
    if (!entity) return false;
    return (entity.flags & nodemod.FL.FAKECLIENT) !== 0;
}

/**
 * Check if entity is alive
 * Checks health > 0 and deadflag === 0
 */
export function isAlive(entity: nodemod.Entity | null): boolean {
    if (!entity) return false;
    return entity.health > 0 && entity.deadflag === 0;
}

/**
 * Check if entity is connected (has a netname)
 */
export function isConnected(entity: nodemod.Entity | null): boolean {
    if (!entity) return false;
    return !!entity.netname && entity.netname.length > 0;
}

/**
 * Get player IP address
 */
export function getPlayerIP(entity: nodemod.Entity | null): string {
    if (!entity) return '';
    try {
        const buffer = nodemod.eng.getInfoKeyBuffer(entity);
        const ip = nodemod.eng.infoKeyValue(buffer, 'ip') || '';
        // Remove port if present
        return ip.split(':')[0];
    } catch {
        return '';
    }
}

// ============================================================================
// Team Property Utilities
// ============================================================================

/**
 * Get player team name for logging (full name)
 * Returns: 'TERRORIST', 'CT', 'SPECTATOR', or ''
 */
export function getPlayerTeamName(entity: nodemod.Entity | null): string {
    if (!entity) return '';
    try {
        const team = entity.team || 0;
        if (team === 1) return 'TERRORIST';
        if (team === 2) return 'CT';
        if (team === 3) return 'SPECTATOR';
        return '';
    } catch (e) {
        return '';
    }
}

/**
 * Get player team short name for menus (abbreviated)
 * Returns: 'TE', 'CT', 'SPE', or 'UN'
 */
export function getPlayerTeamShort(entity: nodemod.Entity | null): string {
    if (!entity) return 'UN';
    try {
        const team = entity.team || 0;
        if (team === 1) return 'TE';
        if (team === 2) return 'CT';
        if (team === 3 || team === 6) return 'SPE';
        return 'UN';
    } catch (e) {
        return 'UN';
    }
}

/**
 * Get player team number, mapping spectator team 3 to 6 (CS convention)
 */
export function getPlayerTeamNumber(entity: nodemod.Entity | null): number {
    if (!entity) return 0;
    try {
        const team = entity.team || 0;
        if (team === 3) return 6; // Spectator maps to 6 in CS
        return team;
    } catch (e) {
        return 0;
    }
}

// ============================================================================
// Path Utilities
// ============================================================================

/**
 * Get the path to the configs directory
 * This is typically ../configs relative to the plugins directory
 */
export function getConfigsDir(): string {
    return path.join(process.cwd(), '..', 'configs');
}

/**
 * Get the path to the game directory (e.g., valve/)
 * From plugins/, this is 3 levels up: plugins -> nodemod -> addons -> valve
 */
export function getGameDir(): string {
    return path.join(process.cwd(), '..', '..', '..');
}

/**
 * Options for showActivity - provides callbacks to access admin state
 */
export interface ShowActivityOptions {
    getShowActivityLevel: () => number;
    hasAdminAccess: (entity: nodemod.Entity) => boolean;
}

/**
 * Get activity message for a player based on amx_show_activity setting
 * Returns null if message should not be shown to this viewer
 *
 * @param viewer The player viewing the message
 * @param adminName The admin's name performing the action
 * @param message The activity message
 * @param options Callbacks for accessing admin system state
 * @returns Formatted message or null if should not show
 */
export function getActivityMessage(
    viewer: nodemod.Entity,
    adminName: string,
    message: string,
    options: ShowActivityOptions
): string | null {
    const activityLevel = options.getShowActivityLevel();

    // Skip bots
    if (isBot(viewer)) return null;

    if (activityLevel === 0) {
        // Show nothing
        return null;
    } else if (activityLevel === 1) {
        // Show to all without admin name
        return message;
    } else if (activityLevel === 2) {
        // Show to all with admin name
        return `ADMIN ${adminName}: ${message}`;
    } else if (activityLevel >= 3) {
        // Show to all, but only admins see the name
        if (options.hasAdminAccess(viewer)) {
            return `ADMIN ${adminName}: ${message}`;
        } else {
            return message;
        }
    }

    return null;
}

/**
 * Show activity message to a single player based on amx_show_activity setting
 * Equivalent to show_activity_id() from amxmisc.inc
 *
 * @param viewer The player viewing the message
 * @param adminName The admin's name performing the action
 * @param message The activity message
 * @param options Callbacks for accessing admin system state
 */
export function showActivity(
    viewer: nodemod.Entity,
    adminName: string,
    message: string,
    options: ShowActivityOptions
) {
    const displayMsg = getActivityMessage(viewer, adminName, message, options);
    if (displayMsg) {
        nodemodCore.util.sendChat(displayMsg, viewer);
    }
}

/**
 * Check if a user is an admin (has any admin flag except ADMIN_USER)
 * Equivalent to is_user_admin() from amxmisc.inc
 */
export function isUserAdmin(userFlags: number): boolean {
    return userFlags > 0 && !(userFlags & ADMIN_USER);
}

// ============================================================================
// Access Checking Utilities
// ============================================================================

/**
 * Check if user has access to a given level (silent check)
 * Equivalent to access() from amxmisc.inc
 *
 * @param userFlags The user's current access flags
 * @param level Required access level
 * @returns true if user has access
 */
export function access(userFlags: number, level: number): boolean {
    if (level === ADMIN_ALL) {
        return true;
    }
    if (level === ADMIN_ADMIN) {
        return isUserAdmin(userFlags);
    }
    return (userFlags & level) !== 0;
}

/**
 * Check if entity has access (for server console, always returns true)
 *
 * @param entity Entity to check (null for server console)
 * @param level Required access level
 * @param getUserFlags Function to get user's flags
 * @returns true if has access
 */
export function hasAccess(
    entity: nodemod.Entity | null,
    level: number,
    getUserFlags: (entity: nodemod.Entity) => number
): boolean {
    if (!entity) return true; // Server console always has access
    return access(getUserFlags(entity), level);
}

// ============================================================================
// Player Finding Utilities
// ============================================================================

/**
 * Find player by user ID (#123 format)
 */
export function findPlayerByUserId(userId: number): nodemod.Entity | null {
    for (const player of nodemod.players) {
        if (player && nodemod.eng.getPlayerUserId(player) === userId) {
            return player;
        }
    }
    return null;
}

/**
 * Find player by name (partial match, case-insensitive)
 */
export function findPlayerByName(name: string): nodemod.Entity | null {
    const lowerName = name.toLowerCase();
    for (const player of nodemod.players) {
        if (player) {
            const playerName = player.netname?.toLowerCase() || '';
            if (playerName.includes(lowerName)) {
                return player;
            }
        }
    }
    return null;
}

/**
 * Find player by exact name (case-insensitive)
 */
export function findPlayerByExactName(name: string): nodemod.Entity | null {
    const lowerName = name.toLowerCase();
    for (const player of nodemod.players) {
        if (player) {
            if (player.netname?.toLowerCase() === lowerName) {
                return player;
            }
        }
    }
    return null;
}

/**
 * Find player by Steam ID
 */
export function findPlayerBySteamId(steamId: string): nodemod.Entity | null {
    for (const player of nodemod.players) {
        if (player) {
            const playerSteamId = nodemod.eng.getPlayerAuthId(player);
            if (playerSteamId === steamId) {
                return player;
            }
        }
    }
    return null;
}

/**
 * Find player by IP address
 *
 * @param ip IP address to search for
 * @param clientIps Map of entity index to IP address
 */
export function findPlayerByIp(
    ip: string,
    clientIps: { [index: number]: string }
): nodemod.Entity | null {
    for (const player of nodemod.players) {
        if (player) {
            const index = nodemod.eng.indexOfEdict(player);
            const playerIp = clientIps[index];
            if (playerIp && playerIp === ip) {
                return player;
            }
        }
    }
    return null;
}

/**
 * Get all connected players with optional filters
 */
export function getPlayers(options?: {
    excludeBots?: boolean;
    onlyAlive?: boolean;
    isIngame?: (entity: nodemod.Entity) => boolean;
}): nodemod.Entity[] {
    const result: nodemod.Entity[] = [];
    for (const player of nodemod.players) {
        if (!player) continue;
        if (options?.isIngame && !options.isIngame(player)) continue;
        if (options?.excludeBots && isBot(player)) continue;
        if (options?.onlyAlive && !isAlive(player)) continue;
        result.push(player);
    }
    return result;
}

// ============================================================================
// cmd_target - Player Targeting
// Equivalent to cmd_target() from amxmisc.inc
// ============================================================================

export interface CmdTargetOptions {
    /** Function to get user's access flags */
    getUserFlags: (entity: nodemod.Entity) => number;
    /** Function to send message to entity */
    sendMessage: (entity: nodemod.Entity, message: string) => void;
    /** Function to check if player is ingame */
    isIngame?: (entity: nodemod.Entity) => boolean;
}

/**
 * Find a player target with various validation flags
 * Equivalent to cmd_target() from amxmisc.inc
 *
 * @param admin The admin issuing the command (null for server)
 * @param target Target string: "#userid", partial name, or full name
 * @param flags CMDTARGET_* flags
 * @param options Functions for getting user flags and sending messages
 * @returns Target player or null (sends error message on failure)
 */
export function cmdTarget(
    admin: nodemod.Entity | null,
    target: string,
    flags: number,
    options: CmdTargetOptions
): nodemod.Entity | null {
    let player: nodemod.Entity | null = null;

    // Handle #userid format
    if (target.startsWith('#')) {
        const userId = parseInt(target.substring(1));
        if (!isNaN(userId)) {
            player = findPlayerByUserId(userId);
        }
    }

    // Try to find by exact name first
    if (!player) {
        player = findPlayerByExactName(target);
    }

    // Try partial match
    if (!player) {
        player = findPlayerByName(target);
    }

    // Player not found
    if (!player) {
        if (admin) {
            options.sendMessage(admin, 'Client not found');
        }
        return null;
    }

    // Check if ingame
    if (options.isIngame && !options.isIngame(player)) {
        if (admin) {
            options.sendMessage(admin, 'Client not found');
        }
        return null;
    }

    // Check if targeting self when not allowed
    if (!(flags & CMDTARGET_ALLOW_SELF) && player === admin) {
        if (admin) {
            options.sendMessage(admin, 'You cannot target yourself');
        }
        return null;
    }

    // Check immunity
    if (flags & CMDTARGET_OBEY_IMMUNITY) {
        const targetFlags = options.getUserFlags(player);
        const adminFlags = admin ? options.getUserFlags(admin) : 0;

        // Target has immunity and admin doesn't (or admin isn't targeting self)
        if ((targetFlags & ADMIN_IMMUNITY) &&
            !(adminFlags & ADMIN_IMMUNITY) &&
            player !== admin) {
            if (admin) {
                const name = player.netname || 'Unknown';
                options.sendMessage(admin, `Client "${name}" has immunity`);
            }
            return null;
        }
    }

    // Check alive
    if ((flags & CMDTARGET_ONLY_ALIVE) && !isAlive(player)) {
        if (admin) {
            const name = player.netname || 'Unknown';
            options.sendMessage(admin, `That action can't be performed on dead client "${name}"`);
        }
        return null;
    }

    // Check bot
    if ((flags & CMDTARGET_NO_BOTS) && isBot(player)) {
        if (admin) {
            const name = player.netname || 'Unknown';
            options.sendMessage(admin, `That action can't be performed on bot "${name}"`);
        }
        return null;
    }

    return player;
}

// ============================================================================
// IP Validation
// ============================================================================

/**
 * Check if a string is a valid IP address
 */
export function isValidIp(ip: string): boolean {
    const parts = ip.split('.');
    if (parts.length !== 4) return false;

    for (const part of parts) {
        if (part.length === 0 || part.length > 3) return false;
        const num = parseInt(part);
        if (isNaN(num) || num < 0 || num > 255) return false;
    }
    return true;
}

// ============================================================================
// Message Utilities
// ============================================================================

/**
 * Send message to client console
 */
export function sendClientMessage(entity: nodemod.Entity, message: string): void {
    nodemod.eng.clientPrintf(entity, nodemod.PRINT_TYPE.print_console, `* ${message}\n`);
}

/**
 * Send message to entity or log to console
 */
export function sendMessage(entity: nodemod.Entity | null, message: string): void {
    if (entity) {
        nodemod.eng.clientPrintf(entity, nodemod.PRINT_TYPE.print_console, `${message}\n`);
    } else {
        console.log(message);
    }
}

// ============================================================================
// Command Parsing
// ============================================================================

/**
 * Parse a command string into arguments, respecting quoted strings.
 * Equivalent to parse_argN from amxmisc.inc
 *
 * @example
 * parseCommand('say "hello world"') // ['say', 'hello world']
 * parseCommand('kick player reason here') // ['kick', 'player', 'reason', 'here']
 */
export function parseCommand(text: string): string[] {
    const args: string[] = [];
    let current = '';
    let inQuotes = false;

    for (const char of text) {
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

// ============================================================================
// Player Name Utilities
// ============================================================================

/**
 * Get player name, with fallback to 'Unknown'
 */
export function getPlayerName(entity: nodemod.Entity): string {
    return entity.netname || 'Unknown';
}

/**
 * Get admin/entity name - returns 'CONSOLE' for null entity
 */
export function getAdminName(entity: nodemod.Entity | null): string {
    return entity ? (entity.netname || 'Unknown') : 'CONSOLE';
}

// ============================================================================
// Time Utilities
// ============================================================================

// CVAR wrapper for mp_timelimit (module-level for utility functions)
const mpTimelimitWrapper = nodemodCore.cvar.wrap('mp_timelimit');

/**
 * Tracks the gpGlobals->time value when the current map started.
 * This is equivalent to g_game_timeleft in AMXX.
 */
let g_game_timeleft: number = -1; // -1 = not initialized
let g_time_initialized: boolean = false;

/**
 * Initialize time tracking by hooking into dllServerActivate.
 * This should be called once when the admin system loads.
 */
export function initTimeTracking(): void {
    if (g_time_initialized) return;
    g_time_initialized = true;

    // Hook into server activate to track map start time
    nodemod.on('dllServerActivate', () => {
        g_game_timeleft = nodemod.eng.time() || 0;
    });

    // Set initial value for the current map (plugin loaded mid-map)
    g_game_timeleft = nodemod.eng.time() || 0;
}

/**
 * Get current game time in seconds
 * Equivalent to get_gametime() from amxmodx
 */
export function getGameTime(): number {
    return nodemod.eng.time() || 0;
}

/**
 * Get time left on the map in seconds
 * Equivalent to get_timeleft() from amxmodx
 *
 * Uses the same formula as AMXX:
 * timeleft = (g_game_timeleft + mp_timelimit * 60) - gpGlobals->time
 *
 * Where g_game_timeleft is the gpGlobals->time value when the map started.
 *
 * @returns Time left in seconds, or 0 if no time limit or time has expired
 */
export function getTimeLeft(): number {
    const timeLimit = mpTimelimitWrapper.float || 0;

    if (timeLimit <= 0) {
        return 0;
    }

    const currentTime = nodemod.eng.time() || 0;

    // If not initialized, use simple formula (assumes time resets on map change)
    if (g_game_timeleft < 0) {
        return Math.max(0, Math.floor((timeLimit * 60) - currentTime));
    }

    // Formula: (g_game_timeleft + timelimit_in_seconds) - current_time
    const result = (g_game_timeleft + timeLimit * 60) - currentTime;

    return Math.max(0, Math.floor(result));
}

// ============================================================================
// Chat/Console Utilities
// ============================================================================

/**
 * Send a chat message to a specific player or all players
 *
 * @param message The message to send
 * @param target The target entity, or null for all players
 * @param getPlayers Function to get player list (for all-player broadcast)
 */
export function sendChatToTarget(
    message: string,
    target: nodemod.Entity | null,
    getPlayers: () => nodemod.Entity[]
): void {
    if (target) {
        nodemodCore.util.sendChat(message, target);
    } else {
        for (const player of getPlayers()) {
            nodemodCore.util.sendChat(message, player);
        }
    }
}

/**
 * Send a HUD message to a specific player or all players
 *
 * @param message The message to display
 * @param target The target entity, or null for all players
 * @param options HUD display options
 * @param getPlayers Function to get player list (for all-player broadcast)
 */
export function sendHudToTarget(
    message: string,
    target: nodemod.Entity | null,
    options: {
        channel: number;
        x: number;
        y: number;
        r1: number;
        g1: number;
        b1: number;
        a1?: number;
        fadeinTime?: number;
        fadeoutTime?: number;
        holdTime?: number;
    },
    getPlayers: () => nodemod.Entity[]
): void {
    const fullOptions = {
        a1: 255,
        fadeinTime: 128, // 0.5s * 256
        fadeoutTime: 38, // 0.15s * 256
        holdTime: 1536,  // 6.0s * 256
        ...options
    };

    if (target) {
        nodemodCore.util.showHudText(target, message, fullOptions);
    } else {
        for (const player of getPlayers()) {
            nodemodCore.util.showHudText(player, message, fullOptions);
        }
    }
}

// ============================================================================
// Unified Admin System Helpers (lazy import to avoid circular dependency)
// ============================================================================

/**
 * Lazy getter for adminSystem to avoid circular dependency.
 * This is called at runtime, not import time.
 */
function getAdminSystem(): typeof import('./admin').adminSystem {
    return require('./admin').adminSystem;
}

/**
 * Get standard show activity options using adminSystem.
 * Uses lazy import to avoid circular dependency.
 */
export function getShowActivityOptions(): ShowActivityOptions {
    const amxShowActivity = nodemodCore.cvar.wrap('amx_show_activity');
    return {
        getShowActivityLevel: () => amxShowActivity?.int || 2,
        hasAdminAccess: (e) => getAdminSystem().hasAccess(e, ADMIN_ADMIN)
    };
}

/**
 * Send chat message to target entity, or all players if null.
 * Uses lazy import to avoid circular dependency with admin.ts.
 */
export function sendChat(target: nodemod.Entity | null, message: string): void {
    sendChatToTarget(message, target, () => getAdminSystem().getPlayers());
}

/**
 * Show admin activity to all players based on amx_show_activity setting.
 * Unified version that uses lazy import to avoid circular dependency.
 *
 * @param adminEntity The admin performing the action (null for console)
 * @param message The activity message
 */
export function showActivityToAll(adminEntity: nodemod.Entity | null, message: string): void {
    const adminName = getAdminName(adminEntity);
    const options = getShowActivityOptions();

    for (const player of getAdminSystem().getPlayers({ excludeBots: true })) {
        showActivity(player, adminName, message, options);
    }
}

// ============================================================================
// Menu Color Utilities
// ============================================================================

/**
 * AMXX Menu Color Codes
 * These are interpreted by the HL client when rendering menus
 */
export const MenuColors = {
    YELLOW: '\\y',   // Yellow - used for titles and highlights
    WHITE: '\\w',    // White - normal text
    GREY: '\\d',     // Grey/Dark - disabled items
    RED: '\\r',      // Red
    RIGHT: '\\R',    // Right-align following text
} as const;

/**
 * Menu formatter interface matching nodemodCore.menu format
 */
export interface MenuFormatters {
    title?: (title: string) => string;
    item?: (item: { name: string; disabled?: boolean }, index: number, isDisabled: boolean) => string;
    pagination?: (current: number, total: number) => string;
    divider?: () => string;
}

/**
 * Colored menu formatters matching AMXX style
 * Use these with nodemodCore.menu.show({ formatters: coloredMenuFormatters })
 */
export const coloredMenuFormatters: MenuFormatters = {
    title: (title: string) => `${MenuColors.YELLOW}${title}${MenuColors.WHITE}`,
    item: (item: { name: string; disabled?: boolean }, index: number, isDisabled: boolean) => {
        if (isDisabled) {
            return `${MenuColors.GREY}${index + 1}. ${item.name}${MenuColors.WHITE}`;
        }
        return `${index + 1}. ${item.name}`;
    },
    pagination: (current: number, total: number) =>
        `${MenuColors.YELLOW}Page ${current}/${total}${MenuColors.WHITE}`,
    divider: () => '',
};

/**
 * Get colored formatters with custom title color
 */
export function getColoredFormatters(options?: {
    titleColor?: string;
    disabledColor?: string;
}): MenuFormatters {
    const titleColor = options?.titleColor ?? MenuColors.YELLOW;
    const disabledColor = options?.disabledColor ?? MenuColors.GREY;

    return {
        title: (title: string) => `${titleColor}${title}${MenuColors.WHITE}`,
        item: (item: { name: string; disabled?: boolean }, index: number, isDisabled: boolean) => {
            if (isDisabled) {
                return `${disabledColor}${index + 1}. ${item.name}${MenuColors.WHITE}`;
            }
            return `${index + 1}. ${item.name}`;
        },
        pagination: (current: number, total: number) =>
            `${titleColor}Page ${current}/${total}${MenuColors.WHITE}`,
        divider: () => '',
    };
}

// ============================================================================
// Map Utilities
// ============================================================================

/**
 * Check if a map exists and is valid
 * Tries engine function first, falls back to file system check
 */
export function isMapValid(mapName: string): boolean {
    try {
        return !!nodemod.eng.isMapValid(mapName);
    } catch (e) {
        // Fallback: check if map file exists
        const mapPath = path.join(getGameDir(), 'maps', `${mapName}.bsp`);
        return fs.existsSync(mapPath);
    }
}

/**
 * Options for loading maps from config files
 */
export interface LoadMapsOptions {
    /** Maps to skip (e.g., current map, last map) - compared case-insensitive */
    skipMaps?: string[];
}

/**
 * Load maps from a single file
 * Handles comments, .bsp extensions, and whitespace
 *
 * @param filePath Path to the maps file
 * @param options Optional settings
 * @returns Array of map names, or empty array if file doesn't exist or has no maps
 */
export function loadMapsFromFile(filePath: string, options?: LoadMapsOptions): string[] {
    if (!fs.existsSync(filePath)) {
        return [];
    }

    const skipMaps = (options?.skipMaps || []).map(m => m.toLowerCase());
    const maps: string[] = [];

    try {
        const content = fs.readFileSync(filePath, 'utf-8');
        const lines = content.split('\n');

        for (const line of lines) {
            let mapName = line.trim().split(/\s+/)[0] || '';

            // Skip comments and empty lines
            if (!mapName || mapName.startsWith(';') || mapName.startsWith('//')) {
                continue;
            }

            // Also skip if first char is not alphanumeric (covers other comment styles)
            if (!/^[a-zA-Z0-9]/.test(mapName)) {
                continue;
            }

            // Remove .bsp extension if present
            if (mapName.toLowerCase().endsWith('.bsp')) {
                mapName = mapName.slice(0, -4);
            }

            // Skip if in skipMaps list
            if (skipMaps.includes(mapName.toLowerCase())) {
                continue;
            }

            maps.push(mapName);
        }
    } catch (e) {
        // Return empty array on error
    }

    return maps;
}

/**
 * Load maps from standard locations with fallback
 * Tries: maps.ini -> mapcyclefile cvar -> mapcycle.txt
 *
 * @param options Optional settings (skipMaps, etc.)
 * @returns Array of map names from the first file that has entries
 */
export function loadMapList(options?: LoadMapsOptions): string[] {
    const configsDir = getConfigsDir();
    const gameDir = getGameDir();
    const mapcycleFile = nodemodCore.cvar.getString('mapcyclefile') || 'mapcycle.txt';

    const filesToTry = [
        path.join(configsDir, 'maps.ini'),
        path.join(gameDir, mapcycleFile),
        path.join(gameDir, 'mapcycle.txt')
    ];

    // Remove duplicates (if mapcyclefile is 'mapcycle.txt')
    const uniqueFiles = [...new Set(filesToTry)];

    for (const filePath of uniqueFiles) {
        const maps = loadMapsFromFile(filePath, options);
        if (maps.length > 0) {
            return maps;
        }
    }

    return [];
}

// ============================================================================
// Message Utilities
// ============================================================================

/**
 * Send SVC_INTERMISSION message to all clients
 * This freezes player movement during map transitions
 * Equivalent to AMXX: message_begin(MSG_ALL, SVC_INTERMISSION); message_end();
 */
export function sendIntermission(): void {
    try {
        nodemod.eng.messageBegin(nodemod.MSG_DEST.ALL, nodemod.MSG_TYPE.INTERMISSION, [0, 0, 0], null);
        nodemod.eng.messageEnd();
    } catch (e) {
        // Silently fail if message API not available
    }
}
