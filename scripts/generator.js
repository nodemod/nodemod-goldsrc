import customs from './customs.js';

// Consolidated type mappings to eliminate redundancy
const TYPE_MAPPINGS = {
  // Basic types - js2cpp and cpp2js mappings
  basicTypes: {
    'const char *': { 
      js2cpp: (value) => `utils::js2string(isolate, ${value})`,
      cpp2js: (value) => `v8::String::NewFromUtf8(isolate, ${value} ? ${value} : "").ToLocalChecked()`
    },
    'const char*': { 
      js2cpp: (value) => `utils::js2string(isolate, ${value})`,
      cpp2js: (value) => `v8::String::NewFromUtf8(isolate, ${value} ? ${value} : "").ToLocalChecked()`
    },
    'char *': { 
      js2cpp: (value) => `utils::js2string(isolate, ${value})`,
      cpp2js: (value) => `v8::String::NewFromUtf8(isolate, ${value} ? ${value} : "").ToLocalChecked()`
    },
    'char*': { 
      js2cpp: (value) => `utils::js2string(isolate, ${value})`,
      cpp2js: (value) => `v8::String::NewFromUtf8(isolate, ${value} ? ${value} : "").ToLocalChecked()`
    },
    'char': { 
      js2cpp: (value) => `${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'int': { 
      js2cpp: (value) => `${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'unsigned int': { 
      js2cpp: (value) => `${value}->Uint32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'float': { 
      js2cpp: (value) => `${value}->NumberValue(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'double': { 
      js2cpp: (value) => `${value}->NumberValue(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'unsigned char': { 
      js2cpp: (value) => `${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'unsigned short': { 
      js2cpp: (value) => `${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'byte': { 
      js2cpp: (value) => `${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'qboolean': { 
      js2cpp: (value) => `${value}->BooleanValue(isolate)`,
      cpp2js: (value) => `v8::Boolean::New(isolate, ${value})`
    },
    'ALERT_TYPE': { 
      js2cpp: (value) => `(ALERT_TYPE)${value}->Int32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'FORCE_TYPE': { 
      js2cpp: (value) => `*(FORCE_TYPE*)utils::jsToBytes(isolate, ${value})`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'PRINT_TYPE': { 
      js2cpp: (value) => `*(PRINT_TYPE*)utils::jsToBytes(isolate, ${value})`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'CRC32_t': { 
      js2cpp: (value) => `${value}->Uint32Value(context).ToChecked()`,
      cpp2js: (value) => `v8::Number::New(isolate, ${value})`
    },
    'vec3_t': { 
      js2cpp: (value, paramName) => `${paramName}_vec`, // Return variable name - will be handled specially
      cpp2js: (value) => `utils::vect2js(isolate, ${value})`,
    },
    'TraceResult': { 
      js2cpp: (value) => `structures::unwrapTraceResult(isolate, ${value})`,
      cpp2js: (value) => `structures::wrapTraceResult(isolate, &${value})`
    },
    'void*': { 
      js2cpp: (value) => `nullptr /* void* not supported */`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    'FILE *': { 
      js2cpp: (value) => `nullptr /* FILE* not supported */`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    '$rest': { 
      js2cpp: (value) => `/* variadic args */`,
      cpp2js: (value) => `/* variadic args */`
    },
    // Direct pointer type matches
    'int *': { 
      js2cpp: (value) => `(int*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::intArrayToJS(isolate, ${value}, 1)` // Default to 1, may need custom handling for larger arrays
    },
    'float *': { 
      js2cpp: (value) => `(float*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::floatArrayToJS(isolate, ${value}, 3)` // Most float* in HLSDK are vec3_t
    },
    'const float *': { 
      js2cpp: (value) => `(const float*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::floatArrayToJS(isolate, ${value}, 3)` // Most const float* in HLSDK are vec3_t
    },
    'void *': { 
      js2cpp: (value) => `utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    'unsigned char *': { 
      js2cpp: (value) => `(unsigned char*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::byteArrayToJS(isolate, ${value}, 1)` // Default to 1, may need custom handling for larger arrays
    },
    'unsigned char **': { 
      js2cpp: (value) => `(unsigned char**)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    'char **': { 
      js2cpp: (value) => `(char**)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::stringArrayToJS(isolate, ${value})`
    },
    'const struct usercmd_s *': { 
      js2cpp: (value) => `structures::unwrapUserCmd(isolate, ${value})`,
      cpp2js: (value) => `structures::wrapUserCmd(isolate, (void*)${value})`
    },
    'const struct netadr_s *': { 
      js2cpp: (value) => `structures::unwrapNetAdr(isolate, ${value})`,
      cpp2js: (value) => `structures::wrapNetAdr(isolate, (void*)${value})`
    }
  },

  // Struct mappings - base name to wrapper functions
  structMappings: {
    'edict_t': { unwrap: 'structures::unwrapEntity', wrap: 'structures::wrapEntity' },
    'edict_s': { unwrap: 'structures::unwrapEntity', wrap: 'structures::wrapEntity' },
    'entvars_s': { unwrap: 'structures::unwrapEntvars', wrap: 'structures::wrapEntvars' },
    'clientdata_s': { unwrap: 'structures::unwrapClientData', wrap: 'structures::wrapClientData' },
    'entity_state_s': { unwrap: 'structures::unwrapEntityState', wrap: 'structures::wrapEntityState' },
    'usercmd_s': { unwrap: 'structures::unwrapUserCmd', wrap: 'structures::wrapUserCmd' },
    'netadr_s': { unwrap: 'structures::unwrapNetAdr', wrap: 'structures::wrapNetAdr' },
    'weapon_data_s': { unwrap: 'structures::unwrapWeaponData', wrap: 'structures::wrapWeaponData' },
    'playermove_s': { unwrap: 'structures::unwrapPlayerMove', wrap: 'structures::wrapPlayerMove' },
    'customization_t': { unwrap: 'structures::unwrapCustomization', wrap: 'structures::wrapCustomization' },
    'KeyValueData': { unwrap: 'structures::unwrapKeyValueData', wrap: 'structures::wrapKeyValueData' },
    'SAVERESTOREDATA': { unwrap: 'structures::unwrapSaveRestoreData', wrap: 'structures::wrapSaveRestoreData' },
    'TYPEDESCRIPTION': { unwrap: 'structures::unwrapTypeDescription', wrap: 'structures::wrapTypeDescription' },
    'delta_s': { unwrap: 'structures::unwrapDelta', wrap: 'structures::wrapDelta' },
    'cvar_s': { unwrap: 'structures::unwrapCvar', wrap: 'structures::wrapCvar' },
    'cvar_t': { unwrap: 'structures::unwrapCvar', wrap: 'structures::wrapCvar' },
    'TraceResult': { unwrap: 'structures::unwrapTraceResult', wrap: 'structures::wrapTraceResult' }
  },

  // Pointer type mappings
  pointerMappings: {
    'float': { 
      js2cpp: (value) => `(float*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::floatArrayToJS(isolate, ${value}, 3)` // Most float* in HLSDK are 3D vectors
    },
    'int': { 
      js2cpp: (value) => `(int*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `v8::Number::New(isolate, *${value})` // Most int* in HLSDK are single output values
    },
    'char *': { 
      js2cpp: (value) => `utils::js2string(isolate, ${value})`,
      cpp2js: (value) => `v8::String::NewFromUtf8(isolate, ${value} ? ${value} : "").ToLocalChecked()`
    },
    'void': { 
      js2cpp: (value) => `utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    'CRC32_t': { 
      js2cpp: (value) => `(CRC32_t*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    },
    'unsigned char': { 
      js2cpp: (value) => `(unsigned char*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::byteArrayToJS(isolate, ${value}, 1)` // Default to single byte, needs custom handling for actual arrays
    },
    'byte': { 
      js2cpp: (value) => `(byte*)utils::jsToPointer(isolate, ${value})`,
      cpp2js: (value) => `utils::byteArrayToJS(isolate, ${value}, 1)` // Default to single byte, needs custom handling for actual arrays  
    },
    'FILE': { 
      js2cpp: (value) => `nullptr /* FILE* not supported */`,
      cpp2js: (value) => `v8::External::New(isolate, ${value})`
    }
  }
};

// Helper function to normalize type strings
function normalizeType(type) {
  return type.trim();
}

// Helper function to extract struct name from pointer types
function extractStructName(type) {
  const match = type.match(/^(?:const\s+)?(?:struct\s+)?(.+?)\s*\*?\s*$/);
  return match ? match[1].trim() : null;
}

// Helper function to check if a type is a pointer
function isPointerType(type) {
  return type.includes('*');
}

// Helper function to get struct mapping
function getStructMapping(type, isJs2Cpp, value) {
  const structName = extractStructName(type);
  if (!structName) return null;

  const mapping = TYPE_MAPPINGS.structMappings[structName];
  if (!mapping) return null;

  if (isJs2Cpp) {
    return `${mapping.unwrap}(isolate, ${value})`;
  } else {
    return `${mapping.wrap}(isolate, ${value})`;
  }
}

// Helper function to get pointer mapping
function getPointerMapping(type, isJs2Cpp, value) {
  const baseName = extractStructName(type.replace(/\*+$/, ''));
  if (!baseName) return null;

  const mapping = TYPE_MAPPINGS.pointerMappings[baseName];
  if (!mapping) return null;

  if (isJs2Cpp) {
    return mapping.js2cpp(value);
  } else {
    return mapping.cpp2js(value);
  }
}

// Генератор транслейтов
const generator = {
  js2cpp(type, value, paramName = 'param') {
    const normalizedType = normalizeType(type);

    // Check basic types first
    const basicMapping = TYPE_MAPPINGS.basicTypes[normalizedType];
    if (basicMapping) {
      return basicMapping.js2cpp(value, paramName);
    }

    // Try struct mapping
    const structMapping = getStructMapping(normalizedType, true, value);
    if (structMapping) {
      return structMapping;
    }

    // Try pointer mapping if it's a pointer type
    if (isPointerType(normalizedType)) {
      const pointerMapping = getPointerMapping(normalizedType, true, value);
      if (pointerMapping) {
        return pointerMapping;
      }
    }

    console.warn(`No '${type}' js2cpp`);
    return `nullptr /* ${type} */`;
  },

  cpp2js(type, value) {
    const normalizedType = normalizeType(type);

    // Check basic types first
    const basicMapping = TYPE_MAPPINGS.basicTypes[normalizedType];
    if (basicMapping) {
      return basicMapping.cpp2js(value);
    }

    // Try struct mapping
    const structMapping = getStructMapping(normalizedType, false, value);
    if (structMapping) {
      return structMapping;
    }

    // Try pointer mapping if it's a pointer type
    if (isPointerType(normalizedType)) {
      const pointerMapping = getPointerMapping(normalizedType, false, value);
      if (pointerMapping) {
        return pointerMapping;
      }
    }

    console.warn(`Type ${type} not found: cpp2js`);
    return `v8::External::New(isolate, ${value} /* ${type} */)`;
  },

  packReturn(func, value) {
    if (func.type === 'void') {
      return value;
    }

    const generated = this.cpp2js(func.type, value)
    if (!generated || generated.includes('/* ') && generated.includes(' */')) {
      console.log(`No ${func.type} type packReturn`);
      if (func.type.includes('*')) {
        return `info.GetReturnValue().Set(v8::External::New(isolate, ${value}));`;
      }
      return `${value} /* TODO: type ${func.type} */`;
    }

    // For string types, use a temporary variable to avoid calling the function twice
    if ((func.type.includes('char *') || func.type.includes('char*')) && generated.includes('?')) {
      return `const char* temp_str = ${value};
  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, temp_str ? temp_str : "").ToLocalChecked())`;
    }

    return `info.GetReturnValue().Set(${generated})`;
  },

  generateCppFunction(func, source, prefix) {
    const customBody = customs[prefix.split('_')[1]]?.[func.name]?.api?.body;
    
    // Apply same array + length pattern detection as events and TypeScript generation
    let regularArgs = func.args ? [...func.args] : [];
    const processedArgs = [];
    const skipIndices = new Set();
    
    // Detect array + length parameter pairs
    for (let i = 0; i < regularArgs.length; i++) {
      if (skipIndices.has(i)) continue;
      
      const current = regularArgs[i];
      const next = regularArgs[i + 1];
      
      // Check if next parameter is a length parameter AND current is actually an array type
      if (next && next.name.toLowerCase().includes('length') && 
          (current.type.includes('*') && !current.type.includes('char'))) {
        // Current parameter is an array (pointer type, but not char*), next is its length
        processedArgs.push({
          ...current,
          isArrayWithLength: true,
          lengthParam: next
        });
        skipIndices.add(i + 1); // Skip the length parameter
      } else {
        processedArgs.push(current);
      }
    }
    
    regularArgs = processedArgs;
    
    // Generate warning messages for pointer parameters instead of early returns
    const structureTypes = ['edict_t', 'edict_s', 'entvars_s', 'clientdata_s', 'entity_state_s', 'usercmd_s', 'netadr_s', 'weapon_data_s', 'playermove_s', 'customization_t', 'KeyValueData', 'SAVERESTOREDATA', 'TYPEDESCRIPTION', 'delta_s', 'cvar_s', 'TraceResult'];
    const nullChecks = regularArgs.map((arg, i) => {
      if (arg.type.includes('*') && !arg.type.includes('char') && !arg.isArrayWithLength) {
        // Skip validation for structure types that use wrap/unwrap pattern
        const isStructureType = structureTypes.some(structType => arg.type.includes(structType));
        if (isStructureType) {
          return '';
        }
        return `if (!info[${i}]->IsExternal()) {
    printf("Warning: ${func.name} parameter ${i} (${arg.type}) is not External, using nullptr\\n");
  }`;
      }
      return '';
    }).filter(check => check).join('\n  ');
    
    const nullCheckSection = nullChecks ? `\n  ${nullChecks}\n` : '';
    
    // Generate parameter conversion code
    const paramConversions = regularArgs.map((arg, i) => {
      // Handle vec3_t parameters specially
      if (arg.type === 'vec3_t') {
        return `
  vec3_t ${arg.name}_vec;
  utils::js2vect(isolate, v8::Local<v8::Array>::Cast(info[${i}]), ${arg.name}_vec);`;
      } else if (arg.isArrayWithLength) {
        // Handle array + length parameter conversion
        const arrayType = arg.type;
        const lengthParamName = arg.lengthParam.name;
        
        if (arrayType.includes('int*')) {
          return `
  // Convert JavaScript array to int array for ${arg.name}
  v8::Local<v8::Array> ${arg.name}_array = v8::Local<v8::Array>::Cast(info[${i}]);
  int ${lengthParamName} = ${arg.name}_array->Length();
  int* ${arg.name} = new int[${lengthParamName}];
  for (int j = 0; j < ${lengthParamName}; j++) {
    ${arg.name}[j] = ${arg.name}_array->Get(context, j).ToLocalChecked()->Int32Value(context).ToChecked();
  }`;
        } else if (arrayType.includes('float*')) {
          return `
  // Convert JavaScript array to float array for ${arg.name}
  v8::Local<v8::Array> ${arg.name}_array = v8::Local<v8::Array>::Cast(info[${i}]);
  int ${lengthParamName} = ${arg.name}_array->Length();
  float* ${arg.name} = new float[${lengthParamName}];
  for (int j = 0; j < ${lengthParamName}; j++) {
    ${arg.name}[j] = ${arg.name}_array->Get(context, j).ToLocalChecked()->NumberValue(context).ToChecked();
  }`;
        } else if (arrayType.includes('unsigned char*') || arrayType.includes('byte*')) {
          return `
  // Convert JavaScript array to byte array for ${arg.name}
  v8::Local<v8::Array> ${arg.name}_array = v8::Local<v8::Array>::Cast(info[${i}]);
  int ${lengthParamName} = ${arg.name}_array->Length();
  unsigned char* ${arg.name} = new unsigned char[${lengthParamName}];
  for (int j = 0; j < ${lengthParamName}; j++) {
    ${arg.name}[j] = ${arg.name}_array->Get(context, j).ToLocalChecked()->Int32Value(context).ToChecked();
  }`;
        } else {
          // Fallback for unknown array types - use proper array conversion
          return `
  // Convert JavaScript array to C array for ${arg.name} (${arrayType})
  v8::Local<v8::Array> ${arg.name}_array = v8::Local<v8::Array>::Cast(info[${i}]);
  int ${lengthParamName} = ${arg.name}_array->Length();
  int* ${arg.name} = new int[${lengthParamName}]; // Default to int array for unknown types
  for (int j = 0; j < ${lengthParamName}; j++) {
    ${arg.name}[j] = ${arg.name}_array->Get(context, j).ToLocalChecked()->Int32Value(context).ToChecked();
  }`;
        }
      } else {
        // Regular parameter - no conversion needed, just js2cpp mapping
        return '';
      }
    }).filter(conv => conv).join('');
    
    // Generate cleanup code for allocated arrays
    const cleanupCode = regularArgs.map((arg) => {
      if (arg.isArrayWithLength) {
        return `\n  delete[] ${arg.name}; // Free allocated array`;
      }
      return '';
    }).filter(cleanup => cleanup).join('');
    
    // Generate function call parameters - use original func.args for proper mapping to C function
    const callParams = (func.args || []).map((arg, i) => {
      // Check if this argument was processed as part of an array+length pair
      const processedArg = regularArgs.find(pArg => pArg.name === arg.name);
      if (processedArg && processedArg.isArrayWithLength) {
        // This is the array parameter - just use the variable name
        return arg.name;
      } else if (regularArgs.some(pArg => pArg.lengthParam && pArg.lengthParam.name === arg.name)) {
        // This is the length parameter - use the length variable
        return arg.name;
      } else {
        // Regular parameter - use js2cpp conversion
        const argIndex = regularArgs.findIndex(pArg => pArg.name === arg.name);
        if (argIndex >= 0) {
          return this.js2cpp(arg.type, `info[${argIndex}]`, arg.name);
        } else {
          // This shouldn't happen, but provide a fallback
          return `nullptr /* missing parameter: ${arg.name} */`;
        }
      }
    }).join(',\n');
    
    return `void ${prefix}_${func.name}(const v8::FunctionCallbackInfo<v8::Value>& info)
{
  V8_STUFF();${paramConversions}
${nullCheckSection}
  ${customBody || this.packReturn(func, `(*${source}${source.includes('->') ? '->' : '.'}${func.name})(${callParams})`)};${cleanupCode}
}`;
  }
}

export default generator;