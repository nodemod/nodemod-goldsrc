// Plugin Loader System
// Loads plugins dynamically based on plugins.ini configuration
// Similar to AMX Mod X plugin loading system

import fs from 'fs';
import path from 'path';

/**
 * Plugin metadata interface
 * Each plugin must export this information
 */
export interface PluginMetadata {
    /** Plugin name (e.g., "Admin Base") */
    name: string;
    /** Plugin version (e.g., "1.0.0") */
    version: string;
    /** Plugin author(s) */
    author: string;
    /** Plugin description */
    description?: string;
}

/**
 * Plugin interface that all plugins must implement
 */
export interface Plugin {
    /** Plugin metadata */
    readonly metadata: PluginMetadata;
    /** Called when plugin is loaded (optional) */
    onLoad?(): void;
    /** Called when plugin is unloaded (optional) */
    onUnload?(): void;
}

/**
 * Plugin class constructor type
 * Plugins export a class that takes pluginName in constructor
 */
export type PluginConstructor = new (pluginName: string) => Plugin;

/**
 * Loaded plugin entry
 */
export interface LoadedPlugin {
    /** The plugin instance */
    plugin: Plugin;
    /** Plugin name from plugins.ini */
    pluginName: string;
    /** Load status */
    status: 'running' | 'error' | 'stopped';
    /** Error message if status is 'error' */
    error?: string;
}

/**
 * Plugin Loader
 * Manages loading and tracking of plugins
 */
class PluginLoader {
    private plugins: Map<string, LoadedPlugin> = new Map();
    private configPath: string;

    constructor() {
        // Default config path - configs are in nodemod/configs/, not plugins/configs/
        this.configPath = path.join(process.cwd(), '..', 'configs', 'plugins.ini');
    }

    /**
     * Set the path to plugins.ini
     */
    setConfigPath(configPath: string): void {
        this.configPath = configPath;
    }

    /**
     * Load a plugin by name
     * @param pluginName Name from plugins.ini (e.g., 'adminchat')
     * @param importPath Optional import path (defaults to `./${pluginName}`)
     */
    async loadPlugin(pluginName: string, importPath?: string): Promise<void> {
        const modulePath = importPath || `./${pluginName}`;

        try {
            // Dynamic import
            const module = await import(modulePath);

            // Get the plugin class (default export)
            const PluginClass: PluginConstructor = module.default;

            if (!PluginClass || typeof PluginClass !== 'function') {
                throw new Error(`Plugin ${pluginName} does not export a class as default`);
            }

            // Instantiate with the plugin name
            const plugin = new PluginClass(pluginName);

            this.plugins.set(pluginName, {
                plugin,
                pluginName,
                status: 'running'
            });

            // Call onLoad if available
            if (plugin.onLoad) {
                try {
                    plugin.onLoad();
                } catch (e) {
                    console.error(`[PluginLoader] Error in onLoad for ${pluginName}:`, e);
                }
            }
        } catch (e) {
            console.error(`[PluginLoader] Failed to load ${pluginName}:`, e);
            this.plugins.set(pluginName, {
                plugin: null as any,
                pluginName,
                status: 'error',
                error: String(e)
            });
        }
    }

    /**
     * Load a plugin synchronously (for require-style loading)
     * @param pluginName Name from plugins.ini (e.g., 'adminchat')
     * @param importPath Optional import path (defaults to `./${pluginName}`)
     */
    loadPluginSync(pluginName: string, importPath?: string): void {
        const modulePath = importPath || `./${pluginName}`;

        try {
            const module = require(modulePath);

            let plugin: Plugin;

            // Check if module exports a pre-created instance (e.g., admin exports adminSystem)
            // This handles singleton services that other plugins depend on
            const instanceName = pluginName.replace(/-/g, '') + 'System';
            const altInstanceName = pluginName.replace(/-/g, '');

            if (module[instanceName] && typeof module[instanceName] === 'object' && module[instanceName].metadata) {
                plugin = module[instanceName];
            } else if (module[altInstanceName] && typeof module[altInstanceName] === 'object' && module[altInstanceName].metadata) {
                plugin = module[altInstanceName];
            } else {
                // Get the plugin class (default export)
                const PluginClass: PluginConstructor = module.default;

                if (!PluginClass || typeof PluginClass !== 'function') {
                    throw new Error(`Plugin ${pluginName} does not export a class as default (got ${typeof PluginClass})`);
                }

                // Instantiate with the plugin name
                plugin = new PluginClass(pluginName);
            }

            this.plugins.set(pluginName, {
                plugin,
                pluginName,
                status: 'running'
            });

            // Call onLoad if available
            if (plugin.onLoad) {
                try {
                    plugin.onLoad();
                } catch (e) {
                    console.error(`[PluginLoader] Error in onLoad for ${pluginName}:`, e);
                }
            }
        } catch (e) {
            console.error(`[PluginLoader] Failed to load ${pluginName}:`, e);
            this.plugins.set(pluginName, {
                plugin: null as any,
                pluginName,
                status: 'error',
                error: String(e)
            });
        }
    }

    /**
     * Load plugins from plugins.ini
     * Returns list of plugin names to load
     */
    parsePluginsIni(): string[] {
        const plugins: string[] = [];

        if (!fs.existsSync(this.configPath)) {
            return [];
        }

        try {
            const content = fs.readFileSync(this.configPath, 'utf-8');
            const lines = content.split('\n');

            for (const line of lines) {
                const trimmed = line.trim();

                // Skip empty lines and comments
                if (!trimmed || trimmed.startsWith(';')) {
                    continue;
                }

                // Extract plugin name (everything before semicolon comment)
                const parts = trimmed.split(';');
                const pluginName = parts[0].trim();

                if (pluginName) {
                    plugins.push(pluginName);
                }
            }
        } catch (e) {
            console.error(`[PluginLoader] Error reading plugins.ini:`, e);
        }

        return plugins;
    }

    /**
     * Load all plugins from plugins.ini
     */
    loadFromConfig(): void {
        const pluginNames = this.parsePluginsIni();

        for (const pluginName of pluginNames) {
            // Skip admin as it's loaded separately (dependency)
            if (pluginName === 'admin') {
                continue;
            }

            this.loadPluginSync(pluginName);
        }
    }

    /**
     * Get all loaded plugins
     */
    getPlugins(): LoadedPlugin[] {
        return Array.from(this.plugins.values());
    }

    /**
     * Get plugin count
     */
    getPluginCount(): number {
        return this.plugins.size;
    }

    /**
     * Get running plugin count
     */
    getRunningCount(): number {
        return Array.from(this.plugins.values()).filter(p => p.status === 'running').length;
    }

    /**
     * Get plugin by name
     */
    getPlugin(pluginName: string): LoadedPlugin | undefined {
        return this.plugins.get(pluginName);
    }

    /**
     * Check if a plugin is loaded
     */
    isLoaded(pluginName: string): boolean {
        const plugin = this.plugins.get(pluginName);
        return plugin?.status === 'running';
    }
}

// Singleton instance
export const pluginLoader = new PluginLoader();

/**
 * Helper function to create plugin metadata
 */
export function createPluginMetadata(
    name: string,
    version: string,
    author: string,
    description?: string
): PluginMetadata {
    return { name, version, author, description };
}
