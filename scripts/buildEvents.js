// Создаем events.cpp и events.hpp, в которых все евенты автоматически транслируются в nodejs
import { promises as fs } from 'fs';
import customs from './customs.js';
import fileMaker from './fileMaker.js';
import generator from './generator.js';
import { camelize } from './util.js';
import { parseHamFiles } from './hamParser.js';

function argToValue(arg) {
  return generator.cpp2js(arg.type, arg.name) || 'v8::Boolean::New(isolate, false)';
  //return generator(arg.name);
}

function getFixedArgToValue(arg) {
  // Use custom array conversion if available
  if (arg.jsConversion) {
    return arg.jsConversion;
  }
  
  const value = generator.cpp2js(arg.type, arg.name) || 'v8::Boolean::New(isolate, false)';
  // Fix const void* to void* cast for v8::External::New  
  if (value.includes('v8::External::New(isolate, ') && arg.type.includes('const')) {
    return value.replace('v8::External::New(isolate, ', 'v8::External::New(isolate, (void*)');
  }
  return value;
}

function getReturnStatement(type) {
  const getOverrideCheck = (returnCast) => {
    return `
    if (gpMetaGlobals->mres == MRES_SUPERCEDE || gpMetaGlobals->mres == MRES_OVERRIDE) {
      return ${returnCast}gpMetaGlobals->override_ret;
    }`;
  };

  if (type.includes('*')) {
    return `${getOverrideCheck('('+type+')')}\n    return nullptr;`;
  } else if (type === 'int' || type === 'qboolean' || type === 'unsigned int') {
    return `${getOverrideCheck('('+type+')(intptr_t)')}\n    return 0;`;
  } else if (type === 'float') {
    return `${getOverrideCheck('(float)(intptr_t)')}\n    return 0.0f;`;
  } else if (type === 'double') {
    return `${getOverrideCheck('(double)(intptr_t)')}\n    return 0.0;`;
  } else {
    return `${getOverrideCheck('('+type+')(intptr_t)')}\n    return 0;`;
  }
}

function getEventName(func, prefix) {
  return camelize(`${prefix}${func.name.replace(/^pfn/, '')}`);
}

function getArrayConversion(arrayType, arrayName, lengthName) {
  if (arrayType.includes('int*')) {
    return `utils::intArrayToJS(isolate, ${arrayName}, ${lengthName})`;
  } else if (arrayType.includes('float*')) {
    return `utils::floatArrayToJS(isolate, ${arrayName}, ${lengthName})`;
  } else if (arrayType.includes('byte*') || arrayType.includes('unsigned char*')) {
    return `utils::byteArrayToJS(isolate, ${arrayName}, ${lengthName})`;
  } else {
    // Fallback for other pointer types
    return `utils::intArrayToJS(isolate, ${arrayName}, ${lengthName})`;
  }
}

