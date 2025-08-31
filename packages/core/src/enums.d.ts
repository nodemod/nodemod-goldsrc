// This file is generated automatically. Don't edit it.
declare namespace nodemod {
  // Metamod result constants
  const enum META_RES {
    UNSET = 0,    // Uninitialized (causes error)
    IGNORED = 1,  // Plugin didn't take any action, continue normally
    HANDLED = 2,  // Plugin did something, but original function still executes
    OVERRIDE = 3, // Execute original function, but use plugin's return value
    SUPERCEDE = 4 // Skip original function entirely, use plugin's behavior
  }

  // Alert types for engine functions
  const enum ALERT_TYPE {
    at_notice = 0,
    at_console = 1,
    at_aiconsole = 2,
    at_warning = 3,
    at_error = 4,
    at_logged = 5
  }

  // Print types for client output
  const enum PRINT_TYPE {
    print_console = 0,
    print_center = 1,
    print_chat = 2
  }

  // Force types for consistency checking
  const enum FORCE_TYPE {
    force_exactfile = 0,
    force_model_samebounds = 1,
    force_model_specifybounds = 2,
    force_model_specifybounds_if_avail = 3
  }

  // Message destination types for engine messaging
  const enum MSG_DEST {
    BROADCAST = 0,      // Message to all players without delivery guarantee
    ONE = 1,           // Message to one player with delivery guarantee
    ALL = 2,           // Message with delivery guarantee to all players
    INIT = 3,          // Write to the init string
    PVS = 4,           // All players in potentially visible set of point
    PAS = 5,           // All players in potentially audible set
    PVS_R = 6,         // All players in PVS with reliable delivery
    PAS_R = 7,         // All players in PAS with reliable delivery
    ONE_UNRELIABLE = 8, // Message to one player without delivery guarantee
    SPEC = 9           // Message to all HLTV proxy
  }

  // Network message types for Half-Life protocol
  const enum MSG_TYPE {
    BAD = 0,
    NOP = 1,
    DISCONNECT = 2,
    EVENT = 3,
    VERSION = 4,
    SETVIEW = 5,
    SOUND = 6,
    TIME = 7,
    PRINT = 8,
    STUFFTEXT = 9,
    SETANGLE = 10,
    SERVERINFO = 11,
    LIGHTSTYLE = 12,
    UPDATEUSERINFO = 13,
    DELTADESCRIPTION = 14,
    CLIENTDATA = 15,
    STOPSOUND = 16,
    PINGS = 17,
    PARTICLE = 18,
    DAMAGE = 19,
    SPAWNSTATIC = 20,
    EVENT_RELIABLE = 21,
    SPAWNBASELINE = 22,
    TEMPENTITY = 23,
    SETPAUSE = 24,
    SIGNONNUM = 25,
    CENTERPRINT = 26,
    KILLEDMONSTER = 27,
    FOUNDSECRET = 28,
    SPAWNSTATICSOUND = 29,
    INTERMISSION = 30,
    FINALE = 31,
    CDTRACK = 32,
    RESTORE = 33,
    CUTSCENE = 34,
    WEAPONANIM = 35,
    DECALNAME = 36,
    ROOMTYPE = 37,
    ADDANGLE = 38,
    NEWUSERMSG = 39,
    PACKETENTITIES = 40,
    DELTAPACKETENTITIES = 41,
    CHOKE = 42,
    RESOURCELIST = 43,
    NEWMOVEVARS = 44,
    RESOURCEREQUEST = 45,
    CUSTOMIZATION = 46,
    CROSSHAIRANGLE = 47,
    SOUNDFADE = 48,
    FILETXFERFAILED = 49,
    HLTV = 50,
    DIRECTOR = 51,
    VOICEINIT = 52,
    VOICEDATA = 53,
    SENDEXTRAINFO = 54,
    TIMESCALE = 55,
    RESOURCELOCATION = 56,
    SENDCVARVALUE = 57,
    SENDCVARVALUE2 = 58
  }

  // Sound attenuation constants for pfnEmitSound
  const enum ATTN {
    NONE = 0,         // No attenuation
    NORM = 0.8,       // Normal attenuation
    IDLE = 2.0,       // Idle attenuation
    STATIC = 1.25     // Static attenuation
  }

  // Sound flags for pfnEmitSound fFlags parameter
  const enum SND {
    SPAWNING = 256,     // 1<<8 - We're spawning, used in some cases for ambients
    STOP = 32,          // 1<<5 - Stop sound
    CHANGE_VOL = 64,    // 1<<6 - Change sound volume
    CHANGE_PITCH = 128  // 1<<7 - Change sound pitch
  }

