#include "extdll.h"
#include "structures.hpp"
#include "common_macros.hpp"
#include "util/convert.hpp"
#include "node/utils.hpp"
#include <unordered_map>

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
      wrappedEntities[entityId].Reset(args.GetIsolate(), args.This());
      args.GetReturnValue().Set(args.This());
    });

    v8::Local<v8::ObjectTemplate> _entity = entityConstructor->InstanceTemplate();
    _entity->SetInternalFieldCount(1);

    // Entity ID (read-only computed property)
    _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "id").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());
            if (edict == nullptr) {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), 0));
            } else {
                int entityId = (*g_engfuncs.pfnIndexOfEdict)(edict);
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), entityId));
            }
        });

    // Entvars property - returns wrapped entvars_t object
    _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "entvars").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());
            if (edict == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(structures::wrapEntvars(info.GetIsolate(), &edict->v));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());
            if (edict == nullptr) return;
            
            entvars_t *newEntvars = structures::unwrapEntvars(info.GetIsolate(), value);
            if (newEntvars != nullptr) {
                edict->v = *newEntvars;  // Copy the entire entvars structure
            }
        });

    // Common entvars field accessors that delegate to the entvars object
    #define DELEGATE_ACCESSOR(FIELD_NAME) \
        _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, FIELD_NAME).ToLocalChecked(), \
            [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) { \
                edict_t *edict = structures::unwrapEntity(info.GetIsolate(), info.Holder()); \
                if (edict == nullptr) return; \
                v8::Local<v8::Value> entvars = structures::wrapEntvars(info.GetIsolate(), &edict->v); \
                if (!entvars.IsEmpty() && entvars->IsObject()) { \
                    v8::Local<v8::Object> entvarsObj = entvars.As<v8::Object>(); \
                    auto context = info.GetIsolate()->GetCurrentContext(); \
                    v8::Local<v8::Value> value = entvarsObj->Get(context, property).ToLocalChecked(); \
                    info.GetReturnValue().Set(value); \
                } \
            }, \
            [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) { \
                edict_t *edict = structures::unwrapEntity(info.GetIsolate(), info.Holder()); \
                if (edict == nullptr) return; \
                v8::Local<v8::Value> entvars = structures::wrapEntvars(info.GetIsolate(), &edict->v); \
                if (!entvars.IsEmpty() && entvars->IsObject()) { \
                    v8::Local<v8::Object> entvarsObj = entvars.As<v8::Object>(); \
                    auto context = info.GetIsolate()->GetCurrentContext(); \
                    entvarsObj->Set(context, property, value).Check(); \
                } \
            })

    DELEGATE_ACCESSOR("classname");
	  DELEGATE_ACCESSOR("globalname");
	
	  DELEGATE_ACCESSOR("origin");
	  DELEGATE_ACCESSOR("oldorigin");
	  DELEGATE_ACCESSOR("velocity");
	  DELEGATE_ACCESSOR("basevelocity");
	  DELEGATE_ACCESSOR("clbasevelocity");

	  DELEGATE_ACCESSOR("movedir");

	  DELEGATE_ACCESSOR("angles");
	  DELEGATE_ACCESSOR("avelocity");
	  DELEGATE_ACCESSOR("punchangle");
	  DELEGATE_ACCESSOR("v_angle");

	  DELEGATE_ACCESSOR("endpos");
	  DELEGATE_ACCESSOR("startpos");
	  DELEGATE_ACCESSOR("impacttime");
	  DELEGATE_ACCESSOR("starttime");

	  DELEGATE_ACCESSOR("fixangle");
	  DELEGATE_ACCESSOR("idealpitch");
	  DELEGATE_ACCESSOR("pitch_speed");
	  DELEGATE_ACCESSOR("ideal_yaw");
	  DELEGATE_ACCESSOR("yaw_speed");

	  DELEGATE_ACCESSOR("modelindex");

	  DELEGATE_ACCESSOR("model");
	  DELEGATE_ACCESSOR("viewmodel");	
	  DELEGATE_ACCESSOR("weaponmodel");

	  DELEGATE_ACCESSOR("absmin");
	  DELEGATE_ACCESSOR("absmax");
	  DELEGATE_ACCESSOR("mins");
	  DELEGATE_ACCESSOR("maxs");
	  DELEGATE_ACCESSOR("size");

	  DELEGATE_ACCESSOR("ltime");
	  DELEGATE_ACCESSOR("nextthink");

	  DELEGATE_ACCESSOR("movetype");
	  DELEGATE_ACCESSOR("solid");

	  DELEGATE_ACCESSOR("skin");
	  DELEGATE_ACCESSOR("body");
	  DELEGATE_ACCESSOR("effects");
	  DELEGATE_ACCESSOR("gravity");
	  DELEGATE_ACCESSOR("friction");

	  DELEGATE_ACCESSOR("light_level");

	  DELEGATE_ACCESSOR("sequence");
	  DELEGATE_ACCESSOR("gaitsequence");
	  DELEGATE_ACCESSOR("frame");
	  DELEGATE_ACCESSOR("animtime");
	  DELEGATE_ACCESSOR("framerate");	
	  DELEGATE_ACCESSOR("controller");
	  DELEGATE_ACCESSOR("blending");

	  DELEGATE_ACCESSOR("scale");
	  DELEGATE_ACCESSOR("rendermode");
	  DELEGATE_ACCESSOR("renderamt");
	  DELEGATE_ACCESSOR("rendercolor");
	  DELEGATE_ACCESSOR("renderfx");

	  DELEGATE_ACCESSOR("health");
	  DELEGATE_ACCESSOR("frags");
	  DELEGATE_ACCESSOR("weapons");
	  DELEGATE_ACCESSOR("takedamage");

	  DELEGATE_ACCESSOR("deadflag");
	  DELEGATE_ACCESSOR("view_ofs");

	  DELEGATE_ACCESSOR("button");
	  DELEGATE_ACCESSOR("impulse");

	  DELEGATE_ACCESSOR("chain");
	  DELEGATE_ACCESSOR("dmg_inflictor");
	  DELEGATE_ACCESSOR("enemy");
	  DELEGATE_ACCESSOR("aiment");
	  DELEGATE_ACCESSOR("owner");
	  DELEGATE_ACCESSOR("groundentity");

	  DELEGATE_ACCESSOR("spawnflags");
	  DELEGATE_ACCESSOR("flags");
	
	  DELEGATE_ACCESSOR("colormap");
	  DELEGATE_ACCESSOR("team");

	  DELEGATE_ACCESSOR("max_health");
	  DELEGATE_ACCESSOR("teleport_time");
	  DELEGATE_ACCESSOR("armortype");
	  DELEGATE_ACCESSOR("armorvalue");
	  DELEGATE_ACCESSOR("waterlevel");
	  DELEGATE_ACCESSOR("watertype");

	  DELEGATE_ACCESSOR("target");
	  DELEGATE_ACCESSOR("targetname");
	  DELEGATE_ACCESSOR("netname");
	  DELEGATE_ACCESSOR("message");

	  DELEGATE_ACCESSOR("dmg_take");
	  DELEGATE_ACCESSOR("dmg_save");
	  DELEGATE_ACCESSOR("dmg");
	  DELEGATE_ACCESSOR("dmgtime");

	  DELEGATE_ACCESSOR("noise");
	  DELEGATE_ACCESSOR("noise1");
	  DELEGATE_ACCESSOR("noise2");
	  DELEGATE_ACCESSOR("noise3");

	  DELEGATE_ACCESSOR("speed");
	  DELEGATE_ACCESSOR("air_finished");
	  DELEGATE_ACCESSOR("pain_finished");
	  DELEGATE_ACCESSOR("radsuit_finished");

	  DELEGATE_ACCESSOR("pContainingEntity");

	  DELEGATE_ACCESSOR("playerclass");
	  DELEGATE_ACCESSOR("maxspeed");

	  DELEGATE_ACCESSOR("fov");
	  DELEGATE_ACCESSOR("weaponanim");

	  DELEGATE_ACCESSOR("pushmsec");

	  DELEGATE_ACCESSOR("bInDuck");
	  DELEGATE_ACCESSOR("flTimeStepSound");
	  DELEGATE_ACCESSOR("flSwimTime");
	  DELEGATE_ACCESSOR("flDuckTime");
	  DELEGATE_ACCESSOR("iStepLeft");
	  DELEGATE_ACCESSOR("flFallVelocity");

	  DELEGATE_ACCESSOR("gamestate");

	  DELEGATE_ACCESSOR("oldbuttons");

	  DELEGATE_ACCESSOR("groupinfo");

	  // For mods
	  DELEGATE_ACCESSOR("iuser1");
	  DELEGATE_ACCESSOR("iuser2");
	  DELEGATE_ACCESSOR("iuser3");
	  DELEGATE_ACCESSOR("iuser4");
	  DELEGATE_ACCESSOR("fuser1");
	  DELEGATE_ACCESSOR("fuser2");
	  DELEGATE_ACCESSOR("fuser3");
	  DELEGATE_ACCESSOR("fuser4");
	  DELEGATE_ACCESSOR("vuser1");
	  DELEGATE_ACCESSOR("vuser2");
	  DELEGATE_ACCESSOR("vuser3");
	  DELEGATE_ACCESSOR("vuser4");
	  DELEGATE_ACCESSOR("euser1");
	  DELEGATE_ACCESSOR("euser2");
	  DELEGATE_ACCESSOR("euser3");
	  DELEGATE_ACCESSOR("euser4");

    #undef DELEGATE_ACCESSOR
    
    structures::entity.Set(isolate, _entity);
    global->Set(convert::str2js(isolate, "Entity"), entityConstructor);
  }
}