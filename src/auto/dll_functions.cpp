// This file is generated automatically. Don't edit it.
#include <string>
#include "v8.h"
#include "extdll.h"
#include "meta_api.h"
#include "node/nodeimpl.hpp"
#include "node/utils.hpp"

#define V8_STUFF() v8::Isolate* isolate = info.GetIsolate(); \
  v8::Locker locker(isolate); \
  v8::HandleScope scope(isolate); \
  v8::Local<v8::Context> context = isolate->GetCurrentContext()

#include "structures/structures.hpp"

extern gamedll_funcs_t *gpGamedllFuncs;

// nodemod.dll.gameInit();
void sf_dll_pfnGameInit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnGameInit)();
}

// nodemod.dll.spawn(pent: Entity);
void sf_dll_pfnSpawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnSpawn)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.dll.think(pent: Entity);
void sf_dll_pfnThink(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnThink)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.use(pentUsed: Entity, pentOther: Entity);
void sf_dll_pfnUse(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnUse)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]));
}

// nodemod.dll.touch(pentTouched: Entity, pentOther: Entity);
void sf_dll_pfnTouch(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnTouch)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]));
}

// nodemod.dll.blocked(pentBlocked: Entity, pentOther: Entity);
void sf_dll_pfnBlocked(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnBlocked)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]));
}

// nodemod.dll.keyValue(pentKeyvalue: Entity, pkvd: KeyValueData);
void sf_dll_pfnKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnKeyValue)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapKeyValueData(isolate, info[1]));
}

// nodemod.dll.save(pent: Entity, pSaveData: SaveRestoreData);
void sf_dll_pfnSave(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSave)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapSaveRestoreData(isolate, info[1]));
}

// nodemod.dll.restore(pent: Entity, pSaveData: SaveRestoreData, globalEntity: number);
void sf_dll_pfnRestore(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnRestore)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapSaveRestoreData(isolate, info[1]),
info[2]->Int32Value(context).ToChecked())));
}

// nodemod.dll.setAbsBox(pent: Entity);
void sf_dll_pfnSetAbsBox(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSetAbsBox)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.saveWriteFields(value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number);
void sf_dll_pfnSaveWriteFields(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  //if (!info[2]->IsExternal()) {
    //printf("Warning: pfnSaveWriteFields parameter 2 (void*) is not External, using nullptr\n");
  //}

  (*gpGamedllFuncs->dllapi_table->pfnSaveWriteFields)(structures::unwrapSaveRestoreData(isolate, info[0]),
utils::js2string(isolate, info[1]),
nullptr /* void* not supported */,
structures::unwrapTypeDescription(isolate, info[3]),
info[4]->Int32Value(context).ToChecked());
}

// nodemod.dll.saveReadFields(value0: SaveRestoreData, value1: string, value2: ArrayBuffer | Uint8Array | null, value3: TypeDescription, value4: number);
void sf_dll_pfnSaveReadFields(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  //if (!info[2]->IsExternal()) {
    //printf("Warning: pfnSaveReadFields parameter 2 (void*) is not External, using nullptr\n");
  //}

  (*gpGamedllFuncs->dllapi_table->pfnSaveReadFields)(structures::unwrapSaveRestoreData(isolate, info[0]),
utils::js2string(isolate, info[1]),
nullptr /* void* not supported */,
structures::unwrapTypeDescription(isolate, info[3]),
info[4]->Int32Value(context).ToChecked());
}

// nodemod.dll.saveGlobalState(value0: SaveRestoreData);
void sf_dll_pfnSaveGlobalState(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSaveGlobalState)(structures::unwrapSaveRestoreData(isolate, info[0]));
}

// nodemod.dll.restoreGlobalState(value0: SaveRestoreData);
void sf_dll_pfnRestoreGlobalState(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnRestoreGlobalState)(structures::unwrapSaveRestoreData(isolate, info[0]));
}

// nodemod.dll.resetGlobalState();
void sf_dll_pfnResetGlobalState(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnResetGlobalState)();
}

