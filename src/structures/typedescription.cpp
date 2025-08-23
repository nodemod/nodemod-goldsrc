#include "structures.hpp"
#include "common_macros.hpp"
#include <eiface.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> typeDescriptionTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedTypeDescriptions;

TYPEDESCRIPTION* unwrapTypeDescription_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<TYPEDESCRIPTION*>(field);
}

void createTypeDescriptionTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // FIELDTYPE enum field - custom setter to handle enum conversion
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "fieldType").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TYPEDESCRIPTION *td = unwrapTypeDescription_internal(info.GetIsolate(), info.Holder());
            if (td == nullptr) {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), 0));
            } else {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), static_cast<int>(td->fieldType)));
            }
        },
        [](v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info) {
            TYPEDESCRIPTION *td = unwrapTypeDescription_internal(info.GetIsolate(), info.Holder());
            if (td == nullptr) return;
            int intVal = value->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked();
            td->fieldType = static_cast<FIELDTYPE>(intVal);
        });
    
    // String field name - read-only since it's typically a const char*
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "fieldName").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TYPEDESCRIPTION *td = unwrapTypeDescription_internal(info.GetIsolate(), info.Holder());
            if (td == nullptr || td->fieldName == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), td->fieldName).ToLocalChecked());
            }
        });
    
    // Integer fields
    ACCESSOR_T(TYPEDESCRIPTION, unwrapTypeDescription_internal, templ, "fieldOffset", fieldOffset, GETN, SETINT);
    ACCESSOR_T(TYPEDESCRIPTION, unwrapTypeDescription_internal, templ, "fieldSize", fieldSize, GETN, SETINT);
    ACCESSOR_T(TYPEDESCRIPTION, unwrapTypeDescription_internal, templ, "flags", flags, GETN, SETINT);
    
    // Add helper methods to get field type as string
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "fieldTypeName").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TYPEDESCRIPTION *td = unwrapTypeDescription_internal(info.GetIsolate(), info.Holder());
            if (td == nullptr) {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "UNKNOWN").ToLocalChecked());
                return;
            }
            
            const char* typeName;
            switch (td->fieldType) {
                case FIELD_FLOAT: typeName = "FIELD_FLOAT"; break;
                case FIELD_STRING: typeName = "FIELD_STRING"; break;
                case FIELD_ENTITY: typeName = "FIELD_ENTITY"; break;
                case FIELD_CLASSPTR: typeName = "FIELD_CLASSPTR"; break;
                case FIELD_EHANDLE: typeName = "FIELD_EHANDLE"; break;
                case FIELD_EVARS: typeName = "FIELD_EVARS"; break;
                case FIELD_EDICT: typeName = "FIELD_EDICT"; break;
                case FIELD_VECTOR: typeName = "FIELD_VECTOR"; break;
                case FIELD_POSITION_VECTOR: typeName = "FIELD_POSITION_VECTOR"; break;
                case FIELD_POINTER: typeName = "FIELD_POINTER"; break;
                case FIELD_INTEGER: typeName = "FIELD_INTEGER"; break;
                case FIELD_FUNCTION: typeName = "FIELD_FUNCTION"; break;
                case FIELD_BOOLEAN: typeName = "FIELD_BOOLEAN"; break;
                case FIELD_SHORT: typeName = "FIELD_SHORT"; break;
                case FIELD_CHARACTER: typeName = "FIELD_CHARACTER"; break;
                case FIELD_TIME: typeName = "FIELD_TIME"; break;
                case FIELD_MODELNAME: typeName = "FIELD_MODELNAME"; break;
                case FIELD_SOUNDNAME: typeName = "FIELD_SOUNDNAME"; break;
                default: typeName = "UNKNOWN"; break;
            }
            
            info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), typeName).ToLocalChecked());
        });
    
    // Helper to check if field is global
    templ->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "isGlobal").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            TYPEDESCRIPTION *td = unwrapTypeDescription_internal(info.GetIsolate(), info.Holder());
            if (td == nullptr) {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
            } else {
                info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), (td->flags & FTYPEDESC_GLOBAL) != 0));
            }
        });
    
    typeDescriptionTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapTypeDescription(v8::Isolate* isolate, void* typedesc) {
    v8::Locker locker(isolate);
    if (!typedesc) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedTypeDescriptions.find(typedesc) != wrappedTypeDescriptions.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedTypeDescriptions[typedesc]);
    }
    
    // Create template if not initialized
    if (typeDescriptionTemplate.IsEmpty()) {
        createTypeDescriptionTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = typeDescriptionTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, typedesc);
    
    wrappedTypeDescriptions[typedesc].Reset(isolate, obj);
    return obj;
}

void* unwrapTypeDescription(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapTypeDescription_internal(isolate, obj);
}

}