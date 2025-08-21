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

function getEventName(func, prefix) {
  return camelize(`${prefix}${func.name.replace(/^pfn/, '')}`);
}

function getFunction(func, prefix, type) {
  const customBody = customs[type]?.[func.name]?.event?.body;
  customBody && console.log(func.name, 'YES')
  const eventName = getEventName(func, prefix);
  const description = `// nodemod.on('${eventName}', (${func.args && func.args.map(v => v.name).join(', ')}) => console.log('${eventName} fired!'));`;
  if (customBody) {
    func._eventName = `${prefix}_${func.name}`;
    return `${description}
  ${func.type === 'NULL' ? 'void' : func.type} ${prefix}_${func.name} (${customs[type]?.[func.name]?.event?.argsString || func.args.map(v => `${v.type} ${v.name}`).join(', ')}) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", [=](v8::Isolate* isolate) {
      ${customBody}
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
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
    return `${description}
  ${func.type} ${prefix}_${func.name} () {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", nullptr, 0);
  }`;
  }


  return `${description}
  ${func.type} ${prefix}_${func.name} (${func.args.map(v => `${v.type} ${v.name}`).join(', ')}) {
    SET_META_RESULT(MRES_IGNORED);
    event::findAndCall("${eventName}", [=](v8::Isolate* isolate) {
      unsigned int v8_argCount = ${func.args.length};
      v8::Local<v8::Value>* v8_args = new v8::Local<v8::Value>[${func.args.length}];
      ${func.args.map((v, i) => `v8_args[${i}] = ${argToValue(v)}; // ${v.name} (${v.type})`).join('\n      ')}
      return std::pair<unsigned int, v8::Local<v8::Value>*>(v8_argCount, v8_args);
    });
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
    const [, type, name, argsPart] = prettyLine.match(/^([0-9a-z_ *]+) ?\(\*([0-9a-zA-Z_]+) ?\) ?\( ?([\s\S]+) ?\)/) || [];
    if (!type) {
      throw Error(`Invalid: ${prettyLine}`);
    }

    const args = argsPart.split(/, ?/).map(v => v.trim()).filter(v => v !== 'void').map((v, i) => {
      if (v === '...') {
        throw Error('rest not support');
        return { name: 'args', type: '$rest' };
      }

      if (v.match(/^[a-z_A-Z0-9]+ ?\*?$/)) {
        return { name: `value${i}`, type: v };
      }

      const [, type, name] = v.match(/^([a-z_A-Z0-9 ]+[ \*]+ ?)([a-z_A-Z0-9[\]]*)$/) || [];
      if (!type) {
        throw Error(`Invalid arg: ${v}`);
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
  const eifaceContent = await fs.readFile('./deps/hlsdk/engine/eiface.h').then(v => v.toString());
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

  await fs.writeFile('./packages/types/@types/index.d.ts', fileMaker.typings.makeIndex(computed));
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

function computeFunctionApi(func, source) {
  const jsName = camelize(func.name.replace(/^pfn/, ''));
  return {
    original: func.original,
    definition: `{ "${camelize(func.name.replace(/^pfn/, ''))}", sf_${source}_${func.name} }`,
    body: `// nodemod.eng.${jsName}();\n${generator.generateCppFunction(func, 'g_engfuncs', 'sf_eng')}`,
    typing: `${jsName}: (${(func.args || []).map(v => v.name).join(', ')}) => unknown`
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
