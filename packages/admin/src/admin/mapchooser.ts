// Nextmap Chooser Plugin
// Converted from AMX Mod X mapchooser.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO
//
// Implementation Notes:
// - Sound playback uses per-player loop instead of broadcast (client_cmd(0, ...))
//   This is intentional to allow filtering (excludeBots) and provides equivalent behavior.
// - State persistence uses file-based storage instead of localinfo
//   This persists across server restarts, not just map changes.

import * as fs from 'fs';
import * as path from 'path';
import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

// Number of maps to show in voting menu
const SELECT_MAPS = 5;

class MapChooser extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Nextmap Chooser',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Automatic nextmap voting system'
    };

    // Map list loaded from config
    private mapNames: string[] = [];

    // Indices of maps selected for current vote
    private nextMapIndices: number[] = [];

    // Vote counts for each option (5 maps + extend + none)
    private voteCounts: number[] = [];

    // Number of maps in current vote
    private mapVoteNum: number = 0;

    // Team scores for CS-style games (CT, T)
    private teamScores: [number, number] = [0, 0];

    // Last played map (to avoid repeat)
    private lastMap: string = '';

    // Whether voting has already been triggered this map
    private voteTriggered: boolean = false;

    // Timer for periodic check
    private checkTimerId: ReturnType<typeof setInterval> | null = null;

    // Players who have already voted
    private votedPlayers: Set<number> = new Set();

    // CVARs
    private amxExtendmapMax: any;
    private amxExtendmapStep: any;
    private amxNextmap: any;
    private mpTimelimit: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Register CVARs
        this.registerCvar('amx_extendmap_max', '90', nodemod.FCVAR.SERVER, 'Maximum timelimit for map extension');
        this.registerCvar('amx_extendmap_step', '15', nodemod.FCVAR.SERVER, 'Minutes to extend map by');

        this.amxExtendmapMax = cvar.wrap('amx_extendmap_max');
        this.amxExtendmapStep = cvar.wrap('amx_extendmap_step');
        this.amxNextmap = cvar.wrap('amx_nextmap');
        this.mpTimelimit = cvar.wrap('mp_timelimit');

        // Register for CS TeamScore event if available
        this.registerTeamScoreEvent();

        // Load last map from localinfo (simulated via file)
        this.loadLastMap();

        // Load map list
        if (this.loadSettings()) {
            // Start periodic check every 15 seconds
            this.checkTimerId = setInterval(() => {
                this.voteNextmap();
            }, 15000);
        }
    }

    private registerTeamScoreEvent() {
        // Register for TeamScore game event (CS-specific)
        // This tracks round wins for winlimit/maxrounds logic
        try {
            // Cast to any since TeamScore is a game-specific event
            (nodemodCore.events as any).on('TeamScore', (data: any) => {
                if (data && data.team) {
                    const teamIndex = data.team === 'CT' ? 0 : 1;
                    this.teamScores[teamIndex] = data.score || 0;
                }
            });
        } catch (e) {
            // Event may not be available for non-CS games
        }
    }

    private loadLastMap() {
        // Try to read last map from a state file
        const stateFile = path.join(utils.getConfigsDir(), 'mapchooser_state.txt');
        try {
            if (fs.existsSync(stateFile)) {
                this.lastMap = fs.readFileSync(stateFile, 'utf-8').trim();
                // Clear it after reading
                fs.writeFileSync(stateFile, '');
            }
        } catch (e) {
            // Ignore errors
        }
    }

    private saveLastMap() {
        // Save current map as last map for next session
        const stateFile = path.join(utils.getConfigsDir(), 'mapchooser_state.txt');
        try {
            const currentMap = nodemod.mapname || '';
            fs.writeFileSync(stateFile, currentMap);
        } catch (e) {
            console.error('[MapChooser] Error saving last map:', e);
        }
    }

    private loadSettings(): boolean {
        const currentMap = nodemod.mapname || '';
        const skipMaps = [currentMap];
        if (this.lastMap) {
            skipMaps.push(this.lastMap);
        }

        this.mapNames = utils.loadMapList({ skipMaps });
        return this.mapNames.length > 0;
    }

    private getTimeLeft(): number {
        const timeLimit = this.mpTimelimit.float || 0;
        if (timeLimit <= 0) return -1; // No time limit
        return utils.getTimeLeft();
    }

    private voteNextmap() {
        const winLimit = cvar.getInt('mp_winlimit') || 0;
        const maxRounds = cvar.getInt('mp_maxrounds') || 0;

        // Check if we should trigger a vote based on game conditions
        if (winLimit > 0) {
            // Check win limit (vote when 2 rounds from winning)
            const threshold = winLimit - 2;
            if (this.teamScores[0] < threshold && this.teamScores[1] < threshold) {
                this.voteTriggered = false;
                return;
            }
        } else if (maxRounds > 0) {
            // Check max rounds (vote when 2 rounds from max)
            const totalRounds = this.teamScores[0] + this.teamScores[1];
            if (totalRounds < maxRounds - 2) {
                this.voteTriggered = false;
                return;
            }
        } else {
            // Check time limit (vote when 1-129 seconds remaining)
            const timeLeft = this.getTimeLeft();
            if (timeLeft < 1 || timeLeft > 129) {
                this.voteTriggered = false;
                return;
            }
        }

        // Don't vote twice
        if (this.voteTriggered) return;
        this.voteTriggered = true;

        // Start the vote
        this.startVote();
    }

    private startVote() {
        this.votedPlayers.clear();

        // Select random maps for voting
        const maxMaps = Math.min(this.mapNames.length, SELECT_MAPS);
        this.nextMapIndices = [];
        this.voteCounts = new Array(SELECT_MAPS + 2).fill(0);
        this.mapVoteNum = 0;

        for (let i = 0; i < maxMaps; i++) {
            let randomIndex: number;
            do {
                randomIndex = Math.floor(Math.random() * this.mapNames.length);
            } while (this.nextMapIndices.includes(randomIndex));

            this.nextMapIndices.push(randomIndex);
            this.mapVoteNum++;
        }

        // Build menu items
        const items: any[] = [];

        // Add map choices
        for (let i = 0; i < this.mapVoteNum; i++) {
            const mapIndex = this.nextMapIndices[i];
            const mapName = this.mapNames[mapIndex];
            const optionIndex = i;

            items.push({
                name: mapName,
                handler: (client: nodemod.Entity) => {
                    this.countVote(client, optionIndex);
                }
            });
        }

        // Add spacer before extend/none options (empty name = blank line)
        items.push({ name: '' });

        // Add extend option if applicable
        const winLimit = cvar.getInt('mp_winlimit') || 0;
        const maxRounds = cvar.getInt('mp_maxrounds') || 0;
        const timeLimit = this.mpTimelimit.float || 0;
        const extendMax = this.amxExtendmapMax.float || 90;

        if (winLimit === 0 && maxRounds === 0 && timeLimit < extendMax) {
            const currentMap = nodemod.mapname || 'current map';
            items.push({
                name: this.getLangWithFallback(null, 'EXTED_MAP', currentMap),
                handler: (client: nodemod.Entity) => {
                    this.countVote(client, SELECT_MAPS);
                }
            });
        }

        // Add "None" option
        items.push({
            name: this.getLangWithFallback(null, 'NONE'),
            handler: (client: nodemod.Entity) => {
                this.countVote(client, SELECT_MAPS + 1);
            }
        });

        // Show menu to all players
        const title = this.getLangWithFallback(null, 'CHOOSE_NEXTM');
        nodemodCore.menu.show({
            title,
            items,
            time: 15,
            formatters: utils.coloredMenuFormatters
        });

        // Announce vote
        this.sendChat(null, this.getLangWithFallback(null, 'TIME_CHOOSE'));

        // Play sound
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            try {
                nodemod.eng.clientCommand(player, "spk \"Gman/Gman_Choose2\"\n");
            } catch (e) {
                // Ignore if sound fails
            }
        }

        this.logAmx('Vote: Voting for the nextmap started');

        // Check votes after 15 seconds
        setTimeout(() => this.checkVotes(), 15000);
    }

    private countVote(entity: nodemod.Entity, key: number) {
        const playerIndex = nodemod.eng.indexOfEdict(entity);

        // Check if already voted
        if (this.votedPlayers.has(playerIndex)) {
            return;
        }
        this.votedPlayers.add(playerIndex);

        // Record vote
        this.voteCounts[key]++;

        // Show vote answer if enabled
        const showAnswers = cvar.getInt('amx_vote_answers') || 0;
        if (showAnswers) {
            const playerName = this.getPlayerName(entity);

            if (key === SELECT_MAPS) {
                this.sendChat(null, this.getLangWithFallback(null, 'CHOSE_EXT', playerName));
            } else if (key < SELECT_MAPS) {
                const mapName = this.mapNames[this.nextMapIndices[key]];
                this.sendChat(null, this.getLangWithFallback(null, 'X_CHOSE_X', playerName, mapName));
            }
        }
    }

    private checkVotes() {
        // Find winning option among maps
        let bestIndex = 0;
        for (let i = 1; i < this.mapVoteNum; i++) {
            if (this.voteCounts[i] > this.voteCounts[bestIndex]) {
                bestIndex = i;
            }
        }

        const extendVotes = this.voteCounts[SELECT_MAPS];
        const noneVotes = this.voteCounts[SELECT_MAPS + 1];
        const bestMapVotes = this.voteCounts[bestIndex];

        // Check if extend won
        if (extendVotes > bestMapVotes && extendVotes > noneVotes) {
            const currentMap = nodemod.mapname || 'current map';
            const stepTime = this.amxExtendmapStep.float || 15;
            const currentTimeLimit = this.mpTimelimit.float || 0;

            this.mpTimelimit.float = currentTimeLimit + stepTime;

            this.sendChat(null, this.getLangWithFallback(null, 'CHO_FIN_EXT', stepTime));
            this.logAmx(`Vote: Voting for the nextmap finished. Map ${currentMap} will be extended to next ${stepTime} minutes`);

            // Allow another vote later
            this.voteTriggered = false;
            return;
        }

        // Set nextmap if a map won (and beat "None")
        if (bestMapVotes > 0 && bestMapVotes >= noneVotes) {
            const winningMap = this.mapNames[this.nextMapIndices[bestIndex]];
            cvar.setString('amx_nextmap', winningMap);
        }

        // Announce result
        const nextMap = cvar.getString('amx_nextmap') || 'unknown';
        this.sendChat(null, this.getLangWithFallback(null, 'CHO_FIN_NEXT', nextMap));
        this.logAmx(`Vote: Voting for the nextmap finished. The nextmap will be ${nextMap}`);
    }

    // Called when plugin is unloaded - save current map as last map
    onUnload() {
        this.saveLastMap();
        if (this.checkTimerId) {
            clearInterval(this.checkTimerId);
            this.checkTimerId = null;
        }
    }
}

export default MapChooser;
