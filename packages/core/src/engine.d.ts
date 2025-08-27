// This file is generated automatically. Don't edit it.
/// <reference path="./structures.d.ts" />
/// <reference path="./enums.d.ts" />

declare namespace nodemod {
  interface Engine {
    /** int	(*pfnPrecacheModel)( const char *s ); */
    precacheModel(s: string): number;
    /** int	(*pfnPrecacheSound)( const char *s ); */
    precacheSound(s: string): number;
    /** void	(*pfnSetModel)( edict_t *e, const char *m ); */
    setModel(e: Entity, m: string): void;
    /** int	(*pfnModelIndex)( const char *m ); */
    modelIndex(m: string): number;
    /** int	(*pfnModelFrames)( int modelIndex ); */
    modelFrames(modelIndex: number): number;
    /** void	(*pfnSetSize)( edict_t *e, const float *rgflMin, const float *rgflMax ); */
    setSize(e: Entity, rgflMin: number[], rgflMax: number[]): void;
    /** void	(*pfnChangeLevel)( const char *s1, const char *s2 ); */
    changeLevel(s1: string, s2: string): void;
    /** void	(*pfnGetSpawnParms)( edict_t *ent ); */
    getSpawnParms(ent: Entity): void;
    /** void	(*pfnSaveSpawnParms)( edict_t *ent ); */
    saveSpawnParms(ent: Entity): void;
    /** float	(*pfnVecToYaw)( const float *rgflVector ); */
    vecToYaw(rgflVector: number[]): number;
    /** void	(*pfnVecToAngles)( const float *rgflVectorIn, float *rgflVectorOut ); */
    vecToAngles(rgflVectorIn: number[], rgflVectorOut: number[]): void;
    /** void	(*pfnMoveToOrigin)( edict_t *ent, const float *pflGoal, float dist, int iMoveType ); */
    moveToOrigin(ent: Entity, pflGoal: number[], dist: number, iMoveType: number): void;
    /** void	(*pfnChangeYaw)( edict_t* ent ); */
    changeYaw(ent: Entity): void;
    /** void	(*pfnChangePitch)( edict_t* ent ); */
    changePitch(ent: Entity): void;
    /** edict_t*	(*pfnFindEntityByString)( edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue ); */
    findEntityByString(pEdictStartSearchAfter: Entity, pszField: string, pszValue: string): Entity;
    /** int	(*pfnGetEntityIllum)( edict_t* pEnt ); */
    getEntityIllum(pEnt: Entity): number;
    /** edict_t*	(*pfnFindEntityInSphere)( edict_t *pEdictStartSearchAfter, const float *org, float rad ); */
    findEntityInSphere(pEdictStartSearchAfter: Entity, org: number[], rad: number): Entity;
    /** edict_t*	(*pfnFindClientInPVS)( edict_t *pEdict ); */
    findClientInPVS(pEdict: Entity): Entity;
    /** edict_t*	(*pfnEntitiesInPVS)( edict_t *pplayer ); */
    entitiesInPVS(pplayer: Entity): Entity;
    /** void	(*pfnMakeVectors)( const float *rgflVector ); */
    makeVectors(rgflVector: number[]): void;
    /** void	(*pfnAngleVectors)( const float *rgflVector, float *forward, float *right, float *up ); */
    angleVectors(rgflVector: number[], forward: number[], right: number[], up: number[]): void;
    /** edict_t*	(*pfnCreateEntity)( void ); */
    createEntity(): Entity;
    /** void	(*pfnRemoveEntity)( edict_t* e ); */
    removeEntity(e: Entity): void;
    /** edict_t*	(*pfnCreateNamedEntity)( int className ); */
    createNamedEntity(className: number): Entity;
    /** void	(*pfnMakeStatic)( edict_t *ent ); */
    makeStatic(ent: Entity): void;
    /** int	(*pfnEntIsOnFloor)( edict_t *e ); */
    entIsOnFloor(e: Entity): number;
    /** int	(*pfnDropToFloor)( edict_t* e ); */
    dropToFloor(e: Entity): number;
    /** int	(*pfnWalkMove)( edict_t *ent, float yaw, float dist, int iMode ); */
    walkMove(ent: Entity, yaw: number, dist: number, iMode: number): number;
    /** void	(*pfnSetOrigin)( edict_t *e, const float *rgflOrigin ); */
    setOrigin(e: Entity, rgflOrigin: number[]): void;
    /** void	(*pfnEmitSound)( edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch ); */
    emitSound(entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number): void;
    /** void	(*pfnEmitAmbientSound)( edict_t *entity, const float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch ); */
    emitAmbientSound(entity: Entity, pos: number[], samp: string, vol: number, attenuation: number, fFlags: number, pitch: number): void;
    /** void	(*pfnTraceLine)( const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr ); */
    traceLine(start: number[], end: number[], flags: number, skipEntity: Entity | null): TraceResult;
    /** void	(*pfnTraceToss)( edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr ); */
    traceToss(pent: Entity, pentToIgnore: Entity): TraceResult;
    /** int	(*pfnTraceMonsterHull)( edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr ); */
    traceMonsterHull(pEdict: Entity, v1: number[], v2: number[], fNoMonsters: number, pentToSkip: Entity): TraceMonsterHullResult;
    /** void	(*pfnTraceHull)( const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr ); */
    traceHull(v1: number[], v2: number[], fNoMonsters: number, hullNumber: number, pentToSkip: Entity): TraceResult;
    /** void	(*pfnTraceModel)( const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr ); */
    traceModel(v1: number[], v2: number[], hullNumber: number, pent: Entity): TraceResult;
    /** const char *(*pfnTraceTexture)( edict_t *pTextureEntity, const float *v1, const float *v2 ); */
    traceTexture(pTextureEntity: Entity, v1: number[], v2: number[]): string;
    /** void	(*pfnTraceSphere)( const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr ); */
    traceSphere(v1: number[], v2: number[], fNoMonsters: number, radius: number, pentToSkip: Entity): TraceResult;
    /** void	(*pfnGetAimVector)( edict_t *ent, float speed, float *rgflReturn ); */
    getAimVector(ent: Entity, speed: number, rgflReturn: number[]): void;
    /** void	(*pfnServerCommand)( const char *str ); */
    serverCommand(str: string): void;
    /** void	(*pfnServerExecute)( void ); */
    serverExecute(): void;
    /** void	(*pfnClientCommand)( edict_t* pEdict, const char *szFmt, ... ); */
    clientCommand(entity: Entity, commandArgs: string): void;
    /** void	(*pfnParticleEffect)( const float *org, const float *dir, float color, float count ); */
    particleEffect(org: number[], dir: number[], color: number, count: number): void;
    /** void	(*pfnLightStyle)( int style, const char *val ); */
    lightStyle(style: number, val: string): void;
    /** int	(*pfnDecalIndex)( const char *name ); */
    decalIndex(name: string): number;
    /** int	(*pfnPointContents)( const float *rgflVector ); */
    pointContents(rgflVector: number[]): number;
    /** void	(*pfnMessageBegin)( int msg_dest, int msg_type, const float *pOrigin, edict_t *ed ); */
    messageBegin(msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity | null): void;
    /** void	(*pfnMessageEnd)( void ); */
    messageEnd(): void;
    /** void	(*pfnWriteByte)( int iValue ); */
    writeByte(iValue: number): void;
    /** void	(*pfnWriteChar)( int iValue ); */
    writeChar(iValue: number): void;
    /** void	(*pfnWriteShort)( int iValue ); */
    writeShort(iValue: number): void;
    /** void	(*pfnWriteLong)( int iValue ); */
    writeLong(iValue: number): void;
    /** void	(*pfnWriteAngle)( float flValue ); */
    writeAngle(flValue: number): void;
    /** void	(*pfnWriteCoord)( float flValue ); */
    writeCoord(flValue: number): void;
    /** void	(*pfnWriteString)( const char *sz ); */
    writeString(sz: string): void;
    /** void	(*pfnWriteEntity)( int iValue ); */
    writeEntity(iValue: number): void;
    /** void	(*pfnCVarRegister)( cvar_t *pCvar ); */
    cVarRegister(cvar: Cvar): void;
    /** float	(*pfnCVarGetFloat)( const char *szVarName ); */
    cVarGetFloat(szVarName: string): number;
    /** const char* (*pfnCVarGetString)( const char *szVarName ); */
    cVarGetString(szVarName: string): string;
    /** void	(*pfnCVarSetFloat)( const char *szVarName, float flValue ); */
    cVarSetFloat(szVarName: string, flValue: number): void;
    /** void	(*pfnCVarSetString)( const char *szVarName, const char *szValue ); */
    cVarSetString(szVarName: string, szValue: string): void;
    /** void	(*pfnAlertMessage)( ALERT_TYPE atype, const char *szFmt, ... ); */
    alertMessage(atype: number, szFmt: string, ...args: any[]): void;
    /** void	(*pfnEngineFprintf)( FILE *pfile, const char *szFmt, ... ); */
    engineFprintf(pfile: FileHandle, szFmt: string, ...args: any[]): void;
    /** void*	(*pfnPvAllocEntPrivateData)( edict_t *pEdict, int cb ); */
    pvAllocEntPrivateData(pEdict: Entity, cb: number): ArrayBuffer | Uint8Array | null;
    /** void*	(*pfnPvEntPrivateData)( edict_t *pEdict ); */
    pvEntPrivateData(pEdict: Entity): ArrayBuffer | Uint8Array | null;
    /** void	(*pfnFreeEntPrivateData)( edict_t *pEdict ); */
    freeEntPrivateData(pEdict: Entity): void;
    /** const char *(*pfnSzFromIndex)( int iString ); */
    szFromIndex(iString: number): string;
    /** int	(*pfnAllocString)( const char *szValue ); */
    allocString(szValue: string): number;
    /** struct entvars_s *(*pfnGetVarsOfEnt)( edict_t *pEdict ); */
    getVarsOfEnt(pEdict: Entity): Entvars;
    /** edict_t*	(*pfnPEntityOfEntOffset)( int iEntOffset ); */
    pEntityOfEntOffset(iEntOffset: number): Entity;
    /** int	(*pfnEntOffsetOfPEntity)( const edict_t *pEdict ); */
    entOffsetOfPEntity(pEdict: Entity): number;
    /** int	(*pfnIndexOfEdict)( const edict_t *pEdict ); */
    indexOfEdict(pEdict: Entity): number;
    /** edict_t*	(*pfnPEntityOfEntIndex)( int iEntIndex ); */
    pEntityOfEntIndex(iEntIndex: number): Entity;
    /** edict_t*	(*pfnFindEntityByVars)( struct entvars_s* pvars ); */
    findEntityByVars(pvars: Entvars): Entity;
    /** void*	(*pfnGetModelPtr)( edict_t* pEdict ); */
    getModelPtr(pEdict: Entity): ArrayBuffer | Uint8Array | null;
    /** int	(*pfnRegUserMsg)( const char *pszName, int iSize ); */
    regUserMsg(pszName: string, iSize: number): number;
    /** void	(*pfnAnimationAutomove)( const edict_t* pEdict, float flTime ); */
    animationAutomove(pEdict: Entity, flTime: number): void;
    /** void	(*pfnGetBonePosition)( const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles ); */
    getBonePosition(pEdict: Entity, iBone: number, rgflOrigin: number[], rgflAngles: number[]): void;
    /** void* (*pfnFunctionFromName)( const char *pName ); */
    functionFromName(pName: string): ArrayBuffer | Uint8Array | null;
    /** const char *(*pfnNameForFunction)( void *function ); */
    nameForFunction(callback: ArrayBuffer | Uint8Array | null): string;
    /** void	(*pfnClientPrintf)( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg ); // JOHN: engine callbacks so game DLL can print messages to individual clients */
    clientPrintf(pEdict: Entity, ptype: number, szMsg: string): void;
    /** void	(*pfnServerPrint)( const char *szMsg ); */
    serverPrint(szMsg: string): void;
    /** const char *(*pfnCmd_Args)( void );		// these 3 added */
    cmdArgs(): string;
    /** const char *(*pfnCmd_Argv)( int argc );		// so game DLL can easily */
    cmdArgv(argc: number): string;
    /** int	(*pfnCmd_Argc)( void );		// access client 'cmd' strings */
    cmdArgc(): number;
    /** void	(*pfnGetAttachment)( const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles ); */
    getAttachment(pEdict: Entity, iAttachment: number, rgflOrigin: number[], rgflAngles: number[]): void;
    /** int	(*pfnRandomLong)( int lLow, int lHigh ); */
    randomLong(lLow: number, lHigh: number): number;
    /** float	(*pfnRandomFloat)( float flLow, float flHigh ); */
    randomFloat(flLow: number, flHigh: number): number;
    /** void	(*pfnSetView)( const edict_t *pClient, const edict_t *pViewent ); */
    setView(pClient: Entity, pViewent: Entity): void;
    /** float	(*pfnTime)( void ); */
    time(): number;
    /** void	(*pfnCrosshairAngle)( const edict_t *pClient, float pitch, float yaw ); */
    crosshairAngle(pClient: Entity, pitch: number, yaw: number): void;
    /** byte*	(*pfnLoadFileForMe)( const char *filename, int *pLength ); */
    loadFileForMe(filename: string): number[] | null;
    /** void	(*pfnFreeFile)( void *buffer ); */
    freeFile(buffer: ArrayBuffer | Uint8Array | null): void;
    /** void	(*pfnEndSection)( const char *pszSectionName ); // trigger_endsection */
    endSection(pszSectionName: string): void;
    /** int	(*pfnCompareFileTime)( char *filename1, char *filename2, int *iCompare ); */
    compareFileTime(filename1: string, filename2: string, iCompare: number[]): number;
    /** void	(*pfnGetGameDir)( char *szGetGameDir ); */
    getGameDir(szGetGameDir: string): void;
    /** void	(*pfnCvar_RegisterVariable)( cvar_t *variable ); */
    cvarRegisterVariable(variable: Cvar): void;
    /** void	(*pfnFadeClientVolume)( const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds ); */
    fadeClientVolume(pEdict: Entity, fadePercent: number, fadeOutSeconds: number, holdTime: number, fadeInSeconds: number): void;
    /** void	(*pfnSetClientMaxspeed)( const edict_t *pEdict, float fNewMaxspeed ); */
    setClientMaxspeed(pEdict: Entity, fNewMaxspeed: number): void;
    /** edict_t	*(*pfnCreateFakeClient)( const char *netname ); // returns NULL if fake client can't be created */
    createFakeClient(netname: string): Entity;
    /** void	(*pfnRunPlayerMove)( edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec ); */
    runPlayerMove(fakeclient: Entity, viewangles: number[], forwardmove: number, sidemove: number, upmove: number, buttons: number, impulse: number, msec: number): void;
    /** int	(*pfnNumberOfEntities)( void ); */
    numberOfEntities(): number;
    /** char*	(*pfnGetInfoKeyBuffer)( edict_t *e );			// passing in NULL gets the serverinfo */
    getInfoKeyBuffer(e: Entity): string;
    /** char*	(*pfnInfoKeyValue)( char *infobuffer, const char *key ); */
    infoKeyValue(infobuffer: string, key: string): string;
    /** void	(*pfnSetKeyValue)( char *infobuffer, const char *key, const char *value ); */
    setKeyValue(infobuffer: string, key: string, value: string): void;
    /** void	(*pfnSetClientKeyValue)( int clientIndex, char *infobuffer, const char *key, const char *value ); */
    setClientKeyValue(clientIndex: number, infobuffer: string, key: string, value: string): void;
    /** int	(*pfnIsMapValid)( const char *filename ); */
    isMapValid(filename: string): number;
    /** void	(*pfnStaticDecal)( const float *origin, int decalIndex, int entityIndex, int modelIndex ); */
    staticDecal(origin: number[], decalIndex: number, entityIndex: number, modelIndex: number): void;
    /** int	(*pfnPrecacheGeneric)( const char *s ); */
    precacheGeneric(s: string): number;
    /** int	(*pfnGetPlayerUserId)( edict_t *e ); // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients */
    getPlayerUserId(e: Entity): number;
    /** void	(*pfnBuildSoundMsg)( edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed ); */
    buildSoundMsg(entity: Entity, channel: number, sample: string, volume: number, attenuation: number, fFlags: number, pitch: number, msg_dest: number, msg_type: number, pOrigin: number[], ed: Entity): void;
    /** int	(*pfnIsDedicatedServer)( void );			// is this a dedicated server? */
    isDedicatedServer(): number;
    /** cvar_t	*(*pfnCVarGetPointer)( const char *szVarName ); */
    cVarGetPointer(szVarName: string): Cvar;
    /** unsigned int (*pfnGetPlayerWONId)( edict_t *e ); // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients */
    getPlayerWONId(e: Entity): number;
    /** void	(*pfnInfo_RemoveKey)( char *s, const char *key ); */
    infoRemoveKey(s: string, key: string): void;
    /** const char *(*pfnGetPhysicsKeyValue)( const edict_t *pClient, const char *key ); */
    getPhysicsKeyValue(pClient: Entity, key: string): string;
    /** void	(*pfnSetPhysicsKeyValue)( const edict_t *pClient, const char *key, const char *value ); */
    setPhysicsKeyValue(pClient: Entity, key: string, value: string): void;
    /** const char *(*pfnGetPhysicsInfoString)( const edict_t *pClient ); */
    getPhysicsInfoString(pClient: Entity): string;
    /** unsigned short (*pfnPrecacheEvent)( int type, const char*psz ); */
    precacheEvent(type: number, psz: string): number;
    /** void	(*pfnPlaybackEvent)( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, const float *origin, const float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 ); */
    playbackEvent(flags: number, pInvoker: Entity, eventindex: number, delay: number, origin: number[], angles: number[], fparam1: number, fparam2: number, iparam1: number, iparam2: number, bparam1: number, bparam2: number): void;
    /** unsigned char *(*pfnSetFatPVS)( const float *org ); */
    setFatPVS(org: number[]): number[] | null;
    /** unsigned char *(*pfnSetFatPAS)( const float *org ); */
    setFatPAS(org: number[]): number[] | null;
    /** int	(*pfnCheckVisibility )( const edict_t *entity, unsigned char *pset ); */
    checkVisibility(entity: Entity, pset: number[]): number;
    /** void	(*pfnDeltaSetField)	( struct delta_s *pFields, const char *fieldname ); */
    deltaSetField(pFields: Delta, fieldname: string): void;
    /** void	(*pfnDeltaUnsetField)( struct delta_s *pFields, const char *fieldname ); */
    deltaUnsetField(pFields: Delta, fieldname: string): void;
    /** void	(*pfnDeltaAddEncoder)( const char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) ); */
    deltaAddEncoder(encoderName: string, callback: (pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void): void;
    /** int	(*pfnGetCurrentPlayer)( void ); */
    getCurrentPlayer(): number;
    /** int	(*pfnCanSkipPlayer)( const edict_t *player ); */
    canSkipPlayer(player: Entity): number;
    /** int	(*pfnDeltaFindField)( struct delta_s *pFields, const char *fieldname ); */
    deltaFindField(pFields: Delta, fieldname: string): number;
    /** void	(*pfnDeltaSetFieldByIndex)( struct delta_s *pFields, int fieldNumber ); */
    deltaSetFieldByIndex(pFields: Delta, fieldNumber: number): void;
    /** void	(*pfnDeltaUnsetFieldByIndex)( struct delta_s *pFields, int fieldNumber ); */
    deltaUnsetFieldByIndex(pFields: Delta, fieldNumber: number): void;
    /** void	(*pfnSetGroupMask)( int mask, int op ); */
    setGroupMask(mask: number, op: number): void;
    /** int	(*pfnCreateInstancedBaseline)( int classname, struct entity_state_s *baseline ); */
    createInstancedBaseline(classname: number, baseline: EntityState): number;
    /** void	(*pfnCvar_DirectSet)( struct cvar_s *var, const char *value ); */
    cvarDirectSet(variable: Cvar, value: string): void;
    /** void	(*pfnForceUnmodified)( FORCE_TYPE type, const float *mins, const float *maxs, const char *filename ); */
    forceUnmodified(type: number, mins: number[], maxs: number[], filename: string): void;
    /** void	(*pfnGetPlayerStats)( const edict_t *pClient, int *ping, int *packet_loss ); */
    getPlayerStats(pClient: Entity, ping: number[], packet_loss: number[]): void;
    /** void	(*pfnAddServerCommand)( const char *cmd_name, void (*function) (void) ); */
    addServerCommand(commandName: string, callback: () => void): void;
    /** qboolean	(*pfnVoice_GetClientListening)(int iReceiver, int iSender); */
    voiceGetClientListening(iReceiver: number, iSender: number): boolean;
    /** qboolean	(*pfnVoice_SetClientListening)(int iReceiver, int iSender, qboolean bListen); */
    voiceSetClientListening(iReceiver: number, iSender: number, bListen: boolean): boolean;
    /** const char *(*pfnGetPlayerAuthId)		( edict_t *e ); */
    getPlayerAuthId(e: Entity): string;
    /** void	*(*pfnSequenceGet)( const char *fileName, const char *entryName ); */
    sequenceGet(fileName: string, entryName: string): ArrayBuffer | Uint8Array | null;
    /** void	*(*pfnSequencePickSentence)( const char *groupName, int pickMethod, int *picked ); */
    sequencePickSentence(groupName: string, pickMethod: number, picked: number[]): ArrayBuffer | Uint8Array | null;
    /** int	(*pfnGetFileSize)( const char *filename ); */
    getFileSize(filename: string): number;
    /** unsigned int (*pfnGetApproxWavePlayLen)( const char *filepath ); */
    getApproxWavePlayLen(filepath: string): number;
    /** int	(*pfnIsCareerMatch)( void ); */
    isCareerMatch(): number;
    /** int	(*pfnGetLocalizedStringLength)( const char *label ); */
    getLocalizedStringLength(label: string): number;
    /** void	(*pfnRegisterTutorMessageShown)( int mid ); */
    registerTutorMessageShown(mid: number): void;
    /** int	(*pfnGetTimesTutorMessageShown)( int mid ); */
    getTimesTutorMessageShown(mid: number): number;
    /** void	(*pfnProcessTutorMessageDecayBuffer)( int *buffer, int bufferLength ); */
    processTutorMessageDecayBuffer(buffer: number[]): void;
    /** void	(*pfnConstructTutorMessageDecayBuffer)( int *buffer, int bufferLength ); */
    constructTutorMessageDecayBuffer(buffer: number[]): void;
    /** void	(*pfnResetTutorMessageDecayData)( void ); */
    resetTutorMessageDecayData(): void;
    /** void	(*pfnQueryClientCvarValue)( const edict_t *player, const char *cvarName ); */
    queryClientCvarValue(player: Entity, cvarName: string): void;
    /** void	(*pfnQueryClientCvarValue2)( const edict_t *player, const char *cvarName, int requestID ); */
    queryClientCvarValue2(player: Entity, cvarName: string, requestID: number): void;
    /** int	(*CheckParm)( char *parm, char **ppnext ); */
    checkParm(parm: string, ppnext: string[]): number;
    /** edict_t* (*pfnPEntityOfEntIndexAllEntities)( int iEntIndex ); */
    pEntityOfEntIndexAllEntities(iEntIndex: number): Entity;
  }
  const eng: Engine;
}