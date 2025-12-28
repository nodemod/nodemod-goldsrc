// Admin Chat Plugin
// Converted from AMX Mod X adminchat.sma to TypeScript for NodeMod
// Original by the AMX Mod X Development Team, originally developed by OLO

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_CHAT, CMDTARGET_ALLOW_SELF } from './constants';

import { BasePlugin } from './baseplugin';
import * as utils from './utils';
import { Plugin, PluginMetadata, pluginLoader } from './pluginloader';
import localization from './localization';

// Colors for HUD messages
const COLORS: { [key: string]: [number, number, number] } = {
    'white': [255, 255, 255],
    'red': [255, 0, 0],
    'green': [0, 255, 0],
    'blue': [0, 0, 255],
    'yellow': [255, 255, 0],
    'magenta': [255, 0, 255],
    'cyan': [0, 255, 255],
    'orange': [227, 96, 8],
    'ocean': [45, 89, 116],
    'maroon': [103, 44, 38]
};

// Color shortcuts for @ chat
const COLOR_SHORTCUTS: { [key: string]: string } = {
    'w': 'white',
    'r': 'red',
    'g': 'green',
    'b': 'blue',
    'y': 'yellow',
    'm': 'magenta',
    'c': 'cyan',
    'o': 'orange'
};

// Color keys for localization lookup
const COLOR_KEYS: { [key: string]: string } = {
    'COLOR_WHITE': 'white',
    'COLOR_RED': 'red',
    'COLOR_GREEN': 'green',
    'COLOR_BLUE': 'blue',
    'COLOR_YELLOW': 'yellow',
    'COLOR_MAGENTA': 'magenta',
    'COLOR_CYAN': 'cyan',
    'COLOR_ORANGE': 'orange'
};

// HUD positions based on @ count
const HUD_POSITIONS: [number, number][] = [
    [0, 0],       // unused
    [0.05, 0.55], // @ - left side
    [-1, 0.2],    // @@ - center top
    [-1, 0.7]     // @@@ - center bottom
];

// Message channel for HUD (cycles 3-6)
let msgChannel = 3;

class AdminChat extends BasePlugin implements Plugin {
    // Plugin metadata
    readonly metadata: PluginMetadata = {
        name: 'Admin Chat',
        version: '1.9.0',
        author: 'AMX Mod X Team',
        description: 'Admin chat commands and HUD messaging'
    };

    constructor(pluginName: string) {
        super(pluginName);

        // Register commands
        this.registerCommands();

        // Hook into say commands
        this.hookSayCommands();
    }

    private registerCommands() {
        this.registerCommand('amx_say', ADMIN_CHAT, '<message> - sends message to all players', (entity, args) => {
            this.cmdSay(entity, args);
        });

        this.registerCommand('amx_chat', ADMIN_CHAT, '<message> - sends message to admins', (entity, args) => {
            this.cmdChat(entity, args);
        });

        this.registerCommand('amx_psay', ADMIN_CHAT, '<name or #userid> <message> - sends private message', (entity, args) => {
            this.cmdPsay(entity, args);
        });

        this.registerCommand('amx_tsay', ADMIN_CHAT, '<color> <message> - sends HUD message (left)', (entity, args) => {
            this.cmdTsay(entity, args, true);
        });

        this.registerCommand('amx_csay', ADMIN_CHAT, '<color> <message> - sends HUD message (center)', (entity, args) => {
            this.cmdTsay(entity, args, false);
        });
    }

    private hookSayCommands() {
        // Hook into client commands to catch say and say_team
        nodemod.on('dllClientCommand', (entity: nodemod.Entity, text: string) => {
            const args = utils.parseCommand(text);
            if (args.length === 0) return;

            const cmd = args[0].toLowerCase();

            if (cmd === 'say') {
                const message = args.slice(1).join(' ').replace(/^"|"$/g, '');
                if (this.handleSayChat(entity, message)) {
                    nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
                }
            } else if (cmd === 'say_team') {
                const message = args.slice(1).join(' ').replace(/^"|"$/g, '');
                if (this.handleSayAdmin(entity, message)) {
                    nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
                }
            }
        });
    }

    private getNextChannel(): number {
        msgChannel++;
        if (msgChannel > 6 || msgChannel < 3) {
            msgChannel = 3;
        }
        return msgChannel;
    }

    /**
     * Look up a color name from any available language translation.
     * This allows admins to use color names in their own language.
     * Returns the internal color name (e.g., 'red') or null if not found.
     *
     * Matches AMX behavior: loops through all colors and all languages
     * to find a match for the input color name.
     */
    private lookupColorByLocalizedName(input: string): string | null {
        const inputLower = input.toLowerCase();

        // First check if it's already an English color name
        if (COLORS[inputLower]) {
            return inputLower;
        }

        // Get available languages for adminchat plugin
        const langs = localization.getAvailableLanguages(this.pluginName);

        // Check each color key against all language translations
        // This matches the original AMX behavior that loops through all languages
        for (const [colorKey, colorName] of Object.entries(COLOR_KEYS)) {
            for (const lang of langs) {
                // Get the translation for this color in THIS specific language
                const translated = localization.getLangForCode(lang, this.pluginName, colorKey);
                if (translated && translated.toLowerCase() === inputLower) {
                    return colorName;
                }
            }
        }

        return null;
    }