function getFunction(func, prefix, type) {
  const customBody = customs[type]?.[func.name]?.event?.body;
  customBody && console.log(func.name, 'YES')
  const eventName = getEventName(func, prefix);
  
  // Handle variadic functions specially
  const hasVariadic = func.args && func.args.some(arg => arg.type === '$rest');
  let regularArgs = func.args ? func.args.filter(arg => arg.type !== '$rest') : [];
  
  // Process array + length patterns
  const processedArgs = [];
  const skipIndices = new Set();
  
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
        jsConversion: getArrayConversion(current.type, current.name, next.name),
        isArrayWithLength: true
      });
      skipIndices.add(i + 1); // Skip the length parameter
    } else {
      processedArgs.push(current);
    }
  }
  
  regularArgs = processedArgs;
  
  const description = `// nodemod.on('${eventName}', (${regularArgs.map(v => v.name).join(', ')}) => console.log('${eventName} fired!'));`;
  
  if (customBody) {
    func._eventName = `${prefix}_${func.name}`;
    const needsReturn = func.type !== 'NULL' && func.type !== 'void';
    const returnStatement = needsReturn ? getReturnStatement(func.type) : '';
    return `${description}
  ${func.type === 'NULL' ? 'void' : func.type} ${prefix}_${func.name} (${customs[type]?.[func.name]?.event?.argsString || func.args.map(v => `${v.type} ${v.name}`).join(', ')}) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", [=](v8::Isolate* isolate) {
      ${customBody}
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });${returnStatement}
  }`;
  }

  if (func.type === 'NULL') {
    return `// NULL ${prefix}_${func.name}`;
  }

  func._eventName = `${prefix}_${func.name}`;
  if (func.name === 'pfnStartFrame' && prefix !== 'post') {
    return `// ${func.name} - Run Node.js UV loop tick and fire event
  ${func.type} ${prefix}_${func.name} () {
    SET_META_RESULT(MRES_IGNORED);
    nodeImpl.Tick();
    event::findAndCall("${eventName}", nullptr, 0);
  }`;
  }

  if (func.args.length === 0) {
    const needsReturn = func.type !== 'void';
    const returnStatement = needsReturn ? getReturnStatement(func.type) : '';
    return `${description}
  ${func.type} ${prefix}_${func.name} () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", nullptr, 0);${returnStatement}
  }`;
  }


  const needsReturn = func.type !== 'void';
  const returnStatement = needsReturn ? getReturnStatement(func.type) : '';
  
  // For variadic functions, only include regular arguments in the event
  if (hasVariadic) {
    return `${description}
  ${func.type} ${prefix}_${func.name} (${regularArgs.map(v => `${v.type} ${v.name}`).join(', ')}, ...) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = ${regularArgs.length};
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[${regularArgs.length}];
      ${regularArgs.map((v, i) => `v8_args[${i}] = ${getFixedArgToValue(v)}; // ${v.name} (${v.type})`).join('\n      ')}
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });${returnStatement}
  }`;
  }
  
  return `${description}
  ${func.type} ${prefix}_${func.name} (${func.args.map(v => `${v.type} ${v.name}`).join(', ')}) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = ${regularArgs.length};
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[${regularArgs.length}];
      ${regularArgs.map((v, i) => `v8_args[${i}] = ${getFixedArgToValue(v)}; // ${v.name} (${v.type})`).join('\n      ')}
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });${returnStatement}
  }`;
}

function getFunctions(dllFunctions, prefix, type) {
  return {
    functions: dllFunctions.map(v => getFunction(v, prefix, type)),
    definitions: dllFunctions.map(v => v._eventName || 'NULL')
  };
}

function clearByDefines(list) {
  return list;
  // Не нужно пока
  const isDefineTrue = true;
  const response = [];
  list.forEach(v => {
    if (v.startsWith('#')) {
      console.log('process rule ', v);
      const ifDef = v.match(/#ifdef (.+)/);
      console.log(ifDef)
      return;
    }
  });

  return response;
}

function parseFunction(line) {
  try {
    const prettyLine = line.replace(/[ \t]+/g, ' ');
    const [, type, name, argsPart] = prettyLine.match(/^([0-9a-z_A-Z *]+) ?\(\*([0-9a-zA-Z_]+) ?\) ?\( ?([\s\S]+) ?\)/) || [];
    if (!type) {
      throw Error(`Invalid: ${prettyLine}`);
    }

    // Handle function pointer arguments that might span multiple comma-separated parts
    let processedArgs = [];
    let currentArg = '';
    let parenCount = 0;
    
    const parts = argsPart.split(',');
    for (const part of parts) {
      currentArg += (currentArg ? ',' : '') + part.trim();
      parenCount += (part.match(/\(/g) || []).length;
      parenCount -= (part.match(/\)/g) || []).length;
      
      if (parenCount === 0) {
        processedArgs.push(currentArg);
        currentArg = '';
      }
    }
    
    const args = processedArgs.filter(v => v !== 'void').map((v, i) => {
      if (v === '...') {
        return { name: 'args', type: '$rest' };
      }

      // Handle function pointers like "void (*function) (void)"
      if (v.includes('(*)')) {
        return { name: `callback${i}`, type: 'void*' };
      }

      if (v.match(/^[a-z_A-Z0-9]+ ?\*?$/)) {
        return { name: `value${i}`, type: v };
      }

      const [, type, name] = v.match(/^([a-z_A-Z0-9 ]+[ \*]+ ?)([a-z_A-Z0-9[\]]*)$/) || [];
      if (!type) {
        // Fallback for complex types
        return { name: `value${i}`, type: 'void*' };
      }

      const response = { name: name || `value${i}`, type: type.trim() };
      const [, size] = response.name.match(/\[(\d+)\]$/) || [];
      response.name.includes('[') && console.log(response.name, size)
      if (size) {
        response.type = `${response.type}*`;
        response.size = size;
        response.name = response.name.replace(/\[\d+\]$/, '');
      }

      return response;
    });

    return {
      type: type.trim(),
      name,
      args,
      rawArgs: argsPart,
      original: line
    };
  }
  catch (error) {
    console.log(`Error function: ${line}\n${error.stack}`);
    return {
      type: 'NULL',
      name: line.match(/\(\*([0-9a-zA-Z_]+)\)/)[1],
      original: line
    };
  }
}

(async () => {
  const eifaceContent = await fs.readFile('./build/vcpkg_installed/x86-linux/include/hlsdk/engine/eiface.h').then(v => v.toString());
  const [_, dllFunctionsPart] = eifaceContent.match(/#endif\n\ntypedef struct \n{\n([\s\S]+)\n} DLL_FUNCTIONS;/m);
  const dllFunctionsLines = dllFunctionsPart.split('\n').map(v => v.trim().replace(/\/\*.+\*\//g, '')).filter(v => v && !v.startsWith('//'));
  const dllFunctions = dllFunctionsLines.map(parseFunction);

  const [, engineFunctionsPart] = eifaceContent.match(/typedef struct enginefuncs_s\n{\n([\s\S]+)\n} enginefuncs_t;/m);
  const engineFunctionsLines = clearByDefines(engineFunctionsPart.split('\n').map(v => v.trim().replace(/\/\*.+\*\//g, '')).filter(v => v && !v.startsWith('//')));
  const engineFunctions = engineFunctionsLines.map(parseFunction);
  // return;
  const baseDllFunctions = getFunctions(dllFunctions, 'dll', 'dll');
  const postDllFunctions = getFunctions(dllFunctions, 'postDll', 'dll');

  const computed = {
    dll: dllFunctions.map(v => computeFunction(v, 'dll')),
    eng: engineFunctions.filter(v => !v.name.includes('CRC32')).map(v => computeFunction(v, 'eng'))
  };

  const structureInterfaces = await parseStructureInterfaces();
  
  // Collect all event names from DLL and Engine functions
  const eventNames = [
    ...dllFunctions.map(v => getEventName(v, 'dll')),
    ...dllFunctions.map(v => getEventName(v, 'postDll')),
    ...engineFunctions.map(v => getEventName(v, 'eng')),  
    ...engineFunctions.map(v => getEventName(v, 'postEng'))
  ];

  // Generate event interfaces  
  const eventInterfaces = [
    ...dllFunctions.map(v => computeEventInterface(v, 'dll')),
    ...dllFunctions.map(v => computeEventInterface(v, 'postDll')),
    ...engineFunctions.filter(v => !v.name.includes('CRC32')).map(v => computeEventInterface(v, 'eng')),
    ...engineFunctions.filter(v => !v.name.includes('CRC32')).map(v => computeEventInterface(v, 'postEng'))
  ];

  // Parse Ham files for type generation
  const hamData = await parseHamFiles();

  // Generate split type files
  const typeFiles = fileMaker.typings.makeIndex(computed, structureInterfaces, eventNames, eventInterfaces, hamData);

  // Write each type file to packages/core/types
  for (const [filename, content] of Object.entries(typeFiles)) {
    await fs.writeFile(`./packages/core/types/${filename}`, content);
  }
  const { engineFunctionsFile, dllFunctionsFile } = fileMaker.makeFunctions(computed);
  await fs.writeFile('./src/auto/engine_functions.cpp', engineFunctionsFile);
  await fs.writeFile('./src/auto/dll_functions.cpp', dllFunctionsFile);

  const file = `// This file builded by: node scripts/buildEvents.js
  #include <extdll.h>
  #include "node/nodeimpl.hpp"
  #include "node/events.hpp"
  #include "meta_api.h"
  #include "node/utils.hpp"
  #include "structures/structures.hpp"

  /* BASE EVENTS */
    ${baseDllFunctions.functions.join('\n\n')}

    DLL_FUNCTIONS g_DllFunctionTable = {
      ${baseDllFunctions.definitions.join(',\n      ')}
    };

  /* POST EVENTS */
    ${postDllFunctions.functions.join('\n\n')}

    DLL_FUNCTIONS g_DllFunctionTable_Post = {
      ${postDllFunctions.definitions.join(',\n      ')}
    };

    void registerDllEvents()
    {
      // base
      ${dllFunctions.map(v => `event::register_event("${getEventName(v, 'dll')}", "");`).join('\n')}
      // post
      ${dllFunctions.map(v => `event::register_event("${getEventName(v, 'postDll')}", "");`).join('\n')}
    }
  `;

  await fs.writeFile('./src/auto/dll_events.cpp', file);

  const baseEngineFunctions = getFunctions(engineFunctions, 'eng', 'eng');
  const postEngineFunctions = getFunctions(engineFunctions, 'postEng', 'eng');
  const engineFile = `// This file builded by: node scripts/buildEvents.js
  #include <extdll.h>
  #include "node/nodeimpl.hpp"
  #include "node/events.hpp"
  #include "meta_api.h"
  #include "node/utils.hpp"
  #include "structures/structures.hpp"

  /* BASE EVENTS */
    ${baseEngineFunctions.functions.join('\n\n')}

    enginefuncs_t g_EngineFunctionsTable = {
      ${baseEngineFunctions.definitions.join(',\n      ')}
    };

  /* POST EVENTS */
    ${postEngineFunctions.functions.join('\n\n')}

    enginefuncs_t g_EngineFunctionsTable_Post = {
      ${postEngineFunctions.definitions.join(',\n      ')}
    };

    void registerEngineEvents()
    {
      // base
      ${engineFunctions.map(v => `event::register_event("${getEventName(v, 'eng')}", "");`).join('\n')}
      // post
      ${engineFunctions.map(v => `event::register_event("${getEventName(v, 'postEng')}", "");`).join('\n')}
    }
  `;

  await fs.writeFile('./src/auto/engine_events.cpp', engineFile);


})();

function cTypeToTsType(cType) {
  const normalizedType = cType.trim();
  
  // Basic types - match generator.js TYPE_MAPPINGS.basicTypes
  if (normalizedType === 'int' || normalizedType === 'unsigned int' || normalizedType === 'byte' || 
      normalizedType === 'unsigned short' || normalizedType === 'short' || normalizedType === 'char' ||
      normalizedType === 'unsigned char') {
    return 'number';
  }
  
  if (normalizedType === 'qboolean') {
    return 'boolean';
  }
  
  if (normalizedType === 'float' || normalizedType === 'double') {
    return 'number';
  }
  
  if (normalizedType === 'const char *' || normalizedType === 'const char*' || 
      normalizedType === 'char *' || normalizedType === 'char*') {
    return 'string';
  }
  
  if (normalizedType === 'void') {
    return 'void';
  }
  
  // Enum types - match generator.js enum mappings
  if (normalizedType === 'ALERT_TYPE' || normalizedType === 'FORCE_TYPE' || normalizedType === 'PRINT_TYPE') {
    return 'number';
  }
  
  if (normalizedType === 'CRC32_t') {
    return 'number';
  }
  
  // Vector type - match generator.js vec3_t mapping
  if (normalizedType === 'vec3_t') {
    return 'number[]';
  }
  
  // Entity types - match generator.js struct mappings
  if (normalizedType.includes('edict_t') || normalizedType.includes('edict_s')) {
    return 'Entity';
  }
  
  // Struct types - match generator.js structMappings
  if (normalizedType.includes('entvars_s')) {
    return 'Entvars';
  }
  
  if (normalizedType.includes('clientdata_s')) {
    return 'ClientData';
  }
  
  if (normalizedType.includes('entity_state_s')) {
    return 'EntityState';
  }
  
  if (normalizedType.includes('usercmd_s')) {
    return 'UserCmd';
  }
  
  if (normalizedType.includes('netadr_s')) {
    return 'NetAdr';
  }
  
  if (normalizedType.includes('weapon_data_s')) {
    return 'WeaponData';
  }
  
  if (normalizedType.includes('playermove_s')) {
    return 'PlayerMove';
  }
  
  if (normalizedType.includes('customization_t')) {
    return 'Customization';
  }
  
  if (normalizedType.includes('KeyValueData')) {
    return 'KeyValueData';
  }
  
  if (normalizedType.includes('SAVERESTOREDATA')) {
    return 'SaveRestoreData';
  }
  
  if (normalizedType.includes('TYPEDESCRIPTION')) {
    return 'TypeDescription';
  }
  
  if (normalizedType.includes('delta_s')) {
    return 'Delta';
  }
  
  if (normalizedType.includes('cvar_t') || normalizedType.includes('cvar_s')) {
    return 'Cvar';
  }
  
  if (normalizedType.includes('TraceResult')) {
    return 'TraceResult';
  }
  
  // Array types - match generator.js pointer mappings
  if (normalizedType.includes('float *') || normalizedType.includes('const float *')) {
    return 'number[]';
  }
  
  if (normalizedType.includes('int *') || normalizedType.includes('const int *')) {
    return 'number[]';
  }
  
  if (normalizedType.includes('unsigned char*') || normalizedType.includes('unsigned char *') || 
      normalizedType.includes('byte*') || normalizedType.includes('byte *')) {
    return 'number[]';
  }
  
  if (normalizedType.includes('char **')) {
    return 'string[]';
  }
  
  // FILE pointer
  if (normalizedType === 'FILE *') {
    return 'FileHandle';
  }
  
  // Function pointers
  if (normalizedType.includes('(*') || normalizedType.includes('void *function')) {
    return 'Function';
  }
  
  // Generic void pointer - for binary data/buffers
  if (normalizedType === 'void*' || normalizedType === 'void *') {
    return 'ArrayBuffer | Uint8Array | null'; // Binary data buffers
  }
  
  // Variadic args
  if (normalizedType === '$rest') {
    return '...args: any[]';
  }
  
  // Unknown pointer types - be conservative
  if (normalizedType.includes('*')) {
    return 'unknown'; // Unknown pointer type - needs specific mapping
  }
  
  return 'unknown';
}

function computeFunctionApi(func, source) {
  const jsName = camelize(func.name.replace(/^pfn/, ''));
  const returnType = cTypeToTsType(func.type);
  
  // Check for custom TypeScript definitions in customs.js (for API functions)
  const sourceType = source === 'eng' ? 'eng' : 'dll';
  const customImpl = customs[sourceType] && customs[sourceType][func.name];
  
  if (customImpl && customImpl.typescript && customImpl.typescript.parameters && customImpl.api) {
    // Only apply custom TypeScript if there's also a custom API implementation
    const parameters = customImpl.typescript.parameters;
    const paramTypes = parameters.map(p => `${p.name}: ${p.type}`);
    const customReturnType = customImpl.typescript.returnType || returnType;
    
    return {
      original: func.original,
      definition: `{ "${jsName}", sf_${source}_${func.name} }`,
      body: `// nodemod.${source}.${jsName}();\n${generator.generateCppFunction(func, source === 'eng' ? 'g_engfuncs' : 'gpGamedllFuncs->dllapi_table', `sf_${source}`)}`,
      typing: `${jsName}(${paramTypes.join(', ')}): ${customReturnType}`
    };
  }
  
  // Handle variadic functions
  const hasVariadic = func.args && func.args.some(arg => arg.type === '$rest');
  let regularArgs = func.args ? func.args.filter(arg => arg.type !== '$rest') : [];
  
  // Apply same array + length pattern detection as events (for API functions)
  const processedArgs = [];
  const skipIndices = new Set();
  
  for (let i = 0; i < regularArgs.length; i++) {
    if (skipIndices.has(i)) continue;
    
    const current = regularArgs[i];
    const next = regularArgs[i + 1];
    
    // Check if next parameter is a length parameter AND current is actually an array type
    if (next && next.name.toLowerCase().includes('length') && 
        (current.type.includes('*') && !current.type.includes('char'))) {
      // Current parameter is an array (pointer type, but not char*), next is its length - combine them
      processedArgs.push(current); // Keep only the array parameter
      skipIndices.add(i + 1); // Skip the length parameter
    } else {
      processedArgs.push(current);
    }
  }
  
  regularArgs = processedArgs;
  
  // Fix reserved keywords in parameter names
  const paramTypes = regularArgs.map(arg => {
    let paramName = arg.name;
    // Replace reserved keywords
    if (paramName === 'var') paramName = 'variable';
    if (paramName === 'function') paramName = 'callback';
    if (paramName === 'class') paramName = 'className';
    return `${paramName}: ${cTypeToTsType(arg.type)}`;
  });
  
  // Add variadic args if present
  if (hasVariadic) {
    paramTypes.push('...args: any[]');
  }
  
  return {
    original: func.original,
    definition: `{ "${camelize(func.name.replace(/^pfn/, ''))}", sf_${source}_${func.name} }`,
    body: `// nodemod.${source}.${jsName}(${paramTypes.join(', ')});\n${generator.generateCppFunction(func, source === 'eng' ? 'g_engfuncs' : 'gpGamedllFuncs->dllapi_table', `sf_${source}`)}`,
    typing: `${jsName}(${paramTypes.join(', ')}): ${returnType}`
  };
}

