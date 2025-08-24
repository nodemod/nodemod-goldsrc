export default {
  eng: {
    pfnClientCommand: {
      event: {
        argsString: 'edict_t* ed, const char *szFmt, ...',
        body: `unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);
  v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGS()).ToLocalChecked();`
      },
      typescript: {
        parameters: [
          { name: 'entity', type: 'Entity', originalType: 'edict_t *' },
          { name: 'commandArgs', type: 'string', originalType: 'string' }
        ]
      },
      api: {
        body: `(*g_engfuncs.pfnClientCommand)(structures::unwrapEntity(isolate, info[0]), utils::js2string(isolate, info[1]));`
      }
    },
    pfnAlertMessage: {
      api: {
        body: `(*g_engfuncs.pfnAlertMessage)((ALERT_TYPE)info[0]->Int32Value(context).ToChecked(), "%s", utils::js2string(isolate, info[1]));`
      }
    },
    pfnCVarRegister: {
      api: {
        body: `(*g_engfuncs.pfnCVarRegister)((cvar_t*)structures::unwrapCvar(isolate, info[0]));`
      }
    },
    pfnCvar_RegisterVariable: {
      api: {
        body: `(*g_engfuncs.pfnCvar_RegisterVariable)((cvar_t*)structures::unwrapCvar(isolate, info[0]));`
      }
    },
    pfnCvar_DirectSet: {
      api: {
        body: `(*g_engfuncs.pfnCvar_DirectSet)((cvar_t*)structures::unwrapCvar(isolate, info[0]), utils::js2string(isolate, info[1]));`
      }
    },
    pfnEngineFprintf: {
      api: {
        body: `fprintf((FILE*)utils::jsToPointer(isolate, info[0]), "%s", utils::js2string(isolate, info[1]));`
      }
    },
    pfnMessageBegin: {
      api: true,
      typescript: {
        parameters: [
          { name: 'msg_dest', type: 'number', originalType: 'int' },
          { name: 'msg_type', type: 'number', originalType: 'int' },
          { name: 'pOrigin', type: 'number[]', originalType: 'const float *' },
          { name: 'ed', type: 'Entity | undefined', originalType: 'edict_t *' }
        ]
      }
    }
  },
  dll: {
    pfnClientCommand: {
      event: {
        argsString: 'edict_t* ed',
        body: `unsigned int v8_argCount = 2;
  v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[2];
  v8_args[0] = structures::wrapEntity(isolate, ed);

  // refactor it str hell
  if (CMD_ARGC() > 1) {
  char buf[100];
  sprintf(buf, "%s %s", CMD_ARGV(0), CMD_ARGS());
  v8_args[1] = v8::String::NewFromUtf8(isolate, buf).ToLocalChecked();
  } else {
    v8_args[1] = v8::String::NewFromUtf8(isolate, CMD_ARGV(0)).ToLocalChecked();
  }`
      },
      typescript: {
        parameters: [
          { name: 'client', type: 'Entity', originalType: 'edict_t *' },
          { name: 'commandText', type: 'string', originalType: 'string' }
        ]
      }
    }
  }
};