    private sendHudMessage(
        target: nodemod.Entity | null,
        message: string,
        color: [number, number, number],
        x: number,
        y: number
    ) {
        const channel = this.getNextChannel();
        const yPos = y + channel / 35.0;

        // AMXX TE_TEXTMESSAGE format:
        // - x, y: float * 8192 (signed 16-bit), -1.0 for center = -8192
        // - times: seconds * 256 (1/256th second units)
        const options = {
            channel,
            x: x === -1 ? -8192 : Math.floor(x * 8192),
            y: Math.floor(yPos * 8192),
            r1: color[0],
            g1: color[1],
            b1: color[2],
            a1: 255,
            fadeinTime: Math.floor(0.5 * 256),   // 0.5s = 128
            fadeoutTime: Math.floor(0.15 * 256), // 0.15s = 38
            holdTime: Math.floor(6.0 * 256)      // 6.0s = 1536
        };

        if (target) {
            nodemodCore.util.showHudText(target, message, options);
        } else {
            for (const player of adminSystem.getPlayers({ excludeBots: true })) {
                nodemodCore.util.showHudText(player, message, options);
            }
        }
    }

    // Handle say @message for HUD messages
    private handleSayChat(entity: nodemod.Entity, message: string): boolean {
        if (!adminSystem.hasAccess(entity, ADMIN_CHAT)) {
            return false;
        }

        // Count @ symbols at start
        let atCount = 0;
        while (message[atCount] === '@') {
            atCount++;
        }

        if (atCount === 0 || atCount > 3) {
            return false;
        }

        // Check for color code
        let colorName = 'white';
        let msgStart = atCount;

        const colorChar = message[atCount]?.toLowerCase();
        if (colorChar && COLOR_SHORTCUTS[colorChar]) {
            colorName = COLOR_SHORTCUTS[colorChar];
            msgStart++;
        }

        // Skip whitespace
        while (message[msgStart] === ' ') {
            msgStart++;
        }

        const text = message.substring(msgStart);
        if (!text) {
            return false;
        }

        const name = this.getPlayerName(entity);
        const color = COLORS[colorName] || COLORS['white'];
        const pos = HUD_POSITIONS[atCount] || HUD_POSITIONS[1];

        // Dual logging: AMXX internal format + HL engine format
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        const userId = nodemod.eng.getPlayerUserId(entity);
        this.logAmx(`Chat: "${name}<${userId}><${authId}><>" tsay "${text}"`);
        this.logMessage(entity, 'amx_tsay', { text, color: colorName });

        // Send HUD message based on amx_show_activity
        const options = this.getShowActivityOptions();
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            const displayMsg = utils.getActivityMessage(player, name, text, options);
            if (displayMsg) {
                this.sendHudMessage(player, displayMsg, color, pos[0], pos[1]);
                nodemod.eng.clientPrintf(player, nodemod.PRINT_TYPE.print_console, `${displayMsg}\n`);
            }
        }

