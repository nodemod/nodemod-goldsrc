// This file builded by: node scripts/buildEvents.js
  #include <extdll.h>
  #include "node/nodeimpl.hpp"
  #include "node/events.hpp"
  #include "meta_api.h"
  #include "node/utils.hpp"
  #include "structures/structures.hpp"

  /* BASE EVENTS */
    // nodemod.on('dllGameInit', () => console.log('dllGameInit fired!'));
  void dll_pfnGameInit () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllGameInit", nullptr, 0);
  }

// nodemod.on('dllSpawn', (pent) => console.log('dllSpawn fired!'));
  int dll_pfnSpawn (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSpawn", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllThink', (pent) => console.log('dllThink fired!'));
  void dll_pfnThink (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllUse', (pentUsed, pentOther) => console.log('dllUse fired!'));
  void dll_pfnUse (edict_t * pentUsed, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllUse", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentUsed); // pentUsed (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllTouch', (pentTouched, pentOther) => console.log('dllTouch fired!'));
  void dll_pfnTouch (edict_t * pentTouched, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllTouch", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentTouched); // pentTouched (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllBlocked', (pentBlocked, pentOther) => console.log('dllBlocked fired!'));
  void dll_pfnBlocked (edict_t * pentBlocked, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllBlocked", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentBlocked); // pentBlocked (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllKeyValue', (pentKeyvalue, pkvd) => console.log('dllKeyValue fired!'));
  void dll_pfnKeyValue (edict_t * pentKeyvalue, KeyValueData * pkvd) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentKeyvalue); // pentKeyvalue (edict_t *)
      v8_args[1] = v8::External::New(isolate, pkvd /* KeyValueData  */); // pkvd (KeyValueData *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSave', (pent, pSaveData) => console.log('dllSave fired!'));
  void dll_pfnSave (edict_t * pent, SAVERESTOREDATA * pSaveData) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSave", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[1] = v8::External::New(isolate, pSaveData /* SAVERESTOREDATA  */); // pSaveData (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllRestore', (pent, pSaveData, globalEntity) => console.log('dllRestore fired!'));
  int dll_pfnRestore (edict_t * pent, SAVERESTOREDATA * pSaveData, int globalEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllRestore", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[1] = v8::External::New(isolate, pSaveData /* SAVERESTOREDATA  */); // pSaveData (SAVERESTOREDATA *)
      v8_args[2] = v8::Number::New(isolate, globalEntity); // globalEntity (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllSetAbsBox', (pent) => console.log('dllSetAbsBox fired!'));
  void dll_pfnSetAbsBox (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSetAbsBox", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSaveWriteFields', (value0, value1, value2, value3, value4) => console.log('dllSaveWriteFields fired!'));
  void dll_pfnSaveWriteFields (SAVERESTOREDATA* value0, const char* value1, void* value2, TYPEDESCRIPTION* value3, int value4) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSaveWriteFields", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA */); // value0 (SAVERESTOREDATA*)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value1).ToLocalChecked(); // value1 (const char*)
      v8_args[2] = v8::External::New(isolate, value2 /* void */); // value2 (void*)
      v8_args[3] = v8::External::New(isolate, value3 /* TYPEDESCRIPTION */); // value3 (TYPEDESCRIPTION*)
      v8_args[4] = v8::Number::New(isolate, value4); // value4 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSaveReadFields', (value0, value1, value2, value3, value4) => console.log('dllSaveReadFields fired!'));
  void dll_pfnSaveReadFields (SAVERESTOREDATA* value0, const char* value1, void* value2, TYPEDESCRIPTION* value3, int value4) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSaveReadFields", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA */); // value0 (SAVERESTOREDATA*)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value1).ToLocalChecked(); // value1 (const char*)
      v8_args[2] = v8::External::New(isolate, value2 /* void */); // value2 (void*)
      v8_args[3] = v8::External::New(isolate, value3 /* TYPEDESCRIPTION */); // value3 (TYPEDESCRIPTION*)
      v8_args[4] = v8::Number::New(isolate, value4); // value4 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSaveGlobalState', (value0) => console.log('dllSaveGlobalState fired!'));
  void dll_pfnSaveGlobalState (SAVERESTOREDATA * value0) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSaveGlobalState", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA  */); // value0 (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllRestoreGlobalState', (value0) => console.log('dllRestoreGlobalState fired!'));
  void dll_pfnRestoreGlobalState (SAVERESTOREDATA * value0) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllRestoreGlobalState", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA  */); // value0 (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllResetGlobalState', () => console.log('dllResetGlobalState fired!'));
  void dll_pfnResetGlobalState () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllResetGlobalState", nullptr, 0);
  }

// nodemod.on('dllClientConnect', (pEntity, pszName, pszAddress, szRejectReason) => console.log('dllClientConnect fired!'));
  qboolean dll_pfnClientConnect (edict_t * pEntity, const char * pszName, const char * pszAddress, char* szRejectReason) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientConnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, pszName).ToLocalChecked(); // pszName (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, pszAddress).ToLocalChecked(); // pszAddress (const char *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, szRejectReason).ToLocalChecked(); // szRejectReason (char*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllClientDisconnect', (pEntity) => console.log('dllClientDisconnect fired!'));
  void dll_pfnClientDisconnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientDisconnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllClientKill', (pEntity) => console.log('dllClientKill fired!'));
  void dll_pfnClientKill (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientKill", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllClientPutInServer', (pEntity) => console.log('dllClientPutInServer fired!'));
  void dll_pfnClientPutInServer (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientPutInServer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllClientCommand', (pEntity) => console.log('dllClientCommand fired!'));
  void dll_pfnClientCommand (edict_t* ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);

  // refactor it str hell
  if (CMD_ARGC() > 1) {
  char buf[100];
  sprintf(buf, "%s %s", CMD_ARGV(0), CMD_ARGS());
  v8_args[1] = v8::String::NewFromUtf8(isolate, buf).ToLocalChecked();
  } else {
    v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGV(0)).ToLocalChecked();
  }
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllClientUserInfoChanged', (pEntity, infobuffer) => console.log('dllClientUserInfoChanged fired!'));
  void dll_pfnClientUserInfoChanged (edict_t * pEntity, char * infobuffer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllClientUserInfoChanged", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, infobuffer).ToLocalChecked(); // infobuffer (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllServerActivate', (pEdictList, edictCount, clientMax) => console.log('dllServerActivate fired!'));
  void dll_pfnServerActivate (edict_t * pEdictList, int edictCount, int clientMax) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllServerActivate", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictList); // pEdictList (edict_t *)
      v8_args[1] = v8::Number::New(isolate, edictCount); // edictCount (int)
      v8_args[2] = v8::Number::New(isolate, clientMax); // clientMax (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllServerDeactivate', () => console.log('dllServerDeactivate fired!'));
  void dll_pfnServerDeactivate () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllServerDeactivate", nullptr, 0);
  }

// nodemod.on('dllPlayerPreThink', (pEntity) => console.log('dllPlayerPreThink fired!'));
  void dll_pfnPlayerPreThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPlayerPreThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllPlayerPostThink', (pEntity) => console.log('dllPlayerPostThink fired!'));
  void dll_pfnPlayerPostThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPlayerPostThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// pfnStartFrame
  void dll_pfnStartFrame () {
    SET_META_RESULT(MRES_IGNORED);
  }

// nodemod.on('dllParmsNewLevel', () => console.log('dllParmsNewLevel fired!'));
  void dll_pfnParmsNewLevel () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllParmsNewLevel", nullptr, 0);
  }

// nodemod.on('dllParmsChangeLevel', () => console.log('dllParmsChangeLevel fired!'));
  void dll_pfnParmsChangeLevel () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllParmsChangeLevel", nullptr, 0);
  }

// nodemod.on('dllGetGameDescription', () => console.log('dllGetGameDescription fired!'));
  const char * dll_pfnGetGameDescription () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllGetGameDescription", nullptr, 0);
    return nullptr;
  }

// nodemod.on('dllPlayerCustomization', (pEntity, pCustom) => console.log('dllPlayerCustomization fired!'));
  void dll_pfnPlayerCustomization (edict_t * pEntity, customization_t * pCustom) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPlayerCustomization", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::External::New(isolate, pCustom /* customization_t  */); // pCustom (customization_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSpectatorConnect', (pEntity) => console.log('dllSpectatorConnect fired!'));
  void dll_pfnSpectatorConnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSpectatorConnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSpectatorDisconnect', (pEntity) => console.log('dllSpectatorDisconnect fired!'));
  void dll_pfnSpectatorDisconnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSpectatorDisconnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSpectatorThink', (pEntity) => console.log('dllSpectatorThink fired!'));
  void dll_pfnSpectatorThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSpectatorThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllSysError', (error_string) => console.log('dllSysError fired!'));
  void dll_pfnSys_Error (const char * error_string) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSysError", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, error_string).ToLocalChecked(); // error_string (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllPMMove', (ppmove, server) => console.log('dllPMMove fired!'));
  void dll_pfnPM_Move (struct playermove_s * ppmove, qboolean server) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPMMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, ppmove /* playermove_s  */); // ppmove (struct playermove_s *)
      v8_args[1] = v8::Boolean::New(isolate, server); // server (qboolean)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllPMInit', (ppmove) => console.log('dllPMInit fired!'));
  void dll_pfnPM_Init (struct playermove_s * ppmove) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPMInit", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, ppmove /* playermove_s  */); // ppmove (struct playermove_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllPMFindTextureType', (name) => console.log('dllPMFindTextureType fired!'));
  char dll_pfnPM_FindTextureType (const char * name) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllPMFindTextureType", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name).ToLocalChecked(); // name (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllSetupVisibility', (pViewEntity, pClient, pvs, pas) => console.log('dllSetupVisibility fired!'));
  void dll_pfnSetupVisibility (struct edict_s * pViewEntity, struct edict_s * pClient, unsigned char ** pvs, unsigned char ** pas) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllSetupVisibility", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::External::New(isolate, pViewEntity /* edict_s  */); // pViewEntity (struct edict_s *)
      v8_args[1] = v8::External::New(isolate, pClient /* edict_s  */); // pClient (struct edict_s *)
      v8_args[2] = v8::External::New(isolate, pvs /* unsigned char * */); // pvs (unsigned char **)
      v8_args[3] = v8::External::New(isolate, pas /* unsigned char * */); // pas (unsigned char **)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllUpdateClientData', (ent, sendweapons, cd) => console.log('dllUpdateClientData fired!'));
  void dll_pfnUpdateClientData (const struct edict_s * ent, int sendweapons, struct clientdata_s * cd) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllUpdateClientData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, (void*)ent /* edict_s  */); // ent (const struct edict_s *)
      v8_args[1] = v8::Number::New(isolate, sendweapons); // sendweapons (int)
      v8_args[2] = v8::External::New(isolate, cd /* clientdata_s  */); // cd (struct clientdata_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllAddToFullPack', (state, e, ent, host, hostflags, player, pSet) => console.log('dllAddToFullPack fired!'));
  int dll_pfnAddToFullPack (struct entity_state_s * state, int e, edict_t * ent, edict_t * host, int hostflags, int player, unsigned char * pSet) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllAddToFullPack", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = v8::External::New(isolate, state /* entity_state_s  */); // state (struct entity_state_s *)
      v8_args[1] = v8::Number::New(isolate, e); // e (int)
      v8_args[2] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[3] = structures::wrapEntity(isolate, host); // host (edict_t *)
      v8_args[4] = v8::Number::New(isolate, hostflags); // hostflags (int)
      v8_args[5] = v8::Number::New(isolate, player); // player (int)
      v8_args[6] = v8::External::New(isolate, pSet /* unsigned char  */); // pSet (unsigned char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllCreateBaseline', (player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs) => console.log('dllCreateBaseline fired!'));
  void dll_pfnCreateBaseline (int player, int eindex, struct entity_state_s * baseline, struct edict_s * entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllCreateBaseline", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = v8::Number::New(isolate, player); // player (int)
      v8_args[1] = v8::Number::New(isolate, eindex); // eindex (int)
      v8_args[2] = v8::External::New(isolate, baseline /* entity_state_s  */); // baseline (struct entity_state_s *)
      v8_args[3] = v8::External::New(isolate, entity /* edict_s  */); // entity (struct edict_s *)
      v8_args[4] = v8::Number::New(isolate, playermodelindex); // playermodelindex (int)
      v8_args[5] = utils::vect2js(isolate, player_mins); // player_mins (vec3_t)
      v8_args[6] = utils::vect2js(isolate, player_maxs); // player_maxs (vec3_t)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllRegisterEncoders', () => console.log('dllRegisterEncoders fired!'));
  void dll_pfnRegisterEncoders () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllRegisterEncoders", nullptr, 0);
  }

