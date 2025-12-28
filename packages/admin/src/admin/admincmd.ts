// Admin Commands Plugin
// Converted from AMX Mod X admincmd.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import {
    ADMIN_KICK,
    ADMIN_BAN,
    ADMIN_SLAY,
    ADMIN_MAP,
    ADMIN_CVAR,
    ADMIN_CFG,
    ADMIN_RCON,
    ADMIN_ADMIN,
    ADMIN_PASSWORD,
    ADMIN_IMMUNITY,
    ADMIN_RESERVATION,
    CMDTARGET_OBEY_IMMUNITY,
    CMDTARGET_ALLOW_SELF,
    CMDTARGET_ONLY_ALIVE,
    CMDTARGET_NO_BOTS,
    getFlags
} from './constants';

import localization from './localization';
import * as utils from './utils';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import { BasePlugin } from './baseplugin';
import fs from 'fs';
import path from 'path';

const cvar = nodemodCore.cvar;

// Old connection queue size
const OLD_CONNECTION_QUEUE = 10;

interface DisconnectedPlayer {
    name: string;
    steamId: string;
    ip: string;
    access: number;
}

class AdminCommands extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Commands',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Basic admin console commands'
    };

    // Old connection queue for tracking recently disconnected players
    private disconnectionQueue: DisconnectedPlayer[] = [];
    private queueTracker = 0;

    // Pause state
    private pausable: any;
    private rconPassword: any;
    private paused = false;
    private pauseAllowed = false;
    private pauseInitiator: nodemod.Entity | null = null;
    private savedPausable = 0;

    // Protected CVARs that require RCON access to modify
    private protectedCvars: Set<string> = new Set();

    constructor(pluginName: string) {
        super(pluginName);

        // Load localization dictionaries
        localization.loadDictionary('adminhelp');

        // Get CVAR pointers
        this.pausable = cvar.wrap('pausable');
        this.rconPassword = cvar.wrap('rcon_password');

        // Register commands
        this.registerCommands();

        // Setup event handlers
        this.setupEventHandlers();

        // Initialize protected CVARs (equivalent to plugin_cfg in AMXX)
        this.initProtectedCvars();
    }

    /**
     * Initialize protected CVARs that require RCON access to modify
     * Equivalent to plugin_cfg() in admincmd.sma
     */
    private initProtectedCvars() {
        const cvarsToProtect = [
            'rcon_password',
            'amx_show_activity',
            'amx_mode',
            'amx_password_field',
            'amx_default_access',
            'amx_reserved_slots',
            'amx_reservation',
            'amx_sql_table',
            'amx_sql_host',
            'amx_sql_user',
            'amx_sql_pass',
            'amx_sql_db',
            'amx_sql_type'
        ];

        for (const cvarName of cvarsToProtect) {
            this.protectedCvars.add(cvarName.toLowerCase());
        }
    }

    /**
     * Add a CVAR to the protected list (called by amx_cvar add)
     */
    addProtectedCvar(cvarName: string): void {
        this.protectedCvars.add(cvarName.toLowerCase());
    }

    /**
     * Check if a CVAR is protected
     */
    isProtectedCvar(cvarName: string): boolean {
        return this.protectedCvars.has(cvarName.toLowerCase());
    }

    /**
     * Get localized string for a player
     */

    private registerCommands() {
        this.registerCommand('amx_kick', ADMIN_KICK, '<name or #userid> [reason]', (entity, args) => {
            this.cmdKick(entity, args);
        });

        this.registerCommand('amx_ban', ADMIN_BAN, '<name or #userid> <minutes> [reason]', (entity, args) => {
            this.cmdBan(entity, args, false);
        });

        this.registerCommand('amx_banip', ADMIN_BAN, '<name or #userid> <minutes> [reason]', (entity, args) => {
            this.cmdBan(entity, args, true);
        });

        this.registerCommand('amx_addban', ADMIN_BAN, '<authid or ip> <minutes> [reason]', (entity, args) => {
            this.cmdAddBan(entity, args);
        });

        this.registerCommand('amx_unban', ADMIN_BAN, '<authid or ip>', (entity, args) => {
            this.cmdUnban(entity, args);
        });

        this.registerCommand('amx_slay', ADMIN_SLAY, '<name or #userid>', (entity, args) => {
            this.cmdSlay(entity, args);
        });

        this.registerCommand('amx_slap', ADMIN_SLAY, '<name or #userid> [damage]', (entity, args) => {
            this.cmdSlap(entity, args);
        });

        this.registerCommand('amx_map', ADMIN_MAP, '<mapname>', (entity, args) => {
            this.cmdMap(entity, args);
        });

        this.registerCommand('amx_cvar', ADMIN_CVAR, '<cvar> [value]', (entity, args) => {
            this.cmdCvar(entity, args);
        });

        this.registerCommand('amx_cfg', ADMIN_CFG, '<filename>', (entity, args) => {
            this.cmdCfg(entity, args);
        });

        this.registerCommand('amx_nick', ADMIN_SLAY, '<name or #userid> <new nick>', (entity, args) => {
            this.cmdNick(entity, args);
        });

        this.registerCommand('amx_leave', ADMIN_KICK, '<tag> [tag] [tag] [tag]', (entity, args) => {
            this.cmdLeave(entity, args);
        });

        this.registerCommand('amx_pause', ADMIN_CVAR, '- pause or unpause the game', (entity, args) => {
            this.cmdPause(entity);
        });

        this.registerCommand('amx_who', ADMIN_ADMIN, '- displays who is on server', (entity, args) => {
            this.cmdWho(entity);
        });

        this.registerCommand('amx_rcon', ADMIN_RCON, '<command line>', (entity, args) => {
            this.cmdRcon(entity, args);
        });

        this.registerCommand('amx_showrcon', ADMIN_RCON, '<command line>', (entity, args) => {
            this.cmdShowRcon(entity, args);
        });

        this.registerCommand('amx_last', ADMIN_BAN, '- list recently disconnected clients', (entity, args) => {
            this.cmdLast(entity);
        });

        this.registerCommand('amx_plugins', ADMIN_ADMIN, '[page] - list loaded plugins', (entity, args) => {
            this.cmdPlugins(entity, args);
        });

        this.registerCommand('amx_modules', ADMIN_ADMIN, '- list loaded modules', (entity, args) => {
            this.cmdModules(entity);
        });

        // Client-only command (no help entry)
        nodemodCore.cmd.registerClient('pauseAck', (entity) => {
            this.cmdPauseAck();
        });
    }

    private setupEventHandlers() {
        // Track disconnections for amx_last/amx_addban
        nodemod.on('dllClientDisconnect', (entity: nodemod.Entity) => {
            if (!utils.isBot(entity)) {
                this.insertDisconnectionInfo(entity);
            }
        });
    }

    /**
     * Insert player info into disconnection queue
     */
    private insertDisconnectionInfo(entity: nodemod.Entity) {
        const steamId = nodemod.eng.getPlayerAuthId(entity) || '';
        const name = entity.netname || 'Unknown';
        const index = nodemod.eng.indexOfEdict(entity);
        const ip = utils.getPlayerIP(entity);
        const access = adminSystem.getUserFlags(entity);

        // Check if this is the same as the last entry (update instead of insert)
        if (this.disconnectionQueue.length > 0) {
            const lastIndex = (this.queueTracker - 1 + OLD_CONNECTION_QUEUE) % OLD_CONNECTION_QUEUE;
            const last = this.disconnectionQueue[lastIndex];
            if (last && last.steamId === steamId && last.ip === ip) {
                last.name = name;
                last.access = access;
                return;
            }
        }

        // Insert new entry
        const entry: DisconnectedPlayer = {
            name,
            steamId,
            ip,
            access
        };

        if (this.disconnectionQueue.length < OLD_CONNECTION_QUEUE) {
            this.disconnectionQueue.push(entry);
        } else {
            this.disconnectionQueue[this.queueTracker] = entry;
            this.queueTracker = (this.queueTracker + 1) % OLD_CONNECTION_QUEUE;
        }
    }

    /**
     * Get info from disconnection queue by index
     */
    private getDisconnectionInfo(index: number): DisconnectedPlayer | null {
        if (index >= this.disconnectionQueue.length) {
            return null;
        }
        const target = (this.queueTracker + index) % this.disconnectionQueue.length;
        return this.disconnectionQueue[target];
    }






    /**
     * Show admin activity to players based on amx_show_activity setting
     */

    // ============================================================================
    // Command Implementations
    // ============================================================================

    /**
     * amx_kick <name or #userid> [reason]
     */
    private cmdKick(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_KICK)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_KICK'));
            return;
        }

        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_ALLOW_SELF);
        if (!target) return;

        const reason = args.slice(1).join(' ');
        const adminName = this.getAdminName(entity);
        const targetName = target.netname || 'Unknown';
        const targetUserId = nodemod.eng.getPlayerUserId(target);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Kick: "${adminName}<${adminUserId}><${adminAuthId}><>" kick "${targetName}<${targetUserId}><${targetAuthId}><>" (reason "${reason}")`);
        }

        // Show activity
        if (reason) {
            this.showActivity(entity, this.getLang(null, 'ADMIN_KICKED_REASON', adminName, targetName, reason));
        } else {
            this.showActivity(entity, this.getLang(null, 'ADMIN_KICKED', adminName, targetName));
        }

        // Kick the player
        if (utils.isBot(target)) {
            nodemod.eng.serverCommand(`kick #${targetUserId}\n`);
        } else {
            if (reason) {
                nodemod.eng.serverCommand(`kick #${targetUserId} "${reason}"\n`);
            } else {
                nodemod.eng.serverCommand(`kick #${targetUserId}\n`);
            }
        }

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CLIENT_KICKED', targetName)}`);
    }

    /**
     * amx_ban / amx_banip <name or #userid> <minutes> [reason]
     */
    private cmdBan(entity: nodemod.Entity | null, args: string[], byIp: boolean) {
        if (!adminSystem.cmdAccess(entity, ADMIN_BAN)) return;

        if (args.length < 2) {
            this.sendConsole(entity, this.getLang(entity, byIp ? 'USAGE_BANIP' : 'USAGE_BAN'));
            return;
        }

        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_NO_BOTS | CMDTARGET_ALLOW_SELF);
        if (!target) return;

        const minutes = args[1];
        const reason = args.slice(2).join(' ');
        const nMinutes = parseInt(minutes) || 0;

        const adminName = this.getAdminName(entity);
        const targetName = target.netname || 'Unknown';
        const targetUserId = nodemod.eng.getPlayerUserId(target);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
        const targetIp = utils.getPlayerIP(target);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Ban: "${adminName}<${adminUserId}><${adminAuthId}><>" ban and kick "${targetName}<${targetUserId}><${targetAuthId}><>" (minutes "${minutes}") (reason "${reason}")`);
        }

        // Format ban duration message
        const durationMsg = nMinutes > 0
            ? this.getLang(null, 'FOR_MIN', minutes)
            : this.getLang(null, 'PERM');
        const bannedMsg = this.getLang(null, 'BANNED');

        // Build kick message
        let kickReason: string;
        if (reason) {
            kickReason = `${reason} (${bannedMsg} ${durationMsg})`;
        } else {
            kickReason = `${bannedMsg} ${durationMsg}`;
        }

        // Execute ban
        if (byIp) {
            nodemod.eng.serverCommand(`kick #${targetUserId} "${kickReason}"\n`);
            nodemod.eng.serverCommand(`addip "${minutes}" "${targetIp}"\n`);
            nodemod.eng.serverCommand(`writeip\n`);
        } else {
            nodemod.eng.serverCommand(`kick #${targetUserId} "${kickReason}"\n`);
            nodemod.eng.serverCommand(`banid ${minutes} ${targetAuthId}\n`);
            nodemod.eng.serverCommand(`writeid\n`);
        }

        // Show activity
        let activityMsg = `${this.getLang(null, 'BAN')} ${targetName} ${durationMsg}`;
        if (reason) {
            activityMsg += ` (${this.getLang(null, 'REASON')}: ${reason})`;
        }
        this.showActivity(entity, activityMsg);

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CLIENT_BANNED', targetName)}`);
    }

    /**
     * amx_addban <"authid" or ip> <minutes> [reason]
     * Bans a disconnected player by SteamID or IP
     */
    private cmdAddBan(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_BAN)) return;

        if (args.length < 2) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_ADDBAN'));
            return;
        }

        const target = args[0];
        const minutes = args[1];
        const reason = args.slice(2).join(' ');

        // Check for invalid steam IDs
        const invalidIds = ['STEAM_ID_PENDING', 'STEAM_ID_LAN', 'HLTV', '4294967295', 'VALVE_ID_LAN', 'VALVE_ID_PENDING'];
        if (invalidIds.some(id => target.toUpperCase() === id)) {
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CANNOT_BAN', target)}`);
            return;
        }

        const isIp = target.includes('.');
        const hasRcon = entity ? adminSystem.hasAccess(entity, ADMIN_RCON) : true;

        // If admin doesn't have RCON, check immunity of disconnected players
        if (!hasRcon) {
            let canBan = false;

            for (let i = 0; i < this.disconnectionQueue.length; i++) {
                const info = this.getDisconnectionInfo(i);
                if (!info) continue;

                const match = isIp ? (info.ip === target) : (info.steamId === target);
                if (match) {
                    if (info.access & ADMIN_IMMUNITY) {
                        this.sendConsole(entity, `[AMXX] ${target} : ${this.getLang(entity, 'CLIENT_IMM', info.name)}`);
                        return;
                    }
                    canBan = true;
                    break;
                }
            }

            if (!canBan) {
                this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'ADDBAN_ONLY_RECENT')}`);
                return;
            }
        }

        // Execute ban
        const adminName = this.getAdminName(entity);

        if (isIp) {
            nodemod.eng.serverCommand(`addip "${minutes}" "${target}"\n`);
            nodemod.eng.serverCommand(`writeip\n`);
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'IP_ADDED', target)}`);
        } else {
            nodemod.eng.serverCommand(`banid ${minutes} ${target}\n`);
            nodemod.eng.serverCommand(`writeid\n`);
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'AUTHID_ADDED', target)}`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_ADDBAN', adminName, target));

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" ban "${target}" (minutes "${minutes}") (reason "${reason}")`);
        }
    }

    /**
     * amx_unban <"authid" or ip>
     */
    private cmdUnban(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_BAN)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_UNBAN'));
            return;
        }

        const target = args[0];
        const adminName = this.getAdminName(entity);
        const isIp = target.includes('.');

        if (isIp) {
            nodemod.eng.serverCommand(`removeip "${target}"\n`);
            nodemod.eng.serverCommand(`writeip\n`);
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'IP_REMOVED', target)}`);
        } else {
            nodemod.eng.serverCommand(`removeid ${target}\n`);
            nodemod.eng.serverCommand(`writeid\n`);
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'AUTHID_REMOVED', target)}`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_UNBAN', adminName, target));

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" unban "${target}"`);
        }
    }

    /**
     * amx_slay <name or #userid>
     */
    private cmdSlay(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_SLAY)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_SLAY'));
            return;
        }

        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_ALLOW_SELF | CMDTARGET_ONLY_ALIVE);
        if (!target) return;

        const adminName = this.getAdminName(entity);
        const targetName = target.netname || 'Unknown';

        // Kill the player using the game's kill function
        nodemod.dll.clientKill(target);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
            const targetUserId = nodemod.eng.getPlayerUserId(target);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" slay "${targetName}<${targetUserId}><${targetAuthId}><>"`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_SLAYED', adminName, targetName));
        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CLIENT_SLAYED', targetName)}`);
    }

    /**
     * amx_slap <name or #userid> [damage]
     */
    private cmdSlap(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_SLAY)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_SLAP'));
            return;
        }

        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_ALLOW_SELF | CMDTARGET_ONLY_ALIVE);
        if (!target) return;

        const damage = args.length > 1 ? parseInt(args[1]) || 0 : 0;
        const adminName = this.getAdminName(entity);
        const targetName = target.netname || 'Unknown';

        // Apply random velocity (knockback)
        const velocity = target.velocity || [0, 0, 0];
        velocity[0] += (Math.random() - 0.5) * 300;
        velocity[1] += (Math.random() - 0.5) * 300;
        velocity[2] += 200 + Math.random() * 100;
        target.velocity = velocity;

        // Apply damage - kill player if lethal
        if (damage > 0) {
            if (target.health <= damage) {
                nodemod.dll.clientKill(target);
            } else {
                target.health -= damage;
            }
        }

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
            const targetUserId = nodemod.eng.getPlayerUserId(target);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" slap with ${damage} damage "${targetName}<${targetUserId}><${targetAuthId}><>"`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_SLAPPED', adminName, targetName, damage));
        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CLIENT_SLAPPED', targetName, damage)}`);
    }

    /**
     * amx_map <mapname>
     */
    private cmdMap(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_MAP)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_MAP'));
            return;
        }

        const mapName = args[0];
        const adminName = this.getAdminName(entity);

        // Check if map is valid
        if (!nodemod.eng.isMapValid(mapName)) {
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'MAP_NOT_FOUND')}`);
            return;
        }

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" changelevel "${mapName}"`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_MAP', adminName, mapName));

        // Send SVC_INTERMISSION to freeze clients during map change
        utils.sendIntermission();

        // Change map after delay
        setTimeout(() => {
            nodemod.eng.serverCommand(`changelevel ${mapName}\n`);
        }, 2000);
    }

    /**
     * amx_cvar <cvar> [value]
     */
    private cmdCvar(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CVAR)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_CVAR'));
            return;
        }

        const cvarName = args[0];
        const adminName = this.getAdminName(entity);

        // Handle "add" subcommand (RCON only)
        if (cvarName.toLowerCase() === 'add' && args.length >= 2) {
            if (!adminSystem.hasAccess(entity, ADMIN_RCON)) {
                this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'NO_ACCESS')}`);
                return;
            }
            // Add CVAR to protected list (requires RCON access to modify)
            const targetCvar = args[1];
            this.addProtectedCvar(targetCvar);
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CVAR_PROTECTED_ADDED', targetCvar)}`);
            return;
        }

        // Check if CVAR exists
        const cvarWrapper = cvar.wrap(cvarName);
        if (!cvarWrapper) {
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'UNKNOWN_CVAR', cvarName)}`);
            return;
        }

        // Check for protected CVARs (need RCON access)
        if (this.isProtectedCvar(cvarName) && !adminSystem.hasAccess(entity, ADMIN_RCON)) {
            // Exception: sv_password can be changed with ADMIN_PASSWORD
            if (cvarName.toLowerCase() === 'sv_password' && adminSystem.hasAccess(entity, ADMIN_PASSWORD)) {
                // Allow
            } else {
                this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CVAR_NO_ACC')}`);
                return;
            }
        }

        // If no value provided, show current value
        if (args.length < 2) {
            const currentValue = cvarWrapper.value || '';
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CVAR_IS', cvarName, currentValue)}`);
            return;
        }

        const newValue = args.slice(1).join(' ');

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" set cvar (name "${cvarName}") (value "${newValue}")`);
        }

        // Set the CVAR
        cvar.setString(cvarName, newValue);

        // Show activity (hide protected values)
        const displayValue = this.isProtectedCvar(cvarName)
            ? `*** ${this.getLang(null, 'PROTECTED')} ***`
            : newValue;

        this.showActivity(entity, this.getLang(null, 'SET_CVAR_TO', cvarName, displayValue));
        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CVAR_CHANGED', cvarName, newValue)}`);
    }

    /**
     * amx_cfg <filename>
     */
    private cmdCfg(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_CFG'));
            return;
        }

        const filename = args[0];
        const adminName = this.getAdminName(entity);

        // Check if file exists
        const configsDir = path.join(process.cwd(), '..', 'configs');
        const filePath = path.join(configsDir, filename);

        if (!fs.existsSync(filePath)) {
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'FILE_NOT_FOUND', filename)}`);
            return;
        }

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" execute cfg (file "${filename}")`);
        }

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'EXECUTING_FILE', filename)}`);
        nodemod.eng.serverCommand(`exec ${filePath}\n`);

        this.showActivity(entity, this.getLang(null, 'ADMIN_CFG', adminName, filename));
    }

    /**
     * amx_nick <name or #userid> <new nick>
     */
    private cmdNick(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_SLAY)) return;

        if (args.length < 2) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_NICK'));
            return;
        }

        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_ALLOW_SELF);
        if (!target) return;

        const newNick = args.slice(1).join(' ');
        const adminName = this.getAdminName(entity);
        const oldName = target.netname || 'Unknown';

        // Change the player's name
        nodemod.eng.clientCommand(target, `name "${newNick}"\n`);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
            const targetUserId = nodemod.eng.getPlayerUserId(target);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" change nick to "${newNick}" "${oldName}<${targetUserId}><${targetAuthId}><>"`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_NICK', adminName, oldName, newNick));
        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'CHANGED_NICK', oldName, newNick)}`);
    }

    /**
     * amx_leave <tag> [tag] [tag] [tag]
     * Kick all players NOT matching any of the given tags
     */
    private cmdLeave(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_KICK)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_LEAVE'));
            return;
        }

        const tags = args.slice(0, 4); // Max 4 tags
        const adminName = this.getAdminName(entity);
        let kickCount = 0;

        for (const player of adminSystem.getPlayers()) {
            const playerName = player.netname || '';

            // Check if player name contains any of the tags
            const hasTag = tags.some(tag => playerName.toLowerCase().includes(tag.toLowerCase()));
            if (hasTag) {
                this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'SKIP_MATCH', playerName, tags.find(t => playerName.toLowerCase().includes(t.toLowerCase())))}`);
                continue;
            }

            // Check immunity
            if (adminSystem.getUserFlags(player) & ADMIN_IMMUNITY) {
                this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'SKIP_IMM', playerName)}`);
                continue;
            }

            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'KICK_PL', playerName)}`);

            const userId = nodemod.eng.getPlayerUserId(player);
            if (utils.isBot(player)) {
                nodemod.eng.serverCommand(`kick #${userId}\n`);
            } else {
                const reason = this.getLang(player, 'YOU_DROPPED');
                nodemod.eng.serverCommand(`kick #${userId} "${reason}"\n`);
            }
            kickCount++;
        }

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'KICKED_CLIENTS', kickCount)}`);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Kick: "${adminName}<${adminUserId}><${adminAuthId}><>" leave some group (tags: ${tags.join(', ')})`);
        }

        this.showActivity(entity, this.getLang(null, 'ADMIN_LEAVE', adminName, tags.join(', ')));
    }

    /**
     * amx_pause - Pause/unpause the game
     */
    private cmdPause(entity: nodemod.Entity | null) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CVAR)) return;

        const adminName = this.getAdminName(entity);

        // Store current pausable value
        if (this.pausable) {
            this.savedPausable = this.pausable.float || 0;
        }

        // Find a human player to send the pause command to
        let slayer: nodemod.Entity | null = entity;
        if (!slayer) {
            const humanPlayers = adminSystem.getPlayers({ excludeBots: true });
            if (humanPlayers.length > 0) {
                slayer = humanPlayers[0];
            }
        }

        if (!slayer) {
            this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'UNABLE_PAUSE')}`);
            return;
        }

        // Set pausable to 1 and allow pause
        cvar.setFloat('pausable', 1.0);
        this.pauseAllowed = true;
        this.pauseInitiator = entity;

        // Send pause command to player
        nodemod.eng.clientCommand(slayer, 'pause\n');
        nodemod.eng.clientCommand(slayer, 'pauseAck\n');

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" ${this.paused ? 'unpause' : 'pause'} server`);
        }

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, this.paused ? 'UNPAUSING' : 'PAUSING')}`);
        this.showActivity(entity, `${this.getLang(null, this.paused ? 'UNPAUSE' : 'PAUSE')} server`);
    }

    /**
     * pauseAck client command handler
     */
    private cmdPauseAck() {
        if (!this.pauseAllowed) return;

        // Restore pausable value
        cvar.setFloat('pausable', this.savedPausable);

        const statusMsg = this.paused ? 'UNPAUSED' : 'PAUSED';
        this.sendConsole(this.pauseInitiator, `[AMXX] Server ${this.getLang(this.pauseInitiator, statusMsg)}`);

        this.pauseAllowed = false;
        this.paused = !this.paused;
    }

    /**
     * amx_who - Display connected players with access info
     */
    private cmdWho(entity: nodemod.Entity | null) {
        if (!adminSystem.cmdAccess(entity, ADMIN_ADMIN)) return;

        const adminName = this.getAdminName(entity);

        this.sendConsole(entity, '');
        this.sendConsole(entity, `${this.getLang(entity, 'CLIENTS_ON_SERVER')}:`);
        this.sendConsole(entity, ` #  nick                 authid                                   userid immu res  access`);

        let playerCount = 0;
        for (const player of adminSystem.getPlayers()) {
            const playerName = player.netname || 'Unknown';
            const authId = nodemod.eng.getPlayerAuthId(player) || '';
            const userId = nodemod.eng.getPlayerUserId(player);
            const flags = adminSystem.getUserFlags(player);
            const flagsStr = getFlags(flags);
            const hasImmunity = (flags & ADMIN_IMMUNITY) ? this.getLang(entity, 'YES') : this.getLang(entity, 'NO');
            const hasReservation = (flags & ADMIN_RESERVATION) ? this.getLang(entity, 'YES') : this.getLang(entity, 'NO');

            this.sendConsole(entity, `${String(nodemod.eng.indexOfEdict(player)).padStart(2)}  ${playerName.padEnd(20).substring(0, 20)} ${authId.padEnd(40).substring(0, 40)} ${String(userId).padEnd(6)} ${hasImmunity.padEnd(4)} ${hasReservation.padEnd(4)} ${flagsStr}`);
            playerCount++;
        }

        this.sendConsole(entity, `${this.getLang(entity, 'TOTAL_NUM', playerCount)}`);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" ask for players list`);
        }
    }

    /**
     * amx_rcon <command line>
     */
    private cmdRcon(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_RCON)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_RCON'));
            return;
        }

        const command = args.join(' ');
        const adminName = this.getAdminName(entity);

        // Log the action
        if (entity) {
            const adminAuthId = nodemod.eng.getPlayerAuthId(entity) || '';
            const adminUserId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" server console (cmdline "${command}")`);
        }

        this.sendConsole(entity, `[AMXX] ${this.getLang(entity, 'COM_SENT_SERVER', command)}`);
        nodemod.eng.serverCommand(`${command}\n`);
    }

    /**
     * amx_showrcon <command line>
     * Same as amx_rcon but also shows the output to the admin
     */
    private cmdShowRcon(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_RCON)) return;

        // If no rcon_password is set, just execute as normal rcon
        const rconPass = this.rconPassword?.value || '';
        if (!rconPass) {
            this.cmdRcon(entity, args);
            return;
        }

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_RCON'));
            return;
        }

        const command = args.join(' ');

        // Send rcon password and command to client
        if (entity) {
            nodemod.eng.clientCommand(entity, `rcon_password ${rconPass}\n`);
            nodemod.eng.clientCommand(entity, `rcon ${command}\n`);
        }
    }

    /**
     * amx_last - Display recently disconnected players
     */
    private cmdLast(entity: nodemod.Entity | null) {
        if (!adminSystem.cmdAccess(entity, ADMIN_BAN)) return;

        this.sendConsole(entity, `${'name'.padStart(19)} ${'authid'.padEnd(20)} ${'ip'.padEnd(15)} access`);

        for (let i = 0; i < this.disconnectionQueue.length; i++) {
            const info = this.getDisconnectionInfo(i);
            if (!info) continue;

            const flagsStr = getFlags(info.access);
            this.sendConsole(entity, `${info.name.padStart(19)} ${info.steamId.padEnd(20)} ${info.ip.padEnd(15)} ${flagsStr}`);
        }

        this.sendConsole(entity, `${this.disconnectionQueue.length} old connections saved.`);
    }

    /**
     * amx_plugins [page] - List loaded plugins with pagination
     */
    private cmdPlugins(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_ADMIN)) return;

        const PLUGINS_PER_PAGE = 10;
        const plugins = pluginLoader.getPlugins();
        const totalPlugins = plugins.length;
        const totalPages = Math.ceil(totalPlugins / PLUGINS_PER_PAGE);

        // Parse page number
        let page = 1;
        if (args.length > 0) {
            const parsedPage = parseInt(args[0]);
            if (!isNaN(parsedPage) && parsedPage > 0) {
                page = Math.min(parsedPage, totalPages);
            }
        }

        const startIndex = (page - 1) * PLUGINS_PER_PAGE;
        const endIndex = Math.min(startIndex + PLUGINS_PER_PAGE, totalPlugins);

        this.sendConsole(entity, '');
        this.sendConsole(entity, `----- ${this.getLang(entity, 'LOADED_PLUGINS')} -----`);
        const lName = this.getLang(entity, 'NAME').padEnd(23);
        const lVersion = this.getLang(entity, 'VERSION').padEnd(9);
        const lAuthor = this.getLang(entity, 'AUTHOR').padEnd(17);
        const lFile = this.getLang(entity, 'FILE').padEnd(17);
        const lStatus = this.getLang(entity, 'STATUS');
        this.sendConsole(entity, `       ${lName} ${lVersion} ${lAuthor} ${lFile} ${lStatus}`);

        for (let i = startIndex; i < endIndex; i++) {
            const loaded = plugins[i];
            const meta = loaded.plugin.metadata;
            const index = String(i).padStart(3);
            const name = (meta.name || 'Unknown').padEnd(23).substring(0, 23);
            const version = (meta.version || '?').padEnd(9).substring(0, 9);
            const author = (meta.author || 'Unknown').padEnd(17).substring(0, 17);
            const filename = (loaded.pluginName || '?').padEnd(17).substring(0, 17);
            const status = loaded.status === 'running' ? 'running' : loaded.status;

            this.sendConsole(entity, `[${index}] ${name} ${version} ${author} ${filename} ${status}`);
        }

        const runningCount = pluginLoader.getRunningCount();
        this.sendConsole(entity, `----- ${localization.getLang(entity, 'adminhelp', 'HELP_ENTRIES', startIndex + 1, endIndex, totalPlugins)} -----`);
        this.sendConsole(entity, this.getLang(entity, 'PLUGINS_RUN', runningCount, runningCount));

        if (page < totalPages) {
            this.sendConsole(entity, `----- ${localization.getLang(entity, 'adminhelp', 'HELP_USE_MORE', 'amx_plugins', page + 1)} -----`);
        }
    }

    /**
     * amx_modules - List loaded modules (stub - NodeMod doesn't have separate modules)
     *
     * Note: This command is intentionally a stub. Unlike AMX Mod X which has a separate
     * module system (*.amxx plugins + *.dll/*.so modules), NodeMod uses a unified plugin
     * architecture where all functionality is provided through plugins. The module system
     * concept doesn't apply to NodeMod's design.
     */
    private cmdModules(entity: nodemod.Entity | null) {
        if (!adminSystem.cmdAccess(entity, ADMIN_ADMIN)) return;

        this.sendConsole(entity, '');
        this.sendConsole(entity, '----- Loaded Modules -----');
        this.sendConsole(entity, '  NodeMod uses a unified plugin architecture.');
        this.sendConsole(entity, '  There is no separate module system.');
        this.sendConsole(entity, '  Use amx_plugins to see loaded plugins.');
        this.sendConsole(entity, '----- 0 modules -----');
    }
}



export default AdminCommands;
