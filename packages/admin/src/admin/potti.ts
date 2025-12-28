// Potti Plugin - Bot Controller
// Converted from AMX Mod X potti.sma (v1.40 by p3tsin) to TypeScript for NodeMod
// Allows admins to create a bot which can be fully controlled

import nodemodCore from '@nodemod/core';
import { adminSystem } from './admin';
import { ADMIN_LEVEL_D } from './constants';
import * as utils from './utils';
import { Plugin, PluginMetadata } from './pluginloader';
import { BasePlugin } from './baseplugin';

const cvar = nodemodCore.cvar;

// Access level for bot commands (ADMIN_LEVEL_D = "p" in users.ini)
const ACCESS = ADMIN_LEVEL_D;

// HUD channel for showing bot info
const HUD_CHANNEL = 4;

// Task interval for bot info updates (milliseconds)
const BOTINFO_INTERVAL = 1000;

// Commands which are not blocked/executed in fullcontrol mode
const FULLCONTROL_CMDS = [
    'amx_botadd', 'amx_botexec', 'amx_botmove', 'amx_botcmds', 'amx_botdel',
    'specmode', 'follow'
];

// Bot info states
const enum BotInfo {
    NONE = 0,
    MOTD = 1
}

// Weapon slot configurations for different mods
interface WeaponSlotConfig {
    slotsNum: number;
    slots: number[][];
}

class Potti extends BasePlugin implements Plugin {
    readonly metadata: PluginMetadata = {
        name: 'Potti',
        version: '1.40',
        author: 'p3tsin (TS port by NodeMod)',
        description: 'Allows admins to create a bot which can be fully controlled'
    };

    // Using mod (0: other, 1: cstrike, 2: specialists)
    private usingMod = 0;
    private moneyOffset = 115;

    // Player -> Bot mapping (botindex[ownerIndex] = bot entity)
    private botIndex: Map<number, nodemod.Entity | null> = new Map();
    // Bot -> Owner mapping (botowner[botIndex] = owner entity)
    private botOwner: Map<number, nodemod.Entity | null> = new Map();

    // Bot info and menu state per owner
    private botInfo: Map<number, BotInfo> = new Map();
    private hasMenu: Map<number, number> = new Map(); // 0 = none, 1 = bot menu, 2 = slot menu

    // Movement modes per owner
    private copyMovements: Map<number, number> = new Map();
    private fullControl: Map<number, boolean> = new Map();
    private pendingRespawn: Map<number, boolean> = new Map();  // Track bots waiting to respawn

    // Bot movement state (per owner index)
    private botMove: Map<number, number[]> = new Map();
    private botAngles: Map<number, number[]> = new Map();
    private botButtons: Map<number, number> = new Map();
    private botImpulses: Map<number, number> = new Map();

    // Current weapon tracking for TS mod
    private currentWeapon: Map<number, number[]> = new Map();

    // Bot count
    private botsNum = 0;

    // CVAR handles
    private cvHudColor: any;
    private cvHudPos: any;

    // Message IDs
    private msgShowMenu = 0;
    private msgTextMsg = 0;

    // Weapon slot configuration
    private weaponConfig: WeaponSlotConfig = { slotsNum: 0, slots: [] };
    private slotMenuKeys = 0;

    // Bot info interval
    private botInfoInterval: ReturnType<typeof setInterval> | null = null;

    // Event handler references for cleanup
    private eventHandlers: { event: string; handler: (...args: any[]) => void }[] = [];

    constructor(pluginName: string) {
        super(pluginName);

        this.initMod();
        this.registerCvars();
        this.registerCommands();
        this.setupEventHandlers();
    }

    private initMod(): void {
        // Detect mod type - check the current working directory path
        try {
            const cwd = nodemodCore.cwd || process.cwd() || '';
            const cwdLower = cwd.toLowerCase();
            if (cwdLower.includes('cstrike') || cwdLower.includes('czero') || cwdLower.includes('csv15') || cwdLower.includes('cs13')) {
                this.usingMod = 1;
            } else if (cwdLower.includes('ts') || cwdLower.includes('specialists')) {
                this.usingMod = 2;
            }
        } catch {
            this.usingMod = 0;
        }

        // Setup weapon slots based on mod
        this.setupWeaponSlots();
    }

