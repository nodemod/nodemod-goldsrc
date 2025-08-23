#pragma once
#include "v8.h"
#include "extdll.h"

namespace structures {
  extern v8::Eternal<v8::ObjectTemplate> entity;

  extern void createEntityTemplate(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global);
  extern  v8::Local<v8::Value> wrapEntity(v8::Isolate* isolate, const edict_t* entity);
  extern edict_t* unwrapEntity(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);

  // TraceResult wrapper functions
  extern v8::Local<v8::Value> wrapTraceResult(v8::Isolate* isolate, TraceResult* trace);
  extern TraceResult* unwrapTraceResult(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);

  // Stub implementations for other Half-Life structures (return v8::External for now)
  extern v8::Local<v8::Value> wrapEntvars(v8::Isolate* isolate, entvars_t* entvars);
  extern entvars_t* unwrapEntvars(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapClientData(v8::Isolate* isolate, void* clientdata);
  extern void* unwrapClientData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapEntityState(v8::Isolate* isolate, void* entitystate);
  extern void* unwrapEntityState(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapUserCmd(v8::Isolate* isolate, void* usercmd);
  extern void* unwrapUserCmd(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapNetAdr(v8::Isolate* isolate, void* netadr);
  extern void* unwrapNetAdr(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapWeaponData(v8::Isolate* isolate, void* weapondata);
  extern void* unwrapWeaponData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapPlayerMove(v8::Isolate* isolate, void* playermove);
  extern void* unwrapPlayerMove(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapCustomization(v8::Isolate* isolate, void* customization);
  extern void* unwrapCustomization(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapKeyValueData(v8::Isolate* isolate, void* keyvalue);
  extern void* unwrapKeyValueData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapSaveRestoreData(v8::Isolate* isolate, void* savedata);
  extern void* unwrapSaveRestoreData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapTypeDescription(v8::Isolate* isolate, void* typedesc);
  extern void* unwrapTypeDescription(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapDelta(v8::Isolate* isolate, void* delta);
  extern void* unwrapDelta(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
  
  extern v8::Local<v8::Value> wrapCvar(v8::Isolate* isolate, void* cvar);
  extern void* unwrapCvar(v8::Isolate* isolate, const v8::Local<v8::Value> &obj);
}