// nodemod.on('dllGetWeaponData', (player, info) => console.log('dllGetWeaponData fired!'));
  int dll_pfnGetWeaponData (struct edict_s * player, struct weapon_data_s * info) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllGetWeaponData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, player /* edict_s  */); // player (struct edict_s *)
      v8_args[1] = v8::External::New(isolate, info /* weapon_data_s  */); // info (struct weapon_data_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllCmdStart', (player, cmd, random_seed) => console.log('dllCmdStart fired!'));
  void dll_pfnCmdStart (const edict_t * player, const struct usercmd_s * cmd, unsigned int random_seed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllCmdStart", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::External::New(isolate, (void*)cmd /* usercmd_s  */); // cmd (const struct usercmd_s *)
      v8_args[2] = v8::Number::New(isolate, random_seed); // random_seed (unsigned int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllCmdEnd', (player) => console.log('dllCmdEnd fired!'));
  void dll_pfnCmdEnd (const edict_t * player) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllCmdEnd", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('dllConnectionlessPacket', (net_from, args, response_buffer, response_buffer_size) => console.log('dllConnectionlessPacket fired!'));
  int dll_pfnConnectionlessPacket (const struct netadr_s * net_from, const char * args, char * response_buffer, int * response_buffer_size) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllConnectionlessPacket", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::External::New(isolate, (void*)net_from /* netadr_s  */); // net_from (const struct netadr_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, args).ToLocalChecked(); // args (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, response_buffer).ToLocalChecked(); // response_buffer (char *)
      v8_args[3] = v8::External::New(isolate, response_buffer_size /* int  */); // response_buffer_size (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllGetHullBounds', (hullnumber, mins, maxs) => console.log('dllGetHullBounds fired!'));
  int dll_pfnGetHullBounds (int hullnumber, float * mins, float * maxs) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllGetHullBounds", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::Number::New(isolate, hullnumber); // hullnumber (int)
      v8_args[1] = v8::External::New(isolate, mins /* float  */); // mins (float *)
      v8_args[2] = v8::External::New(isolate, maxs /* float  */); // maxs (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllCreateInstancedBaselines', () => console.log('dllCreateInstancedBaselines fired!'));
  void dll_pfnCreateInstancedBaselines () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllCreateInstancedBaselines", nullptr, 0);
  }

