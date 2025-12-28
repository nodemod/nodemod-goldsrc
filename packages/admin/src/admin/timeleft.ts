// TimeLeft Plugin
// Converted from AMX Mod X timeleft.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { BasePlugin } from './baseplugin';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import * as utils from './utils';

const cvar = nodemodCore.cvar;

// Display flags
const DISPLAY_FLAG_HUD = 1;      // 'a' - Show HUD message
const DISPLAY_FLAG_VOICE = 2;   // 'b' - Play voice announcement
const DISPLAY_FLAG_NO_REMAINING = 4;  // 'c' - Don't say "remaining"
const DISPLAY_FLAG_NO_UNITS = 8;      // 'd' - Don't say hours/minutes/seconds
const DISPLAY_FLAG_COUNTDOWN = 16;    // 'e' - Countdown mode

// Time display settings
interface TimeDisplaySetting {
    time: number;       // Time in seconds
    flags: number;      // Display flags
}

class TimeLeft extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'TimeLeft',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Shows time remaining on the map'
    };

    // Display settings
    private timeSettings: TimeDisplaySetting[] = [];
    private lastTime: number = 0;
    private countDown: number = 0;
    private inCountdownMode: boolean = false;

    // CVARs
    private amxTimeVoice: any;
    private mpTimelimit: any;
    private amxTimeleft: any;

    // Timer
    private timer: NodeJS.Timeout | null = null;

    constructor(pluginName: string) {
        super(pluginName);

        // Register CVARs
        this.registerCvar('amx_time_voice', '1', nodemod.FCVAR.SERVER, 'Enable voice announcements for time');
        this.registerCvar('amx_timeleft', '00:00', nodemod.FCVAR.SERVER | nodemod.FCVAR.UNLOGGED, 'Current timeleft value');

        // Get CVAR wrappers
        this.amxTimeVoice = cvar.wrap('amx_time_voice');
        this.mpTimelimit = cvar.wrap('mp_timelimit');
        this.amxTimeleft = cvar.wrap('amx_timeleft');

        // Register commands
        this.registerCommands();

        // Start the timer
        this.startTimer();
    }

    private registerCommands() {
        // Client say commands
        nodemod.on('dllClientCommand', (entity: nodemod.Entity, text: string) => {
            const args = utils.parseCommand(text);
            if (args.length === 0) return;

            const cmd = args[0].toLowerCase();

            if (cmd === 'say') {
                const message = args.slice(1).join(' ').toLowerCase().replace(/"/g, '');
                if (message === 'timeleft') {
                    this.sayTimeLeft(entity);
                } else if (message === 'thetime') {
                    this.sayTheTime(entity);
                }
            }
        });

        // Server command for display settings
        nodemodCore.cmd.registerServer('amx_time_display', (args: string[]) => {
            this.setDisplaying(args);
        });
    }

    /**
     * Get remaining time in seconds
     * Uses the shared utils.getTimeLeft() for consistent calculation
     */
    private getTimeLeft(): number {
        return utils.getTimeLeft();
    }

    /**
     * Handle "say timeleft" command
     */
    private sayTimeLeft(entity: nodemod.Entity) {
        const timelimit = this.mpTimelimit?.float || 0;

        if (timelimit > 0) {
            const timeLeft = this.getTimeLeft();

            // Play voice announcement if enabled
            if (this.amxTimeVoice?.int) {
                const voiceCmd = this.setTimeVoice(0, timeLeft);
                if (voiceCmd) {
                    nodemod.eng.clientCommand(entity, voiceCmd);
                }
            }

            // Show time left to all players
            const mins = Math.floor(timeLeft / 60);
            const secs = timeLeft % 60;
            const message = this.getLang(null, 'TIME_LEFT') + `:  ${mins}:${secs.toString().padStart(2, '0')}`;

            for (const player of adminSystem.getPlayers()) {
                nodemodCore.util.sendChat(message, player);
            }
        } else {
            const message = this.getLang(null, 'NO_T_LIMIT');
            for (const player of adminSystem.getPlayers()) {
                nodemodCore.util.sendChat(message, player);
            }
        }
    }

    /**
     * Handle "say thetime" command
     */
    private sayTheTime(entity: nodemod.Entity) {
        // Play voice announcement if enabled
        if (this.amxTimeVoice?.int) {
            const now = new Date();
            const hours = now.getHours();
            const mins = now.getMinutes();

            const hoursWord = this.numToWord(hours === 0 ? 12 : (hours > 12 ? hours - 12 : hours));
            const minsWord = mins > 0 ? this.numToWord(mins) : '';
            const period = hours < 12 ? 'am' : 'pm';

            // Format: time_is_now {hours} _period {mins} {am/pm}
            // _period is a voice file, needs spaces between words
            const minsStr = minsWord ? `${minsWord} ` : '';
            const voiceCmd = `spk "fvox/time_is_now ${hoursWord} _period ${minsStr}${period}"\n`;
            nodemod.eng.clientCommand(entity, voiceCmd);
        }

        // Format current time
        const now = new Date();
        const dateStr = now.toLocaleDateString('en-US', {
            month: '2-digit',
            day: '2-digit',
            year: 'numeric'
        });
        const timeStr = now.toLocaleTimeString('en-US', {
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit',
            hour12: false
        });

        const message = this.getLang(null, 'THE_TIME') + `:   ${dateStr} - ${timeStr}`;

        for (const player of adminSystem.getPlayers()) {
            nodemodCore.util.sendChat(message, player);
        }
    }

    /**
     * Convert number to spoken word format
     */
    private numToWord(num: number): string {
        const words = [
            'zero', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine',
            'ten', 'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen',
            'seventeen', 'eighteen', 'nineteen'
        ];
        const tens = ['', '', 'twenty', 'thirty', 'fourty', 'fifty'];

        if (num < 20) {
            return words[num] || '';
        }
        if (num < 60) {
            const t = Math.floor(num / 10);
            const o = num % 10;
            return o > 0 ? `${tens[t]} ${words[o]}` : tens[t];
        }
        return num.toString();
    }

    /**
     * Generate voice command for time announcement
     * Uses Half-Life VOX system - words are built-in and don't need directory prefix
     */
    private setTimeVoice(flags: number, timeLeft: number): string {
        const parts: string[] = [];
        let secs = timeLeft % 60;
        let mins = Math.floor(timeLeft / 60);

        if (secs > 0) {
            parts.push(this.numToWord(secs));
            if (!(flags & DISPLAY_FLAG_NO_UNITS)) {
                parts.push('seconds');
            }
        }

        if (mins > 59) {
            const hours = Math.floor(mins / 60);
            parts.unshift(this.numToWord(hours));
            if (!(flags & DISPLAY_FLAG_NO_UNITS)) {
                parts.splice(1, 0, 'hours');
            }
            mins = mins % 60;
        }

        if (mins > 0) {
            const insertIdx = parts.length > 0 && parts[0] !== 'hours' ? 0 : (parts.length > 1 ? 2 : 0);
            const minParts = [this.numToWord(mins)];
            if (!(flags & DISPLAY_FLAG_NO_UNITS)) {
                minParts.push('minutes');
            }
            parts.splice(insertIdx, 0, ...minParts);
        }

        if (!(flags & DISPLAY_FLAG_NO_REMAINING)) {
            parts.push('remaining');
        }

        if (parts.length === 0) return '';
        // VOX words work without prefix when following a sentence or standalone
        // e.g., "spk fvox/time_is_now one pm" or "spk five minutes remaining"
        return `spk "${parts.join(' ')}"\n`;
    }

    /**
     * Format time text for display
     */
    private setTimeText(entity: nodemod.Entity, timeLeft: number): string {
        const secs = timeLeft % 60;
        const mins = Math.floor(timeLeft / 60);

        if (secs === 0) {
            const minWord = mins > 1
                ? this.getLang(entity, 'MINUTES')
                : this.getLang(entity, 'MINUTE');
            return `${mins} ${minWord}`;
        } else if (mins === 0) {
            const secWord = secs > 1
                ? this.getLang(entity, 'SECONDS')
                : this.getLang(entity, 'SECOND');
            return `${secs} ${secWord}`;
        } else {
            const minWord = mins > 1
                ? this.getLang(entity, 'MINUTES')
                : this.getLang(entity, 'MINUTE');
            const secWord = secs > 1
                ? this.getLang(entity, 'SECONDS')
                : this.getLang(entity, 'SECOND');
            return `${mins} ${minWord} ${secs} ${secWord}`;
        }
    }

    /**
     * Find display format for current time
     */
    private findDisplayFormat(time: number): number {
        for (let i = 0; i < this.timeSettings.length; i++) {
            const setting = this.timeSettings[i];

            if (setting.flags & DISPLAY_FLAG_COUNTDOWN) {
                // Countdown mode: trigger when time drops below threshold
                if (setting.time > time) {
                    if (!this.inCountdownMode) {
                        this.countDown = this.inCountdownMode ? this.countDown : time;
                        this.inCountdownMode = true;

                        // Switch to faster timer for countdown
                        this.stopTimer();
                        this.startTimer(1000);
                    }
                    return i;
                }
            } else if (setting.time === time) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Server command to configure display settings
     */
    private setDisplaying(args: string[]) {
        this.timeSettings = [];

        for (const arg of args) {
            const parts = utils.parseCommand(arg);
            if (parts.length >= 2) {
                const flags = this.readFlags(parts[0]);
                const time = parseInt(parts[1]) || 0;

                if (time > 0) {
                    this.timeSettings.push({ time, flags });
                }
            }
        }
    }

    /**
     * Parse display flags string
     */
    private readFlags(flagStr: string): number {
        let flags = 0;
        for (const char of flagStr.toLowerCase()) {
            switch (char) {
                case 'a': flags |= DISPLAY_FLAG_HUD; break;
                case 'b': flags |= DISPLAY_FLAG_VOICE; break;
                case 'c': flags |= DISPLAY_FLAG_NO_REMAINING; break;
                case 'd': flags |= DISPLAY_FLAG_NO_UNITS; break;
                case 'e': flags |= DISPLAY_FLAG_COUNTDOWN; break;
            }
        }
        return flags;
    }

    /**
     * Main timer callback - updates timeleft and triggers announcements
     */
    private timeRemain() {
        const gameTimeLeft = this.getTimeLeft();
        const timeLeft = this.inCountdownMode ? --this.countDown : gameTimeLeft;

        // Update amx_timeleft CVAR
        const mins = Math.floor(gameTimeLeft / 60);
        const secs = gameTimeLeft % 60;
        cvar.setString('amx_timeleft', `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`);

        // Check if we should exit countdown mode (time extended during countdown)
        if (this.inCountdownMode && gameTimeLeft > this.countDown) {
            this.inCountdownMode = false;
            this.stopTimer();
            this.startTimer();
            return;
        }

        if (timeLeft > 0 && this.lastTime !== timeLeft) {
            this.lastTime = timeLeft;
            const settingIdx = this.findDisplayFormat(timeLeft);

            if (settingIdx !== -1) {
                const flags = this.timeSettings[settingIdx].flags;
                const players = adminSystem.getPlayers({ excludeBots: true });

                // HUD message
                if (flags & DISPLAY_FLAG_HUD) {
                    for (const player of players) {
                        const text = this.setTimeText(player, timeLeft);

                        if (nodemodCore.util.showHudText) {
                            const holdTime = (flags & DISPLAY_FLAG_COUNTDOWN) ? 1.1 : 3.0;
                            nodemodCore.util.showHudText(player, text, {
                                x: -8192,  // Center (-1.0 * 8192)
                                y: Math.floor(0.85 * 8192),
                                r1: 255, g1: 255, b1: 255,
                                holdTime: Math.floor(holdTime * 256),
                                channel: 4
                            });
                        } else {
                            nodemod.eng.clientPrintf(player, nodemod.PRINT_TYPE.print_center, text);
                        }
                    }
                }

                // Voice announcement
                if (flags & DISPLAY_FLAG_VOICE) {
                    const voiceCmd = this.setTimeVoice(flags, timeLeft);
                    if (voiceCmd) {
                        for (const player of players) {
                            nodemod.eng.clientCommand(player, voiceCmd);
                        }
                    }
                }
            }
        }
    }

    /**
     * Start the timer
     */
    private startTimer(intervalMs: number = 800) {
        if (this.timer) {
            clearInterval(this.timer);
        }
        this.timer = setInterval(() => this.timeRemain(), intervalMs);
    }

    /**
     * Stop the timer
     */
    private stopTimer() {
        if (this.timer) {
            clearInterval(this.timer);
            this.timer = null;
        }
    }

    /**
     * Plugin unload
     */
    onUnload() {
        this.stopTimer();
    }
}

export default TimeLeft;
