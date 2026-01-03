// Generator for Ham hook callback functions with proper parameter passing
import { promises as fs } from 'fs';
import { hookTypeMap, getParamName } from './hamParamNames.js';

// Type encoding -> C++ type and V8 conversion (extends hookTypeMap with v8Convert)
const typeInfo = {
  'Void': { cppType: null, isReturn: true },
  'Int': { ...hookTypeMap['Int'], v8Convert: (name) => `v8::Integer::New(isolate, ${name})` },
  'Float': { ...hookTypeMap['Float'], v8Convert: (name) => `v8::Number::New(isolate, ${name})` },
  'Bool': { ...hookTypeMap['Bool'], v8Convert: (name) => `v8::Boolean::New(isolate, ${name})` },
  'Short': { ...hookTypeMap['Short'], v8Convert: (name) => `v8::Integer::New(isolate, ${name})` },
  'Str': { ...hookTypeMap['Str'], v8Convert: (name) => `${name} ? v8::String::NewFromUtf8(isolate, ${name}).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()` },
  'Cbase': { ...hookTypeMap['Cbase'], v8Convert: (name) => `getEdictFromThis(${name}) ? structures::wrapEntity(isolate, getEdictFromThis(${name})) : v8::Null(isolate).As<v8::Value>()` },
  'Entvar': { ...hookTypeMap['Entvar'], v8Convert: (name) => `${name} ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(${name})) : v8::Null(isolate).As<v8::Value>()` },
  'Edict': { ...hookTypeMap['Edict'], v8Convert: (name) => `${name} ? structures::wrapEntity(isolate, static_cast<edict_t*>(${name})) : v8::Null(isolate).As<v8::Value>()` },
  'pVector': { ...hookTypeMap['pVector'], v8Convert: (name) => `utils::vect2js(isolate, ${name})` },
  'Vector': { ...hookTypeMap['Vector'], isReturn: true },
  'pFloat': { ...hookTypeMap['pFloat'], v8Convert: (name) => `${name} ? v8::Number::New(isolate, *${name}) : v8::Null(isolate).As<v8::Value>()` },
  'Trace': { ...hookTypeMap['Trace'], v8Convert: (name) => `${name} ? structures::wrapTraceResult(isolate, static_cast<TraceResult*>(${name})) : v8::Null(isolate).As<v8::Value>()` },
  'ItemInfo': { ...hookTypeMap['ItemInfo'], v8Convert: (name) => `v8::Null(isolate).As<v8::Value>()` }, // TODO: implement ItemInfo wrapping
};

// Parse function signature from name like "Hook_Void_Cbase_Cbase_Int_Float"
function parseHookSignature(name) {
  const parts = name.replace('Hook_', '').split('_');
  const returnType = parts[0];
  const paramTypes = parts.slice(1);

  return { returnType, paramTypes };
}

// Generate parameter names using shared module
function generateParamNames(funcName, paramTypes) {
  const usedNames = new Set();
  return paramTypes.map((type, idx) => {
    let name = getParamName(funcName, type, idx, paramTypes);

    // Handle duplicates
    let baseName = name;
    let counter = 1;
    while (usedNames.has(name)) {
      counter++;
      name = `${baseName}${counter}`;
    }
    usedNames.add(name);
    return name;
  });
}