// nodemod.dll.clientConnect(pEntity: Entity, pszName: string, pszAddress: string, szRejectReason: string);
void sf_dll_pfnClientConnect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Boolean::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnClientConnect)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]),
utils::js2string(isolate, info[3]))));
}

// nodemod.dll.clientDisconnect(pEntity: Entity);
void sf_dll_pfnClientDisconnect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnClientDisconnect)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.clientKill(pEntity: Entity);
void sf_dll_pfnClientKill(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnClientKill)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.clientPutInServer(pEntity: Entity);
void sf_dll_pfnClientPutInServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnClientPutInServer)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.clientCommand(pEntity: Entity);
void sf_dll_pfnClientCommand(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnClientCommand)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.clientUserInfoChanged(pEntity: Entity, infobuffer: string);
void sf_dll_pfnClientUserInfoChanged(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnClientUserInfoChanged)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.dll.serverActivate(pEdictList: Entity, edictCount: number, clientMax: number);
void sf_dll_pfnServerActivate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnServerActivate)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
info[2]->Int32Value(context).ToChecked());
}

// nodemod.dll.serverDeactivate();
void sf_dll_pfnServerDeactivate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnServerDeactivate)();
}

// nodemod.dll.playerPreThink(pEntity: Entity);
void sf_dll_pfnPlayerPreThink(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnPlayerPreThink)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.playerPostThink(pEntity: Entity);
void sf_dll_pfnPlayerPostThink(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnPlayerPostThink)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.startFrame();
void sf_dll_pfnStartFrame(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnStartFrame)();
}

// nodemod.dll.parmsNewLevel();
void sf_dll_pfnParmsNewLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnParmsNewLevel)();
}

// nodemod.dll.parmsChangeLevel();
void sf_dll_pfnParmsChangeLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnParmsChangeLevel)();
}

// nodemod.dll.getGameDescription();
void sf_dll_pfnGetGameDescription(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  const char* temp_str = (*gpGamedllFuncs->dllapi_table->pfnGetGameDescription)();
  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, temp_str ? temp_str : "").ToLocalChecked());
}

// nodemod.dll.playerCustomization(pEntity: Entity, pCustom: Customization);
void sf_dll_pfnPlayerCustomization(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnPlayerCustomization)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapCustomization(isolate, info[1]));
}

// nodemod.dll.spectatorConnect(pEntity: Entity);
void sf_dll_pfnSpectatorConnect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSpectatorConnect)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.spectatorDisconnect(pEntity: Entity);
void sf_dll_pfnSpectatorDisconnect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSpectatorDisconnect)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.spectatorThink(pEntity: Entity);
void sf_dll_pfnSpectatorThink(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSpectatorThink)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.sysError(error_string: string);
void sf_dll_pfnSys_Error(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSys_Error)(utils::js2string(isolate, info[0]));
}

// nodemod.dll.pMMove(ppmove: PlayerMove, server: boolean);
void sf_dll_pfnPM_Move(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnPM_Move)(structures::unwrapPlayerMove(isolate, info[0]),
info[1]->BooleanValue(isolate));
}

// nodemod.dll.pMInit(ppmove: PlayerMove);
void sf_dll_pfnPM_Init(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnPM_Init)(structures::unwrapPlayerMove(isolate, info[0]));
}

// nodemod.dll.pMFindTextureType(name: string);
void sf_dll_pfnPM_FindTextureType(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnPM_FindTextureType)(utils::js2string(isolate, info[0]))));
}

// nodemod.dll.setupVisibility(pViewEntity: Entity, pClient: Entity, pvs: number[], pas: number[]);
void sf_dll_pfnSetupVisibility(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnSetupVisibility)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]),
(unsigned char**)utils::jsToPointer(isolate, info[2]),
(unsigned char**)utils::jsToPointer(isolate, info[3]));
}