    private setupWeaponSlots(): void {
        switch (this.usingMod) {
            case 1: // Counter-Strike
                this.weaponConfig = {
                    slotsNum: 5,
                    slots: [
                        [3, 5, 7, 8, 12, 13, 14, 15, 18, 19, 20, 21, 22, 23, 24, 27, 28, 30], // Primary
                        [1, 10, 11, 16, 17, 26], // Secondary
                        [29], // Knife
                        [4, 9, 25], // Grenades
                        [6] // Bomb
                    ]
                };
                this.moneyOffset = 115; // 32-bit offset
                break;
            case 2: // The Specialists
                this.weaponConfig = {
                    slotsNum: 6,
                    slots: [
                        [24, 25, 34, 35],
                        [1, 9, 12, 14, 22, 28, 31],
                        [3, 6, 7, 17, 19, 23],
                        [4, 5, 11, 13, 15, 18, 20, 26, 27, 32, 33],
                        [8, 10, 16, 21, 30],
                        [36] // kungfu
                    ]
                };
                break;
            default:
                this.weaponConfig = { slotsNum: 0, slots: [] };
        }

        // Calculate slot menu keys
        this.slotMenuKeys = 0;
        for (let i = 0; i < this.weaponConfig.slotsNum; i++) {
            this.slotMenuKeys |= (1 << i);
        }
    }

    private registerCvars(): void {
        this.cvHudColor = this.registerCvar('potti_hudcolor', '50 150 200', 0, 'HUD message color (R G B)');
        this.cvHudPos = this.registerCvar('potti_hudpos', '0.82 0.85', 0, 'HUD message position (X Y)');
    }

    private registerCommands(): void {
        this.registerCommand('amx_botadd', ACCESS, '<name> - Create a new bot', (entity, args) => {
            this.cmdMakeBot(entity, args);
        });

        this.registerCommand('amx_botexec', ACCESS, '<cmd> - Execute a command on your bot', (entity, args) => {
            this.cmdExecBot(entity, args);
        });

        this.registerCommand('amx_botmove', ACCESS, '<0-3> - Make the bot copy your movements', (entity, args) => {
            this.cmdMoveBot(entity, args);
        });

        this.registerCommand('amx_botcmds', ACCESS, '<0/1> - Send all commands to your bot', (entity, args) => {
            this.cmdCmdsBot(entity, args);
        });

        this.registerCommand('amx_botdel', ACCESS, '- Kicks your bot', (entity, args) => {
            this.cmdRemoveBot(entity);
        });
    }

    private setupEventHandlers(): void {
        // Client disconnect handler
        const disconnectHandler = (entity: nodemod.Entity) => {
            this.onClientDisconnect(entity);
        };
        nodemod.on('dllClientDisconnect', disconnectHandler);
        this.eventHandlers.push({ event: 'dllClientDisconnect', handler: disconnectHandler });

        // Client command handler
        const commandHandler = (entity: nodemod.Entity, text: string) => {
            this.onClientCommand(entity, text);
        };
        nodemod.on('dllClientCommand', commandHandler);
        this.eventHandlers.push({ event: 'dllClientCommand', handler: commandHandler });

        // CmdStart handler for movement copying
        const cmdStartHandler = (player: nodemod.Entity, cmd: nodemod.UserCmd) => {
            this.onCmdStart(player, cmd);
        };
        nodemod.on('dllCmdStart', cmdStartHandler);
        this.eventHandlers.push({ event: 'dllCmdStart', handler: cmdStartHandler });

        // StartFrame handler for bot movement
        const startFrameHandler = () => {
            this.onStartFrame();
        };
        nodemod.on('dllStartFrame', startFrameHandler);
        this.eventHandlers.push({ event: 'dllStartFrame', handler: startFrameHandler });

        // ClientKill handler
        const clientKillHandler = (entity: nodemod.Entity) => {
            this.onClientKill(entity);
        };
        nodemod.on('dllClientKill', clientKillHandler);
        this.eventHandlers.push({ event: 'dllClientKill', handler: clientKillHandler });
    }

    // ============================================================================
    // Bot Management
    // ============================================================================

    private addBotForwards(): void {
        // Start bot info interval
        if (!this.botInfoInterval) {
            this.botInfoInterval = setInterval(() => {
                this.checkBotInfo();
            }, BOTINFO_INTERVAL);
        }
    }

