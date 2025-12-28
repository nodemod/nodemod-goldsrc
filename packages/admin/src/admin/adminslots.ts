// Admin Slots Plugin
// Converted from AMX Mod X adminslots.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_RESERVATION } from './constants';
import localization from './localization';
import * as utils from './utils';
import { Plugin, PluginMetadata } from './pluginloader';
import { BasePlugin } from './baseplugin';

const cvar = nodemodCore.cvar;

// ============================================================================
// Admin Slots Plugin
// ============================================================================

class AdminSlots extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Slots',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Reserved slots for admins'
    };

    // Pending auth players (player index -> entity)
    private pendingAuth: Map<number, nodemod.Entity> = new Map();

    // Auth check timer
    private authCheckTimer: ReturnType<typeof setInterval> | null = null;

    constructor(pluginName: string) {
        super(pluginName);

        // Register CVARs
        this.registerCvar('amx_reservation', '0', nodemod.FCVAR.SERVER, 'Number of reserved slots for admins');
        this.registerCvar('amx_hideslots', '0', nodemod.FCVAR.SERVER, 'Hide unused slots (0=off, 1=on)');

        // Load localization
        localization.loadDictionary(this.pluginName);

        // Setup event handlers
        this.setupEventHandlers();

        // Start auth polling timer (every 0.7s like AMXX)
        this.authCheckTimer = setInterval(() => this.checkPendingAuth(), 700);

        // Map loaded task (equivalent to plugin_cfg + set_task)
        setTimeout(() => {
            this.onMapLoaded();
        }, 3000);
    }

    private setupEventHandlers() {
        // Client connect - check if auth is immediately available or add to pending
        // This mirrors AMXX's C_ClientConnect_Post behavior
        nodemod.on('dllClientConnect', (entity: nodemod.Entity) => {
            if (utils.isBot(entity)) return;

            const authId = nodemod.eng.getPlayerAuthId(entity) || '';

            // Check if auth is already available (not pending)
            if (authId && authId !== 'STEAM_ID_PENDING' && authId !== '') {
                // Auth available immediately - process now
                this.onClientAuthorized(entity);
            } else {
                // Auth pending - add to list for polling
                const index = nodemod.eng.indexOfEdict(entity);
                this.pendingAuth.set(index, entity);
            }
        });

        // Client disconnect - update visible slots and remove from pending
        nodemod.on('dllClientDisconnect', (entity: nodemod.Entity) => {
            const index = nodemod.eng.indexOfEdict(entity);
            this.pendingAuth.delete(index);
            this.onClientDisconnect(entity);
        });
    }

    /**
     * Check pending auth players - mirrors AMXX's C_StartFrame_Post polling
     */
    private checkPendingAuth() {
        for (const [index, entity] of this.pendingAuth) {
            const authId = nodemod.eng.getPlayerAuthId(entity) || '';

            // Check if auth completed
            if (authId && authId !== 'STEAM_ID_PENDING' && authId !== '') {
                this.pendingAuth.delete(index);
                this.onClientAuthorized(entity);
            }
        }
    }


    private getMaxPlayers(): number {
        return cvar.getInt('maxplayers') || 32;
    }

    private getPlayerCount(): number {
        return nodemod.players.length;
    }

    private onMapLoaded() {
        const hideSlots = cvar.getInt('amx_hideslots') || 0;
        if (!hideSlots) return;

        const maxPlayers = this.getMaxPlayers();
        const players = this.getPlayerCount();
        const reservation = cvar.getInt('amx_reservation') || 0;
        const limit = maxPlayers - reservation;

        this.setVisibleSlots(players, maxPlayers, limit);
    }

    private onClientAuthorized(entity: nodemod.Entity) {
        if (utils.isBot(entity)) return;

        const maxPlayers = this.getMaxPlayers();
        const players = this.getPlayerCount();
        const reservation = cvar.getInt('amx_reservation') || 0;
        const limit = maxPlayers - reservation;

        // Check if player has reservation access or if there's room
        const userFlags = adminSystem.getUserFlags(entity);
        const hasReservation = (userFlags & ADMIN_RESERVATION) !== 0;

        if (hasReservation || players <= limit) {
            // Player can stay
            const hideSlots = cvar.getInt('amx_hideslots') || 0;
            if (hideSlots === 1) {
                this.setVisibleSlots(players, maxPlayers, limit);
            }
            return;
        }

        // No room and no reservation - kick the player
        const reason = this.getLang(entity, 'DROPPED_RES');
        const userId = nodemod.eng.getPlayerUserId(entity);

        nodemod.eng.serverCommand(`kick #${userId} "${reason}"\n`);
    }

    private onClientDisconnect(entity: nodemod.Entity) {
        const hideSlots = cvar.getInt('amx_hideslots') || 0;
        if (!hideSlots) return;

        const maxPlayers = this.getMaxPlayers();
        const reservation = cvar.getInt('amx_reservation') || 0;
        const players = this.getPlayerCount() - 1; // -1 because this player is leaving

        this.setVisibleSlots(players, maxPlayers, maxPlayers - reservation);
    }

    private setVisibleSlots(players: number, maxPlayers: number, limit: number) {
        let num = players + 1;

        if (players === maxPlayers) {
            num = maxPlayers;
        } else if (players < limit) {
            num = limit;
        }

        cvar.setString('sv_visiblemaxplayers', String(num));
    }

    onUnload() {
        if (this.authCheckTimer) {
            clearInterval(this.authCheckTimer);
            this.authCheckTimer = null;
        }
        this.pendingAuth.clear();
    }
}



export default AdminSlots;