// Generate a single hook function body
function generateHookFunction(funcName, sig, paramNames, isVectorReturn = false) {
  const { returnType, paramTypes } = sig;
  const info = typeInfo[returnType];

  const hasParams = paramTypes.length > 0 && paramTypes[0] !== 'Void';
  const paramCount = hasParams ? paramTypes.length : 0;

  // Build parameter list for function signature
  let funcParams = 'Hook* hook, void* pthis';
  if (isVectorReturn) {
    funcParams = 'Hook* hook, float* out, void* pthis';
  }

  for (let i = 0; i < paramCount; i++) {
    const pType = typeInfo[paramTypes[i]];
    if (pType && pType.cppType) {
      funcParams += `, ${pType.cppType} ${paramNames[i]}`;
    }
  }

  // Build function body
  const lines = [];

  // PUSH macro
  if (returnType === 'Int') {
    lines.push('    PUSH_INT()');
  } else if (returnType === 'Float') {
    lines.push('    PUSH_FLOAT()');
  } else if (returnType === 'Vector') {
    lines.push('    PUSH_VECTOR()');
  } else if (returnType === 'Cbase') {
    lines.push('    PUSH_CBASE()');
  } else {
    lines.push('    PUSH_VOID()');
  }
  lines.push('');

  // Pre-callbacks with parameter passing
  if (paramCount > 0) {
    lines.push('    // Execute pre-callbacks with parameters');
    lines.push('    {');
    lines.push('        v8::Isolate* isolate = mgr.getIsolate();');
    lines.push('        if (isolate) {');
    lines.push('            v8::Locker locker(isolate);');
    lines.push('            v8::Isolate::Scope isolateScope(isolate);');
    lines.push('            v8::HandleScope handleScope(isolate);');
    lines.push('');
    lines.push(`            v8::Local<v8::Value> extraArgs[${paramCount}] = {`);

    for (let i = 0; i < paramCount; i++) {
      const pType = paramTypes[i];
      const pName = paramNames[i];
      const info = typeInfo[pType];
      const v8Code = info?.v8Convert ? info.v8Convert(pName) : `v8::Null(isolate).As<v8::Value>()`;
      const comma = i < paramCount - 1 ? ',' : '';
      lines.push(`                ${v8Code}${comma}`);
    }

    lines.push('            };');
    lines.push('            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);');
    lines.push('        }');
    lines.push('    }');
  } else {
    lines.push('    executeCallbacks(hook, pthis, true);');
  }
  lines.push('');

  // Call original function
  lines.push('    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {');

  // Build original function typedef
  let origParams = 'void*';
  for (let i = 0; i < paramCount; i++) {
    const pType = typeInfo[paramTypes[i]];
    if (pType && pType.cppType) {
      origParams += `, ${pType.cppType}`;
    }
  }

  let origReturnType = 'void';
  if (returnType === 'Int') origReturnType = 'int';
  else if (returnType === 'Float') origReturnType = 'float';
  else if (returnType === 'Vector') origReturnType = 'void';
  else if (returnType === 'Cbase') origReturnType = 'void*';

  lines.push(`        typedef ${origReturnType} (*OrigFunc)(${origParams});`);
  lines.push('        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());');

  // Build call
  let callArgs = 'pthis';
  for (let i = 0; i < paramCount; i++) {
    callArgs += `, ${paramNames[i]}`;
  }

  if (returnType === 'Void' || returnType === 'Vector') {
    lines.push(`        orig(${callArgs});`);
  } else {
    lines.push(`        origret = orig(${callArgs});`);
  }

  lines.push('    }');
  lines.push('');

  // Post-callbacks with parameter passing
  if (paramCount > 0) {
    lines.push('    // Execute post-callbacks with parameters');
    lines.push('    {');
    lines.push('        v8::Isolate* isolate = mgr.getIsolate();');
    lines.push('        if (isolate) {');
    lines.push('            v8::Locker locker(isolate);');
    lines.push('            v8::Isolate::Scope isolateScope(isolate);');
    lines.push('            v8::HandleScope handleScope(isolate);');
    lines.push('');
    lines.push(`            v8::Local<v8::Value> extraArgs[${paramCount}] = {`);

    for (let i = 0; i < paramCount; i++) {
      const pType = paramTypes[i];
      const pName = paramNames[i];
      const info = typeInfo[pType];
      const v8Code = info?.v8Convert ? info.v8Convert(pName) : `v8::Null(isolate).As<v8::Value>()`;
      const comma = i < paramCount - 1 ? ',' : '';
      lines.push(`                ${v8Code}${comma}`);
    }

    lines.push('            };');
    lines.push('            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);');
    lines.push('        }');
    lines.push('    }');
  } else {
    lines.push('    executeCallbacks(hook, pthis, false);');
  }

  // POP and return
  lines.push('    POP()');

  if (returnType === 'Int') {
    lines.push('    CHECK_RETURN_INT()');
  } else if (returnType === 'Float') {
    lines.push('    CHECK_RETURN_FLOAT()');
  } else if (returnType === 'Cbase') {
    lines.push('    CHECK_RETURN_CBASE()');
  }

  return lines;
}

// Parse existing hook_callbacks.cpp to extract function signatures
async function parseExistingCallbacks(filePath) {
  const content = await fs.readFile(filePath, 'utf-8');

  // Find all Hook_* function definitions
  const funcRegex = /^(void|int)\s+(Hook_\w+)\(Hook\*\s+hook,\s*([^)]+)\)/gm;
  const functions = [];

  let match;
  while ((match = funcRegex.exec(content)) !== null) {
    const [, retType, funcName, paramsStr] = match;

    // Skip deprecated/removed
    if (funcName === 'Hook_Deprecated' || funcName === 'Hook_Removed') continue;

    // Parse the signature from function name
    const sig = parseHookSignature(funcName);

    // Generate parameter names using shared module (instead of parsing from file)
    const paramTypes = sig.paramTypes.filter(t => t !== 'Void');
    const generatedParamNames = generateParamNames(funcName, paramTypes);

    functions.push({
      funcName,
      returnType: retType,
      sig,
      paramNames: generatedParamNames,
      isVectorReturn: funcName.startsWith('Hook_Vector_')
    });
  }

  return functions;
}