    private delBotForwards(): void {
        if (this.botInfoInterval) {
            clearInterval(this.botInfoInterval);
            this.botInfoInterval = null;
        }
    }

    private resetControls(ownerIndex: number): void {
        this.botMove.set(ownerIndex, [0, 0, 0]);
        this.botAngles.set(ownerIndex, [0, 0, 0]);
        this.botButtons.set(ownerIndex, 0);
        this.botImpulses.set(ownerIndex, 0);
    }

    private getBotIndex(owner: nodemod.Entity): nodemod.Entity | null {
        const index = nodemod.eng.indexOfEdict(owner);
        return this.botIndex.get(index) || null;
    }

    private setBotIndex(owner: nodemod.Entity, bot: nodemod.Entity | null): void {
        const index = nodemod.eng.indexOfEdict(owner);
        this.botIndex.set(index, bot);
    }

    private getBotOwner(bot: nodemod.Entity): nodemod.Entity | null {
        const index = nodemod.eng.indexOfEdict(bot);
        return this.botOwner.get(index) || null;
    }

    private setBotOwner(bot: nodemod.Entity, owner: nodemod.Entity | null): void {
        const index = nodemod.eng.indexOfEdict(bot);
        this.botOwner.set(index, owner);
    }

    private getOwnerIndex(owner: nodemod.Entity): number {
        return nodemod.eng.indexOfEdict(owner);
    }


    // ============================================================================
    // Command Implementations
    // ============================================================================

    private cmdMakeBot(entity: nodemod.Entity | null, args: string[]): void {
        if (!adminSystem.cmdAccess(entity, ACCESS)) return;
        if (!entity) {
            this.sendConsole(null, '[Potti] This command must be used by a player');
            return;
        }

        if (args.length < 1) {
            this.sendConsole(entity, '[Potti] Usage: amx_botadd <name>');
            return;
        }

        if (utils.isConnected(this.getBotIndex(entity))) {
            this.sendConsole(entity, '[Potti] You already have a bot connected!');
            return;
        }

        const name = args.join(' ');
        const bot = nodemod.eng.createFakeClient(name);

        if (!bot) {
            this.sendConsole(entity, "[Potti] Couldn't create a bot, server full?");
            return;
        }
        const botIndex = nodemod.eng.indexOfEdict(bot);

        // Free entity private data and set up bot settings
        nodemod.eng.freeEntPrivateData(bot);
        this.setBotSettings(bot);

        if (!this.botsNum) {
            this.addBotForwards();
        }

        // Connect the bot
        const rejectReason = nodemod.dll.clientConnect(bot, name, '127.0.0.1', '');

        if (!utils.isConnected(bot)) {
            if (!this.botsNum) {
                this.delBotForwards();
            }
            this.sendConsole(entity, `[Potti] Connection rejected: ${rejectReason}`);
            return;
        }

        // Put bot in server
        nodemod.dll.clientPutInServer(bot);

        // Set fake client flags
        bot.spawnflags = bot.spawnflags | nodemod.FL.FAKECLIENT;
        bot.flags = bot.flags | nodemod.FL.FAKECLIENT;

        const ownerIndex = this.getOwnerIndex(entity);
        this.resetControls(ownerIndex);
        this.fullControl.set(ownerIndex, false);
        this.copyMovements.set(ownerIndex, 0);
        this.setBotIndex(entity, bot);
        this.setBotOwner(bot, entity);
        this.hasMenu.set(ownerIndex, 0);
        this.botsNum++;

        this.sendConsole(entity, `[Potti] Bot successfully created! Id: ${botIndex}, name: ${name}`);
    }

