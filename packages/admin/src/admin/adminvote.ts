// Admin Votes Plugin
// Converted from AMX Mod X adminvote.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_VOTE, ADMIN_ADMIN, CMDTARGET_OBEY_IMMUNITY, CMDTARGET_ALLOW_SELF } from './constants';

import localization from './localization';
import * as utils from './utils';
import { Plugin, PluginMetadata } from './pluginloader';
import { BasePlugin } from './baseplugin';

const cvar = nodemodCore.cvar;

// Vote state
interface VoteState {
    active: boolean;
    yesNoVote: boolean;
    optionNames: string[];
    voteCounts: number[];
    voteRatio: number;
    answer: string;  // Command template with %s placeholder
    execResult: boolean;  // Whether to ask admin for confirmation
    voteCaller: nodemod.Entity | null;
    voteCallerIndex: number;
    timeoutId: ReturnType<typeof setTimeout> | null;
    confirmTimeoutId: ReturnType<typeof setTimeout> | null;
}

class AdminVote extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Votes',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Admin voting commands'
    };

    private state: VoteState;
    private votedPlayers: Set<number> = new Set(); // Track who has voted

    // CVARs
    private amxVoteAnswers: any;
    private amxVoteDelay: any;
    private amxVoteTime: any;
    private amxVotemapRatio: any;
    private amxVoteRatio: any;
    private amxVotekickRatio: any;
    private amxVotebanRatio: any;
    private amxLastVoting: any;  // Persists vote timing across plugin reloads

    constructor(pluginName: string) {
        super(pluginName);

        // Load localization
        localization.loadDictionary(this.pluginName);

        // Initialize state
        this.state = this.getCleanState();

        // Register CVARs
        this.registerCvars();

        // Register commands
        this.registerCommands();
    }

    private getCleanState(): VoteState {
        return {
            active: false,
            yesNoVote: false,
            optionNames: ['', '', '', ''],
            voteCounts: [0, 0, 0, 0],
            voteRatio: 0.02,
            answer: '',
            execResult: false,
            voteCaller: null,
            voteCallerIndex: 0,
            timeoutId: null,
            confirmTimeoutId: null
        };
    }

    private registerCvars() {
        // CVARs are registered in admin.ts, just wrap them here
        this.amxVoteAnswers = cvar.wrap('amx_vote_answers');
        this.amxVoteDelay = cvar.wrap('amx_vote_delay');
        this.amxVoteTime = cvar.wrap('amx_vote_time');
        this.amxVotemapRatio = cvar.wrap('amx_votemap_ratio');
        this.amxVoteRatio = cvar.wrap('amx_vote_ratio');
        this.amxVotekickRatio = cvar.wrap('amx_votekick_ratio');
        this.amxVotebanRatio = cvar.wrap('amx_voteban_ratio');
        this.amxLastVoting = cvar.wrap('amx_last_voting');
    }

    // Helper to get/set last voting time via CVAR for persistence
    private getLastVotingTime(): number {
        return this.amxLastVoting?.float || 0;
    }

    private setLastVotingTime(time: number): void {
        cvar.setFloat('amx_last_voting', time);
    }


    private registerCommands() {
        this.registerCommand('amx_votemap', ADMIN_VOTE, '<map> [map] [map] [map] - starts a vote for map(s)', (entity, args) => {
            this.cmdVoteMap(entity, args);
        });

        this.registerCommand('amx_votekick', ADMIN_VOTE, '<name or #userid> - starts a vote to kick player', (entity, args) => {
            this.cmdVoteKickBan(entity, args, false);
        });

        this.registerCommand('amx_voteban', ADMIN_VOTE, '<name or #userid> - starts a vote to ban player', (entity, args) => {
            this.cmdVoteKickBan(entity, args, true);
        });

        this.registerCommand('amx_vote', ADMIN_VOTE, '<question> <answer#1> <answer#2> - starts a custom vote', (entity, args) => {
            this.cmdVote(entity, args);
        });

        this.registerCommand('amx_cancelvote', ADMIN_VOTE, '- cancels the current vote', (entity, args) => {
            this.cmdCancelVote(entity);
        });
    }





    private canStartVote(entity: nodemod.Entity | null): boolean {
        const now = this.getGameTime();

        // Check if a vote is currently in progress
        if (this.state.active) {
            this.sendConsole(entity, this.getLang(entity, 'ALREADY_VOTING'));
            return false;
        }

        // Check if still in voting period or cooldown
        const lastVoting = this.getLastVotingTime();
        if (lastVoting > now) {
            // Vote still in progress (lastVoting = end time)
            this.sendConsole(entity, this.getLang(entity, 'ALREADY_VOTING'));
            return false;
        }

        // Check vote delay (cooldown after vote ends)
        const delay = this.amxVoteDelay?.float || 60;
        if (lastVoting > 0 && lastVoting + delay > now) {
            this.sendConsole(entity, this.getLang(entity, 'VOTING_NOT_ALLOW'));
            return false;
        }

        return true;
    }

    /**
     * Record a player's vote
     */
    private recordVote(entity: nodemod.Entity, optionIndex: number) {
        if (!this.state.active) return;
        if (optionIndex < 0 || optionIndex > 3) return;

        const playerIndex = nodemod.eng.indexOfEdict(entity);

        // Check if already voted
        if (this.votedPlayers.has(playerIndex)) {
            return;
        }

        // Record vote
        this.votedPlayers.add(playerIndex);
        this.state.voteCounts[optionIndex]++;

        // Show vote answer if enabled
        if (this.amxVoteAnswers?.int || 0) {
            const name = entity.netname || 'Unknown';
            if (this.state.yesNoVote) {
                const msg = optionIndex === 0
                    ? this.getLang(null, 'VOTED_FOR', name)
                    : this.getLang(null, 'VOTED_AGAINST', name);
                this.sendChat(null, msg);
            } else {
                this.sendChat(null, this.getLang(null, 'VOTED_FOR_OPT', name, optionIndex + 1));
            }
        }
    }

    /**
     * Show a vote menu to all players
     * @param includeNone - If true, adds a "None" abstention option at the end
     */
    private showVoteMenu(title: string, options: string[], voteTime: number, includeNone: boolean = false) {
        const items = options.map((opt, i) => ({
            name: opt,
            handler: (client: nodemod.Entity) => this.recordVote(client, i)
        }));

        // Add "None" abstention option if requested (for multi-map votes)
        if (includeNone) {
            const lNone = this.getLang(null, 'NONE');
            items.push({
                name: lNone,
                handler: (client: nodemod.Entity) => {
                    // Record as abstention - doesn't count towards any option
                    const playerIndex = nodemod.eng.indexOfEdict(client);
                    if (!this.votedPlayers.has(playerIndex)) {
                        this.votedPlayers.add(playerIndex);
                        // Show vote answer if enabled
                        if (this.amxVoteAnswers?.int || 0) {
                            const name = client.netname || 'Unknown';
                            this.sendChat(null, this.getLang(null, 'ABSTAINED', name));
                        }
                    }
                }
            });
        }

        // Show menu to all connected players (no entity = broadcast to all)
        nodemodCore.menu.show({
            title,
            items,
            time: voteTime,
            formatters: utils.coloredMenuFormatters
        });
    }

    /**
     * Show a yes/no vote menu to all players
     */
    private showYesNoVoteMenu(question: string, voteTime: number) {
        const lYes = this.getLang(null, 'YES');
        const lNo = this.getLang(null, 'NO');

        const items = [
            { name: lYes, handler: (client: nodemod.Entity) => this.recordVote(client, 0) },
            { name: lNo, handler: (client: nodemod.Entity) => this.recordVote(client, 1) }
        ];

        // Show menu to all connected players (no entity = broadcast to all)
        nodemodCore.menu.show({
            title: question,
            items,
            time: voteTime,
            formatters: utils.coloredMenuFormatters
        });
    }

    /**
     * Show confirmation menu to vote caller
     */
    private showConfirmMenu(caller: nodemod.Entity, result: string) {
        const lResult = this.getLang(caller, 'THE_RESULT');
        const lContinue = this.getLang(caller, 'WANT_CONTINUE');
        const lYes = this.getLang(caller, 'YES');
        const lNo = this.getLang(caller, 'NO');

        nodemodCore.menu.show({
            title: `${lResult}: ${result}\n\n${lContinue}`,
            items: [
                {
                    name: lYes,
                    handler: (client: nodemod.Entity) => {
                        if (this.state.confirmTimeoutId) {
                            clearTimeout(this.state.confirmTimeoutId);
                            this.state.confirmTimeoutId = null;
                        }
                        // Accept - execute the command
                        const cmd = this.state.answer.replace('%s', this.state.optionNames[0]);
                        setTimeout(() => {
                            nodemod.eng.serverCommand(`${cmd}\n`);
                        }, 2000);
                        this.logAmx(`Vote: Result accepted`);
                        this.sendChat(null, this.getLang(null, 'RES_ACCEPTED'));
                        this.state = this.getCleanState();
                        this.votedPlayers.clear();
                    }
                },
                {
                    name: lNo,
                    handler: (client: nodemod.Entity) => {
                        if (this.state.confirmTimeoutId) {
                            clearTimeout(this.state.confirmTimeoutId);
                            this.state.confirmTimeoutId = null;
                        }
                        // Refuse
                        this.logAmx(`Vote: Result refused`);
                        this.sendChat(null, this.getLang(null, 'RES_REF'));
                        this.state = this.getCleanState();
                        this.votedPlayers.clear();
                    }
                }
            ],
            entity: caller,
            time: 10,
            formatters: utils.coloredMenuFormatters
        });
    }

    private checkVotes() {
        if (!this.state.active) return;

        // Find the winning option
        let best = 0;
        if (!this.state.yesNoVote) {
            for (let a = 0; a < 4; a++) {
                if (this.state.voteCounts[a] > this.state.voteCounts[best]) {
                    best = a;
                }
            }
        }

        const votesNum = this.state.voteCounts.reduce((a, b) => a + b, 0);
        const iRatio = votesNum ? Math.ceil(this.state.voteRatio * votesNum) : 1;
        const iResult = this.state.voteCounts[best];

        if (iResult < iRatio) {
            // Vote failed - broadcast to all
            const lFailed = this.getLang(null, 'VOTING_FAILED');
            if (this.state.yesNoVote) {
                this.sendChat(null, this.getLang(null, 'VOTING_RES_1',
                    lFailed, this.state.voteCounts[0], this.state.voteCounts[1], iRatio));
            } else {
                this.sendChat(null, this.getLang(null, 'VOTING_RES_2',
                    lFailed, iResult, iRatio));
            }
            this.logAmx(`Vote: Failed (got ${iResult}) (needed ${iRatio})`);
            this.state = this.getCleanState();
            this.votedPlayers.clear();
            return;
        }

        // Vote succeeded
        const execCmd = this.state.answer.replace('%s', this.state.optionNames[best]);

        if (this.state.execResult && this.state.voteCaller) {
            // Show confirmation menu to the vote caller
            const caller = this.state.voteCaller;

            // Check if caller is still connected
            if (utils.isConnected(caller)) {
                // Store the winning option for confirmation
                this.state.optionNames[0] = this.state.optionNames[best];
                this.state.active = false;

                this.showConfirmMenu(caller, execCmd);

                this.state.confirmTimeoutId = setTimeout(() => {
                    // Auto-refuse if no response
                    this.logAmx(`Vote: Result auto-refused (timeout)`);
                    this.sendChat(null, this.getLang(null, 'RES_REF'));
                    this.state = this.getCleanState();
                    this.votedPlayers.clear();
                }, 10000);
            } else {
                // Caller disconnected, execute directly
                setTimeout(() => {
                    nodemod.eng.serverCommand(`${execCmd}\n`);
                }, 2000);
                this.state = this.getCleanState();
                this.votedPlayers.clear();
            }
        } else {
            // Execute without confirmation (custom votes)
            // Custom votes don't execute commands, just show results
            this.state = this.getCleanState();
            this.votedPlayers.clear();
        }

        // Announce success to all players
        const lSuccess = this.getLang(null, 'VOTING_SUCCESS');
        const message = this.getLang(null, 'VOTING_RES_3', lSuccess, iResult, iRatio, execCmd);
        this.sendChat(null, message);
        this.logAmx(`Vote: Success (got ${iResult}) (needed ${iRatio}) (result "${execCmd}")`);
    }

    // amx_cancelvote - cancel current vote
    private cmdCancelVote(entity: nodemod.Entity | null) {
        if (!adminSystem.cmdAccess(entity, ADMIN_VOTE)) return;

        if (!this.state.active && !this.state.confirmTimeoutId) {
            this.sendConsole(entity, this.getLang(entity, 'NO_VOTE_CANC'));
            return;
        }

        const name = entity ? (entity.netname || 'Unknown') : 'CONSOLE';
        const authId = entity ? (nodemod.eng.getPlayerAuthId(entity) || '') : '';
        const userId = entity ? nodemod.eng.getPlayerUserId(entity) : 0;

        this.logAmx(`Vote: "${name}<${userId}><${authId}><>" cancel vote session`);

        // Show activity
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            utils.showActivity(player, name, this.getLang(player, 'ADMIN_CANC_VOTE'), this.getShowActivityOptions());
        }

        this.sendConsole(entity, this.getLang(entity, 'VOTING_CANC'));
        this.sendChat(null, this.getLang(null, 'VOTING_CANC'));

        // Clear timeouts
        if (this.state.timeoutId) {
            clearTimeout(this.state.timeoutId);
        }
        if (this.state.confirmTimeoutId) {
            clearTimeout(this.state.confirmTimeoutId);
        }

        this.setLastVotingTime(this.getGameTime());
        this.state = this.getCleanState();
        this.votedPlayers.clear();
    }

    // amx_votemap <map> [map] [map] [map]
    private cmdVoteMap(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_VOTE)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_VOTEMAP'));
            return;
        }

        if (!this.canStartVote(entity)) return;

        // Validate maps (up to 4)
        const validMaps: string[] = [];
        for (let i = 0; i < Math.min(args.length, 4); i++) {
            const mapName = args[i];
            if (mapName && mapName.length > 0 && nodemod.eng.isMapValid(mapName)) {
                validMaps.push(mapName);
            }
        }

        if (validMaps.length === 0) {
            const lMaps = args.length === 1
                ? this.getLang(entity, 'MAP_IS')
                : this.getLang(entity, 'MAPS_ARE');
            this.sendConsole(entity, this.getLang(entity, 'GIVEN_NOT_VALID', lMaps));
            return;
        }

        // Setup vote state
        this.state = this.getCleanState();
        this.state.active = true;
        this.state.execResult = true;
        this.state.voteCaller = entity;
        this.state.voteCallerIndex = entity ? nodemod.eng.indexOfEdict(entity) : 0;
        this.state.voteRatio = this.amxVotemapRatio?.float || 0.40;
        this.state.answer = 'changelevel %s';

        for (let i = 0; i < validMaps.length; i++) {
            this.state.optionNames[i] = validMaps[i];
        }

        const name = entity ? (entity.netname || 'Unknown') : 'CONSOLE';
        const authId = entity ? (nodemod.eng.getPlayerAuthId(entity) || '') : '';
        const userId = entity ? nodemod.eng.getPlayerUserId(entity) : 0;

        if (validMaps.length === 1) {
            this.logAmx(`Vote: "${name}<${userId}><${authId}><>" vote map (map "${validMaps[0]}")`);
        } else {
            this.logAmx(`Vote: "${name}<${userId}><${authId}><>" vote maps (map#1 "${validMaps[0]}") (map#2 "${validMaps[1] || ''}") (map#3 "${validMaps[2] || ''}") (map#4 "${validMaps[3] || ''}")`);
        }

        // Show activity
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            utils.showActivity(player, name, this.getLang(player, 'ADMIN_VOTE_MAP'), this.getShowActivityOptions());
        }

        const voteTime = Math.floor((this.amxVoteTime?.float || 10) + 2);
        this.setLastVotingTime(this.getGameTime() + voteTime);
        this.votedPlayers.clear();

        // Show vote menu
        if (validMaps.length > 1) {
            // Multiple maps - show list with "None" abstention option
            this.state.yesNoVote = false;
            const lChoose = this.getLang(null, 'CHOOSE_MAP');
            this.showVoteMenu(lChoose, validMaps, voteTime, true);
        } else {
            // Single map - yes/no
            this.state.yesNoVote = true;
            const lChange = this.getLang(null, 'CHANGE_MAP_TO');
            this.showYesNoVoteMenu(`${lChange} ${validMaps[0]}?`, voteTime);
        }

        this.state.timeoutId = setTimeout(() => this.checkVotes(), voteTime * 1000);
        this.sendConsole(entity, this.getLang(entity, 'VOTING_STARTED'));
    }

    // amx_vote <question> <answer#1> <answer#2> [answer#3] [answer#4]
    private cmdVote(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_VOTE)) return;

        if (args.length < 3) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_VOTE'));
            return;
        }

        if (!this.canStartVote(entity)) return;

        const question = args[0];

        // Check for forbidden content
        if (question.toLowerCase().includes('sv_password') ||
            question.toLowerCase().includes('rcon_password')) {
            this.sendConsole(entity, this.getLang(entity, 'VOTING_FORBIDDEN'));
            return;
        }

        // Get options (up to 4)
        const options: string[] = [];
        for (let i = 1; i < Math.min(args.length, 5); i++) {
            options.push(args[i]);
        }

        // Setup vote state
        this.state = this.getCleanState();
        this.state.active = true;
        this.state.execResult = false; // Custom votes don't execute commands
        this.state.yesNoVote = false;
        this.state.voteCaller = entity;
        this.state.voteCallerIndex = entity ? nodemod.eng.indexOfEdict(entity) : 0;
        this.state.voteRatio = this.amxVoteRatio?.float || 0.02;
        this.state.answer = `${question.replace(/%/g, '')} - %s`;

        for (let i = 0; i < options.length; i++) {
            this.state.optionNames[i] = options[i];
        }

        const name = entity ? (entity.netname || 'Unknown') : 'CONSOLE';
        const authId = entity ? (nodemod.eng.getPlayerAuthId(entity) || '') : '';
        const userId = entity ? nodemod.eng.getPlayerUserId(entity) : 0;

        this.logAmx(`Vote: "${name}<${userId}><${authId}><>" vote custom (question "${question}") (option#1 "${options[0]}") (option#2 "${options[1] || ''}")`);

        // Show activity
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            utils.showActivity(player, name, this.getLang(player, 'ADMIN_VOTE_CUS'), this.getShowActivityOptions());
        }

        const voteTime = Math.floor((this.amxVoteTime?.float || 10) + 2);
        this.setLastVotingTime(this.getGameTime() + voteTime);
        this.votedPlayers.clear();

        // Show vote menu
        const lVote = this.getLang(null, 'VOTE');
        this.showVoteMenu(`${lVote}: ${question}`, options, voteTime);

        this.state.timeoutId = setTimeout(() => this.checkVotes(), voteTime * 1000);
        this.sendConsole(entity, this.getLang(entity, 'VOTING_STARTED'));
    }

    // amx_votekick / amx_voteban
    private cmdVoteKickBan(entity: nodemod.Entity | null, args: string[], isBan: boolean) {
        if (!adminSystem.cmdAccess(entity, ADMIN_VOTE)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, isBan ? 'USAGE_VOTEBAN' : 'USAGE_VOTEKICK'));
            return;
        }

        if (!this.canStartVote(entity)) return;

        // Find target player
        const target = adminSystem.cmdTarget(entity, args[0], CMDTARGET_OBEY_IMMUNITY | CMDTARGET_ALLOW_SELF);
        if (!target) return;

        // Can't ban bots
        if (isBan && utils.isBot(target)) {
            const targetName = target.netname || 'Unknown';
            this.sendConsole(entity, this.getLang(entity, 'ACTION_PERFORMED', targetName));
            return;
        }

        const targetName = target.netname || 'Unknown';
        const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
        const targetUserId = nodemod.eng.getPlayerUserId(target);

        // Setup vote state
        this.state = this.getCleanState();
        this.state.active = true;
        this.state.yesNoVote = true;
        this.state.execResult = true;
        this.state.voteCaller = entity;
        this.state.voteCallerIndex = entity ? nodemod.eng.indexOfEdict(entity) : 0;
        this.state.voteRatio = isBan
            ? (this.amxVotebanRatio?.float || 0.60)
            : (this.amxVotekickRatio?.float || 0.40);

        if (isBan) {
            // Determine if IP ban is needed
            if (targetAuthId === '4294967295' ||
                targetAuthId === 'HLTV' ||
                targetAuthId === 'STEAM_ID_LAN' ||
                targetAuthId.toUpperCase() === 'VALVE_ID_LAN') {
                // IP ban
                this.state.optionNames[0] = utils.getPlayerIP(target);
                this.state.answer = 'addip 30.0 %s';
            } else {
                // Steam ID ban
                this.state.optionNames[0] = targetAuthId;
                this.state.answer = 'banid 30.0 %s kick';
            }
        } else {
            // Kick by user ID
            this.state.optionNames[0] = String(targetUserId);
            this.state.answer = 'kick #%s';
        }

        const name = entity ? (entity.netname || 'Unknown') : 'CONSOLE';
        const authId = entity ? (nodemod.eng.getPlayerAuthId(entity) || '') : '';
        const userId = entity ? nodemod.eng.getPlayerUserId(entity) : 0;

        this.logAmx(`Vote: "${name}<${userId}><${authId}><>" vote ${isBan ? 'ban' : 'kick'} (target "${targetName}")`);

        // Show activity
        const lKickBan = this.getLang(null, isBan ? 'BAN' : 'KICK');
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            utils.showActivity(player, name,
                this.getLang(player, 'ADMIN_VOTE_FOR', lKickBan, targetName), this.getShowActivityOptions());
        }

        const voteTime = Math.floor((this.amxVoteTime?.float || 10) + 2);
        this.setLastVotingTime(this.getGameTime() + voteTime);
        this.votedPlayers.clear();

        // Show yes/no vote menu
        const lAction = isBan ? this.getLang(null, 'BAN') : this.getLang(null, 'KICK');
        this.showYesNoVoteMenu(`${lAction} ${targetName}?`, voteTime);

        this.state.timeoutId = setTimeout(() => this.checkVotes(), voteTime * 1000);
        this.sendConsole(entity, this.getLang(entity, 'VOTING_STARTED'));
    }
}



export default AdminVote;
