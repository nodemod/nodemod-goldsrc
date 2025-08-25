import customs from './customs.js';

// Генератор транслейтов
const generator = {
  js2cpp(type, value) {
    const extractorGenerator = {
      'const char *': `utils::js2string(isolate, ${value})`,
      'const char*': `utils::js2string(isolate, ${value})`,
      'char *': `utils::js2string(isolate, ${value})`,
      'char*': `utils::js2string(isolate, ${value})`,
      char: `${value}->Int32Value(context).ToChecked()`,
      int: `${value}->Int32Value(context).ToChecked()`,
      'unsigned int': `${value}->Uint32Value(context).ToChecked()`,
      float: `${value}->NumberValue(context).ToChecked()`,
      double: `${value}->NumberValue(context).ToChecked()`,
      'unsigned char': `${value}->Int32Value(context).ToChecked()`,
      'unsigned short': `${value}->Int32Value(context).ToChecked()`,
      byte: `${value}->Int32Value(context).ToChecked()`,
      qboolean: `${value}->BooleanValue(isolate)`,
      ALERT_TYPE: `(ALERT_TYPE)${value}->Int32Value(context).ToChecked()`,
      FORCE_TYPE: `*(FORCE_TYPE*)utils::jsToBytes(isolate, ${value})`,
      PRINT_TYPE: `*(PRINT_TYPE*)utils::jsToBytes(isolate, ${value})`,
      CRC32_t: `${value}->Uint32Value(context).ToChecked()`,
      vec3_t: `utils::js2vect(isolate, ${value})`,
      'void*': `nullptr /* void* not supported */`,
      'FILE *': `nullptr /* FILE* not supported */`,
      '$rest': `/* variadic args */`,
    };

    if (extractorGenerator[type]) {
      return extractorGenerator[type];
    }

    const [_, name] = type.match(/(?:const )?(?:struct )?(.+)\*/) || [];
    if (name) {
      const structs = {
        edict_t: `structures::unwrapEntity(isolate, ${value})`,
        'edict_s ': `structures::unwrapEntity(isolate, ${value})`,
        'const edict_s ': `structures::unwrapEntity(isolate, ${value})`,
        'struct edict_s ': `structures::unwrapEntity(isolate, ${value})`,
        'const struct edict_s ': `structures::unwrapEntity(isolate, ${value})`,
        'entvars_s': `structures::unwrapEntvars(isolate, ${value})`,
        'entvars_s ': `structures::unwrapEntvars(isolate, ${value})`,
        'struct entvars_s': `structures::unwrapEntvars(isolate, ${value})`,
        'struct entvars_s ': `structures::unwrapEntvars(isolate, ${value})`,
        'clientdata_s ': `structures::unwrapClientData(isolate, ${value})`,
        'struct clientdata_s ': `structures::unwrapClientData(isolate, ${value})`,
        'entity_state_s ': `structures::unwrapEntityState(isolate, ${value})`,
        'struct entity_state_s ': `structures::unwrapEntityState(isolate, ${value})`,
        'usercmd_s ': `structures::unwrapUserCmd(isolate, ${value})`,
        'const usercmd_s ': `structures::unwrapUserCmd(isolate, ${value})`,
        'const struct usercmd_s ': `structures::unwrapUserCmd(isolate, ${value})`,
        'netadr_s ': `structures::unwrapNetAdr(isolate, ${value})`,
        'const netadr_s ': `structures::unwrapNetAdr(isolate, ${value})`,
        'const struct netadr_s ': `structures::unwrapNetAdr(isolate, ${value})`,
        'weapon_data_s ': `structures::unwrapWeaponData(isolate, ${value})`,
        'struct weapon_data_s ': `structures::unwrapWeaponData(isolate, ${value})`,
        'playermove_s ': `structures::unwrapPlayerMove(isolate, ${value})`,
        'struct playermove_s ': `structures::unwrapPlayerMove(isolate, ${value})`,
        'customization_t ': `structures::unwrapCustomization(isolate, ${value})`,
        'KeyValueData ': `structures::unwrapKeyValueData(isolate, ${value})`,
        'SAVERESTOREDATA': `structures::unwrapSaveRestoreData(isolate, ${value})`,
        'SAVERESTOREDATA ': `structures::unwrapSaveRestoreData(isolate, ${value})`,
        'TYPEDESCRIPTION': `structures::unwrapTypeDescription(isolate, ${value})`,
        'TYPEDESCRIPTION ': `structures::unwrapTypeDescription(isolate, ${value})`,
        'delta_s ': `structures::unwrapDelta(isolate, ${value})`,
        'struct delta_s ': `structures::unwrapDelta(isolate, ${value})`,
        'cvar_s ': `structures::unwrapCvar(isolate, ${value})`,
        'struct cvar_s ': `structures::unwrapCvar(isolate, ${value})`,
        'float ': `(float*)utils::jsToPointer(isolate, ${value})`,
        'int ': `(int*)utils::jsToPointer(isolate, ${value})`,
        'char *': `(char**)utils::jsToPointer(isolate, ${value})`,
        'void ': `utils::jsToPointer(isolate, ${value})`,
        'CRC32_t ': `(CRC32_t*)utils::jsToPointer(isolate, ${value})`,
        'unsigned char ': `(unsigned char*)utils::jsToPointer(isolate, ${value})`,
        'unsigned char *': `(unsigned char**)utils::jsToPointer(isolate, ${value})`,
        'byte': `(byte*)utils::jsToPointer(isolate, ${value})`,
        'TraceResult': `structures::unwrapTraceResult(isolate, ${value})`,
        'cvar_t': `structures::unwrapCvar(isolate, ${value})`
      };

      // Add exact string matches for problematic types
      const exactMatches = {
        'KeyValueData *': `structures::unwrapKeyValueData(isolate, ${value})`,
        'customization_t *': `structures::unwrapCustomization(isolate, ${value})`,
        'struct playermove_s *': `structures::unwrapPlayerMove(isolate, ${value})`,
        'struct edict_s *': `structures::unwrapEntity(isolate, ${value})`,
        'const struct edict_s *': `structures::unwrapEntity(isolate, ${value})`,
        'struct clientdata_s *': `structures::unwrapClientData(isolate, ${value})`,
        'struct entity_state_s *': `structures::unwrapEntityState(isolate, ${value})`,
        'struct weapon_data_s *': `structures::unwrapWeaponData(isolate, ${value})`,
        'const struct usercmd_s *': `structures::unwrapUserCmd(isolate, ${value})`,
        'const struct netadr_s *': `structures::unwrapNetAdr(isolate, ${value})`,
        'int *': `(int*)utils::jsToPointer(isolate, ${value})`,
        'float *': `(float*)utils::jsToPointer(isolate, ${value})`,
        'const float *': `(const float*)utils::jsToPointer(isolate, ${value})`,
        'void *': `utils::jsToPointer(isolate, ${value})`,
        'struct delta_s *': `structures::unwrapDelta(isolate, ${value})`,
        'struct cvar_s *': `structures::unwrapCvar(isolate, ${value})`,
        'cvar_t *': `structures::unwrapCvar(isolate, ${value})`,
        'TraceResult *': `structures::unwrapTraceResult(isolate, ${value})`,
        'FILE *': `nullptr /* FILE* not supported */`,
        'CRC32_t *': `(CRC32_t*)utils::jsToPointer(isolate, ${value})`,
        'char **': `(char**)utils::jsToPointer(isolate, ${value})`,
        'unsigned char *': `(unsigned char*)utils::jsToPointer(isolate, ${value})`
      };

      if (exactMatches[type]) {
        return exactMatches[type];
      }

      if (structs[name.trim()]) {
        return structs[name.trim()];
      }
    }

    console.warn(`No '${type}' js2cpp`);
    return `nullptr /* ${type} */`;
  },

  cpp2js(type, value) {
    const types = {
      char: `v8::Number::New(isolate, ${value})`,
      'unsigned char': `v8::Number::New(isolate, ${value})`,
      'unsigned short': `v8::Number::New(isolate, ${value})`,
      byte: `v8::Number::New(isolate, ${value})`,
      'unsigned int': `v8::Number::New(isolate, ${value})`,
      int: `v8::Number::New(isolate, ${value})`,
      float: `v8::Number::New(isolate, ${value})`,
      double: `v8::Number::New(isolate, ${value})`,
      qboolean: `v8::Boolean::New(isolate, ${value})`,
      ALERT_TYPE: `v8::Number::New(isolate, ${value})`,
      FORCE_TYPE: `v8::Number::New(isolate, ${value})`,
      PRINT_TYPE: `v8::Number::New(isolate, ${value})`,
      CRC32_t: `v8::Number::New(isolate, ${value})`,
      vec3_t: `utils::vect2js(isolate, ${value})`,
      TraceResult: `structures::wrapTraceResult(isolate, &${value})`
    };

    if (types[type]) {
      return types[type];
    }

    const [_, name] = type.match(/(?:const )?(?:struct )?(.+)\*/) || [];
    if (name) {
      const structs = {
        edict_t: `structures::wrapEntity(isolate, ${value})`,
        'edict_s ': `structures::wrapEntity(isolate, ${value})`,
        'const edict_s ': `structures::wrapEntity(isolate, ${value})`,
        'struct edict_s ': `structures::wrapEntity(isolate, ${value})`,
        'const struct edict_s ': `structures::wrapEntity(isolate, ${value})`,
        'entvars_s': `structures::wrapEntvars(isolate, ${value})`,
        'entvars_s ': `structures::wrapEntvars(isolate, ${value})`,
        'struct entvars_s': `structures::wrapEntvars(isolate, ${value})`,
        'struct entvars_s ': `structures::wrapEntvars(isolate, ${value})`,
        'clientdata_s ': `structures::wrapClientData(isolate, ${value})`,
        'struct clientdata_s ': `structures::wrapClientData(isolate, ${value})`,
        'entity_state_s ': `structures::wrapEntityState(isolate, ${value})`,
        'struct entity_state_s ': `structures::wrapEntityState(isolate, ${value})`,
        'usercmd_s ': `structures::wrapUserCmd(isolate, ${value})`,
        'const usercmd_s ': `structures::wrapUserCmd(isolate, ${value})`,
        'const struct usercmd_s ': `structures::wrapUserCmd(isolate, ${value})`,
        'netadr_s ': `structures::wrapNetAdr(isolate, ${value})`,
        'const netadr_s ': `structures::wrapNetAdr(isolate, ${value})`,
        'const struct netadr_s ': `structures::wrapNetAdr(isolate, ${value})`,
        'weapon_data_s ': `structures::wrapWeaponData(isolate, ${value})`,
        'struct weapon_data_s ': `structures::wrapWeaponData(isolate, ${value})`,
        'playermove_s ': `structures::wrapPlayerMove(isolate, ${value})`,
        'struct playermove_s ': `structures::wrapPlayerMove(isolate, ${value})`,
        'customization_t ': `structures::wrapCustomization(isolate, ${value})`,
        'KeyValueData ': `structures::wrapKeyValueData(isolate, ${value})`,
        'SAVERESTOREDATA': `structures::wrapSaveRestoreData(isolate, ${value})`,
        'SAVERESTOREDATA ': `structures::wrapSaveRestoreData(isolate, ${value})`,
        'TYPEDESCRIPTION': `structures::wrapTypeDescription(isolate, ${value})`,
        'TYPEDESCRIPTION ': `structures::wrapTypeDescription(isolate, ${value})`,
        'delta_s ': `structures::wrapDelta(isolate, ${value})`,
        'struct delta_s ': `structures::wrapDelta(isolate, ${value})`,
        'cvar_s ': `structures::wrapCvar(isolate, ${value})`,
        'struct cvar_s ': `structures::wrapCvar(isolate, ${value})`,
        char: `v8::String::NewFromUtf8(isolate, ${value}).ToLocalChecked()`,
        'const float ': `utils::floatArrayToJS(isolate, ${value})`,
        'float ': `utils::floatArrayToJS(isolate, ${value})`,
        'const int ': `utils::intArrayToJS(isolate, ${value})`,
        'int ': `utils::intArrayToJS(isolate, ${value})`,
        'char *': `utils::stringArrayToJS(isolate, ${value})`,
        'void ': `v8::External::New(isolate, ${value})`,
        'CRC32_t ': `v8::External::New(isolate, ${value})`,
        'unsigned char ': `utils::byteArrayToJS(isolate, ${value})`,
        'unsigned char *': `utils::byteArrayToJS(isolate, ${value})`,
        'byte': `utils::byteArrayToJS(isolate, ${value})`,
        'TraceResult ': `structures::wrapTraceResult(isolate, ${value})`,
        'cvar_t ': `structures::wrapCvar(isolate, ${value})`,
        'FILE ': `v8::External::New(isolate, ${value})`
      };

      // Add exact string matches for problematic types
      const exactMatches2 = {
        'KeyValueData *': `structures::wrapKeyValueData(isolate, ${value})`,
        'customization_t *': `structures::wrapCustomization(isolate, ${value})`,
        'struct playermove_s *': `structures::wrapPlayerMove(isolate, ${value})`,
        'struct edict_s *': `structures::wrapEntity(isolate, ${value})`,
        'const struct edict_s *': `structures::wrapEntity(isolate, ${value})`,
        'struct clientdata_s *': `structures::wrapClientData(isolate, ${value})`,
        'struct entity_state_s *': `structures::wrapEntityState(isolate, ${value})`,
        'struct weapon_data_s *': `structures::wrapWeaponData(isolate, ${value})`,
        'const struct usercmd_s *': `structures::wrapUserCmd(isolate, ${value})`,
        'const struct netadr_s *': `structures::wrapNetAdr(isolate, ${value})`,
        'int *': `utils::intArrayToJS(isolate, ${value})`,
        'float *': `utils::floatArrayToJS(isolate, ${value})`,
        'const float *': `utils::floatArrayToJS(isolate, ${value})`,
        'void *': `v8::External::New(isolate, ${value})`,
        'void*': `v8::External::New(isolate, ${value})`,
        'struct delta_s *': `structures::wrapDelta(isolate, ${value})`,
        'struct cvar_s *': `structures::wrapCvar(isolate, ${value})`,
        'cvar_t *': `structures::wrapCvar(isolate, ${value})`,
        'TraceResult *': `structures::wrapTraceResult(isolate, ${value})`,
        'FILE *': `v8::External::New(isolate, ${value})`,
        'CRC32_t *': `v8::External::New(isolate, ${value})`,
        'char **': `utils::stringArrayToJS(isolate, ${value})`,
        'unsigned char *': `utils::byteArrayToJS(isolate, ${value})`
      };

      if (exactMatches2[type]) {
        return exactMatches2[type];
      }

      if (structs[name.trim()]) {
        return structs[name.trim()];
      }

      console.warn(`Struct ${type} not found: cpp2js`);
      return `v8::External::New(isolate, ${value} /* ${name} */)`;
    }

    console.warn(`Type ${type} not found: cpp2js`);
    return null;
  },

  packReturn(func, value) {
    if (func.type === 'void') {
      return value;
    }

    const generated = this.cpp2js(func.type, value)
    if (!generated) {
      console.log(`No ${func.type} type packReturn`);
      if (func.type.includes('*')) {
        return `info.GetReturnValue().Set(v8::External::New(isolate, ${value}));`;
      }

      return `${value} /* TODO: type ${func.type} */`;
    }


    return `info.GetReturnValue().Set(${generated})`;
  },

  generateCppFunction(func, source, prefix) {
    const customBody = customs[prefix.split('_')[1]]?.[func.name]?.api?.body;
    return `void ${prefix}_${func.name}(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  ${customBody || this.packReturn(func, `(*${source}.${func.name})(${(func.args || []).map((v, i) => this.js2cpp(v.type, `info[${i}]`)).join(',\n')})`)};
}`;
  }
}

export default generator;