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
        body: `// Create a new cvar_t structure from JavaScript object
  if (!info[0]->IsObject()) {
    printf("Error: pfnCVarRegister requires an object\\n");
    return;
  }
  
  v8::Local<v8::Object> jsObj = info[0]->ToObject(context).ToLocalChecked();
  
  // Extract required fields
  v8::Local<v8::String> nameKey = v8::String::NewFromUtf8(isolate, "name").ToLocalChecked();
  v8::Local<v8::String> stringKey = v8::String::NewFromUtf8(isolate, "string").ToLocalChecked();
  v8::Local<v8::String> flagsKey = v8::String::NewFromUtf8(isolate, "flags").ToLocalChecked();
  v8::Local<v8::String> valueKey = v8::String::NewFromUtf8(isolate, "value").ToLocalChecked();
  
  if (!jsObj->Has(context, nameKey).ToChecked() || !jsObj->Has(context, stringKey).ToChecked()) {
    printf("Error: pfnCVarRegister requires 'name' and 'string' fields\\n");
    return;
  }
  
  // Allocate and initialize cvar_t structure
  cvar_t* cvar = new cvar_t();
  memset(cvar, 0, sizeof(cvar_t));
  
  // Copy strings (need to keep them alive)
  v8::String::Utf8Value nameStr(isolate, jsObj->Get(context, nameKey).ToLocalChecked());
  v8::String::Utf8Value stringStr(isolate, jsObj->Get(context, stringKey).ToLocalChecked());
  
  cvar->name = strdup(*nameStr);
  cvar->string = strdup(*stringStr);
  cvar->flags = jsObj->Has(context, flagsKey).ToChecked() ? 
    jsObj->Get(context, flagsKey).ToLocalChecked()->Int32Value(context).ToChecked() : 0;
  cvar->value = jsObj->Has(context, valueKey).ToChecked() ? 
    jsObj->Get(context, valueKey).ToLocalChecked()->NumberValue(context).ToChecked() : 0.0f;
  cvar->next = nullptr;
  
  (*g_engfuncs.pfnCVarRegister)(cvar);`
      },
      typescript: {
        parameters: [
          { name: 'cvar', type: 'Cvar', originalType: 'cvar_t *' }
        ]
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
    },
    pfnTraceLine: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  if (!info[0]->IsExternal()) {
    printf("Warning: pfnTraceLine parameter 0 (const float *) is not External, using nullptr\\n");
  }
  if (!info[1]->IsExternal()) {
    printf("Warning: pfnTraceLine parameter 1 (const float *) is not External, using nullptr\\n");
  }

  (*g_engfuncs.pfnTraceLine)((const float*)utils::jsToPointer(isolate, info[0]),
(const float*)utils::jsToPointer(isolate, info[1]),
info[2]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[3]),
&trace);

  // Return the populated TraceResult
  info.GetReturnValue().Set(structures::wrapTraceResult(isolate, &trace));`
      },
      typescript: {
        parameters: [
          { name: 'start', type: 'number[]', originalType: 'const float *' },
          { name: 'end', type: 'number[]', originalType: 'const float *' },
          { name: 'flags', type: 'number', originalType: 'int' },
          { name: 'skipEntity', type: 'Entity | undefined', originalType: 'edict_t *' }
        ],
        returnType: 'TraceResult'
      }
    },
    pfnTraceToss: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  (*g_engfuncs.pfnTraceToss)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]),
&trace);

  // Return the populated TraceResult
  info.GetReturnValue().Set(structures::wrapTraceResult(isolate, &trace));`
      },
      typescript: {
        parameters: [
          { name: 'pent', type: 'Entity', originalType: 'edict_t *' },
          { name: 'pentToIgnore', type: 'Entity', originalType: 'edict_t *' }
        ],
        returnType: 'TraceResult'
      }
    },
    pfnTraceMonsterHull: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  if (!info[1]->IsExternal()) {
    printf("Warning: pfnTraceMonsterHull parameter 1 (const float *) is not External, using nullptr\\n");
  }
  if (!info[2]->IsExternal()) {
    printf("Warning: pfnTraceMonsterHull parameter 2 (const float *) is not External, using nullptr\\n");
  }

  int result = (*g_engfuncs.pfnTraceMonsterHull)(structures::unwrapEntity(isolate, info[0]),
(const float*)utils::jsToPointer(isolate, info[1]),
(const float*)utils::jsToPointer(isolate, info[2]),
info[3]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
&trace);

  // Return object with both result and trace
  v8::Local<v8::Object> resultObj = v8::Object::New(isolate);
  resultObj->Set(context, v8::String::NewFromUtf8(isolate, "result").ToLocalChecked(), v8::Number::New(isolate, result)).Check();
  resultObj->Set(context, v8::String::NewFromUtf8(isolate, "trace").ToLocalChecked(), structures::wrapTraceResult(isolate, &trace)).Check();
  info.GetReturnValue().Set(resultObj);`
      },
      typescript: {
        parameters: [
          { name: 'pEdict', type: 'Entity', originalType: 'edict_t *' },
          { name: 'v1', type: 'number[]', originalType: 'const float *' },
          { name: 'v2', type: 'number[]', originalType: 'const float *' },
          { name: 'fNoMonsters', type: 'number', originalType: 'int' },
          { name: 'pentToSkip', type: 'Entity', originalType: 'edict_t *' }
        ],
        returnType: 'TraceMonsterHullResult'
      }
    },
    pfnTraceHull: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  if (!info[0]->IsExternal()) {
    printf("Warning: pfnTraceHull parameter 0 (const float *) is not External, using nullptr\\n");
  }
  if (!info[1]->IsExternal()) {
    printf("Warning: pfnTraceHull parameter 1 (const float *) is not External, using nullptr\\n");
  }

  (*g_engfuncs.pfnTraceHull)((const float*)utils::jsToPointer(isolate, info[0]),
(const float*)utils::jsToPointer(isolate, info[1]),
info[2]->Int32Value(context).ToChecked(),
info[3]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
&trace);

  // Return the populated TraceResult
  info.GetReturnValue().Set(structures::wrapTraceResult(isolate, &trace));`
      },
      typescript: {
        parameters: [
          { name: 'v1', type: 'number[]', originalType: 'const float *' },
          { name: 'v2', type: 'number[]', originalType: 'const float *' },
          { name: 'fNoMonsters', type: 'number', originalType: 'int' },
          { name: 'hullNumber', type: 'number', originalType: 'int' },
          { name: 'pentToSkip', type: 'Entity', originalType: 'edict_t *' }
        ],
        returnType: 'TraceResult'
      }
    },
    pfnTraceModel: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  if (!info[0]->IsExternal()) {
    printf("Warning: pfnTraceModel parameter 0 (const float *) is not External, using nullptr\\n");
  }
  if (!info[1]->IsExternal()) {
    printf("Warning: pfnTraceModel parameter 1 (const float *) is not External, using nullptr\\n");
  }

  (*g_engfuncs.pfnTraceModel)((const float*)utils::jsToPointer(isolate, info[0]),
(const float*)utils::jsToPointer(isolate, info[1]),
info[2]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[3]),
&trace);

  // Return the populated TraceResult
  info.GetReturnValue().Set(structures::wrapTraceResult(isolate, &trace));`
      },
      typescript: {
        parameters: [
          { name: 'v1', type: 'number[]', originalType: 'const float *' },
          { name: 'v2', type: 'number[]', originalType: 'const float *' },
          { name: 'hullNumber', type: 'number', originalType: 'int' },
          { name: 'pent', type: 'Entity', originalType: 'edict_t *' }
        ],
        returnType: 'TraceResult'
      }
    },
    pfnTraceSphere: {
      api: {
        body: `// Allocate TraceResult on stack
  TraceResult trace;
  
  if (!info[0]->IsExternal()) {
    printf("Warning: pfnTraceSphere parameter 0 (const float *) is not External, using nullptr\\n");
  }
  if (!info[1]->IsExternal()) {
    printf("Warning: pfnTraceSphere parameter 1 (const float *) is not External, using nullptr\\n");
  }

  (*g_engfuncs.pfnTraceSphere)((const float*)utils::jsToPointer(isolate, info[0]),
(const float*)utils::jsToPointer(isolate, info[1]),
info[2]->Int32Value(context).ToChecked(),
info[3]->NumberValue(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
&trace);

  // Return the populated TraceResult
  info.GetReturnValue().Set(structures::wrapTraceResult(isolate, &trace));`
      },
      typescript: {
        parameters: [
          { name: 'v1', type: 'number[]', originalType: 'const float *' },
          { name: 'v2', type: 'number[]', originalType: 'const float *' },
          { name: 'fNoMonsters', type: 'number', originalType: 'int' },
          { name: 'radius', type: 'number', originalType: 'float' },
          { name: 'pentToSkip', type: 'Entity', originalType: 'edict_t *' }
        ],
        returnType: 'TraceResult'
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