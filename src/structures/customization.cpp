#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapCustomization(v8::Isolate* isolate, void* customization) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!customization) {
        return v8::Null(isolate);
    }
    
    customization_t* custom = static_cast<customization_t*>(customization);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "bInUse").ToLocalChecked(), 
        v8::Boolean::New(isolate, custom->bInUse)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "bTranslated").ToLocalChecked(), 
        v8::Boolean::New(isolate, custom->bTranslated)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "nUserData1").ToLocalChecked(), 
        v8::Number::New(isolate, custom->nUserData1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "nUserData2").ToLocalChecked(), 
        v8::Number::New(isolate, custom->nUserData2)).Check();
    
    return obj;
}

void* unwrapCustomization(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    customization_t* custom = new customization_t();
    
    v8::Local<v8::Value> val;
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "bInUse").ToLocalChecked()).ToLocalChecked();
    if (val->IsBoolean()) custom->bInUse = val->BooleanValue(isolate);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "bTranslated").ToLocalChecked()).ToLocalChecked();
    if (val->IsBoolean()) custom->bTranslated = val->BooleanValue(isolate);
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "nUserData1").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) custom->nUserData1 = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "nUserData2").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) custom->nUserData2 = val->Int32Value(context).FromJust();
    
    return custom;
}

}