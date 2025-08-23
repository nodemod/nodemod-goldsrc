#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapTraceResult(v8::Isolate* isolate, TraceResult* trace) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!trace) {
        return v8::Null(isolate);
    }

    // Create a JavaScript object to represent TraceResult
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    
    // Set TraceResult properties
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "allSolid"), 
        v8::Boolean::New(isolate, trace->fAllSolid)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "startSolid"), 
        v8::Boolean::New(isolate, trace->fStartSolid)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "inOpen"), 
        v8::Boolean::New(isolate, trace->fInOpen)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "inWater"), 
        v8::Boolean::New(isolate, trace->fInWater)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "fraction"), 
        v8::Number::New(isolate, trace->flFraction)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "endPos"), 
        utils::vect2js(isolate, trace->vecEndPos)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "planeDist"), 
        v8::Number::New(isolate, trace->flPlaneDist)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "planeNormal"), 
        utils::vect2js(isolate, trace->vecPlaneNormal)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "hit"), 
        trace->pHit ? wrapEntity(isolate, trace->pHit) : v8::Null(isolate)).Check();
    
    obj->Set(isolate->GetCurrentContext(), 
        convert::str2js(isolate, "hitGroup"), 
        v8::Number::New(isolate, trace->iHitgroup)).Check();

    return obj;
}

TraceResult* unwrapTraceResult(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (obj->IsNull() || obj->IsUndefined() || !obj->IsObject()) {
        return nullptr;
    }

    // For now, return a pointer to the External data if it's an External object
    if (obj->IsExternal()) {
        v8::Local<v8::External> ext = obj.As<v8::External>();
        return static_cast<TraceResult*>(ext->Value());
    }

    // TODO: Implement full JS object to TraceResult conversion if needed
    // For now, just return nullptr as most engine functions pass TraceResult* as output parameters
    return nullptr;
}

}