// nodemod.on('dllInconsistentFile', (player, filename, disconnect_message) => console.log('dllInconsistentFile fired!'));
  int dll_pfnInconsistentFile (const struct edict_s * player, const char * filename, char * disconnect_message) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllInconsistentFile", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, (void*)player /* edict_s  */); // player (const struct edict_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, filename).ToLocalChecked(); // filename (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, disconnect_message).ToLocalChecked(); // disconnect_message (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('dllAllowLagCompensation', () => console.log('dllAllowLagCompensation fired!'));
  int dll_pfnAllowLagCompensation () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("dllAllowLagCompensation", nullptr, 0);
    return 0;
  }

    DLL_FUNCTIONS g_DllFunctionTable = {
      dll_pfnGameInit,
      dll_pfnSpawn,
      dll_pfnThink,
      dll_pfnUse,
      dll_pfnTouch,
      dll_pfnBlocked,
      dll_pfnKeyValue,
      dll_pfnSave,
      dll_pfnRestore,
      dll_pfnSetAbsBox,
      dll_pfnSaveWriteFields,
      dll_pfnSaveReadFields,
      dll_pfnSaveGlobalState,
      dll_pfnRestoreGlobalState,
      dll_pfnResetGlobalState,
      dll_pfnClientConnect,
      dll_pfnClientDisconnect,
      dll_pfnClientKill,
      dll_pfnClientPutInServer,
      dll_pfnClientCommand,
      dll_pfnClientUserInfoChanged,
      dll_pfnServerActivate,
      dll_pfnServerDeactivate,
      dll_pfnPlayerPreThink,
      dll_pfnPlayerPostThink,
      dll_pfnStartFrame,
      dll_pfnParmsNewLevel,
      dll_pfnParmsChangeLevel,
      dll_pfnGetGameDescription,
      dll_pfnPlayerCustomization,
      dll_pfnSpectatorConnect,
      dll_pfnSpectatorDisconnect,
      dll_pfnSpectatorThink,
      dll_pfnSys_Error,
      dll_pfnPM_Move,
      dll_pfnPM_Init,
      dll_pfnPM_FindTextureType,
      dll_pfnSetupVisibility,
      dll_pfnUpdateClientData,
      dll_pfnAddToFullPack,
      dll_pfnCreateBaseline,
      dll_pfnRegisterEncoders,
      dll_pfnGetWeaponData,
      dll_pfnCmdStart,
      dll_pfnCmdEnd,
      dll_pfnConnectionlessPacket,
      dll_pfnGetHullBounds,
      dll_pfnCreateInstancedBaselines,
      dll_pfnInconsistentFile,
      dll_pfnAllowLagCompensation
    };

  /* POST EVENTS */
    // nodemod.on('postDllGameInit', () => console.log('postDllGameInit fired!'));
  void postDll_pfnGameInit () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllGameInit", nullptr, 0);
  }

