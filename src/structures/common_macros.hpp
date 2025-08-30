#pragma once
#include "v8.h"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

extern enginefuncs_t g_engfuncs;

// Type conversion macros for getters
#define GETN(v) v
#define GETVEC3(v) utils::vect2js(info.GetIsolate(), v)
#define GETSTR(v) convert::str2js(info.GetIsolate(), (*g_engfuncs.pfnSzFromIndex)(v))
#define GETBOOL(v) v8::Boolean::New(info.GetIsolate(), v)
#define GETQBOOL(v) v8::Boolean::New(info.GetIsolate(), v != 0)
#define GETBYTE(v) v8::Number::New(info.GetIsolate(), static_cast<int>(v))

// Type conversion macros for setters
#define SETFLOAT(v) v->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked()
#define SETINT(v) v->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked()
#define SETSTR(v) (*g_engfuncs.pfnAllocString)(utils::js2string(info.GetIsolate(), v))
#define SETVEC3(v, f) utils::js2vect(info.GetIsolate(), v8::Local<v8::Array>::Cast(v), f)
#define SETBOOL(v) v->BooleanValue(info.GetIsolate())
#define SETQBOOL(v) (v->BooleanValue(info.GetIsolate()) ? 1 : 0)
#define SETBYTE(v) static_cast<byte>([](v8::Local<v8::Value> val, v8::Isolate* isolate) { \
    int intVal = val->Int32Value(isolate->GetCurrentContext()).ToChecked(); \
    return intVal < 0 ? 0 : (intVal > 255 ? 255 : intVal); \
}(v, info.GetIsolate()))

// Generic getter macro for any structure type
#define GETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, TYPE) \
    ([](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) { \
        STRUCT_TYPE *obj = UNWRAP_FN(info.GetIsolate(), info.Holder()); \
        if (obj == NULL) return; \
        info.GetReturnValue().Set(TYPE(obj->FIELD)); \
    })

// Generic setter macro for any structure type (for simple assignments)
#define SETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, TYPE) \
    ([](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) { \
        STRUCT_TYPE *obj = UNWRAP_FN(info.GetIsolate(), info.Holder()); \
        if (obj == NULL) return; \
        obj->FIELD = TYPE(value); \
    })

// Generic setter macro for any structure type (for functions like SETVEC3)
#define SETTERL_T(STRUCT_TYPE, UNWRAP_FN, FIELD, TYPE) \
    ([](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) { \
        STRUCT_TYPE *obj = UNWRAP_FN(info.GetIsolate(), info.Holder()); \
        if (obj == NULL) return; \
        TYPE(value, obj->FIELD); \
    })

// Accessor macros for any structure type
#define ACCESSOR_T(STRUCT_TYPE, UNWRAP_FN, TEMPLATE, NAME, FIELD, GET, SET) \
    TEMPLATE->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, NAME).ToLocalChecked(), \
        GETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, GET), \
        SETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, SET))

#define ACCESSORL_T(STRUCT_TYPE, UNWRAP_FN, TEMPLATE, NAME, FIELD, GET, SET) \
    TEMPLATE->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, NAME).ToLocalChecked(), \
        GETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, GET), \
        SETTERL_T(STRUCT_TYPE, UNWRAP_FN, FIELD, SET))

// Read-only accessor (no setter)
#define ACCESSOR_RO_T(STRUCT_TYPE, UNWRAP_FN, TEMPLATE, NAME, FIELD, GET) \
    TEMPLATE->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, NAME).ToLocalChecked(), \
        GETTER_T(STRUCT_TYPE, UNWRAP_FN, FIELD, GET))

// Legacy macros for entity.cpp compatibility (these will be replaced)
#define GETTER(FIELD, TYPE) GETTER_T(edict_t, structures::unwrapEntity, FIELD, TYPE)
#define SETTER(FIELD, TYPE) SETTER_T(edict_t, structures::unwrapEntity, FIELD, TYPE)
#define SETTERL(FIELD, TYPE) SETTERL_T(edict_t, structures::unwrapEntity, FIELD, TYPE)
#define ACCESSOR(TEMPLATE, NAME, FIELD, GET, SET) ACCESSOR_T(edict_t, structures::unwrapEntity, TEMPLATE, NAME, FIELD, GET, SET)
#define ACCESSORL(TEMPLATE, NAME, FIELD, GET, SET) ACCESSORL_T(edict_t, structures::unwrapEntity, TEMPLATE, NAME, FIELD, GET, SET)