function tc(original, f) {
  try {
    return { status: 'success', ...f() };
  } catch (error) {
    console.log(error.stack);
    return {
      status: 'failed',
      reason: error.message,
      original
    };
  }
}

function computeEventInterface(func, prefix) {
  const eventName = getEventName(func, prefix);
  
  // Check for custom TypeScript definitions in customs.js
  const sourceType = prefix.startsWith('post') ? prefix.replace('post', '').toLowerCase() : prefix;
  const customImpl = customs[sourceType] && customs[sourceType][func.name];
  
  if (customImpl && customImpl.typescript && customImpl.typescript.parameters) {
    const parameters = customImpl.typescript.parameters;
    const paramSignature = parameters.map(p => `${p.name}: ${p.type}`).join(', ');
    const signature = `(${paramSignature}) => void`;
    
    return {
      name: eventName,
      parameters,
      signature,
      hasVariadic: false
    };
  }
  
  // Fall back to original function signature parsing
  const hasVariadic = func.args && func.args.some(arg => arg.type === '$rest');
  let regularArgs = func.args ? func.args.filter(arg => arg.type !== '$rest') : [];
  
  // Apply same array + length pattern detection as events
  const processedArgs = [];
  const skipIndices = new Set();
  
  for (let i = 0; i < regularArgs.length; i++) {
    if (skipIndices.has(i)) continue;
    
    const current = regularArgs[i];
    const next = regularArgs[i + 1];
    
    // Check if next parameter is a length parameter AND current is actually an array type
    if (next && next.name.toLowerCase().includes('length') && 
        (current.type.includes('*') && !current.type.includes('char'))) {
      // Current parameter is an array (pointer type, but not char*), next is its length - combine them
      processedArgs.push(current); // Keep only the array parameter
      skipIndices.add(i + 1); // Skip the length parameter
    } else {
      processedArgs.push(current);
    }
  }
  
  regularArgs = processedArgs;
  
  if (regularArgs.length === 0) {
    return {
      name: eventName,
      parameters: [],
      signature: '() => void'
    };
  }

  const parameters = regularArgs.map(arg => {
    let paramName = arg.name;
    // Replace reserved keywords
    if (paramName === 'var') paramName = 'variable';
    if (paramName === 'function') paramName = 'callback';
    if (paramName === 'class') paramName = 'className';
    
    return {
      name: paramName,
      type: cTypeToTsType(arg.type),
      originalType: arg.type
    };
  });

  const paramSignature = parameters.map(p => `${p.name}: ${p.type}`).join(', ');
  const signature = hasVariadic ? `(${paramSignature}, ...args: any[]) => void` : `(${paramSignature}) => void`;
  
  return {
    name: eventName,
    parameters,
    signature,
    hasVariadic
  };
}