  // Console variable (cvar) flags
  const enum FCVAR {
    NONE = 0,                // No flags
    ARCHIVE = 1,             // 1<<0 - Set to cause it to be saved to vars.rc
    USERINFO = 2,            // 1<<1 - Changes the client's info string
    SERVER = 4,              // 1<<2 - Notifies players when changed
    EXTDLL = 8,              // 1<<3 - Defined by external DLL
    CLIENTDLL = 16,          // 1<<4 - Defined by the client dll
    PROTECTED = 32,          // 1<<5 - Server cvar, but we don't send the data since it's a password, etc.
    SPONLY = 64,             // 1<<6 - This cvar cannot be changed by clients connected to a multiplayer server
    PRINTABLEONLY = 128,     // 1<<7 - This cvar's string cannot contain unprintable characters
    UNLOGGED = 256,          // 1<<8 - If this is a FCVAR_SERVER, don't log changes to the log file / console
    NOEXTRAWHITEPACE = 512   // 1<<9 - Strip trailing/leading white space from this cvar
  }

  /** pev(entity, pev_button) or pev(entity, pev_oldbuttons) values */
  const enum IN_BUTTON {
    ATTACK = 1,        // 1<<0
    JUMP = 2,          // 1<<1
    DUCK = 4,          // 1<<2
    FORWARD = 8,       // 1<<3
    BACK = 16,         // 1<<4
    USE = 32,          // 1<<5
    CANCEL = 64,       // 1<<6
    LEFT = 128,        // 1<<7
    RIGHT = 256,       // 1<<8
    MOVELEFT = 512,    // 1<<9
    MOVERIGHT = 1024,  // 1<<10
    ATTACK2 = 2048,    // 1<<11
    RUN = 4096,        // 1<<12
    RELOAD = 8192,     // 1<<13
    ALT1 = 16384,      // 1<<14
    SCORE = 32768      // 1<<15 - Used by client.dll for when scoreboard is held down
  }

  /** pev(entity, pev_flags) values */
  const enum FL {
    FLY = 1,                    // 1<<0 - Changes the SV_Movestep() behavior to not need to be on ground
    SWIM = 2,                   // 1<<1 - Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
    CONVEYOR = 4,               // 1<<2
    CLIENT = 8,                 // 1<<3
    INWATER = 16,               // 1<<4
    MONSTER = 32,               // 1<<5
    GODMODE = 64,               // 1<<6
    NOTARGET = 128,             // 1<<7
    SKIPLOCALHOST = 256,        // 1<<8 - Don't send entity to local host, it's predicting this entity itself
    ONGROUND = 512,             // 1<<9 - At rest / on the ground
    PARTIALGROUND = 1024,       // 1<<10 - Not all corners are valid
    WATERJUMP = 2048,           // 1<<11 - Player jumping out of water
    FROZEN = 4096,              // 1<<12 - Player is frozen for 3rd person camera
    FAKECLIENT = 8192,          // 1<<13 - JAC: fake client, simulated server side; don't send network messages to them
    DUCKING = 16384,            // 1<<14 - Player flag -- Player is fully crouched
    FLOAT = 32768,              // 1<<15 - Apply floating force to this entity when in water
    GRAPHED = 65536,            // 1<<16 - Worldgraph has this ent listed as something that blocks a connection
    IMMUNE_WATER = 131072,      // 1<<17
    IMMUNE_SLIME = 262144,      // 1<<18
    IMMUNE_LAVA = 524288,       // 1<<19
    PROXY = 1048576,            // 1<<20 - This is a spectator proxy
    ALWAYSTHINK = 2097152,      // 1<<21 - Brush model flag -- call think every frame regardless of nextthink - ltime
    BASEVELOCITY = 4194304,     // 1<<22 - Base velocity has been applied this frame
    MONSTERCLIP = 8388608,      // 1<<23 - Only collide in with monsters who have FL_MONSTERCLIP set
    ONTRAIN = 16777216,         // 1<<24 - Player is _controlling_ a train
    WORLDBRUSH = 33554432,      // 1<<25 - Not moveable/removeable brush entity
    SPECTATOR = 67108864,       // 1<<26 - This client is a spectator
    CUSTOMENTITY = 536870912,   // 1<<29 - This is a custom entity
    KILLME = 1073741824,        // 1<<30 - This entity is marked for death
    DORMANT = 2147483648        // 1<<31 - Entity is dormant, no updates to client
  }

  /** engfunc(EngFunc_WalkMove, entity, Float:yaw, Float:dist, iMode) iMode values */
  const enum WALKMOVE {
    NORMAL = 0,     // Normal walkmove
    WORLDONLY = 1,  // Doesn't hit ANY entities, no matter what the solid type
    CHECKONLY = 2   // Move, but don't touch triggers
  }

