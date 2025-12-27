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
          { name: 'ed', type: 'Entity | null', originalType: 'edict_t *' }
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
          { name: 'skipEntity', type: 'Entity | null', originalType: 'edict_t *' }
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
    },
    pfnLoadFileForMe: {
      api: {
        body: `int fileLength = 0;
  byte* result = (*g_engfuncs.pfnLoadFileForMe)(utils::js2string(isolate, info[0]), &fileLength);
  if (result && fileLength > 0) {
    auto jsArray = utils::byteArrayToJS(isolate, result, fileLength);
    (*g_engfuncs.pfnFreeFile)(result);
    info.GetReturnValue().Set(jsArray);
  } else {
    info.GetReturnValue().Set(v8::Null(isolate));
  }`
      },
      typescript: {
        parameters: [
          { name: 'filename', type: 'string', originalType: 'const char *' }
        ],
        returnType: 'number[] | null'
      }
    },
    pfnSetFatPVS: {
      api: {
        body: `byte* result = (*g_engfuncs.pfnSetFatPVS)((const float*)utils::jsToPointer(isolate, info[0]));
  if (result) {
    // FatPVS size is implementation-dependent, use a reasonable default
    info.GetReturnValue().Set(utils::byteArrayToJS(isolate, result, 256));
  } else {
    info.GetReturnValue().Set(v8::Null(isolate));
  }`
      },
      typescript: {
        parameters: [
          { name: 'org', type: 'number[]', originalType: 'const float *' }
        ],
        returnType: 'number[] | null'
      }
    },
    pfnSetFatPAS: {
      api: {
        body: `byte* result = (*g_engfuncs.pfnSetFatPAS)((const float*)utils::jsToPointer(isolate, info[0]));
  if (result) {
    // FatPAS size is implementation-dependent, use a reasonable default  
    info.GetReturnValue().Set(utils::byteArrayToJS(isolate, result, 256));
  } else {
    info.GetReturnValue().Set(v8::Null(isolate));
  }`
      },
      typescript: {
        parameters: [
          { name: 'org', type: 'number[]', originalType: 'const float *' }
        ],
        returnType: 'number[] | null'
      }
    },
    pfnAddServerCommand: {
      api: {
        body: `// Handle JavaScript function callbacks for server commands
  if (!info[0]->IsString()) {
    printf("Error: pfnAddServerCommand requires a string command name\\n");
    return;
  }
  
  if (!info[1]->IsFunction()) {
    printf("Error: pfnAddServerCommand requires a function callback\\n");
    return;
  }
  
  std::string cmdName = utils::js_to_string(isolate, info[0]);
  v8::Local<v8::Function> jsCallback = info[1].As<v8::Function>();
  
  //printf("[DEBUG] Registering server command: '%s'\\n", cmdName.c_str());
  
  // Store the JavaScript callback globally
  extern std::unordered_map<std::string, v8::Global<v8::Function>> serverCommandCallbacks;
  extern std::unordered_map<std::string, v8::Global<v8::Context>> serverCommandContexts;
  
  serverCommandCallbacks[cmdName].Reset(isolate, jsCallback);
  serverCommandContexts[cmdName].Reset(isolate, context);
  
  //printf("[DEBUG] Stored callback for '%s', total commands: %zu\\n", cmdName.c_str(), serverCommandCallbacks.size());
  
  // Create C++ wrapper function
  static auto wrapperFunction = []() {
    //printf("[DEBUG] Server command wrapper called\\n");
    // Get command name from engine - use CMD_ARGV(0) not CMD_ARGS()
    const char* cmdNamePtr = CMD_ARGV(0);
    std::string cmdName(cmdNamePtr ? cmdNamePtr : "");
    //printf("[DEBUG] CMD_ARGV(0) returned: '%s'\\n", cmdNamePtr ? cmdNamePtr : "(null)");
    //printf("[DEBUG] Extracted command name: '%s'\\n", cmdName.c_str());
    
    auto callbackIter = serverCommandCallbacks.find(cmdName);
    auto contextIter = serverCommandContexts.find(cmdName);
    
    //printf("[DEBUG] Looking for callback for command: '%s'\\n", cmdName.c_str());
    //printf("[DEBUG] Found callback: %s\\n", (callbackIter != serverCommandCallbacks.end()) ? "YES" : "NO");
    
    if (callbackIter != serverCommandCallbacks.end() && contextIter != serverCommandContexts.end()) {
      //printf("[DEBUG] Calling JavaScript callback for '%s'\\n", cmdName.c_str());
      auto isolate = nodeImpl.GetIsolate();
      v8::Locker locker(isolate);
      v8::Isolate::Scope isolateScope(isolate);
      v8::HandleScope handleScope(isolate);
      
      v8::Local<v8::Context> ctx = contextIter->second.Get(isolate);
      v8::Context::Scope contextScope(ctx);
      
      v8::Local<v8::Function> callback = callbackIter->second.Get(isolate);
      
      // Call the JavaScript function with no arguments (server command signature)
      v8::TryCatch tryCatch(isolate);
      callback->Call(ctx, ctx->Global(), 0, nullptr);
      
      if (tryCatch.HasCaught()) {
        v8::String::Utf8Value error(isolate, tryCatch.Exception());
        printf("Error in server command callback: %s\\n", *error);
      }
    } else {
      printf("[DEBUG] No callback found for command: '%s'\\n", cmdName.c_str());
    }
  };
  
  //printf("[DEBUG] Calling pfnAddServerCommand for '%s'\\n", cmdName.c_str());
  (*g_engfuncs.pfnAddServerCommand)(cmdName.c_str(), wrapperFunction);
  //printf("[DEBUG] pfnAddServerCommand completed for '%s'\\n", cmdName.c_str());`
      },
      typescript: {
        parameters: [
          { name: 'commandName', type: 'string', originalType: 'const char *' },
          { name: 'callback', type: '() => void', originalType: 'void (*)(void)' }
        ]
      }
    },
    pfnSetClientKeyValue: {
      api: {
        body: `int clientIndex = info[0]->Int32Value(context).ToChecked();
  edict_t* entity = structures::unwrapEntity(isolate, info[1]);
  char* infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)(entity);
  (*g_engfuncs.pfnSetClientKeyValue)(clientIndex,
    infobuffer,
    utils::js2string(isolate, info[2]),
    utils::js2string(isolate, info[3]));`
      },
      typescript: {
        parameters: [
          { name: 'clientIndex', type: 'number', originalType: 'int' },
          { name: 'entity', type: 'Entity', originalType: 'edict_t *' },
          { name: 'key', type: 'string', originalType: 'const char *' },
          { name: 'value', type: 'string', originalType: 'const char *' }
        ]
      }
    },
    pfnDeltaAddEncoder: {
      api: {
        body: `// Handle JavaScript function callbacks for delta encoders
  if (!info[0]->IsString()) {
    printf("Error: pfnDeltaAddEncoder requires a string name\\n");
    return;
  }
  
  if (!info[1]->IsFunction()) {
    printf("Error: pfnDeltaAddEncoder requires a function callback\\n");
    return;
  }
  
  std::string encoderName = utils::js_to_string(isolate, info[0]);
  v8::Local<v8::Function> jsCallback = info[1].As<v8::Function>();
  
  //printf("[DEBUG] Registering delta encoder: '%s'\\n", encoderName.c_str());
  
  // Store the JavaScript callback globally
  extern std::unordered_map<std::string, v8::Global<v8::Function>> deltaEncoderCallbacks;
  extern std::unordered_map<std::string, v8::Global<v8::Context>> deltaEncoderContexts;
  
  deltaEncoderCallbacks[encoderName].Reset(isolate, jsCallback);
  deltaEncoderContexts[encoderName].Reset(isolate, context);
  
  //printf("[DEBUG] Stored delta encoder callback for '%s', total encoders: %zu\\n", encoderName.c_str(), deltaEncoderCallbacks.size());
  
  // Create C++ wrapper function specific to this encoder
  // We need to capture the encoderName in the lambda closure
  auto namedWrapperFunction = [encoderName](struct delta_s *pFields, const unsigned char *from, const unsigned char *to) {
    //printf("[DEBUG] Delta encoder callback triggered for: '%s'\\n", encoderName.c_str());
    
    auto callbackIter = deltaEncoderCallbacks.find(encoderName);
    auto contextIter = deltaEncoderContexts.find(encoderName);
    
    //printf("[DEBUG] Looking for delta encoder callback for: '%s'\\n", encoderName.c_str());
    //printf("[DEBUG] Found callback: %s\\n", (callbackIter != deltaEncoderCallbacks.end()) ? "YES" : "NO");
    
    if (callbackIter != deltaEncoderCallbacks.end() && contextIter != deltaEncoderContexts.end()) {
      //printf("[DEBUG] Calling JavaScript delta encoder callback for '%s'\\n", encoderName.c_str());
      auto isolate = nodeImpl.GetIsolate();
      v8::Locker locker(isolate);
      v8::Isolate::Scope isolateScope(isolate);
      v8::HandleScope handleScope(isolate);
      
      v8::Local<v8::Context> ctx = contextIter->second.Get(isolate);
      v8::Context::Scope contextScope(ctx);
      
      v8::Local<v8::Function> callback = callbackIter->second.Get(isolate);
      
      // Prepare arguments for JavaScript callback
      v8::Local<v8::Value> args[3];
      
      // TODO: Properly wrap delta_s structure - for now use null
      args[0] = v8::Null(isolate);
      
      // TODO: Properly wrap byte arrays - for now use null  
      args[1] = v8::Null(isolate);
      args[2] = v8::Null(isolate);
      
      // Call the JavaScript function
      v8::TryCatch tryCatch(isolate);
      callback->Call(ctx, ctx->Global(), 3, args);
      
      if (tryCatch.HasCaught()) {
        v8::String::Utf8Value error(isolate, tryCatch.Exception());
        printf("Error in delta encoder callback: %s\\n", *error);
      }
    } else {
      //printf("[DEBUG] No callback found for delta encoder: '%s'\\n", encoderName.c_str());
    }
  };
  
  // We need to store the lambda function pointer so it doesn't get destroyed
  // Create a static wrapper that captures by value
  static std::unordered_map<std::string, std::function<void(struct delta_s *, const unsigned char *, const unsigned char *)>> deltaEncoderWrappers;
  deltaEncoderWrappers[encoderName] = namedWrapperFunction;
  
  // Convert to C function pointer
  auto cWrapperFunction = [](struct delta_s *pFields, const unsigned char *from, const unsigned char *to) {
    // This is tricky - we need to figure out which encoder this is for
    // For a simplified approach, we'll call all registered encoders
    //printf("[DEBUG] Generic delta encoder wrapper called\\n");
    
    for (auto& pair : deltaEncoderWrappers) {
      //printf("[DEBUG] Trying delta encoder: '%s'\\n", pair.first.c_str());
      pair.second(pFields, from, to);
    }
  };
  
  //printf("[DEBUG] Calling pfnDeltaAddEncoder for '%s'\\n", encoderName.c_str());
  (*g_engfuncs.pfnDeltaAddEncoder)(encoderName.c_str(), cWrapperFunction);
  //printf("[DEBUG] pfnDeltaAddEncoder completed for '%s'\\n", encoderName.c_str());`
      },
      typescript: {
        parameters: [
          { name: 'encoderName', type: 'string', originalType: 'const char *' },
          { name: 'callback', type: '(pFields: any, from: ArrayBuffer | Uint8Array | null, to: ArrayBuffer | Uint8Array | null) => void', originalType: 'void (*)(struct delta_s *, const unsigned char *, const unsigned char *)' }
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