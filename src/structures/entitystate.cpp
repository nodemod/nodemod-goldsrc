#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapEntityState(v8::Isolate* isolate, void* entitystate) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!entitystate) {
        return v8::Null(isolate);
    }
    
    entity_state_t* state = static_cast<entity_state_t*>(entitystate);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "entityType").ToLocalChecked(), 
        v8::Number::New(isolate, state->entityType)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "number").ToLocalChecked(), 
        v8::Number::New(isolate, state->number)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "msg_time").ToLocalChecked(), 
        v8::Number::New(isolate, state->msg_time)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "messagenum").ToLocalChecked(), 
        v8::Number::New(isolate, state->messagenum)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked(), 
        utils::vect2js(isolate, state->origin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "angles").ToLocalChecked(), 
        utils::vect2js(isolate, state->angles)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "modelindex").ToLocalChecked(), 
        v8::Number::New(isolate, state->modelindex)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "sequence").ToLocalChecked(), 
        v8::Number::New(isolate, state->sequence)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "frame").ToLocalChecked(), 
        v8::Number::New(isolate, state->frame)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "colormap").ToLocalChecked(), 
        v8::Number::New(isolate, state->colormap)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "skin").ToLocalChecked(), 
        v8::Number::New(isolate, state->skin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "solid").ToLocalChecked(), 
        v8::Number::New(isolate, state->solid)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "effects").ToLocalChecked(), 
        v8::Number::New(isolate, state->effects)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "scale").ToLocalChecked(), 
        v8::Number::New(isolate, state->scale)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "eflags").ToLocalChecked(), 
        v8::Number::New(isolate, state->eflags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "rendermode").ToLocalChecked(), 
        v8::Number::New(isolate, state->rendermode)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "renderamt").ToLocalChecked(), 
        v8::Number::New(isolate, state->renderamt)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "renderfx").ToLocalChecked(), 
        v8::Number::New(isolate, state->renderfx)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "movetype").ToLocalChecked(), 
        v8::Number::New(isolate, state->movetype)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "animtime").ToLocalChecked(), 
        v8::Number::New(isolate, state->animtime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "framerate").ToLocalChecked(), 
        v8::Number::New(isolate, state->framerate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "body").ToLocalChecked(), 
        v8::Number::New(isolate, state->body)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "velocity").ToLocalChecked(), 
        utils::vect2js(isolate, state->velocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "mins").ToLocalChecked(), 
        utils::vect2js(isolate, state->mins)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "maxs").ToLocalChecked(), 
        utils::vect2js(isolate, state->maxs)).Check();
    
    return obj;
}

void* unwrapEntityState(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    entity_state_t* state = new entity_state_t();
    
    // Extract numeric fields
    v8::Local<v8::Value> val;
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "entityType").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) state->entityType = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "number").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) state->number = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "msg_time").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) state->msg_time = val->NumberValue(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "messagenum").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) state->messagenum = val->Int32Value(context).FromJust();
    
    // Extract vector fields
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked()).ToLocalChecked();
    if (val->IsArray()) utils::js2vect(isolate, val, state->origin);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "angles").ToLocalChecked()).ToLocalChecked();
    if (val->IsArray()) utils::js2vect(isolate, val, state->angles);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "velocity").ToLocalChecked()).ToLocalChecked();
    if (val->IsArray()) utils::js2vect(isolate, val, state->velocity);
    
    return state;
}

}