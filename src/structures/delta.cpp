#include "structures.hpp"
#include "../util/convert.hpp"

namespace structures {

v8::Local<v8::Value> wrapDelta(v8::Isolate* isolate, void* delta) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!delta) {
        return v8::Null(isolate);
    }
    
    return v8::External::New(isolate, delta);
}

void* unwrapDelta(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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