#include "structures/structures.hpp"
#include "extdll.h"
#include <v8.h>
#include <vector>

extern globalvars_t   *gpGlobals;
extern enginefuncs_t	 g_engfuncs;

namespace players
{
  void getPlayers(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    auto isolate = info.GetIsolate();
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    std::vector<edict_t*> ents;

    for (int i = 0; i < gpGlobals->maxClients; i++) {
      edict_t* entity = (*g_engfuncs.pfnPEntityOfEntIndex)(i + 1);
      if ((*g_engfuncs.pfnGetPlayerUserId)(entity) <= 0) {
        continue;
      }
      ents.push_back(entity);
    }

    v8::Local<v8::Array> array = v8::Array::New(isolate, ents.size());
    for (int i = 0; i < ents.size(); i++) {
      array->Set(isolate->GetCurrentContext(), i, structures::wrapEntity(isolate, ents[i]));
    }

    info.GetReturnValue().Set(array);
  }
}
