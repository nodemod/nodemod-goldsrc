#include "extdll.h"
#include "structures.hpp"
#include "util/convert.hpp"
#include "node/utils.hpp"
#include <unordered_map>

extern enginefuncs_t g_engfuncs;

#define E_GET (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());

#define GETN(v) v
#define GETVEC3(v) utils::vect2js(info.GetIsolate(), v)
#define GETSTR(v) convert::str2js(info.GetIsolate(), (*g_engfuncs.pfnSzFromIndex)(v))

#define SETFLOAT(v) v->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked()
#define SETINT(v) v->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked()
#define SETSTR(v) (*g_engfuncs.pfnAllocString)(utils::js2string(info.GetIsolate(), v))
#define SETVEC3(v, f) utils::js2vect(info.GetIsolate(), v8::Local<v8::Array>::Cast(v), f)

#define GETTER(FIELD, TYPE) ([](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) { \
        edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder()); \
        if (edict == NULL) return;\
        info.GetReturnValue().Set(TYPE(edict->FIELD)); })

#define SETTER(field, TYPE) ([](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) { \
        edict_t *edict = E_GET \
        if (edict == NULL) return; \
        edict->field = TYPE(value); })

#define SETTERL(field, TYPE) ([](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) { \
        edict_t *edict = E_GET \
        if (edict == NULL) return; \
        TYPE(value, edict->field); })

#define ACCESSOR(entity, name, field, GET, SET) entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name).ToLocalChecked(), GETTER(field, GET), SETTER(field, SET))
#define ACCESSORL(entity, name, field, GET, SET) entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name).ToLocalChecked(), GETTER(field, GET), SETTERL(field, SET))

namespace structures
{
  v8::Eternal<v8::ObjectTemplate> entity;
  std::unordered_map<int, v8::Persistent<v8::Object>> wrappedEntities;

  void cacheEntity(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    // wrappedEntities[convert::js2int(isolate, info[0])].Reset(isolate, info[1]);
  }

