// Admin System Entry Point
// Loads the admin system and plugins based on plugins.ini configuration.
//
// Plugin Loading:
//   Plugins are loaded based on configs/plugins.ini. The plugin loader
//   dynamically imports each plugin and instantiates it with the plugin name.
//
// Load Order:
//   1. Admin plugin (core system, registers base CVARs)
//   2. Other plugins from plugins.ini (register their CVARs)
//   3. Execute amxx.cfg and sql.cfg (sets CVAR values)
//
// Usage:
//   import adminSystem from './admin'
//   import { ADMIN_KICK, CMDTARGET_OBEY_IMMUNITY } from './constants'
//   import { storage, StorageAdapter } from './storage'
//   import localization from './localization'
//   import * as utils from './utils'
//   import { pluginLoader } from './pluginloader'

import { pluginLoader } from './pluginloader';
import { adminSystem } from './admin';

// Export plugin loader for other code to query loaded plugins
export { pluginLoader, Plugin, PluginMetadata } from './pluginloader';

// Load admin system first (other plugins depend on it)
pluginLoader.loadPluginSync('admin');

// Load remaining plugins from plugins.ini
pluginLoader.loadFromConfig();

// Execute config files AFTER all plugins are loaded
// This ensures all CVARs are registered before amxx.cfg sets their values
adminSystem.executeConfigFiles();
