// This file is generated automatically. Don't edit it.
/// <reference path="./structures.d.ts" />

declare namespace nodemod {
  // Event callbacks map
  interface EventCallbacks {
    /**
     * Event handler for dllGameInit
     */
    "dllGameInit": () => void;
    /**
     * Event handler for dllSpawn
     * @param pent edict_t * - Entity
     */
    "dllSpawn": (pent: Entity) => void;
    /**
     * Event handler for dllThink
     * @param pent edict_t * - Entity
     */
    "dllThink": (pent: Entity) => void;
    /**
     * Event handler for dllUse
     * @param pentUsed edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "dllUse": (pentUsed: Entity, pentOther: Entity) => void;
    /**
     * Event handler for dllTouch
     * @param pentTouched edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "dllTouch": (pentTouched: Entity, pentOther: Entity) => void;
    /**
     * Event handler for dllBlocked
     * @param pentBlocked edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "dllBlocked": (pentBlocked: Entity, pentOther: Entity) => void;
    /**
     * Event handler for dllKeyValue
     * @param pentKeyvalue edict_t * - Entity
     * @param pkvd KeyValueData * - KeyValueData
     */
    "dllKeyValue": (pentKeyvalue: Entity, pkvd: KeyValueData) => void;
    /**
     * Event handler for dllSave
     * @param pent edict_t * - Entity
     * @param pSaveData SAVERESTOREDATA * - SaveRestoreData
     */
    "dllSave": (pent: Entity, pSaveData: SaveRestoreData) => void;
    /**
     * Event handler for dllRestore
     * @param pent edict_t * - Entity
     * @param pSaveData SAVERESTOREDATA * - SaveRestoreData
     * @param globalEntity int - number
     */
    "dllRestore": (pent: Entity, pSaveData: SaveRestoreData, globalEntity: number) => void;
    /**
     * Event handler for dllSetAbsBox
     * @param pent edict_t * - Entity
     */
    "dllSetAbsBox": (pent: Entity) => void;
    /**
     * Event handler for dllSaveWriteFields
     * @param value0 SAVERESTOREDATA* - SaveRestoreData
     * @param value1 const char* - string
     * @param value2 void* - ArrayBuffer | Uint8Array | null
     * @param value3 TYPEDESCRIPTION* - TypeDescription
     * @param value4 int - number
     */
    "dllSaveWriteFields": (value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number) => void;
    /**
     * Event handler for dllSaveReadFields
     * @param value0 SAVERESTOREDATA* - SaveRestoreData
     * @param value1 const char* - string
     * @param value2 void* - ArrayBuffer | Uint8Array | null
     * @param value3 TYPEDESCRIPTION* - TypeDescription
     * @param value4 int - number
     */
    "dllSaveReadFields": (value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number) => void;
    /**
     * Event handler for dllSaveGlobalState
     * @param value0 SAVERESTOREDATA * - SaveRestoreData
     */
    "dllSaveGlobalState": (value0: SaveRestoreData) => void;
    /**
     * Event handler for dllRestoreGlobalState
     * @param value0 SAVERESTOREDATA * - SaveRestoreData
     */
    "dllRestoreGlobalState": (value0: SaveRestoreData) => void;
    /**
     * Event handler for dllResetGlobalState
     */
    "dllResetGlobalState": () => void;
    /**
     * Event handler for dllClientConnect
     * @param pEntity edict_t * - Entity
     * @param pszName const char * - string
     * @param pszAddress const char * - string
     * @param szRejectReason char* - string
     */
    "dllClientConnect": (pEntity: Entity, pszName: string, pszAddress: string, szRejectReason: string) => void;
    /**
     * Event handler for dllClientDisconnect
     * @param pEntity edict_t * - Entity
     */
    "dllClientDisconnect": (pEntity: Entity) => void;
    /**
     * Event handler for dllClientKill
     * @param pEntity edict_t * - Entity
     */
    "dllClientKill": (pEntity: Entity) => void;
    /**
     * Event handler for dllClientPutInServer
     * @param pEntity edict_t * - Entity
     */
    "dllClientPutInServer": (pEntity: Entity) => void;
    /**
     * Event handler for dllClientCommand
     * @param client edict_t * - Entity
     * @param commandText string - string
     */
    "dllClientCommand": (client: Entity, commandText: string) => void;
    /**
     * Event handler for dllClientUserInfoChanged
     * @param pEntity edict_t * - Entity
     * @param infobuffer char * - string
     */
    "dllClientUserInfoChanged": (pEntity: Entity, infobuffer: string) => void;
    /**
     * Event handler for dllServerActivate
     * @param pEdictList edict_t * - Entity
     * @param edictCount int - number
     * @param clientMax int - number
     */
    "dllServerActivate": (pEdictList: Entity, edictCount: number, clientMax: number) => void;
    /**
     * Event handler for dllServerDeactivate
     */
    "dllServerDeactivate": () => void;
    /**
     * Event handler for dllPlayerPreThink
     * @param pEntity edict_t * - Entity
     */
    "dllPlayerPreThink": (pEntity: Entity) => void;
    /**
     * Event handler for dllPlayerPostThink
     * @param pEntity edict_t * - Entity
     */
    "dllPlayerPostThink": (pEntity: Entity) => void;
    /**
     * Event handler for dllStartFrame
     */
    "dllStartFrame": () => void;
    /**
     * Event handler for dllParmsNewLevel
     */
    "dllParmsNewLevel": () => void;
    /**
     * Event handler for dllParmsChangeLevel
     */
    "dllParmsChangeLevel": () => void;
    /**
     * Event handler for dllGetGameDescription
     */
    "dllGetGameDescription": () => void;
    /**
     * Event handler for dllPlayerCustomization
     * @param pEntity edict_t * - Entity
     * @param pCustom customization_t * - Customization
     */
    "dllPlayerCustomization": (pEntity: Entity, pCustom: Customization) => void;
    /**
     * Event handler for dllSpectatorConnect
     * @param pEntity edict_t * - Entity
     */
    "dllSpectatorConnect": (pEntity: Entity) => void;
    /**
     * Event handler for dllSpectatorDisconnect
     * @param pEntity edict_t * - Entity
     */
    "dllSpectatorDisconnect": (pEntity: Entity) => void;
    /**
     * Event handler for dllSpectatorThink
     * @param pEntity edict_t * - Entity
     */
    "dllSpectatorThink": (pEntity: Entity) => void;
    /**
     * Event handler for dllSysError
     * @param error_string const char * - string
     */
    "dllSysError": (error_string: string) => void;
    /**
     * Event handler for dllPMMove
     * @param ppmove struct playermove_s * - PlayerMove
     * @param server qboolean - boolean
     */
    "dllPMMove": (ppmove: PlayerMove, server: boolean) => void;
    /**
     * Event handler for dllPMInit
     * @param ppmove struct playermove_s * - PlayerMove
     */
    "dllPMInit": (ppmove: PlayerMove) => void;
    /**
     * Event handler for dllPMFindTextureType
     * @param name const char * - string
     */
    "dllPMFindTextureType": (name: string) => void;
    /**
     * Event handler for dllSetupVisibility
     * @param pViewEntity struct edict_s * - Entity
     * @param pClient struct edict_s * - Entity
     * @param pvs unsigned char ** - number[]
     * @param pas unsigned char ** - number[]
     */
    "dllSetupVisibility": (pViewEntity: Entity, pClient: Entity, pvs: number[], pas: number[]) => void;
    /**
     * Event handler for dllUpdateClientData
     * @param ent const struct edict_s * - Entity
     * @param sendweapons int - number
     * @param cd struct clientdata_s * - ClientData
     */
    "dllUpdateClientData": (ent: Entity, sendweapons: number, cd: ClientData) => void;
    /**
     * Event handler for dllAddToFullPack
     * @param state struct entity_state_s * - EntityState
     * @param e int - number
     * @param ent edict_t * - Entity
     * @param host edict_t * - Entity
     * @param hostflags int - number
     * @param player int - number
     * @param pSet unsigned char * - number[]
     */
    "dllAddToFullPack": (state: EntityState, e: number, ent: Entity, host: Entity, hostflags: number, player: number, pSet: number[]) => void;
    /**
     * Event handler for dllCreateBaseline
     * @param player int - number
     * @param eindex int - number
     * @param baseline struct entity_state_s * - EntityState
     * @param entity struct edict_s * - Entity
     * @param playermodelindex int - number
     * @param player_mins vec3_t - number[]
     * @param player_maxs vec3_t - number[]
     */
    "dllCreateBaseline": (player: number, eindex: number, baseline: EntityState, entity: Entity, playermodelindex: number, player_mins: number[], player_maxs: number[]) => void;
    /**
     * Event handler for dllRegisterEncoders
     */
    "dllRegisterEncoders": () => void;
    /**
     * Event handler for dllGetWeaponData
     * @param player struct edict_s * - Entity
     * @param info struct weapon_data_s * - WeaponData
     */
    "dllGetWeaponData": (player: Entity, info: WeaponData) => void;
    /**
     * Event handler for dllCmdStart
     * @param player const edict_t * - Entity
     * @param cmd const struct usercmd_s * - UserCmd
     * @param random_seed unsigned int - number
     */
    "dllCmdStart": (player: Entity, cmd: UserCmd, random_seed: number) => void;
    /**
     * Event handler for dllCmdEnd
     * @param player const edict_t * - Entity
     */
    "dllCmdEnd": (player: Entity) => void;
    /**
     * Event handler for dllConnectionlessPacket
     * @param net_from const struct netadr_s * - NetAdr
     * @param args const char * - string
     * @param response_buffer char * - string
     * @param response_buffer_size int * - number[]
     */
    "dllConnectionlessPacket": (net_from: NetAdr, args: string, response_buffer: string, response_buffer_size: number[]) => void;
    /**
     * Event handler for dllGetHullBounds
     * @param hullnumber int - number
     * @param mins float * - number[]
     * @param maxs float * - number[]
     */
    "dllGetHullBounds": (hullnumber: number, mins: number[], maxs: number[]) => void;
    /**
     * Event handler for dllCreateInstancedBaselines
     */
    "dllCreateInstancedBaselines": () => void;
    /**
     * Event handler for dllInconsistentFile
     * @param player const struct edict_s * - Entity
     * @param filename const char * - string
     * @param disconnect_message char * - string
     */
    "dllInconsistentFile": (player: Entity, filename: string, disconnect_message: string) => void;
    /**
     * Event handler for dllAllowLagCompensation
     */
    "dllAllowLagCompensation": () => void;
    /**
     * Event handler for postDllGameInit
     */
    "postDllGameInit": () => void;
    /**
     * Event handler for postDllSpawn
     * @param pent edict_t * - Entity
     */
    "postDllSpawn": (pent: Entity) => void;
    /**
     * Event handler for postDllThink
     * @param pent edict_t * - Entity
     */
    "postDllThink": (pent: Entity) => void;
    /**
     * Event handler for postDllUse
     * @param pentUsed edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "postDllUse": (pentUsed: Entity, pentOther: Entity) => void;
    /**
     * Event handler for postDllTouch
     * @param pentTouched edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "postDllTouch": (pentTouched: Entity, pentOther: Entity) => void;
    /**
     * Event handler for postDllBlocked
     * @param pentBlocked edict_t * - Entity
     * @param pentOther edict_t * - Entity
     */
    "postDllBlocked": (pentBlocked: Entity, pentOther: Entity) => void;
    /**
     * Event handler for postDllKeyValue
     * @param pentKeyvalue edict_t * - Entity
     * @param pkvd KeyValueData * - KeyValueData
     */
    "postDllKeyValue": (pentKeyvalue: Entity, pkvd: KeyValueData) => void;
    /**
     * Event handler for postDllSave
     * @param pent edict_t * - Entity
     * @param pSaveData SAVERESTOREDATA * - SaveRestoreData
     */
    "postDllSave": (pent: Entity, pSaveData: SaveRestoreData) => void;
    /**
     * Event handler for postDllRestore
     * @param pent edict_t * - Entity
     * @param pSaveData SAVERESTOREDATA * - SaveRestoreData
     * @param globalEntity int - number
     */
    "postDllRestore": (pent: Entity, pSaveData: SaveRestoreData, globalEntity: number) => void;
    /**
     * Event handler for postDllSetAbsBox
     * @param pent edict_t * - Entity
     */
    "postDllSetAbsBox": (pent: Entity) => void;
    /**
     * Event handler for postDllSaveWriteFields
     * @param value0 SAVERESTOREDATA* - SaveRestoreData
     * @param value1 const char* - string
     * @param value2 void* - ArrayBuffer | Uint8Array | null
     * @param value3 TYPEDESCRIPTION* - TypeDescription
     * @param value4 int - number
     */
    "postDllSaveWriteFields": (value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number) => void;
    /**
     * Event handler for postDllSaveReadFields
     * @param value0 SAVERESTOREDATA* - SaveRestoreData
     * @param value1 const char* - string
     * @param value2 void* - ArrayBuffer | Uint8Array | null
     * @param value3 TYPEDESCRIPTION* - TypeDescription
     * @param value4 int - number
     */
    "postDllSaveReadFields": (value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number) => void;
    /**
     * Event handler for postDllSaveGlobalState
     * @param value0 SAVERESTOREDATA * - SaveRestoreData
     */
    "postDllSaveGlobalState": (value0: SaveRestoreData) => void;
    /**
     * Event handler for postDllRestoreGlobalState
     * @param value0 SAVERESTOREDATA * - SaveRestoreData
     */
    "postDllRestoreGlobalState": (value0: SaveRestoreData) => void;
    /**
     * Event handler for postDllResetGlobalState
     */
    "postDllResetGlobalState": () => void;
    /**
     * Event handler for postDllClientConnect
     * @param pEntity edict_t * - Entity
     * @param pszName const char * - string
     * @param pszAddress const char * - string
     * @param szRejectReason char* - string
     */
    "postDllClientConnect": (pEntity: Entity, pszName: string, pszAddress: string, szRejectReason: string) => void;
    /**
     * Event handler for postDllClientDisconnect
     * @param pEntity edict_t * - Entity
     */
    "postDllClientDisconnect": (pEntity: Entity) => void;
    /**
     * Event handler for postDllClientKill
     * @param pEntity edict_t * - Entity
     */
    "postDllClientKill": (pEntity: Entity) => void;
    /**
     * Event handler for postDllClientPutInServer
     * @param pEntity edict_t * - Entity
     */
    "postDllClientPutInServer": (pEntity: Entity) => void;
    /**
     * Event handler for postDllClientCommand
     * @param client edict_t * - Entity
     * @param commandText string - string
     */
    "postDllClientCommand": (client: Entity, commandText: string) => void;
    /**
     * Event handler for postDllClientUserInfoChanged
     * @param pEntity edict_t * - Entity
     * @param infobuffer char * - string
     */
    "postDllClientUserInfoChanged": (pEntity: Entity, infobuffer: string) => void;
    /**
     * Event handler for postDllServerActivate
     * @param pEdictList edict_t * - Entity
     * @param edictCount int - number
     * @param clientMax int - number
     */
    "postDllServerActivate": (pEdictList: Entity, edictCount: number, clientMax: number) => void;
    /**
     * Event handler for postDllServerDeactivate
     */
    "postDllServerDeactivate": () => void;
    /**
     * Event handler for postDllPlayerPreThink
     * @param pEntity edict_t * - Entity
     */
    "postDllPlayerPreThink": (pEntity: Entity) => void;
    /**
     * Event handler for postDllPlayerPostThink
     * @param pEntity edict_t * - Entity
     */
    "postDllPlayerPostThink": (pEntity: Entity) => void;
    /**
     * Event handler for postDllStartFrame
     */
    "postDllStartFrame": () => void;
    /**
     * Event handler for postDllParmsNewLevel
     */
    "postDllParmsNewLevel": () => void;
    /**
     * Event handler for postDllParmsChangeLevel
     */
    "postDllParmsChangeLevel": () => void;
    /**
     * Event handler for postDllGetGameDescription
     */
    "postDllGetGameDescription": () => void;
    /**
     * Event handler for postDllPlayerCustomization
     * @param pEntity edict_t * - Entity
     * @param pCustom customization_t * - Customization
     */
    "postDllPlayerCustomization": (pEntity: Entity, pCustom: Customization) => void;
    /**
     * Event handler for postDllSpectatorConnect
     * @param pEntity edict_t * - Entity
     */
    "postDllSpectatorConnect": (pEntity: Entity) => void;
    /**
     * Event handler for postDllSpectatorDisconnect
     * @param pEntity edict_t * - Entity
     */
    "postDllSpectatorDisconnect": (pEntity: Entity) => void;
    /**
     * Event handler for postDllSpectatorThink
     * @param pEntity edict_t * - Entity
     */
    "postDllSpectatorThink": (pEntity: Entity) => void;
    /**
     * Event handler for postDllSysError
     * @param error_string const char * - string
     */
    "postDllSysError": (error_string: string) => void;
    /**
     * Event handler for postDllPMMove
     * @param ppmove struct playermove_s * - PlayerMove
     * @param server qboolean - boolean
     */
    "postDllPMMove": (ppmove: PlayerMove, server: boolean) => void;
    /**
     * Event handler for postDllPMInit
     * @param ppmove struct playermove_s * - PlayerMove
     */
    "postDllPMInit": (ppmove: PlayerMove) => void;
    /**
     * Event handler for postDllPMFindTextureType
     * @param name const char * - string
     */
    "postDllPMFindTextureType": (name: string) => void;
    /**
     * Event handler for postDllSetupVisibility
     * @param pViewEntity struct edict_s * - Entity
     * @param pClient struct edict_s * - Entity
     * @param pvs unsigned char ** - number[]
     * @param pas unsigned char ** - number[]
     */
    "postDllSetupVisibility": (pViewEntity: Entity, pClient: Entity, pvs: number[], pas: number[]) => void;
    /**
     * Event handler for postDllUpdateClientData
     * @param ent const struct edict_s * - Entity
     * @param sendweapons int - number
     * @param cd struct clientdata_s * - ClientData
     */
    "postDllUpdateClientData": (ent: Entity, sendweapons: number, cd: ClientData) => void;
    /**
     * Event handler for postDllAddToFullPack
     * @param state struct entity_state_s * - EntityState
     * @param e int - number
     * @param ent edict_t * - Entity
     * @param host edict_t * - Entity
     * @param hostflags int - number
     * @param player int - number
     * @param pSet unsigned char * - number[]
     */
    "postDllAddToFullPack": (state: EntityState, e: number, ent: Entity, host: Entity, hostflags: number, player: number, pSet: number[]) => void;
    /**
     * Event handler for postDllCreateBaseline
     * @param player int - number
     * @param eindex int - number
     * @param baseline struct entity_state_s * - EntityState
     * @param entity struct edict_s * - Entity
     * @param playermodelindex int - number
     * @param player_mins vec3_t - number[]
     * @param player_maxs vec3_t - number[]
     */
    "postDllCreateBaseline": (player: number, eindex: number, baseline: EntityState, entity: Entity, playermodelindex: number, player_mins: number[], player_maxs: number[]) => void;
    /**
     * Event handler for postDllRegisterEncoders
     */
    "postDllRegisterEncoders": () => void;
    /**
     * Event handler for postDllGetWeaponData
     * @param player struct edict_s * - Entity
     * @param info struct weapon_data_s * - WeaponData
     */
    "postDllGetWeaponData": (player: Entity, info: WeaponData) => void;
    /**
     * Event handler for postDllCmdStart
     * @param player const edict_t * - Entity
     * @param cmd const struct usercmd_s * - UserCmd
     * @param random_seed unsigned int - number
     */
    "postDllCmdStart": (player: Entity, cmd: UserCmd, random_seed: number) => void;
    /**
     * Event handler for postDllCmdEnd
     * @param player const edict_t * - Entity
     */
    "postDllCmdEnd": (player: Entity) => void;
    /**
     * Event handler for postDllConnectionlessPacket
     * @param net_from const struct netadr_s * - NetAdr
     * @param args const char * - string
     * @param response_buffer char * - string
     * @param response_buffer_size int * - number[]
     */
    "postDllConnectionlessPacket": (net_from: NetAdr, args: string, response_buffer: string, response_buffer_size: number[]) => void;
    /**
     * Event handler for postDllGetHullBounds
     * @param hullnumber int - number
     * @param mins float * - number[]
     * @param maxs float * - number[]
     */
    "postDllGetHullBounds": (hullnumber: number, mins: number[], maxs: number[]) => void;
    /**
     * Event handler for postDllCreateInstancedBaselines
     */
    "postDllCreateInstancedBaselines": () => void;
    /**
     * Event handler for postDllInconsistentFile
     * @param player const struct edict_s * - Entity
     * @param filename const char * - string
     * @param disconnect_message char * - string
     */
    "postDllInconsistentFile": (player: Entity, filename: string, disconnect_message: string) => void;
    /**
     * Event handler for postDllAllowLagCompensation
     */
    "postDllAllowLagCompensation": () => void;
    /**
     * Event handler for engPrecacheModel
     * @param s const char * - string
     */
    "engPrecacheModel": (s: string) => void;
    /**
     * Event handler for engPrecacheSound
     * @param s const char * - string
     */
    "engPrecacheSound": (s: string) => void;
    /**
     * Event handler for engSetModel
     * @param e edict_t * - Entity
     * @param m const char * - string
     */
    "engSetModel": (e: Entity, m: string) => void;
    /**
     * Event handler for engModelIndex
     * @param m const char * - string
     */
    "engModelIndex": (m: string) => void;
    /**
     * Event handler for engModelFrames
     * @param modelIndex int - number
     */
    "engModelFrames": (modelIndex: number) => void;
    /**
     * Event handler for engSetSize
     * @param e edict_t * - Entity
     * @param rgflMin const float * - number[]
     * @param rgflMax const float * - number[]
     */
    "engSetSize": (e: Entity, rgflMin: number[], rgflMax: number[]) => void;
    /**
     * Event handler for engChangeLevel
     * @param s1 const char * - string
     * @param s2 const char * - string
     */
    "engChangeLevel": (s1: string, s2: string) => void;
    /**
     * Event handler for engGetSpawnParms
     * @param ent edict_t * - Entity
     */
    "engGetSpawnParms": (ent: Entity) => void;
    /**
     * Event handler for engSaveSpawnParms
     * @param ent edict_t * - Entity
     */
    "engSaveSpawnParms": (ent: Entity) => void;
    /**
     * Event handler for engVecToYaw
     * @param rgflVector const float * - number[]
     */
    "engVecToYaw": (rgflVector: number[]) => void;
    /**
     * Event handler for engVecToAngles
     * @param rgflVectorIn const float * - number[]
     * @param rgflVectorOut float * - number[]
     */
    "engVecToAngles": (rgflVectorIn: number[], rgflVectorOut: number[]) => void;
    /**
     * Event handler for engMoveToOrigin
     * @param ent edict_t * - Entity
     * @param pflGoal const float * - number[]
     * @param dist float - number
     * @param iMoveType int - number
     */
    "engMoveToOrigin": (ent: Entity, pflGoal: number[], dist: number, iMoveType: number) => void;
    /**
     * Event handler for engChangeYaw
     * @param ent edict_t* - Entity
     */
    "engChangeYaw": (ent: Entity) => void;
    /**
     * Event handler for engChangePitch
     * @param ent edict_t* - Entity
     */
    "engChangePitch": (ent: Entity) => void;
    /**
     * Event handler for engFindEntityByString
     * @param pEdictStartSearchAfter edict_t * - Entity
     * @param pszField const char * - string
     * @param pszValue const char * - string
     */
    "engFindEntityByString": (pEdictStartSearchAfter: Entity, pszField: string, pszValue: string) => void;
    /**
     * Event handler for engGetEntityIllum
     * @param pEnt edict_t* - Entity
     */
    "engGetEntityIllum": (pEnt: Entity) => void;
    /**
     * Event handler for engFindEntityInSphere
     * @param pEdictStartSearchAfter edict_t * - Entity
     * @param org const float * - number[]
     * @param rad float - number
     */
    "engFindEntityInSphere": (pEdictStartSearchAfter: Entity, org: number[], rad: number) => void;
    /**
     * Event handler for engFindClientInPVS
     * @param pEdict edict_t * - Entity
     */
    "engFindClientInPVS": (pEdict: Entity) => void;
    /**
     * Event handler for engEntitiesInPVS
     * @param pplayer edict_t * - Entity
     */
    "engEntitiesInPVS": (pplayer: Entity) => void;
    /**
     * Event handler for engMakeVectors
     * @param rgflVector const float * - number[]
     */
    "engMakeVectors": (rgflVector: number[]) => void;
    /**
     * Event handler for engAngleVectors
     * @param rgflVector const float * - number[]
     * @param forward float * - number[]
     * @param right float * - number[]
     * @param up float * - number[]
     */
    "engAngleVectors": (rgflVector: number[], forward: number[], right: number[], up: number[]) => void;
    /**
     * Event handler for engCreateEntity
     */
    "engCreateEntity": () => void;
    /**
     * Event handler for engRemoveEntity
     * @param e edict_t* - Entity
     */
    "engRemoveEntity": (e: Entity) => void;
    /**
     * Event handler for engCreateNamedEntity
     * @param className int - number
     */
    "engCreateNamedEntity": (className: number) => void;
    /**
     * Event handler for engMakeStatic
     * @param ent edict_t * - Entity
     */
    "engMakeStatic": (ent: Entity) => void;
    /**
     * Event handler for engEntIsOnFloor
     * @param e edict_t * - Entity
     */
    "engEntIsOnFloor": (e: Entity) => void;
    /**
     * Event handler for engDropToFloor
     * @param e edict_t* - Entity
     */
    "engDropToFloor": (e: Entity) => void;
    /**
     * Event handler for engWalkMove
     * @param ent edict_t * - Entity
     * @param yaw float - number
     * @param dist float - number
     * @param iMode int - number
     */
    "engWalkMove": (ent: Entity, yaw: number, dist: number, iMode: number) => void;
    /**
     * Event handler for engSetOrigin
     * @param e edict_t * - Entity
     * @param rgflOrigin const float * - number[]
     */
    "engSetOrigin": (e: Entity, rgflOrigin: number[]) => void;
    /**
     * Event handler for engEmitSound
     * @param entity edict_t * - Entity
     * @param channel int - number
     * @param sample const char * - string
     * @param volume float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     */
    "engEmitSound": (entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number) => void;
    /**
     * Event handler for engEmitAmbientSound
     * @param entity edict_t * - Entity
     * @param pos const float * - number[]
     * @param samp const char * - string
     * @param vol float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     */
    "engEmitAmbientSound": (entity: Entity, pos: number[], samp: string, vol: number, attenuation: number, fFlags: number, pitch: number) => void;
    /**
     * Event handler for engTraceLine
     * @param start const float * - number[]
     * @param end const float * - number[]
     * @param flags int - number
     * @param skipEntity edict_t * - Entity | null
     */
    "engTraceLine": (start: number[], end: number[], flags: number, skipEntity: Entity | null) => void;
    /**
     * Event handler for engTraceToss
     * @param pent edict_t * - Entity
     * @param pentToIgnore edict_t * - Entity
     */
    "engTraceToss": (pent: Entity, pentToIgnore: Entity) => void;
    /**
     * Event handler for engTraceMonsterHull
     * @param pEdict edict_t * - Entity
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param pentToSkip edict_t * - Entity
     */
    "engTraceMonsterHull": (pEdict: Entity, v1: number[], v2: number[], fNoMonsters: number, pentToSkip: Entity) => void;
    /**
     * Event handler for engTraceHull
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param hullNumber int - number
     * @param pentToSkip edict_t * - Entity
     */
    "engTraceHull": (v1: number[], v2: number[], fNoMonsters: number, hullNumber: number, pentToSkip: Entity) => void;
    /**
     * Event handler for engTraceModel
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param hullNumber int - number
     * @param pent edict_t * - Entity
     */
    "engTraceModel": (v1: number[], v2: number[], hullNumber: number, pent: Entity) => void;
    /**
     * Event handler for engTraceTexture
     * @param pTextureEntity edict_t * - Entity
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     */
    "engTraceTexture": (pTextureEntity: Entity, v1: number[], v2: number[]) => void;
    /**
     * Event handler for engTraceSphere
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param radius float - number
     * @param pentToSkip edict_t * - Entity
     */
    "engTraceSphere": (v1: number[], v2: number[], fNoMonsters: number, radius: number, pentToSkip: Entity) => void;
    /**
     * Event handler for engGetAimVector
     * @param ent edict_t * - Entity
     * @param speed float - number
     * @param rgflReturn float * - number[]
     */
    "engGetAimVector": (ent: Entity, speed: number, rgflReturn: number[]) => void;
    /**
     * Event handler for engServerCommand
     * @param str const char * - string
     */
    "engServerCommand": (str: string) => void;
    /**
     * Event handler for engServerExecute
     */
    "engServerExecute": () => void;
    /**
     * Event handler for engClientCommand
     * @param entity edict_t * - Entity
     * @param commandArgs string - string
     */
    "engClientCommand": (entity: Entity, commandArgs: string) => void;
    /**
     * Event handler for engParticleEffect
     * @param org const float * - number[]
     * @param dir const float * - number[]
     * @param color float - number
     * @param count float - number
     */
    "engParticleEffect": (org: number[], dir: number[], color: number, count: number) => void;
    /**
     * Event handler for engLightStyle
     * @param style int - number
     * @param val const char * - string
     */
    "engLightStyle": (style: number, val: string) => void;
    /**
     * Event handler for engDecalIndex
     * @param name const char * - string
     */
    "engDecalIndex": (name: string) => void;
    /**
     * Event handler for engPointContents
     * @param rgflVector const float * - number[]
     */
    "engPointContents": (rgflVector: number[]) => void;
    /**
     * Event handler for engMessageBegin
     * @param msg_dest int - number
     * @param msg_type int - number
     * @param pOrigin const float * - number[]
     * @param ed edict_t * - Entity | null
     */
    "engMessageBegin": (msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity | null) => void;
    /**
     * Event handler for engMessageEnd
     */
    "engMessageEnd": () => void;
    /**
     * Event handler for engWriteByte
     * @param iValue int - number
     */
    "engWriteByte": (iValue: number) => void;
    /**
     * Event handler for engWriteChar
     * @param iValue int - number
     */
    "engWriteChar": (iValue: number) => void;
    /**
     * Event handler for engWriteShort
     * @param iValue int - number
     */
    "engWriteShort": (iValue: number) => void;
    /**
     * Event handler for engWriteLong
     * @param iValue int - number
     */
    "engWriteLong": (iValue: number) => void;
    /**
     * Event handler for engWriteAngle
     * @param flValue float - number
     */
    "engWriteAngle": (flValue: number) => void;
    /**
     * Event handler for engWriteCoord
     * @param flValue float - number
     */
    "engWriteCoord": (flValue: number) => void;
    /**
     * Event handler for engWriteString
     * @param sz const char * - string
     */
    "engWriteString": (sz: string) => void;
    /**
     * Event handler for engWriteEntity
     * @param iValue int - number
     */
    "engWriteEntity": (iValue: number) => void;
    /**
     * Event handler for engCVarRegister
     * @param cvar cvar_t * - Cvar
     */
    "engCVarRegister": (cvar: Cvar) => void;
    /**
     * Event handler for engCVarGetFloat
     * @param szVarName const char * - string
     */
    "engCVarGetFloat": (szVarName: string) => void;
    /**
     * Event handler for engCVarGetString
     * @param szVarName const char * - string
     */
    "engCVarGetString": (szVarName: string) => void;
    /**
     * Event handler for engCVarSetFloat
     * @param szVarName const char * - string
     * @param flValue float - number
     */
    "engCVarSetFloat": (szVarName: string, flValue: number) => void;
    /**
     * Event handler for engCVarSetString
     * @param szVarName const char * - string
     * @param szValue const char * - string
     */
    "engCVarSetString": (szVarName: string, szValue: string) => void;
    /**
     * Event handler for engAlertMessage
     * @param atype ALERT_TYPE - number
     * @param szFmt const char * - string
     */
    "engAlertMessage": (atype: number, szFmt: string, ...args: any[]) => void;
    /**
     * Event handler for engEngineFprintf
     * @param pfile FILE * - FileHandle
     * @param szFmt const char * - string
     */
    "engEngineFprintf": (pfile: FileHandle, szFmt: string, ...args: any[]) => void;
    /**
     * Event handler for engPvAllocEntPrivateData
     * @param pEdict edict_t * - Entity
     * @param cb int - number
     */
    "engPvAllocEntPrivateData": (pEdict: Entity, cb: number) => void;
    /**
     * Event handler for engPvEntPrivateData
     * @param pEdict edict_t * - Entity
     */
    "engPvEntPrivateData": (pEdict: Entity) => void;
    /**
     * Event handler for engFreeEntPrivateData
     * @param pEdict edict_t * - Entity
     */
    "engFreeEntPrivateData": (pEdict: Entity) => void;
    /**
     * Event handler for engSzFromIndex
     * @param iString int - number
     */
    "engSzFromIndex": (iString: number) => void;
    /**
     * Event handler for engAllocString
     * @param szValue const char * - string
     */
    "engAllocString": (szValue: string) => void;
    /**
     * Event handler for engGetVarsOfEnt
     * @param pEdict edict_t * - Entity
     */
    "engGetVarsOfEnt": (pEdict: Entity) => void;
    /**
     * Event handler for engPEntityOfEntOffset
     * @param iEntOffset int - number
     */
    "engPEntityOfEntOffset": (iEntOffset: number) => void;
    /**
     * Event handler for engEntOffsetOfPEntity
     * @param pEdict const edict_t * - Entity
     */
    "engEntOffsetOfPEntity": (pEdict: Entity) => void;
    /**
     * Event handler for engIndexOfEdict
     * @param pEdict const edict_t * - Entity
     */
    "engIndexOfEdict": (pEdict: Entity) => void;
    /**
     * Event handler for engPEntityOfEntIndex
     * @param iEntIndex int - number
     */
    "engPEntityOfEntIndex": (iEntIndex: number) => void;
    /**
     * Event handler for engFindEntityByVars
     * @param pvars struct entvars_s* - Entvars
     */
    "engFindEntityByVars": (pvars: Entvars) => void;
    /**
     * Event handler for engGetModelPtr
     * @param pEdict edict_t* - Entity
     */
    "engGetModelPtr": (pEdict: Entity) => void;
    /**
     * Event handler for engRegUserMsg
     * @param pszName const char * - string
     * @param iSize int - number
     */
    "engRegUserMsg": (pszName: string, iSize: number) => void;
    /**
     * Event handler for engAnimationAutomove
     * @param pEdict const edict_t* - Entity
     * @param flTime float - number
     */
    "engAnimationAutomove": (pEdict: Entity, flTime: number) => void;
    /**
     * Event handler for engGetBonePosition
     * @param pEdict const edict_t* - Entity
     * @param iBone int - number
     * @param rgflOrigin float * - number[]
     * @param rgflAngles float * - number[]
     */
    "engGetBonePosition": (pEdict: Entity, iBone: number, rgflOrigin: number[], rgflAngles: number[]) => void;
    /**
     * Event handler for engFunctionFromName
     * @param pName const char * - string
     */
    "engFunctionFromName": (pName: string) => void;
    /**
     * Event handler for engNameForFunction
     * @param callback void * - ArrayBuffer | Uint8Array | null
     */
    "engNameForFunction": (callback: ArrayBuffer | Uint8Array | null) => void;
    /**
     * Event handler for engClientPrintf
     * @param pEdict edict_t* - Entity
     * @param ptype PRINT_TYPE - number
     * @param szMsg const char * - string
     */
    "engClientPrintf": (pEdict: Entity, ptype: number, szMsg: string) => void;
    /**
     * Event handler for engServerPrint
     * @param szMsg const char * - string
     */
    "engServerPrint": (szMsg: string) => void;
    /**
     * Event handler for engCmdArgs
     */
    "engCmdArgs": () => void;
    /**
     * Event handler for engCmdArgv
     * @param argc int - number
     */
    "engCmdArgv": (argc: number) => void;
    /**
     * Event handler for engCmdArgc
     */
    "engCmdArgc": () => void;
    /**
     * Event handler for engGetAttachment
     * @param pEdict const edict_t * - Entity
     * @param iAttachment int - number
     * @param rgflOrigin float * - number[]
     * @param rgflAngles float * - number[]
     */
    "engGetAttachment": (pEdict: Entity, iAttachment: number, rgflOrigin: number[], rgflAngles: number[]) => void;
    /**
     * Event handler for engRandomLong
     * @param lLow int - number
     * @param lHigh int - number
     */
    "engRandomLong": (lLow: number, lHigh: number) => void;
    /**
     * Event handler for engRandomFloat
     * @param flLow float - number
     * @param flHigh float - number
     */
    "engRandomFloat": (flLow: number, flHigh: number) => void;
    /**
     * Event handler for engSetView
     * @param pClient const edict_t * - Entity
     * @param pViewent const edict_t * - Entity
     */
    "engSetView": (pClient: Entity, pViewent: Entity) => void;
    /**
     * Event handler for engTime
     */
    "engTime": () => void;
    /**
     * Event handler for engCrosshairAngle
     * @param pClient const edict_t * - Entity
     * @param pitch float - number
     * @param yaw float - number
     */
    "engCrosshairAngle": (pClient: Entity, pitch: number, yaw: number) => void;
    /**
     * Event handler for engLoadFileForMe
     * @param filename const char * - string
     */
    "engLoadFileForMe": (filename: string) => void;
    /**
     * Event handler for engFreeFile
     * @param buffer void * - ArrayBuffer | Uint8Array | null
     */
    "engFreeFile": (buffer: ArrayBuffer | Uint8Array | null) => void;
    /**
     * Event handler for engEndSection
     * @param pszSectionName const char * - string
     */
    "engEndSection": (pszSectionName: string) => void;
    /**
     * Event handler for engCompareFileTime
     * @param filename1 char * - string
     * @param filename2 char * - string
     * @param iCompare int * - number[]
     */
    "engCompareFileTime": (filename1: string, filename2: string, iCompare: number[]) => void;
    /**
     * Event handler for engGetGameDir
     * @param szGetGameDir char * - string
     */
    "engGetGameDir": (szGetGameDir: string) => void;
    /**
     * Event handler for engCvarRegisterVariable
     * @param variable cvar_t * - Cvar
     */
    "engCvarRegisterVariable": (variable: Cvar) => void;
    /**
     * Event handler for engFadeClientVolume
     * @param pEdict const edict_t * - Entity
     * @param fadePercent int - number
     * @param fadeOutSeconds int - number
     * @param holdTime int - number
     * @param fadeInSeconds int - number
     */
    "engFadeClientVolume": (pEdict: Entity, fadePercent: number, fadeOutSeconds: number, holdTime: number, fadeInSeconds: number) => void;
    /**
     * Event handler for engSetClientMaxspeed
     * @param pEdict const edict_t * - Entity
     * @param fNewMaxspeed float - number
     */
    "engSetClientMaxspeed": (pEdict: Entity, fNewMaxspeed: number) => void;
    /**
     * Event handler for engCreateFakeClient
     * @param netname const char * - string
     */
    "engCreateFakeClient": (netname: string) => void;
    /**
     * Event handler for engRunPlayerMove
     * @param fakeclient edict_t * - Entity
     * @param viewangles const float * - number[]
     * @param forwardmove float - number
     * @param sidemove float - number
     * @param upmove float - number
     * @param buttons unsigned short - number
     * @param impulse byte - number
     * @param msec byte - number
     */
    "engRunPlayerMove": (fakeclient: Entity, viewangles: number[], forwardmove: number, sidemove: number, upmove: number, buttons: number, impulse: number, msec: number) => void;
    /**
     * Event handler for engNumberOfEntities
     */
    "engNumberOfEntities": () => void;
    /**
     * Event handler for engGetInfoKeyBuffer
     * @param e edict_t * - Entity
     */
    "engGetInfoKeyBuffer": (e: Entity) => void;
    /**
     * Event handler for engInfoKeyValue
     * @param infobuffer char * - string
     * @param key const char * - string
     */
    "engInfoKeyValue": (infobuffer: string, key: string) => void;
    /**
     * Event handler for engSetKeyValue
     * @param infobuffer char * - string
     * @param key const char * - string
     * @param value const char * - string
     */
    "engSetKeyValue": (infobuffer: string, key: string, value: string) => void;
    /**
     * Event handler for engSetClientKeyValue
     * @param clientIndex int - number
     * @param infobuffer char * - string
     * @param key const char * - string
     * @param value const char * - string
     */
    "engSetClientKeyValue": (clientIndex: number, infobuffer: string, key: string, value: string) => void;
    /**
     * Event handler for engIsMapValid
     * @param filename const char * - string
     */
    "engIsMapValid": (filename: string) => void;
    /**
     * Event handler for engStaticDecal
     * @param origin const float * - number[]
     * @param decalIndex int - number
     * @param entityIndex int - number
     * @param modelIndex int - number
     */
    "engStaticDecal": (origin: number[], decalIndex: number, entityIndex: number, modelIndex: number) => void;
    /**
     * Event handler for engPrecacheGeneric
     * @param s const char * - string
     */
    "engPrecacheGeneric": (s: string) => void;
    /**
     * Event handler for engGetPlayerUserId
     * @param e edict_t * - Entity
     */
    "engGetPlayerUserId": (e: Entity) => void;
    /**
     * Event handler for engBuildSoundMsg
     * @param entity edict_t * - Entity
     * @param channel int - number
     * @param sample const char * - string
     * @param volume float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     * @param msg_dest int - number
     * @param msg_type int - number
     * @param pOrigin const float * - number[]
     * @param ed edict_t * - Entity
     */
    "engBuildSoundMsg": (entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number, msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity) => void;
    /**
     * Event handler for engIsDedicatedServer
     */
    "engIsDedicatedServer": () => void;
    /**
     * Event handler for engCVarGetPointer
     * @param szVarName const char * - string
     */
    "engCVarGetPointer": (szVarName: string) => void;
    /**
     * Event handler for engGetPlayerWONId
     * @param e edict_t * - Entity
     */
    "engGetPlayerWONId": (e: Entity) => void;
    /**
     * Event handler for engInfoRemoveKey
     * @param s char * - string
     * @param key const char * - string
     */
    "engInfoRemoveKey": (s: string, key: string) => void;
    /**
     * Event handler for engGetPhysicsKeyValue
     * @param pClient const edict_t * - Entity
     * @param key const char * - string
     */
    "engGetPhysicsKeyValue": (pClient: Entity, key: string) => void;
    /**
     * Event handler for engSetPhysicsKeyValue
     * @param pClient const edict_t * - Entity
     * @param key const char * - string
     * @param value const char * - string
     */
    "engSetPhysicsKeyValue": (pClient: Entity, key: string, value: string) => void;
    /**
     * Event handler for engGetPhysicsInfoString
     * @param pClient const edict_t * - Entity
     */
    "engGetPhysicsInfoString": (pClient: Entity) => void;
    /**
     * Event handler for engPrecacheEvent
     * @param type int - number
     * @param psz const char* - string
     */
    "engPrecacheEvent": (type: number, psz: string) => void;
    /**
     * Event handler for engPlaybackEvent
     * @param flags int - number
     * @param pInvoker const edict_t * - Entity
     * @param eventindex unsigned short - number
     * @param delay float - number
     * @param origin const float * - number[]
     * @param angles const float * - number[]
     * @param fparam1 float - number
     * @param fparam2 float - number
     * @param iparam1 int - number
     * @param iparam2 int - number
     * @param bparam1 int - number
     * @param bparam2 int - number
     */
    "engPlaybackEvent": (flags: number, pInvoker: Entity, eventindex: number, delay: number, origin: number[], angles: number[], fparam1: number, fparam2: number, iparam1: number, iparam2: number, bparam1: number, bparam2: number) => void;
    /**
     * Event handler for engSetFatPVS
     * @param org const float * - number[]
     */
    "engSetFatPVS": (org: number[]) => void;
    /**
     * Event handler for engSetFatPAS
     * @param org const float * - number[]
     */
    "engSetFatPAS": (org: number[]) => void;
    /**
     * Event handler for engCheckVisibility
     * @param entity const edict_t * - Entity
     * @param pset unsigned char * - number[]
     */
    "engCheckVisibility": (entity: Entity, pset: number[]) => void;
    /**
     * Event handler for engDeltaSetField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "engDeltaSetField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for engDeltaUnsetField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "engDeltaUnsetField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for engDeltaAddEncoder
     * @param encoderName const char * - string
     * @param callback void (*)(struct delta_s *, const unsigned char *, const unsigned char *) - (pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void
     */
    "engDeltaAddEncoder": (encoderName: string, callback: (pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void) => void;
    /**
     * Event handler for engGetCurrentPlayer
     */
    "engGetCurrentPlayer": () => void;
    /**
     * Event handler for engCanSkipPlayer
     * @param player const edict_t * - Entity
     */
    "engCanSkipPlayer": (player: Entity) => void;
    /**
     * Event handler for engDeltaFindField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "engDeltaFindField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for engDeltaSetFieldByIndex
     * @param pFields struct delta_s * - Delta
     * @param fieldNumber int - number
     */
    "engDeltaSetFieldByIndex": (pFields: Delta, fieldNumber: number) => void;
    /**
     * Event handler for engDeltaUnsetFieldByIndex
     * @param pFields struct delta_s * - Delta
     * @param fieldNumber int - number
     */
    "engDeltaUnsetFieldByIndex": (pFields: Delta, fieldNumber: number) => void;
    /**
     * Event handler for engSetGroupMask
     * @param mask int - number
     * @param op int - number
     */
    "engSetGroupMask": (mask: number, op: number) => void;
    /**
     * Event handler for engCreateInstancedBaseline
     * @param classname int - number
     * @param baseline struct entity_state_s * - EntityState
     */
    "engCreateInstancedBaseline": (classname: number, baseline: EntityState) => void;
    /**
     * Event handler for engCvarDirectSet
     * @param variable struct cvar_s * - Cvar
     * @param value const char * - string
     */
    "engCvarDirectSet": (variable: Cvar, value: string) => void;
    /**
     * Event handler for engForceUnmodified
     * @param type FORCE_TYPE - number
     * @param mins const float * - number[]
     * @param maxs const float * - number[]
     * @param filename const char * - string
     */
    "engForceUnmodified": (type: number, mins: number[], maxs: number[], filename: string) => void;
    /**
     * Event handler for engGetPlayerStats
     * @param pClient const edict_t * - Entity
     * @param ping int * - number[]
     * @param packet_loss int * - number[]
     */
    "engGetPlayerStats": (pClient: Entity, ping: number[], packet_loss: number[]) => void;
    /**
     * Event handler for engAddServerCommand
     * @param commandName const char * - string
     * @param callback void (*)(void) - () => void
     */
    "engAddServerCommand": (commandName: string, callback: () => void) => void;
    /**
     * Event handler for engVoiceGetClientListening
     * @param iReceiver int - number
     * @param iSender int - number
     */
    "engVoiceGetClientListening": (iReceiver: number, iSender: number) => void;
    /**
     * Event handler for engVoiceSetClientListening
     * @param iReceiver int - number
     * @param iSender int - number
     * @param bListen qboolean - boolean
     */
    "engVoiceSetClientListening": (iReceiver: number, iSender: number, bListen: boolean) => void;
    /**
     * Event handler for engGetPlayerAuthId
     * @param e edict_t * - Entity
     */
    "engGetPlayerAuthId": (e: Entity) => void;
    /**
     * Event handler for engSequenceGet
     * @param fileName const char * - string
     * @param entryName const char * - string
     */
    "engSequenceGet": (fileName: string, entryName: string) => void;
    /**
     * Event handler for engSequencePickSentence
     * @param groupName const char * - string
     * @param pickMethod int - number
     * @param picked int * - number[]
     */
    "engSequencePickSentence": (groupName: string, pickMethod: number, picked: number[]) => void;
    /**
     * Event handler for engGetFileSize
     * @param filename const char * - string
     */
    "engGetFileSize": (filename: string) => void;
    /**
     * Event handler for engGetApproxWavePlayLen
     * @param filepath const char * - string
     */
    "engGetApproxWavePlayLen": (filepath: string) => void;
    /**
     * Event handler for engIsCareerMatch
     */
    "engIsCareerMatch": () => void;
    /**
     * Event handler for engGetLocalizedStringLength
     * @param label const char * - string
     */
    "engGetLocalizedStringLength": (label: string) => void;
    /**
     * Event handler for engRegisterTutorMessageShown
     * @param mid int - number
     */
    "engRegisterTutorMessageShown": (mid: number) => void;
    /**
     * Event handler for engGetTimesTutorMessageShown
     * @param mid int - number
     */
    "engGetTimesTutorMessageShown": (mid: number) => void;
    /**
     * Event handler for engProcessTutorMessageDecayBuffer
     * @param buffer int * - number[]
     */
    "engProcessTutorMessageDecayBuffer": (buffer: number[]) => void;
    /**
     * Event handler for engConstructTutorMessageDecayBuffer
     * @param buffer int * - number[]
     */
    "engConstructTutorMessageDecayBuffer": (buffer: number[]) => void;
    /**
     * Event handler for engResetTutorMessageDecayData
     */
    "engResetTutorMessageDecayData": () => void;
    /**
     * Event handler for engQueryClientCvarValue
     * @param player const edict_t * - Entity
     * @param cvarName const char * - string
     */
    "engQueryClientCvarValue": (player: Entity, cvarName: string) => void;
    /**
     * Event handler for engQueryClientCvarValue2
     * @param player const edict_t * - Entity
     * @param cvarName const char * - string
     * @param requestID int - number
     */
    "engQueryClientCvarValue2": (player: Entity, cvarName: string, requestID: number) => void;
    /**
     * Event handler for engCheckParm
     * @param parm char * - string
     * @param ppnext char ** - string[]
     */
    "engCheckParm": (parm: string, ppnext: string[]) => void;
    /**
     * Event handler for engPEntityOfEntIndexAllEntities
     * @param iEntIndex int - number
     */
    "engPEntityOfEntIndexAllEntities": (iEntIndex: number) => void;
    /**
     * Event handler for postEngPrecacheModel
     * @param s const char * - string
     */
    "postEngPrecacheModel": (s: string) => void;
    /**
     * Event handler for postEngPrecacheSound
     * @param s const char * - string
     */
    "postEngPrecacheSound": (s: string) => void;
    /**
     * Event handler for postEngSetModel
     * @param e edict_t * - Entity
     * @param m const char * - string
     */
    "postEngSetModel": (e: Entity, m: string) => void;
    /**
     * Event handler for postEngModelIndex
     * @param m const char * - string
     */
    "postEngModelIndex": (m: string) => void;
    /**
     * Event handler for postEngModelFrames
     * @param modelIndex int - number
     */
    "postEngModelFrames": (modelIndex: number) => void;
    /**
     * Event handler for postEngSetSize
     * @param e edict_t * - Entity
     * @param rgflMin const float * - number[]
     * @param rgflMax const float * - number[]
     */
    "postEngSetSize": (e: Entity, rgflMin: number[], rgflMax: number[]) => void;
    /**
     * Event handler for postEngChangeLevel
     * @param s1 const char * - string
     * @param s2 const char * - string
     */
    "postEngChangeLevel": (s1: string, s2: string) => void;
    /**
     * Event handler for postEngGetSpawnParms
     * @param ent edict_t * - Entity
     */
    "postEngGetSpawnParms": (ent: Entity) => void;
    /**
     * Event handler for postEngSaveSpawnParms
     * @param ent edict_t * - Entity
     */
    "postEngSaveSpawnParms": (ent: Entity) => void;
    /**
     * Event handler for postEngVecToYaw
     * @param rgflVector const float * - number[]
     */
    "postEngVecToYaw": (rgflVector: number[]) => void;
    /**
     * Event handler for postEngVecToAngles
     * @param rgflVectorIn const float * - number[]
     * @param rgflVectorOut float * - number[]
     */
    "postEngVecToAngles": (rgflVectorIn: number[], rgflVectorOut: number[]) => void;
    /**
     * Event handler for postEngMoveToOrigin
     * @param ent edict_t * - Entity
     * @param pflGoal const float * - number[]
     * @param dist float - number
     * @param iMoveType int - number
     */
    "postEngMoveToOrigin": (ent: Entity, pflGoal: number[], dist: number, iMoveType: number) => void;
    /**
     * Event handler for postEngChangeYaw
     * @param ent edict_t* - Entity
     */
    "postEngChangeYaw": (ent: Entity) => void;
    /**
     * Event handler for postEngChangePitch
     * @param ent edict_t* - Entity
     */
    "postEngChangePitch": (ent: Entity) => void;
    /**
     * Event handler for postEngFindEntityByString
     * @param pEdictStartSearchAfter edict_t * - Entity
     * @param pszField const char * - string
     * @param pszValue const char * - string
     */
    "postEngFindEntityByString": (pEdictStartSearchAfter: Entity, pszField: string, pszValue: string) => void;
    /**
     * Event handler for postEngGetEntityIllum
     * @param pEnt edict_t* - Entity
     */
    "postEngGetEntityIllum": (pEnt: Entity) => void;
    /**
     * Event handler for postEngFindEntityInSphere
     * @param pEdictStartSearchAfter edict_t * - Entity
     * @param org const float * - number[]
     * @param rad float - number
     */
    "postEngFindEntityInSphere": (pEdictStartSearchAfter: Entity, org: number[], rad: number) => void;
    /**
     * Event handler for postEngFindClientInPVS
     * @param pEdict edict_t * - Entity
     */
    "postEngFindClientInPVS": (pEdict: Entity) => void;
    /**
     * Event handler for postEngEntitiesInPVS
     * @param pplayer edict_t * - Entity
     */
    "postEngEntitiesInPVS": (pplayer: Entity) => void;
    /**
     * Event handler for postEngMakeVectors
     * @param rgflVector const float * - number[]
     */
    "postEngMakeVectors": (rgflVector: number[]) => void;
    /**
     * Event handler for postEngAngleVectors
     * @param rgflVector const float * - number[]
     * @param forward float * - number[]
     * @param right float * - number[]
     * @param up float * - number[]
     */
    "postEngAngleVectors": (rgflVector: number[], forward: number[], right: number[], up: number[]) => void;
    /**
     * Event handler for postEngCreateEntity
     */
    "postEngCreateEntity": () => void;
    /**
     * Event handler for postEngRemoveEntity
     * @param e edict_t* - Entity
     */
    "postEngRemoveEntity": (e: Entity) => void;
    /**
     * Event handler for postEngCreateNamedEntity
     * @param className int - number
     */
    "postEngCreateNamedEntity": (className: number) => void;
    /**
     * Event handler for postEngMakeStatic
     * @param ent edict_t * - Entity
     */
    "postEngMakeStatic": (ent: Entity) => void;
    /**
     * Event handler for postEngEntIsOnFloor
     * @param e edict_t * - Entity
     */
    "postEngEntIsOnFloor": (e: Entity) => void;
    /**
     * Event handler for postEngDropToFloor
     * @param e edict_t* - Entity
     */
    "postEngDropToFloor": (e: Entity) => void;
    /**
     * Event handler for postEngWalkMove
     * @param ent edict_t * - Entity
     * @param yaw float - number
     * @param dist float - number
     * @param iMode int - number
     */
    "postEngWalkMove": (ent: Entity, yaw: number, dist: number, iMode: number) => void;
    /**
     * Event handler for postEngSetOrigin
     * @param e edict_t * - Entity
     * @param rgflOrigin const float * - number[]
     */
    "postEngSetOrigin": (e: Entity, rgflOrigin: number[]) => void;
    /**
     * Event handler for postEngEmitSound
     * @param entity edict_t * - Entity
     * @param channel int - number
     * @param sample const char * - string
     * @param volume float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     */
    "postEngEmitSound": (entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number) => void;
    /**
     * Event handler for postEngEmitAmbientSound
     * @param entity edict_t * - Entity
     * @param pos const float * - number[]
     * @param samp const char * - string
     * @param vol float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     */
    "postEngEmitAmbientSound": (entity: Entity, pos: number[], samp: string, vol: number, attenuation: number, fFlags: number, pitch: number) => void;
    /**
     * Event handler for postEngTraceLine
     * @param start const float * - number[]
     * @param end const float * - number[]
     * @param flags int - number
     * @param skipEntity edict_t * - Entity | null
     */
    "postEngTraceLine": (start: number[], end: number[], flags: number, skipEntity: Entity | null) => void;
    /**
     * Event handler for postEngTraceToss
     * @param pent edict_t * - Entity
     * @param pentToIgnore edict_t * - Entity
     */
    "postEngTraceToss": (pent: Entity, pentToIgnore: Entity) => void;
    /**
     * Event handler for postEngTraceMonsterHull
     * @param pEdict edict_t * - Entity
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param pentToSkip edict_t * - Entity
     */
    "postEngTraceMonsterHull": (pEdict: Entity, v1: number[], v2: number[], fNoMonsters: number, pentToSkip: Entity) => void;
    /**
     * Event handler for postEngTraceHull
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param hullNumber int - number
     * @param pentToSkip edict_t * - Entity
     */
    "postEngTraceHull": (v1: number[], v2: number[], fNoMonsters: number, hullNumber: number, pentToSkip: Entity) => void;
    /**
     * Event handler for postEngTraceModel
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param hullNumber int - number
     * @param pent edict_t * - Entity
     */
    "postEngTraceModel": (v1: number[], v2: number[], hullNumber: number, pent: Entity) => void;
    /**
     * Event handler for postEngTraceTexture
     * @param pTextureEntity edict_t * - Entity
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     */
    "postEngTraceTexture": (pTextureEntity: Entity, v1: number[], v2: number[]) => void;
    /**
     * Event handler for postEngTraceSphere
     * @param v1 const float * - number[]
     * @param v2 const float * - number[]
     * @param fNoMonsters int - number
     * @param radius float - number
     * @param pentToSkip edict_t * - Entity
     */
    "postEngTraceSphere": (v1: number[], v2: number[], fNoMonsters: number, radius: number, pentToSkip: Entity) => void;
    /**
     * Event handler for postEngGetAimVector
     * @param ent edict_t * - Entity
     * @param speed float - number
     * @param rgflReturn float * - number[]
     */
    "postEngGetAimVector": (ent: Entity, speed: number, rgflReturn: number[]) => void;
    /**
     * Event handler for postEngServerCommand
     * @param str const char * - string
     */
    "postEngServerCommand": (str: string) => void;
    /**
     * Event handler for postEngServerExecute
     */
    "postEngServerExecute": () => void;
    /**
     * Event handler for postEngClientCommand
     * @param entity edict_t * - Entity
     * @param commandArgs string - string
     */
    "postEngClientCommand": (entity: Entity, commandArgs: string) => void;
    /**
     * Event handler for postEngParticleEffect
     * @param org const float * - number[]
     * @param dir const float * - number[]
     * @param color float - number
     * @param count float - number
     */
    "postEngParticleEffect": (org: number[], dir: number[], color: number, count: number) => void;
    /**
     * Event handler for postEngLightStyle
     * @param style int - number
     * @param val const char * - string
     */
    "postEngLightStyle": (style: number, val: string) => void;
    /**
     * Event handler for postEngDecalIndex
     * @param name const char * - string
     */
    "postEngDecalIndex": (name: string) => void;
    /**
     * Event handler for postEngPointContents
     * @param rgflVector const float * - number[]
     */
    "postEngPointContents": (rgflVector: number[]) => void;
    /**
     * Event handler for postEngMessageBegin
     * @param msg_dest int - number
     * @param msg_type int - number
     * @param pOrigin const float * - number[]
     * @param ed edict_t * - Entity | null
     */
    "postEngMessageBegin": (msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity | null) => void;
    /**
     * Event handler for postEngMessageEnd
     */
    "postEngMessageEnd": () => void;
    /**
     * Event handler for postEngWriteByte
     * @param iValue int - number
     */
    "postEngWriteByte": (iValue: number) => void;
    /**
     * Event handler for postEngWriteChar
     * @param iValue int - number
     */
    "postEngWriteChar": (iValue: number) => void;
    /**
     * Event handler for postEngWriteShort
     * @param iValue int - number
     */
    "postEngWriteShort": (iValue: number) => void;
    /**
     * Event handler for postEngWriteLong
     * @param iValue int - number
     */
    "postEngWriteLong": (iValue: number) => void;
    /**
     * Event handler for postEngWriteAngle
     * @param flValue float - number
     */
    "postEngWriteAngle": (flValue: number) => void;
    /**
     * Event handler for postEngWriteCoord
     * @param flValue float - number
     */
    "postEngWriteCoord": (flValue: number) => void;
    /**
     * Event handler for postEngWriteString
     * @param sz const char * - string
     */
    "postEngWriteString": (sz: string) => void;
    /**
     * Event handler for postEngWriteEntity
     * @param iValue int - number
     */
    "postEngWriteEntity": (iValue: number) => void;
    /**
     * Event handler for postEngCVarRegister
     * @param cvar cvar_t * - Cvar
     */
    "postEngCVarRegister": (cvar: Cvar) => void;
    /**
     * Event handler for postEngCVarGetFloat
     * @param szVarName const char * - string
     */
    "postEngCVarGetFloat": (szVarName: string) => void;
    /**
     * Event handler for postEngCVarGetString
     * @param szVarName const char * - string
     */
    "postEngCVarGetString": (szVarName: string) => void;
    /**
     * Event handler for postEngCVarSetFloat
     * @param szVarName const char * - string
     * @param flValue float - number
     */
    "postEngCVarSetFloat": (szVarName: string, flValue: number) => void;
    /**
     * Event handler for postEngCVarSetString
     * @param szVarName const char * - string
     * @param szValue const char * - string
     */
    "postEngCVarSetString": (szVarName: string, szValue: string) => void;
    /**
     * Event handler for postEngAlertMessage
     * @param atype ALERT_TYPE - number
     * @param szFmt const char * - string
     */
    "postEngAlertMessage": (atype: number, szFmt: string, ...args: any[]) => void;
    /**
     * Event handler for postEngEngineFprintf
     * @param pfile FILE * - FileHandle
     * @param szFmt const char * - string
     */
    "postEngEngineFprintf": (pfile: FileHandle, szFmt: string, ...args: any[]) => void;
    /**
     * Event handler for postEngPvAllocEntPrivateData
     * @param pEdict edict_t * - Entity
     * @param cb int - number
     */
    "postEngPvAllocEntPrivateData": (pEdict: Entity, cb: number) => void;
    /**
     * Event handler for postEngPvEntPrivateData
     * @param pEdict edict_t * - Entity
     */
    "postEngPvEntPrivateData": (pEdict: Entity) => void;
    /**
     * Event handler for postEngFreeEntPrivateData
     * @param pEdict edict_t * - Entity
     */
    "postEngFreeEntPrivateData": (pEdict: Entity) => void;
    /**
     * Event handler for postEngSzFromIndex
     * @param iString int - number
     */
    "postEngSzFromIndex": (iString: number) => void;
    /**
     * Event handler for postEngAllocString
     * @param szValue const char * - string
     */
    "postEngAllocString": (szValue: string) => void;
    /**
     * Event handler for postEngGetVarsOfEnt
     * @param pEdict edict_t * - Entity
     */
    "postEngGetVarsOfEnt": (pEdict: Entity) => void;
    /**
     * Event handler for postEngPEntityOfEntOffset
     * @param iEntOffset int - number
     */
    "postEngPEntityOfEntOffset": (iEntOffset: number) => void;
    /**
     * Event handler for postEngEntOffsetOfPEntity
     * @param pEdict const edict_t * - Entity
     */
    "postEngEntOffsetOfPEntity": (pEdict: Entity) => void;
    /**
     * Event handler for postEngIndexOfEdict
     * @param pEdict const edict_t * - Entity
     */
    "postEngIndexOfEdict": (pEdict: Entity) => void;
    /**
     * Event handler for postEngPEntityOfEntIndex
     * @param iEntIndex int - number
     */
    "postEngPEntityOfEntIndex": (iEntIndex: number) => void;
    /**
     * Event handler for postEngFindEntityByVars
     * @param pvars struct entvars_s* - Entvars
     */
    "postEngFindEntityByVars": (pvars: Entvars) => void;
    /**
     * Event handler for postEngGetModelPtr
     * @param pEdict edict_t* - Entity
     */
    "postEngGetModelPtr": (pEdict: Entity) => void;
    /**
     * Event handler for postEngRegUserMsg
     * @param pszName const char * - string
     * @param iSize int - number
     */
    "postEngRegUserMsg": (pszName: string, iSize: number) => void;
    /**
     * Event handler for postEngAnimationAutomove
     * @param pEdict const edict_t* - Entity
     * @param flTime float - number
     */
    "postEngAnimationAutomove": (pEdict: Entity, flTime: number) => void;
    /**
     * Event handler for postEngGetBonePosition
     * @param pEdict const edict_t* - Entity
     * @param iBone int - number
     * @param rgflOrigin float * - number[]
     * @param rgflAngles float * - number[]
     */
    "postEngGetBonePosition": (pEdict: Entity, iBone: number, rgflOrigin: number[], rgflAngles: number[]) => void;
    /**
     * Event handler for postEngFunctionFromName
     * @param pName const char * - string
     */
    "postEngFunctionFromName": (pName: string) => void;
    /**
     * Event handler for postEngNameForFunction
     * @param callback void * - ArrayBuffer | Uint8Array | null
     */
    "postEngNameForFunction": (callback: ArrayBuffer | Uint8Array | null) => void;
    /**
     * Event handler for postEngClientPrintf
     * @param pEdict edict_t* - Entity
     * @param ptype PRINT_TYPE - number
     * @param szMsg const char * - string
     */
    "postEngClientPrintf": (pEdict: Entity, ptype: number, szMsg: string) => void;
    /**
     * Event handler for postEngServerPrint
     * @param szMsg const char * - string
     */
    "postEngServerPrint": (szMsg: string) => void;
    /**
     * Event handler for postEngCmdArgs
     */
    "postEngCmdArgs": () => void;
    /**
     * Event handler for postEngCmdArgv
     * @param argc int - number
     */
    "postEngCmdArgv": (argc: number) => void;
    /**
     * Event handler for postEngCmdArgc
     */
    "postEngCmdArgc": () => void;
    /**
     * Event handler for postEngGetAttachment
     * @param pEdict const edict_t * - Entity
     * @param iAttachment int - number
     * @param rgflOrigin float * - number[]
     * @param rgflAngles float * - number[]
     */
    "postEngGetAttachment": (pEdict: Entity, iAttachment: number, rgflOrigin: number[], rgflAngles: number[]) => void;
    /**
     * Event handler for postEngRandomLong
     * @param lLow int - number
     * @param lHigh int - number
     */
    "postEngRandomLong": (lLow: number, lHigh: number) => void;
    /**
     * Event handler for postEngRandomFloat
     * @param flLow float - number
     * @param flHigh float - number
     */
    "postEngRandomFloat": (flLow: number, flHigh: number) => void;
    /**
     * Event handler for postEngSetView
     * @param pClient const edict_t * - Entity
     * @param pViewent const edict_t * - Entity
     */
    "postEngSetView": (pClient: Entity, pViewent: Entity) => void;
    /**
     * Event handler for postEngTime
     */
    "postEngTime": () => void;
    /**
     * Event handler for postEngCrosshairAngle
     * @param pClient const edict_t * - Entity
     * @param pitch float - number
     * @param yaw float - number
     */
    "postEngCrosshairAngle": (pClient: Entity, pitch: number, yaw: number) => void;
    /**
     * Event handler for postEngLoadFileForMe
     * @param filename const char * - string
     */
    "postEngLoadFileForMe": (filename: string) => void;
    /**
     * Event handler for postEngFreeFile
     * @param buffer void * - ArrayBuffer | Uint8Array | null
     */
    "postEngFreeFile": (buffer: ArrayBuffer | Uint8Array | null) => void;
    /**
     * Event handler for postEngEndSection
     * @param pszSectionName const char * - string
     */
    "postEngEndSection": (pszSectionName: string) => void;
    /**
     * Event handler for postEngCompareFileTime
     * @param filename1 char * - string
     * @param filename2 char * - string
     * @param iCompare int * - number[]
     */
    "postEngCompareFileTime": (filename1: string, filename2: string, iCompare: number[]) => void;
    /**
     * Event handler for postEngGetGameDir
     * @param szGetGameDir char * - string
     */
    "postEngGetGameDir": (szGetGameDir: string) => void;
    /**
     * Event handler for postEngCvarRegisterVariable
     * @param variable cvar_t * - Cvar
     */
    "postEngCvarRegisterVariable": (variable: Cvar) => void;
    /**
     * Event handler for postEngFadeClientVolume
     * @param pEdict const edict_t * - Entity
     * @param fadePercent int - number
     * @param fadeOutSeconds int - number
     * @param holdTime int - number
     * @param fadeInSeconds int - number
     */
    "postEngFadeClientVolume": (pEdict: Entity, fadePercent: number, fadeOutSeconds: number, holdTime: number, fadeInSeconds: number) => void;
    /**
     * Event handler for postEngSetClientMaxspeed
     * @param pEdict const edict_t * - Entity
     * @param fNewMaxspeed float - number
     */
    "postEngSetClientMaxspeed": (pEdict: Entity, fNewMaxspeed: number) => void;
    /**
     * Event handler for postEngCreateFakeClient
     * @param netname const char * - string
     */
    "postEngCreateFakeClient": (netname: string) => void;
    /**
     * Event handler for postEngRunPlayerMove
     * @param fakeclient edict_t * - Entity
     * @param viewangles const float * - number[]
     * @param forwardmove float - number
     * @param sidemove float - number
     * @param upmove float - number
     * @param buttons unsigned short - number
     * @param impulse byte - number
     * @param msec byte - number
     */
    "postEngRunPlayerMove": (fakeclient: Entity, viewangles: number[], forwardmove: number, sidemove: number, upmove: number, buttons: number, impulse: number, msec: number) => void;
    /**
     * Event handler for postEngNumberOfEntities
     */
    "postEngNumberOfEntities": () => void;
    /**
     * Event handler for postEngGetInfoKeyBuffer
     * @param e edict_t * - Entity
     */
    "postEngGetInfoKeyBuffer": (e: Entity) => void;
    /**
     * Event handler for postEngInfoKeyValue
     * @param infobuffer char * - string
     * @param key const char * - string
     */
    "postEngInfoKeyValue": (infobuffer: string, key: string) => void;
    /**
     * Event handler for postEngSetKeyValue
     * @param infobuffer char * - string
     * @param key const char * - string
     * @param value const char * - string
     */
    "postEngSetKeyValue": (infobuffer: string, key: string, value: string) => void;
    /**
     * Event handler for postEngSetClientKeyValue
     * @param clientIndex int - number
     * @param infobuffer char * - string
     * @param key const char * - string
     * @param value const char * - string
     */
    "postEngSetClientKeyValue": (clientIndex: number, infobuffer: string, key: string, value: string) => void;
    /**
     * Event handler for postEngIsMapValid
     * @param filename const char * - string
     */
    "postEngIsMapValid": (filename: string) => void;
    /**
     * Event handler for postEngStaticDecal
     * @param origin const float * - number[]
     * @param decalIndex int - number
     * @param entityIndex int - number
     * @param modelIndex int - number
     */
    "postEngStaticDecal": (origin: number[], decalIndex: number, entityIndex: number, modelIndex: number) => void;
    /**
     * Event handler for postEngPrecacheGeneric
     * @param s const char * - string
     */
    "postEngPrecacheGeneric": (s: string) => void;
    /**
     * Event handler for postEngGetPlayerUserId
     * @param e edict_t * - Entity
     */
    "postEngGetPlayerUserId": (e: Entity) => void;
    /**
     * Event handler for postEngBuildSoundMsg
     * @param entity edict_t * - Entity
     * @param channel int - number
     * @param sample const char * - string
     * @param volume float - number
     * @param attenuation float - number
     * @param fFlags int - number
     * @param pitch int - number
     * @param msg_dest int - number
     * @param msg_type int - number
     * @param pOrigin const float * - number[]
     * @param ed edict_t * - Entity
     */
    "postEngBuildSoundMsg": (entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number, msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity) => void;
    /**
     * Event handler for postEngIsDedicatedServer
     */
    "postEngIsDedicatedServer": () => void;
    /**
     * Event handler for postEngCVarGetPointer
     * @param szVarName const char * - string
     */
    "postEngCVarGetPointer": (szVarName: string) => void;
    /**
     * Event handler for postEngGetPlayerWONId
     * @param e edict_t * - Entity
     */
    "postEngGetPlayerWONId": (e: Entity) => void;
    /**
     * Event handler for postEngInfoRemoveKey
     * @param s char * - string
     * @param key const char * - string
     */
    "postEngInfoRemoveKey": (s: string, key: string) => void;
    /**
     * Event handler for postEngGetPhysicsKeyValue
     * @param pClient const edict_t * - Entity
     * @param key const char * - string
     */
    "postEngGetPhysicsKeyValue": (pClient: Entity, key: string) => void;
    /**
     * Event handler for postEngSetPhysicsKeyValue
     * @param pClient const edict_t * - Entity
     * @param key const char * - string
     * @param value const char * - string
     */
    "postEngSetPhysicsKeyValue": (pClient: Entity, key: string, value: string) => void;
    /**
     * Event handler for postEngGetPhysicsInfoString
     * @param pClient const edict_t * - Entity
     */
    "postEngGetPhysicsInfoString": (pClient: Entity) => void;
    /**
     * Event handler for postEngPrecacheEvent
     * @param type int - number
     * @param psz const char* - string
     */
    "postEngPrecacheEvent": (type: number, psz: string) => void;
    /**
     * Event handler for postEngPlaybackEvent
     * @param flags int - number
     * @param pInvoker const edict_t * - Entity
     * @param eventindex unsigned short - number
     * @param delay float - number
     * @param origin const float * - number[]
     * @param angles const float * - number[]
     * @param fparam1 float - number
     * @param fparam2 float - number
     * @param iparam1 int - number
     * @param iparam2 int - number
     * @param bparam1 int - number
     * @param bparam2 int - number
     */
    "postEngPlaybackEvent": (flags: number, pInvoker: Entity, eventindex: number, delay: number, origin: number[], angles: number[], fparam1: number, fparam2: number, iparam1: number, iparam2: number, bparam1: number, bparam2: number) => void;
    /**
     * Event handler for postEngSetFatPVS
     * @param org const float * - number[]
     */
    "postEngSetFatPVS": (org: number[]) => void;
    /**
     * Event handler for postEngSetFatPAS
     * @param org const float * - number[]
     */
    "postEngSetFatPAS": (org: number[]) => void;
    /**
     * Event handler for postEngCheckVisibility
     * @param entity const edict_t * - Entity
     * @param pset unsigned char * - number[]
     */
    "postEngCheckVisibility": (entity: Entity, pset: number[]) => void;
    /**
     * Event handler for postEngDeltaSetField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "postEngDeltaSetField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for postEngDeltaUnsetField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "postEngDeltaUnsetField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for postEngDeltaAddEncoder
     * @param encoderName const char * - string
     * @param callback void (*)(struct delta_s *, const unsigned char *, const unsigned char *) - (pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void
     */
    "postEngDeltaAddEncoder": (encoderName: string, callback: (pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void) => void;
    /**
     * Event handler for postEngGetCurrentPlayer
     */
    "postEngGetCurrentPlayer": () => void;
    /**
     * Event handler for postEngCanSkipPlayer
     * @param player const edict_t * - Entity
     */
    "postEngCanSkipPlayer": (player: Entity) => void;
    /**
     * Event handler for postEngDeltaFindField
     * @param pFields struct delta_s * - Delta
     * @param fieldname const char * - string
     */
    "postEngDeltaFindField": (pFields: Delta, fieldname: string) => void;
    /**
     * Event handler for postEngDeltaSetFieldByIndex
     * @param pFields struct delta_s * - Delta
     * @param fieldNumber int - number
     */
    "postEngDeltaSetFieldByIndex": (pFields: Delta, fieldNumber: number) => void;
    /**
     * Event handler for postEngDeltaUnsetFieldByIndex
     * @param pFields struct delta_s * - Delta
     * @param fieldNumber int - number
     */
    "postEngDeltaUnsetFieldByIndex": (pFields: Delta, fieldNumber: number) => void;
    /**
     * Event handler for postEngSetGroupMask
     * @param mask int - number
     * @param op int - number
     */
    "postEngSetGroupMask": (mask: number, op: number) => void;
    /**
     * Event handler for postEngCreateInstancedBaseline
     * @param classname int - number
     * @param baseline struct entity_state_s * - EntityState
     */
    "postEngCreateInstancedBaseline": (classname: number, baseline: EntityState) => void;
    /**
     * Event handler for postEngCvarDirectSet
     * @param variable struct cvar_s * - Cvar
     * @param value const char * - string
     */
    "postEngCvarDirectSet": (variable: Cvar, value: string) => void;
    /**
     * Event handler for postEngForceUnmodified
     * @param type FORCE_TYPE - number
     * @param mins const float * - number[]
     * @param maxs const float * - number[]
     * @param filename const char * - string
     */
    "postEngForceUnmodified": (type: number, mins: number[], maxs: number[], filename: string) => void;
    /**
     * Event handler for postEngGetPlayerStats
     * @param pClient const edict_t * - Entity
     * @param ping int * - number[]
     * @param packet_loss int * - number[]
     */
    "postEngGetPlayerStats": (pClient: Entity, ping: number[], packet_loss: number[]) => void;
    /**
     * Event handler for postEngAddServerCommand
     * @param commandName const char * - string
     * @param callback void (*)(void) - () => void
     */
    "postEngAddServerCommand": (commandName: string, callback: () => void) => void;
    /**
     * Event handler for postEngVoiceGetClientListening
     * @param iReceiver int - number
     * @param iSender int - number
     */
    "postEngVoiceGetClientListening": (iReceiver: number, iSender: number) => void;
    /**
     * Event handler for postEngVoiceSetClientListening
     * @param iReceiver int - number
     * @param iSender int - number
     * @param bListen qboolean - boolean
     */
    "postEngVoiceSetClientListening": (iReceiver: number, iSender: number, bListen: boolean) => void;
    /**
     * Event handler for postEngGetPlayerAuthId
     * @param e edict_t * - Entity
     */
    "postEngGetPlayerAuthId": (e: Entity) => void;
    /**
     * Event handler for postEngSequenceGet
     * @param fileName const char * - string
     * @param entryName const char * - string
     */
    "postEngSequenceGet": (fileName: string, entryName: string) => void;
    /**
     * Event handler for postEngSequencePickSentence
     * @param groupName const char * - string
     * @param pickMethod int - number
     * @param picked int * - number[]
     */
    "postEngSequencePickSentence": (groupName: string, pickMethod: number, picked: number[]) => void;
    /**
     * Event handler for postEngGetFileSize
     * @param filename const char * - string
     */
    "postEngGetFileSize": (filename: string) => void;
    /**
     * Event handler for postEngGetApproxWavePlayLen
     * @param filepath const char * - string
     */
    "postEngGetApproxWavePlayLen": (filepath: string) => void;
    /**
     * Event handler for postEngIsCareerMatch
     */
    "postEngIsCareerMatch": () => void;
    /**
     * Event handler for postEngGetLocalizedStringLength
     * @param label const char * - string
     */
    "postEngGetLocalizedStringLength": (label: string) => void;
    /**
     * Event handler for postEngRegisterTutorMessageShown
     * @param mid int - number
     */
    "postEngRegisterTutorMessageShown": (mid: number) => void;
    /**
     * Event handler for postEngGetTimesTutorMessageShown
     * @param mid int - number
     */
    "postEngGetTimesTutorMessageShown": (mid: number) => void;
    /**
     * Event handler for postEngProcessTutorMessageDecayBuffer
     * @param buffer int * - number[]
     */
    "postEngProcessTutorMessageDecayBuffer": (buffer: number[]) => void;
    /**
     * Event handler for postEngConstructTutorMessageDecayBuffer
     * @param buffer int * - number[]
     */
    "postEngConstructTutorMessageDecayBuffer": (buffer: number[]) => void;
    /**
     * Event handler for postEngResetTutorMessageDecayData
     */
    "postEngResetTutorMessageDecayData": () => void;
    /**
     * Event handler for postEngQueryClientCvarValue
     * @param player const edict_t * - Entity
     * @param cvarName const char * - string
     */
    "postEngQueryClientCvarValue": (player: Entity, cvarName: string) => void;
    /**
     * Event handler for postEngQueryClientCvarValue2
     * @param player const edict_t * - Entity
     * @param cvarName const char * - string
     * @param requestID int - number
     */
    "postEngQueryClientCvarValue2": (player: Entity, cvarName: string, requestID: number) => void;
    /**
     * Event handler for postEngCheckParm
     * @param parm char * - string
     * @param ppnext char ** - string[]
     */
    "postEngCheckParm": (parm: string, ppnext: string[]) => void;
    /**
     * Event handler for postEngPEntityOfEntIndexAllEntities
     * @param iEntIndex int - number
     */
    "postEngPEntityOfEntIndexAllEntities": (iEntIndex: number) => void;
  }
}