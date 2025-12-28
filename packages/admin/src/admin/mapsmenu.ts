// Maps Menu Plugin
// Converted from AMX Mod X mapsmenu.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_MAP, ADMIN_VOTE } from './constants';

import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

// Maximum maps that can be selected for voting
const MAX_VOTE_MAPS = 4;

// Player state for votemap selection
interface PlayerVoteState {
    selectedMaps: number[];  // Indices into mapNames array
}

// Active vote state
interface VoteState {
    active: boolean;
    callerIndex: number;
    caller: nodemod.Entity | null;
    mapIndices: number[];
    voteCounts: number[];
    votedPlayers: Set<number>;
    timeoutId: ReturnType<typeof setTimeout> | null;
    confirmTimeoutId: ReturnType<typeof setTimeout> | null;
    winningMapIndex: number;
}

class MapsMenu extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Maps Menu',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Changelevel and votemap menus'
    };

    // Map list loaded from config
    private mapNames: string[] = [];

    // Per-player vote selection state
    private playerStates: Map<number, PlayerVoteState> = new Map();

    // Current vote state
    private voteState: VoteState;

    // CVARs
    private amxVotemapRatio: any;
    private amxVoteTime: any;
    private amxVoteDelay: any;
    private amxVoteAnswers: any;
    private amxLastVoting: any;

    constructor(pluginName: string) {
        super(pluginName);

        // Load common dictionary for shared strings
        const localization = require('./localization').default;
        localization.loadDictionary('common');

        // Initialize vote state
        this.voteState = this.getCleanVoteState();

        // Wrap CVARs (registered elsewhere)
        this.amxVotemapRatio = cvar.wrap('amx_votemap_ratio');
        this.amxVoteTime = cvar.wrap('amx_vote_time');
        this.amxVoteDelay = cvar.wrap('amx_vote_delay');
        this.amxVoteAnswers = cvar.wrap('amx_vote_answers');
        this.amxLastVoting = cvar.wrap('amx_last_voting');

        // Load map list
        this.loadSettings();

        // Register commands
        this.registerCommands();
    }

    private getCleanVoteState(): VoteState {
        return {
            active: false,
            callerIndex: 0,
            caller: null,
            mapIndices: [],
            voteCounts: [],
            votedPlayers: new Set(),
            timeoutId: null,
            confirmTimeoutId: null,
            winningMapIndex: -1
        };
    }

    private loadSettings(): boolean {
        this.mapNames = utils.loadMapList();
        return this.mapNames.length > 0;
    }

    private registerCommands() {
        this.registerCommand('amx_mapmenu', ADMIN_MAP, '- displays changelevel menu', (entity, args) => {
            this.cmdMapsMenu(entity);
        });

        this.registerCommand('amx_votemapmenu', ADMIN_VOTE, '- displays votemap menu', (entity, args) => {
            this.cmdVoteMapMenu(entity);
        });
    }

    private getPlayerState(entity: nodemod.Entity): PlayerVoteState {
        const index = nodemod.eng.indexOfEdict(entity);
        let state = this.playerStates.get(index);
        if (!state) {
            state = { selectedMaps: [] };
            this.playerStates.set(index, state);
        }
        return state;
    }

    // ========================================================================
    // Changelevel Menu (amx_mapmenu)
    // ========================================================================

    private cmdMapsMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_MAP)) return;

        if (this.mapNames.length === 0) {
            this.sendConsole(entity, this.getLang(entity, 'NO_MAPS_MENU'));
            this.sendChat(entity, this.getLang(entity, 'NO_MAPS_MENU'));
            return;
        }

        this.displayMapsMenu(entity);
    }

    private displayMapsMenu(entity: nodemod.Entity) {
        const title = this.getLang(entity, 'CHANGLE_MENU');

        // Build all items - core menu handles pagination
        // Validate maps at display time (engine may not have maps registered at plugin init)
        const items: any[] = [];
        for (let i = 0; i < this.mapNames.length; i++) {
            const mapName = this.mapNames[i];
            const mapIndex = i;
            const isValid = utils.isMapValid(mapName);

            items.push({
                name: mapName,
                disabled: !isValid,
                handler: (client: nodemod.Entity) => {
                    this.changeLevel(client, mapIndex);
                }
            });
        }

        nodemodCore.menu.show({
            title,
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private changeLevel(entity: nodemod.Entity, mapIndex: number) {
        const mapName = this.mapNames[mapIndex];
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        const userId = nodemod.eng.getPlayerUserId(entity);

        // Show activity
        this.showActivityKey(entity, 'ADMIN_CHANGEL_1', 'ADMIN_CHANGEL_2', mapName);

        this.logAmx(`Cmd: "${this.getAdminName(entity)}<${userId}><${authId}><>" changelevel "${mapName}"`);

        // Send SVC_INTERMISSION to freeze clients during map change
        // This prevents players from moving during the transition
        utils.sendIntermission();

        // Change map after delay
        setTimeout(() => {
            nodemod.eng.serverCommand(`changelevel ${mapName}\n`);
        }, 2000);
    }

    // ========================================================================
    // Votemap Menu (amx_votemapmenu)
    // ========================================================================

    private cmdVoteMapMenu(entity: nodemod.Entity | null) {
        if (!entity) return;
        if (!adminSystem.cmdAccess(entity, ADMIN_VOTE)) return;

        // Check if voting is in progress
        const lastVoting = this.amxLastVoting?.float || 0;
        const gameTime = this.getGameTime();

        if (lastVoting > gameTime) {
            this.sendChat(entity, this.getLang(entity, 'ALREADY_VOT'));
            return;
        }

        if (this.mapNames.length === 0) {
            this.sendConsole(entity, this.getLang(entity, 'NO_MAPS_MENU'));
            this.sendChat(entity, this.getLang(entity, 'NO_MAPS_MENU'));
            return;
        }

        // Reset player's selection
        const state = this.getPlayerState(entity);
        state.selectedMaps = [];

        this.displayVoteMapsMenu(entity);
    }

    private displayVoteMapsMenu(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);
        const title = this.getLang(entity, 'VOTEMAP_MENU');

        // Build items
        const items: any[] = [];

        // Add map choices
        for (let i = 0; i < this.mapNames.length; i++) {
            const mapName = this.mapNames[i];
            const mapIndex = i;
            const isSelected = state.selectedMaps.includes(mapIndex);
            const canSelect = state.selectedMaps.length < MAX_VOTE_MAPS && !isSelected;

            if (isSelected) {
                // Already selected - show disabled with marker
                items.push({
                    name: `[*] ${mapName}`,
                    disabled: true
                });
            } else if (!canSelect) {
                // Max selections reached
                items.push({
                    name: mapName,
                    disabled: true
                });
            } else {
                items.push({
                    name: mapName,
                    handler: (client: nodemod.Entity) => {
                        this.selectMap(client, mapIndex);
                    }
                });
            }
        }

        // Add "Start Vote" option at the beginning if maps are selected
        if (state.selectedMaps.length > 0) {
            items.unshift({
                name: `>> ${this.getLang(entity, 'START_VOT')} <<`,
                handler: (client: nodemod.Entity) => {
                    this.startVote(client);
                }
            });
        }

        // Build footer showing selected maps
        let footer = '';
        if (state.selectedMaps.length > 0) {
            footer = `\n${this.getLang(entity, 'SEL_MAPS')}:\n`;
            for (const idx of state.selectedMaps) {
                footer += `  ${this.mapNames[idx]}\n`;
            }
        }

        // Show menu
        nodemodCore.menu.show({
            title: title + footer,
            items,
            entity,
            exitText: this.getLangWithFallback(entity, 'EXIT'),
            prevText: this.getLangWithFallback(entity, 'BACK'),
            nextText: this.getLangWithFallback(entity, 'MORE'),
            formatters: utils.coloredMenuFormatters
        });
    }

    private selectMap(entity: nodemod.Entity, mapIndex: number) {
        const state = this.getPlayerState(entity);

        if (state.selectedMaps.length >= MAX_VOTE_MAPS) {
            return;
        }

        if (!state.selectedMaps.includes(mapIndex)) {
            state.selectedMaps.push(mapIndex);
        }

        // Refresh menu
        setTimeout(() => {
            this.displayVoteMapsMenu(entity);
        }, 0);
    }

    private startVote(entity: nodemod.Entity) {
        const state = this.getPlayerState(entity);

        if (state.selectedMaps.length === 0) {
            return;
        }

        // Check vote delay
        const lastVoting = this.amxLastVoting?.float || 0;
        const voteDelay = this.amxVoteDelay?.float || 60;
        const gameTime = this.getGameTime();

        if (lastVoting > gameTime) {
            this.sendChat(entity, this.getLang(entity, 'ALREADY_VOT'));
            return;
        }

        if (lastVoting > 0 && lastVoting + voteDelay > gameTime) {
            this.sendChat(entity, this.getLang(entity, 'VOT_NOW_ALLOW'));
            return;
        }

        // Setup vote state
        this.voteState = this.getCleanVoteState();
        this.voteState.active = true;
        this.voteState.caller = entity;
        this.voteState.callerIndex = nodemod.eng.indexOfEdict(entity);
        this.voteState.mapIndices = [...state.selectedMaps];
        this.voteState.voteCounts = new Array(state.selectedMaps.length + 1).fill(0); // +1 for "None"

        const voteTime = Math.floor((this.amxVoteTime?.float || 10) + 2);

        // Set last voting time
        if (this.amxLastVoting) {
            cvar.setFloat('amx_last_voting', gameTime + voteTime);
        }

        // Show activity
        this.showActivityKey(entity, 'ADMIN_V_MAP_1', 'ADMIN_V_MAP_2');

        // Log
        const adminName = this.getAdminName(entity);
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        const userId = nodemod.eng.getPlayerUserId(entity);
        const mapNamesForLog = state.selectedMaps.map(i => this.mapNames[i]);
        this.logAmx(`Vote: "${adminName}<${userId}><${authId}><>" vote maps (${mapNamesForLog.map((m, i) => `map#${i + 1} "${m}"`).join(') (')})`);

        // Show vote menu to all players
        this.showVoteMenuToAll(entity, voteTime);

        // Set timeout to check votes
        this.voteState.timeoutId = setTimeout(() => {
            this.checkVotes();
        }, voteTime * 1000);
    }

    private showVoteMenuToAll(caller: nodemod.Entity, voteTime: number) {
        const state = this.getPlayerState(caller);
        const callerIndex = nodemod.eng.indexOfEdict(caller);

        // Build vote items
        const items: any[] = [];

        if (state.selectedMaps.length > 1) {
            // Multiple maps - show list
            for (let i = 0; i < state.selectedMaps.length; i++) {
                const mapName = this.mapNames[state.selectedMaps[i]];
                const optionIndex = i;

                items.push({
                    name: mapName,
                    handler: (client: nodemod.Entity) => {
                        this.recordVote(client, optionIndex);
                    }
                });
            }

            // Add "None" option
            items.push({
                name: this.getLangWithFallback(null, 'NONE'),
                handler: (client: nodemod.Entity) => {
                    this.recordVote(client, state.selectedMaps.length);
                }
            });
        } else {
            // Single map - yes/no
            const mapName = this.mapNames[state.selectedMaps[0]];

            items.push({
                name: this.getLangWithFallback(null, 'YES'),
                handler: (client: nodemod.Entity) => {
                    this.recordVote(client, 0);
                }
            });

            items.push({
                name: this.getLangWithFallback(null, 'NO'),
                handler: (client: nodemod.Entity) => {
                    this.recordVote(client, 1);
                }
            });
        }

        // Determine title
        let title: string;
        if (state.selectedMaps.length > 1) {
            title = this.getLang(null, 'WHICH_MAP');
        } else {
            const mapName = this.mapNames[state.selectedMaps[0]];
            title = `${this.getLang(null, 'CHANGE_MAP_TO')}\n${mapName}?`;
        }

        // Show to all players except caller gets cancel option
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            const playerIndex = nodemod.eng.indexOfEdict(player);
            const isCaller = playerIndex === callerIndex;

            if (isCaller) {
                // Caller gets cancel option
                const callerItems = [...items];
                callerItems.push({
                    name: this.getLang(player, 'CANC_VOTE'),
                    handler: (client: nodemod.Entity) => {
                        this.cancelVote(client);
                    }
                });

                nodemodCore.menu.show({
                    title,
                    items: callerItems,
                    entity: player,
                    time: voteTime,
                    formatters: utils.coloredMenuFormatters
                });
            } else {
                nodemodCore.menu.show({
                    title,
                    items,
                    entity: player,
                    time: voteTime,
                    formatters: utils.coloredMenuFormatters
                });
            }
        }
    }

    private recordVote(entity: nodemod.Entity, optionIndex: number) {
        if (!this.voteState.active) return;

        const playerIndex = nodemod.eng.indexOfEdict(entity);

        // Check if already voted
        if (this.voteState.votedPlayers.has(playerIndex)) {
            return;
        }
        this.voteState.votedPlayers.add(playerIndex);

        // Record vote
        if (optionIndex >= 0 && optionIndex < this.voteState.voteCounts.length) {
            this.voteState.voteCounts[optionIndex]++;
        }

        // Show vote answer if enabled
        if (this.amxVoteAnswers?.int) {
            const playerName = this.getPlayerName(entity);
            this.sendChat(null, this.getLang(null, 'X_VOTED_FOR', playerName, optionIndex + 1));
        }
    }

    private cancelVote(entity: nodemod.Entity) {
        if (!this.voteState.active) return;

        // Only caller can cancel
        const playerIndex = nodemod.eng.indexOfEdict(entity);
        if (playerIndex !== this.voteState.callerIndex) return;

        // Clear timeout
        if (this.voteState.timeoutId) {
            clearTimeout(this.voteState.timeoutId);
        }

        this.sendChat(null, this.getLang(null, 'VOT_CANC'));
        this.logAmx('Vote: Cancel vote session');

        // Set last voting time
        cvar.setFloat('amx_last_voting', this.getGameTime());

        this.voteState = this.getCleanVoteState();
    }

    private checkVotes() {
        if (!this.voteState.active) return;

        this.voteState.active = false;

        const numMaps = this.voteState.mapIndices.length;
        const isSingleMap = numMaps === 1;

        // Find winning option
        let best = 0;
        for (let i = 1; i < numMaps; i++) {
            if (this.voteState.voteCounts[i] > this.voteState.voteCounts[best]) {
                best = i;
            }
        }

        // Calculate vote requirements
        const totalVotes = this.voteState.voteCounts.reduce((a, b) => a + b, 0);
        const ratio = this.amxVotemapRatio?.float || 0.40;
        const required = totalVotes > 0 ? Math.ceil(ratio * totalVotes) : 1;
        const bestVotes = this.voteState.voteCounts[best];

        // For single map yes/no vote, index 0 = yes, index 1 = no
        // For multi map, last index is "None"
        const noneIndex = isSingleMap ? 1 : numMaps;
        const noneVotes = this.voteState.voteCounts[noneIndex] || 0;

        // Check if vote passed
        if (isSingleMap) {
            // Yes/No vote - yes votes must meet ratio
            if (bestVotes >= required && best === 0) {
                // Yes won
                this.voteState.winningMapIndex = this.voteState.mapIndices[0];
            } else {
                // Failed
                this.voteState.winningMapIndex = -1;
            }
        } else {
            // Multi-map vote
            if (bestVotes >= required && bestVotes > noneVotes) {
                this.voteState.winningMapIndex = this.voteState.mapIndices[best];
            } else {
                this.voteState.winningMapIndex = -1;
            }
        }

        if (this.voteState.winningMapIndex >= 0) {
            const mapName = this.mapNames[this.voteState.winningMapIndex];
            this.sendChat(null, this.getLang(null, 'VOTE_SUCCESS', mapName));
            this.logAmx(`Vote: ${this.getLang(null, 'VOTE_SUCCESS', mapName)}`);

            // Show confirmation to caller if connected
            const caller = this.voteState.caller;
            if (caller && utils.isConnected(caller)) {
                this.showConfirmMenu(caller, mapName);
            } else {
                // Caller disconnected - execute directly
                this.executeMapChange(mapName);
            }
        } else {
            this.sendChat(null, this.getLang(null, 'VOTE_FAILED'));
            this.logAmx(`Vote: ${this.getLang(null, 'VOTE_FAILED')}`);
            this.voteState = this.getCleanVoteState();
        }
    }

    private showConfirmMenu(caller: nodemod.Entity, mapName: string) {
        const title = `${this.getLang(caller, 'THE_WINNER')}: ${mapName}\n\n${this.getLang(caller, 'WANT_CONT')}`;

        const items = [
            {
                name: this.getLangWithFallback(caller, 'YES'),
                handler: (client: nodemod.Entity) => {
                    if (this.voteState.confirmTimeoutId) {
                        clearTimeout(this.voteState.confirmTimeoutId);
                    }
                    this.sendChat(null, this.getLang(null, 'RESULT_ACC'));
                    this.logAmx(`Vote: ${this.getLang(null, 'RESULT_ACC')}`);
                    this.executeMapChange(mapName);
                    this.voteState = this.getCleanVoteState();
                }
            },
            {
                name: this.getLangWithFallback(caller, 'NO'),
                handler: (client: nodemod.Entity) => {
                    if (this.voteState.confirmTimeoutId) {
                        clearTimeout(this.voteState.confirmTimeoutId);
                    }
                    this.sendChat(null, this.getLang(null, 'RESULT_REF'));
                    this.logAmx(`Vote: ${this.getLang(null, 'RESULT_REF')}`);
                    this.voteState = this.getCleanVoteState();
                }
            }
        ];

        nodemodCore.menu.show({
            title,
            items,
            entity: caller,
            time: 10,
            formatters: utils.coloredMenuFormatters
        });

        // Auto-refuse after timeout
        this.voteState.confirmTimeoutId = setTimeout(() => {
            this.sendChat(null, this.getLang(null, 'RESULT_REF'));
            this.logAmx(`Vote: ${this.getLang(null, 'RESULT_REF')}`);
            this.voteState = this.getCleanVoteState();
        }, 10000);
    }

    private executeMapChange(mapName: string) {
        // Send SVC_INTERMISSION to freeze clients during map change
        utils.sendIntermission();

        setTimeout(() => {
            nodemod.eng.serverCommand(`changelevel ${mapName}\n`);
        }, 2000);
    }

}



export default MapsMenu;
