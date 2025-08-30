#include "structures.hpp"
#include "common_macros.hpp"
#include <eiface.h>
#include <unordered_map>
#include <cstring>

namespace structures {

v8::Eternal<v8::ObjectTemplate> keyValueDataTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedKeyValueData;

KeyValueData* createKeyValueDataFromJS(v8::Isolate* isolate, const v8::Local<v8::Object>& jsObj) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    KeyValueData* kvd = new KeyValueData();
    memset(kvd, 0, sizeof(KeyValueData));
    
    // Get string properties and allocate C strings
    auto context = isolate->GetCurrentContext();
    
    // szClassName
    auto classNameKey = v8::String::NewFromUtf8(isolate, "szClassName").ToLocalChecked();
    if (jsObj->Has(context, classNameKey).FromMaybe(false)) {
        auto classNameVal = jsObj->Get(context, classNameKey).ToLocalChecked();
        if (classNameVal->IsString()) {
            v8::String::Utf8Value className(isolate, classNameVal);
            kvd->szClassName = strdup(*className);
        }
    }
    
    // szKeyName
    auto keyNameKey = v8::String::NewFromUtf8(isolate, "szKeyName").ToLocalChecked();
    if (jsObj->Has(context, keyNameKey).FromMaybe(false)) {
        auto keyNameVal = jsObj->Get(context, keyNameKey).ToLocalChecked();
        if (keyNameVal->IsString()) {
            v8::String::Utf8Value keyName(isolate, keyNameVal);
            kvd->szKeyName = strdup(*keyName);
        }
    }
    
    // szValue
    auto valueKey = v8::String::NewFromUtf8(isolate, "szValue").ToLocalChecked();
    if (jsObj->Has(context, valueKey).FromMaybe(false)) {
        auto valueVal = jsObj->Get(context, valueKey).ToLocalChecked();
        if (valueVal->IsString()) {
            v8::String::Utf8Value value(isolate, valueVal);
            kvd->szValue = strdup(*value);
        }
    }
    
    // fHandled
    auto handledKey = v8::String::NewFromUtf8(isolate, "fHandled").ToLocalChecked();
    if (jsObj->Has(context, handledKey).FromMaybe(false)) {
        auto handledVal = jsObj->Get(context, handledKey).ToLocalChecked();
        if (handledVal->IsNumber()) {
            kvd->fHandled = handledVal->Int32Value(context).FromMaybe(0);
        }
    }
    
    
    return kvd;
}

KeyValueData* unwrapKeyValueData_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto objectLocal = object.ToLocalChecked();
    
    // Check if it's a wrapped KeyValueData (has internal field)
    if (objectLocal->InternalFieldCount() > 0) {
        auto field = objectLocal->GetAlignedPointerFromInternalField(0);
        return static_cast<KeyValueData*>(field);
    }
    
    // If it's a plain JavaScript object, create a new KeyValueData
    return createKeyValueDataFromJS(isolate, objectLocal);
}

void createKeyValueDataTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // String fields - these are const char* so they're read-only from JavaScript
    // szClassName - read-only
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "szClassName").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            KeyValueData *kvd = unwrapKeyValueData_internal(info.GetIsolate(), info.Holder());
            if (kvd == nullptr || kvd->szClassName == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), kvd->szClassName).ToLocalChecked());
            }
        });
    
    // szKeyName - read-only
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "szKeyName").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            KeyValueData *kvd = unwrapKeyValueData_internal(info.GetIsolate(), info.Holder());
            if (kvd == nullptr || kvd->szKeyName == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), kvd->szKeyName).ToLocalChecked());
            }
        });
    
    // szValue - read-only
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "szValue").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            KeyValueData *kvd = unwrapKeyValueData_internal(info.GetIsolate(), info.Holder());
            if (kvd == nullptr || kvd->szValue == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), kvd->szValue).ToLocalChecked());
            }
        });
    
    // fHandled - this is an output field that DLL can set
    ACCESSOR_T(KeyValueData, unwrapKeyValueData_internal, templ, "fHandled", fHandled, GETN, SETINT);
    
    keyValueDataTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapKeyValueData(v8::Isolate* isolate, void* keyvalue) {
    v8::Locker locker(isolate);
    if (!keyvalue) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedKeyValueData.find(keyvalue) != wrappedKeyValueData.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedKeyValueData[keyvalue]);
    }
    
    // Create template if not initialized
    if (keyValueDataTemplate.IsEmpty()) {
        createKeyValueDataTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = keyValueDataTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, keyvalue);
    
    wrappedKeyValueData[keyvalue].Reset(isolate, obj);
    return obj;
}

void* unwrapKeyValueData(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapKeyValueData_internal(isolate, obj);
}

}