// nodemod.on('postDllSpawn', (pent) => console.log('postDllSpawn fired!'));
  int postDll_pfnSpawn (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSpawn", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllThink', (pent) => console.log('postDllThink fired!'));
  void postDll_pfnThink (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllUse', (pentUsed, pentOther) => console.log('postDllUse fired!'));
  void postDll_pfnUse (edict_t * pentUsed, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllUse", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentUsed); // pentUsed (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllTouch', (pentTouched, pentOther) => console.log('postDllTouch fired!'));
  void postDll_pfnTouch (edict_t * pentTouched, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllTouch", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentTouched); // pentTouched (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllBlocked', (pentBlocked, pentOther) => console.log('postDllBlocked fired!'));
  void postDll_pfnBlocked (edict_t * pentBlocked, edict_t * pentOther) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllBlocked", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentBlocked); // pentBlocked (edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pentOther); // pentOther (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllKeyValue', (pentKeyvalue, pkvd) => console.log('postDllKeyValue fired!'));
  void postDll_pfnKeyValue (edict_t * pentKeyvalue, KeyValueData * pkvd) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pentKeyvalue); // pentKeyvalue (edict_t *)
      v8_args[1] = v8::External::New(isolate, pkvd /* KeyValueData  */); // pkvd (KeyValueData *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSave', (pent, pSaveData) => console.log('postDllSave fired!'));
  void postDll_pfnSave (edict_t * pent, SAVERESTOREDATA * pSaveData) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSave", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[1] = v8::External::New(isolate, pSaveData /* SAVERESTOREDATA  */); // pSaveData (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllRestore', (pent, pSaveData, globalEntity) => console.log('postDllRestore fired!'));
  int postDll_pfnRestore (edict_t * pent, SAVERESTOREDATA * pSaveData, int globalEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllRestore", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[1] = v8::External::New(isolate, pSaveData /* SAVERESTOREDATA  */); // pSaveData (SAVERESTOREDATA *)
      v8_args[2] = v8::Number::New(isolate, globalEntity); // globalEntity (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllSetAbsBox', (pent) => console.log('postDllSetAbsBox fired!'));
  void postDll_pfnSetAbsBox (edict_t * pent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSetAbsBox", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSaveWriteFields', (value0, value1, value2, value3, value4) => console.log('postDllSaveWriteFields fired!'));
  void postDll_pfnSaveWriteFields (SAVERESTOREDATA* value0, const char* value1, void* value2, TYPEDESCRIPTION* value3, int value4) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSaveWriteFields", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA */); // value0 (SAVERESTOREDATA*)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value1).ToLocalChecked(); // value1 (const char*)
      v8_args[2] = v8::External::New(isolate, value2 /* void */); // value2 (void*)
      v8_args[3] = v8::External::New(isolate, value3 /* TYPEDESCRIPTION */); // value3 (TYPEDESCRIPTION*)
      v8_args[4] = v8::Number::New(isolate, value4); // value4 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSaveReadFields', (value0, value1, value2, value3, value4) => console.log('postDllSaveReadFields fired!'));
  void postDll_pfnSaveReadFields (SAVERESTOREDATA* value0, const char* value1, void* value2, TYPEDESCRIPTION* value3, int value4) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSaveReadFields", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA */); // value0 (SAVERESTOREDATA*)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value1).ToLocalChecked(); // value1 (const char*)
      v8_args[2] = v8::External::New(isolate, value2 /* void */); // value2 (void*)
      v8_args[3] = v8::External::New(isolate, value3 /* TYPEDESCRIPTION */); // value3 (TYPEDESCRIPTION*)
      v8_args[4] = v8::Number::New(isolate, value4); // value4 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSaveGlobalState', (value0) => console.log('postDllSaveGlobalState fired!'));
  void postDll_pfnSaveGlobalState (SAVERESTOREDATA * value0) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSaveGlobalState", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA  */); // value0 (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllRestoreGlobalState', (value0) => console.log('postDllRestoreGlobalState fired!'));
  void postDll_pfnRestoreGlobalState (SAVERESTOREDATA * value0) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllRestoreGlobalState", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, value0 /* SAVERESTOREDATA  */); // value0 (SAVERESTOREDATA *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllResetGlobalState', () => console.log('postDllResetGlobalState fired!'));
  void postDll_pfnResetGlobalState () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllResetGlobalState", nullptr, 0);
  }

// nodemod.on('postDllClientConnect', (pEntity, pszName, pszAddress, szRejectReason) => console.log('postDllClientConnect fired!'));
  qboolean postDll_pfnClientConnect (edict_t * pEntity, const char * pszName, const char * pszAddress, char* szRejectReason) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientConnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, pszName).ToLocalChecked(); // pszName (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, pszAddress).ToLocalChecked(); // pszAddress (const char *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, szRejectReason).ToLocalChecked(); // szRejectReason (char*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllClientDisconnect', (pEntity) => console.log('postDllClientDisconnect fired!'));
  void postDll_pfnClientDisconnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientDisconnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllClientKill', (pEntity) => console.log('postDllClientKill fired!'));
  void postDll_pfnClientKill (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientKill", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllClientPutInServer', (pEntity) => console.log('postDllClientPutInServer fired!'));
  void postDll_pfnClientPutInServer (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientPutInServer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllClientCommand', (pEntity) => console.log('postDllClientCommand fired!'));
  void postDll_pfnClientCommand (edict_t* ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);

  // refactor it str hell
  if (CMD_ARGC() > 1) {
  char buf[100];
  sprintf(buf, "%s %s", CMD_ARGV(0), CMD_ARGS());
  v8_args[1] = v8::String::NewFromUtf8(isolate, buf).ToLocalChecked();
  } else {
    v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGV(0)).ToLocalChecked();
  }
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllClientUserInfoChanged', (pEntity, infobuffer) => console.log('postDllClientUserInfoChanged fired!'));
  void postDll_pfnClientUserInfoChanged (edict_t * pEntity, char * infobuffer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllClientUserInfoChanged", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, infobuffer).ToLocalChecked(); // infobuffer (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllServerActivate', (pEdictList, edictCount, clientMax) => console.log('postDllServerActivate fired!'));
  void postDll_pfnServerActivate (edict_t * pEdictList, int edictCount, int clientMax) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllServerActivate", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictList); // pEdictList (edict_t *)
      v8_args[1] = v8::Number::New(isolate, edictCount); // edictCount (int)
      v8_args[2] = v8::Number::New(isolate, clientMax); // clientMax (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllServerDeactivate', () => console.log('postDllServerDeactivate fired!'));
  void postDll_pfnServerDeactivate () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllServerDeactivate", nullptr, 0);
  }

// nodemod.on('postDllPlayerPreThink', (pEntity) => console.log('postDllPlayerPreThink fired!'));
  void postDll_pfnPlayerPreThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPlayerPreThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllPlayerPostThink', (pEntity) => console.log('postDllPlayerPostThink fired!'));
  void postDll_pfnPlayerPostThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPlayerPostThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// pfnStartFrame
  void postDll_pfnStartFrame () {
    SET_META_RESULT(MRES_IGNORED);
  }

// nodemod.on('postDllParmsNewLevel', () => console.log('postDllParmsNewLevel fired!'));
  void postDll_pfnParmsNewLevel () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllParmsNewLevel", nullptr, 0);
  }

// nodemod.on('postDllParmsChangeLevel', () => console.log('postDllParmsChangeLevel fired!'));
  void postDll_pfnParmsChangeLevel () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllParmsChangeLevel", nullptr, 0);
  }

// nodemod.on('postDllGetGameDescription', () => console.log('postDllGetGameDescription fired!'));
  const char * postDll_pfnGetGameDescription () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllGetGameDescription", nullptr, 0);
    return nullptr;
  }

// nodemod.on('postDllPlayerCustomization', (pEntity, pCustom) => console.log('postDllPlayerCustomization fired!'));
  void postDll_pfnPlayerCustomization (edict_t * pEntity, customization_t * pCustom) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPlayerCustomization", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      v8_args[1] = v8::External::New(isolate, pCustom /* customization_t  */); // pCustom (customization_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSpectatorConnect', (pEntity) => console.log('postDllSpectatorConnect fired!'));
  void postDll_pfnSpectatorConnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSpectatorConnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSpectatorDisconnect', (pEntity) => console.log('postDllSpectatorDisconnect fired!'));
  void postDll_pfnSpectatorDisconnect (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSpectatorDisconnect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSpectatorThink', (pEntity) => console.log('postDllSpectatorThink fired!'));
  void postDll_pfnSpectatorThink (edict_t * pEntity) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSpectatorThink", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEntity); // pEntity (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllSysError', (error_string) => console.log('postDllSysError fired!'));
  void postDll_pfnSys_Error (const char * error_string) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSysError", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, error_string).ToLocalChecked(); // error_string (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllPMMove', (ppmove, server) => console.log('postDllPMMove fired!'));
  void postDll_pfnPM_Move (struct playermove_s * ppmove, qboolean server) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPMMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, ppmove /* playermove_s  */); // ppmove (struct playermove_s *)
      v8_args[1] = v8::Boolean::New(isolate, server); // server (qboolean)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllPMInit', (ppmove) => console.log('postDllPMInit fired!'));
  void postDll_pfnPM_Init (struct playermove_s * ppmove) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPMInit", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, ppmove /* playermove_s  */); // ppmove (struct playermove_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllPMFindTextureType', (name) => console.log('postDllPMFindTextureType fired!'));
  char postDll_pfnPM_FindTextureType (const char * name) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllPMFindTextureType", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name).ToLocalChecked(); // name (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllSetupVisibility', (pViewEntity, pClient, pvs, pas) => console.log('postDllSetupVisibility fired!'));
  void postDll_pfnSetupVisibility (struct edict_s * pViewEntity, struct edict_s * pClient, unsigned char ** pvs, unsigned char ** pas) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllSetupVisibility", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::External::New(isolate, pViewEntity /* edict_s  */); // pViewEntity (struct edict_s *)
      v8_args[1] = v8::External::New(isolate, pClient /* edict_s  */); // pClient (struct edict_s *)
      v8_args[2] = v8::External::New(isolate, pvs /* unsigned char * */); // pvs (unsigned char **)
      v8_args[3] = v8::External::New(isolate, pas /* unsigned char * */); // pas (unsigned char **)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllUpdateClientData', (ent, sendweapons, cd) => console.log('postDllUpdateClientData fired!'));
  void postDll_pfnUpdateClientData (const struct edict_s * ent, int sendweapons, struct clientdata_s * cd) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllUpdateClientData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, (void*)ent /* edict_s  */); // ent (const struct edict_s *)
      v8_args[1] = v8::Number::New(isolate, sendweapons); // sendweapons (int)
      v8_args[2] = v8::External::New(isolate, cd /* clientdata_s  */); // cd (struct clientdata_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllAddToFullPack', (state, e, ent, host, hostflags, player, pSet) => console.log('postDllAddToFullPack fired!'));
  int postDll_pfnAddToFullPack (struct entity_state_s * state, int e, edict_t * ent, edict_t * host, int hostflags, int player, unsigned char * pSet) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllAddToFullPack", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = v8::External::New(isolate, state /* entity_state_s  */); // state (struct entity_state_s *)
      v8_args[1] = v8::Number::New(isolate, e); // e (int)
      v8_args[2] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[3] = structures::wrapEntity(isolate, host); // host (edict_t *)
      v8_args[4] = v8::Number::New(isolate, hostflags); // hostflags (int)
      v8_args[5] = v8::Number::New(isolate, player); // player (int)
      v8_args[6] = v8::External::New(isolate, pSet /* unsigned char  */); // pSet (unsigned char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllCreateBaseline', (player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs) => console.log('postDllCreateBaseline fired!'));
  void postDll_pfnCreateBaseline (int player, int eindex, struct entity_state_s * baseline, struct edict_s * entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllCreateBaseline", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = v8::Number::New(isolate, player); // player (int)
      v8_args[1] = v8::Number::New(isolate, eindex); // eindex (int)
      v8_args[2] = v8::External::New(isolate, baseline /* entity_state_s  */); // baseline (struct entity_state_s *)
      v8_args[3] = v8::External::New(isolate, entity /* edict_s  */); // entity (struct edict_s *)
      v8_args[4] = v8::Number::New(isolate, playermodelindex); // playermodelindex (int)
      v8_args[5] = utils::vect2js(isolate, player_mins); // player_mins (vec3_t)
      v8_args[6] = utils::vect2js(isolate, player_maxs); // player_maxs (vec3_t)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllRegisterEncoders', () => console.log('postDllRegisterEncoders fired!'));
  void postDll_pfnRegisterEncoders () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllRegisterEncoders", nullptr, 0);
  }