// nodemod.dll.updateClientData(ent: Entity, sendweapons: number, cd: ClientData);
void sf_dll_pfnUpdateClientData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnUpdateClientData)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
structures::unwrapClientData(isolate, info[2]));
}

// nodemod.dll.addToFullPack(state: EntityState, e: number, ent: Entity, host: Entity, hostflags: number, player: number, pSet: number[]);
void sf_dll_pfnAddToFullPack(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnAddToFullPack)(structures::unwrapEntityState(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[2]),
structures::unwrapEntity(isolate, info[3]),
info[4]->Int32Value(context).ToChecked(),
info[5]->Int32Value(context).ToChecked(),
(unsigned char*)utils::jsToPointer(isolate, info[6]))));
}

// nodemod.dll.createBaseline(player: number, eindex: number, baseline: EntityState, entity: Entity, playermodelindex: number, player_mins: number[], player_maxs: number[]);
void sf_dll_pfnCreateBaseline(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();
  vec3_t player_mins_vec;
  utils::js2vect(isolate, v8::Local<v8::Array>::Cast(info[5]), player_mins_vec);
  vec3_t player_maxs_vec;
  utils::js2vect(isolate, v8::Local<v8::Array>::Cast(info[6]), player_maxs_vec);

  (*gpGamedllFuncs->dllapi_table->pfnCreateBaseline)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked(),
structures::unwrapEntityState(isolate, info[2]),
structures::unwrapEntity(isolate, info[3]),
info[4]->Int32Value(context).ToChecked(),
player_mins_vec,
player_maxs_vec);
}

// nodemod.dll.registerEncoders();
void sf_dll_pfnRegisterEncoders(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnRegisterEncoders)();
}

// nodemod.dll.getWeaponData(player: Entity, info: WeaponData);
void sf_dll_pfnGetWeaponData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnGetWeaponData)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapWeaponData(isolate, info[1]))));
}

// nodemod.dll.cmdStart(player: Entity, cmd: UserCmd, random_seed: number);
void sf_dll_pfnCmdStart(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnCmdStart)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapUserCmd(isolate, info[1]),
info[2]->Uint32Value(context).ToChecked());
}

// nodemod.dll.cmdEnd(player: Entity);
void sf_dll_pfnCmdEnd(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnCmdEnd)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.dll.connectionlessPacket(net_from: NetAdr, args: string, response_buffer: string, response_buffer_size: number[]);
void sf_dll_pfnConnectionlessPacket(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  //if (!info[3]->IsExternal()) {
    //printf("Warning: pfnConnectionlessPacket parameter 3 (int *) is not External, using nullptr\n");
  //}

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnConnectionlessPacket)(structures::unwrapNetAdr(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]),
(int*)utils::jsToPointer(isolate, info[3]))));
}

// nodemod.dll.getHullBounds(hullnumber: number, mins: number[], maxs: number[]);
void sf_dll_pfnGetHullBounds(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  //if (!info[1]->IsExternal()) {
    //printf("Warning: pfnGetHullBounds parameter 1 (float *) is not External, using nullptr\n");
  //}
  //if (!info[2]->IsExternal()) {
    //printf("Warning: pfnGetHullBounds parameter 2 (float *) is not External, using nullptr\n");
  //}

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnGetHullBounds)(info[0]->Int32Value(context).ToChecked(),
(float*)utils::jsToPointer(isolate, info[1]),
(float*)utils::jsToPointer(isolate, info[2]))));
}

// nodemod.dll.createInstancedBaselines();
void sf_dll_pfnCreateInstancedBaselines(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  (*gpGamedllFuncs->dllapi_table->pfnCreateInstancedBaselines)();
}

// nodemod.dll.inconsistentFile(player: Entity, filename: string, disconnect_message: string);
void sf_dll_pfnInconsistentFile(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnInconsistentFile)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]))));
}

// nodemod.dll.allowLagCompensation();
void sf_dll_pfnAllowLagCompensation(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*gpGamedllFuncs->dllapi_table->pfnAllowLagCompensation)()));
}