    private cmdExecBot(entity: nodemod.Entity | null, args: string[]): void {
        if (!adminSystem.cmdAccess(entity, ACCESS)) return;
        if (!entity) {
            this.sendConsole(null, '[Potti] This command must be used by a player');
            return;
        }

        const bot = this.getBotIndex(entity);
        if (!utils.isConnected(bot)) {
            this.sendConsole(entity, '[Potti] You have no bot connected..');
            return;
        }

        if (args.length < 1) {
            this.sendConsole(entity, '[Potti] Usage: amx_botexec <cmd> [args]');
            return;
        }

        const cmd = args[0];
        const botName = bot!.netname || 'Unknown';

        if (cmd === 'say' || cmd === 'say_team') {
            const sayText = args.slice(1).join(' ');
            const team = cmd === 'say_team' ? 1 : 0;
            this.botSay(bot!, team, sayText);
            this.sendConsole(entity, `[Potti] Executed on ${botName}: ${cmd} ${sayText}`);
            return;
        }

        const arg1 = args[1] || '';
        const arg2 = args[2] || '';

        if (cmd === 'kill') {
            nodemod.dll.clientKill(bot!);
        } else if (cmd === 'name') {
            this.setUserInfo(bot!, 'name', arg1);
        } else if (cmd === 'model') {
            this.setUserInfo(bot!, 'model', arg1);
        } else {
            this.botCommand(bot!, cmd, arg1, arg2);
        }

        this.sendConsole(entity, `[Potti] Executed on ${botName}: ${cmd} ${arg1} ${arg2}`);
    }

    private cmdMoveBot(entity: nodemod.Entity | null, args: string[]): void {
        if (!adminSystem.cmdAccess(entity, ACCESS)) return;
        if (!entity) {
            this.sendConsole(null, '[Potti] This command must be used by a player');
            return;
        }

        if (!utils.isConnected(this.getBotIndex(entity))) {
            this.sendConsole(entity, '[Potti] You have no bot connected..');
            return;
        }

        if (args.length < 1) {
            this.sendConsole(entity, '[Potti] Usage: amx_botmove <0-3>');
            return;
        }

        let num = parseInt(args[0]) || 0;
        if (num < 0 || num > 3) num = 0;

        const ownerIndex = this.getOwnerIndex(entity);
        this.copyMovements.set(ownerIndex, num);

        let message = '';
        switch (num) {
            case 0: message = 'not copying your moves at all'; break;
            case 1: message = 'copying your movements'; break;
            case 2: message = 'copying your movements and aiming the same spot when shooting'; break;
            case 3: message = 'copying your movements in reverse mode'; break;
        }

        this.sendConsole(entity, `[Potti] Your bot is now ${message}`);
    }

    private cmdCmdsBot(entity: nodemod.Entity | null, args: string[]): void {
        if (!adminSystem.cmdAccess(entity, ACCESS)) return;
        if (!entity) {
            this.sendConsole(null, '[Potti] This command must be used by a player');
            return;
        }

        if (!utils.isConnected(this.getBotIndex(entity))) {
            this.sendConsole(entity, '[Potti] You have no bot connected..');
            return;
        }

        if (args.length < 1) {
            this.sendConsole(entity, '[Potti] Usage: amx_botcmds <0/1>');
            return;
        }

        const value = parseInt(args[0]) ? true : false;
        const ownerIndex = this.getOwnerIndex(entity);
        this.fullControl.set(ownerIndex, value);

        this.sendConsole(entity, `[Potti] Fullcontrol mode: ${value ? "ON (you can't control yourself now)" : 'OFF'}`);
        this.slotMenu(entity, value);
    }

    private cmdRemoveBot(entity: nodemod.Entity | null): void {
        if (!adminSystem.cmdAccess(entity, ACCESS)) return;
        if (!entity) {
            this.sendConsole(null, '[Potti] This command must be used by a player');
            return;
        }

        const bot = this.getBotIndex(entity);
        if (utils.isConnected(bot)) {
            const name = bot!.netname || 'Unknown';
            const userId = nodemod.eng.getPlayerUserId(bot!);
            this.sendConsole(entity, `[Potti] Bot ${name} kicked!`);
            nodemod.eng.serverCommand(`kick #${userId}\n`);
        } else {
            this.sendConsole(entity, '[Potti] You have no bot connected..');
        }
    }

    // ============================================================================
    // Event Handlers
    // ============================================================================

    private onClientDisconnect(entity: nodemod.Entity): void {
        const index = nodemod.eng.indexOfEdict(entity);

        if (utils.isBot(entity)) {
            const owner = this.getBotOwner(entity);
            if (owner) {
                const ownerIndex = this.getOwnerIndex(owner);
                this.copyMovements.set(ownerIndex, 0);
                this.setBotOwner(entity, null);
                this.setBotIndex(owner, null);
                this.botsNum--;
                if (!this.botsNum) {
                    this.delBotForwards();
                }
            }
        } else {
            const bot = this.getBotIndex(entity);
            if (utils.isConnected(bot)) {
                const userId = nodemod.eng.getPlayerUserId(bot!);
                nodemod.eng.serverCommand(`kick #${userId}\n`);
            }
            this.fullControl.set(index, false);
            this.copyMovements.set(index, 0);
            this.hasMenu.set(index, 0);
            this.setBotIndex(entity, null);
        }
    }

