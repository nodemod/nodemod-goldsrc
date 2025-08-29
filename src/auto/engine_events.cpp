// This file builded by: node scripts/buildEvents.js
  #include <extdll.h>
  #include "node/nodeimpl.hpp"
  #include "node/events.hpp"
  #include "meta_api.h"
  #include "node/utils.hpp"
  #include "structures/structures.hpp"

  /* BASE EVENTS */
    // nodemod.on('engPrecacheModel', (s) => console.log('engPrecacheModel fired!'));
  int eng_pfnPrecacheModel (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPrecacheModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engPrecacheSound', (s) => console.log('engPrecacheSound fired!'));
  int eng_pfnPrecacheSound (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPrecacheSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engSetModel', (e, m) => console.log('engSetModel fired!'));
  void eng_pfnSetModel (edict_t * e, const char * m) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, m ? m : "").ToLocalChecked(); // m (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engModelIndex', (m) => console.log('engModelIndex fired!'));
  int eng_pfnModelIndex (const char * m) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engModelIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, m ? m : "").ToLocalChecked(); // m (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engModelFrames', (modelIndex) => console.log('engModelFrames fired!'));
  int eng_pfnModelFrames (int modelIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engModelFrames", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, modelIndex); // modelIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engSetSize', (e, rgflMin, rgflMax) => console.log('engSetSize fired!'));
  void eng_pfnSetSize (edict_t * e, const float * rgflMin, const float * rgflMax) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetSize", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflMin, 3); // rgflMin (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflMax, 3); // rgflMax (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engChangeLevel', (s1, s2) => console.log('engChangeLevel fired!'));
  void eng_pfnChangeLevel (const char * s1, const char * s2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engChangeLevel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s1 ? s1 : "").ToLocalChecked(); // s1 (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, s2 ? s2 : "").ToLocalChecked(); // s2 (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetSpawnParms', (ent) => console.log('engGetSpawnParms fired!'));
  void eng_pfnGetSpawnParms (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetSpawnParms", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSaveSpawnParms', (ent) => console.log('engSaveSpawnParms fired!'));
  void eng_pfnSaveSpawnParms (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSaveSpawnParms", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engVecToYaw', (rgflVector) => console.log('engVecToYaw fired!'));
  float eng_pfnVecToYaw (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engVecToYaw", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('engVecToAngles', (rgflVectorIn, rgflVectorOut) => console.log('engVecToAngles fired!'));
  void eng_pfnVecToAngles (const float * rgflVectorIn, float * rgflVectorOut) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engVecToAngles", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVectorIn, 3); // rgflVectorIn (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflVectorOut, 3); // rgflVectorOut (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engMoveToOrigin', (ent, pflGoal, dist, iMoveType) => console.log('engMoveToOrigin fired!'));
  void eng_pfnMoveToOrigin (edict_t * ent, const float * pflGoal, float dist, int iMoveType) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engMoveToOrigin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, pflGoal, 3); // pflGoal (const float *)
      v8_args[2] = v8::Number::New(isolate, dist); // dist (float)
      v8_args[3] = v8::Number::New(isolate, iMoveType); // iMoveType (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engChangeYaw', (ent) => console.log('engChangeYaw fired!'));
  void eng_pfnChangeYaw (edict_t* ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engChangeYaw", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engChangePitch', (ent) => console.log('engChangePitch fired!'));
  void eng_pfnChangePitch (edict_t* ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engChangePitch", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engFindEntityByString', (pEdictStartSearchAfter, pszField, pszValue) => console.log('engFindEntityByString fired!'));
  edict_t* eng_pfnFindEntityByString (edict_t * pEdictStartSearchAfter, const char * pszField, const char * pszValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFindEntityByString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictStartSearchAfter); // pEdictStartSearchAfter (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, pszField ? pszField : "").ToLocalChecked(); // pszField (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, pszValue ? pszValue : "").ToLocalChecked(); // pszValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engGetEntityIllum', (pEnt) => console.log('engGetEntityIllum fired!'));
  int eng_pfnGetEntityIllum (edict_t* pEnt) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetEntityIllum", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEnt); // pEnt (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engFindEntityInSphere', (pEdictStartSearchAfter, org, rad) => console.log('engFindEntityInSphere fired!'));
  edict_t* eng_pfnFindEntityInSphere (edict_t * pEdictStartSearchAfter, const float * org, float rad) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFindEntityInSphere", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictStartSearchAfter); // pEdictStartSearchAfter (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      v8_args[2] = v8::Number::New(isolate, rad); // rad (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engFindClientInPVS', (pEdict) => console.log('engFindClientInPVS fired!'));
  edict_t* eng_pfnFindClientInPVS (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFindClientInPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engEntitiesInPVS', (pplayer) => console.log('engEntitiesInPVS fired!'));
  edict_t* eng_pfnEntitiesInPVS (edict_t * pplayer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEntitiesInPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pplayer); // pplayer (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engMakeVectors', (rgflVector) => console.log('engMakeVectors fired!'));
  void eng_pfnMakeVectors (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engMakeVectors", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engAngleVectors', (rgflVector, forward, right, up) => console.log('engAngleVectors fired!'));
  void eng_pfnAngleVectors (const float * rgflVector, float * forward, float * right, float * up) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engAngleVectors", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, forward, 3); // forward (float *)
      v8_args[2] = utils::floatArrayToJS(isolate, right, 3); // right (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, up, 3); // up (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCreateEntity', () => console.log('engCreateEntity fired!'));
  edict_t* eng_pfnCreateEntity () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCreateEntity", nullptr, 0);
    return nullptr;
  }

// nodemod.on('engRemoveEntity', (e) => console.log('engRemoveEntity fired!'));
  void eng_pfnRemoveEntity (edict_t* e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRemoveEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCreateNamedEntity', (className) => console.log('engCreateNamedEntity fired!'));
  edict_t* eng_pfnCreateNamedEntity (int className) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCreateNamedEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, className); // className (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engMakeStatic', (ent) => console.log('engMakeStatic fired!'));
  void eng_pfnMakeStatic (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engMakeStatic", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engEntIsOnFloor', (e) => console.log('engEntIsOnFloor fired!'));
  int eng_pfnEntIsOnFloor (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEntIsOnFloor", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engDropToFloor', (e) => console.log('engDropToFloor fired!'));
  int eng_pfnDropToFloor (edict_t* e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDropToFloor", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engWalkMove', (ent, yaw, dist, iMode) => console.log('engWalkMove fired!'));
  int eng_pfnWalkMove (edict_t * ent, float yaw, float dist, int iMode) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWalkMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = v8::Number::New(isolate, yaw); // yaw (float)
      v8_args[2] = v8::Number::New(isolate, dist); // dist (float)
      v8_args[3] = v8::Number::New(isolate, iMode); // iMode (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engSetOrigin', (e, rgflOrigin) => console.log('engSetOrigin fired!'));
  void eng_pfnSetOrigin (edict_t * e, const float * rgflOrigin) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetOrigin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engEmitSound', (entity, channel, sample, volume, attenuation, fFlags, pitch) => console.log('engEmitSound fired!'));
  void eng_pfnEmitSound (edict_t * entity, int channel, const char * sample, float volume, float attenuation, int fFlags, int pitch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEmitSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = v8::Number::New(isolate, channel); // channel (int)
      v8_args[2] = v8::String::NewFromUtf8(isolate, sample ? sample : "").ToLocalChecked(); // sample (const char *)
      v8_args[3] = v8::Number::New(isolate, volume); // volume (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engEmitAmbientSound', (entity, pos, samp, vol, attenuation, fFlags, pitch) => console.log('engEmitAmbientSound fired!'));
  void eng_pfnEmitAmbientSound (edict_t * entity, const float * pos, const char * samp, float vol, float attenuation, int fFlags, int pitch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEmitAmbientSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, pos, 3); // pos (const float *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, samp ? samp : "").ToLocalChecked(); // samp (const char *)
      v8_args[3] = v8::Number::New(isolate, vol); // vol (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTraceLine', (v1, v2, fNoMonsters, pentToSkip, ptr) => console.log('engTraceLine fired!'));
  void eng_pfnTraceLine (const float * v1, const float * v2, int fNoMonsters, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceLine", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[4] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTraceToss', (pent, pentToIgnore, ptr) => console.log('engTraceToss fired!'));
  void eng_pfnTraceToss (edict_t* pent, edict_t* pentToIgnore, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceToss", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t*)
      v8_args[1] = structures::wrapEntity(isolate, pentToIgnore); // pentToIgnore (edict_t*)
      v8_args[2] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTraceMonsterHull', (pEdict, v1, v2, fNoMonsters, pentToSkip, ptr) => console.log('engTraceMonsterHull fired!'));
  int eng_pfnTraceMonsterHull (edict_t * pEdict, const float * v1, const float * v2, int fNoMonsters, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceMonsterHull", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[3] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engTraceHull', (v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr) => console.log('engTraceHull fired!'));
  void eng_pfnTraceHull (const float * v1, const float * v2, int fNoMonsters, int hullNumber, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceHull", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = v8::Number::New(isolate, hullNumber); // hullNumber (int)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTraceModel', (v1, v2, hullNumber, pent, ptr) => console.log('engTraceModel fired!'));
  void eng_pfnTraceModel (const float * v1, const float * v2, int hullNumber, edict_t * pent, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, hullNumber); // hullNumber (int)
      v8_args[3] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[4] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTraceTexture', (pTextureEntity, v1, v2) => console.log('engTraceTexture fired!'));
  const char * eng_pfnTraceTexture (edict_t * pTextureEntity, const float * v1, const float * v2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceTexture", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pTextureEntity); // pTextureEntity (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engTraceSphere', (v1, v2, fNoMonsters, radius, pentToSkip, ptr) => console.log('engTraceSphere fired!'));
  void eng_pfnTraceSphere (const float * v1, const float * v2, int fNoMonsters, float radius, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTraceSphere", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = v8::Number::New(isolate, radius); // radius (float)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetAimVector', (ent, speed, rgflReturn) => console.log('engGetAimVector fired!'));
  void eng_pfnGetAimVector (edict_t * ent, float speed, float * rgflReturn) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetAimVector", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = v8::Number::New(isolate, speed); // speed (float)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflReturn, 3); // rgflReturn (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engServerCommand', (str) => console.log('engServerCommand fired!'));
  void eng_pfnServerCommand (const char * str) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engServerCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, str ? str : "").ToLocalChecked(); // str (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engServerExecute', () => console.log('engServerExecute fired!'));
  void eng_pfnServerExecute () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engServerExecute", nullptr, 0);
  }

// nodemod.on('engClientCommand', (pEdict, szFmt) => console.log('engClientCommand fired!'));
  void eng_pfnClientCommand (edict_t* ed, const char *szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engClientCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);
  v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGS()).ToLocalChecked();
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engParticleEffect', (org, dir, color, count) => console.log('engParticleEffect fired!'));
  void eng_pfnParticleEffect (const float * org, const float * dir, float color, float count) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engParticleEffect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, dir, 3); // dir (const float *)
      v8_args[2] = v8::Number::New(isolate, color); // color (float)
      v8_args[3] = v8::Number::New(isolate, count); // count (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engLightStyle', (style, val) => console.log('engLightStyle fired!'));
  void eng_pfnLightStyle (int style, const char * val) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engLightStyle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, style); // style (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, val ? val : "").ToLocalChecked(); // val (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engDecalIndex', (name) => console.log('engDecalIndex fired!'));
  int eng_pfnDecalIndex (const char * name) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDecalIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name ? name : "").ToLocalChecked(); // name (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engPointContents', (rgflVector) => console.log('engPointContents fired!'));
  int eng_pfnPointContents (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPointContents", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engMessageBegin', (msg_dest, msg_type, pOrigin, ed) => console.log('engMessageBegin fired!'));
  void eng_pfnMessageBegin (int msg_dest, int msg_type, const float * pOrigin, edict_t * ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engMessageBegin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, msg_dest); // msg_dest (int)
      v8_args[1] = v8::Number::New(isolate, msg_type); // msg_type (int)
      v8_args[2] = utils::floatArrayToJS(isolate, pOrigin, 3); // pOrigin (const float *)
      v8_args[3] = structures::wrapEntity(isolate, ed); // ed (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engMessageEnd', () => console.log('engMessageEnd fired!'));
  void eng_pfnMessageEnd () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engMessageEnd", nullptr, 0);
  }

// nodemod.on('engWriteByte', (iValue) => console.log('engWriteByte fired!'));
  void eng_pfnWriteByte (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteByte", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteChar', (iValue) => console.log('engWriteChar fired!'));
  void eng_pfnWriteChar (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteChar", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteShort', (iValue) => console.log('engWriteShort fired!'));
  void eng_pfnWriteShort (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteShort", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteLong', (iValue) => console.log('engWriteLong fired!'));
  void eng_pfnWriteLong (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteLong", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteAngle', (flValue) => console.log('engWriteAngle fired!'));
  void eng_pfnWriteAngle (float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteAngle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteCoord', (flValue) => console.log('engWriteCoord fired!'));
  void eng_pfnWriteCoord (float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteCoord", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteString', (sz) => console.log('engWriteString fired!'));
  void eng_pfnWriteString (const char * sz) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, sz ? sz : "").ToLocalChecked(); // sz (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engWriteEntity', (iValue) => console.log('engWriteEntity fired!'));
  void eng_pfnWriteEntity (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engWriteEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCVarRegister', (pCvar) => console.log('engCVarRegister fired!'));
  void eng_pfnCVarRegister (cvar_t * pCvar) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarRegister", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapCvar(isolate, pCvar); // pCvar (cvar_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCVarGetFloat', (szVarName) => console.log('engCVarGetFloat fired!'));
  float eng_pfnCVarGetFloat (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarGetFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('engCVarGetString', (szVarName) => console.log('engCVarGetString fired!'));
  const char* eng_pfnCVarGetString (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarGetString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engCVarSetFloat', (szVarName, flValue) => console.log('engCVarSetFloat fired!'));
  void eng_pfnCVarSetFloat (const char * szVarName, float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarSetFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      v8_args[1] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCVarSetString', (szVarName, szValue) => console.log('engCVarSetString fired!'));
  void eng_pfnCVarSetString (const char * szVarName, const char * szValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarSetString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szValue ? szValue : "").ToLocalChecked(); // szValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engAlertMessage', (atype, szFmt) => console.log('engAlertMessage fired!'));
  void eng_pfnAlertMessage (ALERT_TYPE atype, const char * szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engAlertMessage", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, atype); // atype (ALERT_TYPE)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szFmt ? szFmt : "").ToLocalChecked(); // szFmt (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engEngineFprintf', (pfile, szFmt) => console.log('engEngineFprintf fired!'));
  void eng_pfnEngineFprintf (FILE * pfile, const char * szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEngineFprintf", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, pfile); // pfile (FILE *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szFmt ? szFmt : "").ToLocalChecked(); // szFmt (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engPvAllocEntPrivateData', (pEdict, cb) => console.log('engPvAllocEntPrivateData fired!'));
  void* eng_pfnPvAllocEntPrivateData (edict_t * pEdict, int cb) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPvAllocEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      v8_args[1] = v8::Number::New(isolate, cb); // cb (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engPvEntPrivateData', (pEdict) => console.log('engPvEntPrivateData fired!'));
  void* eng_pfnPvEntPrivateData (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPvEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engFreeEntPrivateData', (pEdict) => console.log('engFreeEntPrivateData fired!'));
  void eng_pfnFreeEntPrivateData (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFreeEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSzFromIndex', (iString) => console.log('engSzFromIndex fired!'));
  const char * eng_pfnSzFromIndex (int iString) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSzFromIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iString); // iString (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engAllocString', (szValue) => console.log('engAllocString fired!'));
  int eng_pfnAllocString (const char * szValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engAllocString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szValue ? szValue : "").ToLocalChecked(); // szValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engGetVarsOfEnt', (pEdict) => console.log('engGetVarsOfEnt fired!'));
  struct entvars_s * eng_pfnGetVarsOfEnt (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetVarsOfEnt", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engPEntityOfEntOffset', (iEntOffset) => console.log('engPEntityOfEntOffset fired!'));
  edict_t* eng_pfnPEntityOfEntOffset (int iEntOffset) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPEntityOfEntOffset", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntOffset); // iEntOffset (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engEntOffsetOfPEntity', (pEdict) => console.log('engEntOffsetOfPEntity fired!'));
  int eng_pfnEntOffsetOfPEntity (const edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEntOffsetOfPEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engIndexOfEdict', (pEdict) => console.log('engIndexOfEdict fired!'));
  int eng_pfnIndexOfEdict (const edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engIndexOfEdict", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engPEntityOfEntIndex', (iEntIndex) => console.log('engPEntityOfEntIndex fired!'));
  edict_t* eng_pfnPEntityOfEntIndex (int iEntIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPEntityOfEntIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntIndex); // iEntIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engFindEntityByVars', (pvars) => console.log('engFindEntityByVars fired!'));
  edict_t* eng_pfnFindEntityByVars (struct entvars_s* pvars) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFindEntityByVars", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntvars(isolate, pvars); // pvars (struct entvars_s*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engGetModelPtr', (pEdict) => console.log('engGetModelPtr fired!'));
  void* eng_pfnGetModelPtr (edict_t* pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetModelPtr", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engRegUserMsg', (pszName, iSize) => console.log('engRegUserMsg fired!'));
  int eng_pfnRegUserMsg (const char * pszName, int iSize) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRegUserMsg", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pszName ? pszName : "").ToLocalChecked(); // pszName (const char *)
      v8_args[1] = v8::Number::New(isolate, iSize); // iSize (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engAnimationAutomove', (pEdict, flTime) => console.log('engAnimationAutomove fired!'));
  void eng_pfnAnimationAutomove (const edict_t* pEdict, float flTime) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engAnimationAutomove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t*)
      v8_args[1] = v8::Number::New(isolate, flTime); // flTime (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetBonePosition', (pEdict, iBone, rgflOrigin, rgflAngles) => console.log('engGetBonePosition fired!'));
  void eng_pfnGetBonePosition (const edict_t* pEdict, int iBone, float * rgflOrigin, float * rgflAngles) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetBonePosition", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t*)
      v8_args[1] = v8::Number::New(isolate, iBone); // iBone (int)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, rgflAngles, 3); // rgflAngles (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engFunctionFromName', (pName) => console.log('engFunctionFromName fired!'));
  void* eng_pfnFunctionFromName (const char * pName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFunctionFromName", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pName ? pName : "").ToLocalChecked(); // pName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engNameForFunction', (function) => console.log('engNameForFunction fired!'));
  const char * eng_pfnNameForFunction (void * function) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engNameForFunction", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, function); // function (void *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engClientPrintf', (pEdict, ptype, szMsg) => console.log('engClientPrintf fired!'));
  void eng_pfnClientPrintf (edict_t* pEdict, PRINT_TYPE ptype, const char * szMsg) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engClientPrintf", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t*)
      v8_args[1] = v8::Number::New(isolate, ptype); // ptype (PRINT_TYPE)
      v8_args[2] = v8::String::NewFromUtf8(isolate, szMsg ? szMsg : "").ToLocalChecked(); // szMsg (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engServerPrint', (szMsg) => console.log('engServerPrint fired!'));
  void eng_pfnServerPrint (const char * szMsg) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engServerPrint", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szMsg ? szMsg : "").ToLocalChecked(); // szMsg (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCmdArgs', () => console.log('engCmdArgs fired!'));
  const char * eng_pfnCmd_Args () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCmdArgs", nullptr, 0);
    return nullptr;
  }

// nodemod.on('engCmdArgv', (argc) => console.log('engCmdArgv fired!'));
  const char * eng_pfnCmd_Argv (int argc) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCmdArgv", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, argc); // argc (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engCmdArgc', () => console.log('engCmdArgc fired!'));
  int eng_pfnCmd_Argc () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCmdArgc", nullptr, 0);
    return 0;
  }

// nodemod.on('engGetAttachment', (pEdict, iAttachment, rgflOrigin, rgflAngles) => console.log('engGetAttachment fired!'));
  void eng_pfnGetAttachment (const edict_t * pEdict, int iAttachment, float * rgflOrigin, float * rgflAngles) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetAttachment", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, iAttachment); // iAttachment (int)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, rgflAngles, 3); // rgflAngles (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCRC32Init', (pulCRC) => console.log('engCRC32Init fired!'));
  void eng_pfnCRC32_Init (CRC32_t * pulCRC) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCRC32Init", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCRC32ProcessBuffer', (pulCRC, p, len) => console.log('engCRC32ProcessBuffer fired!'));
  void eng_pfnCRC32_ProcessBuffer (CRC32_t * pulCRC, void * p, int len) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCRC32ProcessBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      v8_args[1] = v8::External::New(isolate, p); // p (void *)
      v8_args[2] = v8::Number::New(isolate, len); // len (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCRC32ProcessByte', (pulCRC, ch) => console.log('engCRC32ProcessByte fired!'));
  void eng_pfnCRC32_ProcessByte (CRC32_t * pulCRC, unsigned char ch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCRC32ProcessByte", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      v8_args[1] = v8::Number::New(isolate, ch); // ch (unsigned char)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCRC32Final', (pulCRC) => console.log('engCRC32Final fired!'));
  CRC32_t eng_pfnCRC32_Final (CRC32_t pulCRC) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCRC32Final", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, pulCRC); // pulCRC (CRC32_t)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engRandomLong', (lLow, lHigh) => console.log('engRandomLong fired!'));
  int eng_pfnRandomLong (int lLow, int lHigh) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRandomLong", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, lLow); // lLow (int)
      v8_args[1] = v8::Number::New(isolate, lHigh); // lHigh (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engRandomFloat', (flLow, flHigh) => console.log('engRandomFloat fired!'));
  float eng_pfnRandomFloat (float flLow, float flHigh) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRandomFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, flLow); // flLow (float)
      v8_args[1] = v8::Number::New(isolate, flHigh); // flHigh (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('engSetView', (pClient, pViewent) => console.log('engSetView fired!'));
  void eng_pfnSetView (const edict_t * pClient, const edict_t * pViewent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetView", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pViewent); // pViewent (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engTime', () => console.log('engTime fired!'));
  float eng_pfnTime () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engTime", nullptr, 0);
    return 0.0f;
  }

// nodemod.on('engCrosshairAngle', (pClient, pitch, yaw) => console.log('engCrosshairAngle fired!'));
  void eng_pfnCrosshairAngle (const edict_t * pClient, float pitch, float yaw) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCrosshairAngle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, pitch); // pitch (float)
      v8_args[2] = v8::Number::New(isolate, yaw); // yaw (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engLoadFileForMe', (filename, pLength) => console.log('engLoadFileForMe fired!'));
  byte* eng_pfnLoadFileForMe (const char * filename, int * pLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engLoadFileForMe", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      v8_args[1] = utils::intArrayToJS(isolate, pLength, 1); // pLength (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engFreeFile', (buffer) => console.log('engFreeFile fired!'));
  void eng_pfnFreeFile (void * buffer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFreeFile", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, buffer); // buffer (void *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engEndSection', (pszSectionName) => console.log('engEndSection fired!'));
  void eng_pfnEndSection (const char * pszSectionName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engEndSection", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pszSectionName ? pszSectionName : "").ToLocalChecked(); // pszSectionName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCompareFileTime', (filename1, filename2, iCompare) => console.log('engCompareFileTime fired!'));
  int eng_pfnCompareFileTime (char * filename1, char * filename2, int * iCompare) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCompareFileTime", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename1 ? filename1 : "").ToLocalChecked(); // filename1 (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, filename2 ? filename2 : "").ToLocalChecked(); // filename2 (char *)
      v8_args[2] = utils::intArrayToJS(isolate, iCompare, 1); // iCompare (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engGetGameDir', (szGetGameDir) => console.log('engGetGameDir fired!'));
  void eng_pfnGetGameDir (char * szGetGameDir) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetGameDir", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szGetGameDir ? szGetGameDir : "").ToLocalChecked(); // szGetGameDir (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCvarRegisterVariable', (variable) => console.log('engCvarRegisterVariable fired!'));
  void eng_pfnCvar_RegisterVariable (cvar_t * variable) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCvarRegisterVariable", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapCvar(isolate, variable); // variable (cvar_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engFadeClientVolume', (pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds) => console.log('engFadeClientVolume fired!'));
  void eng_pfnFadeClientVolume (const edict_t * pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engFadeClientVolume", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, fadePercent); // fadePercent (int)
      v8_args[2] = v8::Number::New(isolate, fadeOutSeconds); // fadeOutSeconds (int)
      v8_args[3] = v8::Number::New(isolate, holdTime); // holdTime (int)
      v8_args[4] = v8::Number::New(isolate, fadeInSeconds); // fadeInSeconds (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSetClientMaxspeed', (pEdict, fNewMaxspeed) => console.log('engSetClientMaxspeed fired!'));
  void eng_pfnSetClientMaxspeed (const edict_t * pEdict, float fNewMaxspeed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetClientMaxspeed", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, fNewMaxspeed); // fNewMaxspeed (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCreateFakeClient', (netname) => console.log('engCreateFakeClient fired!'));
  edict_t * eng_pfnCreateFakeClient (const char * netname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCreateFakeClient", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, netname ? netname : "").ToLocalChecked(); // netname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engRunPlayerMove', (fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec) => console.log('engRunPlayerMove fired!'));
  void eng_pfnRunPlayerMove (edict_t * fakeclient, const float * viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRunPlayerMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 8;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[8];
      v8_args[0] = structures::wrapEntity(isolate, fakeclient); // fakeclient (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, viewangles, 3); // viewangles (const float *)
      v8_args[2] = v8::Number::New(isolate, forwardmove); // forwardmove (float)
      v8_args[3] = v8::Number::New(isolate, sidemove); // sidemove (float)
      v8_args[4] = v8::Number::New(isolate, upmove); // upmove (float)
      v8_args[5] = v8::Number::New(isolate, buttons); // buttons (unsigned short)
      v8_args[6] = v8::Number::New(isolate, impulse); // impulse (byte)
      v8_args[7] = v8::Number::New(isolate, msec); // msec (byte)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engNumberOfEntities', () => console.log('engNumberOfEntities fired!'));
  int eng_pfnNumberOfEntities () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engNumberOfEntities", nullptr, 0);
    return 0;
  }

// nodemod.on('engGetInfoKeyBuffer', (e) => console.log('engGetInfoKeyBuffer fired!'));
  char* eng_pfnGetInfoKeyBuffer (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetInfoKeyBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engInfoKeyValue', (infobuffer, key) => console.log('engInfoKeyValue fired!'));
  char* eng_pfnInfoKeyValue (char * infobuffer, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engInfoKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engSetKeyValue', (infobuffer, key, value) => console.log('engSetKeyValue fired!'));
  void eng_pfnSetKeyValue (char * infobuffer, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSetClientKeyValue', (clientIndex, infobuffer, key, value) => console.log('engSetClientKeyValue fired!'));
  void eng_pfnSetClientKeyValue (int clientIndex, char * infobuffer, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetClientKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, clientIndex); // clientIndex (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engIsMapValid', (filename) => console.log('engIsMapValid fired!'));
  int eng_pfnIsMapValid (const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engIsMapValid", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engStaticDecal', (origin, decalIndex, entityIndex, modelIndex) => console.log('engStaticDecal fired!'));
  void eng_pfnStaticDecal (const float * origin, int decalIndex, int entityIndex, int modelIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engStaticDecal", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, origin, 3); // origin (const float *)
      v8_args[1] = v8::Number::New(isolate, decalIndex); // decalIndex (int)
      v8_args[2] = v8::Number::New(isolate, entityIndex); // entityIndex (int)
      v8_args[3] = v8::Number::New(isolate, modelIndex); // modelIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engPrecacheGeneric', (s) => console.log('engPrecacheGeneric fired!'));
  int eng_pfnPrecacheGeneric (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPrecacheGeneric", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engGetPlayerUserId', (e) => console.log('engGetPlayerUserId fired!'));
  int eng_pfnGetPlayerUserId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPlayerUserId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engBuildSoundMsg', (entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed) => console.log('engBuildSoundMsg fired!'));
  void eng_pfnBuildSoundMsg (edict_t * entity, int channel, const char * sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float * pOrigin, edict_t * ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engBuildSoundMsg", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 11;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[11];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = v8::Number::New(isolate, channel); // channel (int)
      v8_args[2] = v8::String::NewFromUtf8(isolate, sample ? sample : "").ToLocalChecked(); // sample (const char *)
      v8_args[3] = v8::Number::New(isolate, volume); // volume (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      v8_args[7] = v8::Number::New(isolate, msg_dest); // msg_dest (int)
      v8_args[8] = v8::Number::New(isolate, msg_type); // msg_type (int)
      v8_args[9] = utils::floatArrayToJS(isolate, pOrigin, 3); // pOrigin (const float *)
      v8_args[10] = structures::wrapEntity(isolate, ed); // ed (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engIsDedicatedServer', () => console.log('engIsDedicatedServer fired!'));
  int eng_pfnIsDedicatedServer () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engIsDedicatedServer", nullptr, 0);
    return 0;
  }

// nodemod.on('engCVarGetPointer', (szVarName) => console.log('engCVarGetPointer fired!'));
  cvar_t * eng_pfnCVarGetPointer (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCVarGetPointer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engGetPlayerWONId', (e) => console.log('engGetPlayerWONId fired!'));
  unsigned int eng_pfnGetPlayerWONId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPlayerWONId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engInfoRemoveKey', (s, key) => console.log('engInfoRemoveKey fired!'));
  void eng_pfnInfo_RemoveKey (char * s, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engInfoRemoveKey", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetPhysicsKeyValue', (pClient, key) => console.log('engGetPhysicsKeyValue fired!'));
  const char * eng_pfnGetPhysicsKeyValue (const edict_t * pClient, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPhysicsKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engSetPhysicsKeyValue', (pClient, key, value) => console.log('engSetPhysicsKeyValue fired!'));
  void eng_pfnSetPhysicsKeyValue (const edict_t * pClient, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetPhysicsKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetPhysicsInfoString', (pClient) => console.log('engGetPhysicsInfoString fired!'));
  const char * eng_pfnGetPhysicsInfoString (const edict_t * pClient) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPhysicsInfoString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engPrecacheEvent', (type, psz) => console.log('engPrecacheEvent fired!'));
  unsigned short eng_pfnPrecacheEvent (int type, const char* psz) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPrecacheEvent", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, type); // type (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, psz ? psz : "").ToLocalChecked(); // psz (const char*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engPlaybackEvent', (flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2) => console.log('engPlaybackEvent fired!'));
  void eng_pfnPlaybackEvent (int flags, const edict_t * pInvoker, unsigned short eventindex, float delay, const float * origin, const float * angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPlaybackEvent", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 12;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[12];
      v8_args[0] = v8::Number::New(isolate, flags); // flags (int)
      v8_args[1] = structures::wrapEntity(isolate, pInvoker); // pInvoker (const edict_t *)
      v8_args[2] = v8::Number::New(isolate, eventindex); // eventindex (unsigned short)
      v8_args[3] = v8::Number::New(isolate, delay); // delay (float)
      v8_args[4] = utils::floatArrayToJS(isolate, origin, 3); // origin (const float *)
      v8_args[5] = utils::floatArrayToJS(isolate, angles, 3); // angles (const float *)
      v8_args[6] = v8::Number::New(isolate, fparam1); // fparam1 (float)
      v8_args[7] = v8::Number::New(isolate, fparam2); // fparam2 (float)
      v8_args[8] = v8::Number::New(isolate, iparam1); // iparam1 (int)
      v8_args[9] = v8::Number::New(isolate, iparam2); // iparam2 (int)
      v8_args[10] = v8::Number::New(isolate, bparam1); // bparam1 (int)
      v8_args[11] = v8::Number::New(isolate, bparam2); // bparam2 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSetFatPVS', (org) => console.log('engSetFatPVS fired!'));
  unsigned char * eng_pfnSetFatPVS (const float * org) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetFatPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engSetFatPAS', (org) => console.log('engSetFatPAS fired!'));
  unsigned char * eng_pfnSetFatPAS (const float * org) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetFatPAS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engCheckVisibility', (entity, pset) => console.log('engCheckVisibility fired!'));
  int eng_pfnCheckVisibility (const edict_t * entity, unsigned char * pset) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCheckVisibility", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (const edict_t *)
      v8_args[1] = utils::byteArrayToJS(isolate, pset, 1); // pset (unsigned char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engDeltaSetField', (pFields, fieldname) => console.log('engDeltaSetField fired!'));
  void eng_pfnDeltaSetField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaSetField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engDeltaUnsetField', (pFields, fieldname) => console.log('engDeltaUnsetField fired!'));
  void eng_pfnDeltaUnsetField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaUnsetField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engDeltaAddEncoder', (name, value1) => console.log('engDeltaAddEncoder fired!'));
  void eng_pfnDeltaAddEncoder (const char * name, void* value1) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaAddEncoder", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name ? name : "").ToLocalChecked(); // name (const char *)
      v8_args[1] = v8::External::New(isolate, value1); // value1 (void*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetCurrentPlayer', () => console.log('engGetCurrentPlayer fired!'));
  int eng_pfnGetCurrentPlayer () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetCurrentPlayer", nullptr, 0);
    return 0;
  }

// nodemod.on('engCanSkipPlayer', (player) => console.log('engCanSkipPlayer fired!'));
  int eng_pfnCanSkipPlayer (const edict_t * player) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCanSkipPlayer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engDeltaFindField', (pFields, fieldname) => console.log('engDeltaFindField fired!'));
  int eng_pfnDeltaFindField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaFindField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engDeltaSetFieldByIndex', (pFields, fieldNumber) => console.log('engDeltaSetFieldByIndex fired!'));
  void eng_pfnDeltaSetFieldByIndex (struct delta_s * pFields, int fieldNumber) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaSetFieldByIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::Number::New(isolate, fieldNumber); // fieldNumber (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engDeltaUnsetFieldByIndex', (pFields, fieldNumber) => console.log('engDeltaUnsetFieldByIndex fired!'));
  void eng_pfnDeltaUnsetFieldByIndex (struct delta_s * pFields, int fieldNumber) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engDeltaUnsetFieldByIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::Number::New(isolate, fieldNumber); // fieldNumber (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engSetGroupMask', (mask, op) => console.log('engSetGroupMask fired!'));
  void eng_pfnSetGroupMask (int mask, int op) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSetGroupMask", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, mask); // mask (int)
      v8_args[1] = v8::Number::New(isolate, op); // op (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCreateInstancedBaseline', (classname, baseline) => console.log('engCreateInstancedBaseline fired!'));
  int eng_pfnCreateInstancedBaseline (int classname, struct entity_state_s * baseline) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCreateInstancedBaseline", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, classname); // classname (int)
      v8_args[1] = structures::wrapEntityState(isolate, baseline); // baseline (struct entity_state_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engCvarDirectSet', (var, value) => console.log('engCvarDirectSet fired!'));
  void eng_pfnCvar_DirectSet (struct cvar_s * var, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCvarDirectSet", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapCvar(isolate, var); // var (struct cvar_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engForceUnmodified', (type, mins, maxs, filename) => console.log('engForceUnmodified fired!'));
  void eng_pfnForceUnmodified (FORCE_TYPE type, const float * mins, const float * maxs, const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engForceUnmodified", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, type); // type (FORCE_TYPE)
      v8_args[1] = utils::floatArrayToJS(isolate, mins, 3); // mins (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, maxs, 3); // maxs (const float *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetPlayerStats', (pClient, ping, packet_loss) => console.log('engGetPlayerStats fired!'));
  void eng_pfnGetPlayerStats (const edict_t * pClient, int * ping, int * packet_loss) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPlayerStats", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = utils::intArrayToJS(isolate, ping, 1); // ping (int *)
      v8_args[2] = utils::intArrayToJS(isolate, packet_loss, 1); // packet_loss (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engAddServerCommand', (cmd_name, value1) => console.log('engAddServerCommand fired!'));
  void eng_pfnAddServerCommand (const char * cmd_name, void* value1) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engAddServerCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, cmd_name ? cmd_name : "").ToLocalChecked(); // cmd_name (const char *)
      v8_args[1] = v8::External::New(isolate, value1); // value1 (void*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engVoiceGetClientListening', (iReceiver, iSender) => console.log('engVoiceGetClientListening fired!'));
  qboolean eng_pfnVoice_GetClientListening (int iReceiver, int iSender) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engVoiceGetClientListening", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, iReceiver); // iReceiver (int)
      v8_args[1] = v8::Number::New(isolate, iSender); // iSender (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engVoiceSetClientListening', (iReceiver, iSender, bListen) => console.log('engVoiceSetClientListening fired!'));
  qboolean eng_pfnVoice_SetClientListening (int iReceiver, int iSender, qboolean bListen) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engVoiceSetClientListening", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::Number::New(isolate, iReceiver); // iReceiver (int)
      v8_args[1] = v8::Number::New(isolate, iSender); // iSender (int)
      v8_args[2] = v8::Boolean::New(isolate, bListen); // bListen (qboolean)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engGetPlayerAuthId', (e) => console.log('engGetPlayerAuthId fired!'));
  const char * eng_pfnGetPlayerAuthId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetPlayerAuthId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engSequenceGet', (fileName, entryName) => console.log('engSequenceGet fired!'));
  void * eng_pfnSequenceGet (const char * fileName, const char * entryName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSequenceGet", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, fileName ? fileName : "").ToLocalChecked(); // fileName (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, entryName ? entryName : "").ToLocalChecked(); // entryName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engSequencePickSentence', (groupName, pickMethod, picked) => console.log('engSequencePickSentence fired!'));
  void * eng_pfnSequencePickSentence (const char * groupName, int pickMethod, int * picked) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engSequencePickSentence", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, groupName ? groupName : "").ToLocalChecked(); // groupName (const char *)
      v8_args[1] = v8::Number::New(isolate, pickMethod); // pickMethod (int)
      v8_args[2] = utils::intArrayToJS(isolate, picked, 1); // picked (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('engGetFileSize', (filename) => console.log('engGetFileSize fired!'));
  int eng_pfnGetFileSize (const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetFileSize", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engGetApproxWavePlayLen', (filepath) => console.log('engGetApproxWavePlayLen fired!'));
  unsigned int eng_pfnGetApproxWavePlayLen (const char * filepath) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetApproxWavePlayLen", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filepath ? filepath : "").ToLocalChecked(); // filepath (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engIsCareerMatch', () => console.log('engIsCareerMatch fired!'));
  int eng_pfnIsCareerMatch () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engIsCareerMatch", nullptr, 0);
    return 0;
  }

// nodemod.on('engGetLocalizedStringLength', (label) => console.log('engGetLocalizedStringLength fired!'));
  int eng_pfnGetLocalizedStringLength (const char * label) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetLocalizedStringLength", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, label ? label : "").ToLocalChecked(); // label (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engRegisterTutorMessageShown', (mid) => console.log('engRegisterTutorMessageShown fired!'));
  void eng_pfnRegisterTutorMessageShown (int mid) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engRegisterTutorMessageShown", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, mid); // mid (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engGetTimesTutorMessageShown', (mid) => console.log('engGetTimesTutorMessageShown fired!'));
  int eng_pfnGetTimesTutorMessageShown (int mid) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engGetTimesTutorMessageShown", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, mid); // mid (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engProcessTutorMessageDecayBuffer', (buffer) => console.log('engProcessTutorMessageDecayBuffer fired!'));
  void eng_pfnProcessTutorMessageDecayBuffer (int * buffer, int bufferLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engProcessTutorMessageDecayBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::intArrayToJS(isolate, buffer, bufferLength); // buffer (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engConstructTutorMessageDecayBuffer', (buffer) => console.log('engConstructTutorMessageDecayBuffer fired!'));
  void eng_pfnConstructTutorMessageDecayBuffer (int * buffer, int bufferLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engConstructTutorMessageDecayBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::intArrayToJS(isolate, buffer, bufferLength); // buffer (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engResetTutorMessageDecayData', () => console.log('engResetTutorMessageDecayData fired!'));
  void eng_pfnResetTutorMessageDecayData () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engResetTutorMessageDecayData", nullptr, 0);
  }

// nodemod.on('engQueryClientCvarValue', (player, cvarName) => console.log('engQueryClientCvarValue fired!'));
  void eng_pfnQueryClientCvarValue (const edict_t * player, const char * cvarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engQueryClientCvarValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, cvarName ? cvarName : "").ToLocalChecked(); // cvarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engQueryClientCvarValue2', (player, cvarName, requestID) => console.log('engQueryClientCvarValue2 fired!'));
  void eng_pfnQueryClientCvarValue2 (const edict_t * player, const char * cvarName, int requestID) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engQueryClientCvarValue2", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, cvarName ? cvarName : "").ToLocalChecked(); // cvarName (const char *)
      v8_args[2] = v8::Number::New(isolate, requestID); // requestID (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('engCheckParm', (parm, ppnext) => console.log('engCheckParm fired!'));
  int eng_CheckParm (char * parm, char ** ppnext) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engCheckParm", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, parm ? parm : "").ToLocalChecked(); // parm (char *)
      v8_args[1] = utils::stringArrayToJS(isolate, ppnext); // ppnext (char **)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('engPEntityOfEntIndexAllEntities', (iEntIndex) => console.log('engPEntityOfEntIndexAllEntities fired!'));
  edict_t* eng_pfnPEntityOfEntIndexAllEntities (int iEntIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("engPEntityOfEntIndexAllEntities", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntIndex); // iEntIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

    enginefuncs_t g_EngineFunctionsTable = {
      eng_pfnPrecacheModel,
      eng_pfnPrecacheSound,
      eng_pfnSetModel,
      eng_pfnModelIndex,
      eng_pfnModelFrames,
      eng_pfnSetSize,
      eng_pfnChangeLevel,
      eng_pfnGetSpawnParms,
      eng_pfnSaveSpawnParms,
      eng_pfnVecToYaw,
      eng_pfnVecToAngles,
      eng_pfnMoveToOrigin,
      eng_pfnChangeYaw,
      eng_pfnChangePitch,
      eng_pfnFindEntityByString,
      eng_pfnGetEntityIllum,
      eng_pfnFindEntityInSphere,
      eng_pfnFindClientInPVS,
      eng_pfnEntitiesInPVS,
      eng_pfnMakeVectors,
      eng_pfnAngleVectors,
      eng_pfnCreateEntity,
      eng_pfnRemoveEntity,
      eng_pfnCreateNamedEntity,
      eng_pfnMakeStatic,
      eng_pfnEntIsOnFloor,
      eng_pfnDropToFloor,
      eng_pfnWalkMove,
      eng_pfnSetOrigin,
      eng_pfnEmitSound,
      eng_pfnEmitAmbientSound,
      eng_pfnTraceLine,
      eng_pfnTraceToss,
      eng_pfnTraceMonsterHull,
      eng_pfnTraceHull,
      eng_pfnTraceModel,
      eng_pfnTraceTexture,
      eng_pfnTraceSphere,
      eng_pfnGetAimVector,
      eng_pfnServerCommand,
      eng_pfnServerExecute,
      eng_pfnClientCommand,
      eng_pfnParticleEffect,
      eng_pfnLightStyle,
      eng_pfnDecalIndex,
      eng_pfnPointContents,
      eng_pfnMessageBegin,
      eng_pfnMessageEnd,
      eng_pfnWriteByte,
      eng_pfnWriteChar,
      eng_pfnWriteShort,
      eng_pfnWriteLong,
      eng_pfnWriteAngle,
      eng_pfnWriteCoord,
      eng_pfnWriteString,
      eng_pfnWriteEntity,
      eng_pfnCVarRegister,
      eng_pfnCVarGetFloat,
      eng_pfnCVarGetString,
      eng_pfnCVarSetFloat,
      eng_pfnCVarSetString,
      eng_pfnAlertMessage,
      eng_pfnEngineFprintf,
      eng_pfnPvAllocEntPrivateData,
      eng_pfnPvEntPrivateData,
      eng_pfnFreeEntPrivateData,
      eng_pfnSzFromIndex,
      eng_pfnAllocString,
      eng_pfnGetVarsOfEnt,
      eng_pfnPEntityOfEntOffset,
      eng_pfnEntOffsetOfPEntity,
      eng_pfnIndexOfEdict,
      eng_pfnPEntityOfEntIndex,
      eng_pfnFindEntityByVars,
      eng_pfnGetModelPtr,
      eng_pfnRegUserMsg,
      eng_pfnAnimationAutomove,
      eng_pfnGetBonePosition,
      eng_pfnFunctionFromName,
      eng_pfnNameForFunction,
      eng_pfnClientPrintf,
      eng_pfnServerPrint,
      eng_pfnCmd_Args,
      eng_pfnCmd_Argv,
      eng_pfnCmd_Argc,
      eng_pfnGetAttachment,
      eng_pfnCRC32_Init,
      eng_pfnCRC32_ProcessBuffer,
      eng_pfnCRC32_ProcessByte,
      eng_pfnCRC32_Final,
      eng_pfnRandomLong,
      eng_pfnRandomFloat,
      eng_pfnSetView,
      eng_pfnTime,
      eng_pfnCrosshairAngle,
      eng_pfnLoadFileForMe,
      eng_pfnFreeFile,
      eng_pfnEndSection,
      eng_pfnCompareFileTime,
      eng_pfnGetGameDir,
      eng_pfnCvar_RegisterVariable,
      eng_pfnFadeClientVolume,
      eng_pfnSetClientMaxspeed,
      eng_pfnCreateFakeClient,
      eng_pfnRunPlayerMove,
      eng_pfnNumberOfEntities,
      eng_pfnGetInfoKeyBuffer,
      eng_pfnInfoKeyValue,
      eng_pfnSetKeyValue,
      eng_pfnSetClientKeyValue,
      eng_pfnIsMapValid,
      eng_pfnStaticDecal,
      eng_pfnPrecacheGeneric,
      eng_pfnGetPlayerUserId,
      eng_pfnBuildSoundMsg,
      eng_pfnIsDedicatedServer,
      eng_pfnCVarGetPointer,
      eng_pfnGetPlayerWONId,
      eng_pfnInfo_RemoveKey,
      eng_pfnGetPhysicsKeyValue,
      eng_pfnSetPhysicsKeyValue,
      eng_pfnGetPhysicsInfoString,
      eng_pfnPrecacheEvent,
      eng_pfnPlaybackEvent,
      eng_pfnSetFatPVS,
      eng_pfnSetFatPAS,
      eng_pfnCheckVisibility,
      eng_pfnDeltaSetField,
      eng_pfnDeltaUnsetField,
      eng_pfnDeltaAddEncoder,
      eng_pfnGetCurrentPlayer,
      eng_pfnCanSkipPlayer,
      eng_pfnDeltaFindField,
      eng_pfnDeltaSetFieldByIndex,
      eng_pfnDeltaUnsetFieldByIndex,
      eng_pfnSetGroupMask,
      eng_pfnCreateInstancedBaseline,
      eng_pfnCvar_DirectSet,
      eng_pfnForceUnmodified,
      eng_pfnGetPlayerStats,
      eng_pfnAddServerCommand,
      eng_pfnVoice_GetClientListening,
      eng_pfnVoice_SetClientListening,
      eng_pfnGetPlayerAuthId,
      eng_pfnSequenceGet,
      eng_pfnSequencePickSentence,
      eng_pfnGetFileSize,
      eng_pfnGetApproxWavePlayLen,
      eng_pfnIsCareerMatch,
      eng_pfnGetLocalizedStringLength,
      eng_pfnRegisterTutorMessageShown,
      eng_pfnGetTimesTutorMessageShown,
      eng_pfnProcessTutorMessageDecayBuffer,
      eng_pfnConstructTutorMessageDecayBuffer,
      eng_pfnResetTutorMessageDecayData,
      eng_pfnQueryClientCvarValue,
      eng_pfnQueryClientCvarValue2,
      eng_CheckParm,
      eng_pfnPEntityOfEntIndexAllEntities
    };

  /* POST EVENTS */
    // nodemod.on('postEngPrecacheModel', (s) => console.log('postEngPrecacheModel fired!'));
  int postEng_pfnPrecacheModel (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPrecacheModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngPrecacheSound', (s) => console.log('postEngPrecacheSound fired!'));
  int postEng_pfnPrecacheSound (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPrecacheSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngSetModel', (e, m) => console.log('postEngSetModel fired!'));
  void postEng_pfnSetModel (edict_t * e, const char * m) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, m ? m : "").ToLocalChecked(); // m (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngModelIndex', (m) => console.log('postEngModelIndex fired!'));
  int postEng_pfnModelIndex (const char * m) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngModelIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, m ? m : "").ToLocalChecked(); // m (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngModelFrames', (modelIndex) => console.log('postEngModelFrames fired!'));
  int postEng_pfnModelFrames (int modelIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngModelFrames", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, modelIndex); // modelIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngSetSize', (e, rgflMin, rgflMax) => console.log('postEngSetSize fired!'));
  void postEng_pfnSetSize (edict_t * e, const float * rgflMin, const float * rgflMax) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetSize", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflMin, 3); // rgflMin (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflMax, 3); // rgflMax (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngChangeLevel', (s1, s2) => console.log('postEngChangeLevel fired!'));
  void postEng_pfnChangeLevel (const char * s1, const char * s2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngChangeLevel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s1 ? s1 : "").ToLocalChecked(); // s1 (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, s2 ? s2 : "").ToLocalChecked(); // s2 (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetSpawnParms', (ent) => console.log('postEngGetSpawnParms fired!'));
  void postEng_pfnGetSpawnParms (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetSpawnParms", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSaveSpawnParms', (ent) => console.log('postEngSaveSpawnParms fired!'));
  void postEng_pfnSaveSpawnParms (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSaveSpawnParms", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngVecToYaw', (rgflVector) => console.log('postEngVecToYaw fired!'));
  float postEng_pfnVecToYaw (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngVecToYaw", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('postEngVecToAngles', (rgflVectorIn, rgflVectorOut) => console.log('postEngVecToAngles fired!'));
  void postEng_pfnVecToAngles (const float * rgflVectorIn, float * rgflVectorOut) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngVecToAngles", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVectorIn, 3); // rgflVectorIn (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflVectorOut, 3); // rgflVectorOut (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngMoveToOrigin', (ent, pflGoal, dist, iMoveType) => console.log('postEngMoveToOrigin fired!'));
  void postEng_pfnMoveToOrigin (edict_t * ent, const float * pflGoal, float dist, int iMoveType) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngMoveToOrigin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, pflGoal, 3); // pflGoal (const float *)
      v8_args[2] = v8::Number::New(isolate, dist); // dist (float)
      v8_args[3] = v8::Number::New(isolate, iMoveType); // iMoveType (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngChangeYaw', (ent) => console.log('postEngChangeYaw fired!'));
  void postEng_pfnChangeYaw (edict_t* ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngChangeYaw", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngChangePitch', (ent) => console.log('postEngChangePitch fired!'));
  void postEng_pfnChangePitch (edict_t* ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngChangePitch", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngFindEntityByString', (pEdictStartSearchAfter, pszField, pszValue) => console.log('postEngFindEntityByString fired!'));
  edict_t* postEng_pfnFindEntityByString (edict_t * pEdictStartSearchAfter, const char * pszField, const char * pszValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFindEntityByString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictStartSearchAfter); // pEdictStartSearchAfter (edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, pszField ? pszField : "").ToLocalChecked(); // pszField (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, pszValue ? pszValue : "").ToLocalChecked(); // pszValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngGetEntityIllum', (pEnt) => console.log('postEngGetEntityIllum fired!'));
  int postEng_pfnGetEntityIllum (edict_t* pEnt) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetEntityIllum", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEnt); // pEnt (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngFindEntityInSphere', (pEdictStartSearchAfter, org, rad) => console.log('postEngFindEntityInSphere fired!'));
  edict_t* postEng_pfnFindEntityInSphere (edict_t * pEdictStartSearchAfter, const float * org, float rad) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFindEntityInSphere", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdictStartSearchAfter); // pEdictStartSearchAfter (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      v8_args[2] = v8::Number::New(isolate, rad); // rad (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngFindClientInPVS', (pEdict) => console.log('postEngFindClientInPVS fired!'));
  edict_t* postEng_pfnFindClientInPVS (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFindClientInPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngEntitiesInPVS', (pplayer) => console.log('postEngEntitiesInPVS fired!'));
  edict_t* postEng_pfnEntitiesInPVS (edict_t * pplayer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEntitiesInPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pplayer); // pplayer (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngMakeVectors', (rgflVector) => console.log('postEngMakeVectors fired!'));
  void postEng_pfnMakeVectors (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngMakeVectors", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngAngleVectors', (rgflVector, forward, right, up) => console.log('postEngAngleVectors fired!'));
  void postEng_pfnAngleVectors (const float * rgflVector, float * forward, float * right, float * up) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngAngleVectors", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, forward, 3); // forward (float *)
      v8_args[2] = utils::floatArrayToJS(isolate, right, 3); // right (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, up, 3); // up (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCreateEntity', () => console.log('postEngCreateEntity fired!'));
  edict_t* postEng_pfnCreateEntity () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCreateEntity", nullptr, 0);
    return nullptr;
  }

// nodemod.on('postEngRemoveEntity', (e) => console.log('postEngRemoveEntity fired!'));
  void postEng_pfnRemoveEntity (edict_t* e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRemoveEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCreateNamedEntity', (className) => console.log('postEngCreateNamedEntity fired!'));
  edict_t* postEng_pfnCreateNamedEntity (int className) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCreateNamedEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, className); // className (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngMakeStatic', (ent) => console.log('postEngMakeStatic fired!'));
  void postEng_pfnMakeStatic (edict_t * ent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngMakeStatic", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngEntIsOnFloor', (e) => console.log('postEngEntIsOnFloor fired!'));
  int postEng_pfnEntIsOnFloor (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEntIsOnFloor", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngDropToFloor', (e) => console.log('postEngDropToFloor fired!'));
  int postEng_pfnDropToFloor (edict_t* e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDropToFloor", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngWalkMove', (ent, yaw, dist, iMode) => console.log('postEngWalkMove fired!'));
  int postEng_pfnWalkMove (edict_t * ent, float yaw, float dist, int iMode) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWalkMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = v8::Number::New(isolate, yaw); // yaw (float)
      v8_args[2] = v8::Number::New(isolate, dist); // dist (float)
      v8_args[3] = v8::Number::New(isolate, iMode); // iMode (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngSetOrigin', (e, rgflOrigin) => console.log('postEngSetOrigin fired!'));
  void postEng_pfnSetOrigin (edict_t * e, const float * rgflOrigin) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetOrigin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngEmitSound', (entity, channel, sample, volume, attenuation, fFlags, pitch) => console.log('postEngEmitSound fired!'));
  void postEng_pfnEmitSound (edict_t * entity, int channel, const char * sample, float volume, float attenuation, int fFlags, int pitch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEmitSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = v8::Number::New(isolate, channel); // channel (int)
      v8_args[2] = v8::String::NewFromUtf8(isolate, sample ? sample : "").ToLocalChecked(); // sample (const char *)
      v8_args[3] = v8::Number::New(isolate, volume); // volume (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngEmitAmbientSound', (entity, pos, samp, vol, attenuation, fFlags, pitch) => console.log('postEngEmitAmbientSound fired!'));
  void postEng_pfnEmitAmbientSound (edict_t * entity, const float * pos, const char * samp, float vol, float attenuation, int fFlags, int pitch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEmitAmbientSound", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 7;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[7];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, pos, 3); // pos (const float *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, samp ? samp : "").ToLocalChecked(); // samp (const char *)
      v8_args[3] = v8::Number::New(isolate, vol); // vol (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTraceLine', (v1, v2, fNoMonsters, pentToSkip, ptr) => console.log('postEngTraceLine fired!'));
  void postEng_pfnTraceLine (const float * v1, const float * v2, int fNoMonsters, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceLine", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[4] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTraceToss', (pent, pentToIgnore, ptr) => console.log('postEngTraceToss fired!'));
  void postEng_pfnTraceToss (edict_t* pent, edict_t* pentToIgnore, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceToss", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pent); // pent (edict_t*)
      v8_args[1] = structures::wrapEntity(isolate, pentToIgnore); // pentToIgnore (edict_t*)
      v8_args[2] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTraceMonsterHull', (pEdict, v1, v2, fNoMonsters, pentToSkip, ptr) => console.log('postEngTraceMonsterHull fired!'));
  int postEng_pfnTraceMonsterHull (edict_t * pEdict, const float * v1, const float * v2, int fNoMonsters, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceMonsterHull", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[3] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngTraceHull', (v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr) => console.log('postEngTraceHull fired!'));
  void postEng_pfnTraceHull (const float * v1, const float * v2, int fNoMonsters, int hullNumber, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceHull", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = v8::Number::New(isolate, hullNumber); // hullNumber (int)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTraceModel', (v1, v2, hullNumber, pent, ptr) => console.log('postEngTraceModel fired!'));
  void postEng_pfnTraceModel (const float * v1, const float * v2, int hullNumber, edict_t * pent, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceModel", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, hullNumber); // hullNumber (int)
      v8_args[3] = structures::wrapEntity(isolate, pent); // pent (edict_t *)
      v8_args[4] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTraceTexture', (pTextureEntity, v1, v2) => console.log('postEngTraceTexture fired!'));
  const char * postEng_pfnTraceTexture (edict_t * pTextureEntity, const float * v1, const float * v2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceTexture", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pTextureEntity); // pTextureEntity (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngTraceSphere', (v1, v2, fNoMonsters, radius, pentToSkip, ptr) => console.log('postEngTraceSphere fired!'));
  void postEng_pfnTraceSphere (const float * v1, const float * v2, int fNoMonsters, float radius, edict_t * pentToSkip, TraceResult * ptr) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTraceSphere", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 6;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[6];
      v8_args[0] = utils::floatArrayToJS(isolate, v1, 3); // v1 (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, v2, 3); // v2 (const float *)
      v8_args[2] = v8::Number::New(isolate, fNoMonsters); // fNoMonsters (int)
      v8_args[3] = v8::Number::New(isolate, radius); // radius (float)
      v8_args[4] = structures::wrapEntity(isolate, pentToSkip); // pentToSkip (edict_t *)
      v8_args[5] = structures::wrapTraceResult(isolate, ptr); // ptr (TraceResult *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetAimVector', (ent, speed, rgflReturn) => console.log('postEngGetAimVector fired!'));
  void postEng_pfnGetAimVector (edict_t * ent, float speed, float * rgflReturn) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetAimVector", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, ent); // ent (edict_t *)
      v8_args[1] = v8::Number::New(isolate, speed); // speed (float)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflReturn, 3); // rgflReturn (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngServerCommand', (str) => console.log('postEngServerCommand fired!'));
  void postEng_pfnServerCommand (const char * str) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngServerCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, str ? str : "").ToLocalChecked(); // str (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngServerExecute', () => console.log('postEngServerExecute fired!'));
  void postEng_pfnServerExecute () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngServerExecute", nullptr, 0);
  }

// nodemod.on('postEngClientCommand', (pEdict, szFmt) => console.log('postEngClientCommand fired!'));
  void postEng_pfnClientCommand (edict_t* ed, const char *szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngClientCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);
  v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGS()).ToLocalChecked();
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngParticleEffect', (org, dir, color, count) => console.log('postEngParticleEffect fired!'));
  void postEng_pfnParticleEffect (const float * org, const float * dir, float color, float count) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngParticleEffect", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      v8_args[1] = utils::floatArrayToJS(isolate, dir, 3); // dir (const float *)
      v8_args[2] = v8::Number::New(isolate, color); // color (float)
      v8_args[3] = v8::Number::New(isolate, count); // count (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngLightStyle', (style, val) => console.log('postEngLightStyle fired!'));
  void postEng_pfnLightStyle (int style, const char * val) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngLightStyle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, style); // style (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, val ? val : "").ToLocalChecked(); // val (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngDecalIndex', (name) => console.log('postEngDecalIndex fired!'));
  int postEng_pfnDecalIndex (const char * name) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDecalIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name ? name : "").ToLocalChecked(); // name (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngPointContents', (rgflVector) => console.log('postEngPointContents fired!'));
  int postEng_pfnPointContents (const float * rgflVector) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPointContents", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, rgflVector, 3); // rgflVector (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngMessageBegin', (msg_dest, msg_type, pOrigin, ed) => console.log('postEngMessageBegin fired!'));
  void postEng_pfnMessageBegin (int msg_dest, int msg_type, const float * pOrigin, edict_t * ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngMessageBegin", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, msg_dest); // msg_dest (int)
      v8_args[1] = v8::Number::New(isolate, msg_type); // msg_type (int)
      v8_args[2] = utils::floatArrayToJS(isolate, pOrigin, 3); // pOrigin (const float *)
      v8_args[3] = structures::wrapEntity(isolate, ed); // ed (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngMessageEnd', () => console.log('postEngMessageEnd fired!'));
  void postEng_pfnMessageEnd () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngMessageEnd", nullptr, 0);
  }

// nodemod.on('postEngWriteByte', (iValue) => console.log('postEngWriteByte fired!'));
  void postEng_pfnWriteByte (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteByte", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteChar', (iValue) => console.log('postEngWriteChar fired!'));
  void postEng_pfnWriteChar (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteChar", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteShort', (iValue) => console.log('postEngWriteShort fired!'));
  void postEng_pfnWriteShort (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteShort", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteLong', (iValue) => console.log('postEngWriteLong fired!'));
  void postEng_pfnWriteLong (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteLong", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteAngle', (flValue) => console.log('postEngWriteAngle fired!'));
  void postEng_pfnWriteAngle (float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteAngle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteCoord', (flValue) => console.log('postEngWriteCoord fired!'));
  void postEng_pfnWriteCoord (float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteCoord", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteString', (sz) => console.log('postEngWriteString fired!'));
  void postEng_pfnWriteString (const char * sz) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, sz ? sz : "").ToLocalChecked(); // sz (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngWriteEntity', (iValue) => console.log('postEngWriteEntity fired!'));
  void postEng_pfnWriteEntity (int iValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngWriteEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iValue); // iValue (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCVarRegister', (pCvar) => console.log('postEngCVarRegister fired!'));
  void postEng_pfnCVarRegister (cvar_t * pCvar) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarRegister", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapCvar(isolate, pCvar); // pCvar (cvar_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCVarGetFloat', (szVarName) => console.log('postEngCVarGetFloat fired!'));
  float postEng_pfnCVarGetFloat (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarGetFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('postEngCVarGetString', (szVarName) => console.log('postEngCVarGetString fired!'));
  const char* postEng_pfnCVarGetString (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarGetString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngCVarSetFloat', (szVarName, flValue) => console.log('postEngCVarSetFloat fired!'));
  void postEng_pfnCVarSetFloat (const char * szVarName, float flValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarSetFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      v8_args[1] = v8::Number::New(isolate, flValue); // flValue (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCVarSetString', (szVarName, szValue) => console.log('postEngCVarSetString fired!'));
  void postEng_pfnCVarSetString (const char * szVarName, const char * szValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarSetString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szValue ? szValue : "").ToLocalChecked(); // szValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngAlertMessage', (atype, szFmt) => console.log('postEngAlertMessage fired!'));
  void postEng_pfnAlertMessage (ALERT_TYPE atype, const char * szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngAlertMessage", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, atype); // atype (ALERT_TYPE)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szFmt ? szFmt : "").ToLocalChecked(); // szFmt (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngEngineFprintf', (pfile, szFmt) => console.log('postEngEngineFprintf fired!'));
  void postEng_pfnEngineFprintf (FILE * pfile, const char * szFmt, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEngineFprintf", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, pfile); // pfile (FILE *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, szFmt ? szFmt : "").ToLocalChecked(); // szFmt (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngPvAllocEntPrivateData', (pEdict, cb) => console.log('postEngPvAllocEntPrivateData fired!'));
  void* postEng_pfnPvAllocEntPrivateData (edict_t * pEdict, int cb) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPvAllocEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      v8_args[1] = v8::Number::New(isolate, cb); // cb (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngPvEntPrivateData', (pEdict) => console.log('postEngPvEntPrivateData fired!'));
  void* postEng_pfnPvEntPrivateData (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPvEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngFreeEntPrivateData', (pEdict) => console.log('postEngFreeEntPrivateData fired!'));
  void postEng_pfnFreeEntPrivateData (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFreeEntPrivateData", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSzFromIndex', (iString) => console.log('postEngSzFromIndex fired!'));
  const char * postEng_pfnSzFromIndex (int iString) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSzFromIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iString); // iString (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngAllocString', (szValue) => console.log('postEngAllocString fired!'));
  int postEng_pfnAllocString (const char * szValue) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngAllocString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szValue ? szValue : "").ToLocalChecked(); // szValue (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngGetVarsOfEnt', (pEdict) => console.log('postEngGetVarsOfEnt fired!'));
  struct entvars_s * postEng_pfnGetVarsOfEnt (edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetVarsOfEnt", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngPEntityOfEntOffset', (iEntOffset) => console.log('postEngPEntityOfEntOffset fired!'));
  edict_t* postEng_pfnPEntityOfEntOffset (int iEntOffset) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPEntityOfEntOffset", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntOffset); // iEntOffset (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngEntOffsetOfPEntity', (pEdict) => console.log('postEngEntOffsetOfPEntity fired!'));
  int postEng_pfnEntOffsetOfPEntity (const edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEntOffsetOfPEntity", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngIndexOfEdict', (pEdict) => console.log('postEngIndexOfEdict fired!'));
  int postEng_pfnIndexOfEdict (const edict_t * pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngIndexOfEdict", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngPEntityOfEntIndex', (iEntIndex) => console.log('postEngPEntityOfEntIndex fired!'));
  edict_t* postEng_pfnPEntityOfEntIndex (int iEntIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPEntityOfEntIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntIndex); // iEntIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngFindEntityByVars', (pvars) => console.log('postEngFindEntityByVars fired!'));
  edict_t* postEng_pfnFindEntityByVars (struct entvars_s* pvars) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFindEntityByVars", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntvars(isolate, pvars); // pvars (struct entvars_s*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngGetModelPtr', (pEdict) => console.log('postEngGetModelPtr fired!'));
  void* postEng_pfnGetModelPtr (edict_t* pEdict) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetModelPtr", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngRegUserMsg', (pszName, iSize) => console.log('postEngRegUserMsg fired!'));
  int postEng_pfnRegUserMsg (const char * pszName, int iSize) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRegUserMsg", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pszName ? pszName : "").ToLocalChecked(); // pszName (const char *)
      v8_args[1] = v8::Number::New(isolate, iSize); // iSize (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngAnimationAutomove', (pEdict, flTime) => console.log('postEngAnimationAutomove fired!'));
  void postEng_pfnAnimationAutomove (const edict_t* pEdict, float flTime) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngAnimationAutomove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t*)
      v8_args[1] = v8::Number::New(isolate, flTime); // flTime (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetBonePosition', (pEdict, iBone, rgflOrigin, rgflAngles) => console.log('postEngGetBonePosition fired!'));
  void postEng_pfnGetBonePosition (const edict_t* pEdict, int iBone, float * rgflOrigin, float * rgflAngles) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetBonePosition", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t*)
      v8_args[1] = v8::Number::New(isolate, iBone); // iBone (int)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, rgflAngles, 3); // rgflAngles (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngFunctionFromName', (pName) => console.log('postEngFunctionFromName fired!'));
  void* postEng_pfnFunctionFromName (const char * pName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFunctionFromName", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pName ? pName : "").ToLocalChecked(); // pName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngNameForFunction', (function) => console.log('postEngNameForFunction fired!'));
  const char * postEng_pfnNameForFunction (void * function) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngNameForFunction", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, function); // function (void *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngClientPrintf', (pEdict, ptype, szMsg) => console.log('postEngClientPrintf fired!'));
  void postEng_pfnClientPrintf (edict_t* pEdict, PRINT_TYPE ptype, const char * szMsg) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngClientPrintf", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (edict_t*)
      v8_args[1] = v8::Number::New(isolate, ptype); // ptype (PRINT_TYPE)
      v8_args[2] = v8::String::NewFromUtf8(isolate, szMsg ? szMsg : "").ToLocalChecked(); // szMsg (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngServerPrint', (szMsg) => console.log('postEngServerPrint fired!'));
  void postEng_pfnServerPrint (const char * szMsg) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngServerPrint", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szMsg ? szMsg : "").ToLocalChecked(); // szMsg (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCmdArgs', () => console.log('postEngCmdArgs fired!'));
  const char * postEng_pfnCmd_Args () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCmdArgs", nullptr, 0);
    return nullptr;
  }

// nodemod.on('postEngCmdArgv', (argc) => console.log('postEngCmdArgv fired!'));
  const char * postEng_pfnCmd_Argv (int argc) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCmdArgv", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, argc); // argc (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngCmdArgc', () => console.log('postEngCmdArgc fired!'));
  int postEng_pfnCmd_Argc () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCmdArgc", nullptr, 0);
    return 0;
  }

// nodemod.on('postEngGetAttachment', (pEdict, iAttachment, rgflOrigin, rgflAngles) => console.log('postEngGetAttachment fired!'));
  void postEng_pfnGetAttachment (const edict_t * pEdict, int iAttachment, float * rgflOrigin, float * rgflAngles) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetAttachment", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, iAttachment); // iAttachment (int)
      v8_args[2] = utils::floatArrayToJS(isolate, rgflOrigin, 3); // rgflOrigin (float *)
      v8_args[3] = utils::floatArrayToJS(isolate, rgflAngles, 3); // rgflAngles (float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCRC32Init', (pulCRC) => console.log('postEngCRC32Init fired!'));
  void postEng_pfnCRC32_Init (CRC32_t * pulCRC) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCRC32Init", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCRC32ProcessBuffer', (pulCRC, p, len) => console.log('postEngCRC32ProcessBuffer fired!'));
  void postEng_pfnCRC32_ProcessBuffer (CRC32_t * pulCRC, void * p, int len) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCRC32ProcessBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      v8_args[1] = v8::External::New(isolate, p); // p (void *)
      v8_args[2] = v8::Number::New(isolate, len); // len (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCRC32ProcessByte', (pulCRC, ch) => console.log('postEngCRC32ProcessByte fired!'));
  void postEng_pfnCRC32_ProcessByte (CRC32_t * pulCRC, unsigned char ch) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCRC32ProcessByte", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::External::New(isolate, pulCRC); // pulCRC (CRC32_t *)
      v8_args[1] = v8::Number::New(isolate, ch); // ch (unsigned char)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCRC32Final', (pulCRC) => console.log('postEngCRC32Final fired!'));
  CRC32_t postEng_pfnCRC32_Final (CRC32_t pulCRC) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCRC32Final", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, pulCRC); // pulCRC (CRC32_t)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngRandomLong', (lLow, lHigh) => console.log('postEngRandomLong fired!'));
  int postEng_pfnRandomLong (int lLow, int lHigh) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRandomLong", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, lLow); // lLow (int)
      v8_args[1] = v8::Number::New(isolate, lHigh); // lHigh (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngRandomFloat', (flLow, flHigh) => console.log('postEngRandomFloat fired!'));
  float postEng_pfnRandomFloat (float flLow, float flHigh) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRandomFloat", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, flLow); // flLow (float)
      v8_args[1] = v8::Number::New(isolate, flHigh); // flHigh (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0.0f;
  }

// nodemod.on('postEngSetView', (pClient, pViewent) => console.log('postEngSetView fired!'));
  void postEng_pfnSetView (const edict_t * pClient, const edict_t * pViewent) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetView", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = structures::wrapEntity(isolate, pViewent); // pViewent (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngTime', () => console.log('postEngTime fired!'));
  float postEng_pfnTime () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngTime", nullptr, 0);
    return 0.0f;
  }

// nodemod.on('postEngCrosshairAngle', (pClient, pitch, yaw) => console.log('postEngCrosshairAngle fired!'));
  void postEng_pfnCrosshairAngle (const edict_t * pClient, float pitch, float yaw) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCrosshairAngle", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, pitch); // pitch (float)
      v8_args[2] = v8::Number::New(isolate, yaw); // yaw (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngLoadFileForMe', (filename, pLength) => console.log('postEngLoadFileForMe fired!'));
  byte* postEng_pfnLoadFileForMe (const char * filename, int * pLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngLoadFileForMe", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      v8_args[1] = utils::intArrayToJS(isolate, pLength, 1); // pLength (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngFreeFile', (buffer) => console.log('postEngFreeFile fired!'));
  void postEng_pfnFreeFile (void * buffer) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFreeFile", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::External::New(isolate, buffer); // buffer (void *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngEndSection', (pszSectionName) => console.log('postEngEndSection fired!'));
  void postEng_pfnEndSection (const char * pszSectionName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngEndSection", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, pszSectionName ? pszSectionName : "").ToLocalChecked(); // pszSectionName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCompareFileTime', (filename1, filename2, iCompare) => console.log('postEngCompareFileTime fired!'));
  int postEng_pfnCompareFileTime (char * filename1, char * filename2, int * iCompare) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCompareFileTime", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename1 ? filename1 : "").ToLocalChecked(); // filename1 (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, filename2 ? filename2 : "").ToLocalChecked(); // filename2 (char *)
      v8_args[2] = utils::intArrayToJS(isolate, iCompare, 1); // iCompare (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngGetGameDir', (szGetGameDir) => console.log('postEngGetGameDir fired!'));
  void postEng_pfnGetGameDir (char * szGetGameDir) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetGameDir", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szGetGameDir ? szGetGameDir : "").ToLocalChecked(); // szGetGameDir (char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCvarRegisterVariable', (variable) => console.log('postEngCvarRegisterVariable fired!'));
  void postEng_pfnCvar_RegisterVariable (cvar_t * variable) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCvarRegisterVariable", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapCvar(isolate, variable); // variable (cvar_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngFadeClientVolume', (pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds) => console.log('postEngFadeClientVolume fired!'));
  void postEng_pfnFadeClientVolume (const edict_t * pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngFadeClientVolume", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 5;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[5];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, fadePercent); // fadePercent (int)
      v8_args[2] = v8::Number::New(isolate, fadeOutSeconds); // fadeOutSeconds (int)
      v8_args[3] = v8::Number::New(isolate, holdTime); // holdTime (int)
      v8_args[4] = v8::Number::New(isolate, fadeInSeconds); // fadeInSeconds (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSetClientMaxspeed', (pEdict, fNewMaxspeed) => console.log('postEngSetClientMaxspeed fired!'));
  void postEng_pfnSetClientMaxspeed (const edict_t * pEdict, float fNewMaxspeed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetClientMaxspeed", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pEdict); // pEdict (const edict_t *)
      v8_args[1] = v8::Number::New(isolate, fNewMaxspeed); // fNewMaxspeed (float)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCreateFakeClient', (netname) => console.log('postEngCreateFakeClient fired!'));
  edict_t * postEng_pfnCreateFakeClient (const char * netname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCreateFakeClient", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, netname ? netname : "").ToLocalChecked(); // netname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngRunPlayerMove', (fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec) => console.log('postEngRunPlayerMove fired!'));
  void postEng_pfnRunPlayerMove (edict_t * fakeclient, const float * viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRunPlayerMove", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 8;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[8];
      v8_args[0] = structures::wrapEntity(isolate, fakeclient); // fakeclient (edict_t *)
      v8_args[1] = utils::floatArrayToJS(isolate, viewangles, 3); // viewangles (const float *)
      v8_args[2] = v8::Number::New(isolate, forwardmove); // forwardmove (float)
      v8_args[3] = v8::Number::New(isolate, sidemove); // sidemove (float)
      v8_args[4] = v8::Number::New(isolate, upmove); // upmove (float)
      v8_args[5] = v8::Number::New(isolate, buttons); // buttons (unsigned short)
      v8_args[6] = v8::Number::New(isolate, impulse); // impulse (byte)
      v8_args[7] = v8::Number::New(isolate, msec); // msec (byte)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngNumberOfEntities', () => console.log('postEngNumberOfEntities fired!'));
  int postEng_pfnNumberOfEntities () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngNumberOfEntities", nullptr, 0);
    return 0;
  }

// nodemod.on('postEngGetInfoKeyBuffer', (e) => console.log('postEngGetInfoKeyBuffer fired!'));
  char* postEng_pfnGetInfoKeyBuffer (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetInfoKeyBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngInfoKeyValue', (infobuffer, key) => console.log('postEngInfoKeyValue fired!'));
  char* postEng_pfnInfoKeyValue (char * infobuffer, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngInfoKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngSetKeyValue', (infobuffer, key, value) => console.log('postEngSetKeyValue fired!'));
  void postEng_pfnSetKeyValue (char * infobuffer, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSetClientKeyValue', (clientIndex, infobuffer, key, value) => console.log('postEngSetClientKeyValue fired!'));
  void postEng_pfnSetClientKeyValue (int clientIndex, char * infobuffer, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetClientKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, clientIndex); // clientIndex (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, infobuffer ? infobuffer : "").ToLocalChecked(); // infobuffer (char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngIsMapValid', (filename) => console.log('postEngIsMapValid fired!'));
  int postEng_pfnIsMapValid (const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngIsMapValid", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngStaticDecal', (origin, decalIndex, entityIndex, modelIndex) => console.log('postEngStaticDecal fired!'));
  void postEng_pfnStaticDecal (const float * origin, int decalIndex, int entityIndex, int modelIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngStaticDecal", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = utils::floatArrayToJS(isolate, origin, 3); // origin (const float *)
      v8_args[1] = v8::Number::New(isolate, decalIndex); // decalIndex (int)
      v8_args[2] = v8::Number::New(isolate, entityIndex); // entityIndex (int)
      v8_args[3] = v8::Number::New(isolate, modelIndex); // modelIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngPrecacheGeneric', (s) => console.log('postEngPrecacheGeneric fired!'));
  int postEng_pfnPrecacheGeneric (const char * s) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPrecacheGeneric", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngGetPlayerUserId', (e) => console.log('postEngGetPlayerUserId fired!'));
  int postEng_pfnGetPlayerUserId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPlayerUserId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngBuildSoundMsg', (entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed) => console.log('postEngBuildSoundMsg fired!'));
  void postEng_pfnBuildSoundMsg (edict_t * entity, int channel, const char * sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float * pOrigin, edict_t * ed) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngBuildSoundMsg", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 11;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[11];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (edict_t *)
      v8_args[1] = v8::Number::New(isolate, channel); // channel (int)
      v8_args[2] = v8::String::NewFromUtf8(isolate, sample ? sample : "").ToLocalChecked(); // sample (const char *)
      v8_args[3] = v8::Number::New(isolate, volume); // volume (float)
      v8_args[4] = v8::Number::New(isolate, attenuation); // attenuation (float)
      v8_args[5] = v8::Number::New(isolate, fFlags); // fFlags (int)
      v8_args[6] = v8::Number::New(isolate, pitch); // pitch (int)
      v8_args[7] = v8::Number::New(isolate, msg_dest); // msg_dest (int)
      v8_args[8] = v8::Number::New(isolate, msg_type); // msg_type (int)
      v8_args[9] = utils::floatArrayToJS(isolate, pOrigin, 3); // pOrigin (const float *)
      v8_args[10] = structures::wrapEntity(isolate, ed); // ed (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngIsDedicatedServer', () => console.log('postEngIsDedicatedServer fired!'));
  int postEng_pfnIsDedicatedServer () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngIsDedicatedServer", nullptr, 0);
    return 0;
  }

// nodemod.on('postEngCVarGetPointer', (szVarName) => console.log('postEngCVarGetPointer fired!'));
  cvar_t * postEng_pfnCVarGetPointer (const char * szVarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCVarGetPointer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, szVarName ? szVarName : "").ToLocalChecked(); // szVarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngGetPlayerWONId', (e) => console.log('postEngGetPlayerWONId fired!'));
  unsigned int postEng_pfnGetPlayerWONId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPlayerWONId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngInfoRemoveKey', (s, key) => console.log('postEngInfoRemoveKey fired!'));
  void postEng_pfnInfo_RemoveKey (char * s, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngInfoRemoveKey", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, s ? s : "").ToLocalChecked(); // s (char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetPhysicsKeyValue', (pClient, key) => console.log('postEngGetPhysicsKeyValue fired!'));
  const char * postEng_pfnGetPhysicsKeyValue (const edict_t * pClient, const char * key) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPhysicsKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngSetPhysicsKeyValue', (pClient, key, value) => console.log('postEngSetPhysicsKeyValue fired!'));
  void postEng_pfnSetPhysicsKeyValue (const edict_t * pClient, const char * key, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetPhysicsKeyValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, key ? key : "").ToLocalChecked(); // key (const char *)
      v8_args[2] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetPhysicsInfoString', (pClient) => console.log('postEngGetPhysicsInfoString fired!'));
  const char * postEng_pfnGetPhysicsInfoString (const edict_t * pClient) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPhysicsInfoString", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngPrecacheEvent', (type, psz) => console.log('postEngPrecacheEvent fired!'));
  unsigned short postEng_pfnPrecacheEvent (int type, const char* psz) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPrecacheEvent", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, type); // type (int)
      v8_args[1] = v8::String::NewFromUtf8(isolate, psz ? psz : "").ToLocalChecked(); // psz (const char*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngPlaybackEvent', (flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2) => console.log('postEngPlaybackEvent fired!'));
  void postEng_pfnPlaybackEvent (int flags, const edict_t * pInvoker, unsigned short eventindex, float delay, const float * origin, const float * angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPlaybackEvent", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 12;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[12];
      v8_args[0] = v8::Number::New(isolate, flags); // flags (int)
      v8_args[1] = structures::wrapEntity(isolate, pInvoker); // pInvoker (const edict_t *)
      v8_args[2] = v8::Number::New(isolate, eventindex); // eventindex (unsigned short)
      v8_args[3] = v8::Number::New(isolate, delay); // delay (float)
      v8_args[4] = utils::floatArrayToJS(isolate, origin, 3); // origin (const float *)
      v8_args[5] = utils::floatArrayToJS(isolate, angles, 3); // angles (const float *)
      v8_args[6] = v8::Number::New(isolate, fparam1); // fparam1 (float)
      v8_args[7] = v8::Number::New(isolate, fparam2); // fparam2 (float)
      v8_args[8] = v8::Number::New(isolate, iparam1); // iparam1 (int)
      v8_args[9] = v8::Number::New(isolate, iparam2); // iparam2 (int)
      v8_args[10] = v8::Number::New(isolate, bparam1); // bparam1 (int)
      v8_args[11] = v8::Number::New(isolate, bparam2); // bparam2 (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSetFatPVS', (org) => console.log('postEngSetFatPVS fired!'));
  unsigned char * postEng_pfnSetFatPVS (const float * org) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetFatPVS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngSetFatPAS', (org) => console.log('postEngSetFatPAS fired!'));
  unsigned char * postEng_pfnSetFatPAS (const float * org) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetFatPAS", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::floatArrayToJS(isolate, org, 3); // org (const float *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngCheckVisibility', (entity, pset) => console.log('postEngCheckVisibility fired!'));
  int postEng_pfnCheckVisibility (const edict_t * entity, unsigned char * pset) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCheckVisibility", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, entity); // entity (const edict_t *)
      v8_args[1] = utils::byteArrayToJS(isolate, pset, 1); // pset (unsigned char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngDeltaSetField', (pFields, fieldname) => console.log('postEngDeltaSetField fired!'));
  void postEng_pfnDeltaSetField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaSetField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngDeltaUnsetField', (pFields, fieldname) => console.log('postEngDeltaUnsetField fired!'));
  void postEng_pfnDeltaUnsetField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaUnsetField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngDeltaAddEncoder', (name, value1) => console.log('postEngDeltaAddEncoder fired!'));
  void postEng_pfnDeltaAddEncoder (const char * name, void* value1) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaAddEncoder", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, name ? name : "").ToLocalChecked(); // name (const char *)
      v8_args[1] = v8::External::New(isolate, value1); // value1 (void*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetCurrentPlayer', () => console.log('postEngGetCurrentPlayer fired!'));
  int postEng_pfnGetCurrentPlayer () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetCurrentPlayer", nullptr, 0);
    return 0;
  }

// nodemod.on('postEngCanSkipPlayer', (player) => console.log('postEngCanSkipPlayer fired!'));
  int postEng_pfnCanSkipPlayer (const edict_t * player) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCanSkipPlayer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngDeltaFindField', (pFields, fieldname) => console.log('postEngDeltaFindField fired!'));
  int postEng_pfnDeltaFindField (struct delta_s * pFields, const char * fieldname) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaFindField", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, fieldname ? fieldname : "").ToLocalChecked(); // fieldname (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngDeltaSetFieldByIndex', (pFields, fieldNumber) => console.log('postEngDeltaSetFieldByIndex fired!'));
  void postEng_pfnDeltaSetFieldByIndex (struct delta_s * pFields, int fieldNumber) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaSetFieldByIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::Number::New(isolate, fieldNumber); // fieldNumber (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngDeltaUnsetFieldByIndex', (pFields, fieldNumber) => console.log('postEngDeltaUnsetFieldByIndex fired!'));
  void postEng_pfnDeltaUnsetFieldByIndex (struct delta_s * pFields, int fieldNumber) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngDeltaUnsetFieldByIndex", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapDelta(isolate, pFields); // pFields (struct delta_s *)
      v8_args[1] = v8::Number::New(isolate, fieldNumber); // fieldNumber (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngSetGroupMask', (mask, op) => console.log('postEngSetGroupMask fired!'));
  void postEng_pfnSetGroupMask (int mask, int op) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSetGroupMask", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, mask); // mask (int)
      v8_args[1] = v8::Number::New(isolate, op); // op (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCreateInstancedBaseline', (classname, baseline) => console.log('postEngCreateInstancedBaseline fired!'));
  int postEng_pfnCreateInstancedBaseline (int classname, struct entity_state_s * baseline) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCreateInstancedBaseline", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, classname); // classname (int)
      v8_args[1] = structures::wrapEntityState(isolate, baseline); // baseline (struct entity_state_s *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngCvarDirectSet', (var, value) => console.log('postEngCvarDirectSet fired!'));
  void postEng_pfnCvar_DirectSet (struct cvar_s * var, const char * value) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCvarDirectSet", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapCvar(isolate, var); // var (struct cvar_s *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, value ? value : "").ToLocalChecked(); // value (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngForceUnmodified', (type, mins, maxs, filename) => console.log('postEngForceUnmodified fired!'));
  void postEng_pfnForceUnmodified (FORCE_TYPE type, const float * mins, const float * maxs, const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngForceUnmodified", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 4;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[4];
      v8_args[0] = v8::Number::New(isolate, type); // type (FORCE_TYPE)
      v8_args[1] = utils::floatArrayToJS(isolate, mins, 3); // mins (const float *)
      v8_args[2] = utils::floatArrayToJS(isolate, maxs, 3); // maxs (const float *)
      v8_args[3] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetPlayerStats', (pClient, ping, packet_loss) => console.log('postEngGetPlayerStats fired!'));
  void postEng_pfnGetPlayerStats (const edict_t * pClient, int * ping, int * packet_loss) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPlayerStats", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, pClient); // pClient (const edict_t *)
      v8_args[1] = utils::intArrayToJS(isolate, ping, 1); // ping (int *)
      v8_args[2] = utils::intArrayToJS(isolate, packet_loss, 1); // packet_loss (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngAddServerCommand', (cmd_name, value1) => console.log('postEngAddServerCommand fired!'));
  void postEng_pfnAddServerCommand (const char * cmd_name, void* value1) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngAddServerCommand", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, cmd_name ? cmd_name : "").ToLocalChecked(); // cmd_name (const char *)
      v8_args[1] = v8::External::New(isolate, value1); // value1 (void*)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngVoiceGetClientListening', (iReceiver, iSender) => console.log('postEngVoiceGetClientListening fired!'));
  qboolean postEng_pfnVoice_GetClientListening (int iReceiver, int iSender) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngVoiceGetClientListening", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::Number::New(isolate, iReceiver); // iReceiver (int)
      v8_args[1] = v8::Number::New(isolate, iSender); // iSender (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngVoiceSetClientListening', (iReceiver, iSender, bListen) => console.log('postEngVoiceSetClientListening fired!'));
  qboolean postEng_pfnVoice_SetClientListening (int iReceiver, int iSender, qboolean bListen) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngVoiceSetClientListening", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::Number::New(isolate, iReceiver); // iReceiver (int)
      v8_args[1] = v8::Number::New(isolate, iSender); // iSender (int)
      v8_args[2] = v8::Boolean::New(isolate, bListen); // bListen (qboolean)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngGetPlayerAuthId', (e) => console.log('postEngGetPlayerAuthId fired!'));
  const char * postEng_pfnGetPlayerAuthId (edict_t * e) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetPlayerAuthId", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = structures::wrapEntity(isolate, e); // e (edict_t *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngSequenceGet', (fileName, entryName) => console.log('postEngSequenceGet fired!'));
  void * postEng_pfnSequenceGet (const char * fileName, const char * entryName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSequenceGet", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, fileName ? fileName : "").ToLocalChecked(); // fileName (const char *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, entryName ? entryName : "").ToLocalChecked(); // entryName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngSequencePickSentence', (groupName, pickMethod, picked) => console.log('postEngSequencePickSentence fired!'));
  void * postEng_pfnSequencePickSentence (const char * groupName, int pickMethod, int * picked) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngSequencePickSentence", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = v8::String::NewFromUtf8(isolate, groupName ? groupName : "").ToLocalChecked(); // groupName (const char *)
      v8_args[1] = v8::Number::New(isolate, pickMethod); // pickMethod (int)
      v8_args[2] = utils::intArrayToJS(isolate, picked, 1); // picked (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

// nodemod.on('postEngGetFileSize', (filename) => console.log('postEngGetFileSize fired!'));
  int postEng_pfnGetFileSize (const char * filename) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetFileSize", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filename ? filename : "").ToLocalChecked(); // filename (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngGetApproxWavePlayLen', (filepath) => console.log('postEngGetApproxWavePlayLen fired!'));
  unsigned int postEng_pfnGetApproxWavePlayLen (const char * filepath) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetApproxWavePlayLen", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, filepath ? filepath : "").ToLocalChecked(); // filepath (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngIsCareerMatch', () => console.log('postEngIsCareerMatch fired!'));
  int postEng_pfnIsCareerMatch () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngIsCareerMatch", nullptr, 0);
    return 0;
  }

// nodemod.on('postEngGetLocalizedStringLength', (label) => console.log('postEngGetLocalizedStringLength fired!'));
  int postEng_pfnGetLocalizedStringLength (const char * label) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetLocalizedStringLength", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::String::NewFromUtf8(isolate, label ? label : "").ToLocalChecked(); // label (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngRegisterTutorMessageShown', (mid) => console.log('postEngRegisterTutorMessageShown fired!'));
  void postEng_pfnRegisterTutorMessageShown (int mid) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngRegisterTutorMessageShown", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, mid); // mid (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngGetTimesTutorMessageShown', (mid) => console.log('postEngGetTimesTutorMessageShown fired!'));
  int postEng_pfnGetTimesTutorMessageShown (int mid) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngGetTimesTutorMessageShown", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, mid); // mid (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngProcessTutorMessageDecayBuffer', (buffer) => console.log('postEngProcessTutorMessageDecayBuffer fired!'));
  void postEng_pfnProcessTutorMessageDecayBuffer (int * buffer, int bufferLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngProcessTutorMessageDecayBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::intArrayToJS(isolate, buffer, bufferLength); // buffer (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngConstructTutorMessageDecayBuffer', (buffer) => console.log('postEngConstructTutorMessageDecayBuffer fired!'));
  void postEng_pfnConstructTutorMessageDecayBuffer (int * buffer, int bufferLength) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngConstructTutorMessageDecayBuffer", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = utils::intArrayToJS(isolate, buffer, bufferLength); // buffer (int *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngResetTutorMessageDecayData', () => console.log('postEngResetTutorMessageDecayData fired!'));
  void postEng_pfnResetTutorMessageDecayData () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngResetTutorMessageDecayData", nullptr, 0);
  }

// nodemod.on('postEngQueryClientCvarValue', (player, cvarName) => console.log('postEngQueryClientCvarValue fired!'));
  void postEng_pfnQueryClientCvarValue (const edict_t * player, const char * cvarName) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngQueryClientCvarValue", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, cvarName ? cvarName : "").ToLocalChecked(); // cvarName (const char *)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngQueryClientCvarValue2', (player, cvarName, requestID) => console.log('postEngQueryClientCvarValue2 fired!'));
  void postEng_pfnQueryClientCvarValue2 (const edict_t * player, const char * cvarName, int requestID) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngQueryClientCvarValue2", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 3;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[3];
      v8_args[0] = structures::wrapEntity(isolate, player); // player (const edict_t *)
      v8_args[1] = v8::String::NewFromUtf8(isolate, cvarName ? cvarName : "").ToLocalChecked(); // cvarName (const char *)
      v8_args[2] = v8::Number::New(isolate, requestID); // requestID (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
  }

// nodemod.on('postEngCheckParm', (parm, ppnext) => console.log('postEngCheckParm fired!'));
  int postEng_CheckParm (char * parm, char ** ppnext) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngCheckParm", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 2;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
      v8_args[0] = v8::String::NewFromUtf8(isolate, parm ? parm : "").ToLocalChecked(); // parm (char *)
      v8_args[1] = utils::stringArrayToJS(isolate, ppnext); // ppnext (char **)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return 0;
  }

// nodemod.on('postEngPEntityOfEntIndexAllEntities', (iEntIndex) => console.log('postEngPEntityOfEntIndexAllEntities fired!'));
  edict_t* postEng_pfnPEntityOfEntIndexAllEntities (int iEntIndex) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("postEngPEntityOfEntIndexAllEntities", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = 1;
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[1];
      v8_args[0] = v8::Number::New(isolate, iEntIndex); // iEntIndex (int)
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
    return nullptr;
  }

    enginefuncs_t g_EngineFunctionsTable_Post = {
      postEng_pfnPrecacheModel,
      postEng_pfnPrecacheSound,
      postEng_pfnSetModel,
      postEng_pfnModelIndex,
      postEng_pfnModelFrames,
      postEng_pfnSetSize,
      postEng_pfnChangeLevel,
      postEng_pfnGetSpawnParms,
      postEng_pfnSaveSpawnParms,
      postEng_pfnVecToYaw,
      postEng_pfnVecToAngles,
      postEng_pfnMoveToOrigin,
      postEng_pfnChangeYaw,
      postEng_pfnChangePitch,
      postEng_pfnFindEntityByString,
      postEng_pfnGetEntityIllum,
      postEng_pfnFindEntityInSphere,
      postEng_pfnFindClientInPVS,
      postEng_pfnEntitiesInPVS,
      postEng_pfnMakeVectors,
      postEng_pfnAngleVectors,
      postEng_pfnCreateEntity,
      postEng_pfnRemoveEntity,
      postEng_pfnCreateNamedEntity,
      postEng_pfnMakeStatic,
      postEng_pfnEntIsOnFloor,
      postEng_pfnDropToFloor,
      postEng_pfnWalkMove,
      postEng_pfnSetOrigin,
      postEng_pfnEmitSound,
      postEng_pfnEmitAmbientSound,
      postEng_pfnTraceLine,
      postEng_pfnTraceToss,
      postEng_pfnTraceMonsterHull,
      postEng_pfnTraceHull,
      postEng_pfnTraceModel,
      postEng_pfnTraceTexture,
      postEng_pfnTraceSphere,
      postEng_pfnGetAimVector,
      postEng_pfnServerCommand,
      postEng_pfnServerExecute,
      postEng_pfnClientCommand,
      postEng_pfnParticleEffect,
      postEng_pfnLightStyle,
      postEng_pfnDecalIndex,
      postEng_pfnPointContents,
      postEng_pfnMessageBegin,
      postEng_pfnMessageEnd,
      postEng_pfnWriteByte,
      postEng_pfnWriteChar,
      postEng_pfnWriteShort,
      postEng_pfnWriteLong,
      postEng_pfnWriteAngle,
      postEng_pfnWriteCoord,
      postEng_pfnWriteString,
      postEng_pfnWriteEntity,
      postEng_pfnCVarRegister,
      postEng_pfnCVarGetFloat,
      postEng_pfnCVarGetString,
      postEng_pfnCVarSetFloat,
      postEng_pfnCVarSetString,
      postEng_pfnAlertMessage,
      postEng_pfnEngineFprintf,
      postEng_pfnPvAllocEntPrivateData,
      postEng_pfnPvEntPrivateData,
      postEng_pfnFreeEntPrivateData,
      postEng_pfnSzFromIndex,
      postEng_pfnAllocString,
      postEng_pfnGetVarsOfEnt,
      postEng_pfnPEntityOfEntOffset,
      postEng_pfnEntOffsetOfPEntity,
      postEng_pfnIndexOfEdict,
      postEng_pfnPEntityOfEntIndex,
      postEng_pfnFindEntityByVars,
      postEng_pfnGetModelPtr,
      postEng_pfnRegUserMsg,
      postEng_pfnAnimationAutomove,
      postEng_pfnGetBonePosition,
      postEng_pfnFunctionFromName,
      postEng_pfnNameForFunction,
      postEng_pfnClientPrintf,
      postEng_pfnServerPrint,
      postEng_pfnCmd_Args,
      postEng_pfnCmd_Argv,
      postEng_pfnCmd_Argc,
      postEng_pfnGetAttachment,
      postEng_pfnCRC32_Init,
      postEng_pfnCRC32_ProcessBuffer,
      postEng_pfnCRC32_ProcessByte,
      postEng_pfnCRC32_Final,
      postEng_pfnRandomLong,
      postEng_pfnRandomFloat,
      postEng_pfnSetView,
      postEng_pfnTime,
      postEng_pfnCrosshairAngle,
      postEng_pfnLoadFileForMe,
      postEng_pfnFreeFile,
      postEng_pfnEndSection,
      postEng_pfnCompareFileTime,
      postEng_pfnGetGameDir,
      postEng_pfnCvar_RegisterVariable,
      postEng_pfnFadeClientVolume,
      postEng_pfnSetClientMaxspeed,
      postEng_pfnCreateFakeClient,
      postEng_pfnRunPlayerMove,
      postEng_pfnNumberOfEntities,
      postEng_pfnGetInfoKeyBuffer,
      postEng_pfnInfoKeyValue,
      postEng_pfnSetKeyValue,
      postEng_pfnSetClientKeyValue,
      postEng_pfnIsMapValid,
      postEng_pfnStaticDecal,
      postEng_pfnPrecacheGeneric,
      postEng_pfnGetPlayerUserId,
      postEng_pfnBuildSoundMsg,
      postEng_pfnIsDedicatedServer,
      postEng_pfnCVarGetPointer,
      postEng_pfnGetPlayerWONId,
      postEng_pfnInfo_RemoveKey,
      postEng_pfnGetPhysicsKeyValue,
      postEng_pfnSetPhysicsKeyValue,
      postEng_pfnGetPhysicsInfoString,
      postEng_pfnPrecacheEvent,
      postEng_pfnPlaybackEvent,
      postEng_pfnSetFatPVS,
      postEng_pfnSetFatPAS,
      postEng_pfnCheckVisibility,
      postEng_pfnDeltaSetField,
      postEng_pfnDeltaUnsetField,
      postEng_pfnDeltaAddEncoder,
      postEng_pfnGetCurrentPlayer,
      postEng_pfnCanSkipPlayer,
      postEng_pfnDeltaFindField,
      postEng_pfnDeltaSetFieldByIndex,
      postEng_pfnDeltaUnsetFieldByIndex,
      postEng_pfnSetGroupMask,
      postEng_pfnCreateInstancedBaseline,
      postEng_pfnCvar_DirectSet,
      postEng_pfnForceUnmodified,
      postEng_pfnGetPlayerStats,
      postEng_pfnAddServerCommand,
      postEng_pfnVoice_GetClientListening,
      postEng_pfnVoice_SetClientListening,
      postEng_pfnGetPlayerAuthId,
      postEng_pfnSequenceGet,
      postEng_pfnSequencePickSentence,
      postEng_pfnGetFileSize,
      postEng_pfnGetApproxWavePlayLen,
      postEng_pfnIsCareerMatch,
      postEng_pfnGetLocalizedStringLength,
      postEng_pfnRegisterTutorMessageShown,
      postEng_pfnGetTimesTutorMessageShown,
      postEng_pfnProcessTutorMessageDecayBuffer,
      postEng_pfnConstructTutorMessageDecayBuffer,
      postEng_pfnResetTutorMessageDecayData,
      postEng_pfnQueryClientCvarValue,
      postEng_pfnQueryClientCvarValue2,
      postEng_CheckParm,
      postEng_pfnPEntityOfEntIndexAllEntities
    };

    void registerEngineEvents()
    {
      // base
      event::register_event("engPrecacheModel", "");
event::register_event("engPrecacheSound", "");
event::register_event("engSetModel", "");
event::register_event("engModelIndex", "");
event::register_event("engModelFrames", "");
event::register_event("engSetSize", "");
event::register_event("engChangeLevel", "");
event::register_event("engGetSpawnParms", "");
event::register_event("engSaveSpawnParms", "");
event::register_event("engVecToYaw", "");
event::register_event("engVecToAngles", "");
event::register_event("engMoveToOrigin", "");
event::register_event("engChangeYaw", "");
event::register_event("engChangePitch", "");
event::register_event("engFindEntityByString", "");
event::register_event("engGetEntityIllum", "");
event::register_event("engFindEntityInSphere", "");
event::register_event("engFindClientInPVS", "");
event::register_event("engEntitiesInPVS", "");
event::register_event("engMakeVectors", "");
event::register_event("engAngleVectors", "");
event::register_event("engCreateEntity", "");
event::register_event("engRemoveEntity", "");
event::register_event("engCreateNamedEntity", "");
event::register_event("engMakeStatic", "");
event::register_event("engEntIsOnFloor", "");
event::register_event("engDropToFloor", "");
event::register_event("engWalkMove", "");
event::register_event("engSetOrigin", "");
event::register_event("engEmitSound", "");
event::register_event("engEmitAmbientSound", "");
event::register_event("engTraceLine", "");
event::register_event("engTraceToss", "");
event::register_event("engTraceMonsterHull", "");
event::register_event("engTraceHull", "");
event::register_event("engTraceModel", "");
event::register_event("engTraceTexture", "");
event::register_event("engTraceSphere", "");
event::register_event("engGetAimVector", "");
event::register_event("engServerCommand", "");
event::register_event("engServerExecute", "");
event::register_event("engClientCommand", "");
event::register_event("engParticleEffect", "");
event::register_event("engLightStyle", "");
event::register_event("engDecalIndex", "");
event::register_event("engPointContents", "");
event::register_event("engMessageBegin", "");
event::register_event("engMessageEnd", "");
event::register_event("engWriteByte", "");
event::register_event("engWriteChar", "");
event::register_event("engWriteShort", "");
event::register_event("engWriteLong", "");
event::register_event("engWriteAngle", "");
event::register_event("engWriteCoord", "");
event::register_event("engWriteString", "");
event::register_event("engWriteEntity", "");
event::register_event("engCVarRegister", "");
event::register_event("engCVarGetFloat", "");
event::register_event("engCVarGetString", "");
event::register_event("engCVarSetFloat", "");
event::register_event("engCVarSetString", "");
event::register_event("engAlertMessage", "");
event::register_event("engEngineFprintf", "");
event::register_event("engPvAllocEntPrivateData", "");
event::register_event("engPvEntPrivateData", "");
event::register_event("engFreeEntPrivateData", "");
event::register_event("engSzFromIndex", "");
event::register_event("engAllocString", "");
event::register_event("engGetVarsOfEnt", "");
event::register_event("engPEntityOfEntOffset", "");
event::register_event("engEntOffsetOfPEntity", "");
event::register_event("engIndexOfEdict", "");
event::register_event("engPEntityOfEntIndex", "");
event::register_event("engFindEntityByVars", "");
event::register_event("engGetModelPtr", "");
event::register_event("engRegUserMsg", "");
event::register_event("engAnimationAutomove", "");
event::register_event("engGetBonePosition", "");
event::register_event("engFunctionFromName", "");
event::register_event("engNameForFunction", "");
event::register_event("engClientPrintf", "");
event::register_event("engServerPrint", "");
event::register_event("engCmdArgs", "");
event::register_event("engCmdArgv", "");
event::register_event("engCmdArgc", "");
event::register_event("engGetAttachment", "");
event::register_event("engCRC32Init", "");
event::register_event("engCRC32ProcessBuffer", "");
event::register_event("engCRC32ProcessByte", "");
event::register_event("engCRC32Final", "");
event::register_event("engRandomLong", "");
event::register_event("engRandomFloat", "");
event::register_event("engSetView", "");
event::register_event("engTime", "");
event::register_event("engCrosshairAngle", "");
event::register_event("engLoadFileForMe", "");
event::register_event("engFreeFile", "");
event::register_event("engEndSection", "");
event::register_event("engCompareFileTime", "");
event::register_event("engGetGameDir", "");
event::register_event("engCvarRegisterVariable", "");
event::register_event("engFadeClientVolume", "");
event::register_event("engSetClientMaxspeed", "");
event::register_event("engCreateFakeClient", "");
event::register_event("engRunPlayerMove", "");
event::register_event("engNumberOfEntities", "");
event::register_event("engGetInfoKeyBuffer", "");
event::register_event("engInfoKeyValue", "");
event::register_event("engSetKeyValue", "");
event::register_event("engSetClientKeyValue", "");
event::register_event("engIsMapValid", "");
event::register_event("engStaticDecal", "");
event::register_event("engPrecacheGeneric", "");
event::register_event("engGetPlayerUserId", "");
event::register_event("engBuildSoundMsg", "");
event::register_event("engIsDedicatedServer", "");
event::register_event("engCVarGetPointer", "");
event::register_event("engGetPlayerWONId", "");
event::register_event("engInfoRemoveKey", "");
event::register_event("engGetPhysicsKeyValue", "");
event::register_event("engSetPhysicsKeyValue", "");
event::register_event("engGetPhysicsInfoString", "");
event::register_event("engPrecacheEvent", "");
event::register_event("engPlaybackEvent", "");
event::register_event("engSetFatPVS", "");
event::register_event("engSetFatPAS", "");
event::register_event("engCheckVisibility", "");
event::register_event("engDeltaSetField", "");
event::register_event("engDeltaUnsetField", "");
event::register_event("engDeltaAddEncoder", "");
event::register_event("engGetCurrentPlayer", "");
event::register_event("engCanSkipPlayer", "");
event::register_event("engDeltaFindField", "");
event::register_event("engDeltaSetFieldByIndex", "");
event::register_event("engDeltaUnsetFieldByIndex", "");
event::register_event("engSetGroupMask", "");
event::register_event("engCreateInstancedBaseline", "");
event::register_event("engCvarDirectSet", "");
event::register_event("engForceUnmodified", "");
event::register_event("engGetPlayerStats", "");
event::register_event("engAddServerCommand", "");
event::register_event("engVoiceGetClientListening", "");
event::register_event("engVoiceSetClientListening", "");
event::register_event("engGetPlayerAuthId", "");
event::register_event("engSequenceGet", "");
event::register_event("engSequencePickSentence", "");
event::register_event("engGetFileSize", "");
event::register_event("engGetApproxWavePlayLen", "");
event::register_event("engIsCareerMatch", "");
event::register_event("engGetLocalizedStringLength", "");
event::register_event("engRegisterTutorMessageShown", "");
event::register_event("engGetTimesTutorMessageShown", "");
event::register_event("engProcessTutorMessageDecayBuffer", "");
event::register_event("engConstructTutorMessageDecayBuffer", "");
event::register_event("engResetTutorMessageDecayData", "");
event::register_event("engQueryClientCvarValue", "");
event::register_event("engQueryClientCvarValue2", "");
event::register_event("engCheckParm", "");
event::register_event("engPEntityOfEntIndexAllEntities", "");
      // post
      event::register_event("postEngPrecacheModel", "");
event::register_event("postEngPrecacheSound", "");
event::register_event("postEngSetModel", "");
event::register_event("postEngModelIndex", "");
event::register_event("postEngModelFrames", "");
event::register_event("postEngSetSize", "");
event::register_event("postEngChangeLevel", "");
event::register_event("postEngGetSpawnParms", "");
event::register_event("postEngSaveSpawnParms", "");
event::register_event("postEngVecToYaw", "");
event::register_event("postEngVecToAngles", "");
event::register_event("postEngMoveToOrigin", "");
event::register_event("postEngChangeYaw", "");
event::register_event("postEngChangePitch", "");
event::register_event("postEngFindEntityByString", "");
event::register_event("postEngGetEntityIllum", "");
event::register_event("postEngFindEntityInSphere", "");
event::register_event("postEngFindClientInPVS", "");
event::register_event("postEngEntitiesInPVS", "");
event::register_event("postEngMakeVectors", "");
event::register_event("postEngAngleVectors", "");
event::register_event("postEngCreateEntity", "");
event::register_event("postEngRemoveEntity", "");
event::register_event("postEngCreateNamedEntity", "");
event::register_event("postEngMakeStatic", "");
event::register_event("postEngEntIsOnFloor", "");
event::register_event("postEngDropToFloor", "");
event::register_event("postEngWalkMove", "");
event::register_event("postEngSetOrigin", "");
event::register_event("postEngEmitSound", "");
event::register_event("postEngEmitAmbientSound", "");
event::register_event("postEngTraceLine", "");
event::register_event("postEngTraceToss", "");
event::register_event("postEngTraceMonsterHull", "");
event::register_event("postEngTraceHull", "");
event::register_event("postEngTraceModel", "");
event::register_event("postEngTraceTexture", "");
event::register_event("postEngTraceSphere", "");
event::register_event("postEngGetAimVector", "");
event::register_event("postEngServerCommand", "");
event::register_event("postEngServerExecute", "");
event::register_event("postEngClientCommand", "");
event::register_event("postEngParticleEffect", "");
event::register_event("postEngLightStyle", "");
event::register_event("postEngDecalIndex", "");
event::register_event("postEngPointContents", "");
event::register_event("postEngMessageBegin", "");
event::register_event("postEngMessageEnd", "");
event::register_event("postEngWriteByte", "");
event::register_event("postEngWriteChar", "");
event::register_event("postEngWriteShort", "");
event::register_event("postEngWriteLong", "");
event::register_event("postEngWriteAngle", "");
event::register_event("postEngWriteCoord", "");
event::register_event("postEngWriteString", "");
event::register_event("postEngWriteEntity", "");
event::register_event("postEngCVarRegister", "");
event::register_event("postEngCVarGetFloat", "");
event::register_event("postEngCVarGetString", "");
event::register_event("postEngCVarSetFloat", "");
event::register_event("postEngCVarSetString", "");
event::register_event("postEngAlertMessage", "");
event::register_event("postEngEngineFprintf", "");
event::register_event("postEngPvAllocEntPrivateData", "");
event::register_event("postEngPvEntPrivateData", "");
event::register_event("postEngFreeEntPrivateData", "");
event::register_event("postEngSzFromIndex", "");
event::register_event("postEngAllocString", "");
event::register_event("postEngGetVarsOfEnt", "");
event::register_event("postEngPEntityOfEntOffset", "");
event::register_event("postEngEntOffsetOfPEntity", "");
event::register_event("postEngIndexOfEdict", "");
event::register_event("postEngPEntityOfEntIndex", "");
event::register_event("postEngFindEntityByVars", "");
event::register_event("postEngGetModelPtr", "");
event::register_event("postEngRegUserMsg", "");
event::register_event("postEngAnimationAutomove", "");
event::register_event("postEngGetBonePosition", "");
event::register_event("postEngFunctionFromName", "");
event::register_event("postEngNameForFunction", "");
event::register_event("postEngClientPrintf", "");
event::register_event("postEngServerPrint", "");
event::register_event("postEngCmdArgs", "");
event::register_event("postEngCmdArgv", "");
event::register_event("postEngCmdArgc", "");
event::register_event("postEngGetAttachment", "");
event::register_event("postEngCRC32Init", "");
event::register_event("postEngCRC32ProcessBuffer", "");
event::register_event("postEngCRC32ProcessByte", "");
event::register_event("postEngCRC32Final", "");
event::register_event("postEngRandomLong", "");
event::register_event("postEngRandomFloat", "");
event::register_event("postEngSetView", "");
event::register_event("postEngTime", "");
event::register_event("postEngCrosshairAngle", "");
event::register_event("postEngLoadFileForMe", "");
event::register_event("postEngFreeFile", "");
event::register_event("postEngEndSection", "");
event::register_event("postEngCompareFileTime", "");
event::register_event("postEngGetGameDir", "");
event::register_event("postEngCvarRegisterVariable", "");
event::register_event("postEngFadeClientVolume", "");
event::register_event("postEngSetClientMaxspeed", "");
event::register_event("postEngCreateFakeClient", "");
event::register_event("postEngRunPlayerMove", "");
event::register_event("postEngNumberOfEntities", "");
event::register_event("postEngGetInfoKeyBuffer", "");
event::register_event("postEngInfoKeyValue", "");
event::register_event("postEngSetKeyValue", "");
event::register_event("postEngSetClientKeyValue", "");
event::register_event("postEngIsMapValid", "");
event::register_event("postEngStaticDecal", "");
event::register_event("postEngPrecacheGeneric", "");
event::register_event("postEngGetPlayerUserId", "");
event::register_event("postEngBuildSoundMsg", "");
event::register_event("postEngIsDedicatedServer", "");
event::register_event("postEngCVarGetPointer", "");
event::register_event("postEngGetPlayerWONId", "");
event::register_event("postEngInfoRemoveKey", "");
event::register_event("postEngGetPhysicsKeyValue", "");
event::register_event("postEngSetPhysicsKeyValue", "");
event::register_event("postEngGetPhysicsInfoString", "");
event::register_event("postEngPrecacheEvent", "");
event::register_event("postEngPlaybackEvent", "");
event::register_event("postEngSetFatPVS", "");
event::register_event("postEngSetFatPAS", "");
event::register_event("postEngCheckVisibility", "");
event::register_event("postEngDeltaSetField", "");
event::register_event("postEngDeltaUnsetField", "");
event::register_event("postEngDeltaAddEncoder", "");
event::register_event("postEngGetCurrentPlayer", "");
event::register_event("postEngCanSkipPlayer", "");
event::register_event("postEngDeltaFindField", "");
event::register_event("postEngDeltaSetFieldByIndex", "");
event::register_event("postEngDeltaUnsetFieldByIndex", "");
event::register_event("postEngSetGroupMask", "");
event::register_event("postEngCreateInstancedBaseline", "");
event::register_event("postEngCvarDirectSet", "");
event::register_event("postEngForceUnmodified", "");
event::register_event("postEngGetPlayerStats", "");
event::register_event("postEngAddServerCommand", "");
event::register_event("postEngVoiceGetClientListening", "");
event::register_event("postEngVoiceSetClientListening", "");
event::register_event("postEngGetPlayerAuthId", "");
event::register_event("postEngSequenceGet", "");
event::register_event("postEngSequencePickSentence", "");
event::register_event("postEngGetFileSize", "");
event::register_event("postEngGetApproxWavePlayLen", "");
event::register_event("postEngIsCareerMatch", "");
event::register_event("postEngGetLocalizedStringLength", "");
event::register_event("postEngRegisterTutorMessageShown", "");
event::register_event("postEngGetTimesTutorMessageShown", "");
event::register_event("postEngProcessTutorMessageDecayBuffer", "");
event::register_event("postEngConstructTutorMessageDecayBuffer", "");
event::register_event("postEngResetTutorMessageDecayData", "");
event::register_event("postEngQueryClientCvarValue", "");
event::register_event("postEngQueryClientCvarValue2", "");
event::register_event("postEngCheckParm", "");
event::register_event("postEngPEntityOfEntIndexAllEntities", "");
    }
  