// NextMap Plugin
// Converted from AMX Mod X nextmap.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

class NextMap extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'NextMap',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Mapcycle and nextmap management'
    };

    // Next map name
    private nextMap: string = '';

    // Mapcycle file path
    private mapCycleFile: string = '';

    // Position in mapcycle
    private mapCyclePos: number = 0;

    // Full mapcycle list (for validation at changelevel time)
    private mapCycle: string[] = [];

    // CVARs
    private amxNextmap: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Register CVARs
        this.registerCvar('amx_nextmap', '', nodemod.FCVAR.SERVER, 'Next map to change to');
        this.amxNextmap = cvar.wrap('amx_nextmap');

        // Load mapcycle settings
        this.loadMapCycleSettings();

        // Register say commands
        this.registerSayCommands();

        // Register for map end event
        this.registerEvents();
    }

    private getStateFile(): string {
        return path.join(utils.getConfigsDir(), 'nextmap_state.txt');
    }

    private loadMapCycleSettings() {
        // Get mapcycle file
        this.mapCycleFile = cvar.getString('mapcyclefile') || 'mapcycle.txt';

        // Load last position from state file
        const stateFile = this.getStateFile();
        try {
            if (fs.existsSync(stateFile)) {
                const content = fs.readFileSync(stateFile, 'utf-8').trim();
                const parts = content.split(' ');
                const savedCycleFile = parts[0] || '';
                const savedPos = parseInt(parts[1]) || 0;

                // If mapcycle file changed, reset position
                if (savedCycleFile === this.mapCycleFile) {
                    this.mapCyclePos = savedPos;
                } else {
                    this.mapCyclePos = 0;
                }
            }
        } catch (e) {
            this.mapCyclePos = 0;
        }

        // Read mapcycle and set next map
        this.readMapCycle();

        // Save state
        this.saveState();
    }

    private saveState() {
        const stateFile = this.getStateFile();
        try {
            fs.writeFileSync(stateFile, `${this.mapCycleFile} ${this.mapCyclePos}`);
        } catch (e) {
            console.error('[NextMap] Error saving state:', e);
        }
    }

    private readMapCycle() {
        // Try to find mapcycle file - first in game directory, then configs
        let mapCyclePath = path.join(utils.getGameDir(), this.mapCycleFile);

        if (!fs.existsSync(mapCyclePath)) {
            // Try configs directory
            mapCyclePath = path.join(utils.getConfigsDir(), this.mapCycleFile);
        }

        // Load maps from file using shared utility (don't skip any - we need full list)
        const validMaps = utils.loadMapsFromFile(mapCyclePath);

        if (validMaps.length === 0) {
            // Log warning like original SMA
            this.logAmx(`WARNING: Couldn't find a valid map or the file doesn't exist (file "${mapCyclePath}")`);
            // Default to current map
            this.nextMap = nodemod.mapname || 'crossfire';
            cvar.setString('amx_nextmap', this.nextMap);
            return;
        }

        // Store the full mapcycle for validation at changelevel time
        this.mapCycle = validMaps;

        // OBEY_MAPCYCLE mode: go sequentially through the list
        if (this.mapCyclePos >= this.mapCycle.length) {
            this.mapCyclePos = 0;
        }

        // Find next map, skipping current map (unless it's the only one)
        const currentMap = nodemod.mapname || '';
        let nextMap = this.mapCycle[this.mapCyclePos];

        if (this.mapCycle.length > 1 && nextMap.toLowerCase() === currentMap.toLowerCase()) {
            this.mapCyclePos = (this.mapCyclePos + 1) % this.mapCycle.length;
            nextMap = this.mapCycle[this.mapCyclePos];
        }

        this.nextMap = nextMap;
        this.mapCyclePos++;

        cvar.setString('amx_nextmap', this.nextMap);
    }

    private registerSayCommands() {
        // Hook into client commands to catch say commands
        nodemod.on('dllClientCommand', (entity: nodemod.Entity, text: string) => {
            const args = utils.parseCommand(text);
            if (args.length === 0) return;

            const cmd = args[0].toLowerCase();
            if (cmd !== 'say') return;

            const message = args.slice(1).join(' ').replace(/^"|"$/g, '').toLowerCase();

            if (message === 'nextmap') {
                this.sayNextMap(entity);
            } else if (message === 'currentmap') {
                this.sayCurrentMap(entity);
            } else if (message === 'ff') {
                this.sayFFStatus(entity);
            }
        });
    }

    private registerEvents() {
        // Track if we've already triggered map change to avoid duplicates
        let mapChangeTriggered = false;

        // Hook engMessageBegin to detect SVC_INTERMISSION (msg_type=30)
        // This is equivalent to register_event("30", "changeMap", "a") in AMXX
        nodemod.on('engMessageBegin', (msg_dest: number, msg_type: number, pOrigin: number[], ed: nodemod.Entity | null) => {
            // MSG_TYPE.INTERMISSION = 30
            if (msg_type === 30 && !mapChangeTriggered) {
                mapChangeTriggered = true;
                console.log(`[NextMap] SVC_INTERMISSION detected, triggering map change`);
                this.changeMap();
            }
        });

        // Reset flag on server deactivate (new map loading)
        nodemod.on('dllServerDeactivate', () => {
            mapChangeTriggered = false;
            this.saveState();
        });
    }

    private getNextMapName(): string {
        // Check if amx_nextmap has been set externally
        const cvarNextMap = cvar.getString('amx_nextmap') || '';

        if (cvarNextMap) {
            return cvarNextMap;
        }

        // Fall back to our tracked next map
        return this.nextMap || nodemod.mapname || 'unknown';
    }

    private sayNextMap(entity: nodemod.Entity | null) {
        const nextMapName = this.getNextMapName();
        const message = this.getLang(null, 'NEXT_MAP', nextMapName);

        // Send to all players
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            this.sendChat(player, message);
        }
    }

    private sayCurrentMap(entity: nodemod.Entity | null) {
        const currentMap = nodemod.mapname || 'unknown';
        const message = this.getLang(null, 'PLAYED_MAP', currentMap);

        // Send to all players
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            this.sendChat(player, message);
        }
    }

    private sayFFStatus(entity: nodemod.Entity | null) {
        const ffEnabled = cvar.getInt('mp_friendlyfire') || 0;
        const status = ffEnabled ? this.getLang(null, 'ON') : this.getLang(null, 'OFF');
        const message = this.getLang(null, 'FRIEND_FIRE', status);

        // Send to all players
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            this.sendChat(player, message);
        }
    }

    private changeMap() {
        // Get the next map directly from amx_nextmap (set by mapchooser vote)
        // Don't use findValidNextMap which has fallback logic that might override the vote
        let nextMapName = cvar.getString('amx_nextmap') || '';

        // Validate the map exists
        if (!nextMapName || !utils.isMapValid(nextMapName)) {
            // Fall back to finding a valid map
            nextMapName = this.findValidNextMap();
        }

        const chatTime = cvar.getFloat('mp_chattime') || 10;

        // Extend chat time by 2 seconds so engine waits longer
        cvar.setFloat('mp_chattime', chatTime + 2.0);

        console.log(`[NextMap] Changing to: ${nextMapName} in ${chatTime} seconds`);

        // Wait for slightly less than original chattime to beat the engine's automatic changelevel
        const delayMs = Math.max((chatTime - 0.5) * 1000, 1000);

        setTimeout(() => {
            console.log(`[NextMap] setTimeout fired, executing changelevel ${nextMapName}`);

            // Restore original chat time
            cvar.setFloat('mp_chattime', chatTime);

            // Execute changelevel and force immediate processing
            nodemod.eng.serverCommand(`changelevel ${nextMapName}\n`);
            nodemod.eng.serverExecute();
        }, delayMs);
    }

    /**
     * Find a valid next map, skipping invalid maps in the cycle
     */
    private findValidNextMap(): string {
        const currentMap = nodemod.mapname || '';

        // First check if externally set nextmap is valid (and not current map)
        const cvarNextMap = cvar.getString('amx_nextmap') || '';
        if (cvarNextMap && utils.isMapValid(cvarNextMap)) {
            // Allow current map only if it's the only option
            if (this.mapCycle.length <= 1 || cvarNextMap.toLowerCase() !== currentMap.toLowerCase()) {
                return cvarNextMap;
            }
        }

        // If no mapcycle loaded, fall back to current map
        if (this.mapCycle.length === 0) {
            return currentMap || 'crossfire';
        }

        // Find a valid map starting from current position, skipping current map
        const startPos = this.mapCyclePos > 0 ? this.mapCyclePos - 1 : 0;
        for (let i = 0; i < this.mapCycle.length; i++) {
            const idx = (startPos + i) % this.mapCycle.length;
            const mapName = this.mapCycle[idx];

            // Skip current map unless it's the only one
            if (this.mapCycle.length > 1 && mapName.toLowerCase() === currentMap.toLowerCase()) {
                continue;
            }

            if (utils.isMapValid(mapName)) {
                // Update position for next cycle
                this.mapCyclePos = idx + 1;
                this.nextMap = mapName;
                cvar.setString('amx_nextmap', mapName);
                this.saveState();
                return mapName;
            }
        }

        // No valid maps found, log warning and fall back to current map
        this.logAmx(`WARNING: Couldn't find a valid map in the mapcycle`);
        return currentMap || 'crossfire';
    }

    /**
     * Public method to get the next map
     */
    public getNextMap(): string {
        return this.getNextMapName();
    }

    /**
     * Public method to set the next map
     */
    public setNextMap(mapName: string): boolean {
        if (!utils.isMapValid(mapName)) {
            return false;
        }

        this.nextMap = mapName;
        cvar.setString('amx_nextmap', mapName);
        return true;
    }
}



export default NextMap;
