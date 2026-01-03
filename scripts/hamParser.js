// Parser for Ham (Hamsandwich) C++ files to generate TypeScript types
import { promises as fs } from 'fs';
import { paramTypeMap, returnTypeMap, getParamName } from './hamParamNames.js';


// Parse HamType enum from ham_const.h
function parseHamTypeEnum(content) {
  const enumMatch = content.match(/enum HamType\s*\{([\s\S]*?)\};/);
  if (!enumMatch) return [];

  const enumBody = enumMatch[1];
  const entries = [];

  // Match entries like: Ham_Spawn = 0, or Ham_Precache, (with optional comments)
  const lines = enumBody.split('\n');
  let currentValue = 0;

  for (const line of lines) {
    const trimmed = line.trim();
    if (!trimmed || trimmed.startsWith('//')) continue;

    // Match: Ham_FuncName = value, or Ham_FuncName,
    const match = trimmed.match(/^(Ham_\w+)\s*(?:=\s*(\d+))?\s*,?/);
    if (match) {
      const name = match[1];
      if (name === 'Ham_EndMarker') continue; // Skip end marker

      if (match[2]) {
        currentValue = parseInt(match[2], 10);
      }

      // Convert Ham_FuncName to FuncName (remove Ham_ prefix)
      const tsName = name.replace(/^Ham_/, '');
      entries.push({ name: tsName, value: currentValue });
      currentValue++;
    }
  }

  return entries;
}

// Parse HamResult enum from ham_const.h
function parseHamResultEnum(content) {
  const enumMatch = content.match(/enum HamResult\s*\{([\s\S]*?)\};/);
  if (!enumMatch) return [];

  const enumBody = enumMatch[1];
  const entries = [];

  const lines = enumBody.split('\n');
  let currentValue = 0;

  for (const line of lines) {
    const trimmed = line.trim();
    if (!trimmed || trimmed.startsWith('//')) continue;

    const match = trimmed.match(/^(HAM_\w+)\s*(?:=\s*(\d+))?\s*,?/);
    if (match) {
      const name = match[1];
      if (match[2]) {
        currentValue = parseInt(match[2], 10);
      }

      // Convert HAM_UNSET to UNSET (remove HAM_ prefix)
      const tsName = name.replace(/^HAM_/, '');
      entries.push({ name: tsName, value: currentValue });
      currentValue++;
    }
  }

  return entries;
}

// Parse g_hamFunctions array from ham_manager.cpp
function parseHamFunctions(content) {
  // Find the g_hamFunctions array
  const arrayMatch = content.match(/HamFunctionInfo g_hamFunctions\[\]\s*=\s*\{([\s\S]*?)\n\};/);
  if (!arrayMatch) return [];

  const arrayBody = arrayMatch[1];
  const functions = [];

  // Match entries like: {"spawn", HAM_RET_VOID, 0, {}},
  // or: {"takedamage", HAM_RET_INT, 4, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_INT}},
  const entryRegex = /\{\s*"(\w+)"\s*,\s*(HAM_RET_\w+)\s*,\s*(\d+)\s*,\s*\{([^}]*)\}\s*\}/g;

  let match;
  let index = 0;
  while ((match = entryRegex.exec(arrayBody)) !== null) {
    const [, name, returnType, paramCount, paramsStr] = match;

    // Parse parameters
    const params = [];
    if (paramsStr.trim()) {
      const paramMatches = paramsStr.match(/HAM_PARAM_\w+/g);
      if (paramMatches) {
        params.push(...paramMatches);
      }
    }

    functions.push({
      index,
      name,
      returnType,
      paramCount: parseInt(paramCount, 10),
      params
    });
    index++;
  }

  return functions;
}

// Generate TypeScript HAM_FUNC const enum
function generateHamFuncEnum(hamTypes) {
  const lines = [
    '  /** Ham function IDs for virtual function hooking */',
    '  const enum HAM_FUNC {'
  ];

  for (const entry of hamTypes) {
    lines.push(`    ${entry.name} = ${entry.value},`);
  }

  lines.push('  }');
  return lines.join('\n');
}

