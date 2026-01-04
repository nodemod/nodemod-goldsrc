#include "structures.hpp"
#include "common_macros.hpp"
#include "extdll.h"
#include <unordered_map>

extern globalvars_t *gpGlobals;

namespace structures {

v8::Eternal<v8::ObjectTemplate> traceResultTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedTraceResults;

TraceResult* unwrapTraceResult_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<TraceResult*>(field);
}

void createTraceResultTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Boolean flags (int fields used as booleans)
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "allSolid").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
            } else {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), trace->fAllSolid != 0));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) return;
            trace->fAllSolid = value->BooleanValue(info.GetIsolate()) ? 1 : 0;
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "startSolid").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
            } else {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), trace->fStartSolid != 0));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) return;
            trace->fStartSolid = value->BooleanValue(info.GetIsolate()) ? 1 : 0;
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "inOpen").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
            } else {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), trace->fInOpen != 0));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) return;
            trace->fInOpen = value->BooleanValue(info.GetIsolate()) ? 1 : 0;
        });
    
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "inWater").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
            } else {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), trace->fInWater != 0));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) return;
            trace->fInWater = value->BooleanValue(info.GetIsolate()) ? 1 : 0;
        });
    
    // Float fields
    ACCESSOR_T(TraceResult, unwrapTraceResult_internal, templ, "fraction", flFraction, GETN, SETFLOAT);
    ACCESSOR_T(TraceResult, unwrapTraceResult_internal, templ, "planeDist", flPlaneDist, GETN, SETFLOAT);
    
    // Vector fields
    ACCESSORL_T(TraceResult, unwrapTraceResult_internal, templ, "endPos", vecEndPos, GETVEC3, SETVEC3);
    ACCESSORL_T(TraceResult, unwrapTraceResult_internal, templ, "planeNormal", vecPlaneNormal, GETVEC3, SETVEC3);
    
    // Entity field
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "hit").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr || trace->pHit == nullptr) {
                info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
            } else {
                info.GetReturnValue().Set(wrapEntity(info.GetIsolate(), trace->pHit));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TraceResult *trace = unwrapTraceResult_internal(info.GetIsolate(), info.Holder());
            if (trace == nullptr) return;
            if (value->IsNull() || value->IsUndefined()) {
                trace->pHit = nullptr;
            } else {
                trace->pHit = unwrapEntity(info.GetIsolate(), value);
            }
        });
    
    // Integer hitgroup field
    ACCESSOR_T(TraceResult, unwrapTraceResult_internal, templ, "hitGroup", iHitgroup, GETN, SETINT);
    
    traceResultTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapTraceResult(v8::Isolate* isolate, TraceResult* trace) {
    v8::Locker locker(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    
    if (!trace) {
        return v8::Null(isolate);
    }
    
    // Create plain JavaScript object with copied values (not pointer-based accessors)
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "allSolid").ToLocalChecked(), v8::Boolean::New(isolate, trace->fAllSolid)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "startSolid").ToLocalChecked(), v8::Boolean::New(isolate, trace->fStartSolid)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "inOpen").ToLocalChecked(), v8::Boolean::New(isolate, trace->fInOpen)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "inWater").ToLocalChecked(), v8::Boolean::New(isolate, trace->fInWater)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fraction").ToLocalChecked(), v8::Number::New(isolate, trace->flFraction)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "planeDist").ToLocalChecked(), v8::Number::New(isolate, trace->flPlaneDist)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "hitGroup").ToLocalChecked(), v8::Integer::New(isolate, trace->iHitgroup)).Check();
    
    // Create arrays for vectors
    v8::Local<v8::Array> endPos = v8::Array::New(isolate, 3);
    endPos->Set(context, 0, v8::Number::New(isolate, trace->vecEndPos.x)).Check();
    endPos->Set(context, 1, v8::Number::New(isolate, trace->vecEndPos.y)).Check();
    endPos->Set(context, 2, v8::Number::New(isolate, trace->vecEndPos.z)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "endPos").ToLocalChecked(), endPos).Check();
    
    v8::Local<v8::Array> planeNormal = v8::Array::New(isolate, 3);
    planeNormal->Set(context, 0, v8::Number::New(isolate, trace->vecPlaneNormal.x)).Check();
    planeNormal->Set(context, 1, v8::Number::New(isolate, trace->vecPlaneNormal.y)).Check();
    planeNormal->Set(context, 2, v8::Number::New(isolate, trace->vecPlaneNormal.z)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "planeNormal").ToLocalChecked(), planeNormal).Check();
    
    // Handle hit entity - validate pointer is within edict array range
    // before calling wrapEntity which calls IndexOfEdict
    bool validHit = false;
    if (trace->pHit && gpGlobals && gpGlobals->maxEntities > 0) {
        // Get worldspawn (edict 0) to calculate array bounds
        edict_t* worldspawn = (*g_engfuncs.pfnPEntityOfEntIndex)(0);
        if (worldspawn) {
            // Check if pHit is within the valid edict array range
            ptrdiff_t offset = trace->pHit - worldspawn;
            if (offset >= 0 && offset < gpGlobals->maxEntities) {
                // Pointer is within bounds - now safe to dereference and check if freed
                if (!trace->pHit->free) {
                    validHit = true;
                }
            }
        }
    }

    if (validHit) {
        obj->Set(context, v8::String::NewFromUtf8(isolate, "hit").ToLocalChecked(), wrapEntity(isolate, trace->pHit)).Check();
    } else {
        obj->Set(context, v8::String::NewFromUtf8(isolate, "hit").ToLocalChecked(), v8::Null(isolate)).Check();
    }
    
    return obj;
}

TraceResult* unwrapTraceResult(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapTraceResult_internal(isolate, obj);
}

}