#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapKeyValueData(v8::Isolate* isolate, void* keyvalue) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!keyvalue) {
        return v8::Null(isolate);
    }
    
    KeyValueData* kvd = static_cast<KeyValueData*>(keyvalue);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "szClassName").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, kvd->szClassName ? kvd->szClassName : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "szKeyName").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, kvd->szKeyName ? kvd->szKeyName : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "szValue").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, kvd->szValue ? kvd->szValue : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fHandled").ToLocalChecked(), 
        v8::Number::New(isolate, kvd->fHandled)).Check();
    
    return obj;
}

void* unwrapKeyValueData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    KeyValueData* kvd = new KeyValueData();
    
    v8::Local<v8::Value> val;
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "fHandled").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) kvd->fHandled = val->Int32Value(context).FromJust();
    
    return kvd;
}

}