#include "structures.hpp"
#include "common_macros.hpp"
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> saveRestoreDataTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedSaveRestoreDatas;

SAVERESTOREDATA* unwrapSaveRestoreData_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<SAVERESTOREDATA*>(field);
}

void createSaveRestoreDataTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Integer fields
    ACCESSOR_T(SAVERESTOREDATA, unwrapSaveRestoreData_internal, templ, "size", size, GETN, SETINT);
    ACCESSOR_T(SAVERESTOREDATA, unwrapSaveRestoreData_internal, templ, "bufferSize", bufferSize, GETN, SETINT);
    ACCESSOR_T(SAVERESTOREDATA, unwrapSaveRestoreData_internal, templ, "tokenSize", tokenSize, GETN, SETINT);
    ACCESSOR_T(SAVERESTOREDATA, unwrapSaveRestoreData_internal, templ, "tokenCount", tokenCount, GETN, SETINT);
    
    // String field for current map name
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "szCurrentMapName").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            SAVERESTOREDATA *srd = unwrapSaveRestoreData_internal(info.GetIsolate(), info.Holder());
            if (srd == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), srd->szCurrentMapName).ToLocalChecked());
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            SAVERESTOREDATA *srd = unwrapSaveRestoreData_internal(info.GetIsolate(), info.Holder());
            if (srd == nullptr || !value->IsString()) return;
            
            v8::String::Utf8Value str(info.GetIsolate(), value);
            if (str.length() < sizeof(srd->szCurrentMapName)) {
                strncpy(srd->szCurrentMapName, *str, sizeof(srd->szCurrentMapName) - 1);
                srd->szCurrentMapName[sizeof(srd->szCurrentMapName) - 1] = '\0';
            }
        });
    
    saveRestoreDataTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapSaveRestoreData(v8::Isolate* isolate, void* savedata) {
    v8::Locker locker(isolate);
    if (!savedata) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedSaveRestoreDatas.find(savedata) != wrappedSaveRestoreDatas.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedSaveRestoreDatas[savedata]);
    }
    
    // Create template if not initialized
    if (saveRestoreDataTemplate.IsEmpty()) {
        createSaveRestoreDataTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = saveRestoreDataTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, savedata);
    
    wrappedSaveRestoreDatas[savedata].Reset(isolate, obj);
    return obj;
}

void* unwrapSaveRestoreData(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapSaveRestoreData_internal(isolate, obj);
}

}