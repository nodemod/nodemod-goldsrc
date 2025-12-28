// Teleport Menu Plugin
// Converted from AMX Mod X telemenu.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { ADMIN_CFG, ADMIN_IMMUNITY } from './constants';

import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

// Location mode
const LOCATION_CURRENT = 0;     // Teleport to admin's current location
const LOCATION_SAVED = 1;       // Teleport to saved location
const LOCATION_UNSET = -1;      // Location not set yet

class TeleMenu extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Teleport Menu',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Teleport players menu'
    };

    // Per-player menu state
    private menuOption: Map<number, number> = new Map();        // Location mode
    private savedOrigin: Map<number, number[]> = new Map();     // Saved origin

    constructor(pluginName: string) {
        super(pluginName);

        // Register commands
        this.registerCommand('amx_teleportmenu', ADMIN_CFG, '- displays teleport menu', (entity, args) => {
            this.cmdTelMenu(entity);
        });
    }

    private cmdTelMenu(entity: nodemod.Entity | null) {
        if (!entity) {
            console.log('[TeleMenu] Teleport menu is only available for players');
            return;
        }
        if (!adminSystem.cmdAccess(entity, ADMIN_CFG)) return;

        const entityId = nodemod.eng.indexOfEdict(entity);
        // Initialize menu option to LOCATION_UNSET if not set
        if (!this.menuOption.has(entityId)) {
            this.menuOption.set(entityId, LOCATION_UNSET);
        }

        this.displayTelMenu(entity);
    }

    private displayTelMenu(entity: nodemod.Entity) {
        const entityId = nodemod.eng.indexOfEdict(entity);
        const locationMode = this.menuOption.get(entityId) ?? LOCATION_UNSET;
        const isAdminAlive = utils.isAlive(entity);

        // Block menu if admin is alive and not using saved location mode
        // This matches SMA logic: block when (is_user_alive(id) && g_menuOption[id] < 1)
        // LOCATION_UNSET=-1, LOCATION_CURRENT=0 are both < 1 (blocked)
        // LOCATION_SAVED=1 is not < 1 (not blocked)
        const blockMenu = isAdminAlive && locationMode !== LOCATION_SAVED;

        const players = adminSystem.getPlayers({ excludeBots: false });
        const items: any[] = [];

        // Player items
        for (const player of players) {
            const name = player.netname || 'Unknown';
            const isAlive = utils.isAlive(player);
            const isBot = utils.isBot(player);
            const hasImmunity = adminSystem.hasAccess(player, ADMIN_IMMUNITY) && player !== entity;
            const isAdmin = utils.isUserAdmin(adminSystem.getUserFlags(player));

            // Determine if player is disabled in the menu (bots bypass alive check)
            const disabled = blockMenu || (!isAlive && !isBot) || hasImmunity;

            items.push({
                name: isAdmin ? `${name} *` : name,
                disabled: disabled,
                handler: (client: nodemod.Entity) => {
                    this.teleportPlayer(client, player);
                }
            });
        }

        // Location mode option
        const savedPos = this.savedOrigin.get(entityId);

        if (locationMode === LOCATION_SAVED && savedPos) {
            // Show saved location coordinates (preserve precision like SMA)
            items.push({
                name: this.getLangWithFallback(entity, 'TO_LOC', savedPos[0].toFixed(1), savedPos[1].toFixed(1), savedPos[2].toFixed(1)),
                handler: (client: nodemod.Entity) => {
                    // Toggle back to current location mode
                    const clientId = nodemod.eng.indexOfEdict(client);
                    this.menuOption.set(clientId, LOCATION_CURRENT);
                    setTimeout(() => this.displayTelMenu(client), 0);
                }
            });
        } else if (locationMode === LOCATION_UNSET) {
            // Location not set yet - disabled
            items.push({
                name: this.getLangWithFallback(entity, 'CUR_LOC'),
                disabled: true,
                handler: () => { }
            });
        } else {
            // Current location mode
            items.push({
                name: this.getLangWithFallback(entity, 'CUR_LOC'),
                handler: (client: nodemod.Entity) => {
                    // Toggle to saved location mode if we have a saved location
                    const clientId = nodemod.eng.indexOfEdict(client);
                    if (this.savedOrigin.has(clientId)) {
                        this.menuOption.set(clientId, LOCATION_SAVED);
                    }
                    setTimeout(() => this.displayTelMenu(client), 0);
                }
            });
        }

        // Save location option
        items.push({
            name: this.getLangWithFallback(entity, 'SAVE_LOC'),
            handler: (client: nodemod.Entity) => {
                const clientId = nodemod.eng.indexOfEdict(client);

                // If first time saving, switch to current location mode
                if (this.menuOption.get(clientId) === LOCATION_UNSET) {
                    this.menuOption.set(clientId, LOCATION_CURRENT);
                }

                // Save current origin
                const origin = client.origin;
                if (origin) {
                    this.savedOrigin.set(clientId, [...origin]);
                }

                setTimeout(() => this.displayTelMenu(client), 0);
            }
        });

        nodemodCore.menu.show({
            entity,
            title: this.getLangWithFallback(entity, 'TELE_MENU'),
            items,
            formatters: utils.coloredMenuFormatters
        });
    }

    private teleportPlayer(admin: nodemod.Entity, target: nodemod.Entity) {
        const adminId = nodemod.eng.indexOfEdict(admin);
        const adminName = admin.netname || 'Admin';
        const targetName = target.netname || 'Unknown';

        // Check if target is alive
        if (!utils.isAlive(target)) {
            this.sendChat(admin, this.getLangWithFallback(admin, 'CANT_PERF_DEAD', targetName));
            setTimeout(() => this.displayTelMenu(admin), 0);
            return;
        }

        // Determine destination
        const locationMode = this.menuOption.get(adminId) ?? LOCATION_CURRENT;
        let destination: number[];

        if (locationMode === LOCATION_SAVED) {
            const savedPos = this.savedOrigin.get(adminId);
            if (!savedPos) {
                // Fallback to admin's current location
                destination = admin.origin ? [...admin.origin] : [0, 0, 0];
            } else {
                destination = savedPos;
            }
        } else {
            // Use admin's current location
            destination = admin.origin ? [...admin.origin] : [0, 0, 0];
        }

        // Teleport the player
        nodemod.eng.setOrigin(target, destination);

        // Get auth IDs for logging
        const adminAuthId = nodemod.eng.getPlayerAuthId(admin);
        const adminUserId = nodemod.eng.getPlayerUserId(admin);
        const targetAuthId = nodemod.eng.getPlayerAuthId(target);
        const targetUserId = nodemod.eng.getPlayerUserId(target);

        // Log the action
        this.logAmx(`Cmd: "${adminName}<${adminUserId}><${adminAuthId}><>" teleport "${targetName}<${targetUserId}><${targetAuthId}><>"`);

        // Show activity
        this.showActivity(admin, this.getLang(null, 'ADMIN_TELEPORT_2', adminName, targetName));

        // Re-display menu
        setTimeout(() => this.displayTelMenu(admin), 0);
    }

    /**
     * Plugin unload - cleanup per-player data
     */
    onUnload() {
        this.menuOption.clear();
        this.savedOrigin.clear();
    }
}



export default TeleMenu;
