#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapNetAdr(v8::Isolate* isolate, void* netadr) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!netadr) {
        return v8::Null(isolate);
    }
    
    netadr_t* addr = static_cast<netadr_t*>(netadr);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "type").ToLocalChecked(), 
        v8::Number::New(isolate, addr->type)).Check();
    
    // Create IP array
    v8::Local<v8::Array> ipArray = v8::Array::New(isolate, 4);
    for (int i = 0; i < 4; i++) {
        ipArray->Set(context, i, v8::Number::New(isolate, addr->ip[i])).Check();
    }
    obj->Set(context, v8::String::NewFromUtf8(isolate, "ip").ToLocalChecked(), ipArray).Check();
    
    // Create IPX array
    v8::Local<v8::Array> ipxArray = v8::Array::New(isolate, 10);
    for (int i = 0; i < 10; i++) {
        ipxArray->Set(context, i, v8::Number::New(isolate, addr->ipx[i])).Check();
    }
    obj->Set(context, v8::String::NewFromUtf8(isolate, "ipx").ToLocalChecked(), ipxArray).Check();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "port").ToLocalChecked(), 
        v8::Number::New(isolate, addr->port)).Check();
    
    return obj;
}

void* unwrapNetAdr(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    netadr_t* addr = new netadr_t();
    
    // Extract type
    v8::Local<v8::Value> typeVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "type").ToLocalChecked()).ToLocalChecked();
    if (typeVal->IsNumber()) {
        addr->type = static_cast<netadrtype_t>(typeVal->Int32Value(context).FromJust());
    }
    
    // Extract IP array
    v8::Local<v8::Value> ipVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "ip").ToLocalChecked()).ToLocalChecked();
    if (ipVal->IsArray()) {
        v8::Local<v8::Array> ipArray = ipVal.As<v8::Array>();
        for (int i = 0; i < 4 && i < ipArray->Length(); i++) {
            v8::Local<v8::Value> element = ipArray->Get(context, i).ToLocalChecked();
            if (element->IsNumber()) {
                addr->ip[i] = element->Uint32Value(context).FromJust() & 0xFF;
            }
        }
    }
    
    // Extract IPX array
    v8::Local<v8::Value> ipxVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "ipx").ToLocalChecked()).ToLocalChecked();
    if (ipxVal->IsArray()) {
        v8::Local<v8::Array> ipxArray = ipxVal.As<v8::Array>();
        for (int i = 0; i < 10 && i < ipxArray->Length(); i++) {
            v8::Local<v8::Value> element = ipxArray->Get(context, i).ToLocalChecked();
            if (element->IsNumber()) {
                addr->ipx[i] = element->Uint32Value(context).FromJust() & 0xFF;
            }
        }
    }
    
    // Extract port
    v8::Local<v8::Value> portVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "port").ToLocalChecked()).ToLocalChecked();
    if (portVal->IsNumber()) {
        addr->port = portVal->Uint32Value(context).FromJust() & 0xFFFF;
    }
    
    return addr;
}

}