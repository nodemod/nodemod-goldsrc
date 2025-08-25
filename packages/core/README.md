# ⚠️ DEPRECATED: @nodemod/core

This package is **deprecated** and no longer compatible with the current NodeMod plugin system.

## Migration Required

**Use `@nodemod/core-plugin` instead** for the new multi-plugin architecture.

### Replacement Package
✅ **New package:** `packages/core-plugin/` or `@nodemod/core-plugin`

### Key Differences

| Old @nodemod/core | New @nodemod/core-plugin |
|------------------|--------------------------|
| ES modules | CommonJS modules |
| Global nodemod context | Per-plugin isolated context |
| Single script system | Multi-plugin system |
| `import nodemodCore from '@nodemod/core'` | `const nodemodCore = require('@nodemod/core-plugin')` |

## Migration Steps

1. **Update package.json dependencies:**
   ```json
   // Before
   "@nodemod/core": "file:../packages/core"
   
   // After  
   "@nodemod/core-plugin": "file:../../packages/core-plugin"
   ```

2. **Update import style:**
   ```javascript
   // Before
   import nodemodCore from '@nodemod/core';
   
   // After
   const nodemodCore = require('@nodemod/core-plugin');
   ```

3. **Move to plugin architecture:**
   - See `PLUGIN_MIGRATION.md` in project root
   - Use `plugins/example-server/` as reference

## Why Deprecated?

The original `@nodemod/core` was designed for a single-script execution model where one main JavaScript file controlled everything. The new plugin system supports multiple isolated plugins running simultaneously, requiring a different architecture.

## Don't Use This Package

❌ This package will not work with the current NodeMod system
✅ Use `@nodemod/core-plugin` in the new plugin system instead