        return true;
    }

    // Handle say_team @message for admin chat
    private handleSayAdmin(entity: nodemod.Entity, message: string): boolean {
        if (message[0] !== '@') {
            return false;
        }

        const text = message.substring(1).trim();
        if (!text) {
            return false;
        }

        const name = this.getPlayerName(entity);
        const authId = nodemod.eng.getPlayerAuthId(entity) || '';
        const userId = nodemod.eng.getPlayerUserId(entity);
        const isAdmin = adminSystem.hasAccess(entity, ADMIN_CHAT);

        // Dual logging: AMXX internal format + HL engine format
        this.logAmx(`Chat: "${name}<${userId}><${authId}><>" chat "${text}"`);
        this.logMessage(entity, 'amx_chat', { text });

        // Format message with localized format
        const formatted = isAdmin
            ? this.getLang(null, 'ADMIN_CHAT_PREFIX', name, text)
            : this.getLang(null, 'PLAYER_CHAT_PREFIX', name, text);

        // Send to all admins
        for (const player of adminSystem.getPlayers()) {
            // Send to admins (and the sender)
            if (player === entity || adminSystem.hasAccess(player, ADMIN_CHAT)) {
                this.sendChat(player, formatted);
            }
        }

        return true;
    }

    // amx_say <message> - sends message to all players
    private cmdSay(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CHAT)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_SAY'));
            return;
        }

        const message = args.join(' ');
        const name = this.getAdminName(entity);

        // Dual logging: AMXX internal format + HL engine format
        if (entity) {
            const authId = nodemod.eng.getPlayerAuthId(entity) || '';
            const userId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Chat: "${name}<${userId}><${authId}><>" say "${message}"`);
        }
        this.logMessage(entity, 'amx_say', { text: message });

        // Send to all players with localized format
        const formatted = this.getLang(null, 'ALL_CHAT_PREFIX', name, message);
        this.sendChat(null, formatted);
        this.sendConsole(entity, formatted);
    }

    // amx_chat <message> - sends message to admins only
    private cmdChat(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CHAT)) return;

        if (args.length < 1) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_CHAT'));
            return;
        }

        const message = args.join(' ');
        const name = this.getAdminName(entity);

        // Dual logging: AMXX internal format + HL engine format
        if (entity) {
            const authId = nodemod.eng.getPlayerAuthId(entity) || '';
            const userId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Chat: "${name}<${userId}><${authId}><>" chat "${message}"`);
        }
        this.logMessage(entity, 'amx_chat', { text: message });

        // Send to admins only with localized format
        const formatted = this.getLang(null, 'ADMINS_CHAT_PREFIX', name, message);
        this.sendConsole(entity, formatted);

        for (const player of adminSystem.getPlayers()) {
            if (adminSystem.hasAccess(player, ADMIN_CHAT)) {
                this.sendChat(player, formatted);
            }
        }
    }

    // amx_psay <name or #userid> <message> - sends private message
    private cmdPsay(entity: nodemod.Entity | null, args: string[]) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CHAT)) return;

        if (args.length < 2) {
            this.sendConsole(entity, this.getLang(entity, 'USAGE_PSAY'));
            return;
        }

        const targetArg = args[0];
        let message = args.slice(1).join(' ');

        // HLSW fix: remove outer quotes if message is wrapped in them
        if (message.length >= 2 && message[0] === '"' && message[message.length - 1] === '"') {
            message = message.substring(1, message.length - 1).trim();
        }

        // Find target player using cmdTarget (allows self-targeting for psay)
        const target = adminSystem.cmdTarget(entity, targetArg, CMDTARGET_ALLOW_SELF);
        if (!target) {
            // cmdTarget already sends error message
            return;
        }

        const senderName = this.getAdminName(entity);
        const targetName = this.getPlayerName(target);

        // Dual logging: AMXX internal format + HL engine format
        if (entity) {
            const authId = nodemod.eng.getPlayerAuthId(entity) || '';
            const userId = nodemod.eng.getPlayerUserId(entity);
            const targetAuthId = nodemod.eng.getPlayerAuthId(target) || '';
            const targetUserId = nodemod.eng.getPlayerUserId(target);
            this.logAmx(`Chat: "${senderName}<${userId}><${authId}><>" psay "${targetName}<${targetUserId}><${targetAuthId}><>" "${message}"`);
        }
        this.logMessage(entity, 'amx_psay', { text: message, name: targetName });

        // Format and send
        const formatted = `(${targetName}) ${senderName} :   ${message}`;

        // Send to target
        this.sendChat(target, formatted);

        // Send to sender (if different from target and sender is a player)
        if (entity && entity !== target) {
            this.sendChat(entity, formatted);
        }

        this.sendConsole(entity, formatted);
    }

    // amx_tsay/amx_csay <color> <message> - sends HUD message
    private cmdTsay(entity: nodemod.Entity | null, args: string[], leftSide: boolean) {
        if (!adminSystem.cmdAccess(entity, ADMIN_CHAT)) return;

        if (args.length < 2) {
            this.sendConsole(entity, this.getLang(entity, leftSide ? 'USAGE_TSAY' : 'USAGE_CSAY'));
            return;
        }

        const colorArg = args[0];
        let message: string;
        let color: [number, number, number];

        // Check if first arg is a valid color (supports localized names)
        const colorName = this.lookupColorByLocalizedName(colorArg);
        if (colorName && COLORS[colorName]) {
            color = COLORS[colorName];
            message = args.slice(1).join(' ');
        } else {
            // No valid color, use white and include first arg in message
            color = COLORS['white'];
            message = args.join(' ');
        }

        const name = this.getAdminName(entity);
        const x = leftSide ? 0.05 : -1;
        const y = leftSide ? 0.55 : 0.1;

        // Dual logging: AMXX internal format + HL engine format
        const action = leftSide ? 'amx_tsay' : 'amx_csay';
        if (entity) {
            const authId = nodemod.eng.getPlayerAuthId(entity) || '';
            const userId = nodemod.eng.getPlayerUserId(entity);
            this.logAmx(`Chat: "${name}<${userId}><${authId}><>" ${leftSide ? 'tsay' : 'csay'} "${message}"`);
        }
        const colorDisplayName = colorName ? this.getLang(null, `COLOR_${colorName.toUpperCase()}`) : 'white';
        this.logMessage(entity, action, { text: message, color: colorDisplayName });

        // Send HUD message based on amx_show_activity
        const options = this.getShowActivityOptions();
        for (const player of adminSystem.getPlayers({ excludeBots: true })) {
            const displayMsg = utils.getActivityMessage(player, name, message, options);
            if (displayMsg) {
                this.sendHudMessage(player, displayMsg, color, x, y);
                nodemod.eng.clientPrintf(player, nodemod.PRINT_TYPE.print_console, `${displayMsg}\n`);
            }
        }

        this.sendConsole(entity, `${name} :  ${message}`);
    }
}

// Initialize the admin chat system


export default AdminChat;
