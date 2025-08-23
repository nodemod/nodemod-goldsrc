#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"
#include "usercmd.h"

namespace structures {

v8::Local<v8::Value> wrapUserCmd(v8::Isolate* isolate, void* usercmd) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!usercmd) {
        return v8::Null(isolate);
    }
    
    usercmd_s* cmd = static_cast<usercmd_s*>(usercmd);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "lerp_msec").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->lerp_msec)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "msec").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->msec)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "viewangles").ToLocalChecked(), 
        utils::vect2js(isolate, cmd->viewangles)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "forwardmove").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->forwardmove)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "sidemove").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->sidemove)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "upmove").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->upmove)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "lightlevel").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->lightlevel)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "buttons").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->buttons)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "impulse").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->impulse)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "weaponselect").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->weaponselect)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "impact_index").ToLocalChecked(), 
        v8::Number::New(isolate, cmd->impact_index)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "impact_position").ToLocalChecked(), 
        utils::vect2js(isolate, cmd->impact_position)).Check();
    
    return obj;
}

void* unwrapUserCmd(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
    v8::Locker locker(isolate);
    if (obj->IsNull() || obj->IsUndefined()) {
        return nullptr;
    }
    
    if (obj->IsExternal()) {
        v8::Local<v8::External> ext = obj.As<v8::External>();
        return ext->Value();
    }
    
    return nullptr;
}

}