    private onClientCommand(entity: nodemod.Entity, text: string): void {
        const ownerIndex = this.getOwnerIndex(entity);
        const bot = this.getBotIndex(entity);

        if (!bot) return;

        const args = utils.parseCommand(text);
        const cmd = args[0]?.toLowerCase() || '';

        const menu = this.hasMenu.get(ownerIndex) || 0;

        if (menu && cmd === 'menuselect') {
            const num = parseInt(args[1]) || 0;
            this.hasMenu.set(ownerIndex, 0);

            if (this.fullControl.get(ownerIndex)) {
                this.slotMenu(entity, true);
            }

            if (menu === 1) {
                nodemod.eng.clientCommand(bot, `menuselect ${num}\n`);
            } else if (menu === 2) {
                this.changeToWeapon(bot, num);
            }

            nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
            return;
        } else if (this.fullControl.get(ownerIndex)) {
            // Check if command is in the allowed list
            for (const allowedCmd of FULLCONTROL_CMDS) {
                if (cmd === allowedCmd.toLowerCase()) {
                    return; // Let it through
                }
            }

            if (cmd === 'say' || cmd === 'say_team') {
                const sayText = args.slice(1).join(' ');
                const team = cmd === 'say_team' ? 1 : 0;
                this.botSay(bot, team, sayText);
            } else {
                this.botCommand(bot, cmd, args[1] || '', args[2] || '');
            }

            nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
        }
    }

    private onCmdStart(player: nodemod.Entity, cmd: nodemod.UserCmd): void {
        const ownerIndex = this.getOwnerIndex(player);
        const bot = this.getBotIndex(player);
        const movement = this.copyMovements.get(ownerIndex) || 0;

        if (!bot || !movement) return;

        const alive = utils.isAlive(player);
        let buttons = cmd.buttons;

        // If owner is dead, prevent certain actions from affecting them
        if (!alive) {
            const mask = nodemod.IN_BUTTON.JUMP | nodemod.IN_BUTTON.ATTACK |
                nodemod.IN_BUTTON.ATTACK2 | nodemod.IN_BUTTON.FORWARD |
                nodemod.IN_BUTTON.BACK | nodemod.IN_BUTTON.MOVELEFT |
                nodemod.IN_BUTTON.MOVERIGHT;
            cmd.buttons = buttons & ~mask;
        }

        if (utils.isAlive(bot)) {
            // Store movement data
            this.botMove.set(ownerIndex, [cmd.forwardmove, cmd.sidemove, cmd.upmove]);

            let angles = [...cmd.viewangles];

            if (movement === 2 && alive && (buttons & nodemod.IN_BUTTON.ATTACK)) {
                // Aim at owner's look target
                const target = this.getUserAim(player);
                angles = this.aimAtOrigin(bot, target);
            } else if (movement === 3 && alive) {
                // Reverse mode
                angles[1] += (angles[1] < 180.0) ? 180.0 : -180.0;
                if (buttons & nodemod.IN_BUTTON.JUMP) {
                    buttons = (buttons & ~nodemod.IN_BUTTON.JUMP) | nodemod.IN_BUTTON.DUCK;
                } else if (buttons & nodemod.IN_BUTTON.DUCK) {
                    buttons = (buttons & ~nodemod.IN_BUTTON.DUCK) | nodemod.IN_BUTTON.JUMP;
                }
            }

            this.botAngles.set(ownerIndex, angles);
            this.botButtons.set(ownerIndex, buttons);
            this.botImpulses.set(ownerIndex, cmd.impulse);
        }
    }

    // Debug frame counter
    private debugFrameCount = 0;