// Generate TypeScript HAM_RESULT const enum
function generateHamResultEnum(hamResults) {
  const lines = [
    '  /** Ham (Hamsandwich) hook result values */',
    '  const enum HAM_RESULT {'
  ];

  const descriptions = {
    'UNSET': 'Default state',
    'IGNORED': "Hook had no effect, continue normally",
    'HANDLED': "Hook processed the call, but still call original",
    'OVERRIDE': "Use hook's return value instead of original",
    'SUPERCEDE': "Don't call original function at all"
  };

  for (const entry of hamResults) {
    const desc = descriptions[entry.name] || '';
    const comment = desc ? ` // ${desc}` : '';
    lines.push(`    ${entry.name} = ${entry.value},${comment}`);
  }

  lines.push('  }');
  return lines.join('\n');
}

// Generate HamCallbackFor<T> conditional type
function generateHamCallbackType(hamTypes, hamFunctions) {
  const lines = [
    '  // Ham callback type mappings - maps HAM_FUNC to callback signature',
    '  // All callbacks receive \'this_\' (the hooked entity) as the first parameter',
    '  type HamCallbackFor<T extends HAM_FUNC> ='
  ];

  // Create a map of function index to function info
  const funcMap = new Map();
  for (const func of hamFunctions) {
    funcMap.set(func.index, func);
  }

  for (const entry of hamTypes) {
    const func = funcMap.get(entry.value);
    if (!func) {
      // Function not in g_hamFunctions, use default
      lines.push(`    T extends HAM_FUNC.${entry.name} ? (this_: Entity) => HAM_RESULT | void :`);
      continue;
    }

    // Build parameter list with meaningful names
    const paramList = ['this_: Entity'];
    const usedNames = new Set(['this_']);

    for (let i = 0; i < func.params.length; i++) {
      const paramTypeInfo = paramTypeMap[func.params[i]];
      const paramType = paramTypeInfo?.type || 'any';

      // Get param name - try special names first, then fall back to type-based name
      let paramName = getParamName(func.name, func.params[i], i, func.params);

      // Handle duplicate names by appending number
      let baseName = paramName;
      let counter = 1;
      while (usedNames.has(paramName)) {
        counter++;
        paramName = `${baseName}${counter}`;
      }
      usedNames.add(paramName);

      paramList.push(`${paramName}: ${paramType}`);
    }

    // Determine return type
    const tsReturnType = returnTypeMap[func.returnType] || 'void';
    const returnUnion = tsReturnType === 'void' ? 'HAM_RESULT | void' : `HAM_RESULT | ${tsReturnType}`;

    lines.push(`    T extends HAM_FUNC.${entry.name} ? (${paramList.join(', ')}) => ${returnUnion} :`);
  }

  // Default fallback
  lines.push('    (this_: Entity, ...args: any[]) => HAM_RESULT | void;');

  return lines.join('\n');
}

// Main export function
export async function parseHamFiles() {
  const hamConstPath = './src/ham/ham_const.h';
  const hamManagerPath = './src/ham/ham_manager.cpp';

  const [hamConstContent, hamManagerContent] = await Promise.all([
    fs.readFile(hamConstPath, 'utf-8'),
    fs.readFile(hamManagerPath, 'utf-8')
  ]);

  const hamTypes = parseHamTypeEnum(hamConstContent);
  const hamResults = parseHamResultEnum(hamConstContent);
  const hamFunctions = parseHamFunctions(hamManagerContent);

  return {
    hamTypes,
    hamResults,
    hamFunctions,
    generateHamFuncEnum: () => generateHamFuncEnum(hamTypes),
    generateHamResultEnum: () => generateHamResultEnum(hamResults),
    generateHamCallbackType: () => generateHamCallbackType(hamTypes, hamFunctions)
  };
}

export default { parseHamFiles };
