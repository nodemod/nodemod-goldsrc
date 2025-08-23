const fileMaker = {
  makeFile(...body) {
    return `// This file is generated automatically. Don't edit it.\n${body.join('\n')}`;
  },
  makeBlock(prefix, body) {
    return `${prefix} {\n${body.split('\n').map(v => `  ${v}`).join('\n')}\n};`;
  },
  typings: {
    makeIndex(computed, structureInterfaces = []) {
      const interfaceDefinitions = structureInterfaces.map(iface => 
        `  interface ${iface.name} {\n${iface.properties.map(prop => `    ${prop};`).join('\n')}\n  }`
      ).join('\n');
      
      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  interface FileHandle {',
        '    // Opaque file handle - use with engine file functions',
        '  }',
        interfaceDefinitions,
        `  interface Engine {`,
        computed.eng.map(v => `    /** ${v.api.original} */\n    ${v.api.typing};`).join('\n'),
        `  }`,
        `  const eng: Engine;`,
        `}`
      );
    }
  },
  makeFunctions(computed, source) {
    if (!source) {
      return {
        engineFunctionsFile: this.makeFunctions(computed.eng.map(v => v.api), 'eng'),
        dllFunctionsFile: this.makeFunctions(computed.dll.map(v => v.api), 'dll')
      };
    }

    console.log(computed.filter(v => v.status === 'failed'))
    return fileMaker.makeFile(
      [
        '#include <string>',
        '#include "v8.h"',
        '#include "extdll.h"',
        '#include "node/utils.hpp"',
        '#include "structures/structures.hpp"'
      ].join('\n'),
      '',
      'extern enginefuncs_t	 g_engfuncs;',
      '',
      computed.filter(v => v.status === 'success').map(v => v.body).join('\n\n'),
      '',
      fileMaker.makeBlock(
        'static std::pair<std::string, v8::FunctionCallback> engineSpecificFunctions[] =',
        computed.filter(v => v.status === 'success').map(v => v.definition).join(',\n')
      ),
      fileMaker.makeBlock(
        'v8::Local<v8::ObjectTemplate> registerEngineFunctions(v8::Isolate* isolate)',
        [
          'v8::Local <v8::ObjectTemplate> object = v8::ObjectTemplate::New(isolate);',
          fileMaker.makeBlock(
            `for (auto &routine : engineSpecificFunctions)`,
            'object-> Set(v8::String::NewFromUtf8(isolate, routine.first.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), v8::FunctionTemplate::New(isolate, routine.second));'
          ),
          '',
          'return object;'
        ].join('\n')
      ),
      '',
      computed.filter(v => v.status === 'failed').map(v => `// FAILED (${v.reason}): ${v.original}`).join('\n')
    );
  }
};

export default fileMaker;