// nodemod.on('postDllGetWeaponData', (player, info) => console.log('postDllGetWeaponData fired!'));
  int postDll_pfnGetWeaponData (struct edict_s * player, struct weapon_data_s * info) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllGetWeaponData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, player /* edict_s  */); // player (struct edict_s *)
      v8_args[1] = v8::External::New(isolate, info /* weapon_data_s  */); // info (struct weapon_data_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllCmdStart', (player, cmd, random_seed) => console.log('postDllCmdStart fired!'));
  void postDll_pfnCmdStart (const edict_t * player, const struct usercmd_s * cmd, unsigned int random_seed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllCmdStart", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::External::New(isolate, (void*)cmd /* usercmd_s  */); // cmd (const struct usercmd_s *)
      v8_args[2] = v8::Number::New(isolate, random_seed); // random_seed (unsigned int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllCmdEnd', (player) => console.log('postDllCmdEnd fired!'));
  void postDll_pfnCmdEnd (const edict_t * player) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllCmdEnd", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postDllConnectionlessPacket', (net_from, args, response_buffer, response_buffer_size) => console.log('postDllConnectionlessPacket fired!'));
  int postDll_pfnConnectionlessPacket (const struct netadr_s * net_from, const char * args, char * response_buffer, int * response_buffer_size) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllConnectionlessPacket", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::External::New(isolate, (void*)net_from /* netadr_s  */); // net_from (const struct netadr_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, args).ToLocalChecked(); // args (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, response_buffer).ToLocalChecked(); // response_buffer (char *)
      v8_args[3] = v8::External::New(isolate, response_buffer_size /* int  */); // response_buffer_size (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllGetHullBounds', (hullnumber, mins, maxs) => console.log('postDllGetHullBounds fired!'));
  int postDll_pfnGetHullBounds (int hullnumber, float * mins, float * maxs) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllGetHullBounds", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::Number::New(isolate, hullnumber); // hullnumber (int)
      v8_args[1] = v8::External::New(isolate, mins /* float  */); // mins (float *)
      v8_args[2] = v8::External::New(isolate, maxs /* float  */); // maxs (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllCreateInstancedBaselines', () => console.log('postDllCreateInstancedBaselines fired!'));
  void postDll_pfnCreateInstancedBaselines () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllCreateInstancedBaselines", nullptr, 0);
  }