// Generate the updated hook functions
async function generateUpdatedCallbacks() {
  const callbacksPath = './src/ham/hook_callbacks.cpp';
  const functions = await parseExistingCallbacks(callbacksPath);

  console.log(`Found ${functions.length} hook functions to process`);

  // Read the existing file
  let content = await fs.readFile(callbacksPath, 'utf-8');

  // For each function, generate the updated body and replace
  for (const func of functions) {
    const { funcName, sig, paramNames } = func;

    // Skip functions that don't have parameters beyond pthis
    if (paramNames.length === 0) {
      console.log(`  ${funcName}: no extra params, skipping`);
      continue;
    }

    // Skip Vector return types for now (they have special handling)
    if (sig.returnType === 'Vector') {
      console.log(`  ${funcName}: Vector return, skipping (needs special handling)`);
      continue;
    }

    console.log(`  ${funcName}: ${paramNames.length} params [${paramNames.join(', ')}]`);

    // Generate the new function body
    const bodyLines = generateHookFunction(funcName, sig, paramNames, func.isVectorReturn);

    // We'll output what the function should look like
    // Actual replacement would require more careful parsing
  }

  return functions;
}

// Generate complete function replacement text
function generateCompleteFunctionBody(funcName, sig, paramNames, isVectorReturn) {
  const { returnType, paramTypes } = sig;
  const hasParams = paramNames.length > 0;
  const paramCount = hasParams ? paramNames.length : 0;

  // Build parameter list for function signature
  let funcParams = 'Hook* hook, void* pthis';
  if (isVectorReturn) {
    funcParams = 'Hook* hook, float* out, void* pthis';
  }

  // Match param types with param names using the signature
  for (let i = 0; i < paramCount; i++) {
    const pType = typeInfo[paramTypes[i]];
    if (pType && pType.cppType) {
      funcParams += `, ${pType.cppType} ${paramNames[i]}`;
    }
  }

  const lines = [];

  // Function start
  let retTypeStr = returnType === 'Int' ? 'int' : (returnType === 'Cbase' ? 'void*' : 'void');
  lines.push(`${retTypeStr} ${funcName}(${funcParams}) {`);

  // PUSH macro
  if (returnType === 'Int') {
    lines.push('    PUSH_INT()');
  } else if (returnType === 'Float') {
    lines.push('    PUSH_FLOAT()');
  } else if (returnType === 'Vector') {
    lines.push('    PUSH_VECTOR()');
  } else if (returnType === 'Cbase') {
    lines.push('    PUSH_CBASE()');
  } else {
    lines.push('    PUSH_VOID()');
  }
  lines.push('');

  // Pre-callbacks with parameter passing
  if (paramCount > 0) {
    lines.push('    // Execute pre-callbacks with parameters');
    lines.push('    {');
    lines.push('        v8::Isolate* isolate = mgr.getIsolate();');
    lines.push('        if (isolate) {');
    lines.push('            v8::Locker locker(isolate);');
    lines.push('            v8::Isolate::Scope isolateScope(isolate);');
    lines.push('            v8::HandleScope handleScope(isolate);');
    lines.push('');
    lines.push(`            v8::Local<v8::Value> extraArgs[${paramCount}] = {`);

    for (let i = 0; i < paramCount; i++) {
      const pType = paramTypes[i];
      const pName = paramNames[i];
      const info = typeInfo[pType];
      const v8Code = info?.v8Convert ? info.v8Convert(pName) : `v8::Null(isolate).As<v8::Value>()`;
      const comma = i < paramCount - 1 ? ',' : '';
      lines.push(`                ${v8Code}${comma}`);
    }

    lines.push('            };');
    lines.push('            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);');
    lines.push('        }');
    lines.push('    }');
  } else {
    lines.push('    executeCallbacks(hook, pthis, true);');
  }
  lines.push('');

  // Call original function
  lines.push('    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {');

  // Build original function typedef
  let origParams = 'void*';
  for (let i = 0; i < paramCount; i++) {
    const pType = typeInfo[paramTypes[i]];
    if (pType && pType.cppType) {
      origParams += `, ${pType.cppType}`;
    }
  }

  let origReturnType = 'void';
  if (returnType === 'Int') origReturnType = 'int';
  else if (returnType === 'Float') origReturnType = 'float';
  else if (returnType === 'Cbase') origReturnType = 'void*';

  lines.push(`        typedef ${origReturnType} (*OrigFunc)(${origParams});`);
  lines.push('        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());');

  // Build call arguments
  let callArgs = 'pthis';
  for (let i = 0; i < paramCount; i++) {
    callArgs += `, ${paramNames[i]}`;
  }

  if (returnType === 'Void' || returnType === 'Vector') {
    lines.push(`        orig(${callArgs});`);
  } else {
    lines.push(`        origret = orig(${callArgs});`);
  }

  lines.push('    }');
  lines.push('');

  // Post-callbacks with parameter passing
  if (paramCount > 0) {
    lines.push('    // Execute post-callbacks with parameters');
    lines.push('    {');
    lines.push('        v8::Isolate* isolate = mgr.getIsolate();');
    lines.push('        if (isolate) {');
    lines.push('            v8::Locker locker(isolate);');
    lines.push('            v8::Isolate::Scope isolateScope(isolate);');
    lines.push('            v8::HandleScope handleScope(isolate);');
    lines.push('');
    lines.push(`            v8::Local<v8::Value> extraArgs[${paramCount}] = {`);

    for (let i = 0; i < paramCount; i++) {
      const pType = paramTypes[i];
      const pName = paramNames[i];
      const info = typeInfo[pType];
      const v8Code = info?.v8Convert ? info.v8Convert(pName) : `v8::Null(isolate).As<v8::Value>()`;
      const comma = i < paramCount - 1 ? ',' : '';
      lines.push(`                ${v8Code}${comma}`);
    }

    lines.push('            };');
    lines.push('            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);');
    lines.push('        }');
    lines.push('    }');
  } else {
    lines.push('    executeCallbacks(hook, pthis, false);');
  }

  // POP and return
  lines.push('    POP()');

  if (returnType === 'Int') {
    lines.push('    CHECK_RETURN_INT()');
  } else if (returnType === 'Float') {
    lines.push('    CHECK_RETURN_FLOAT()');
  } else if (returnType === 'Cbase') {
    lines.push('    CHECK_RETURN_CBASE()');
  }

  lines.push('}');

  return lines.join('\n');
}