  /** engfunc(EngFunc_MoveToOrigin, entity, Float:goal[3], Float:distance, moveType) moveType values */
  const enum MOVE {
    NORMAL = 0,  // normal move in the direction monster is facing
    STRAFE = 1   // moves in direction specified, no matter which way monster is facing
  }

  /** pev(entity, pev_movetype) values */
  const enum MOVETYPE {
    NONE = 0,           // Never moves
    WALK = 3,           // Player only - moving on the ground
    STEP = 4,           // Gravity, special edge handling -- monsters use this
    FLY = 5,            // No gravity, but still collides with stuff
    TOSS = 6,           // Gravity/Collisions
    PUSH = 7,           // No clip to world, push and crush
    NOCLIP = 8,         // No gravity, no collisions, still do velocity/avelocity
    FLYMISSILE = 9,     // Extra size to monsters
    BOUNCE = 10,        // Just like Toss, but reflect velocity when contacting surfaces
    BOUNCEMISSILE = 11, // Bounce w/o gravity
    FOLLOW = 12,        // Track movement of aiment
    PUSHSTEP = 13       // BSP model that needs physics/world collisions
  }

  /** pev(entity, pev_solid) values */
  const enum SOLID {
    NOT = 0,       // No interaction with other objects
    TRIGGER = 1,   // Touch on edge, but not blocking
    BBOX = 2,      // Touch on edge, block
    SLIDEBOX = 3,  // Touch on edge, but not an onground
    BSP = 4        // BSP clip, touch on edge, block
  }

  /** pev(entity, pev_deadflag) values */
  const enum DEAD {
    NO = 0,           // Alive
    DYING = 1,        // Playing death animation or still falling off of a ledge waiting to hit ground
    DEAD = 2,         // Dead, lying still
    RESPAWNABLE = 3,
    DISCARDBODY = 4
  }

  /** new Float:takedamage, pev(entity, pev_takedamage, takedamage) values */
  const enum DAMAGE {
    NO = 0.0,
    YES = 1.0,
    AIM = 2.0
  }

  /** pev(entity, pev_effects) values */
  const enum EF {
    BRIGHTFIELD = 1,   // Swirling cloud of particles
    MUZZLEFLASH = 2,   // Single frame ELIGHT on entity attachment 0
    BRIGHTLIGHT = 4,   // DLIGHT centered at entity origin
    DIMLIGHT = 8,      // Player flashlight
    INVLIGHT = 16,     // Get lighting from ceiling
    NOINTERP = 32,     // Don't interpolate the next frame
    LIGHT = 64,        // Rocket flare glow sprite
    NODRAW = 128       // Don't draw entity
  }

  /** Spectating camera mode constants (usually stored in pev_iuser1) */
  const enum OBS {
    NONE = 0,
    CHASE_LOCKED = 1,  // Locked Chase Cam
    CHASE_FREE = 2,    // Free Chase Cam
    ROAMING = 3,       // Free Look
    IN_EYE = 4,        // First Person
    MAP_FREE = 5,      // Free Overview
    MAP_CHASE = 6      // Chase Overview
  }

  /** engfunc(EngFunc_PointContents, Float:origin) return values */
  const enum CONTENTS {
    EMPTY = -1,
    SOLID = -2,
    WATER = -3,
    SLIME = -4,
    LAVA = -5,
    SKY = -6,
    ORIGIN = -7,        // Removed at csg time
    CLIP = -8,          // Changed to contents_solid
    CURRENT_0 = -9,
    CURRENT_90 = -10,
    CURRENT_180 = -11,
    CURRENT_270 = -12,
    CURRENT_UP = -13,
    CURRENT_DOWN = -14,
    TRANSLUCENT = -15,
    LADDER = -16,
    FLYFIELD = -17,
    GRAVITY_FLYFIELD = -18,
    FOG = -19
  }

