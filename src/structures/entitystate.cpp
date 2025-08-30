#include "structures.hpp"
#include "common_macros.hpp"
#include <entity_state.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> entityStateTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedEntityStates;

entity_state_s* unwrapEntityState_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<entity_state_s*>(field);
}

void createEntityStateTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Fields in exact order as they appear in entity_state.h
    
    // Fields which are filled in by routines outside of delta compression
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "entityType", entityType, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "number", number, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "msg_time", msg_time, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "messagenum", messagenum, GETN, SETINT);

    // Fields which can be transmitted and reconstructed over the network stream
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "origin", origin, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "angles", angles, GETVEC3, SETVEC3);

    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "modelindex", modelindex, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "sequence", sequence, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "frame", frame, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "colormap", colormap, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "skin", skin, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "solid", solid, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "effects", effects, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "scale", scale, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "eflags", eflags, GETBYTE, SETBYTE);

    // Render information
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "rendermode", rendermode, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "renderamt", renderamt, GETN, SETINT);
    // rendercolor is a color24 struct, not a vector
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "rendercolor").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate(), 3);
            auto context = info.GetIsolate()->GetCurrentContext();
            arr->Set(context, 0, v8::Number::New(info.GetIsolate(), state->rendercolor.r)).Check();
            arr->Set(context, 1, v8::Number::New(info.GetIsolate(), state->rendercolor.g)).Check();
            arr->Set(context, 2, v8::Number::New(info.GetIsolate(), state->rendercolor.b)).Check();
            info.GetReturnValue().Set(arr);
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr || !value->IsArray()) return;
            
            v8::Local<v8::Array> arr = value.As<v8::Array>();
            auto context = info.GetIsolate()->GetCurrentContext();
            if (arr->Length() >= 3) {
                v8::Local<v8::Value> r = arr->Get(context, 0).ToLocalChecked();
                v8::Local<v8::Value> g = arr->Get(context, 1).ToLocalChecked();
                v8::Local<v8::Value> b = arr->Get(context, 2).ToLocalChecked();
                
                if (r->IsNumber()) state->rendercolor.r = r->Int32Value(context).FromJust();
                if (g->IsNumber()) state->rendercolor.g = g->Int32Value(context).FromJust();
                if (b->IsNumber()) state->rendercolor.b = b->Int32Value(context).FromJust();
            }
        });
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "renderfx", renderfx, GETN, SETINT);

    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "movetype", movetype, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "animtime", animtime, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "framerate", framerate, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "body", body, GETN, SETINT);
    
    // Array fields (controller and blending) in exact order from header
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "controller").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate(), 4);
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 4; i++) {
                arr->Set(context, i, v8::Number::New(info.GetIsolate(), state->controller[i])).Check();
            }
            info.GetReturnValue().Set(arr);
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr || !value->IsArray()) return;
            
            v8::Local<v8::Array> arr = v8::Local<v8::Array>::Cast(value);
            auto context = info.GetIsolate()->GetCurrentContext();
            uint32_t length = arr->Length();
            
            for (uint32_t i = 0; i < 4 && i < length; i++) {
                v8::Local<v8::Value> val = arr->Get(context, i).ToLocalChecked();
                if (val->IsNumber()) {
                    state->controller[i] = SETBYTE(val);
                }
            }
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "blending").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate(), 4);
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 4; i++) {
                arr->Set(context, i, v8::Number::New(info.GetIsolate(), state->blending[i])).Check();
            }
            info.GetReturnValue().Set(arr);
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            entity_state_s *state = unwrapEntityState_internal(info.GetIsolate(), info.Holder());
            if (state == nullptr || !value->IsArray()) return;
            
            v8::Local<v8::Array> arr = v8::Local<v8::Array>::Cast(value);
            auto context = info.GetIsolate()->GetCurrentContext();
            uint32_t length = arr->Length();
            
            for (uint32_t i = 0; i < 4 && i < length; i++) {
                v8::Local<v8::Value> val = arr->Get(context, i).ToLocalChecked();
                if (val->IsNumber()) {
                    state->blending[i] = SETBYTE(val);
                }
            }
        });
    
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "velocity", velocity, GETVEC3, SETVEC3);

    // Send bbox down to client for use during prediction
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "mins", mins, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "maxs", maxs, GETVEC3, SETVEC3);

    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "aiment", aiment, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "owner", owner, GETN, SETINT);

    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "friction", friction, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "gravity", gravity, GETN, SETFLOAT);

    // PLAYER SPECIFIC
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "team", team, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "playerclass", playerclass, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "health", health, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "spectator", spectator, GETQBOOL, SETQBOOL);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "weaponmodel", weaponmodel, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "gaitsequence", gaitsequence, GETN, SETINT);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "basevelocity", basevelocity, GETVEC3, SETVEC3);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "usehull", usehull, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "oldbuttons", oldbuttons, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "onground", onground, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "iStepLeft", iStepLeft, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "flFallVelocity", flFallVelocity, GETN, SETFLOAT);

    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "fov", fov, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "weaponanim", weaponanim, GETN, SETINT);

    // Parametric movement overrides
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "startpos", startpos, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "endpos", endpos, GETVEC3, SETVEC3);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "impacttime", impacttime, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "starttime", starttime, GETN, SETFLOAT);

    // For mods - user data fields
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "iuser1", iuser1, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "iuser2", iuser2, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "iuser3", iuser3, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "iuser4", iuser4, GETN, SETINT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "fuser1", fuser1, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "fuser2", fuser2, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "fuser3", fuser3, GETN, SETFLOAT);
    ACCESSOR_T(entity_state_s, unwrapEntityState_internal, templ, "fuser4", fuser4, GETN, SETFLOAT);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "vuser1", vuser1, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "vuser2", vuser2, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "vuser3", vuser3, GETVEC3, SETVEC3);
    ACCESSORL_T(entity_state_s, unwrapEntityState_internal, templ, "vuser4", vuser4, GETVEC3, SETVEC3);
    
    entityStateTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapEntityState(v8::Isolate* isolate, void* entitystate) {
    v8::Locker locker(isolate);
    if (!entitystate) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedEntityStates.find(entitystate) != wrappedEntityStates.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedEntityStates[entitystate]);
    }
    
    // Create template if not initialized
    if (entityStateTemplate.IsEmpty()) {
        createEntityStateTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = entityStateTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, entitystate);
    
    wrappedEntityStates[entitystate].Reset(isolate, obj);
    return obj;
}

void* unwrapEntityState(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapEntityState_internal(isolate, obj);
}

}