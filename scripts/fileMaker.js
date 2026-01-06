const fileMaker = {
  makeFile(...body) {
    return `// This file is generated automatically. Don't edit it.\n${body.join('\n')}`;
  },
  makeBlock(prefix, body) {
    return `${prefix} {\n${body.split('\n').map(v => `  ${v}`).join('\n')}\n};`;
  },
  typings: {
    makeIndex(computed, structureInterfaces = [], eventNames = [], eventInterfaces = [], hamData = null, constGroups = {}, constEnums = {}) {
      // Split into multiple files for better organization
      const enums = this.makeEnums(constGroups, constEnums);
      const eventTypes = this.makeEventTypes(eventNames, eventInterfaces);
      const structures = this.makeStructures(structureInterfaces);
      const engine = this.makeEngine(computed);
      const dll = this.makeDLL(computed);
      const ham = this.makeHam(hamData);
      const core = this.makeCore();

      return {
        'enums.d.ts': enums,
        'events.d.ts': eventTypes,
        'structures.d.ts': structures,
        'engine.d.ts': engine,
        'dll.d.ts': dll,
        'ham.d.ts': ham,
        'index.d.ts': core
      };
    },
    
    makeEnums(constGroups = {}, constEnums = {}) {
      // Helper to generate enum from parsed const group
      const generateEnum = (name, group, doc = '') => {
        if (!group || group.length === 0) return null;
        const docComment = doc ? `  /** ${doc} */\n` : '';
        const members = group.map(c => {
          const comment = c.comment ? ` // ${c.comment}` : '';
          return `    ${c.name} = ${c.value},${comment}`;
        }).join('\n');
        return `${docComment}  const enum ${name} {\n${members}\n  }`;
      };

      // Helper to generate enum from C++ enum
      const generateCppEnum = (name, members, doc = '') => {
        if (!members || members.length === 0) return null;
        const docComment = doc ? `  /** ${doc} */\n` : '';
        const lines = members.map(m => {
          const comment = m.comment ? ` // ${m.comment}` : '';
          return `    ${m.name} = ${m.value},${comment}`;
        }).join('\n');
        return `${docComment}  const enum ${name} {\n${lines}\n  }`;
      };

      // Build dynamic enums from parsed constants
      const dynamicEnums = [
        generateEnum('FL', constGroups.FL, 'pev(entity, pev_flags) values - from const.h'),
        generateEnum('FTRACE', constGroups.FTRACE, 'Trace flags for globalvars_t.trace_flags'),
        generateEnum('WALKMOVE', constGroups.WALKMOVE, 'engfunc(EngFunc_WalkMove) iMode values'),
        generateEnum('MOVETYPE', constGroups.MOVETYPE, 'pev(entity, pev_movetype) values'),
        generateEnum('SOLID', constGroups.SOLID, 'pev(entity, pev_solid) values'),
        generateEnum('DEAD', constGroups.DEAD, 'pev(entity, pev_deadflag) values'),
        generateEnum('DAMAGE', constGroups.DAMAGE, 'pev(entity, pev_takedamage) values'),
        generateEnum('EF', constGroups.EF, 'pev(entity, pev_effects) values'),
        generateEnum('EFLAG', constGroups.EFLAG, 'Entity flags'),
        generateEnum('TE', constGroups.TE, 'Temp entity types for MSG_TYPE.TEMPENTITY messages'),
        generateEnum('TE_EXPLFLAG', constGroups.TE_EXPLFLAG, 'TE_EXPLOSION flags'),
        generateEnum('TE_BOUNCE', constGroups.TE_BOUNCE, 'TE bounce sound types for TE_MODEL'),
        generateEnum('TEFIRE_FLAG', constGroups.TEFIRE_FLAG, 'TE_FIREFIELD flags'),
        generateEnum('MSG', constGroups.MSG, 'Message destination types for engine messaging'),
        generateEnum('CONTENTS', constGroups.CONTENTS, 'engfunc(EngFunc_PointContents) return values'),
        generateEnum('CHAN', constGroups.CHAN, 'Sound channel constants'),
        generateEnum('ATTN', constGroups.ATTN, 'Sound attenuation constants for pfnEmitSound'),
        generateEnum('PITCH', constGroups.PITCH, 'Sound pitch constants'),
        generateEnum('VOL', constGroups.VOL, 'Sound volume constants'),
        generateEnum('IN', constGroups.IN, 'pev(entity, pev_button) or pev(entity, pev_oldbuttons) values'),
        generateEnum('BREAK', constGroups.BREAK, 'Break model material types'),
        generateEnum('BOUNCE', constGroups.BOUNCE, 'Colliding temp entity sound types'),
        generateCppEnum('kRenderMode', constEnums.kRenderMode, 'Rendering modes'),
        generateCppEnum('kRenderFx', constEnums.kRenderFx, 'Rendering effects'),
      ].filter(e => e !== null);

      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  // Metamod result constants',
        '  const enum MRES {',
        '    UNSET = 0,    // Uninitialized (causes error)',
        '    IGNORED = 1,  // Plugin didn\'t take any action, continue normally',
        '    HANDLED = 2,  // Plugin did something, but original function still executes',
        '    OVERRIDE = 3, // Execute original function, but use plugin\'s return value',
        '    SUPERCEDE = 4 // Skip original function entirely, use plugin\'s behavior',
        '  }',
        '',
        '  // Alert types for engine functions',
        '  const enum ALERT_TYPE {',
        '    at_notice = 0,',
        '    at_console = 1,',
        '    at_aiconsole = 2,',
        '    at_warning = 3,',
        '    at_error = 4,',
        '    at_logged = 5',
        '  }',
        '',
        '  // Print types for client output',
        '  const enum PRINT_TYPE {',
        '    print_console = 0,',
        '    print_center = 1,',
        '    print_chat = 2',
        '  }',
        '',
        '  // Force types for consistency checking',
        '  const enum FORCE_TYPE {',
        '    force_exactfile = 0,',
        '    force_model_samebounds = 1,',
        '    force_model_specifybounds = 2,',
        '    force_model_specifybounds_if_avail = 3',
        '  }',
        '',
        // MSG_DEST is now generated from const.h MSG_* constants
        '',
        '  // Network message types for Half-Life protocol (SVC = Server to Client)',
        '  const enum SVC {',
        '    BAD = 0,',
        '    NOP = 1,',
        '    DISCONNECT = 2,',
        '    EVENT = 3,',
        '    VERSION = 4,',
        '    SETVIEW = 5,',
        '    SOUND = 6,',
        '    TIME = 7,',
        '    PRINT = 8,',
        '    STUFFTEXT = 9,',
        '    SETANGLE = 10,',
        '    SERVERINFO = 11,',
        '    LIGHTSTYLE = 12,',
        '    UPDATEUSERINFO = 13,',
        '    DELTADESCRIPTION = 14,',
        '    CLIENTDATA = 15,',
        '    STOPSOUND = 16,',
        '    PINGS = 17,',
        '    PARTICLE = 18,',
        '    DAMAGE = 19,',
        '    SPAWNSTATIC = 20,',
        '    EVENT_RELIABLE = 21,',
        '    SPAWNBASELINE = 22,',
        '    TEMPENTITY = 23,',
        '    SETPAUSE = 24,',
        '    SIGNONNUM = 25,',
        '    CENTERPRINT = 26,',
        '    KILLEDMONSTER = 27,',
        '    FOUNDSECRET = 28,',
        '    SPAWNSTATICSOUND = 29,',
        '    INTERMISSION = 30,',
        '    FINALE = 31,',
        '    CDTRACK = 32,',
        '    RESTORE = 33,',
        '    CUTSCENE = 34,',
        '    WEAPONANIM = 35,',
        '    DECALNAME = 36,',
        '    ROOMTYPE = 37,',
        '    ADDANGLE = 38,',
        '    NEWUSERMSG = 39,',
        '    PACKETENTITIES = 40,',
        '    DELTAPACKETENTITIES = 41,',
        '    CHOKE = 42,',
        '    RESOURCELIST = 43,',
        '    NEWMOVEVARS = 44,',
        '    RESOURCEREQUEST = 45,',
        '    CUSTOMIZATION = 46,',
        '    CROSSHAIRANGLE = 47,',
        '    SOUNDFADE = 48,',
        '    FILETXFERFAILED = 49,',
        '    HLTV = 50,',
        '    DIRECTOR = 51,',
        '    VOICEINIT = 52,',
        '    VOICEDATA = 53,',
        '    SENDEXTRAINFO = 54,',
        '    TIMESCALE = 55,',
        '    RESOURCELOCATION = 56,',
        '    SENDCVARVALUE = 57,',
        '    SENDCVARVALUE2 = 58',
        '  }',
        '',
        // ATTN is now generated from const.h ATTN_* constants
        '',
        '  // Sound flags for pfnEmitSound fFlags parameter',
        '  const enum SND {',
        '    SPAWNING = 256,     // 1<<8 - We\'re spawning, used in some cases for ambients',
        '    STOP = 32,          // 1<<5 - Stop sound',
        '    CHANGE_VOL = 64,    // 1<<6 - Change sound volume',
        '    CHANGE_PITCH = 128  // 1<<7 - Change sound pitch',
        '  }',
        '',
        '  // Console variable (cvar) flags',
        '  const enum FCVAR {',
        '    NONE = 0,                // No flags',
        '    ARCHIVE = 1,             // 1<<0 - Set to cause it to be saved to vars.rc',
        '    USERINFO = 2,            // 1<<1 - Changes the client\'s info string',
        '    SERVER = 4,              // 1<<2 - Notifies players when changed',
        '    EXTDLL = 8,              // 1<<3 - Defined by external DLL',
        '    CLIENTDLL = 16,          // 1<<4 - Defined by the client dll',
        '    PROTECTED = 32,          // 1<<5 - Server cvar, but we don\'t send the data since it\'s a password, etc.',
        '    SPONLY = 64,             // 1<<6 - This cvar cannot be changed by clients connected to a multiplayer server',
        '    PRINTABLEONLY = 128,     // 1<<7 - This cvar\'s string cannot contain unprintable characters',
        '    UNLOGGED = 256,          // 1<<8 - If this is a FCVAR_SERVER, don\'t log changes to the log file / console',
        '    NOEXTRAWHITEPACE = 512   // 1<<9 - Strip trailing/leading white space from this cvar',
        '  }',
        '',
        // IN (buttons) is now generated from const.h IN_* constants
        '',
        // FL is now generated from const.h FL_* constants
        '',
        // WALKMOVE is now generated from const.h WALKMOVE_* constants
        '',
        '  /** engfunc(EngFunc_MoveToOrigin, entity, Float:goal[3], Float:distance, moveType) moveType values */',
        '  const enum MOVE {',
        '    NORMAL = 0,  // normal move in the direction monster is facing',
        '    STRAFE = 1   // moves in direction specified, no matter which way monster is facing',
        '  }',
        '',
        // MOVETYPE is now generated from const.h MOVETYPE_* constants
        '',
        // SOLID, DEAD, DAMAGE, EF are now generated from const.h
        '',
        '  /** Spectating camera mode constants (usually stored in pev_iuser1) */',
        '  const enum OBS {',
        '    NONE = 0,',
        '    CHASE_LOCKED = 1,  // Locked Chase Cam',
        '    CHASE_FREE = 2,    // Free Chase Cam',
        '    ROAMING = 3,       // Free Look',
        '    IN_EYE = 4,        // First Person',
        '    MAP_FREE = 5,      // Free Overview',
        '    MAP_CHASE = 6      // Chase Overview',
        '  }',
        '',
        // CONTENTS is now generated from const.h CONTENTS_* constants
        '',
        '  /** Instant damage values for use with the 3rd parameter of the "Damage" client message */',
        '  const enum DMG {',
        '    GENERIC = 0,         // Generic damage was done',
        '    CRUSH = 1,           // 1<<0 - Crushed by falling or moving object',
        '    BULLET = 2,          // 1<<1 - Shot',
        '    SLASH = 4,           // 1<<2 - Cut, clawed, stabbed',
        '    BURN = 8,            // 1<<3 - Heat burned',
        '    FREEZE = 16,         // 1<<4 - Frozen',
        '    FALL = 32,           // 1<<5 - Fell too far',
        '    BLAST = 64,          // 1<<6 - Explosive blast damage',
        '    CLUB = 128,          // 1<<7 - Crowbar, punch, headbutt',
        '    SHOCK = 256,         // 1<<8 - Electric shock',
        '    SONIC = 512,         // 1<<9 - Sound pulse shockwave',
        '    ENERGYBEAM = 1024,   // 1<<10 - Laser or other high energy beam',
        '    NEVERGIB = 4096,     // 1<<12 - With this bit OR\'d in, no damage type will be able to gib victims upon death',
        '    ALWAYSGIB = 8192,    // 1<<13 - With this bit OR\'d in, any damage type can be made to gib victims upon death',
        '    DROWN = 16384,       // 1<<14 - Drowning',
        '    PARALYZE = 32768,    // 1<<15 - Slows affected creature down',
        '    NERVEGAS = 65536,    // 1<<16 - Nerve toxins, very bad',
        '    POISON = 131072,     // 1<<17 - Blood poisioning',
        '    RADIATION = 262144,  // 1<<18 - Radiation exposure',
        '    DROWNRECOVER = 524288, // 1<<19 - Drowning recovery',
        '    ACID = 1048576,      // 1<<20 - Toxic chemicals or acid burns',
        '    SLOWBURN = 2097152,  // 1<<21 - In an oven',
        '    SLOWFREEZE = 4194304, // 1<<22 - In a subzero freezer',
        '    MORTAR = 8388608,    // 1<<23 - Hit by air raid',
        '    GRENADE = 16777216   // 1<<24 - Counter-Strike only - Hit by HE grenade',
        '  }',
        '',
        '  /** Gib values used on client kill based on instant damage values */',
        '  const enum GIB {',
        '    NORMAL = 0,      // Gib if entity was overkilled',
        '    NEVER = 1,       // Never gib, no matter how much death damage is done',
        '    ALWAYS = 2,      // Always gib',
        '    TRY_HEALTH = -9000 // Gib players if their health is under this value',
        '  }',
        '',
        '  /** Valid constants for fNoMonsters parameter of trace functions */',
        '  const enum IGNORE {',
        '    DONT_IGNORE_MONSTERS = 0,',
        '    MONSTERS = 1,',
        '    MISSILE = 2,',
        '    GLASS = 0x100',
        '  }',
        '',
        '  /** Hull numbers for trace functions */',
        '  const enum HULL {',
        '    POINT = 0,',
        '    HUMAN = 1,',
        '    LARGE = 2,',
        '    HEAD = 3',
        '  }',
        '',
        '  /** Half-Life weapon constants */',
        '  const enum HLW {',
        '    NONE = 0,',
        '    CROWBAR = 1,',
        '    GLOCK = 2,',
        '    PYTHON = 3,',
        '    MP5 = 4,',
        '    CHAINGUN = 5,',
        '    CROSSBOW = 6,',
        '    SHOTGUN = 7,',
        '    RPG = 8,',
        '    GAUSS = 9,',
        '    EGON = 10,',
        '    HORNETGUN = 11,',
        '    HANDGRENADE = 12,',
        '    TRIPMINE = 13,',
        '    SATCHEL = 14,',
        '    SNARK = 15,',
        '    SUIT = 31',
        '  }',
        '',
        '  /** Player physics flags */',
        '  const enum PFLAG {',
        '    ONLADDER = 1,    // 1<<0',
        '    ONSWING = 1,     // 1<<0',
        '    ONTRAIN = 2,     // 1<<1',
        '    ONBARNACLE = 4,  // 1<<2',
        '    DUCKING = 8,     // 1<<3 - In the process of ducking, but not totally squatted yet',
        '    USING = 16,      // 1<<4 - Using a continuous entity',
        '    OBSERVER = 32    // 1<<5 - Player is locked in stationary cam mode',
        '  }',
        '',
        '  /** Player hide HUD values */',
        '  const enum HIDEHUD {',
        '    WEAPONS = 1,             // 1<<0',
        '    FLASHLIGHT = 2,          // 1<<1',
        '    ALL = 4,                 // 1<<2',
        '    HEALTH = 8,              // 1<<3',
        '    TIMER = 16,              // 1<<4',
        '    MONEY = 32,              // 1<<5',
        '    CROSSHAIR = 64,          // 1<<6',
        '    OBSERVER_CROSSHAIR = 128 // 1<<7',
        '  }',
        '',
        '  /** Entity classification */',
        '  const enum CLASS {',
        '    NONE = 0,',
        '    MACHINE = 1,',
        '    PLAYER = 2,',
        '    HUMAN_PASSIVE = 3,',
        '    HUMAN_MILITARY = 4,',
        '    ALIEN_MILITARY = 5,',
        '    ALIEN_PASSIVE = 6,',
        '    ALIEN_MONSTER = 7,',
        '    ALIEN_PREY = 8,',
        '    ALIEN_PREDATOR = 9,',
        '    INSECT = 10,',
        '    PLAYER_ALLY = 11,',
        '    PLAYER_BIOWEAPON = 12,',
        '    ALIEN_BIOWEAPON = 13,',
        '    VEHICLE = 14,',
        '    BARNACLE = 99',
        '  }',
        '',
        '  /** Entity use states */',
        '  const enum USE {',
        '    OFF = 0,',
        '    ON = 1,',
        '    SET = 2,',
        '    TOGGLE = 3',
        '  }',
        '',
        '  /** PlaybackEvent flags */',
        '  const enum FEV {',
        '    NOTHOST = 1,    // 1<<0 - Skip local host for event send',
        '    RELIABLE = 2,   // 1<<1 - Send the event reliably',
        '    GLOBAL = 4,     // 1<<2 - Send to everybody on the server',
        '    UPDATE = 8,     // 1<<3 - Update existing event instead of duplicate',
        '    HOSTONLY = 16,  // 1<<4 - Only send to entity specified as the invoker',
        '    SERVER = 32,    // 1<<5 - Only send if the event was created on the server',
        '    CLIENT = 64     // 1<<6 - Only issue event client side',
        '  }',
        '',
        // TE, TE_EXPLFLAG, TE_BOUNCE, and other constants are now generated from const.h
        '',
        // Insert all dynamic enums generated from const.h
        ...dynamicEnums,
        '}'
      );
    },

    makeEventTypes(eventNames, eventInterfaces) {
      return fileMaker.makeFile(
        '/// <reference path="./structures.d.ts" />',
        '',
        'declare namespace nodemod {',
        '  // Event callbacks map',
        '  interface EventCallbacks {',
        ...eventInterfaces.map(event => {
          const paramDocs = event.parameters.map(param => 
            `     * @param ${param.name} ${param.originalType} - ${param.type}`
          ).join('\n');
          const hasParams = event.parameters.length > 0;
          const paramSignature = event.parameters.map(p => `${p.name}: ${p.type}`).join(', ');
          
          return `    /**\n     * Event handler for ${event.name}\n${hasParams ? paramDocs + '\n' : ''}     */\n    "${event.name}": (${paramSignature}${event.hasVariadic ? ', ...args: any[]' : ''}) => void;`;
        }),
        '  }',
        '}'
      );
    },

    makeStructures(structureInterfaces) {
      const interfaceDefinitions = structureInterfaces.map(iface => {
        const comment = iface.description ? `  /** ${iface.description} */\n` : '';
        const extendsClause = iface.extends ? ` extends ${iface.extends}` : '';
        const propertyLines = iface.properties.map(prop => {
          if (typeof prop === 'string') {
            return `    ${prop};`;
          } else {
            const propComment = prop.comment ? `    /** ${prop.comment} */\n` : '';
            return `${propComment}    ${prop.name}: ${prop.type};`;
          }
        }).join('\n\n');
        return `${comment}  interface ${iface.name}${extendsClause} {\n${propertyLines}\n  }`;
      }).join('\n');

      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  interface FileHandle {',
        '    // Opaque file handle - use with engine file functions',
        '  }',
        '',
        interfaceDefinitions,
        '}'
      );
    },

    makeEngine(computed) {
      return fileMaker.makeFile(
        '/// <reference path="./structures.d.ts" />',
        '/// <reference path="./enums.d.ts" />',
        '',
        'declare namespace nodemod {',
        `  interface Engine {`,
        computed.eng.map(v => `    /** ${v.api.original} */\n    ${v.api.typing};`).join('\n'),
        `  }`,
        `  const eng: Engine;`,
        '}'
      );
    },

    makeDLL(computed) {
      return fileMaker.makeFile(
        '/// <reference path="./structures.d.ts" />',
        '/// <reference path="./enums.d.ts" />',
        '',
        'declare namespace nodemod {',
        `  interface DLL {`,
        computed.dll.map(v => `    /** ${v.api.original} */\n    ${v.api.typing};`).join('\n'),
        `  }`,
        `  const dll: DLL;`,
        '}'
      );
    },

    makeHam(hamData = null) {
      // Generate HAM enums and types from parsed C++ files if available
      const hamResultEnum = hamData ? hamData.generateHamResultEnum() : `  /** Ham (Hamsandwich) hook result values */
  const enum HAM_RESULT {
    UNSET = 0,      // Default state
    IGNORED = 1,    // Hook had no effect, continue normally
    HANDLED = 2,    // Hook processed the call, but still call original
    OVERRIDE = 3,   // Use hook's return value instead of original
    SUPERCEDE = 4   // Don't call original function at all
  }`;

      const hamFuncEnum = hamData ? hamData.generateHamFuncEnum() : '  const enum HAM_FUNC {}';

      const hamCallbackType = hamData ? hamData.generateHamCallbackType() : `  // Ham callback type mappings - maps HAM_FUNC to callback signature
  // All callbacks receive 'this_' (the hooked entity) as the first parameter
  type HamCallbackFor<T extends HAM_FUNC> =
    (this_: Entity, ...args: any[]) => HAM_RESULT | void;`;

      return fileMaker.makeFile(
        'declare namespace nodemod {',
        hamResultEnum,
        '',
        hamFuncEnum,
        '',
        hamCallbackType,
        '',
        '  // Ham (Hamsandwich) virtual function hooking',
        '  interface Ham {',
        '    /** Register a ham hook on an entity class\'s virtual function with typed callback */',
        '    register<F extends HAM_FUNC>(functionId: F, entityClass: string, callback: HamCallbackFor<F>, isPre: boolean): number;',
        '    /** Unregister a ham hook by ID */',
        '    unregister(hookId: number): void;',
        '    /** Set the return value for the hooked function */',
        '    setReturn(value: any): void;',
        '    /** Get the current return value */',
        '    getReturn(): any;',
        '    /** Get the original return value */',
        '    getOrigReturn(): any;',
        '    /** Set result to SUPERCEDE (skip original function) */',
        '    supercede(): void;',
        '    /** Set result to OVERRIDE (use hook\'s return value) */',
        '    override(): void;',
        '    /** Set result to HANDLED (original still executes) */',
        '    handled(): void;',
        '    /** Set result to IGNORED (no effect) */',
        '    ignored(): void;',
        '    /** Ham function constants */',
        '    readonly funcs: typeof HAM_FUNC;',
        '    /** Ham result constants */',
        '    readonly result: typeof HAM_RESULT;',
        '  }',
        '  const ham: Ham;',
        '}'
      );
    },

    makeCore() {
      return fileMaker.makeFile(
        '/// <reference path="./enums.d.ts" />',
        '/// <reference path="./events.d.ts" />',
        '/// <reference path="./structures.d.ts" />',
        '/// <reference path="./engine.d.ts" />',
        '/// <reference path="./dll.d.ts" />',
        '/// <reference path="./ham.d.ts" />',
        '',
        '// Global Entity constructor',
        'declare class Entity {',
        '  /**',
        '   * Creates a new named entity with proper initialization',
        '   * @param classname - Entity classname (e.g. "weapon_ak47", "info_player_start")',
        '   */',
        '  constructor(classname: string);',
        '}',
        '',
        'declare namespace nodemod {',
        '  // Properties',
        '  const cwd: string;',
        '  const gameDir: string;',
        '  const players: Entity[];',
        '  const mapname: string;',
        '  const time: number;',
        '  const frametime: number;',
        '',
        '  // Event system functions',
        '  function on<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;',
        '  function addEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;',
        '  function addListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;',
        '  function removeListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;',
        '  function removeEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;',
        '  function clearListeners(eventName?: keyof EventCallbacks): void;',
        '  function fire<T extends keyof EventCallbacks>(eventName: T, ...args: Parameters<EventCallbacks[T]>): void;',
        '',
        '  // Utility functions',
        '  function getUserMsgId(msgName: string): number;',
        '  function getUserMsgName(msgId: number): string;',
        '  function setMetaResult(result: MRES): void;',
        '  function getMetaResult(): MRES;',
        '  function continueServer(): void;',
        '}'
      );
    }
  },
  makeFunctions(computed, source) {
    if (!source) {
      return {
        engineFunctionsFile: this.makeFunctions(computed.eng.map(v => v.api), 'eng'),
        dllFunctionsFile: this.makeFunctions(computed.dll.map(v => v.api), 'dll')
      };
    }

    console.log('=== COMPUTED FUNCTIONS DEBUG ===');
    console.log('Total computed:', computed.length);
    console.log('Success:', computed.filter(v => v.status === 'success').length);
    console.log('Failed:', computed.filter(v => v.status === 'failed').length);
    console.log('Failed functions:', computed.filter(v => v.status === 'failed').map(v => v.original));
    if (source === 'dll') {
      console.log('DLL Functions being processed:', computed.map(v => ({ status: v.status, original: v.original })));
    }
    return fileMaker.makeFile(
      [
        '#include <string>',
        '#include "v8.h"',
        '#include "extdll.h"',
        source === 'eng' ? '#include "enginecallback.h"' : '#include "meta_api.h"',
        '#include "node/nodeimpl.hpp"',
        '#include "node/utils.hpp"',
        '',
        '#define V8_STUFF() v8::Isolate* isolate = info.GetIsolate(); \\',
        '  v8::Locker locker(isolate); \\',
        '  v8::HandleScope scope(isolate); \\',
        '  v8::Local<v8::Context> context = isolate->GetCurrentContext()',
        '',
        '#include "structures/structures.hpp"'
      ].join('\n'),
      '',
      source === 'eng' ? 'extern enginefuncs_t	 g_engfuncs;' : 'extern gamedll_funcs_t *gpGamedllFuncs;',
      '',
      computed.filter(v => v.status === 'success').map(v => v.body).join('\n\n'),
      '',
      fileMaker.makeBlock(
        `static std::pair<std::string, v8::FunctionCallback> ${source === 'eng' ? 'engine' : 'gamedll'}SpecificFunctions[] =`,
        computed.filter(v => v.status === 'success').map(v => v.definition).join(',\n')
      ),
      fileMaker.makeBlock(
        `v8::Local<v8::ObjectTemplate> register${source === 'eng' ? 'Engine' : 'Dll'}Functions(v8::Isolate* isolate)`,
        [
          'v8::Local <v8::ObjectTemplate> object = v8::ObjectTemplate::New(isolate);',
          fileMaker.makeBlock(
            `for (auto &routine : ${source === 'eng' ? 'engine' : 'gamedll'}SpecificFunctions)`,
            'object-> Set(v8::String::NewFromUtf8(isolate, routine.first.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), v8::FunctionTemplate::New(isolate, routine.second));'
          ),
          '',
          'return object;'
        ].join('\n')
      ),
      '',
      computed.filter(v => v.status === 'failed').map(v => `// FAILED (${v.reason}): ${v.original}`).join('\n')
    );
  }
};

export default fileMaker;