  /** Instant damage values for use with the 3rd parameter of the "Damage" client message */
  const enum DMG {
    GENERIC = 0,         // Generic damage was done
    CRUSH = 1,           // 1<<0 - Crushed by falling or moving object
    BULLET = 2,          // 1<<1 - Shot
    SLASH = 4,           // 1<<2 - Cut, clawed, stabbed
    BURN = 8,            // 1<<3 - Heat burned
    FREEZE = 16,         // 1<<4 - Frozen
    FALL = 32,           // 1<<5 - Fell too far
    BLAST = 64,          // 1<<6 - Explosive blast damage
    CLUB = 128,          // 1<<7 - Crowbar, punch, headbutt
    SHOCK = 256,         // 1<<8 - Electric shock
    SONIC = 512,         // 1<<9 - Sound pulse shockwave
    ENERGYBEAM = 1024,   // 1<<10 - Laser or other high energy beam
    NEVERGIB = 4096,     // 1<<12 - With this bit OR'd in, no damage type will be able to gib victims upon death
    ALWAYSGIB = 8192,    // 1<<13 - With this bit OR'd in, any damage type can be made to gib victims upon death
    DROWN = 16384,       // 1<<14 - Drowning
    PARALYZE = 32768,    // 1<<15 - Slows affected creature down
    NERVEGAS = 65536,    // 1<<16 - Nerve toxins, very bad
    POISON = 131072,     // 1<<17 - Blood poisioning
    RADIATION = 262144,  // 1<<18 - Radiation exposure
    DROWNRECOVER = 524288, // 1<<19 - Drowning recovery
    ACID = 1048576,      // 1<<20 - Toxic chemicals or acid burns
    SLOWBURN = 2097152,  // 1<<21 - In an oven
    SLOWFREEZE = 4194304, // 1<<22 - In a subzero freezer
    MORTAR = 8388608,    // 1<<23 - Hit by air raid
    GRENADE = 16777216   // 1<<24 - Counter-Strike only - Hit by HE grenade
  }

  /** Gib values used on client kill based on instant damage values */
  const enum GIB {
    NORMAL = 0,      // Gib if entity was overkilled
    NEVER = 1,       // Never gib, no matter how much death damage is done
    ALWAYS = 2,      // Always gib
    TRY_HEALTH = -9000 // Gib players if their health is under this value
  }

  /** Valid constants for fNoMonsters parameter of trace functions */
  const enum IGNORE {
    DONT_IGNORE_MONSTERS = 0,
    MONSTERS = 1,
    MISSILE = 2,
    GLASS = 0x100
  }

  /** Hull numbers for trace functions */
  const enum HULL {
    POINT = 0,
    HUMAN = 1,
    LARGE = 2,
    HEAD = 3
  }

  /** Half-Life weapon constants */
  const enum HLW {
    NONE = 0,
    CROWBAR = 1,
    GLOCK = 2,
    PYTHON = 3,
    MP5 = 4,
    CHAINGUN = 5,
    CROSSBOW = 6,
    SHOTGUN = 7,
    RPG = 8,
    GAUSS = 9,
    EGON = 10,
    HORNETGUN = 11,
    HANDGRENADE = 12,
    TRIPMINE = 13,
    SATCHEL = 14,
    SNARK = 15,
    SUIT = 31
  }

  /** Player physics flags */
  const enum PFLAG {
    ONLADDER = 1,    // 1<<0
    ONSWING = 1,     // 1<<0
    ONTRAIN = 2,     // 1<<1
    ONBARNACLE = 4,  // 1<<2
    DUCKING = 8,     // 1<<3 - In the process of ducking, but not totally squatted yet
    USING = 16,      // 1<<4 - Using a continuous entity
    OBSERVER = 32    // 1<<5 - Player is locked in stationary cam mode
  }

  /** Player hide HUD values */
  const enum HIDEHUD {
    WEAPONS = 1,             // 1<<0
    FLASHLIGHT = 2,          // 1<<1
    ALL = 4,                 // 1<<2
    HEALTH = 8,              // 1<<3
    TIMER = 16,              // 1<<4
    MONEY = 32,              // 1<<5
    CROSSHAIR = 64,          // 1<<6
    OBSERVER_CROSSHAIR = 128 // 1<<7
  }

  /** Entity classification */
  const enum CLASS {
    NONE = 0,
    MACHINE = 1,
    PLAYER = 2,
    HUMAN_PASSIVE = 3,
    HUMAN_MILITARY = 4,
    ALIEN_MILITARY = 5,
    ALIEN_PASSIVE = 6,
    ALIEN_MONSTER = 7,
    ALIEN_PREY = 8,
    ALIEN_PREDATOR = 9,
    INSECT = 10,
    PLAYER_ALLY = 11,
    PLAYER_BIOWEAPON = 12,
    ALIEN_BIOWEAPON = 13,
    VEHICLE = 14,
    BARNACLE = 99
  }

  /** Entity use states */
  const enum USE {
    OFF = 0,
    ON = 1,
    SET = 2,
    TOGGLE = 3
  }

  /** PlaybackEvent flags */
  const enum FEV {
    NOTHOST = 1,    // 1<<0 - Skip local host for event send
    RELIABLE = 2,   // 1<<1 - Send the event reliably
    GLOBAL = 4,     // 1<<2 - Send to everybody on the server
    UPDATE = 8,     // 1<<3 - Update existing event instead of duplicate
    HOSTONLY = 16,  // 1<<4 - Only send to entity specified as the invoker
    SERVER = 32,    // 1<<5 - Only send if the event was created on the server
    CLIENT = 64     // 1<<6 - Only issue event client side
  }
}