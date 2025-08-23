#include "structures.hpp"
#include "../util/convert.hpp"

namespace structures {

v8::Local<v8::Value> wrapCvar(v8::Isolate* isolate, void* cvar) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!cvar) {
        return v8::Null(isolate);
    }
    
    cvar_t* cvarStruct = static_cast<cvar_t*>(cvar);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "name").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, cvarStruct->name ? cvarStruct->name : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "string").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, cvarStruct->string ? cvarStruct->string : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flags").ToLocalChecked(), 
        v8::Number::New(isolate, cvarStruct->flags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "value").ToLocalChecked(), 
        v8::Number::New(isolate, cvarStruct->value)).Check();
    
    return obj;
}

void* unwrapCvar(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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