    private onStartFrame(): void {
        if (!this.botsNum) return;

        // Get frametime for msec calculation (matches original glb_frametime)
        const msec = Math.min(255, Math.max(1, Math.round((nodemod as any).frametime * 1000)));

        // Iterate through all players to find bots
        for (const player of nodemod.players) {
            if (!player) continue;
            if (!utils.isBot(player)) continue;

            const owner = this.getBotOwner(player);
            if (!owner) continue;

            const ownerIndex = this.getOwnerIndex(owner);
            const angles = this.botAngles.get(ownerIndex) || [0, 0, 0];
            const move = this.botMove.get(ownerIndex) || [0, 0, 0];
            const buttons = this.botButtons.get(ownerIndex) || 0;
            const impulses = this.botImpulses.get(ownerIndex) || 0;

            const alive = utils.isAlive(player);
            let finalButtons = buttons;
            const botIndex = nodemod.eng.indexOfEdict(player);
            if (alive) {
                player.v_angle = angles;
                const displayAngles = [...angles];
                displayAngles[0] /= -3.0;
                player.angles = displayAngles;
                // Clear respawn flag when alive
                this.pendingRespawn.delete(botIndex);
            } else {
                // Bot is dead - try to respawn once
                if (!this.pendingRespawn.has(botIndex) && player.deadflag >= 2) {
                    this.pendingRespawn.set(botIndex, true);
                    // Respawn and reinitialize
                    nodemod.dll.spawn(player);
                    // Restore fake client flags after spawn
                    player.flags = player.flags | nodemod.FL.FAKECLIENT;
                    player.spawnflags = player.spawnflags | nodemod.FL.FAKECLIENT;
                }
                finalButtons = nodemod.IN_BUTTON.ATTACK | nodemod.IN_BUTTON.JUMP;
            }

            nodemod.eng.runPlayerMove(
                player,
                angles,
                move[0],  // forwardmove
                move[1],  // sidemove
                move[2],  // upmove
                finalButtons,
                impulses,
                msec
            );
        }
    }

    private onClientKill(entity: nodemod.Entity): void {
        const ownerIndex = this.getOwnerIndex(entity);
        const bot = this.getBotIndex(entity);

        if (bot && this.fullControl.get(ownerIndex)) {
            if (utils.isAlive(bot)) {
                nodemod.dll.clientKill(bot);
            } else {
                this.sendConsole(entity, "[Potti] Can't suicide -- your bot is already dead");
            }
            nodemod.setMetaResult(nodemod.META_RES.SUPERCEDE);
        }
    }

    // ============================================================================
    // Bot Info Display
    // ============================================================================

    private checkBotInfo(): void {
        // Parse HUD color
        const colorStr = this.cvHudColor?.value || '50 150 200';
        const colorParts = colorStr.split(' ').map((p: string) => parseInt(p) || 0);
        const color = {
            r: colorParts[0] || 50,
            g: colorParts[1] || 150,
            b: colorParts[2] || 200
        };

        // Parse HUD position
        const posStr = this.cvHudPos?.value || '0.82 0.85';
        const posParts = posStr.split(' ').map((p: string) => parseFloat(p) || 0);
        const pos = {
            x: posParts[0] || 0.82,
            y: posParts[1] || 0.85
        };

        const holdTime = (this.usingMod === 2) ? 120.0 : 1.0;

        // Iterate through all bots
        for (const bot of nodemod.players) {
            if (!bot) continue;
            if (!utils.isBot(bot)) continue;

            const owner = this.getBotOwner(bot);
            if (!owner) continue;

            const ownerIndex = this.getOwnerIndex(owner);

            // Handle pending bot info events
            const info = this.botInfo.get(ownerIndex) || BotInfo.NONE;
            if (info === BotInfo.MOTD) {
                bot.button = nodemod.IN_BUTTON.ATTACK;
                nodemodCore.util.sendChat('[Potti] Note: a MOTD window was closed on your bot', owner);
            }
            this.botInfo.set(ownerIndex, BotInfo.NONE);

            if (!utils.isAlive(bot)) {
                continue;
            }

            const name = bot.netname || 'Unknown';
            const health = Math.floor(bot.health);
            const weapon = this.getCurrentWeapon(bot);
            const weaponName = this.getWeaponName(weapon.id);

            let text = `Name: ${name}, Health: ${health}`;
            text += `\nWeapon: ${weaponName}, Ammo: ${weapon.clip}/${weapon.ammo}`;

            if (this.usingMod === 1) {
                const money = this.getUserMoney(bot);
                text += `\nMoney left: $${money}`;
            }

            // Send HUD message to owner
            // HUD x,y use fixed-point where 8192 = 1.0 (rightmost/bottommost)
            try {
                nodemodCore.util.showHudText(owner, text, {
                    channel: HUD_CHANNEL,
                    x: Math.floor(pos.x * 8192),
                    y: Math.floor(pos.y * 8192),
                    r1: color.r,
                    g1: color.g,
                    b1: color.b,
                    a1: 255,
                    holdTime: Math.floor(holdTime * 256)
                });
            } catch {
                // Ignore HUD errors
            }
        }
    }

