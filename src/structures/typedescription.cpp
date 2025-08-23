#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapTypeDescription(v8::Isolate* isolate, void* typedesc) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!typedesc) {
        return v8::Null(isolate);
    }
    
    TYPEDESCRIPTION* td = static_cast<TYPEDESCRIPTION*>(typedesc);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fieldType").ToLocalChecked(), 
        v8::Number::New(isolate, td->fieldType)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fieldName").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, td->fieldName ? td->fieldName : "").ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fieldOffset").ToLocalChecked(), 
        v8::Number::New(isolate, td->fieldOffset)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fieldSize").ToLocalChecked(), 
        v8::Number::New(isolate, td->fieldSize)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flags").ToLocalChecked(), 
        v8::Number::New(isolate, td->flags)).Check();
    
    return obj;
}

void* unwrapTypeDescription(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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