static std::pair<std::string, v8::FunctionCallback> gamedllSpecificFunctions[] = {
  { "gameInit", sf_dll_pfnGameInit },
  { "spawn", sf_dll_pfnSpawn },
  { "think", sf_dll_pfnThink },
  { "use", sf_dll_pfnUse },
  { "touch", sf_dll_pfnTouch },
  { "blocked", sf_dll_pfnBlocked },
  { "keyValue", sf_dll_pfnKeyValue },
  { "save", sf_dll_pfnSave },
  { "restore", sf_dll_pfnRestore },
  { "setAbsBox", sf_dll_pfnSetAbsBox },
  { "saveWriteFields", sf_dll_pfnSaveWriteFields },
  { "saveReadFields", sf_dll_pfnSaveReadFields },
  { "saveGlobalState", sf_dll_pfnSaveGlobalState },
  { "restoreGlobalState", sf_dll_pfnRestoreGlobalState },
  { "resetGlobalState", sf_dll_pfnResetGlobalState },
  { "clientConnect", sf_dll_pfnClientConnect },
  { "clientDisconnect", sf_dll_pfnClientDisconnect },
  { "clientKill", sf_dll_pfnClientKill },
  { "clientPutInServer", sf_dll_pfnClientPutInServer },
  { "clientCommand", sf_dll_pfnClientCommand },
  { "clientUserInfoChanged", sf_dll_pfnClientUserInfoChanged },
  { "serverActivate", sf_dll_pfnServerActivate },
  { "serverDeactivate", sf_dll_pfnServerDeactivate },
  { "playerPreThink", sf_dll_pfnPlayerPreThink },
  { "playerPostThink", sf_dll_pfnPlayerPostThink },
  { "startFrame", sf_dll_pfnStartFrame },
  { "parmsNewLevel", sf_dll_pfnParmsNewLevel },
  { "parmsChangeLevel", sf_dll_pfnParmsChangeLevel },
  { "getGameDescription", sf_dll_pfnGetGameDescription },
  { "playerCustomization", sf_dll_pfnPlayerCustomization },
  { "spectatorConnect", sf_dll_pfnSpectatorConnect },
  { "spectatorDisconnect", sf_dll_pfnSpectatorDisconnect },
  { "spectatorThink", sf_dll_pfnSpectatorThink },
  { "sysError", sf_dll_pfnSys_Error },
  { "pMMove", sf_dll_pfnPM_Move },
  { "pMInit", sf_dll_pfnPM_Init },
  { "pMFindTextureType", sf_dll_pfnPM_FindTextureType },
  { "setupVisibility", sf_dll_pfnSetupVisibility },
  { "updateClientData", sf_dll_pfnUpdateClientData },
  { "addToFullPack", sf_dll_pfnAddToFullPack },
  { "createBaseline", sf_dll_pfnCreateBaseline },
  { "registerEncoders", sf_dll_pfnRegisterEncoders },
  { "getWeaponData", sf_dll_pfnGetWeaponData },
  { "cmdStart", sf_dll_pfnCmdStart },
  { "cmdEnd", sf_dll_pfnCmdEnd },
  { "connectionlessPacket", sf_dll_pfnConnectionlessPacket },
  { "getHullBounds", sf_dll_pfnGetHullBounds },
  { "createInstancedBaselines", sf_dll_pfnCreateInstancedBaselines },
  { "inconsistentFile", sf_dll_pfnInconsistentFile },
  { "allowLagCompensation", sf_dll_pfnAllowLagCompensation }
};
v8::Local<v8::ObjectTemplate> registerDllFunctions(v8::Isolate* isolate) {
  v8::Local <v8::ObjectTemplate> object = v8::ObjectTemplate::New(isolate);
  for (auto &routine : gamedllSpecificFunctions) {
    object-> Set(v8::String::NewFromUtf8(isolate, routine.first.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), v8::FunctionTemplate::New(isolate, routine.second));
  };
  
  return object;
};