function computeFunction(func, source) {
  return {
    api: tc(func.original, () => computeFunctionApi(func, source)),
    // event: tc(() => computeFunctionEvent(func, source)),
  };
}

async function parseStructureInterfaces() {
  const structureFiles = [
    { file: 'src/structures/entvars.cpp', name: 'Entvars', 
      description: 'Entity variables - properties and state of game entities' },
    { file: 'src/structures/clientdata.cpp', name: 'ClientData',
      description: 'Client-specific data sent from server to client each frame' },
    { file: 'src/structures/entitystate.cpp', name: 'EntityState',
      description: 'Entity state for network transmission' },
    { file: 'src/structures/usercmd.cpp', name: 'UserCmd',
      description: 'Player input commands sent from client to server' },
    { file: 'src/structures/netadr.cpp', name: 'NetAdr',
      description: 'Network address information' },
    { file: 'src/structures/weapondata.cpp', name: 'WeaponData',
      description: 'Weapon state and timing information' },
    { file: 'src/structures/playermove.cpp', name: 'PlayerMove',
      description: 'Player movement state and physics parameters' },
    { file: 'src/structures/customization.cpp', name: 'Customization',
      description: 'Player customization data (sprays, models)' },
    { file: 'src/structures/keyvaluedata.cpp', name: 'KeyValueData',
      description: 'Key-value pairs for entity spawning' },
    { file: 'src/structures/saverestoredata.cpp', name: 'SaveRestoreData',
      description: 'Save/restore game state information' },
    { file: 'src/structures/typedescription.cpp', name: 'TypeDescription',
      description: 'Field type description for save/restore system' },
    { file: 'src/structures/delta.cpp', name: 'Delta',
      description: 'Delta compression structure for network optimization' },
    { file: 'src/structures/cvar.cpp', name: 'Cvar',
      description: 'Console variable (cvar) information' },
    { file: 'src/structures/trace_result.cpp', name: 'TraceResult',
      description: 'Results from line/hull trace operations' }
  ];

  const interfaces = [];

  for (const { file, name, description } of structureFiles) {
    try {
      const content = await fs.readFile(file, 'utf8');
      const properties = parseStructureProperties(content, name);
      interfaces.push({ name, properties, description });
    } catch (error) {
      console.log(`Skipping ${file}: ${error.message}`);
    }
  }

  // Add Entity interface manually since it extends Entvars
  interfaces.push({
    name: 'Entity',
    description: 'Game entity reference with properties and methods',
    extends: 'Entvars', // Entity extends Entvars
    properties: [
      { name: 'id', type: 'number', comment: 'Entity ID from edict index' },
      'getPrivateDataBuffer(offset: number, size: number): Buffer | null',
      'writePrivateDataBuffer(offset: number, buffer: Buffer | ArrayBuffer | Uint8Array): boolean'
    ]
  });

  // Add TraceMonsterHullResult interface
  interfaces.push({
    name: 'TraceMonsterHullResult',
    description: 'Result from monster hull trace operations',
    properties: [
      { name: 'result', type: 'number', comment: 'Return value from pfnTraceMonsterHull - likely collision/movement validity flag' },
      { name: 'trace', type: 'TraceResult', comment: 'Standard trace result with hit information' }
    ]
  });

  return interfaces;
}

