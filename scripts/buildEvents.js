// Создаем events.cpp и events.hpp, в которых все евенты автоматически транслируются в nodejs
import { promises as fs } from 'fs';
import customs from './customs.js';
import fileMaker from './fileMaker.js';
import generator from './generator.js';
import { camelize } from './util.js';

function argToValue(arg) {
  return generator.cpp2js(arg.type, arg.name) || 'v8::Boolean::New(isolate, false)';
  //return generator(arg.name);
}

function getFixedArgToValue(arg) {
  const value = generator.cpp2js(arg.type, arg.name) || 'v8::Boolean::New(isolate, false)';
  // Fix const void* to void* cast for v8::External::New  
  if (value.includes('v8::External::New(isolate, ') && arg.type.includes('const')) {
    return value.replace('v8::External::New(isolate, ', 'v8::External::New(isolate, (void*)');
  }
  return value;
}

function getReturnStatement(type) {
  if (type.includes('*')) {
    return '\n    return nullptr;';
  } else if (type === 'int' || type === 'qboolean' || type === 'unsigned int') {
    return '\n    return 0;';
  } else if (type === 'float') {
    return '\n    return 0.0f;';
  } else if (type === 'double') {
    return '\n    return 0.0;';
  } else {
    return '\n    return 0;';
  }
}

function getEventName(func, prefix) {
  return camelize(`${prefix}${func.name.replace(/^pfn/, '')}`);
}