    // ============================================================================
    // Utility Functions
    // ============================================================================

    private setBotSettings(bot: nodemod.Entity): void {
        this.setUserInfo(bot, 'model', 'gordon');
        this.setUserInfo(bot, 'rate', '3500');
        this.setUserInfo(bot, 'cl_updaterate', '30');
        this.setUserInfo(bot, 'cl_lw', '0');
        this.setUserInfo(bot, 'cl_lc', '0');
        this.setUserInfo(bot, 'tracker', '0');
        this.setUserInfo(bot, 'cl_dlmax', '128');
        this.setUserInfo(bot, 'lefthand', '1');
        this.setUserInfo(bot, 'friends', '0');
        this.setUserInfo(bot, 'dm', '0');
        this.setUserInfo(bot, 'ah', '1');
        this.setUserInfo(bot, '*bot', '1');
        this.setUserInfo(bot, '_cl_autowepswitch', '1');
        this.setUserInfo(bot, '_vgui_menu', '0');
        this.setUserInfo(bot, '_vgui_menus', '0');
    }

    private setUserInfo(entity: nodemod.Entity, key: string, value: string): void {
        const index = nodemod.eng.indexOfEdict(entity);
        nodemod.eng.setClientKeyValue(index, entity, key, value);
    }

    private botCommand(bot: nodemod.Entity, cmd: string, arg1: string = '', arg2: string = ''): void {
        let fullCmd = cmd;
        if (arg1) fullCmd += ` ${arg1}`;
        if (arg2) fullCmd += ` ${arg2}`;
        nodemod.eng.clientCommand(bot, `${fullCmd}\n`);
    }

    private botSay(bot: nodemod.Entity, team: number, text: string): void {
        const cmd = team ? 'say_team' : 'say';
        nodemod.eng.clientCommand(bot, `${cmd} "${text}"\n`);
    }

    private slotMenu(entity: nodemod.Entity, mode: boolean): void {
        const ownerIndex = this.getOwnerIndex(entity);
        this.hasMenu.set(ownerIndex, mode ? 2 : 0);

        // Send ShowMenu message
        const msgId = this.getShowMenuMsgId();
        if (msgId) {
            nodemod.eng.messageBegin(nodemod.MSG_DEST.ONE, msgId, [0, 0, 0], entity);
            nodemod.eng.writeShort(mode ? this.slotMenuKeys : 0);
            nodemod.eng.writeChar(-1);
            nodemod.eng.writeByte(0);
            nodemod.eng.writeString('\n');
            nodemod.eng.messageEnd();
        }
    }

    private getShowMenuMsgId(): number {
        if (!this.msgShowMenu) {
            this.msgShowMenu = nodemod.eng.regUserMsg('ShowMenu', -1) || 0;
        }
        return this.msgShowMenu;
    }

    private getUserAim(entity: nodemod.Entity): number[] {
        const origin = entity.origin || [0, 0, 0];
        const viewOfs = entity.view_ofs || [0, 0, 0];
        const eyePos = [
            origin[0] + viewOfs[0],
            origin[1] + viewOfs[1],
            origin[2] + viewOfs[2]
        ];

        const vAngle = entity.v_angle || [0, 0, 0];
        nodemod.eng.makeVectors(vAngle);

        // Calculate end point 9999 units forward
        const forward: number[] = [0, 0, 0];
        const right: number[] = [0, 0, 0];
        const up: number[] = [0, 0, 0];
        nodemod.eng.angleVectors(vAngle, forward, right, up);

        const endPos = [
            eyePos[0] + forward[0] * 9999,
            eyePos[1] + forward[1] * 9999,
            eyePos[2] + forward[2] * 9999
        ];

        // Trace line
        const trace = nodemod.eng.traceLine(eyePos, endPos, 0, entity);
        return trace.endPos as number[] || endPos;
    }

