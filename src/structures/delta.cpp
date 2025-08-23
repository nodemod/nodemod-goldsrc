#include "structures.hpp"
#include "common_macros.hpp"
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> deltaTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedDeltas;

// Note: delta_s structure is opaque in the engine headers
// We only have access to it through engine functions
// This provides a minimal wrapper for the pointer
struct delta_s* unwrapDelta_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<struct delta_s*>(field);
}

void createDeltaTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Since delta_s is opaque, we can only provide utility methods
    // that use the engine functions to interact with delta fields
    
    // Add a toString method for debugging
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "toString").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            struct delta_s *delta = unwrapDelta_internal(info.GetIsolate(), info.Holder());
            if (delta == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "[Delta: null]").ToLocalChecked());
            } else {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "[Delta: %p]", delta);
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), buffer).ToLocalChecked());
            }
        });
    
    // Add a method to check if the delta is valid (non-null)
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "isValid").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            struct delta_s *delta = unwrapDelta_internal(info.GetIsolate(), info.Holder());
            info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), delta != nullptr));
        });
    
    // Add pointer value as a property for debugging
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "pointer").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            struct delta_s *delta = unwrapDelta_internal(info.GetIsolate(), info.Holder());
            if (delta == nullptr) {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), 0));
            } else {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), reinterpret_cast<uintptr_t>(delta)));
            }
        });
    
    deltaTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapDelta(v8::Isolate* isolate, void* delta) {
    v8::Locker locker(isolate);
    if (!delta) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedDeltas.find(delta) != wrappedDeltas.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedDeltas[delta]);
    }
    
    // Create template if not initialized
    if (deltaTemplate.IsEmpty()) {
        createDeltaTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = deltaTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, delta);
    
    wrappedDeltas[delta].Reset(isolate, obj);
    return obj;
}

void* unwrapDelta(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapDelta_internal(isolate, obj);
}

}