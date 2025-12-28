// Players Menu Plugin
// Converted from AMX Mod X plmenu.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';
import {
    ADMIN_KICK, ADMIN_BAN, ADMIN_SLAY, ADMIN_LEVEL_A, ADMIN_IMMUNITY, readFlags
} from './constants';

// Client command definition
interface ClientCommand {
    name: string;
    command: string;
    flags: number;      // 1=server, 2=admin client, 4=target client, 8=repeat menu
    access: number;
}

// Team definitions
const TEAM_NAMES = ['TERRORIST', 'CT', 'SPECTATOR'];
const TEAM_NUMBERS = ['1', '2', '6'];
const TEAM_INT_NUMBERS = [1, 2, 6];

// CS Team constants
// @TODO: Add CS module detection - original AMXX uses module_exists("cstrike")
// Team menu features should be conditionally enabled based on game mod
const CS_TEAM_UNASSIGNED = 0;
const CS_TEAM_T = 1;
const CS_TEAM_CT = 2;
const CS_TEAM_SPECTATOR = 3;

// CS-specific offsets for CBasePlayer (may vary by game version)
// These are typical offsets for CS 1.6 - adjust if needed
const CS_OFFSET_TEAM = 114;  // m_iTeam offset in CBasePlayer
const CS_OFFSET_MODELNAME = 126;  // m_iModelName offset