// Find a function's body in the content (from function declaration to closing brace)
function findFunctionBody(content, funcName) {
  // Find the function definition start
  const funcDefRegex = new RegExp(`^(void|int)\\s+${funcName}\\s*\\([^)]+\\)\\s*\\{`, 'm');
  const match = content.match(funcDefRegex);

  if (!match) return null;

  const startIdx = match.index;

  // Find the matching closing brace by counting braces
  let braceCount = 0;
  let inFunction = false;
  let endIdx = startIdx;

  for (let i = startIdx; i < content.length; i++) {
    if (content[i] === '{') {
      braceCount++;
      inFunction = true;
    } else if (content[i] === '}') {
      braceCount--;
      if (inFunction && braceCount === 0) {
        endIdx = i + 1;
        break;
      }
    }
  }

  return {
    start: startIdx,
    end: endIdx,
    text: content.substring(startIdx, endIdx)
  };
}

// Apply changes to the file
async function applyChanges(functions) {
  const callbacksPath = './src/ham/hook_callbacks.cpp';
  let content = await fs.readFile(callbacksPath, 'utf-8');

  let replacements = 0;

  // Sort functions by their position in reverse order (so we can replace from end to start)
  const funcPositions = [];

  for (const func of functions) {
    const { funcName, sig, paramNames } = func;

    // Skip functions without params or with Vector return
    if (paramNames.length === 0 || sig.returnType === 'Vector') continue;

    // Special skip for Hook_Void_Float_Int which we already updated
    // (Actually let's regenerate it too for consistency)

    const found = findFunctionBody(content, funcName);
    if (!found) {
      console.log(`  Warning: Could not find ${funcName} in file`);
      continue;
    }

    funcPositions.push({
      ...func,
      ...found
    });
  }

  // Sort by start position descending
  funcPositions.sort((a, b) => b.start - a.start);

  // Apply replacements from end to start
  for (const func of funcPositions) {
    const newBody = generateCompleteFunctionBody(func.funcName, func.sig, func.paramNames, func.isVectorReturn);

    content = content.substring(0, func.start) + newBody + content.substring(func.end);
    replacements++;
  }

  // Write the updated file
  await fs.writeFile(callbacksPath, content);

  console.log(`\nApplied ${replacements} function replacements`);
  return replacements;
}

// Main export
export async function generateHookCallbacks(apply = false) {
  const functions = await parseExistingCallbacks('./src/ham/hook_callbacks.cpp');

  if (apply) {
    return await applyChanges(functions);
  }

  return functions;
}

// For direct execution
if (process.argv[1].endsWith('hookCallbacksGenerator.js')) {
  const apply = process.argv.includes('--apply');
  generateHookCallbacks(apply).catch(console.error);
}

export default { generateHookCallbacks };
