#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapPlayerMove(v8::Isolate* isolate, void* playermove) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!playermove) {
        return v8::Null(isolate);
    }
    
    playermove_t* pm = static_cast<playermove_t*>(playermove);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "player_index").ToLocalChecked(), 
        v8::Number::New(isolate, pm->player_index)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "server").ToLocalChecked(), 
        v8::Boolean::New(isolate, pm->server)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "multiplayer").ToLocalChecked(), 
        v8::Boolean::New(isolate, pm->multiplayer)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "time").ToLocalChecked(), 
        v8::Number::New(isolate, pm->time)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "frametime").ToLocalChecked(), 
        v8::Number::New(isolate, pm->frametime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked(), 
        utils::vect2js(isolate, pm->origin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "angles").ToLocalChecked(), 
        utils::vect2js(isolate, pm->angles)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "velocity").ToLocalChecked(), 
        utils::vect2js(isolate, pm->velocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flags").ToLocalChecked(), 
        v8::Number::New(isolate, pm->flags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "movetype").ToLocalChecked(), 
        v8::Number::New(isolate, pm->movetype)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "onground").ToLocalChecked(), 
        v8::Number::New(isolate, pm->onground)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "waterlevel").ToLocalChecked(), 
        v8::Number::New(isolate, pm->waterlevel)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "maxspeed").ToLocalChecked(), 
        v8::Number::New(isolate, pm->maxspeed)).Check();
    
    return obj;
}

void* unwrapPlayerMove(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (obj->IsNull() || obj->IsUndefined()) {
        return nullptr;
    }
    
    if (obj->IsExternal()) {
        v8::Local<v8::External> ext = obj.As<v8::External>();
        return ext->Value();
    }
    
    if (!obj->IsObject()) {
        return nullptr;
    }
    
    v8::Local<v8::Object> jsObj = obj.As<v8::Object>();
    auto context = isolate->GetCurrentContext();
    playermove_t* pm = new playermove_t();
    
    v8::Local<v8::Value> val;
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "player_index").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) pm->player_index = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked()).ToLocalChecked();
    if (val->IsArray()) utils::js2vect(isolate, val, pm->origin);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "velocity").ToLocalChecked()).ToLocalChecked();
    if (val->IsArray()) utils::js2vect(isolate, val, pm->velocity);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "maxspeed").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) pm->maxspeed = val->NumberValue(context).FromJust();
    
    return pm;
}

}