class PlMenu extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Players Menu',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Kick, ban, slap, slay, team menus'
    };

    // Per-player menu state
    // Note: menuPosition is only needed for menus with persistent options (ban, slap, team, clcmd)
    // The kick menu uses core pagination directly
    private menuPosition: Map<number, number> = new Map();
    private menuOption: Map<number, number> = new Map();
    private menuSettings: Map<number, number> = new Map();
    private menuSelectNum: Map<number, number> = new Map();
    private menuSelect: Map<number, number[]> = new Map();

    // Ban times (in minutes, 0 = permanent)
    private banTimes: number[] = [0, 5, 10, 15, 30, 45, 60];

    // Slap damage settings (0 = slay, then damage values)
    private slapSettings: number[] = [0, 0, 1, 5];

    // Client commands from clcmds.ini
    private clientCommands: ClientCommand[] = [];

    constructor(pluginName: string) {
        super(pluginName);

        // Load clcmds.ini
        this.loadClientCommands();

        // Register commands (uses BasePlugin.registerCommand for auto plugin tracking)
        this.registerPluginCommands();

        // Register server commands for ban times and slap damage
        this.registerServerCommands();
    }

    private registerServerCommands() {
        // amx_plmenu_bantimes <time1> [time2] [time3] ...
        nodemodCore.cmd.registerServer('amx_plmenu_bantimes', (args) => {
            if (args.length === 0) {
                console.log('usage: amx_plmenu_bantimes <time1> [time2] [time3] ...');
                console.log('   use time of 0 for permanent.');
                return;
            }

            this.banTimes = args.map(arg => parseInt(arg) || 0);
        });

        // amx_plmenu_slapdmg <dmg1> [dmg2] [dmg3] ...
        // Slay is automatically inserted as the last option in the menu
        nodemodCore.cmd.registerServer('amx_plmenu_slapdmg', (args) => {
            if (args.length === 0) {
                console.log('usage: amx_plmenu_slapdmg <dmg1> [dmg2] [dmg3] ...');
                console.log('   slay is automatically added as the last option.');
                return;
            }

            this.slapSettings = args.map(arg => parseInt(arg) || 0);
        });
    }

    private loadClientCommands() {
        const filePath = path.join(utils.getConfigsDir(), 'clcmds.ini');

        if (!fs.existsSync(filePath)) {
            return;
        }

        try {
            const content = fs.readFileSync(filePath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();
                if (!trimmed || trimmed.startsWith(';')) continue;

                // Parse: "name" "command" "flags" "access"
                const parts = utils.parseCommand(trimmed);
                if (parts.length >= 4) {
                    const name = parts[0];
                    let command = parts[1];
                    const flags = this.parseClcmdFlags(parts[2]);
                    const access = readFlags(parts[3]);

                    // Replace single quotes with double quotes
                    command = command.replace(/'/g, '"');

                    this.clientCommands.push({ name, command, flags, access });
                }
            }

        } catch (e) {
            console.error('[PlMenu] Error loading clcmds.ini:', e);
        }
    }

    private parseClcmdFlags(flagStr: string): number {
        let flags = 0;
        for (const char of flagStr.toLowerCase()) {
            switch (char) {
                case 'a': flags |= 1; break;  // server command
                case 'b': flags |= 2; break;  // admin client command
                case 'c': flags |= 4; break;  // target client command
                case 'd': flags |= 8; break;  // repeat menu
            }
        }
        return flags;
    }

    private registerPluginCommands() {
        // Kick Menu (uses BasePlugin.registerCommand for auto plugin tracking)
        this.registerCommand('amx_kickmenu', ADMIN_KICK, '- displays kick menu', (entity, args) => {
            this.cmdKickMenu(entity);
        });

        // Ban Menu
        this.registerCommand('amx_banmenu', ADMIN_BAN, '- displays ban menu', (entity, args) => {
            this.cmdBanMenu(entity);
        });

        // Slap/Slay Menu
        this.registerCommand('amx_slapmenu', ADMIN_SLAY, '- displays slap/slay menu', (entity, args) => {
            this.cmdSlapMenu(entity);
        });

        // Team Menu
        this.registerCommand('amx_teammenu', ADMIN_LEVEL_A, '- displays team menu', (entity, args) => {
            this.cmdTeamMenu(entity);
        });

        // Client Commands Menu
        this.registerCommand('amx_clcmdmenu', ADMIN_LEVEL_A, '- displays client cmds menu', (entity, args) => {
            this.cmdClcmdMenu(entity);
        });
    }

    // ==================== KICK MENU ====================

    private cmdKickMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_KICK)) return;

        this.displayKickMenu(entity);
    }

    private displayKickMenu(entity: nodemod.Entity) {
        const players = adminSystem.getPlayers({ excludeBots: false });
        const adminEntity = entity;

        // Build all items - core menu handles pagination
        const items = players.map(player => {
            const name = player.netname || 'Unknown';
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== adminEntity;
            const isAdmin = adminSystem.hasAccess(player, ADMIN_KICK);

            return {
                name: isAdmin ? `${name} *` : name,
                disabled: hasImmunity,
                handler: (client: nodemod.Entity) => {
                    this.kickPlayer(client, player);
                    // Re-display menu after action
                    setTimeout(() => this.displayKickMenu(client), 0);
                }
            };
        });

        nodemodCore.menu.show({
            entity,
            title: this.getLang(entity, 'KICK_MENU'),
            items,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private kickPlayer(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminName = admin.netname || 'Admin';
        const targetName = target.netname || 'Unknown';
        const userId = nodemod.eng.getPlayerUserId(target);

        // Log and show activity
        const adminUserId = nodemod.eng.getPlayerUserId(admin);
        const adminAuthId = nodemod.eng.getPlayerAuthId(admin) || '';
        const targetUserId = nodemod.eng.getPlayerUserId(target);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
        this.logAmx(`Kick: "${adminName}<${adminUserId}><${adminAuthId}><>" kick "${targetName}<${targetUserId}><${targetAuthId}><>"`);
        this.showActivity(admin, this.getLang(null, 'ADMIN_KICK_2', adminName, targetName));

        // Execute kick
        nodemod.eng.serverCommand(`kick #${userId}\n`);
    }

    // ==================== BAN MENU ====================
    // Note: Ban, Slap, Team, and Clcmd menus use manual pagination because they have
    // a persistent "option" item (ban time, slap damage, team, command) that must appear
    // on EVERY page. The core menu pagination can't handle this pattern.

    private cmdBanMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_BAN)) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        this.menuPosition.set(entityId, 0);
        this.menuOption.set(entityId, 0);
        this.menuSettings.set(entityId, this.banTimes[0] || 0);

        this.displayBanMenu(entity, 0);
    }

    private displayBanMenu(entity: nodemod.Entity, pos: number) {
        if (pos < 0) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        const banTime = this.menuSettings.get(entityId) || 0;

        const players = adminSystem.getPlayers({ excludeBots: false });
        const adminEntity = entity;
        const ITEMS_PER_PAGE = 7;  // 7 because we have the ban time option
        const start = pos * ITEMS_PER_PAGE;
        const total = players.length;

        if (start >= total && total > 0) {
            this.menuPosition.set(entityId, 0);
            this.displayBanMenu(entity, 0);
            return;
        }

        const end = Math.min(start + ITEMS_PER_PAGE, total);
        const totalPages = Math.ceil(total / ITEMS_PER_PAGE) || 1;
        const currentPage = pos + 1;

        const items: any[] = [];

        // Player items for this page
        for (let a = start; a < end; a++) {
            const player = players[a];
            const name = player.netname || 'Unknown';
            const isBot = utils.isBot(player);
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== adminEntity;
            const isAdmin = adminSystem.hasAccess(player, ADMIN_BAN);

            items.push({
                name: isAdmin ? `${name} *` : name,
                disabled: isBot || hasImmunity,
                handler: (client: nodemod.Entity) => {
                    this.banPlayer(client, player);
                    const clientId = nodemod.eng.indexOfEdict(client);
                    setTimeout(() => this.displayBanMenu(client, this.menuPosition.get(clientId) || 0), 0);
                }
            });
        }

        // Ban time option
        const banTimeLabel = banTime === 0
            ? this.getLang(entity, 'BAN_PERM')
            : this.getLang(entity, 'BAN_FOR_MIN', banTime.toString());

        items.push({
            name: `[${banTimeLabel}]`,
            handler: (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                let option = (this.menuOption.get(clientId) || 0) + 1;
                option %= this.banTimes.length;
                this.menuOption.set(clientId, option);
                this.menuSettings.set(clientId, this.banTimes[option]);
                setTimeout(() => this.displayBanMenu(client, this.menuPosition.get(clientId) || 0), 0);
            }
        });

        // Navigation
        if (end < total) {
            items.push({
                name: this.getLangWithFallback(entity, 'MORE') + '...',
                handler: (client: nodemod.Entity) => {
                    const clientId = nodemod.eng.indexOfEdict(client);
                    const newPos = (this.menuPosition.get(clientId) || 0) + 1;
                    this.menuPosition.set(clientId, newPos);
                    setTimeout(() => this.displayBanMenu(client, newPos), 0);
                }
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'BAN_MENU')} ${currentPage}/${totalPages}`,
            items,
            onExit: pos > 0 ? (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                const newPos = (this.menuPosition.get(clientId) || 1) - 1;
                this.menuPosition.set(clientId, newPos);
                setTimeout(() => this.displayBanMenu(client, newPos), 0);
            } : undefined,
            formatters: utils.coloredMenuFormatters
        });
    }

    private banPlayer(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminId = nodemod.eng.indexOfEdict(admin);
        const banTime = this.menuSettings.get(adminId) || 0;

        const adminName = admin.netname || 'Admin';
        const targetName = target.netname || 'Unknown';
        const userId = nodemod.eng.getPlayerUserId(target);
        const authId = nodemod.eng.getPlayerAuthId(target);

        // Log
        const adminUserId = nodemod.eng.getPlayerUserId(admin);
        const adminAuthId = nodemod.eng.getPlayerAuthId(admin) || '';
        this.logAmx(`Ban: "${adminName}<${adminUserId}><${adminAuthId}><>" ban and kick "${targetName}<${userId}><${authId}><>" (minutes "${banTime}")`);

        // Show activity
        if (banTime === 0) {
            this.showActivity(admin, this.getLang(null, 'BAN', targetName) + ' ' + this.getLang(null, 'PERM'));
        } else {
            this.showActivity(admin, this.getLang(null, 'BAN', targetName) + ' ' + this.getLang(null, 'FOR_MIN', banTime.toString()));
        }

        // Check if we should ban by IP
        const shouldBanByIp = authId === '4294967295' ||
            authId === 'HLTV' ||
            authId === 'STEAM_ID_LAN' ||
            authId.toUpperCase() === 'VALVE_ID_LAN';

        if (shouldBanByIp) {
            const ipAddr = utils.getPlayerIP(target);
            nodemod.eng.serverCommand(`addip ${banTime} ${ipAddr};writeip\n`);
        } else {
            nodemod.eng.serverCommand(`banid ${banTime} #${userId} kick;writeid\n`);
        }
    }

    // ==================== SLAP/SLAY MENU ====================

    private cmdSlapMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_SLAY)) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        this.menuPosition.set(entityId, 0);
        this.menuOption.set(entityId, 0);
        this.menuSettings.set(entityId, this.slapSettings[0] || 0);

        this.displaySlapMenu(entity, 0);
    }

    private displaySlapMenu(entity: nodemod.Entity, pos: number) {
        if (pos < 0) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        const currentOption = this.menuOption.get(entityId) || 0;
        const damage = this.menuSettings.get(entityId) || 0;

        const players = adminSystem.getPlayers({ excludeBots: false });
        const adminEntity = entity;
        const ITEMS_PER_PAGE = 7;  // 7 because we have the slap/slay option
        const start = pos * ITEMS_PER_PAGE;
        const total = players.length;

        if (start >= total && total > 0) {
            this.menuPosition.set(entityId, 0);
            this.displaySlapMenu(entity, 0);
            return;
        }

        const end = Math.min(start + ITEMS_PER_PAGE, total);
        const totalPages = Math.ceil(total / ITEMS_PER_PAGE) || 1;
        const currentPage = pos + 1;

        const items: any[] = [];

        // Player items for this page
        for (let a = start; a < end; a++) {
            const player = players[a];
            const name = player.netname || 'Unknown';
            const isAlive = utils.isAlive(player);
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== adminEntity;
            const isAdmin = adminSystem.hasAccess(player, ADMIN_SLAY);
            const team = this.getPlayerTeamShort(player);

            items.push({
                name: isAdmin ? `${name} * [${team}]` : `${name} [${team}]`,
                disabled: !isAlive || hasImmunity,
                handler: (client: nodemod.Entity) => {
                    this.slapSlayPlayer(client, player);
                    const clientId = nodemod.eng.indexOfEdict(client);
                    setTimeout(() => this.displaySlapMenu(client, this.menuPosition.get(clientId) || 0), 0);
                }
            });
        }

        // Slap/Slay option
        const optionLabel = currentOption === 0
            ? this.getLang(entity, 'SLAY')
            : this.getLang(entity, 'SLAP_WITH_DMG', damage.toString());

        items.push({
            name: `[${optionLabel}]`,
            handler: (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                let option = (this.menuOption.get(clientId) || 0) + 1;
                option %= this.slapSettings.length;
                this.menuOption.set(clientId, option);
                this.menuSettings.set(clientId, this.slapSettings[option]);
                setTimeout(() => this.displaySlapMenu(client, this.menuPosition.get(clientId) || 0), 0);
            }
        });

        // Navigation
        if (end < total) {
            items.push({
                name: this.getLangWithFallback(entity, 'MORE') + '...',
                handler: (client: nodemod.Entity) => {
                    const clientId = nodemod.eng.indexOfEdict(client);
                    const newPos = (this.menuPosition.get(clientId) || 0) + 1;
                    this.menuPosition.set(clientId, newPos);
                    setTimeout(() => this.displaySlapMenu(client, newPos), 0);
                }
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'SLAP_SLAY_MENU')} ${currentPage}/${totalPages}`,
            items,
            onExit: pos > 0 ? (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                const newPos = (this.menuPosition.get(clientId) || 1) - 1;
                this.menuPosition.set(clientId, newPos);
                setTimeout(() => this.displaySlapMenu(client, newPos), 0);
            } : undefined,
            formatters: utils.coloredMenuFormatters
        });
    }

    private slapSlayPlayer(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminId = nodemod.eng.indexOfEdict(admin);
        const option = this.menuOption.get(adminId) || 0;
        const damage = this.menuSettings.get(adminId) || 0;

        const adminName = admin.netname || 'Admin';
        const targetName = target.netname || 'Unknown';
        const adminUserId = nodemod.eng.getPlayerUserId(admin);
        const adminAuthId = nodemod.eng.getPlayerAuthId(admin) || '';
        const targetUserId = nodemod.eng.getPlayerUserId(target);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';

        if (!utils.isAlive(target)) {
            this.sendChat(admin, this.getLang(admin, 'CANT_PERF_DEAD', targetName));
            return;
        }

        if (option === 0) {
            // Slay
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" slay "${targetName}<${targetUserId}><${targetAuthId}><>"`);
            this.showActivity(admin, this.getLang(null, 'ADMIN_SLAY_2', adminName, targetName));
            nodemod.dll.clientKill(target);
        } else {
            // Slap
            this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" slap with ${damage} damage "${targetName}<${targetUserId}><${targetAuthId}><>"`);
            this.showActivity(admin, this.getLang(null, 'ADMIN_SLAP_2', adminName, targetName, damage.toString()));

            const health = target.health || 0;
            if (health > damage) {
                target.health = health - damage;
                // Apply slap effect (velocity push)
                this.applySlapEffect(target);
            }
        }
    }

    private applySlapEffect(entity: nodemod.Entity) {
        try {
            if (entity.velocity) {
                entity.velocity[0] += (Math.random() - 0.5) * 300;
                entity.velocity[1] += (Math.random() - 0.5) * 300;
                entity.velocity[2] += 200;
            }
        } catch (e) {
            // Velocity manipulation may not be available
        }
    }

    // ==================== TEAM MENU ====================

    private cmdTeamMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_LEVEL_A)) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        this.menuPosition.set(entityId, 0);
        this.menuOption.set(entityId, 0);

        this.displayTeamMenu(entity, 0);
    }

    private displayTeamMenu(entity: nodemod.Entity, pos: number) {
        if (pos < 0) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        const targetTeamIdx = this.menuOption.get(entityId) || 0;

        const players = adminSystem.getPlayers({ excludeBots: false });
        const adminEntity = entity;
        const ITEMS_PER_PAGE = 7;  // 7 because we have the team option
        const start = pos * ITEMS_PER_PAGE;
        const total = players.length;

        if (start >= total && total > 0) {
            this.menuPosition.set(entityId, 0);
            this.displayTeamMenu(entity, 0);
            return;
        }

        const end = Math.min(start + ITEMS_PER_PAGE, total);
        const totalPages = Math.ceil(total / ITEMS_PER_PAGE) || 1;
        const currentPage = pos + 1;

        const items: any[] = [];

        // Player items for this page
        for (let a = start; a < end; a++) {
            const player = players[a];
            const name = player.netname || 'Unknown';
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== adminEntity;
            const isAdmin = adminSystem.hasAccess(player, ADMIN_LEVEL_A);
            const team = this.getPlayerTeamShort(player);
            const playerTeamNum = this.getPlayerTeamNumber(player);

            // Disable if already on target team or has immunity
            const isTargetTeam = playerTeamNum === TEAM_INT_NUMBERS[targetTeamIdx];

            items.push({
                name: isAdmin ? `${name} * [${team}]` : `${name} [${team}]`,
                disabled: hasImmunity || isTargetTeam,
                handler: (client: nodemod.Entity) => {
                    this.transferPlayer(client, player);
                    const clientId = nodemod.eng.indexOfEdict(client);
                    setTimeout(() => this.displayTeamMenu(client, this.menuPosition.get(clientId) || 0), 0);
                }
            });
        }

        // Team option
        items.push({
            name: `[${this.getLang(entity, 'TRANSF_TO', TEAM_NAMES[targetTeamIdx])}]`,
            handler: (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                let option = (this.menuOption.get(clientId) || 0) + 1;
                option %= 3; // 3 teams
                this.menuOption.set(clientId, option);
                setTimeout(() => this.displayTeamMenu(client, this.menuPosition.get(clientId) || 0), 0);
            }
        });

        // Navigation
        if (end < total) {
            items.push({
                name: this.getLangWithFallback(entity, 'MORE') + '...',
                handler: (client: nodemod.Entity) => {
                    const clientId = nodemod.eng.indexOfEdict(client);
                    const newPos = (this.menuPosition.get(clientId) || 0) + 1;
                    this.menuPosition.set(clientId, newPos);
                    setTimeout(() => this.displayTeamMenu(client, newPos), 0);
                }
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'TEAM_MENU')} ${currentPage}/${totalPages}`,
            items,
            onExit: pos > 0 ? (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                const newPos = (this.menuPosition.get(clientId) || 1) - 1;
                this.menuPosition.set(clientId, newPos);
                setTimeout(() => this.displayTeamMenu(client, newPos), 0);
            } : undefined,
            formatters: utils.coloredMenuFormatters
        });
    }

    private transferPlayer(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminId = nodemod.eng.indexOfEdict(admin);
        const teamIdx = this.menuOption.get(adminId) || 0;

        const adminName = admin.netname || 'Admin';
        const targetName = target.netname || 'Unknown';
        const adminUserId = nodemod.eng.getPlayerUserId(admin);
        const adminAuthId = nodemod.eng.getPlayerAuthId(admin) || '';
        const targetUserId = nodemod.eng.getPlayerUserId(target);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';

        this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" transfer "${targetName}<${targetUserId}><${targetAuthId}><>" (team "${TEAM_NAMES[teamIdx]}")`);
        this.showActivity(admin, this.getLang(null, 'ADMIN_TRANSF_2', adminName, targetName, TEAM_NAMES[teamIdx]));

        // Kill player if alive before transfer, preserving death count
        if (utils.isAlive(target)) {
            // Save current death count before killing
            const deathCount = this.getPlayerDeaths(target);

            // Kill the player (this increments death count)
            nodemod.dll.clientKill(target);

            // Restore original death count
            this.setPlayerDeaths(target, deathCount);
        }

        // Map menu index to CS team constant
        const csTeam = teamIdx === 0 ? CS_TEAM_T : teamIdx === 1 ? CS_TEAM_CT : CS_TEAM_SPECTATOR;

        // Use proper CS team transfer
        this.csSetUserTeam(target, csTeam);
    }

    /**
     * Get player death count (CS-specific)
     */
    private getPlayerDeaths(entity: nodemod.Entity): number {
        try {
            // Try to read from CS-specific private data offset for deaths
            // This offset may vary by game version
            const CS_OFFSET_DEATHS = 444;  // Typical offset for m_iDeaths in CS 1.6
            const buffer = entity.getPrivateDataBuffer(CS_OFFSET_DEATHS, 4);
            if (buffer) {
                return buffer.readInt32LE(0);
            }
        } catch (e) {
            // Fall back to frags if available (some mods use negative frags for deaths)
        }
        return 0;
    }

    /**
     * Set player death count (CS-specific)
     */
    private setPlayerDeaths(entity: nodemod.Entity, deaths: number): void {
        try {
            const CS_OFFSET_DEATHS = 444;  // Typical offset for m_iDeaths in CS 1.6
            const buffer = Buffer.alloc(4);
            buffer.writeInt32LE(deaths, 0);
            entity.writePrivateDataBuffer(CS_OFFSET_DEATHS, buffer);

            // Also send ScoreInfo message to update scoreboard
            const playerIndex = nodemod.eng.indexOfEdict(entity);
            const frags = Math.floor(entity.frags || 0);
            const team = entity.team || 0;

            nodemodCore.msg.send({
                type: 'ScoreInfo',
                dest: 2,  // MSG_ALL
                data: [
                    { type: 'byte', value: playerIndex },
                    { type: 'short', value: frags },
                    { type: 'short', value: deaths },
                    { type: 'short', value: 0 },  // class id
                    { type: 'short', value: team }
                ]
            });
        } catch (e) {
            // Private data write not available
        }
    }

    /**
     * Set user team for Counter-Strike.
     * Equivalent to AMX Mod X's cs_set_user_team native.
     * Sets the internal team value and broadcasts TeamInfo message.
     *
     * @param entity - Target player entity
     * @param team - CS team constant (CS_TEAM_T, CS_TEAM_CT, CS_TEAM_SPECTATOR)
     * @param sendTeamInfo - Whether to broadcast TeamInfo message (default: true)
     */
    private csSetUserTeam(entity: nodemod.Entity, team: number, sendTeamInfo: boolean = true): boolean {
        try {
            const playerIndex = nodemod.eng.indexOfEdict(entity);

            // Set the entvars team property
            entity.team = team;

            // Try to set internal CBasePlayer::m_iTeam via private data
            // This is CS-specific and may not work on all mods
            try {
                const teamBuffer = Buffer.alloc(4);
                teamBuffer.writeInt32LE(team, 0);
                entity.writePrivateDataBuffer(CS_OFFSET_TEAM, teamBuffer);
            } catch (e) {
                // Private data write not available, continue with entvars only
            }

            // Broadcast TeamInfo message to all players
            if (sendTeamInfo) {
                let teamInfo: string;
                switch (team) {
                    case CS_TEAM_UNASSIGNED: teamInfo = 'UNASSIGNED'; break;
                    case CS_TEAM_T: teamInfo = 'TERRORIST'; break;
                    case CS_TEAM_CT: teamInfo = 'CT'; break;
                    case CS_TEAM_SPECTATOR: teamInfo = 'SPECTATOR'; break;
                    default: teamInfo = `TEAM_${team}`;
                }

                nodemodCore.msg.send({
                    type: 'TeamInfo',
                    dest: 2,  // MSG_ALL
                    data: [
                        { type: 'byte', value: playerIndex },
                        { type: 'string', value: teamInfo }
                    ]
                });
            }

            // Force model update via client command (standard way across all mods)
            nodemod.eng.clientCommand(entity, `joinclass 1\n`);

            return true;
        } catch (e) {
            console.error('[PlMenu] csSetUserTeam error:', e);
            return false;
        }
    }

    // Team helpers delegated to utils.ts for code reuse
    private getPlayerTeamShort(entity: nodemod.Entity): string {
        return utils.getPlayerTeamShort(entity);
    }

    private getPlayerTeamNumber(entity: nodemod.Entity): number {
        return utils.getPlayerTeamNumber(entity);
    }

    // ==================== CLIENT COMMANDS MENU ====================

    private cmdClcmdMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_LEVEL_A)) return;

        // Build list of commands this admin has access to
        const entityId = nodemod.eng.indexOfEdict(entity);
        const availableCmds: number[] = [];

        for (let i = 0; i < this.clientCommands.length; i++) {
            if (adminSystem.hasAccess(entity, this.clientCommands[i].access)) {
                availableCmds.push(i);
            }
        }

        this.menuSelect.set(entityId, availableCmds);
        this.menuSelectNum.set(entityId, availableCmds.length);
        this.menuPosition.set(entityId, 0);
        this.menuOption.set(entityId, 0);

        this.displayClcmdMenu(entity, 0);
    }

    private displayClcmdMenu(entity: nodemod.Entity, pos: number) {
        if (pos < 0) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        const currentOption = this.menuOption.get(entityId) || 0;
        const availableCmds = this.menuSelect.get(entityId) || [];
        const numCmds = this.menuSelectNum.get(entityId) || 0;

        const players = adminSystem.getPlayers({ excludeBots: false });
        const adminEntity = entity;
        const ITEMS_PER_PAGE = 7;  // 7 because we have the command option
        const start = pos * ITEMS_PER_PAGE;
        const total = players.length;

        if (start >= total && total > 0) {
            this.menuPosition.set(entityId, 0);
            this.displayClcmdMenu(entity, 0);
            return;
        }

        const end = Math.min(start + ITEMS_PER_PAGE, total);
        const totalPages = Math.ceil(total / ITEMS_PER_PAGE) || 1;
        const currentPage = pos + 1;

        const items: any[] = [];

        // Player items for this page
        for (let a = start; a < end; a++) {
            const player = players[a];
            const name = player.netname || 'Unknown';
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== adminEntity;
            const isAdmin = adminSystem.hasAccess(player, ADMIN_LEVEL_A);

            items.push({
                name: isAdmin ? `${name} *` : name,
                disabled: numCmds === 0 || hasImmunity,
                handler: (client: nodemod.Entity) => {
                    this.executeClcmd(client, player);
                }
            });
        }

        // Command option
        const cmdLabel = numCmds > 0
            ? this.clientCommands[availableCmds[currentOption]].name
            : this.getLang(entity, 'NO_CMDS');

        items.push({
            name: `[${cmdLabel}]`,
            handler: (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                const num = this.menuSelectNum.get(clientId) || 0;
                if (num > 0) {
                    let option = (this.menuOption.get(clientId) || 0) + 1;
                    option %= num;
                    this.menuOption.set(clientId, option);
                }
                setTimeout(() => this.displayClcmdMenu(client, this.menuPosition.get(clientId) || 0), 0);
            }
        });

        // Navigation
        if (end < total) {
            items.push({
                name: this.getLangWithFallback(entity, 'MORE') + '...',
                handler: (client: nodemod.Entity) => {
                    const clientId = nodemod.eng.indexOfEdict(client);
                    const newPos = (this.menuPosition.get(clientId) || 0) + 1;
                    this.menuPosition.set(clientId, newPos);
                    setTimeout(() => this.displayClcmdMenu(client, newPos), 0);
                }
            });
        }

        nodemodCore.menu.show({
            entity,
            title: `${this.getLang(entity, 'CL_CMD_MENU')} ${currentPage}/${totalPages}`,
            items,
            onExit: pos > 0 ? (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);
                const newPos = (this.menuPosition.get(clientId) || 1) - 1;
                this.menuPosition.set(clientId, newPos);
                setTimeout(() => this.displayClcmdMenu(client, newPos), 0);
            } : undefined,
            formatters: utils.coloredMenuFormatters
        });
    }

    private executeClcmd(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminId = nodemod.eng.indexOfEdict(admin);
        const currentOption = this.menuOption.get(adminId) || 0;
        const availableCmds = this.menuSelect.get(adminId) || [];

        if (availableCmds.length === 0) return;

        // Check if target is still connected before executing command
        if (!target || !target.netname) {
            this.sendChat(admin, this.getLang(admin, 'PLAYER_NOT_FOUND'));
            setTimeout(() => this.displayClcmdMenu(admin, this.menuPosition.get(adminId) || 0), 0);
            return;
        }

        const cmd = this.clientCommands[availableCmds[currentOption]];
        const flags = cmd.flags;

        // Get target info for replacement
        const authId = nodemod.eng.getPlayerAuthId(target);
        const name = target.netname || 'Unknown';
        const userId = nodemod.eng.getPlayerUserId(target).toString();

        // Replace placeholders
        let command = cmd.command;
        command = command.replace(/%userid%/g, userId);
        command = command.replace(/%authid%/g, authId);
        command = command.replace(/%name%/g, name);

        // Execute based on flags
        if (flags & 1) {
            // Server command
            nodemod.eng.serverCommand(`${command}\n`);
        } else if (flags & 2) {
            // Admin client command
            nodemod.eng.clientCommand(admin, `${command}\n`);
        } else if (flags & 4) {
            // Target client command
            nodemod.eng.clientCommand(target, `${command}\n`);
        }

        // Repeat menu if flag 8 is set
        if (flags & 8) {
            setTimeout(() => this.displayClcmdMenu(admin, this.menuPosition.get(adminId) || 0), 0);
        }
    }

    // ==================== PUBLIC API ====================

    /**
     * Set ban times for ban menu
     */
    public setBanTimes(times: number[]) {
        this.banTimes = times;
    }

    /**
     * Set slap damage values for slap menu
     */
    public setSlapDamage(damages: number[]) {
        // First value is always 0 (slay)
        this.slapSettings = [0, ...damages];
    }
}



export default PlMenu;
