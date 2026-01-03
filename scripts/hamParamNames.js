// Shared parameter name definitions for Ham hook generators
// Used by both hamParser.js (TypeScript types) and hookCallbacksGenerator.js (C++ callbacks)

// Map C++ param types to TypeScript types and default param names
export const paramTypeMap = {
  'HAM_PARAM_NONE': { type: null, cppType: null, name: null },
  'HAM_PARAM_INT': { type: 'number', cppType: 'int', name: 'value' },
  'HAM_PARAM_FLOAT': { type: 'number', cppType: 'float', name: 'value' },
  'HAM_PARAM_VECTOR': { type: 'number[]', cppType: 'float*', name: 'vec' },
  'HAM_PARAM_ENTITY': { type: 'Entity', cppType: 'void*', name: 'other' },
  'HAM_PARAM_ENTVAR': { type: 'Entvars', cppType: 'void*', name: 'other' },
  'HAM_PARAM_STRING': { type: 'string', cppType: 'const char*', name: 'str' },
  'HAM_PARAM_TRACE': { type: 'TraceResult', cppType: 'void*', name: 'trace' },
  'HAM_PARAM_EDICT': { type: 'Entity', cppType: 'void*', name: 'other' },
  'HAM_PARAM_ITEMINFO': { type: 'ItemInfo', cppType: 'void*', name: 'itemInfo' }
};

// Map C++ return types to TypeScript types
export const returnTypeMap = {
  'HAM_RET_VOID': 'void',
  'HAM_RET_INT': 'number',
  'HAM_RET_FLOAT': 'number',
  'HAM_RET_VECTOR': 'number[]',
  'HAM_RET_ENTITY': 'Entity',
  'HAM_RET_STRING': 'string'
};

// Special parameter names for specific functions (indexed by function name, lowercase)
// These override the default type-based names
// Includes both Ham function names (use, takedamage) and Hook function names (hook_void_cbase_cbase_int_float)
export const specialParamNames = {
  // Ham_Use - Void_Cbase_Cbase_Int_Float
  'use': ['activator', 'caller', 'useType', 'value'],
  'hook_void_cbase_cbase_int_float': ['activator', 'caller', 'useType', 'value'],

  // Ham_TakeDamage - Int_Entvar_Entvar_Float_Int
  'takedamage': ['inflictor', 'attacker', 'damage', 'damageBits'],
  'hook_int_entvar_entvar_float_int': ['inflictor', 'attacker', 'damage', 'damageBits'],

  // Ham_TraceAttack - Void_Entvar_Float_Vector_Trace_Int
  'traceattack': ['attacker', 'damage', 'direction', 'trace', 'damageBits'],
  'hook_void_entvar_float_vector_trace_int': ['attacker', 'damage', 'direction', 'trace', 'damageBits'],

  // Ham_TraceBleed - Void_Float_Vector_Trace_Int
  'tracebleed': ['damage', 'direction', 'trace', 'damageBits'],
  'hook_void_float_vector_trace_int': ['damage', 'direction', 'trace', 'damageBits'],

  // Ham_Killed - Void_Entvar_Int
  'killed': ['attacker', 'gibType'],
  'hook_void_entvar_int': ['attacker', 'gibType'],

  // Ham_GiveAmmo - Int_Int_Str_Int
  'giveammo': ['amount', 'name', 'max'],
  'hook_int_int_str_int': ['amount', 'name', 'max'],

  // Ham_AddPoints - Void_Int_Int
  'addpoints': ['score', 'allowNegative'],
  'addpointstoteam': ['score', 'allowNegative'],
  'hook_void_int_int': ['score', 'allowNegative'],

  // Item functions
  'item_addtoplayer': ['player'],
  'item_addduplicate': ['original'],
  'item_getiteminfo': ['itemInfo'],
  'item_holster': ['skipLocal'],

  // Weapon functions
  'weapon_extractammo': ['weapon'],
  'weapon_extractclipammo': ['weapon'],

  // TS mod functions
  'ts_goslow': ['duration', 'mode'],
  'hook_void_float_int': ['duration', 'mode'],
  'ts_breakablerespawn': ['respawnTime'],
};

// Mapping from Hook_* signature types to parameter info
// Maps type codes (Int, Float, Cbase, etc.) to their properties
export const hookTypeMap = {
  'Int': { cppType: 'int', tsType: 'number', defaultName: 'value' },
  'Float': { cppType: 'float', tsType: 'number', defaultName: 'value' },
  'Bool': { cppType: 'bool', tsType: 'boolean', defaultName: 'flag' },
  'Short': { cppType: 'short', tsType: 'number', defaultName: 'value' },
  'Str': { cppType: 'const char*', tsType: 'string', defaultName: 'str' },
  'Cbase': { cppType: 'void*', tsType: 'Entity', defaultName: 'entity', isEntity: true },
  'Entvar': { cppType: 'void*', tsType: 'Entvars', defaultName: 'entvars', isEntvar: true },
  'Edict': { cppType: 'void*', tsType: 'Entity', defaultName: 'edict', isEdict: true },
  'pVector': { cppType: 'float*', tsType: 'number[]', defaultName: 'vec', isVector: true },
  'Vector': { cppType: 'float*', tsType: 'number[]', defaultName: 'vec', isVector: true, isReturn: true },
  'pFloat': { cppType: 'float*', tsType: 'number', defaultName: 'value' },
  'Trace': { cppType: 'void*', tsType: 'TraceResult', defaultName: 'trace', isTrace: true },
  'ItemInfo': { cppType: 'void*', tsType: 'ItemInfo', defaultName: 'itemInfo' },
};

// Generate descriptive parameter name based on function name, type, and position
export function getParamName(funcName, paramType, index, allParamTypes) {
  const lowerName = funcName.toLowerCase();

  // Check for special function-specific names first
  if (specialParamNames[lowerName] && specialParamNames[lowerName][index] !== undefined) {
    return specialParamNames[lowerName][index];
  }

  // Get type info from either paramTypeMap or hookTypeMap
  let typeInfo = paramTypeMap[paramType] || hookTypeMap[paramType];
  if (!typeInfo || !typeInfo.name && !typeInfo.defaultName) {
    return `arg${index + 1}`;
  }

  const baseName = typeInfo.name || typeInfo.defaultName;

  // Count how many params of this same type exist
  let sameTypeCount = 0;
  let myPosition = 0;
  for (let i = 0; i < allParamTypes.length; i++) {
    if (allParamTypes[i] === paramType) {
      if (i < index) myPosition++;
      sameTypeCount++;
    }
  }

  // If multiple params of same type, append position number
  if (sameTypeCount > 1) {
    return `${baseName}${myPosition + 1}`;
  }

  return baseName;
}

export default {
  paramTypeMap,
  returnTypeMap,
  specialParamNames,
  hookTypeMap,
  getParamName
};
