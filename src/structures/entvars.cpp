#include "structures.hpp"
#include "../util/convert.hpp"
#include "../node/utils.hpp"

extern enginefuncs_t g_engfuncs;

namespace structures {

v8::Local<v8::Value> wrapEntvars(v8::Isolate* isolate, entvars_t* entvars) {
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    
    if (!entvars) {
        return v8::Null(isolate);
    }
    
    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    auto context = isolate->GetCurrentContext();
    
    // String properties (using string_t)
    obj->Set(context, v8::String::NewFromUtf8(isolate, "classname").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->classname)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "globalname").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->globalname)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "model").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->model)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "target").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->target)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "targetname").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->targetname)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "netname").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->netname)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "message").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->message)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "noise").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->noise)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "noise1").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->noise1)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "noise2").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->noise2)).ToLocalChecked()).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "noise3").ToLocalChecked(), 
        v8::String::NewFromUtf8(isolate, g_engfuncs.pfnSzFromIndex(entvars->noise3)).ToLocalChecked()).Check();
    
    // Vector properties
    obj->Set(context, v8::String::NewFromUtf8(isolate, "origin").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->origin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "oldorigin").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->oldorigin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "velocity").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->velocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "basevelocity").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->basevelocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "clbasevelocity").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->clbasevelocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "movedir").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->movedir)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "angles").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->angles)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "avelocity").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->avelocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "punchangle").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->punchangle)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "v_angle").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->v_angle)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "endpos").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->endpos)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "startpos").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->startpos)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "absmin").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->absmin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "absmax").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->absmax)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "mins").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->mins)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "maxs").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->maxs)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "size").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->size)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "rendercolor").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->rendercolor)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "view_ofs").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->view_ofs)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "vuser1").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->vuser1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "vuser2").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->vuser2)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "vuser3").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->vuser3)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "vuser4").ToLocalChecked(), 
        utils::vect2js(isolate, entvars->vuser4)).Check();
    
    // Float properties
    obj->Set(context, v8::String::NewFromUtf8(isolate, "impacttime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->impacttime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "starttime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->starttime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "idealpitch").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->idealpitch)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "pitch_speed").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->pitch_speed)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "ideal_yaw").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->ideal_yaw)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "yaw_speed").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->yaw_speed)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "ltime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->ltime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "nextthink").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->nextthink)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "gravity").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->gravity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "friction").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->friction)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "frame").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->frame)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "animtime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->animtime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "framerate").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->framerate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "scale").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->scale)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "renderamt").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->renderamt)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "health").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->health)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "frags").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->frags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "takedamage").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->takedamage)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "max_health").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->max_health)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "teleport_time").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->teleport_time)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "armortype").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->armortype)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "armorvalue").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->armorvalue)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "dmg_take").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->dmg_take)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "dmg_save").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->dmg_save)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "dmg").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->dmg)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "dmgtime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->dmgtime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "speed").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->speed)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "air_finished").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->air_finished)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "pain_finished").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->pain_finished)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "radsuit_finished").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->radsuit_finished)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "maxspeed").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->maxspeed)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fov").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fov)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flFallVelocity").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->flFallVelocity)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser1").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fuser1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser2").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fuser2)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser3").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fuser3)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fuser4").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fuser4)).Check();
    
    // Integer properties
    obj->Set(context, v8::String::NewFromUtf8(isolate, "fixangle").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->fixangle)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "modelindex").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->modelindex)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "viewmodel").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->viewmodel)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "weaponmodel").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->weaponmodel)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "movetype").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->movetype)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "solid").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->solid)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "skin").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->skin)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "body").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->body)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "effects").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->effects)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "light_level").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->light_level)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "sequence").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->sequence)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "gaitsequence").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->gaitsequence)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "rendermode").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->rendermode)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "renderfx").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->renderfx)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "weapons").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->weapons)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "deadflag").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->deadflag)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "button").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->button)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "impulse").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->impulse)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "spawnflags").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->spawnflags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flags").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->flags)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "colormap").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->colormap)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "team").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->team)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "waterlevel").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->waterlevel)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "watertype").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->watertype)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "playerclass").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->playerclass)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "weaponanim").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->weaponanim)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "pushmsec").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->pushmsec)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "bInDuck").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->bInDuck)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flTimeStepSound").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->flTimeStepSound)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flSwimTime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->flSwimTime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "flDuckTime").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->flDuckTime)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iStepLeft").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->iStepLeft)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "gamestate").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->gamestate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "oldbuttons").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->oldbuttons)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "groupinfo").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->groupinfo)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser1").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->iuser1)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser2").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->iuser2)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser3").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->iuser3)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "iuser4").ToLocalChecked(), 
        v8::Number::New(isolate, entvars->iuser4)).Check();
    
    // Entity references (converted to Entity wrappers)
    obj->Set(context, v8::String::NewFromUtf8(isolate, "chain").ToLocalChecked(), 
        entvars->chain ? wrapEntity(isolate, entvars->chain) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "dmg_inflictor").ToLocalChecked(), 
        entvars->dmg_inflictor ? wrapEntity(isolate, entvars->dmg_inflictor) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "enemy").ToLocalChecked(), 
        entvars->enemy ? wrapEntity(isolate, entvars->enemy) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "aiment").ToLocalChecked(), 
        entvars->aiment ? wrapEntity(isolate, entvars->aiment) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "owner").ToLocalChecked(), 
        entvars->owner ? wrapEntity(isolate, entvars->owner) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "groundentity").ToLocalChecked(), 
        entvars->groundentity ? wrapEntity(isolate, entvars->groundentity) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "pContainingEntity").ToLocalChecked(), 
        entvars->pContainingEntity ? wrapEntity(isolate, entvars->pContainingEntity) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "euser1").ToLocalChecked(), 
        entvars->euser1 ? wrapEntity(isolate, entvars->euser1) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "euser2").ToLocalChecked(), 
        entvars->euser2 ? wrapEntity(isolate, entvars->euser2) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "euser3").ToLocalChecked(), 
        entvars->euser3 ? wrapEntity(isolate, entvars->euser3) : v8::Null(isolate)).Check();
    obj->Set(context, v8::String::NewFromUtf8(isolate, "euser4").ToLocalChecked(), 
        entvars->euser4 ? wrapEntity(isolate, entvars->euser4) : v8::Null(isolate)).Check();
    
    return obj;
}

entvars_t* unwrapEntvars(v8::Isolate* isolate, const v8::Local<v8::Value> &obj) {
    v8::Locker locker(isolate);
    if (obj->IsNull() || obj->IsUndefined()) {
        return nullptr;
    }
    
    if (obj->IsExternal()) {
        v8::Local<v8::External> ext = obj.As<v8::External>();
        return static_cast<entvars_t*>(ext->Value());
    }
    
    return nullptr;
}

}