    private aimAtOrigin(bot: nodemod.Entity, target: number[]): number[] {
        const origin = bot.origin || [0, 0, 0];
        const delta = [
            origin[0] - target[0],
            origin[1] - target[1],
            origin[2] - target[2] + 16.0 // Bot aims slightly high
        ];

        const distance2d = Math.sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
        const pitch = Math.atan(delta[2] / distance2d) * (180.0 / Math.PI);
        let yaw = Math.atan(delta[1] / delta[0]) * (180.0 / Math.PI);

        if (delta[0] >= 0.0) {
            yaw += 180.0;
        }

        return [pitch, yaw, 0];
    }

    private changeToWeapon(bot: nodemod.Entity, slot: number): number {
        if (slot > this.weaponConfig.slotsNum) return 0;

        if (this.usingMod === 2 && slot === 6) {
            nodemod.eng.clientCommand(bot, 'weapon_0\n');
            return 1;
        }

        // Get bot's weapons
        const weapons = bot.weapons || 0;
        const slotWeapons = this.weaponConfig.slots[slot - 1] || [];

        for (const weaponId of slotWeapons) {
            if (weapons & (1 << weaponId)) {
                const weaponName = this.getWeaponName(weaponId);
                if (this.usingMod === 2) {
                    nodemod.eng.clientCommand(bot, `weapon_${weaponId}\n`);
                } else {
                    nodemod.eng.clientCommand(bot, `${weaponName}\n`);
                }
                return 1;
            }
        }

        return 0;
    }

    private getCurrentWeapon(entity: nodemod.Entity): { id: number; clip: number; ammo: number } {
        // For TS mod with bots, use stored weapon data
        if (this.usingMod === 2 && utils.isBot(entity)) {
            const index = nodemod.eng.indexOfEdict(entity);
            const data = this.currentWeapon.get(index) || [0, 0, 0];
            return { id: data[0], clip: data[1], ammo: data[2] };
        }

        // Use client data weapon ID if available
        // For now return basic info from entity
        return { id: 0, clip: 0, ammo: 0 };
    }

    private getWeaponName(weaponId: number): string {
        // Basic weapon name lookup
        const csWeapons: { [key: number]: string } = {
            1: 'weapon_p228',
            3: 'weapon_scout',
            4: 'weapon_hegrenade',
            5: 'weapon_xm1014',
            6: 'weapon_c4',
            7: 'weapon_mac10',
            8: 'weapon_aug',
            9: 'weapon_smokegrenade',
            10: 'weapon_elite',
            11: 'weapon_fiveseven',
            12: 'weapon_ump45',
            13: 'weapon_sg550',
            14: 'weapon_galil',
            15: 'weapon_famas',
            16: 'weapon_usp',
            17: 'weapon_glock18',
            18: 'weapon_awp',
            19: 'weapon_mp5navy',
            20: 'weapon_m249',
            21: 'weapon_m3',
            22: 'weapon_m4a1',
            23: 'weapon_tmp',
            24: 'weapon_g3sg1',
            25: 'weapon_flashbang',
            26: 'weapon_deagle',
            27: 'weapon_sg552',
            28: 'weapon_ak47',
            29: 'weapon_knife',
            30: 'weapon_p90'
        };

        if (this.usingMod === 1) {
            const name = csWeapons[weaponId];
            return name ? name.replace('weapon_', '') : 'unknown';
        }

        return `weapon_${weaponId}`;
    }

    private getUserMoney(entity: nodemod.Entity): number {
        // This would require private data access which may not be available
        // Return 0 for now
        return 0;
    }

    // ============================================================================
    // Lifecycle
    // ============================================================================

    onUnload(): void {
        // Cleanup intervals
        this.delBotForwards();

        // Kick all bots owned by this plugin
        for (const player of nodemod.players) {
            if (!player) continue;
            if (!utils.isBot(player)) continue;

            const owner = this.getBotOwner(player);
            if (owner) {
                const userId = nodemod.eng.getPlayerUserId(player);
                nodemod.eng.serverCommand(`kick #${userId}\n`);
            }
        }
    }
}

export default Potti;