  edict_t *unwrapEntity(v8::Isolate *isolate, const v8::Local<v8::Value> &obj)
  {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject())
    {
      return NULL;
    }

    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty())
    {
      return NULL;
    }

    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<edict_t *>(field);
  }

  v8::Local<v8::Value> wrapEntity(v8::Isolate *isolate, const edict_t *entity)
  {
    v8::Locker locker(isolate);
    if (entity == NULL)
    {
      return v8::Null(isolate);
    }

    int entityId = (*g_engfuncs.pfnIndexOfEdict)(entity);
    if (wrappedEntities.find(entityId) != wrappedEntities.end())
    {
      v8::Local<v8::Object> existingObject = v8::Local<v8::Object>::New(isolate, wrappedEntities[entityId]);
      return existingObject;
    }

    // Create a new instance if an object doesn't exist for this ID
    v8::Local<v8::Object> obj = structures::entity.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, const_cast<edict_t*>(entity));
    obj->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, "id").ToLocalChecked(), v8::Number::New(isolate, entityId));

    wrappedEntities[entityId].Reset(isolate, obj);
    return obj;
  }

  void createEntityTemplate(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global)
  {
    v8::Locker locker(isolate);
    v8::EscapableHandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    v8::Local<v8::FunctionTemplate> entityConstructor = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value>& args) {
      edict_t* entity = (*g_engfuncs.pfnCreateNamedEntity)((*g_engfuncs.pfnAllocString)(utils::js2string(args.GetIsolate(), args[0])));

      args.This()->SetAlignedPointerInInternalField(0, entity);

      int entityId = (*g_engfuncs.pfnIndexOfEdict)(entity);
      args.This()->Set(args.GetIsolate()->GetCurrentContext(), convert::str2js(args.GetIsolate(), "id"), v8::Number::New(args.GetIsolate(), entityId));

      wrappedEntities[entityId].Reset(args.GetIsolate(), args.This());
      args.GetReturnValue().Set(args.This());
    });

    v8::Local<v8::ObjectTemplate> _entity = entityConstructor->InstanceTemplate();
    _entity->SetInternalFieldCount(1);

    // entvars
    ACCESSOR(_entity, "classname", v.classname, GETSTR, SETSTR);
    ACCESSOR(_entity, "globalname", v.globalname, GETSTR, SETSTR);
    ACCESSORL(_entity, "origin", v.origin, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "oldorigin", v.oldorigin, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "velocity", v.velocity, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "basevelocity", v.basevelocity, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "clbasevelocity", v.clbasevelocity, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "movedir", v.movedir, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "angles", v.angles, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "avelocity", v.avelocity, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "punchangle", v.punchangle, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "angle", v.v_angle, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "endpos", v.endpos, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "startpos", v.startpos, GETVEC3, SETVEC3);
    ACCESSOR(_entity, "impacttime", v.impacttime, GETN, SETFLOAT);
    ACCESSOR(_entity, "starttime", v.starttime, GETN, SETFLOAT);
    ACCESSOR(_entity, "fixangle", v.fixangle, GETN, SETINT);
    ACCESSOR(_entity, "idealpitch", v.idealpitch, GETN, SETFLOAT);
    ACCESSOR(_entity, "pitchSpeed", v.pitch_speed, GETN, SETFLOAT);
    ACCESSOR(_entity, "idealYaw", v.ideal_yaw, GETN, SETFLOAT);
    ACCESSOR(_entity, "yawSpeed", v.yaw_speed, GETN, SETFLOAT);
    ACCESSOR(_entity, "modelindex", v.modelindex, GETN, SETINT);

    _entity->SetNativeDataProperty(
        convert::str2js(isolate, "model"),
        GETTER(v.model, GETSTR),
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
        {
          edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());
          if (edict == NULL)
            return;
          (*g_engfuncs.pfnSetModel)(edict, convert::js2str(info.GetIsolate(), value));
        });

    ACCESSOR(_entity, "viewmodel", v.viewmodel, GETN, SETINT);
    ACCESSOR(_entity, "weaponmodel", v.weaponmodel, GETN, SETINT);
    ACCESSORL(_entity, "absmin", v.absmin, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "absmax", v.absmax, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "mins", v.mins, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "maxs", v.maxs, GETVEC3, SETVEC3);
    ACCESSORL(_entity, "size", v.maxs, GETVEC3, SETVEC3);
    ACCESSOR(_entity, "ltime", v.ltime, GETN, SETFLOAT);
    ACCESSOR(_entity, "nextthink", v.nextthink, GETN, SETFLOAT);
    ACCESSOR(_entity, "movetype", v.movetype, GETN, SETINT);
    ACCESSOR(_entity, "solid", v.solid, GETN, SETINT);
    ACCESSOR(_entity, "skin", v.skin, GETN, SETINT);
    ACCESSOR(_entity, "body", v.body, GETN, SETINT);
    ACCESSOR(_entity, "effects", v.effects, GETN, SETINT);
    ACCESSOR(_entity, "gravity", v.gravity, GETN, SETFLOAT);
    ACCESSOR(_entity, "friction", v.friction, GETN, SETFLOAT);
    ACCESSOR(_entity, "lightLevel", v.light_level, GETN, SETINT);
    ACCESSOR(_entity, "sequence", v.sequence, GETN, SETINT);
    ACCESSOR(_entity, "gaitsequence", v.gaitsequence, GETN, SETINT);
    ACCESSOR(_entity, "frame", v.frame, GETN, SETFLOAT);
    ACCESSOR(_entity, "animtime", v.animtime, GETN, SETFLOAT);
    ACCESSOR(_entity, "framerate", v.framerate, GETN, SETFLOAT);
	  // byte		controller[4];	// bone controller setting (0..255)
	  // byte		blending[2];	// blending amount between sub-sequences (0..255)
    ACCESSOR(_entity, "scale", v.scale, GETN, SETFLOAT);
    ACCESSOR(_entity, "rendermode", v.rendermode, GETN, SETINT);
    ACCESSOR(_entity, "renderamt", v.renderamt, GETN, SETFLOAT);
    ACCESSORL(_entity, "rendercolor", v.rendercolor, GETVEC3, SETVEC3);
    ACCESSOR(_entity, "renderfx", v.renderfx, GETN, SETINT);
    ACCESSOR(_entity, "health", v.health, GETN, SETFLOAT);
    ACCESSOR(_entity, "frags", v.frags, GETN, SETFLOAT);
    ACCESSOR(_entity, "weapons", v.weapons, GETN, SETINT);
    ACCESSOR(_entity, "takedamage", v.takedamage, GETN, SETFLOAT);
    ACCESSOR(_entity, "deadflag", v.deadflag, GETN, SETINT);
    ACCESSORL(_entity, "viewOfs", v.view_ofs, GETVEC3, SETVEC3);
    ACCESSOR(_entity, "button", v.button, GETN, SETINT);
    ACCESSOR(_entity, "impulse", v.impulse, GETN, SETINT);
    // edict_t		*chain;		// Entity pointer when linked into a linked list
    // edict_t		*dmg_inflictor;
    // edict_t		*enemy;
    // edict_t		*aiment;		// entity pointer when MOVETYPE_FOLLOW
    // edict_t		*owner;
    // edict_t		*groundentity;
    ACCESSOR(_entity, "spawnflags", v.spawnflags, GETN, SETINT);
    ACCESSOR(_entity, "flags", v.flags, GETN, SETINT);
    ACCESSOR(_entity, "colormap", v.colormap, GETN, SETINT);
    ACCESSOR(_entity, "team", v.team, GETN, SETINT);
    ACCESSOR(_entity, "maxHealth", v.max_health, GETN, SETFLOAT);
    ACCESSOR(_entity, "teleportTime", v.teleport_time, GETN, SETFLOAT);
    ACCESSOR(_entity, "armortype", v.armortype, GETN, SETFLOAT);
    ACCESSOR(_entity, "armorvalue", v.armorvalue, GETN, SETFLOAT);
    ACCESSOR(_entity, "waterlevel", v.waterlevel, GETN, SETINT);
    ACCESSOR(_entity, "watertype", v.watertype, GETN, SETINT);
    ACCESSOR(_entity, "target", v.target, GETSTR, SETSTR);
    ACCESSOR(_entity, "targetname", v.targetname, GETSTR, SETSTR);
    ACCESSOR(_entity, "netname", v.netname, GETSTR, SETSTR);
    ACCESSOR(_entity, "message", v.message, GETSTR, SETSTR);
    ACCESSOR(_entity, "dmgTake", v.dmg_take, GETN, SETFLOAT);
    ACCESSOR(_entity, "dmgSave", v.dmg_save, GETN, SETFLOAT);
    ACCESSOR(_entity, "dmg", v.dmg, GETN, SETFLOAT);
    ACCESSOR(_entity, "dmgtime", v.dmgtime, GETN, SETFLOAT);
    ACCESSOR(_entity, "noise", v.noise, GETSTR, SETSTR);
    ACCESSOR(_entity, "noise1", v.noise1, GETSTR, SETSTR);
    ACCESSOR(_entity, "noise2", v.noise2, GETSTR, SETSTR);
    ACCESSOR(_entity, "noise3", v.noise3, GETSTR, SETSTR);
    ACCESSOR(_entity, "speed", v.speed, GETN, SETFLOAT);
    ACCESSOR(_entity, "airFinished", v.air_finished, GETN, SETFLOAT);
    ACCESSOR(_entity, "painFinished", v.pain_finished, GETN, SETFLOAT);
    ACCESSOR(_entity, "radsuitFinished", v.radsuit_finished, GETN, SETFLOAT);
	  // edict_t		*pContainingEntity;
    ACCESSOR(_entity, "playerclass", v.playerclass, GETN, SETINT);
    ACCESSOR(_entity, "maxspeed", v.maxspeed, GETN, SETFLOAT);
    ACCESSOR(_entity, "fov", v.fov, GETN, SETFLOAT);
    ACCESSOR(_entity, "weaponanim", v.weaponanim, GETN, SETINT);
    ACCESSOR(_entity, "pushmsec", v.pushmsec, GETN, SETINT);
    ACCESSOR(_entity, "bInDuck", v.bInDuck, GETN, SETINT);
    ACCESSOR(_entity, "flTimeStepSound", v.flTimeStepSound, GETN, SETINT);
    ACCESSOR(_entity, "flSwimTime", v.flSwimTime, GETN, SETINT);
    ACCESSOR(_entity, "flDuckTime", v.flDuckTime, GETN, SETINT);
    ACCESSOR(_entity, "iStepLeft", v.iStepLeft, GETN, SETINT);
    ACCESSOR(_entity, "fallVelocity", v.flFallVelocity, GETN, SETFLOAT);
    ACCESSOR(_entity, "gamestate", v.gamestate, GETN, SETINT);
    ACCESSOR(_entity, "oldbuttons", v.oldbuttons, GETN, SETINT);
    ACCESSOR(_entity, "groupinfo", v.groupinfo, GETN, SETINT);
    
    structures::entity.Set(isolate, _entity);
    global->Set(convert::str2js(isolate, "Entity"), entityConstructor);
  }
}