function getFunction(func, prefix, type) {
  const customBody = customs[type]?.[func.name]?.event?.body;
  customBody && console.log(func.name, 'YES')
  const eventName = getEventName(func, prefix);
  
  // Handle variadic functions specially
  const hasVariadic = func.args && func.args.some(arg => arg.type === '$rest');
  const regularArgs = func.args ? func.args.filter(arg => arg.type !== '$rest') : [];
  
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
    return `// ${func.name}
  ${func.type} ${prefix}_${func.name} () {
    SET_META_RESULT(MRES_IGNORED);
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
      unsigned int v8_argCount = ${func.args.length};
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[${func.args.length}];
      ${func.args.map((v, i) => `v8_args[${i}] = ${getFixedArgToValue(v)}; // ${v.name} (${v.type})`).join('\n      ')}
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
  await fs.writeFile('./packages/core/index.d.ts', fileMaker.typings.makeIndex(computed, structureInterfaces));
  const { engineFunctionsFile, dllFunctionsFile } = fileMaker.makeFunctions(computed);
  await fs.writeFile('./src/auto/engine_functions.cpp', engineFunctionsFile);

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
  
  if (normalizedType.includes('unsigned char *') || normalizedType.includes('byte *')) {
    return 'Uint8Array';
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
  
  // Generic void pointer (for private data, buffers, etc)
  if (normalizedType === 'void*' || normalizedType === 'void *') {
    return 'ArrayBuffer | null';
  }
  
  // Variadic args
  if (normalizedType === '$rest') {
    return '...args: any[]';
  }
  
  // Pointer types default to ArrayBuffer
  if (normalizedType.includes('*')) {
    return 'ArrayBuffer | null';
  }
  
  return 'unknown';
}

function computeFunctionApi(func, source) {
  const jsName = camelize(func.name.replace(/^pfn/, ''));
  const returnType = cTypeToTsType(func.type);
  
  // Handle variadic functions
  const hasVariadic = func.args && func.args.some(arg => arg.type === '$rest');
  const regularArgs = func.args ? func.args.filter(arg => arg.type !== '$rest') : [];
  
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
    body: `// nodemod.eng.${jsName}();\n${generator.generateCppFunction(func, 'g_engfuncs', 'sf_eng')}`,
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

function computeFunction(func, source) {
  return {
    api: tc(func.original, () => computeFunctionApi(func, source)),
    // event: tc(() => computeFunctionEvent(func, source)),
  };
}

async function parseStructureInterfaces() {
  const structureFiles = [
    { file: 'src/structures/entvars.cpp', name: 'Entvars' },
    { file: 'src/structures/clientdata.cpp', name: 'ClientData' },
    { file: 'src/structures/entitystate.cpp', name: 'EntityState' },
    { file: 'src/structures/usercmd.cpp', name: 'UserCmd' },
    { file: 'src/structures/netadr.cpp', name: 'NetAdr' },
    { file: 'src/structures/weapondata.cpp', name: 'WeaponData' },
    { file: 'src/structures/playermove.cpp', name: 'PlayerMove' },
    { file: 'src/structures/customization.cpp', name: 'Customization' },
    { file: 'src/structures/keyvaluedata.cpp', name: 'KeyValueData' },
    { file: 'src/structures/saverestoredata.cpp', name: 'SaveRestoreData' },
    { file: 'src/structures/typedescription.cpp', name: 'TypeDescription' },
    { file: 'src/structures/delta.cpp', name: 'Delta' },
    { file: 'src/structures/cvar.cpp', name: 'Cvar' },
    { file: 'src/structures/trace_result.cpp', name: 'TraceResult' }
  ];

  const interfaces = [];

  for (const { file, name } of structureFiles) {
    try {
      const content = await fs.readFile(file, 'utf8');
      const properties = parseStructureProperties(content, name);
      interfaces.push({ name, properties });
    } catch (error) {
      console.log(`Skipping ${file}: ${error.message}`);
    }
  }

  // Add Entity interface manually since it's more complex
  interfaces.push({
    name: 'Entity',
    properties: [
      'id: number',
      'classname: string',
      'globalname: string',
      'origin: number[]',
      'oldorigin: number[]',
      'velocity: number[]',
      'basevelocity: number[]',
      'clbasevelocity: number[]',
      'movedir: number[]',
      'angles: number[]',
      'avelocity: number[]',
      'punchangle: number[]',
      'angle: number[]',
      'endpos: number[]',
      'startpos: number[]',
      'impacttime: number',
      'starttime: number',
      'fixangle: number',
      'idealpitch: number',
      'pitchSpeed: number',
      'idealYaw: number',
      'yawSpeed: number',
      'modelindex: number',
      'model: string',
      'viewmodel: number',
      'weaponmodel: number',
      'absmin: number[]',
      'absmax: number[]',
      'mins: number[]',
      'maxs: number[]',
      'size: number[]',
      'ltime: number',
      'nextthink: number',
      'movetype: number',
      'solid: number',
      'skin: number',
      'body: number',
      'effects: number',
      'gravity: number',
      'friction: number',
      'lightLevel: number',
      'sequence: number',
      'gaitsequence: number',
      'frame: number',
      'animtime: number',
      'framerate: number',
      'scale: number',
      'rendermode: number',
      'renderamt: number',
      'rendercolor: number[]',
      'renderfx: number',
      'health: number',
      'frags: number',
      'weapons: number',
      'takedamage: number',
      'deadflag: number',
      'viewOfs: number[]',
      'button: number',
      'impulse: number',
      'spawnflags: number',
      'flags: number',
      'colormap: number',
      'team: number',
      'maxHealth: number',
      'teleportTime: number',
      'armortype: number',
      'armorvalue: number',
      'waterlevel: number',
      'watertype: number',
      'target: string',
      'targetname: string',
      'netname: string',
      'message: string',
      'dmgTake: number',
      'dmgSave: number',
      'dmg: number',
      'dmgtime: number',
      'noise: string',
      'noise1: string',
      'noise2: string',
      'noise3: string',
      'speed: number',
      'airFinished: number',
      'painFinished: number',
      'radsuitFinished: number',
      'playerclass: number',
      'maxspeed: number',
      'fov: number',
      'weaponanim: number',
      'pushmsec: number',
      'bInDuck: number',
      'flTimeStepSound: number',
      'flSwimTime: number',
      'flDuckTime: number',
      'iStepLeft: number',
      'fallVelocity: number',
      'gamestate: number',
      'oldbuttons: number',
      'groupinfo: number'
    ]
  });

  return interfaces;
}

function parseStructureProperties(content, structureName) {
  const properties = [];
  
  // Extract property setting patterns from the C++ wrapper code
  // Match obj->Set calls with various V8 value types
  const setCallRegex = /obj->Set\(context,\s*v8::String::NewFromUtf8\(isolate,\s*"([^"]+)"\)[^,]*,\s*([^;]+);/g;
  
  let match;
  while ((match = setCallRegex.exec(content)) !== null) {
    const propName = match[1];
    const valueExpression = match[2];
    
    // Determine TypeScript type based on the V8 value creation
    let tsType = 'unknown';
    
    if (valueExpression.includes('v8::String::NewFromUtf8') || valueExpression.includes('g_engfuncs.pfnSzFromIndex')) {
      tsType = 'string';
    } else if (valueExpression.includes('v8::Number::New')) {
      tsType = 'number';
    } else if (valueExpression.includes('v8::Boolean::New')) {
      tsType = 'boolean';
    } else if (valueExpression.includes('utils::vect2js')) {
      tsType = 'number[]';
    } else if (valueExpression.includes('wrapEntity')) {
      tsType = 'Entity | null';
    } else if (valueExpression.includes('utils::arr2js') || valueExpression.includes('Array::New') || (propName.includes('ip') && valueExpression.includes('Array'))) {
      tsType = 'number[]';
    }
    
    properties.push(`${propName}: ${tsType}`);
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
      properties.push(`${propName}: number[]`);
    }
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
