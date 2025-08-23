#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapSaveRestoreData(v8::Isolate* isolate, void* savedata) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!savedata) {
        return v8::Null(isolate);
    }
    
    SAVERESTOREDATA* srd = static_cast<SAVERESTOREDATA*>(savedata);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "size").ToLocalChecked(), 
        v8::Number::New(isolate, srd->size)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "bufferSize").ToLocalChecked(), 
        v8::Number::New(isolate, srd->bufferSize)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "tokenSize").ToLocalChecked(), 
        v8::Number::New(isolate, srd->tokenSize)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "tokenCount").ToLocalChecked(), 
        v8::Number::New(isolate, srd->tokenCount)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "szCurrentMapName").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, srd->szCurrentMapName).ToLocalChecked()).Check();
    
    return obj;
}

void* unwrapSaveRestoreData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
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