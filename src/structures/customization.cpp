#include "structures.hpp"
#include "common_macros.hpp"
#include <custom.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> customizationTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedCustomizations;

customization_t* unwrapCustomization_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<customization_t*>(field);
}

void createCustomizationTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Boolean fields
    ACCESSOR_T(customization_t, unwrapCustomization_internal, templ, "bInUse", bInUse, GETBOOL, SETBOOL);
    ACCESSOR_T(customization_t, unwrapCustomization_internal, templ, "bTranslated", bTranslated, GETBOOL, SETBOOL);
    
    // Integer fields
    ACCESSOR_T(customization_t, unwrapCustomization_internal, templ, "nUserData1", nUserData1, GETN, SETINT);
    ACCESSOR_T(customization_t, unwrapCustomization_internal, templ, "nUserData2", nUserData2, GETN, SETINT);
    
    // Resource field (nested structure) - read-only for now
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "resource").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            customization_t *custom = unwrapCustomization_internal(info.GetIsolate(), info.Holder());
            if (custom == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Object> resObj = v8::Object::New(info.GetIsolate());
            auto context = info.GetIsolate()->GetCurrentContext();
            
            // Resource fields
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "szFileName").ToLocalChecked(),
                v8::String::NewFromUtf8(info.GetIsolate(), custom->resource.szFileName).ToLocalChecked()).Check();
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "type").ToLocalChecked(),
                v8::Number::New(info.GetIsolate(), custom->resource.type)).Check();
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "nIndex").ToLocalChecked(),
                v8::Number::New(info.GetIsolate(), custom->resource.nIndex)).Check();
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "nDownloadSize").ToLocalChecked(),
                v8::Number::New(info.GetIsolate(), custom->resource.nDownloadSize)).Check();
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "ucFlags").ToLocalChecked(),
                v8::Number::New(info.GetIsolate(), custom->resource.ucFlags)).Check();
            resObj->Set(context, v8::String::NewFromUtf8(info.GetIsolate(), "playernum").ToLocalChecked(),
                v8::Number::New(info.GetIsolate(), custom->resource.playernum)).Check();
                
            info.GetReturnValue().Set(resObj);
        });
    
    // Pointer fields - these are read-only and return null/undefined for safety
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "pInfo").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "pBuffer").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
        });
    
    // pNext - could potentially wrap as another customization object if needed
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "pNext").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            customization_t *custom = unwrapCustomization_internal(info.GetIsolate(), info.Holder());
            if (custom == nullptr || custom->pNext == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapCustomization(info.GetIsolate(), custom->pNext));
            }
        });
    
    customizationTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapCustomization(v8::Isolate* isolate, void* customization) {
    v8::Locker locker(isolate);
    if (!customization) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedCustomizations.find(customization) != wrappedCustomizations.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedCustomizations[customization]);
    }
    
    // Create template if not initialized
    if (customizationTemplate.IsEmpty()) {
        createCustomizationTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = customizationTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, customization);
    
    wrappedCustomizations[customization].Reset(isolate, obj);
    return obj;
}

void* unwrapCustomization(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapCustomization_internal(isolate, obj);
}

}