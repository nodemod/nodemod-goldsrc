// Admin Base Plugin
// Converted from AMX Mod X admin.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO
//
// Storage Support:
//   The admin system uses a pluggable storage backend. Default is file-based (users.ini).
//   Use amx_storage_backend CVAR to select backend, amx_storage_list to list available.
//
//   To add custom storage (SQL, Redis, etc.), implement StorageAdapter:
//
//   import { storage, StorageAdapter } from './admin/storage';
//
//   class MySqlAdapter implements StorageAdapter {
//     name = 'mysql';
//     description = 'MySQL database';
//     isAvailable() { return true; }
//     async initialize(config) { ... }
//     async load() { ... }
//     async save(entry) { ... }
//     async exists(key, value) { ... }
//     async clear() { ... }
//     async dispose() { ... }
//   }
//
//   storage.register(new MySqlAdapter());

import nodemodCore from '@nodemod/core';
import fs from 'fs';
import path from 'path';

// Import constants (re-export for plugin use)
import {
    // Access level flags
    ADMIN_IMMUNITY,
    ADMIN_RESERVATION,
    ADMIN_KICK,
    ADMIN_BAN,
    ADMIN_SLAY,
    ADMIN_MAP,
    ADMIN_CVAR,
    ADMIN_CFG,
    ADMIN_CHAT,
    ADMIN_VOTE,
    ADMIN_PASSWORD,
    ADMIN_RCON,
    ADMIN_LEVEL_A,
    ADMIN_LEVEL_B,
    ADMIN_LEVEL_C,
    ADMIN_LEVEL_D,
    ADMIN_LEVEL_E,
    ADMIN_LEVEL_F,
    ADMIN_LEVEL_G,
    ADMIN_LEVEL_H,
    ADMIN_MENU,
    ADMIN_ADMIN,
    ADMIN_USER,
    // Admin type flags
    ADMIN_LOOKUP,
    ADMIN_STEAM,
    ADMIN_IPADDR,
    ADMIN_NAME,
    // Account flags
    FLAG_KICK,
    FLAG_TAG,
    FLAG_AUTHID,
    FLAG_IP,
    FLAG_NOPASS,
    FLAG_CASE_SENSITIVE,
    // cmd_target flags
    CMDTARGET_OBEY_IMMUNITY,
    CMDTARGET_ALLOW_SELF,
    CMDTARGET_ONLY_ALIVE,
    CMDTARGET_NO_BOTS,
    // Flag utilities
    readFlags,
    getFlags
} from './constants';

// Import utilities
import * as utils from './utils';

// Import storage system (types + registry)
import {
    storage,
    StorageAdapter,
    StorageConfig,
    StorageEntry
} from './storage';

// Import storage backends (auto-registers them)
import './storage/file';
import './storage/sql';

// Localization system
import localization from './localization';

// Help registry for amx_help (BasePlugin uses this internally)

// Plugin system
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import { BasePlugin } from './baseplugin';

const cvar = nodemodCore.cvar;

/**
 * Admin entry structure (internal use)
 */
export interface AdminEntry {
    auth: string;
    password: string;
    access: number;
    flags: number;
}

