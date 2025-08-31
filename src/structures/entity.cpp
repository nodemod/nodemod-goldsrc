#include "extdll.h"
#include "structures.hpp"
#include "common_macros.hpp"
#include "util/convert.hpp"
#include "node/utils.hpp"
#include <node_buffer.h>
#include <unordered_map>

namespace structures
{
  v8::Eternal<v8::ObjectTemplate> entity;
  std::unordered_map<int, v8::Persistent<v8::Object>> wrappedEntities;

  void cacheEntity(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    // wrappedEntities[convert::js2int(isolate, info[0])].Reset(isolate, info[1]);
  }

  edict_t *unwrapEntity(v8::Isolate *isolate, const v8::Local<v8::Value> &obj)
  {
    v8::Locker locker(isolate);
    if (obj.IsEmpty() || !obj->IsObject())
    {
      return NULL;
    }

    auto object = obj->ToObject(isolate->GetCurrentContext());
    if (object.IsEmpty())
    {
      return NULL;
    }

    auto field = object.ToLocalChecked()->GetAlignedPointerFromInternalField(0);
    return static_cast<edict_t *>(field);
  }

  v8::Local<v8::Value> wrapEntity(v8::Isolate *isolate, const edict_t *entity)
  {
    v8::Locker locker(isolate);
    if (entity == NULL)
    {
      return v8::Null(isolate);
    }

    int entityId = (*g_engfuncs.pfnIndexOfEdict)(entity);
    if (wrappedEntities.find(entityId) != wrappedEntities.end())
    {
      v8::Local<v8::Object> existingObject = v8::Local<v8::Object>::New(isolate, wrappedEntities[entityId]);
      return existingObject;
    }

    // Create a new instance if an object doesn't exist for this ID
    v8::Local<v8::Object> obj = structures::entity.Get(isolate)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetAlignedPointerInInternalField(0, const_cast<edict_t*>(entity));

    wrappedEntities[entityId].Reset(isolate, obj);
    return obj;
  }

