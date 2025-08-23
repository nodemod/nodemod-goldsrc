#include "structures.hpp"
#include "common_macros.hpp"
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> entvarsTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedEntvars;

entvars_t* unwrapEntvars_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<entvars_t*>(field);
}

void createEntvarsTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // String fields (using string_t)
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "classname", classname, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "globalname", globalname, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "target", target, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "targetname", targetname, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "netname", netname, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "message", message, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "noise", noise, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "noise1", noise1, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "noise2", noise2, GETSTR, SETSTR);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "noise3", noise3, GETSTR, SETSTR);
    
    // Special handling for model field (uses SetModel engine function)
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "model").ToLocalChecked(),
        GETTER_T(entvars_t, unwrapEntvars_internal, model, GETSTR),
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr) return;
            
            // Get the edict from the entvars (assuming pContainingEntity is valid)
            if (entvars->pContainingEntity) {
                (*g_engfuncs.pfnSetModel)(entvars->pContainingEntity, convert::js2str(info.GetIsolate(), value));
            }
        });
    
    // Vector fields
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "origin", origin, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "oldorigin", oldorigin, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "velocity", velocity, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "basevelocity", basevelocity, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "clbasevelocity", clbasevelocity, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "movedir", movedir, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "angles", angles, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "avelocity", avelocity, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "punchangle", punchangle, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "v_angle", v_angle, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "endpos", endpos, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "startpos", startpos, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "absmin", absmin, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "absmax", absmax, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "mins", mins, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "maxs", maxs, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "size", size, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "rendercolor", rendercolor, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "view_ofs", view_ofs, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "vuser1", vuser1, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "vuser2", vuser2, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "vuser3", vuser3, GETVEC3, SETVEC3);
    ACCESSORL_T(entvars_t, unwrapEntvars_internal, templ, "vuser4", vuser4, GETVEC3, SETVEC3);
    
    // Float fields
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "impacttime", impacttime, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "starttime", starttime, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "idealpitch", idealpitch, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "pitch_speed", pitch_speed, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "ideal_yaw", ideal_yaw, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "yaw_speed", yaw_speed, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "ltime", ltime, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "nextthink", nextthink, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "gravity", gravity, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "friction", friction, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "frame", frame, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "animtime", animtime, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "framerate", framerate, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "scale", scale, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "renderamt", renderamt, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "health", health, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "frags", frags, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "takedamage", takedamage, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "max_health", max_health, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "teleport_time", teleport_time, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "armortype", armortype, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "armorvalue", armorvalue, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "dmg_take", dmg_take, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "dmg_save", dmg_save, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "dmg", dmg, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "dmgtime", dmgtime, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "speed", speed, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "air_finished", air_finished, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "pain_finished", pain_finished, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "radsuit_finished", radsuit_finished, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "maxspeed", maxspeed, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fov", fov, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "flFallVelocity", flFallVelocity, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fuser1", fuser1, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fuser2", fuser2, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fuser3", fuser3, GETN, SETFLOAT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fuser4", fuser4, GETN, SETFLOAT);
    
    // Integer fields
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "fixangle", fixangle, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "modelindex", modelindex, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "viewmodel", viewmodel, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "weaponmodel", weaponmodel, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "movetype", movetype, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "solid", solid, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "skin", skin, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "body", body, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "effects", effects, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "light_level", light_level, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "sequence", sequence, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "gaitsequence", gaitsequence, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "rendermode", rendermode, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "renderfx", renderfx, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "weapons", weapons, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "deadflag", deadflag, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "button", button, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "impulse", impulse, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "spawnflags", spawnflags, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "flags", flags, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "colormap", colormap, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "team", team, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "waterlevel", waterlevel, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "watertype", watertype, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "playerclass", playerclass, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "weaponanim", weaponanim, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "pushmsec", pushmsec, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "bInDuck", bInDuck, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "flTimeStepSound", flTimeStepSound, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "flSwimTime", flSwimTime, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "flDuckTime", flDuckTime, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "iStepLeft", iStepLeft, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "gamestate", gamestate, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "oldbuttons", oldbuttons, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "groupinfo", groupinfo, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "iuser1", iuser1, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "iuser2", iuser2, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "iuser3", iuser3, GETN, SETINT);
    ACCESSOR_T(entvars_t, unwrapEntvars_internal, templ, "iuser4", iuser4, GETN, SETINT);
    
    // Entity references - wrap as Entity objects when not null
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "chain").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->chain == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->chain));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "dmg_inflictor").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->dmg_inflictor == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->dmg_inflictor));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "enemy").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->enemy == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->enemy));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "aiment").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->aiment == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->aiment));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "owner").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->owner == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->owner));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "groundentity").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->groundentity == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->groundentity));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "pContainingEntity").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->pContainingEntity == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->pContainingEntity));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "euser1").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->euser1 == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->euser1));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "euser2").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->euser2 == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->euser2));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "euser3").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->euser3 == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->euser3));
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "euser4").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entvars_t *entvars = unwrapEntvars_internal(info.GetIsolate(), info.Holder());
            if (entvars == nullptr || entvars->euser4 == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), entvars->euser4));
            }
        });
    
    entvarsTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapEntvars(v8::Isolate* isolate, entvars_t* entvars) {
    v8::Locker locker(isolate);
    if (!entvars) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedEntvars.find(entvars) != wrappedEntvars.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedEntvars[entvars]);
    }
    
    // Create template if not initialized
    if (entvarsTemplate.IsEmpty()) {
        createEntvarsTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = entvarsTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, entvars);
    
    wrappedEntvars[entvars].Reset(isolate, obj);
    return obj;
}

entvars_t* unwrapEntvars(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapEntvars_internal(isolate, obj);
}

}