// nodemod.on('postDllInconsistentFile', (player, filename, disconnect_message) => console.log('postDllInconsistentFile fired!'));
  int postDll_pfnInconsistentFile (const struct edict_s * player, const char * filename, char * disconnect_message) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllInconsistentFile", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, (void*)player /* edict_s  */); // player (const struct edict_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, filename).ToLocalChecked(); // filename (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, disconnect_message).ToLocalChecked(); // disconnect_message (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postDllAllowLagCompensation', () => console.log('postDllAllowLagCompensation fired!'));
  int postDll_pfnAllowLagCompensation () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postDllAllowLagCompensation", nullptr, 0);
    return 0;
  }

    DLL_FUNCTIONS g_DllFunctionTable_Post = {
      postDll_pfnGameInit,
      postDll_pfnSpawn,
      postDll_pfnThink,
      postDll_pfnUse,
      postDll_pfnTouch,
      postDll_pfnBlocked,
      postDll_pfnKeyValue,
      postDll_pfnSave,
      postDll_pfnRestore,
      postDll_pfnSetAbsBox,
      postDll_pfnSaveWriteFields,
      postDll_pfnSaveReadFields,
      postDll_pfnSaveGlobalState,
      postDll_pfnRestoreGlobalState,
      postDll_pfnResetGlobalState,
      postDll_pfnClientConnect,
      postDll_pfnClientDisconnect,
      postDll_pfnClientKill,
      postDll_pfnClientPutInServer,
      postDll_pfnClientCommand,
      postDll_pfnClientUserInfoChanged,
      postDll_pfnServerActivate,
      postDll_pfnServerDeactivate,
      postDll_pfnPlayerPreThink,
      postDll_pfnPlayerPostThink,
      postDll_pfnStartFrame,
      postDll_pfnParmsNewLevel,
      postDll_pfnParmsChangeLevel,
      postDll_pfnGetGameDescription,
      postDll_pfnPlayerCustomization,
      postDll_pfnSpectatorConnect,
      postDll_pfnSpectatorDisconnect,
      postDll_pfnSpectatorThink,
      postDll_pfnSys_Error,
      postDll_pfnPM_Move,
      postDll_pfnPM_Init,
      postDll_pfnPM_FindTextureType,
      postDll_pfnSetupVisibility,
      postDll_pfnUpdateClientData,
      postDll_pfnAddToFullPack,
      postDll_pfnCreateBaseline,
      postDll_pfnRegisterEncoders,
      postDll_pfnGetWeaponData,
      postDll_pfnCmdStart,
      postDll_pfnCmdEnd,
      postDll_pfnConnectionlessPacket,
      postDll_pfnGetHullBounds,
      postDll_pfnCreateInstancedBaselines,
      postDll_pfnInconsistentFile,
      postDll_pfnAllowLagCompensation
    };

    void registerDllEvents()
    {
      // base
      event::register_event("dllGameInit", "");
event::register_event("dllSpawn", "");
event::register_event("dllThink", "");
event::register_event("dllUse", "");
event::register_event("dllTouch", "");
event::register_event("dllBlocked", "");
event::register_event("dllKeyValue", "");
event::register_event("dllSave", "");
event::register_event("dllRestore", "");
event::register_event("dllSetAbsBox", "");
event::register_event("dllSaveWriteFields", "");
event::register_event("dllSaveReadFields", "");
event::register_event("dllSaveGlobalState", "");
event::register_event("dllRestoreGlobalState", "");
event::register_event("dllResetGlobalState", "");
event::register_event("dllClientConnect", "");
event::register_event("dllClientDisconnect", "");
event::register_event("dllClientKill", "");
event::register_event("dllClientPutInServer", "");
event::register_event("dllClientCommand", "");
event::register_event("dllClientUserInfoChanged", "");
event::register_event("dllServerActivate", "");
event::register_event("dllServerDeactivate", "");
event::register_event("dllPlayerPreThink", "");
event::register_event("dllPlayerPostThink", "");
event::register_event("dllStartFrame", "");
event::register_event("dllParmsNewLevel", "");
event::register_event("dllParmsChangeLevel", "");
event::register_event("dllGetGameDescription", "");
event::register_event("dllPlayerCustomization", "");
event::register_event("dllSpectatorConnect", "");
event::register_event("dllSpectatorDisconnect", "");
event::register_event("dllSpectatorThink", "");
event::register_event("dllSysError", "");
event::register_event("dllPMMove", "");
event::register_event("dllPMInit", "");
event::register_event("dllPMFindTextureType", "");
event::register_event("dllSetupVisibility", "");
event::register_event("dllUpdateClientData", "");
event::register_event("dllAddToFullPack", "");
event::register_event("dllCreateBaseline", "");
event::register_event("dllRegisterEncoders", "");
event::register_event("dllGetWeaponData", "");
event::register_event("dllCmdStart", "");
event::register_event("dllCmdEnd", "");
event::register_event("dllConnectionlessPacket", "");
event::register_event("dllGetHullBounds", "");
event::register_event("dllCreateInstancedBaselines", "");
event::register_event("dllInconsistentFile", "");
event::register_event("dllAllowLagCompensation", "");
      // post
      event::register_event("postDllGameInit", "");
event::register_event("postDllSpawn", "");
event::register_event("postDllThink", "");
event::register_event("postDllUse", "");
event::register_event("postDllTouch", "");
event::register_event("postDllBlocked", "");
event::register_event("postDllKeyValue", "");
event::register_event("postDllSave", "");
event::register_event("postDllRestore", "");
event::register_event("postDllSetAbsBox", "");
event::register_event("postDllSaveWriteFields", "");
event::register_event("postDllSaveReadFields", "");
event::register_event("postDllSaveGlobalState", "");
event::register_event("postDllRestoreGlobalState", "");
event::register_event("postDllResetGlobalState", "");
event::register_event("postDllClientConnect", "");
event::register_event("postDllClientDisconnect", "");
event::register_event("postDllClientKill", "");
event::register_event("postDllClientPutInServer", "");
event::register_event("postDllClientCommand", "");
event::register_event("postDllClientUserInfoChanged", "");
event::register_event("postDllServerActivate", "");
event::register_event("postDllServerDeactivate", "");
event::register_event("postDllPlayerPreThink", "");
event::register_event("postDllPlayerPostThink", "");
event::register_event("postDllStartFrame", "");
event::register_event("postDllParmsNewLevel", "");
event::register_event("postDllParmsChangeLevel", "");
event::register_event("postDllGetGameDescription", "");
event::register_event("postDllPlayerCustomization", "");
event::register_event("postDllSpectatorConnect", "");
event::register_event("postDllSpectatorDisconnect", "");
event::register_event("postDllSpectatorThink", "");
event::register_event("postDllSysError", "");
event::register_event("postDllPMMove", "");
event::register_event("postDllPMInit", "");
event::register_event("postDllPMFindTextureType", "");
event::register_event("postDllSetupVisibility", "");
event::register_event("postDllUpdateClientData", "");
event::register_event("postDllAddToFullPack", "");
event::register_event("postDllCreateBaseline", "");
event::register_event("postDllRegisterEncoders", "");
event::register_event("postDllGetWeaponData", "");
event::register_event("postDllCmdStart", "");
event::register_event("postDllCmdEnd", "");
event::register_event("postDllConnectionlessPacket", "");
event::register_event("postDllGetHullBounds", "");
event::register_event("postDllCreateInstancedBaselines", "");
event::register_event("postDllInconsistentFile", "");
event::register_event("postDllAllowLagCompensation", "");
    }
  