function parseStructureProperties(content, structureName) {
  const properties = [];
  const propertyMap = new Map(); // Use Map to avoid duplicates - stores {type, sourceLine}
  
  // Parse new accessor pattern - ACCESSOR_T, ACCESSORL_T macros
  const accessorRegex = /ACCESSOR(?:L)?_T\([^,]+,\s*[^,]+,\s*[^,]+,\s*"([^"]+)",\s*([^,]+),\s*([^,]+),\s*([^)]+)\);/g;
  
  // Parse manual SetNativeDataProperty calls
  const manualAccessorRegex = /templ->SetNativeDataProperty\(v8::String::NewFromUtf8\(isolate,\s*"([^"]+)"\)\.ToLocalChecked\(\)/g;
  
  let match;
  
  // Process ACCESSOR_T/ACCESSORL_T macro calls
  while ((match = accessorRegex.exec(content)) !== null) {
    const propName = match[1]; // Property name
    const fieldName = match[2]; // C++ field name
    const getterType = match[3]; // Getter type (GETN, GETVEC3, etc.)
    const setterType = match[4]; // Setter type (SETINT, SETVEC3, etc.)
    
    const sourceLine = `ACCESSOR_T(..., "${propName}", ${fieldName}, ${getterType}, ${setterType})`;
    
    // Determine TypeScript type from getter/setter types
    let tsType = 'unknown';
    
    if (getterType === 'GETVEC3' || setterType === 'SETVEC3') {
      tsType = 'number[]';
    } else if (getterType === 'GETSTR' || setterType === 'SETSTR') {
      tsType = 'string';
    } else if (getterType === 'GETBOOL' || setterType === 'SETBOOL') {
      tsType = 'boolean';
    } else if (setterType === 'SETINT') {
      tsType = 'number';
    } else if (setterType === 'SETFLOAT') {
      tsType = 'number';
    } else if (getterType === 'GETN') {
      tsType = 'number';
    }
    
    propertyMap.set(propName, { type: tsType, sourceLine });
  }
  
  // Process manual SetNativeDataProperty calls (for complex properties)
  while ((match = manualAccessorRegex.exec(content)) !== null) {
    const propName = match[1];
    
    // Look for the lambda function content after this call
    const startIndex = match.index + match[0].length;
    const contentAfter = content.substring(startIndex, startIndex + 1000);
    
    let tsType = 'unknown';
    const sourceLine = `templ->SetNativeDataProperty("${propName}", ...)`;
    
    // Analyze the lambda content to determine type  
    if (propName === 'pNext' && contentAfter.includes('wrapCustomization')) {
      tsType = 'Customization | null';
    } else if (propName === 'resource' && contentAfter.includes('v8::Object::New') && contentAfter.includes('resObj')) {
      tsType = 'object'; // For nested resource objects
    } else if ((propName === 'pInfo' || propName === 'pBuffer') && contentAfter.includes('v8::Null')) {
      tsType = 'null'; // These always return null for safety
    } else if ((propName === 'ip' || propName === 'ipx') && contentAfter.includes('v8::Array::New')) {
      tsType = 'number[]'; // IP/IPX address arrays
    } else if (propName === 'ipString' && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // IP string representation
    } else if ((propName === 'szClassName' || propName === 'szKeyName' || propName === 'szValue') && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // String properties
    } else if (propName === 'sztexturename' && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // Texture name string
    } else if (propName === 'physinfo' && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // Physics info string
    } else if ((propName === 'fieldName' || propName === 'fieldTypeName') && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // Field name strings
    } else if (propName === 'fieldType' && contentAfter.includes('v8::Number::New')) {
      tsType = 'number'; // Field type enumeration
    } else if ((propName === 'isGlobal' || propName === 'isValid' || propName === 'allSolid' || propName === 'startSolid' || propName === 'inOpen' || propName === 'inWater') && contentAfter.includes('v8::Boolean::New')) {
      tsType = 'boolean'; // Boolean flags
    } else if (propName === 'toString' && contentAfter.includes('v8::String::NewFromUtf8')) {
      tsType = 'string'; // String representation methods
    } else if (propName === 'pointer' && contentAfter.includes('v8::Number::New')) {
      tsType = 'number'; // Raw pointer as number
    } else if (propName === 'model' && contentAfter.includes('pfnSetModel')) {
      tsType = 'string'; // Model path string
    } else if (propName === 'cmd' && contentAfter.includes('wrapUserCmd')) {
      tsType = 'UserCmd | null'; // UserCmd wrapper
    } else if (propName === 'movevars' && contentAfter.includes('v8::Object::New')) {
      tsType = 'object | null'; // Movevars object with properties
    } else if (propName === 'next' && contentAfter.includes('wrapCvar')) {
      tsType = 'Cvar | null'; // Next cvar in linked list
    } else if ((propName === 'pInfo' || propName === 'pBuffer') && contentAfter.includes('v8::Null')) {
      tsType = 'null'; // These are intentionally null for safety
    } else if (contentAfter.includes('wrapCustomization')) {
      tsType = 'Customization | null';
    } else if (contentAfter.includes('wrapEntity')) {
      tsType = 'Entity | null';
    } else if (contentAfter.includes('v8::String::NewFromUtf8') && !contentAfter.includes('pfnSzFromIndex')) {
      tsType = 'string'; // Generic string properties
    } else if (contentAfter.includes('v8::Boolean::New')) {
      tsType = 'boolean'; // Generic boolean properties
    } else if (contentAfter.includes('v8::Number::New')) {
      tsType = 'number'; // Generic number properties
    } else if (contentAfter.includes('v8::Array::New')) {
      tsType = 'number[]'; // Generic arrays
    } else if (contentAfter.includes('v8::Null')) {
      tsType = 'null';
    } else if (contentAfter.includes('v8::Object::New')) {
      tsType = 'object';
    }
    
    propertyMap.set(propName, { type: tsType, sourceLine });
  }
  
  // Extract property setting patterns from the C++ wrapper code  
  // Match obj->Set calls - capture property name and everything up to .Check()
  const setCallRegex = /obj->Set\([^,]*,\s*(?:v8::String::NewFromUtf8\(isolate,\s*"([^"]+)"\)\.ToLocalChecked\(\)|convert::str2js\(isolate,\s*"([^"]+)"\)),[\s\S]*?\)\.Check\(\);/g;
  
  // Process matches
  while ((match = setCallRegex.exec(content)) !== null) {
    const propName = match[1] || match[2]; // Property name from either capture group
    const fullExpression = match[0]; // The full matched expression
    
    // Extract a cleaner source line for the comment
    const sourceLine = fullExpression
      .replace(/\s+/g, ' ') // Normalize whitespace
      .replace(/^obj->Set\([^,]+,\s*/, '') // Remove obj->Set prefix
      .replace(/\)\.Check\(\);$/, ')'); // Remove .Check() suffix
    
    // Determine TypeScript type based on the V8 value creation in the full expression
    let tsType = 'unknown';
    
    // Extract the value part (after the comma) for better type detection
    const commaIndex = fullExpression.indexOf(',', fullExpression.indexOf(propName));
    const valuePart = commaIndex > -1 ? fullExpression.substring(commaIndex) : fullExpression;
    
    // Check patterns in the value part for better accuracy
    // Special handling for specific property names in obj->Set patterns
    if (propName === 'model' && valuePart.includes('pfnSzFromIndex')) {
      tsType = 'string';
    } else if ((propName === 'szClassName' || propName === 'szKeyName' || propName === 'fieldName' || propName === 'fieldTypeName') && valuePart.includes('v8::String::NewFromUtf8')) {
      tsType = 'string';
    } else if (propName === 'fieldType' && valuePart.includes('v8::Number::New')) {
      tsType = 'number';
    } else if ((propName === 'isGlobal' || propName === 'isValid') && valuePart.includes('v8::Boolean::New')) {
      tsType = 'boolean';
    } else if (propName === 'physinfo' && valuePart.includes('v8::String::NewFromUtf8')) {
      tsType = 'string';
    } else if (propName === 'sztexturename' && valuePart.includes('v8::String::NewFromUtf8')) {
      tsType = 'string';
    // Handle ternary expressions with null checks
    } else if (valuePart.includes('?') && valuePart.includes('v8::Null')) {
      // This is a nullable type - check what wrapper is used
      if (valuePart.includes('wrapEntity')) {
        tsType = 'Entity | null';
      } else if (valuePart.includes('wrapEntvars')) {
        tsType = 'Entvars | null';
      } else if (valuePart.includes('wrapTraceResult')) {
        tsType = 'TraceResult | null';
      } else if (valuePart.includes('wrapCvar')) {
        tsType = 'Cvar | null';
      } else if (valuePart.includes('wrapEntityState')) {
        tsType = 'EntityState | null';
      } else if (valuePart.includes('wrapClientData')) {
        tsType = 'ClientData | null';
      } else if (valuePart.includes('wrapUserCmd')) {
        tsType = 'UserCmd | null';
      } else if (valuePart.includes('wrapWeaponData')) {
        tsType = 'WeaponData | null';
      } else if (valuePart.includes('wrapPlayerMove')) {
        tsType = 'PlayerMove | null';
      } else if (valuePart.includes('wrapCustomization')) {
        tsType = 'Customization | null';
      } else if (valuePart.includes('wrapKeyValueData')) {
        tsType = 'KeyValueData | null';
      } else if (valuePart.includes('wrapSaveRestoreData')) {
        tsType = 'SaveRestoreData | null';
      } else if (valuePart.includes('wrapTypeDescription')) {
        tsType = 'TypeDescription | null';
      } else if (valuePart.includes('wrapNetAdr')) {
        tsType = 'NetAdr | null';
      } else if (valuePart.includes('wrapDelta')) {
        tsType = 'Delta | null';
      } else {
        tsType = 'unknown | null';
      }
    } else if (valuePart.includes('wrapEntity')) {
      tsType = 'Entity';
    } else if (valuePart.includes('wrapEntvars')) {
      tsType = 'Entvars';
    } else if (valuePart.includes('wrapTraceResult')) {
      tsType = 'TraceResult';
    } else if (valuePart.includes('wrapCvar')) {
      tsType = 'Cvar';
    } else if (valuePart.includes('wrapEntityState')) {
      tsType = 'EntityState';
    } else if (valuePart.includes('wrapClientData')) {
      tsType = 'ClientData';
    } else if (valuePart.includes('wrapUserCmd')) {
      tsType = 'UserCmd';
    } else if (valuePart.includes('wrapWeaponData')) {
      tsType = 'WeaponData';
    } else if (valuePart.includes('wrapPlayerMove')) {
      tsType = 'PlayerMove';
    } else if (valuePart.includes('wrapCustomization')) {
      tsType = 'Customization';
    } else if (valuePart.includes('wrapKeyValueData')) {
      tsType = 'KeyValueData';
    } else if (valuePart.includes('wrapSaveRestoreData')) {
      tsType = 'SaveRestoreData';
    } else if (valuePart.includes('wrapTypeDescription')) {
      tsType = 'TypeDescription';
    } else if (valuePart.includes('wrapNetAdr')) {
      tsType = 'NetAdr';
    } else if (valuePart.includes('wrapDelta')) {
      tsType = 'Delta';
    } else if (valuePart.includes('g_engfuncs.pfnSzFromIndex')) {
      tsType = 'string';
    } else if (valuePart.includes('convert::strid2js')) {
      tsType = 'string';
    } else if (valuePart.includes('v8::Boolean::New')) {
      tsType = 'boolean';
    } else if (valuePart.includes('utils::vect2js')) {
      tsType = 'number[]';
    } else if (valuePart.includes('utils::arr2js') || valuePart.includes('v8::Array::New')) {
      tsType = 'number[]';
    } else if (valuePart.includes('v8::Number::New')) {
      tsType = 'number';
    } else if (valuePart.includes('v8::Null')) {
      tsType = 'null';
    } else if (valuePart.includes('v8::String::NewFromUtf8') && !valuePart.includes('pfnSzFromIndex')) {
      tsType = 'string';
    }
    
    propertyMap.set(propName, { type: tsType, sourceLine });
  }
  
  // Look for variable assignments that are arrays (for multi-line array creation)
  const arrayVarRegex = /v8::Local<v8::Array>\s+(\w+Array)\s*=/g;
  const arraySetRegex = new RegExp(`obj->Set\\(context,\\s*v8::String::NewFromUtf8\\(isolate,\\s*"([^"]+)"\\)[^,]*,\\s*(\\w+Array)\\)`, 'g');
  
  const arrayVars = [];
  let arrayMatch;
  while ((arrayMatch = arrayVarRegex.exec(content)) !== null) {
    arrayVars.push(arrayMatch[1]);
  }
  
  while ((arrayMatch = arraySetRegex.exec(content)) !== null) {
    const propName = arrayMatch[1];
    const varName = arrayMatch[2];
    if (arrayVars.includes(varName)) {
      // Override with more specific array type and simplified source
      const existing = propertyMap.get(propName);
      propertyMap.set(propName, { 
        type: 'number[]', 
        sourceLine: existing ? existing.sourceLine : `${varName}` 
      });
    }
  }
  
  // Convert Map to array of property strings with comments
  for (const [propName, info] of propertyMap) {
    properties.push({
      name: propName,
      type: info.type,
      comment: info.sourceLine
    });
  }
  
  // Add special handling for specific structures if no properties found
  if (properties.length === 0) {
    switch (structureName) {
      case 'Delta':
        properties.push('// Delta compression structure - internal engine type');
        properties.push('// Fields not exposed in public SDK headers');
        break;
    }
  }
  
  return properties;
}
