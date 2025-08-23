#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapClientData(v8::Isolate* isolate, void* clientdata) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!clientdata) {
        return v8::Null(isolate);
    }
    
    client_data_t* data = static_cast<client_data_t*>(clientdata);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked(), 
        utils::vect2js(isolate, data->origin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "viewangles").ToLocalChecked(), 
        utils::vect2js(isolate, data->viewangles)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iWeaponBits").ToLocalChecked(), 
        v8::Number::New(isolate, data->iWeaponBits)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fov").ToLocalChecked(), 
        v8::Number::New(isolate, data->fov)).Check();
    
    return obj;
}

void* unwrapClientData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    client_data_t* data = new client_data_t();
    
    // Extract origin
    v8::Local<v8::Value> originVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked()).ToLocalChecked();
    if (originVal->IsArray()) {
        utils::js2vect(isolate, originVal, data->origin);
    }
    
    // Extract viewangles
    v8::Local<v8::Value> viewanglesVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "viewangles").ToLocalChecked()).ToLocalChecked();
    if (viewanglesVal->IsArray()) {
        utils::js2vect(isolate, viewanglesVal, data->viewangles);
    }
    
    // Extract iWeaponBits
    v8::Local<v8::Value> weaponBitsVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "iWeaponBits").ToLocalChecked()).ToLocalChecked();
    if (weaponBitsVal->IsNumber()) {
        data->iWeaponBits = weaponBitsVal->Int32Value(context).FromJust();
    }
    
    // Extract fov
    v8::Local<v8::Value> fovVal = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "fov").ToLocalChecked()).ToLocalChecked();
    if (fovVal->IsNumber()) {
        data->fov = fovVal->NumberValue(context).FromJust();
    }
    
    return data;
}

}