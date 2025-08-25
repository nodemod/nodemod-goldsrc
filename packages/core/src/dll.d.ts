// This file is generated automatically. Don't edit it.
/// <reference path="./structures.d.ts" />
/// <reference path="./enums.d.ts" />

declare namespace nodemod {
  interface DLL {
    /** void	(*pfnGameInit)( void ); */
    gameInit(): void;
    /** int	(*pfnSpawn)( edict_t *pent ); */
    spawn(pent: Entity): number;
    /** void	(*pfnThink)( edict_t *pent ); */
    think(pent: Entity): void;
    /** void	(*pfnUse)( edict_t *pentUsed, edict_t *pentOther ); */
    use(pentUsed: Entity, pentOther: Entity): void;
    /** void	(*pfnTouch)( edict_t *pentTouched, edict_t *pentOther ); */
    touch(pentTouched: Entity, pentOther: Entity): void;
    /** void	(*pfnBlocked)( edict_t *pentBlocked, edict_t *pentOther ); */
    blocked(pentBlocked: Entity, pentOther: Entity): void;
    /** void	(*pfnKeyValue)( edict_t *pentKeyvalue, KeyValueData *pkvd ); */
    keyValue(pentKeyvalue: Entity, pkvd: KeyValueData): void;
    /** void	(*pfnSave)( edict_t *pent, SAVERESTOREDATA *pSaveData ); */
    save(pent: Entity, pSaveData: SaveRestoreData): void;
    /** int 	(*pfnRestore)( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity ); */
    restore(pent: Entity, pSaveData: SaveRestoreData, globalEntity: number): number;
    /** void	(*pfnSetAbsBox)( edict_t *pent ); */
    setAbsBox(pent: Entity): void;
    /** void	(*pfnSaveWriteFields)( SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int ); */
    saveWriteFields(value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number): void;
    /** void	(*pfnSaveReadFields)( SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int ); */
    saveReadFields(value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number): void;
    /** void	(*pfnSaveGlobalState)( SAVERESTOREDATA * ); */
    saveGlobalState(value0: SaveRestoreData): void;
    /** void	(*pfnRestoreGlobalState)( SAVERESTOREDATA * ); */
    restoreGlobalState(value0: SaveRestoreData): void;
    /** void	(*pfnResetGlobalState)( void ); */
    resetGlobalState(): void;
    /** qboolean	(*pfnClientConnect)( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128] ); */
    clientConnect(pEntity: Entity, pszName: string, pszAddress: string, szRejectReason: string): boolean;
    /** void	(*pfnClientDisconnect)( edict_t *pEntity ); */
    clientDisconnect(pEntity: Entity): void;
    /** void	(*pfnClientKill)( edict_t *pEntity ); */
    clientKill(pEntity: Entity): void;
    /** void	(*pfnClientPutInServer)( edict_t *pEntity ); */
    clientPutInServer(pEntity: Entity): void;
    /** void	(*pfnClientCommand)( edict_t *pEntity ); */
    clientCommand(pEntity: Entity): void;
    /** void	(*pfnClientUserInfoChanged)( edict_t *pEntity, char *infobuffer ); */
    clientUserInfoChanged(pEntity: Entity, infobuffer: string): void;
    /** void	(*pfnServerActivate)( edict_t *pEdictList, int edictCount, int clientMax ); */
    serverActivate(pEdictList: Entity, edictCount: number, clientMax: number): void;
    /** void	(*pfnServerDeactivate)( void ); */
    serverDeactivate(): void;
    /** void	(*pfnPlayerPreThink)( edict_t *pEntity ); */
    playerPreThink(pEntity: Entity): void;
    /** void	(*pfnPlayerPostThink)( edict_t *pEntity ); */
    playerPostThink(pEntity: Entity): void;
    /** void	(*pfnStartFrame)( void ); */
    startFrame(): void;
    /** void	(*pfnParmsNewLevel)( void ); */
    parmsNewLevel(): void;
    /** void	(*pfnParmsChangeLevel)( void ); */
    parmsChangeLevel(): void;
    /** const char     *(*pfnGetGameDescription)( void ); */
    getGameDescription(): string;
    /** void	(*pfnPlayerCustomization)( edict_t *pEntity, customization_t *pCustom ); */
    playerCustomization(pEntity: Entity, pCustom: Customization): void;
    /** void	(*pfnSpectatorConnect)( edict_t *pEntity ); */
    spectatorConnect(pEntity: Entity): void;
    /** void	(*pfnSpectatorDisconnect)( edict_t *pEntity ); */
    spectatorDisconnect(pEntity: Entity): void;
    /** void	(*pfnSpectatorThink)( edict_t *pEntity ); */
    spectatorThink(pEntity: Entity): void;
    /** void	(*pfnSys_Error)( const char *error_string ); */
    sysError(error_string: string): void;
    /** void	(*pfnPM_Move)( struct playermove_s *ppmove, qboolean server ); */
    pMMove(ppmove: PlayerMove, server: boolean): void;
    /** void	(*pfnPM_Init)( struct playermove_s *ppmove ); */
    pMInit(ppmove: PlayerMove): void;
    /** char	(*pfnPM_FindTextureType)( const char *name ); */
    pMFindTextureType(name: string): number;
    /** void	(*pfnSetupVisibility)( struct edict_s *pViewEntity, struct edict_s *pClient, unsigned char **pvs, unsigned char **pas ); */
    setupVisibility(pViewEntity: Entity, pClient: Entity, pvs: number[], pas: number[]): void;
    /** void	(*pfnUpdateClientData) ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd ); */
    updateClientData(ent: Entity, sendweapons: number, cd: ClientData): void;
    /** int	(*pfnAddToFullPack)( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet ); */
    addToFullPack(state: EntityState, e: number, ent: Entity, host: Entity, hostflags: number, player: number, pSet: number[]): number;
    /** void	(*pfnCreateBaseline)( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs ); */
    createBaseline(player: number, eindex: number, baseline: EntityState, entity: Entity, playermodelindex: number, player_mins: number[], player_maxs: number[]): void;
    /** void	(*pfnRegisterEncoders)( void ); */
    registerEncoders(): void;
    /** int	(*pfnGetWeaponData)( struct edict_s *player, struct weapon_data_s *info ); */
    getWeaponData(player: Entity, info: WeaponData): number;
    /** void	(*pfnCmdStart)( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed ); */
    cmdStart(player: Entity, cmd: UserCmd, random_seed: number): void;
    /** void	(*pfnCmdEnd)( const edict_t *player ); */
    cmdEnd(player: Entity): void;
    /** int	(*pfnConnectionlessPacket )( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size ); */
    connectionlessPacket(net_from: NetAdr, args: string, response_buffer: string, response_buffer_size: number[]): number;
    /** int	(*pfnGetHullBounds)	( int hullnumber, float *mins, float *maxs ); */
    getHullBounds(hullnumber: number, mins: number[], maxs: number[]): number;
    /** void	(*pfnCreateInstancedBaselines) ( void ); */
    createInstancedBaselines(): void;
    /** int	(*pfnInconsistentFile)( const struct edict_s *player, const char *filename, char *disconnect_message ); */
    inconsistentFile(player: Entity, filename: string, disconnect_message: string): number;
    /** int	(*pfnAllowLagCompensation)( void ); */
    allowLagCompensation(): number;
  }
  const dll: DLL;
}