#include "structures.hpp"
#include "common_macros.hpp"
#include <cvardef.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> cvarTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedCvars;

cvar_t* unwrapCvar_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<cvar_t*>(field);
}

void createCvarTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // String fields - read-only since they're const char* / char*
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "name").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            cvar_t *cvar = unwrapCvar_internal(info.GetIsolate(), info.Holder());
            if (cvar == nullptr || cvar->name == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), cvar->name).ToLocalChecked());
            }
        });
    
    // String value - this could potentially be settable
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "string").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            cvar_t *cvar = unwrapCvar_internal(info.GetIsolate(), info.Holder());
            if (cvar == nullptr || cvar->string == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), cvar->string).ToLocalChecked());
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            cvar_t *cvar = unwrapCvar_internal(info.GetIsolate(), info.Holder());
            if (cvar == nullptr) return;
            
            // Note: This modifies the cvar string pointer directly
            // In a real implementation, you'd want to use the engine's Cvar_Set function
            v8::String::Utf8Value str(info.GetIsolate(), value);
            if (*str && cvar->string) {
                // WARNING: This is a simplified implementation
                // Proper implementation would allocate memory and handle the string properly
                // For now, we'll leave it read-only by not actually modifying
                // TODO: Use engine's Cvar_Set or similar function
            }
        });
    
    // Integer flags field
    ACCESSOR_T(cvar_t, unwrapCvar_internal, templ, "flags", flags, GETN, SETINT);
    
    // Float value field
    ACCESSOR_T(cvar_t, unwrapCvar_internal, templ, "value", value, GETN, SETFLOAT);
    
    // Next cvar in the linked list
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "next").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            cvar_t *cvar = unwrapCvar_internal(info.GetIsolate(), info.Holder());
            if (cvar == nullptr || cvar->next == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapCvar(info.GetIsolate(), cvar->next));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            cvar_t *cvar = unwrapCvar_internal(info.GetIsolate(), info.Holder());
            if (cvar == nullptr) return;
            
            if (value->IsNull() || value->IsUndefined()) {
                cvar->next = nullptr;
            } else {
                cvar->next = static_cast<cvar_t*>(unwrapCvar(info.GetIsolate(), value));
            }
        });
    
    cvarTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapCvar(v8::Isolate* isolate, void* cvar) {
    v8::Locker locker(isolate);
    if (!cvar) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedCvars.find(cvar) != wrappedCvars.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedCvars[cvar]);
    }
    
    // Create template if not initialized
    if (cvarTemplate.IsEmpty()) {
        createCvarTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = cvarTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, cvar);
    
    wrappedCvars[cvar].Reset(isolate, obj);
    return obj;
}

void* unwrapCvar(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapCvar_internal(isolate, obj);
}

}