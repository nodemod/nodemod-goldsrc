#include "structures.hpp"
#include "common_macros.hpp"
#include <pm_defs.h>
#include <pm_movevars.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> playerMoveTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedPlayerMoves;

playermove_s* unwrapPlayerMove_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<playermove_s*>(field);
}

void createPlayerMoveTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Integer fields
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "player_index", player_index, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "server", server, GETBOOL, SETBOOL);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "multiplayer", multiplayer, GETBOOL, SETBOOL);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "flags", flags, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "usehull", usehull, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "gravity", gravity, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "maxspeed", maxspeed, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "clientmaxspeed", clientmaxspeed, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "movetype", movetype, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "friction", friction, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "oldbuttons", oldbuttons, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "onground", onground, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "waterlevel", waterlevel, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "watertype", watertype, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "oldwaterlevel", oldwaterlevel, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "chtexturetype", chtexturetype, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "dead", dead, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "deadflag", deadflag, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "spectator", spectator, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "bInDuck", bInDuck, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "flTimeStepSound", flTimeStepSound, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "iStepLeft", iStepLeft, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "numphysent", numphysent, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "nummoveent", nummoveent, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "numvisent", numvisent, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "numtouch", numtouch, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "runfuncs", runfuncs, GETBOOL, SETBOOL);
    
    // Float fields
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "time", time, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "frametime", frametime, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "flFallVelocity", flFallVelocity, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "flSwimTime", flSwimTime, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "flDuckTime", flDuckTime, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "fuser1", fuser1, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "fuser2", fuser2, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "fuser3", fuser3, GETN, SETFLOAT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "fuser4", fuser4, GETN, SETFLOAT);
    
    // Vector fields
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "forward", forward, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "right", right, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "up", up, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "origin", origin, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "angles", angles, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "oldangles", oldangles, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "velocity", velocity, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "movedir", movedir, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "basevelocity", basevelocity, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "view_ofs", view_ofs, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "punchangle", punchangle, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "vuser1", vuser1, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "vuser2", vuser2, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "vuser3", vuser3, GETVEC3, SETVEC3);
    ACCESSORL_T(playermove_s, unwrapPlayerMove_internal, templ, "vuser4", vuser4, GETVEC3, SETVEC3);
    
    // User integer fields
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "iuser1", iuser1, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "iuser2", iuser2, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "iuser3", iuser3, GETN, SETINT);
    ACCESSOR_T(playermove_s, unwrapPlayerMove_internal, templ, "iuser4", iuser4, GETN, SETINT);
    
    // String field - sztexturename
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "sztexturename").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), pm->sztexturename).ToLocalChecked());
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr || !value->IsString()) return;
            
            v8::String::Utf8Value str(info.GetIsolate(), value);
            if (*str) {
                strncpy(pm->sztexturename, *str, 255);
                pm->sztexturename[255] = '\0';
            }
        });
    
    // String field - physinfo
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "physinfo").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), pm->physinfo).ToLocalChecked());
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr || !value->IsString()) return;
            
            v8::String::Utf8Value str(info.GetIsolate(), value);
            if (*str) {
                strncpy(pm->physinfo, *str, MAX_PHYSINFO_STRING - 1);
                pm->physinfo[MAX_PHYSINFO_STRING - 1] = '\0';
            }
        });
    
    // cmd field (usercmd) - returns a wrapped UserCmd object
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "cmd").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapUserCmd(info.GetIsolate(), &pm->cmd));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr) return;
            
            if (value->IsNull() || value->IsUndefined()) {
                memset(&pm->cmd, 0, sizeof(pm->cmd));
            } else {
                // Copy from another UserCmd object
                usercmd_s* srcCmd = static_cast<usercmd_s*>(unwrapUserCmd(info.GetIsolate(), value));
                if (srcCmd != nullptr) {
                    memcpy(&pm->cmd, srcCmd, sizeof(pm->cmd));
                }
            }
        });
    
    // movevars pointer - can be set to another movevars pointer
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "movevars").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr || pm->movevars == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                // Return movevars as an object with its fields
                v8::Local<v8::Object> mv = v8::Object::New(info.GetIsolate());
                auto context = info.GetIsolate()->GetCurrentContext();
                auto movevars = pm->movevars;
                
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "gravity").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->gravity)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "stopspeed").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->stopspeed)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "maxspeed").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->maxspeed)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "spectatormaxspeed").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->spectatormaxspeed)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "accelerate").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->accelerate)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "airaccelerate").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->airaccelerate)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "wateraccelerate").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->wateraccelerate)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "friction").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->friction)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "edgefriction").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->edgefriction)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "waterfriction").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->waterfriction)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "bounce").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->bounce)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "stepsize").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->stepsize)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "maxvelocity").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->maxvelocity)).Check();
                mv->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "footsteps").ToLocalChecked(),
                    v8::Number::New(info.GetIsolate(), movevars->footsteps)).Check();
                
                info.GetReturnValue().Set(mv);
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            playermove_s *pm = unwrapPlayerMove_internal(info.GetIsolate(), info.Holder());
            if (pm == nullptr) return;
            
            if (value->IsNull() || value->IsUndefined()) {
                pm->movevars = nullptr;
            } else {
                // Note: This is a simplified implementation that sets the pointer to null
                // In a real implementation, you'd need a proper movevars wrapper or
                // extract movevars pointer from a wrapped movevars object
                // For now, we'll leave the functionality limited to prevent crashes
                pm->movevars = nullptr;
            }
        });
    
    playerMoveTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapPlayerMove(v8::Isolate* isolate, void* playermove) {
    v8::Locker locker(isolate);
    if (!playermove) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedPlayerMoves.find(playermove) != wrappedPlayerMoves.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedPlayerMoves[playermove]);
    }
    
    // Create template if not initialized
    if (playerMoveTemplate.IsEmpty()) {
        createPlayerMoveTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = playerMoveTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, playermove);
    
    wrappedPlayerMoves[playermove].Reset(isolate, obj);
    return obj;
}

void* unwrapPlayerMove(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapPlayerMove_internal(isolate, obj);
}

}