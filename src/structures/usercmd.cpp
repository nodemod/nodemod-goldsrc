#include "structures.hpp"
#include "common_macros.hpp"
#include <usercmd.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> userCmdTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedUserCmds;

usercmd_t* unwrapUserCmd_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<usercmd_t*>(field);
}

void createUserCmdTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Short/byte fields (treated as integers)
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "lerp_msec", lerp_msec, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "msec", msec, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "lightlevel", lightlevel, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "buttons", buttons, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "impulse", impulse, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "weaponselect", weaponselect, GETN, SETINT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "impact_index", impact_index, GETN, SETINT);
    
    // Float fields for movement
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "forwardmove", forwardmove, GETN, SETFLOAT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "sidemove", sidemove, GETN, SETFLOAT);
    ACCESSOR_T(usercmd_t, unwrapUserCmd_internal, templ, "upmove", upmove, GETN, SETFLOAT);
    
    // Vector fields
    ACCESSORL_T(usercmd_t, unwrapUserCmd_internal, templ, "viewangles", viewangles, GETVEC3, SETVEC3);
    ACCESSORL_T(usercmd_t, unwrapUserCmd_internal, templ, "impact_position", impact_position, GETVEC3, SETVEC3);
    
    userCmdTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapUserCmd(v8::Isolate* isolate, void* usercmd) {
    v8::Locker locker(isolate);
    if (!usercmd) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedUserCmds.find(usercmd) != wrappedUserCmds.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedUserCmds[usercmd]);
    }
    
    // Create template if not initialized
    if (userCmdTemplate.IsEmpty()) {
        createUserCmdTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = userCmdTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, usercmd);
    
    wrappedUserCmds[usercmd].Reset(isolate, obj);
    return obj;
}

void* unwrapUserCmd(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapUserCmd_internal(isolate, obj);
}

}