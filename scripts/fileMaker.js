const fileMaker = {
  makeFile(...body) {
    return `// This file is generated automatically. Don't edit it.\n${body.join('\n')}`;
  },
  makeBlock(prefix, body) {
    return `${prefix} {\n${body.split('\n').map(v => `  ${v}`).join('\n')}\n};`;
  },
  typings: {
    makeIndex(computed, structureInterfaces = [], eventNames = [], eventInterfaces = []) {
      // Split into multiple files for better organization
      const enums = this.makeEnums();
      const eventTypes = this.makeEventTypes(eventNames, eventInterfaces);
      const structures = this.makeStructures(structureInterfaces);
      const engine = this.makeEngine(computed);
      const core = this.makeCore();
      
      return {
        'enums.d.ts': enums,
        'events.d.ts': eventTypes, 
        'structures.d.ts': structures,
        'engine.d.ts': engine,
        'index.d.ts': core
      };
    },
    
    makeEnums() {
      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  // Metamod result constants',
        '  enum META_RES {',
        '    UNSET = 0,    // Uninitialized (causes error)',
        '    IGNORED = 1,  // Plugin didn\'t take any action, continue normally',
        '    HANDLED = 2,  // Plugin did something, but original function still executes',  
        '    OVERRIDE = 3, // Execute original function, but use plugin\'s return value',
        '    SUPERCEDE = 4 // Skip original function entirely, use plugin\'s behavior',
        '  }',
        '',
        '  // Alert types for engine functions',
        '  enum ALERT_TYPE {',
        '    at_notice = 0,',
        '    at_console = 1,',
        '    at_aiconsole = 2,',
        '    at_warning = 3,',
        '    at_error = 4,',
        '    at_logged = 5',
        '  }',
        '',
        '  // Print types for client output',
        '  enum PRINT_TYPE {',
        '    print_console = 0,',
        '    print_center = 1,',
        '    print_chat = 2',
        '  }',
        '',
        '  // Force types for consistency checking',
        '  enum FORCE_TYPE {',
        '    force_exactfile = 0,',
        '    force_model_samebounds = 1,',
        '    force_model_specifybounds = 2,',
        '    force_model_specifybounds_if_avail = 3',
        '  }',
        '}'
      );
    },

    makeEventTypes(eventNames, eventInterfaces) {
      return fileMaker.makeFile(
        '/// <reference path="./structures.d.ts" />',
        '',
        'declare namespace nodemod {',
        '  // Event callbacks map',
        '  interface EventCallbacks {',
        ...eventInterfaces.map(event => {
          const paramDocs = event.parameters.map(param => 
            `     * @param ${param.name} ${param.originalType} - ${param.type}`
          ).join('\n');
          const hasParams = event.parameters.length > 0;
          const paramSignature = event.parameters.map(p => `${p.name}: ${p.type}`).join(', ');
          
          return `    /**\n     * Event handler for ${event.name}\n${hasParams ? paramDocs + '\n' : ''}     */\n    "${event.name}": (${paramSignature}${event.hasVariadic ? ', ...args: any[]' : ''}) => void;`;
        }),
        '  }',
        '}'
      );
    },

    makeStructures(structureInterfaces) {
      const interfaceDefinitions = structureInterfaces.map(iface => {
        const comment = iface.description ? `  /** ${iface.description} */\n` : '';
        const propertyLines = iface.properties.map(prop => {
          if (typeof prop === 'string') {
            return `    ${prop};`;
          } else {
            const propComment = prop.comment ? `    /** ${prop.comment} */\n` : '';
            return `${propComment}    ${prop.name}: ${prop.type};`;
          }
        }).join('\n\n');
        return `${comment}  interface ${iface.name} {\n${propertyLines}\n  }`;
      }).join('\n');

      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  interface FileHandle {',
        '    // Opaque file handle - use with engine file functions',
        '  }',
        '',
        interfaceDefinitions,
        '}'
      );
    },

    makeEngine(computed) {
      return fileMaker.makeFile(
        '/// <reference path="./structures.d.ts" />',
        '/// <reference path="./enums.d.ts" />',
        '',
        'declare namespace nodemod {',
        `  interface Engine {`,
        computed.eng.map(v => `    /** ${v.api.original} */\n    ${v.api.typing};`).join('\n'),
        `  }`,
        `  const eng: Engine;`,
        '}'
      );
    },

    makeCore() {
      return fileMaker.makeFile(
        '/// <reference path="./enums.d.ts" />',
        '/// <reference path="./events.d.ts" />',
        '/// <reference path="./structures.d.ts" />',
        '/// <reference path="./engine.d.ts" />',
        '',
        'declare namespace nodemod {',
        `  // Properties`,
        `  const cwd: string;`,
        `  const players: Entity[];`,
        `  const mapname: string;`,
        `  const time: number;`,
        ``,
        `  // Event system functions`,
        `  function on<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function addEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function addListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function removeListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function removeEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function clearListeners(eventName?: keyof EventCallbacks): void;`,
        `  function fire<T extends keyof EventCallbacks>(eventName: T, ...args: Parameters<EventCallbacks[T]>): void;`,
        ``,
        `  // Utility functions`,
        `  function getUserMsgId(msgName: string): number;`,
        `  function getUserMsgName(msgId: number): string;`,
        `  function setMetaResult(result: number): void;`,
        `  function continueServer(): void;`,
        `}`
      );
    },

    makeSingleIndex(computed, structureInterfaces = [], eventNames = [], eventInterfaces = []) {
      const interfaceDefinitions = structureInterfaces.map(iface => {
        const comment = iface.description ? `  /** ${iface.description} */\n` : '';
        const propertyLines = iface.properties.map(prop => {
          if (typeof prop === 'string') {
            // Simple string property
            return `    ${prop};`;
          } else {
            // Object with name, type, and comment
            const propComment = prop.comment ? `    /** ${prop.comment} */\n` : '';
            return `${propComment}    ${prop.name}: ${prop.type};`;
          }
        }).join('\n\n');
        return `${comment}  interface ${iface.name} {\n${propertyLines}\n  }`;
      }).join('\n');
      
      return fileMaker.makeFile(
        'declare namespace nodemod {',
        '  interface FileHandle {',
        '    // Opaque file handle - use with engine file functions',
        '  }',
        '',
        '  // Metamod result constants',
        '  enum META_RES {',
        '    UNSET = 0,    // Uninitialized (causes error)',
        '    IGNORED = 1,  // Plugin didn\'t take any action, continue normally',
        '    HANDLED = 2,  // Plugin did something, but original function still executes',  
        '    OVERRIDE = 3, // Execute original function, but use plugin\'s return value',
        '    SUPERCEDE = 4 // Skip original function entirely, use plugin\'s behavior',
        '  }',
        '',
        '  // Alert types for engine functions',
        '  enum ALERT_TYPE {',
        '    at_notice = 0,',
        '    at_console = 1,',
        '    at_aiconsole = 2,',
        '    at_warning = 3,',
        '    at_error = 4,',
        '    at_logged = 5',
        '  }',
        '',
        '  // Print types for client output',
        '  enum PRINT_TYPE {',
        '    print_console = 0,',
        '    print_center = 1,',
        '    print_chat = 2',
        '  }',
        '',
        '  // Force types for consistency checking',
        '  enum FORCE_TYPE {',
        '    force_exactfile = 0,',
        '    force_model_samebounds = 1,',
        '    force_model_specifybounds = 2,',
        '    force_model_specifybounds_if_avail = 3',
        '  }',
        '',
        '  // Event name types',
        `  type EventNames = ${eventNames.length > 0 ? eventNames.map(name => `"${name}"`).join('\n    | ') : 'string'};`,
        '',
        '  // Event handler interfaces',
        ...eventInterfaces.map(event => {
          const paramDocs = event.parameters.map(param => 
            `   * @param ${param.name} ${param.originalType} - ${param.type}`
          ).join('\n');
          const hasParams = event.parameters.length > 0;
          
          return `  /**\n   * Event handler for ${event.name}\n${hasParams ? paramDocs + '\n' : ''}   */\n  interface ${event.name}Handler {\n    (${event.parameters.map(p => `${p.name}: ${p.type}`).join(', ')}${event.hasVariadic ? ', ...args: any[]' : ''}): void;\n    readonly args: [${event.parameters.map(p => p.type).join(', ')}];\n    readonly returnType: void;\n  }`;
        }),
        '',
        '  // Event callbacks map',
        '  interface EventCallbacks {',
        ...eventInterfaces.map(event => `    "${event.name}": ${event.name}Handler;`),
        '  }',
        '',
        interfaceDefinitions,
        `  interface Engine {`,
        computed.eng.map(v => `    /** ${v.api.original} */\n    ${v.api.typing};`).join('\n'),
        `  }`,
        `  const eng: Engine;`,
        ``,
        `  // Properties`,
        `  const cwd: string;`,
        `  const players: Entity[];`,
        `  const mapname: string;`,
        `  const time: number;`,
        ``,
        `  // Event system functions`,
        `  function on<T extends EventNames>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function addEventListener<T extends EventNames>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function addListener<T extends EventNames>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function removeListener<T extends EventNames>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function removeEventListener<T extends EventNames>(eventName: T, callback: EventCallbacks[T]): void;`,
        `  function clearListeners(eventName?: EventNames): void;`,
        `  function fire<T extends EventNames>(eventName: T, ...args: Parameters<EventCallbacks[T]>): void;`,
        ``,
        `  // Utility functions`,
        `  function getUserMsgId(msgName: string): number;`,
        `  function getUserMsgName(msgId: number): string;`,
        `  function setMetaResult(result: number): void;`,
        `  function continueServer(): void;`,
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