#include "structures.hpp"
#include "common_macros.hpp"
#include <entity_state.h>
#include <unordered_map>

namespace structures {

v8::Eternal<v8::ObjectTemplate> clientDataTemplate;
std::unordered_map<void*, v8::Persistent<v8::Object>> wrappedClientData;

clientdata_s* unwrapClientData_internal(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject()) {
        return nullptr;
    }
    
    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty()) {
        return nullptr;
    }
    
    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<clientdata_s*>(field);
}

void createClientDataTemplate(v8::Isolate* isolate) {
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    
    v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    
    // Vector fields
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "origin", origin, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "velocity", velocity, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "punchangle", punchangle, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "view_ofs", view_ofs, GETVEC3, SETVEC3);
    
    // Integer fields
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "viewmodel", viewmodel, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "flags", flags, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "waterlevel", waterlevel, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "watertype", watertype, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "bInDuck", bInDuck, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "weapons", weapons, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "flTimeStepSound", flTimeStepSound, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "flDuckTime", flDuckTime, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "flSwimTime", flSwimTime, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "waterjumptime", waterjumptime, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "weaponanim", weaponanim, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "m_iId", m_iId, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "ammo_shells", ammo_shells, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "ammo_nails", ammo_nails, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "ammo_cells", ammo_cells, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "ammo_rockets", ammo_rockets, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "tfstate", tfstate, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "pushmsec", pushmsec, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "deadflag", deadflag, GETN, SETINT);
    
    // Float fields
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "health", health, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "maxspeed", maxspeed, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "fov", fov, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "m_flNextAttack", m_flNextAttack, GETN, SETFLOAT);
    
    // User fields
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "iuser1", iuser1, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "iuser2", iuser2, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "iuser3", iuser3, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "iuser4", iuser4, GETN, SETINT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "fuser1", fuser1, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "fuser2", fuser2, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "fuser3", fuser3, GETN, SETFLOAT);
    ACCESSOR_T(clientdata_s, unwrapClientData_internal, templ, "fuser4", fuser4, GETN, SETFLOAT);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "vuser1", vuser1, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "vuser2", vuser2, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "vuser3", vuser3, GETVEC3, SETVEC3);
    ACCESSORL_T(clientdata_s, unwrapClientData_internal, templ, "vuser4", vuser4, GETVEC3, SETVEC3);
    
    // physinfo is a char array - handle as string
    // TODO: Add physinfo accessor if needed
    
    clientDataTemplate.Set(isolate, templ);
}

v8::Local<v8::Value> wrapClientData(v8::Isolate* isolate, void* clientdata) {
    v8::Locker locker(isolate);
    if (!clientdata) {
        return v8::Null(isolate);
    }
    
    // Check if already wrapped
    if (wrappedClientData.find(clientdata) != wrappedClientData.end()) {
        return v8::Local<v8::Object>::New(isolate, wrappedClientData[clientdata]);
    }
    
    // Create template if not initialized
    if (clientDataTemplate.IsEmpty()) {
        createClientDataTemplate(isolate);
    }
    
    // Create new instance
    v8::Local<v8::Object> obj = clientDataTemplate.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, clientdata);
    
    wrappedClientData[clientdata].Reset(isolate, obj);
    return obj;
}

void* unwrapClientData(v8::Isolate* isolate, const v8::Local<v8::Value>& obj) {
    return unwrapClientData_internal(isolate, obj);
}

}