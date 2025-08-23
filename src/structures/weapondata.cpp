#include "structures.hpp"
#include "common_macros.hpp"
#include <weaponinfo.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> weaponDataTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedWeaponData;

weapon_data_t* unwrapWeaponData_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<weapon_data_t*>(field);
}

void createWeaponDataTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Integer fields
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_iId", m_iId, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_iClip", m_iClip, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fInReload", m_fInReload, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fInSpecialReload", m_fInSpecialReload, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fInZoom", m_fInZoom, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_iWeaponState", m_iWeaponState, GETN, SETINT);
    
    // Float fields
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_flNextPrimaryAttack", m_flNextPrimaryAttack, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_flNextSecondaryAttack", m_flNextSecondaryAttack, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_flTimeWeaponIdle", m_flTimeWeaponIdle, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_flNextReload", m_flNextReload, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_flPumpTime", m_flPumpTime, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fReloadTime", m_fReloadTime, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fAimedDamage", m_fAimedDamage, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "m_fNextAimBonus", m_fNextAimBonus, GETN, SETFLOAT);
    
    // User fields
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "iuser1", iuser1, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "iuser2", iuser2, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "iuser3", iuser3, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "iuser4", iuser4, GETN, SETINT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "fuser1", fuser1, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "fuser2", fuser2, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "fuser3", fuser3, GETN, SETFLOAT);
    ACCESSOR_T(weapon_data_t, unwrapWeaponData_internal, templ, "fuser4", fuser4, GETN, SETFLOAT);
    
    weaponDataTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapWeaponData(v8::Isolate* isolate, void* weapondata) {
    v8::Locker locker(isolate);
    if (!weapondata) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedWeaponData.find(weapondata) != wrappedWeaponData.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedWeaponData[weapondata]);
    }
    
    // Create template if not initialized
    if (weaponDataTemplate.IsEmpty()) {
        createWeaponDataTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = weaponDataTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, weapondata);
    
    wrappedWeaponData[weapondata].Reset(isolate, obj);
    return obj;
}

void* unwrapWeaponData(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapWeaponData_internal(isolate, obj);
}

}