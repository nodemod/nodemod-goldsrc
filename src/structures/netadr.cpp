#include "structures.hpp"
#include "common_macros.hpp"
#include <netadr.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> netadrTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedNetAdrs;

netadr_s* unwrapNetAdr_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<netadr_s*>(field);
}

void createNetAdrTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Type field (netadrtype_t enum) - need custom setter for enum cast
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "type").ToLocalChecked(),
        GETTER_T(netadr_s, unwrapNetAdr_internal, type, GETN),
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            netadr_s *obj = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (obj == NULL) return;
            obj->type = static_cast<netadrtype_t>(value->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked());
        });
    
    // Port field
    ACCESSOR_T(netadr_s, unwrapNetAdr_internal, templ, "port", port, GETN, SETINT);
    
    // IP address array (4 bytes)
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "ip").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate(), 4);
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 4; i++) {
                arr->Set(context, i, v8::Number::New(info.GetIsolate(), addr->ip[i])).Check();
            }
            info.GetReturnValue().Set(arr);
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr || !value->IsArray()) return;
            
            v8::Local<v8::Array> arr = value.As<v8::Array>();
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 4 && i < arr->Length(); i++) {
                v8::Local<v8::Value> element = arr->Get(context, i).ToLocalChecked();
                if (element->IsNumber()) {
                    addr->ip[i] = element->Uint32Value(context).FromJust() & 0xFF;
                }
            }
        });
    
    // IPX address array (10 bytes)
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "ipx").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
                return;
            }
            
            v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate(), 10);
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 10; i++) {
                arr->Set(context, i, v8::Number::New(info.GetIsolate(), addr->ipx[i])).Check();
            }
            info.GetReturnValue().Set(arr);
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr || !value->IsArray()) return;
            
            v8::Local<v8::Array> arr = value.As<v8::Array>();
            auto context = info.GetIsolate()->GetCurrentContext();
            for (int i = 0; i < 10 && i < arr->Length(); i++) {
                v8::Local<v8::Value> element = arr->Get(context, i).ToLocalChecked();
                if (element->IsNumber()) {
                    addr->ipx[i] = element->Uint32Value(context).FromJust() & 0xFF;
                }
            }
        });
    
    // Helper property to get/set IP as a string (e.g., "192.168.1.1")
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "ipString").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "0.0.0.0").ToLocalChecked());
                return;
            }
            
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d.%d.%d.%d", 
                     addr->ip[0], addr->ip[1], addr->ip[2], addr->ip[3]);
            info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), buffer).ToLocalChecked());
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            netadr_s *addr = unwrapNetAdr_internal(info.GetIsolate(), info.Holder());
            if (addr == nullptr || !value->IsString()) return;
            
            v8::String::Utf8Value str(info.GetIsolate(), value);
            if (*str) {
                int ip[4] = {0, 0, 0, 0};
                sscanf(*str, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
                for (int i = 0; i < 4; i++) {
                    addr->ip[i] = ip[i] & 0xFF;
                }
            }
        });
    
    netadrTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapNetAdr(v8::Isolate* isolate, void* netadr) {
    v8::Locker locker(isolate);
    if (!netadr) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedNetAdrs.find(netadr) != wrappedNetAdrs.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedNetAdrs[netadr]);
    }
    
    // Create template if not initialized
    if (netadrTemplate.IsEmpty()) {
        createNetAdrTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = netadrTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, netadr);
    
    wrappedNetAdrs[netadr].Reset(isolate, obj);
    return obj;
}

void* unwrapNetAdr(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapNetAdr_internal(isolate, obj);
}

}