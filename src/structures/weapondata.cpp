#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

namespace structures {

v8::Local<v8::Value> wrapWeaponData(v8::Isolate* isolate, void* weapondata) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!weapondata) {
        return v8::Null(isolate);
    }
    
    weapon_data_t* weapon = static_cast<weapon_data_t*>(weapondata);
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_iId").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_iId)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_iClip").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_iClip)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_flNextPrimaryAttack").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_flNextPrimaryAttack)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_flNextSecondaryAttack").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_flNextSecondaryAttack)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_flTimeWeaponIdle").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_flTimeWeaponIdle)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_fInReload").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_fInReload)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_fInSpecialReload").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_fInSpecialReload)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_flNextReload").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_flNextReload)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_flPumpTime").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_flPumpTime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "m_fReloadTime").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->m_fReloadTime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser1").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->iuser1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser2").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->iuser2)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser1").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->fuser1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser2").ToLocalChecked(), 
        v8::Number::New(isolate, weapon->fuser2)).Check();
    
    return obj;
}

void* unwrapWeaponData(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
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
    weapon_data_t* weapon = new weapon_data_t();
    
    v8::Local<v8::Value> val;
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "m_iId").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) weapon->m_iId = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "m_iClip").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) weapon->m_iClip = val->Int32Value(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "m_flNextPrimaryAttack").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) weapon->m_flNextPrimaryAttack = val->NumberValue(context).FromJust();
    
    val = jsObj->Get(context, v8::String::NewFromUtf8(isolate, "fuser1").ToLocalChecked()).ToLocalChecked();
    if (val->IsNumber()) weapon->fuser1 = val->NumberValue(context).FromJust();
    
    return weapon;
}

}