  void createEntityTemplate(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global)
  {
    v8::Locker locker(isolate);
    v8::EscapableHandleScope scope(isolate);
    auto context = isolate->GetCurrentContext();

    v8::Local<v8::FunctionTemplate> entityConstructor = v8::FunctionTemplate::New(isolate, [](const v8::FunctionCallbackInfo<v8::Value>& args) {
      edict_t* entity = (*g_engfuncs.pfnCreateNamedEntity)((*g_engfuncs.pfnAllocString)(utils::js2string(args.GetIsolate(), args[0])));

      args.This()->SetAlignedPointerInInternalField(0, entity);

      int entityId = (*g_engfuncs.pfnIndexOfEdict)(entity);
      wrappedEntities[entityId].Reset(args.GetIsolate(), args.This());
      args.GetReturnValue().Set(args.This());
    });

    v8::Local<v8::ObjectTemplate> _entity = entityConstructor->InstanceTemplate();
    _entity->SetInternalFieldCount(1);

    // Entity ID (read-only computed property)
    _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "id").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            edict_t *edict = (edict_t *)structures::unwrapEntity(info.GetIsolate(), info.Holder());
            if (edict == nullptr) {
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), 0));
            } else {
                int entityId = (*g_engfuncs.pfnIndexOfEdict)(edict);
                info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), entityId));
            }
        });

    // getPrivateDataBuffer method using SetNativeDataProperty for proper context
    _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "getPrivateDataBuffer").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            // Return a function that will handle the actual call
            v8::Local<v8::Function> func = v8::Function::New(
                info.GetIsolate()->GetCurrentContext(),
                [](const v8::FunctionCallbackInfo<v8::Value>& args) {
                    if (args.Length() < 2) {
                        args.GetIsolate()->ThrowException(v8::Exception::Error(
                            v8::String::NewFromUtf8(args.GetIsolate(), "getPrivateDataBuffer requires offset and size parameters").ToLocalChecked()));
                        return;
                    }
                    
                    edict_t *edict = (edict_t *)structures::unwrapEntity(args.GetIsolate(), args.This());
                    if (edict == nullptr || edict->pvPrivateData == nullptr) {
                        args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
                        return;
                    }
                    
                    // Get offset and size from arguments
                    uint32_t offset = args[0]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
                    uint32_t size = args[1]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
                    
                    if (size == 0) {
                        args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
                        return;
                    }
                    
                    // Calculate pointer with offset
                    void* dataPtr = static_cast<char*>(edict->pvPrivateData) + offset;
                    
                    // Create backing store for the specific memory range
                    std::unique_ptr<v8::BackingStore> backingStore = 
                        v8::ArrayBuffer::NewBackingStore(dataPtr, size, 
                        [](void* data, size_t length, void* deleter_data) {
                            // Do nothing - we don't own this memory
                        }, nullptr);
                    
                    v8::Local<v8::ArrayBuffer> arrayBuffer = 
                        v8::ArrayBuffer::New(args.GetIsolate(), std::move(backingStore));
                    
                    // Convert to Buffer using Node.js context
                    v8::Local<v8::Value> buffer;
                    if (node::Buffer::New(args.GetIsolate(), arrayBuffer, 0, size).ToLocal(&buffer)) {
                        args.GetReturnValue().Set(buffer);
                    } else {
                        args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
                    }
                }).ToLocalChecked();
                
            info.GetReturnValue().Set(func);
        });

    // writePrivateDataBuffer method for writing to entity private data
    _entity->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "writePrivateDataBuffer").ToLocalChecked(),
        [](v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
            // Return a function that will handle the actual call
            v8::Local<v8::Function> func = v8::Function::New(
                info.GetIsolate()->GetCurrentContext(),
                [](const v8::FunctionCallbackInfo<v8::Value>& args) {
                    if (args.Length() < 2) {
                        args.GetIsolate()->ThrowException(v8::Exception::Error(
                            v8::String::NewFromUtf8(args.GetIsolate(), "writePrivateDataBuffer requires offset and buffer parameters").ToLocalChecked()));
                        return;
                    }
                    
                    edict_t *edict = (edict_t *)structures::unwrapEntity(args.GetIsolate(), args.This());
                    if (edict == nullptr || edict->pvPrivateData == nullptr) {
                        args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));
                        return;
                    }
                    
                    // Get offset from first argument
                    uint32_t offset = args[0]->Uint32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
                    
                    // Check if second argument is a Buffer or ArrayBuffer
                    v8::Local<v8::Value> bufferArg = args[1];
                    const char* sourceData = nullptr;
                    size_t sourceSize = 0;
                    
                    if (node::Buffer::HasInstance(bufferArg)) {
                        sourceData = node::Buffer::Data(bufferArg);
                        sourceSize = node::Buffer::Length(bufferArg);
                    } else if (bufferArg->IsArrayBuffer()) {
                        v8::Local<v8::ArrayBuffer> arrayBuffer = bufferArg.As<v8::ArrayBuffer>();
                        sourceData = static_cast<const char*>(arrayBuffer->Data());
                        sourceSize = arrayBuffer->ByteLength();
                    } else if (bufferArg->IsTypedArray()) {
                        v8::Local<v8::TypedArray> typedArray = bufferArg.As<v8::TypedArray>();
                        v8::Local<v8::ArrayBuffer> arrayBuffer = typedArray->Buffer();
                        sourceData = static_cast<const char*>(arrayBuffer->Data()) + typedArray->ByteOffset();
                        sourceSize = typedArray->ByteLength();
                    } else {
                        args.GetIsolate()->ThrowException(v8::Exception::TypeError(
                            v8::String::NewFromUtf8(args.GetIsolate(), "Second argument must be a Buffer, ArrayBuffer, or TypedArray").ToLocalChecked()));
                        return;
                    }
                    
                    if (sourceData == nullptr || sourceSize == 0) {
                        args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));
                        return;
                    }
                    
                    // Calculate destination pointer with offset
                    void* destPtr = static_cast<char*>(edict->pvPrivateData) + offset;
                    
                    // Copy data from source buffer to private data
                    memcpy(destPtr, sourceData, sourceSize);
                    
                    // Return success
                    args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), true));
                }).ToLocalChecked();
                
            info.GetReturnValue().Set(func);
        });

    // Common entvars field accessors that delegate to the entvars object

    DELEGATE_ACCESSOR("classname");
	  DELEGATE_ACCESSOR("globalname");
	
	  DELEGATE_ACCESSOR("origin");
	  DELEGATE_ACCESSOR("oldorigin");
	  DELEGATE_ACCESSOR("velocity");
	  DELEGATE_ACCESSOR("basevelocity");
	  DELEGATE_ACCESSOR("clbasevelocity");
	  DELEGATE_ACCESSOR("movedir");

	  DELEGATE_ACCESSOR("angles");
	  DELEGATE_ACCESSOR("avelocity");
	  DELEGATE_ACCESSOR("punchangle");
	  DELEGATE_ACCESSOR("v_angle");

	  DELEGATE_ACCESSOR("endpos");
	  DELEGATE_ACCESSOR("startpos");
	  DELEGATE_ACCESSOR("impacttime");
	  DELEGATE_ACCESSOR("starttime");

	  DELEGATE_ACCESSOR("fixangle");
	  DELEGATE_ACCESSOR("idealpitch");
	  DELEGATE_ACCESSOR("pitch_speed");
	  DELEGATE_ACCESSOR("ideal_yaw");
	  DELEGATE_ACCESSOR("yaw_speed");

	  DELEGATE_ACCESSOR("modelindex");

	  DELEGATE_ACCESSOR("model");
	  DELEGATE_ACCESSOR("viewmodel");	
	  DELEGATE_ACCESSOR("weaponmodel");

	  DELEGATE_ACCESSOR("absmin");
	  DELEGATE_ACCESSOR("absmax");
	  DELEGATE_ACCESSOR("mins");
	  DELEGATE_ACCESSOR("maxs");
	  DELEGATE_ACCESSOR("size");

	  DELEGATE_ACCESSOR("ltime");
	  DELEGATE_ACCESSOR("nextthink");

	  DELEGATE_ACCESSOR("movetype");
	  DELEGATE_ACCESSOR("solid");

	  DELEGATE_ACCESSOR("skin");
	  DELEGATE_ACCESSOR("body");
	  DELEGATE_ACCESSOR("effects");
	  DELEGATE_ACCESSOR("gravity");
	  DELEGATE_ACCESSOR("friction");

	  DELEGATE_ACCESSOR("light_level");

	  DELEGATE_ACCESSOR("sequence");
	  DELEGATE_ACCESSOR("gaitsequence");
	  DELEGATE_ACCESSOR("frame");
	  DELEGATE_ACCESSOR("animtime");
	  DELEGATE_ACCESSOR("framerate");	
	  DELEGATE_ACCESSOR("controller");
	  DELEGATE_ACCESSOR("blending");

	  DELEGATE_ACCESSOR("scale");
	  DELEGATE_ACCESSOR("rendermode");
	  DELEGATE_ACCESSOR("renderamt");
	  DELEGATE_ACCESSOR("rendercolor");
	  DELEGATE_ACCESSOR("renderfx");

	  DELEGATE_ACCESSOR("health");
	  DELEGATE_ACCESSOR("frags");
	  DELEGATE_ACCESSOR("weapons");
	  DELEGATE_ACCESSOR("takedamage");

	  DELEGATE_ACCESSOR("deadflag");
	  DELEGATE_ACCESSOR("view_ofs");

	  DELEGATE_ACCESSOR("button");
	  DELEGATE_ACCESSOR("impulse");

	  DELEGATE_ACCESSOR("chain");
	  DELEGATE_ACCESSOR("dmg_inflictor");
	  DELEGATE_ACCESSOR("enemy");
	  DELEGATE_ACCESSOR("aiment");
	  DELEGATE_ACCESSOR("owner");
	  DELEGATE_ACCESSOR("groundentity");

	  DELEGATE_ACCESSOR("spawnflags");
	  DELEGATE_ACCESSOR("flags");
	
	  DELEGATE_ACCESSOR("colormap");
	  DELEGATE_ACCESSOR("team");

	  DELEGATE_ACCESSOR("max_health");
	  DELEGATE_ACCESSOR("teleport_time");
	  DELEGATE_ACCESSOR("armortype");
	  DELEGATE_ACCESSOR("armorvalue");
	  DELEGATE_ACCESSOR("waterlevel");
	  DELEGATE_ACCESSOR("watertype");

	  DELEGATE_ACCESSOR("target");
	  DELEGATE_ACCESSOR("targetname");
	  DELEGATE_ACCESSOR("netname");
	  DELEGATE_ACCESSOR("message");

	  DELEGATE_ACCESSOR("dmg_take");
	  DELEGATE_ACCESSOR("dmg_save");
	  DELEGATE_ACCESSOR("dmg");
	  DELEGATE_ACCESSOR("dmgtime");

	  DELEGATE_ACCESSOR("noise");
	  DELEGATE_ACCESSOR("noise1");
	  DELEGATE_ACCESSOR("noise2");
	  DELEGATE_ACCESSOR("noise3");

	  DELEGATE_ACCESSOR("speed");
	  DELEGATE_ACCESSOR("air_finished");
	  DELEGATE_ACCESSOR("pain_finished");
	  DELEGATE_ACCESSOR("radsuit_finished");

	  DELEGATE_ACCESSOR("pContainingEntity");

	  DELEGATE_ACCESSOR("playerclass");
	  DELEGATE_ACCESSOR("maxspeed");

	  DELEGATE_ACCESSOR("fov");
	  DELEGATE_ACCESSOR("weaponanim");

	  DELEGATE_ACCESSOR("pushmsec");

	  DELEGATE_ACCESSOR("bInDuck");
	  DELEGATE_ACCESSOR("flTimeStepSound");
	  DELEGATE_ACCESSOR("flSwimTime");
	  DELEGATE_ACCESSOR("flDuckTime");
	  DELEGATE_ACCESSOR("iStepLeft");
	  DELEGATE_ACCESSOR("flFallVelocity");

	  DELEGATE_ACCESSOR("gamestate");

	  DELEGATE_ACCESSOR("oldbuttons");

	  DELEGATE_ACCESSOR("groupinfo");

	  // For mods
	  DELEGATE_ACCESSOR("iuser1");
	  DELEGATE_ACCESSOR("iuser2");
	  DELEGATE_ACCESSOR("iuser3");
	  DELEGATE_ACCESSOR("iuser4");
	  DELEGATE_ACCESSOR("fuser1");
	  DELEGATE_ACCESSOR("fuser2");
	  DELEGATE_ACCESSOR("fuser3");
	  DELEGATE_ACCESSOR("fuser4");
	  DELEGATE_ACCESSOR("vuser1");
	  DELEGATE_ACCESSOR("vuser2");
	  DELEGATE_ACCESSOR("vuser3");
	  DELEGATE_ACCESSOR("vuser4");
	  DELEGATE_ACCESSOR("euser1");
	  DELEGATE_ACCESSOR("euser2");
	  DELEGATE_ACCESSOR("euser3");
	  DELEGATE_ACCESSOR("euser4");
    
    structures::entity.Set(isolate, _entity);
    global->Set(convert::str2js(isolate, "Entity"), entityConstructor);
  }
}