class AdminSystem extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Base',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Admin base plugin - required for any admin-related functionality'
    };

    // In-memory admin cache
    private admins: AdminEntry[] = [];

    private caseSensitiveName: boolean[] = [];
    private clientIpAddresses: string[] = [];
    private clientFlags: number[] = [];
    private clientIngame: boolean[] = [];
    private cmdLoopback: string;

    // Config variables (CVAR wrappers)
    private amxMode: any;
    private amxPasswordField: any;
    private amxDefaultAccess: any;
    private amxVoteRatio: any;
    private amxVoteTime: any;
    private amxVoteAnswers: any;
    private amxVoteDelay: any;
    private amxLastVoting: any;
    private amxVotekickRatio: any;
    private amxVotebanRatio: any;
    private amxVotemapRatio: any;
    private amxSqlTable: any;
    private amxSqlHost: any;
    private amxSqlUser: any;
    private amxSqlPass: any;
    private amxSqlDb: any;
    private amxSqlType: any;

    constructor(pluginName: string) {
        super(pluginName);
        this.cmdLoopback = `amxauth${this.randomChar()}${this.randomChar()}${this.randomChar()}${this.randomChar()}`;
        this.initializePlugin();

        // If we reloaded while map was running
        // Note: IP-based admin matching won't work until player reconnects
        // because clientIpAddresses is only populated during dllClientConnect
        for (const player of nodemod.players) {
            if (!player) continue;
            const index = nodemod.eng.indexOfEdict(player);
            this.caseSensitiveName[index] = player.netname !== player.netname?.toLowerCase();
            this.clientIngame[index] = true;
            this.accessUser(player);
        }
    }

    /**
     * Get the current storage adapter
     */
    getStorageAdapter(): StorageAdapter | null {
        return storage.getAdapter();
    }

    /**
     * Reload admins from storage
     */
    async reloadAdmins(): Promise<void> {
        this.removeServerUserFlags();
        this.admins = [];

        // Load from storage and convert to AdminEntry
        const entries = await storage.load();
        for (const entry of entries) {
            this.admins.push({
                auth: entry.auth || '',
                password: entry.password || '',
                access: readFlags(entry.access || ''),
                flags: readFlags(entry.flags || '')
            });
        }

        // Re-check all connected players
        for (const player of nodemod.players) {
            if (player && this.isPlayerIngame(player)) {
                this.accessUser(player, player.netname);
            }
        }
    }

    /**
     * Get storage configuration from CVARs
     */
    getStorageConfig(): StorageConfig {
        return storage.getConfig();
    }

    /**
     * Get all cached admins
     */
    getAdmins(): AdminEntry[] {
        return this.admins;
    }

    /**
     * Get admin count
     */
    getAdminCount(): number {
        return this.admins.length;
    }

    private randomChar(): string {
        return String.fromCharCode(65 + Math.floor(Math.random() * 26));
    }

    private initializePlugin() {

        // Initialize time tracking for proper get_timeleft() calculation
        utils.initTimeTracking();

        // Load dictionaries (equivalent to register_dictionary in AMXX)
        localization.loadDictionary('admin');
        localization.loadDictionary('common');

        // Register CVARs first
        this.registerCVars();

        // Register commands
        this.registerCommands();

        // NOTE: Config files (amxx.cfg, sql.cfg) are executed from index.ts
        // AFTER all plugins are loaded, so their CVARs are registered first.

        // Remove user flags from server (equivalent to remove_user_flags(0, read_flags("z")))
        this.removeServerUserFlags();

        // Load admin settings
        this.loadSettings();

        // Setup event handlers
        this.setupEventHandlers();

        // Setup delayed map config loading
        this.setupDelayedLoad();
    }

    private registerCVars() {
        // Core admin CVARs
        this.amxMode = this.registerCvar('amx_mode', '1', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Admin authentication mode (0=disabled, 1=normal, 2=strict)');
        this.amxPasswordField = this.registerCvar('amx_password_field', '_pw', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Client info field used for admin passwords');
        this.amxDefaultAccess = this.registerCvar('amx_default_access', '', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Default access flags for non-admin users');

        // Voting CVARs
        this.amxVoteRatio = this.registerCvar('amx_vote_ratio', '0.02', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Minimum ratio of players needed to start a vote');
        this.amxVoteTime = this.registerCvar('amx_vote_time', '10', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Time in seconds for voting');
        this.amxVoteAnswers = this.registerCvar('amx_vote_answers', '1', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Number of answers in votes');
        this.amxVoteDelay = this.registerCvar('amx_vote_delay', '60', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Delay between votes in seconds');
        this.amxLastVoting = this.registerCvar('amx_last_voting', '0', nodemod.FCVAR.SERVER, 'Timestamp of last voting session');
        // Registered here but wrapped by BasePlugin (amxShowActivity is inherited)
        this.registerCvar('amx_show_activity', '2', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Admin activity display mode (0=none, 1=admins only, 2=all)');

        // Vote-specific ratios
        this.amxVotekickRatio = this.registerCvar('amx_votekick_ratio', '0.40', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Ratio needed for votekick to pass');
        this.amxVotebanRatio = this.registerCvar('amx_voteban_ratio', '0.40', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Ratio needed for voteban to pass');
        this.amxVotemapRatio = this.registerCvar('amx_votemap_ratio', '0.40', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'Ratio needed for votemap to pass');

        // SQL CVARs
        this.amxSqlTable = this.registerCvar('amx_sql_table', 'admins', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL table name for admin data');
        this.amxSqlHost = this.registerCvar('amx_sql_host', '127.0.0.1', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL server host');
        this.amxSqlUser = this.registerCvar('amx_sql_user', 'root', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL server username');
        this.amxSqlPass = this.registerCvar('amx_sql_pass', '', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL server password');
        this.amxSqlDb = this.registerCvar('amx_sql_db', 'amx', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL database name');
        this.amxSqlType = this.registerCvar('amx_sql_type', 'mysql', nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE, 'SQL server type (mysql/sqlite)');

        // Set initial value for amx_last_voting
        cvar.setFloat('amx_last_voting', 0.0);
    }

    // Localization support
    private formatLangString(key: string, ...args: any[]): string {
        const langStrings: { [key: string]: string } = {
            'CL_NOT_FOUND': 'Client not found',
            'INV_PAS': 'Invalid password',
            'PAS_ACC': 'Password accepted',
            'PRIV_SET': 'Privileges set',
            'NO_ENTRY': 'You have no entry to the server',
            'LOADED_ADMIN': 'Loaded 1 admin from file',
            'LOADED_ADMINS': 'Loaded %d admins from file',
            'SQL_LOADED_ADMIN': 'Loaded 1 admin from SQL',
            'SQL_LOADED_ADMINS': 'Loaded %d admins from SQL',
            'SQL_CANT_CON': 'SQL connection failed: %s',
            'SQL_CANT_LOAD_ADMINS': 'SQL load admins failed: %s',
            'NO_ADMINS': 'No admins found'
        };

        let result = langStrings[key] || key;
        for (let i = 0; i < args.length; i++) {
            result = result.replace('%d', args[i].toString()).replace('%s', args[i].toString());
        }
        return result;
    }

    /**
     * Check if entity has access flags (silent)
     * @param entity The entity to check (null for server console)
     * @param level Required access level flags
     * @returns true if has access
     */
    hasAccess(entity: nodemod.Entity | null, level: number): boolean {
        return utils.hasAccess(entity, level, (e) => this.getUserFlags(e));
    }

    /**
     * Check if entity has access to a command (shows error message on failure)
     * @param entity The entity to check (null for server console)
     * @param level Required access level flags
     * @returns true if access granted
     */
    cmdAccess(entity: nodemod.Entity | null, level: number): boolean {
        if (!this.hasAccess(entity, level)) {
            if (entity) {
                utils.sendClientMessage(entity, this.getLangWithFallback(entity, 'NO_ACC_COM'));
            }
            return false;
        }
        return true;
    }

    /**
     * Check if entity is a bot
     */
    isBot(entity: nodemod.Entity | null): boolean {
        return utils.isBot(entity);
    }

    /**
     * Check if entity is alive
     */
    isAlive(entity: nodemod.Entity | null): boolean {
        return utils.isAlive(entity);
    }

    /**
     * Get all players (with optional filters)
     * Note: Does not use isIngame tracking, matching original AMX Mod X get_players() behavior
     */
    getPlayers(options?: { excludeBots?: boolean; onlyAlive?: boolean }): nodemod.Entity[] {
        return utils.getPlayers(options);
    }

    /**
     * Find player by user ID (#123 format)
     */
    findPlayerByUserId(userId: number): nodemod.Entity | null {
        return utils.findPlayerByUserId(userId);
    }

    /**
     * cmd_target - Find a player target with various flags
     */
    cmdTarget(
        admin: nodemod.Entity | null,
        target: string,
        flags: number = 0
    ): nodemod.Entity | null {
        return utils.cmdTarget(admin, target, flags, {
            getUserFlags: (e) => this.getUserFlags(e),
            sendMessage: (e, msg) => utils.sendClientMessage(e, msg),
            isIngame: (e) => this.isPlayerIngame(e)
        });
    }

    private isPlayerIngame(entity: nodemod.Entity): boolean {
        const index = nodemod.eng.indexOfEdict(entity);
        return this.clientIngame[index] || false;
    }

    /**
     * Execute config files (amxx.cfg, sql.cfg)
     * Called from index.ts after all plugins are loaded so their CVARs are registered
     */
    public executeConfigFiles() {
        // Delay config execution until server is fully initialized
        setTimeout(() => {
            this.doExecuteConfigFiles();
        }, 100);
    }

    private doExecuteConfigFiles() {
        try {
            const configsDir = utils.getConfigsDir();
            const gameDir = utils.getGameDir();
            const execPath = path.relative(gameDir, configsDir);

            const amxxCfg = path.join(configsDir, 'amxx.cfg');
            if (fs.existsSync(amxxCfg)) {
                nodemod.eng.serverCommand(`exec ${execPath}/amxx.cfg\n`);
                nodemod.eng.serverExecute();
            }

            const sqlCfg = path.join(configsDir, 'sql.cfg');
            if (fs.existsSync(sqlCfg)) {
                nodemod.eng.serverCommand(`exec ${execPath}/sql.cfg\n`);
                nodemod.eng.serverExecute();
            }
        } catch (error) {
            console.error('[Admin] Error executing config files:', error);
        }
    }

    private setupDelayedLoad() {
        setTimeout(() => {
            this.loadMapConfigs();
        }, 6100);
    }

    private loadMapConfigs() {
        try {
            const configsDir = path.join(process.cwd(), '..', 'configs');
            const currentMap = nodemod.mapname || '';

            if (!currentMap) {
                return;
            }

            const underscoreIndex = currentMap.indexOf('_');
            if (underscoreIndex !== -1) {
                const mapPrefix = currentMap.substring(0, underscoreIndex);
                const prefixConfigPath = path.join(configsDir, 'maps', `prefix_${mapPrefix}.cfg`);

                if (fs.existsSync(prefixConfigPath)) {
                    nodemod.eng.serverCommand(`exec ${prefixConfigPath}\n`);
                }
            }

            const mapConfigPath = path.join(configsDir, 'maps', `${currentMap}.cfg`);
            if (fs.existsSync(mapConfigPath)) {
                nodemod.eng.serverCommand(`exec ${mapConfigPath}\n`);
            }
        } catch (error) {
            console.error('[Admin] Error loading map configs:', error);
        }
    }

    private registerCommands() {
        this.registerCommand('amx_reloadadmins', ADMIN_CFG, '- reload admin list', (entity) => {
            if (!this.cmdAccess(entity, ADMIN_CFG)) return;
            this.cmdReload(entity);
        });

        this.registerCommand('amx_addadmin', ADMIN_RCON, '<player> <flags> [password] [authtype]', (entity, args) => {
            this.addAdminCommand(entity, args);
        });

        this.registerCommand('amx_storage_list', ADMIN_CFG, '- list storage backends', (entity) => {
            if (!this.cmdAccess(entity, ADMIN_CFG)) return;
            storage.listBackends(entity);
        });

        this.registerCommand('amx_storage_use', ADMIN_RCON, '<backend> - switch storage backend', (entity, args) => {
            if (!this.cmdAccess(entity, ADMIN_RCON)) return;
            if (args.length < 1) {
                storage.sendMessage(entity, 'Usage: amx_storage_use <backend_name>');
                storage.sendMessage(entity, 'Use amx_storage_list to see available backends');
                return;
            }
            storage.switchBackend(args[0], entity).catch(err => {
                storage.sendMessage(entity, `[Storage] Failed to switch backend: ${err}`);
            });
        });

        // Client-only command (no help entry)
        nodemodCore.cmd.registerClient(this.cmdLoopback, (entity, args) => {
            this.ackSignal(entity);
        });
    }

    private setupEventHandlers() {
        nodemod.on('dllClientConnect', (pEntity: nodemod.Entity, pszName: string, pszAddress: string, szRejectReason: string) => {
            const index = nodemod.eng.indexOfEdict(pEntity);
            this.caseSensitiveName[index] = false;
            const colonIndex = pszAddress.indexOf(':');
            this.clientIpAddresses[index] = colonIndex !== -1 ? pszAddress.substring(0, colonIndex) : pszAddress;
            this.clientFlags[index] = 0;
            this.clientIngame[index] = false;
        });

        nodemod.on('dllClientPutInServer', (entity) => {
            const index = nodemod.eng.indexOfEdict(entity);
            this.clientIngame[index] = true;

            if (this.amxMode?.int) {
                this.accessUser(entity);
            }
        });

        nodemod.on('dllClientDisconnect', (entity) => {
            const index = nodemod.eng.indexOfEdict(entity);
            this.clientIngame[index] = false;
            this.clientFlags[index] = 0;
            this.caseSensitiveName[index] = false;
            this.clientIpAddresses[index] = "";
        });

        nodemod.on('dllClientUserInfoChanged', (entity) => {
            this.onClientInfoChanged(entity);
        });
    }

    private async loadSettings(): Promise<void> {
        await storage.initialize();
        await this.reloadAdmins();

        const count = this.admins.length;
        if (count === 0) {
            console.log(`[AMXX] ${this.formatLangString('NO_ADMINS')}`);
        } else if (count === 1) {
            console.log(`[AMXX] ${this.formatLangString('LOADED_ADMIN')}`);
        } else {
            console.log(`[AMXX] ${this.formatLangString('LOADED_ADMINS', count)}`);
        }
    }

    private getAccess(id: number, name: string, authid: string, ip: string, password: string): number {
        let result = 0;
        let foundIndex = -1;

        this.caseSensitiveName[id] = false;

        for (let i = 0; i < this.admins.length; i++) {
            const admin = this.admins[i];

            if (admin.flags & FLAG_AUTHID) {
                if (authid === admin.auth) {
                    foundIndex = i;
                    break;
                }
            } else if (admin.flags & FLAG_IP) {
                const authLength = admin.auth.length;
                if (admin.auth[authLength - 1] === '.') {
                    if (ip.startsWith(admin.auth)) {
                        foundIndex = i;
                        break;
                    }
                } else if (ip === admin.auth) {
                    foundIndex = i;
                    break;
                }
            } else {
                if (admin.flags & FLAG_CASE_SENSITIVE) {
                    if (admin.flags & FLAG_TAG) {
                        if (name.includes(admin.auth)) {
                            foundIndex = i;
                            this.caseSensitiveName[id] = true;
                            break;
                        }
                    } else if (name === admin.auth) {
                        foundIndex = i;
                        this.caseSensitiveName[id] = true;
                        break;
                    }
                } else {
                    if (admin.flags & FLAG_TAG) {
                        if (name.toLowerCase().includes(admin.auth.toLowerCase())) {
                            foundIndex = i;
                            break;
                        }
                    } else if (name.toLowerCase() === admin.auth.toLowerCase()) {
                        foundIndex = i;
                        break;
                    }
                }
            }
        }

        if (foundIndex !== -1) {
            const admin = this.admins[foundIndex];

            if (admin.flags & FLAG_NOPASS) {
                result |= 8;
                this.setUserFlags(id, admin.access);

                const flagsStr = getFlags(admin.access);
                this.logAmx(`Login: "${name}<${id}><${authid}><>" became an admin (account "${admin.auth}") (access "${flagsStr}") (address "${ip}")`);
            } else {
                if (password === admin.password) {
                    result |= 12;
                    this.setUserFlags(id, admin.access);

                    const flagsStr = getFlags(admin.access);
                    this.logAmx(`Login: "${name}<${id}><${authid}><>" became an admin (account "${admin.auth}") (access "${flagsStr}") (address "${ip}")`);
                } else {
                    result |= 1;

                    if (admin.flags & FLAG_KICK) {
                        result |= 2;
                        this.logAmx(`Login: "${name}<${id}><${authid}><>" kicked due to invalid password (account "${admin.auth}") (address "${ip}")`);
                    }
                }
            }
        } else if (this.amxMode?.int === 2) {
            result |= 2;
        } else {
            const defaultAccess = this.amxDefaultAccess?.value || "z";
            const defaultFlags = readFlags(defaultAccess);

            if (defaultFlags) {
                result |= 8;
                this.setUserFlags(id, defaultFlags);
            }
        }

        return result;
    }

    private setUserFlags(index: number, flags: number) {
        this.clientFlags[index] = flags;
    }

    private removeUserFlags(entity: nodemod.Entity) {
        const index = nodemod.eng.indexOfEdict(entity);
        this.clientFlags[index] = 0;
    }

    /**
     * Get user's access flags
     */
    getUserFlags(entity: nodemod.Entity): number {
        const index = nodemod.eng.indexOfEdict(entity);
        return this.clientFlags[index] || 0;
    }

    private removeServerUserFlags() {
        this.clientFlags[0] = (this.clientFlags[0] || 0) & ~ADMIN_USER;
    }

    private accessUser(entity: nodemod.Entity, name?: string) {
        this.removeUserFlags(entity);

        const index = nodemod.eng.indexOfEdict(entity);
        const userIp = this.clientIpAddresses[index] || "";
        const userAuthId = nodemod.eng.getPlayerAuthId(entity) || "";
        const userName = name || entity.netname;

        const passField = this.amxPasswordField?.value || "_pw";
        const infoBuffer = nodemod.eng.getInfoKeyBuffer(entity);
        const password = nodemod.eng.infoKeyValue(infoBuffer, passField) || "";

        const result = this.getAccess(index, userName, userAuthId, userIp, password);

        if (result & 1) {
            utils.sendClientMessage(entity, this.formatLangString('INV_PAS'));
        }

        if (result & 2) {
            this.kickClient(entity);
            return;
        }

        if (result & 4) {
            utils.sendClientMessage(entity, this.formatLangString('PAS_ACC'));
        }

        if (result & 8) {
            utils.sendClientMessage(entity, this.formatLangString('PRIV_SET'));
        }
    }

    private kickClient(entity: nodemod.Entity) {
        const userId = nodemod.eng.getPlayerUserId(entity);
        const reason = this.formatLangString('NO_ENTRY');
        nodemod.eng.serverCommand(`kick #${userId} "${reason}"\n`);
    }

    private onClientInfoChanged(entity: nodemod.Entity) {
        if (!this.isPlayerIngame(entity) || !this.amxMode?.int) {
            return;
        }

        const index = nodemod.eng.indexOfEdict(entity);
        const oldName = entity.netname;
        const newName = entity.netname;

        if (this.caseSensitiveName[index]) {
            if (newName !== oldName) {
                this.accessUser(entity, newName);
            }
        } else {
            if (newName.toLowerCase() !== oldName.toLowerCase()) {
                this.accessUser(entity, newName);
            }
        }
    }

    private async cmdReload(client: nodemod.Entity | null = null): Promise<void> {
        // reloadAdmins already calls removeServerUserFlags and re-checks all players
        await this.reloadAdmins();

        const count = this.admins.length;

        if (client) {
            if (count === 1) {
                utils.sendClientMessage(client, `[AMXX] ${this.formatLangString('LOADED_ADMIN')}`);
            } else {
                utils.sendClientMessage(client, `[AMXX] ${this.formatLangString('LOADED_ADMINS', count)}`);
            }
        }
    }

    private async addAdminCommand(entity: nodemod.Entity | null, args: string[]): Promise<void> {
        if (!this.cmdAccess(entity, ADMIN_RCON)) {
            return;
        }

        if (args.length < 2) {
            const msg = "Usage: amx_addadmin <playername|auth> <accessflags> [password] [authtype]";
            if (entity) {
                utils.sendClientMessage(entity, msg);
            } else {
                console.log(msg);
            }
            return;
        }

        const target = args[0];
        const accessFlags = args[1];
        const password = args[2] || "";
        const authType = args[3] || "";

        let idType = ADMIN_STEAM | ADMIN_LOOKUP;

        if (args.length >= 4) {
            const lowerAuthType = authType.toLowerCase();
            if (lowerAuthType === "steam" || lowerAuthType === "steamid" || lowerAuthType === "auth") {
                idType = ADMIN_STEAM;
            } else if (lowerAuthType === "ip") {
                idType = ADMIN_IPADDR;
            } else if (lowerAuthType === "name" || lowerAuthType === "nick") {
                idType = ADMIN_NAME;
                if (lowerAuthType === "name") {
                    idType |= ADMIN_LOOKUP;
                }
            } else {
                const msg = `Unknown id type "${authType}", use one of: steamid, ip, name`;
                if (entity) {
                    utils.sendClientMessage(entity, msg);
                } else {
                    console.log(msg);
                }
                return;
            }
        }

        let player: nodemod.Entity | null = null;
        let finalAuth = target;

        if (idType & ADMIN_STEAM) {
            // Support both Steam IDs (STEAM_0:) and Xash3D IDs (ID_)
            if (!target.includes("STEAM_0:") && !target.startsWith("ID_")) {
                idType |= ADMIN_LOOKUP;
                player = utils.findPlayerByName(target);
            } else {
                player = utils.findPlayerBySteamId(target);
                if (!player) {
                    idType &= ~ADMIN_LOOKUP;
                }
            }
        } else if (idType & ADMIN_NAME) {
            player = utils.findPlayerByName(target);
            if (player) {
                idType |= ADMIN_LOOKUP;
            } else {
                idType &= ~ADMIN_LOOKUP;
            }
        } else if (idType & ADMIN_IPADDR) {
            if (!utils.isValidIp(target)) {
                idType |= ADMIN_LOOKUP;
                player = utils.findPlayerByName(target);
            } else {
                player = utils.findPlayerByIp(target, this.clientIpAddresses);
            }
        }

        if ((idType & ADMIN_LOOKUP) && !player) {
            const msg = this.formatLangString('CL_NOT_FOUND');
            if (entity) {
                utils.sendClientMessage(entity, msg);
            } else {
                console.log(msg);
            }
            return;
        }

        if (idType & ADMIN_LOOKUP && player) {
            if (idType & ADMIN_STEAM) {
                finalAuth = nodemod.eng.getPlayerAuthId(player) || target;
            } else if (idType & ADMIN_IPADDR) {
                const index = nodemod.eng.indexOfEdict(player);
                finalAuth = this.clientIpAddresses[index] || target;
            } else if (idType & ADMIN_NAME) {
                finalAuth = player.netname || target;
            }
        }

        let typeFlags = "";
        if (idType & ADMIN_STEAM) {
            typeFlags += "c";
        } else if (idType & ADMIN_IPADDR) {
            typeFlags += "d";
        }

        if (password.length > 0) {
            typeFlags += "a";
        } else {
            typeFlags += "e";
        }

        await this.addAdmin(entity, finalAuth, accessFlags, password, typeFlags);
        await this.reloadAdmins();

        if (player) {
            const name = player.netname || "";
            this.accessUser(player, name);
        }
    }

    private async addAdmin(entity: nodemod.Entity | null, auth: string, accessFlags: string, password: string, flags: string): Promise<void> {
        if (await storage.exists('auth', auth)) {
            const msg = `[AMXX] ${auth} already exists!`;
            if (entity) {
                utils.sendClientMessage(entity, msg);
            } else {
                console.log(msg);
            }
            return;
        }

        const entry: StorageEntry = {
            auth,
            password,
            access: accessFlags,
            flags
        };

        const success = await storage.save(entry);
        if (success) {
            const msg = `[AMXX] Adding: ${auth} ${accessFlags} ${flags}`;
            if (entity) {
                utils.sendClientMessage(entity, msg);
            } else {
                console.log(msg);
            }
        } else {
            const msg = `[AMXX] Failed to add admin: ${auth}`;
            if (entity) {
                utils.sendClientMessage(entity, msg);
            } else {
                console.error(msg);
            }
        }
    }

    private ackSignal(entity: nodemod.Entity) {
        this.kickClient(entity);
    }
}

// Create singleton instance - admin is a service other plugins depend on
// The 'admin' name matches what's in plugins.ini
const adminSystem = new AdminSystem('admin');

// Export instance for other plugins to use as a service
export { adminSystem };

// Export class as default for the plugin loader
export default AdminSystem;
