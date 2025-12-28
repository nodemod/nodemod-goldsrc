// Storage System
// Generic pluggable storage interface for admin data
// Backends are registered and selected via CVARs

import nodemodCore from '@nodemod/core';

const cvar = nodemodCore.cvar;

/**
 * Generic data entry for storage (string-based for serialization)
 */
export interface StorageEntry {
    [key: string]: string;
}

/**
 * SQL/Database configuration from CVARs
 */
export interface StorageConfig {
    type: string;      // amx_sql_type: mysql, postgres, sqlite, etc.
    host: string;      // amx_sql_host
    user: string;      // amx_sql_user
    password: string;  // amx_sql_pass
    database: string;  // amx_sql_db
    table: string;     // amx_sql_table
}

/**
 * Storage adapter interface - implement this for custom backends
 */
export interface StorageAdapter {
    /**
     * Unique name for this adapter (e.g., "file", "mysql", "sqlite")
     */
    readonly name: string;

    /**
     * Human-readable description
     */
    readonly description: string;

    /**
     * Initialize the adapter
     * @param config Configuration from CVARs
     * @returns true if initialization successful
     */
    initialize(config: StorageConfig): Promise<boolean>;

    /**
     * Load all entries from storage
     * @returns Array of entries
     */
    load(): Promise<StorageEntry[]>;

    /**
     * Save/add an entry to storage
     * @param entry Entry to save
     * @returns true if successful
     */
    save(entry: StorageEntry): Promise<boolean>;

    /**
     * Check if an entry exists (by primary key field)
     * @param key Primary key field name
     * @param value Value to check
     * @returns true if exists
     */
    exists(key: string, value: string): Promise<boolean>;

    /**
     * Clear all entries from storage
     * @returns true if successful
     */
    clear(): Promise<boolean>;

    /**
     * Cleanup/disconnect from storage
     */
    dispose(): Promise<void>;

    /**
     * Check if this adapter is available (dependencies installed, etc.)
     */
    isAvailable(): boolean;
}

/**
 * Storage backend registry
 * Manages available storage adapters and current selection
 */
export class StorageRegistry {
    private adapters: Map<string, StorageAdapter> = new Map();
    private currentAdapter: StorageAdapter | null = null;
    private initialized = false;

    // CVAR for backend selection
    private amxStorageBackend: any;

    constructor() {
        // Register the CVAR for backend selection
        cvar.register(
            'amx_storage_backend',
            'file',
            nodemod.FCVAR.SERVER | nodemod.FCVAR.ARCHIVE,
            'Storage backend to use (use amx_storage_list to see available backends)'
        );
        this.amxStorageBackend = cvar.wrap('amx_storage_backend');

        // Commands are registered by admin/index.ts with proper permissions
    }

    /**
     * Send message to client or console (not both)
     */
    sendMessage(entity: nodemod.Entity | null, message: string): void {
        if (entity) {
            nodemod.eng.clientPrintf(entity, nodemod.PRINT_TYPE.print_console, `${message}\n`);
        } else {
            console.log(message);
        }
    }

    /**
     * Register a storage adapter
     */
    register(adapter: StorageAdapter): void {
        if (this.adapters.has(adapter.name)) {
            console.warn(`[Storage] Adapter "${adapter.name}" already registered, replacing`);
        }
        this.adapters.set(adapter.name, adapter);
    }

    /**
     * Unregister a storage adapter
     */
    unregister(name: string): void {
        this.adapters.delete(name);
    }

    /**
     * Get a registered adapter by name
     */
    get(name: string): StorageAdapter | undefined {
        return this.adapters.get(name);
    }

    /**
     * Get all registered adapters
     */
    getAll(): StorageAdapter[] {
        return Array.from(this.adapters.values());
    }

    /**
     * List available backends (console and client output)
     */
    listBackends(entity?: nodemod.Entity | null): void {
        this.sendMessage(entity ?? null, '[Storage] Available backends:');
        for (const [name, adapter] of this.adapters) {
            const available = adapter.isAvailable() ? '' : ' (unavailable)';
            const current = this.currentAdapter?.name === name ? ' *ACTIVE*' : '';
            this.sendMessage(entity ?? null, `  - ${name}: ${adapter.description}${available}${current}`);
        }
        this.sendMessage(entity ?? null, `[Storage] Current backend CVAR: amx_storage_backend = "${this.amxStorageBackend?.value || 'file'}"`);
    }

    /**
     * Get storage configuration from CVARs
     */
    getConfig(): StorageConfig {
        return {
            type: cvar.getString('amx_sql_type') || 'mysql',
            host: cvar.getString('amx_sql_host') || '127.0.0.1',
            user: cvar.getString('amx_sql_user') || 'root',
            password: cvar.getString('amx_sql_pass') || '',
            database: cvar.getString('amx_sql_db') || 'amx',
            table: cvar.getString('amx_sql_table') || 'admins'
        };
    }

    /**
     * Initialize with the configured backend
     */
    async initialize(): Promise<boolean> {
        if (this.initialized && this.currentAdapter) {
            return true;
        }

        const backendName = this.amxStorageBackend?.value || 'file';
        return this.switchBackend(backendName);
    }

    /**
     * Switch to a different backend
     */
    async switchBackend(name: string, entity?: nodemod.Entity | null): Promise<boolean> {
        const adapter = this.adapters.get(name);
        if (!adapter) {
            this.sendMessage(entity ?? null, `[Storage] Backend "${name}" not registered`);
            this.listBackends(entity);
            return false;
        }

        if (!adapter.isAvailable()) {
            this.sendMessage(entity ?? null, `[Storage] Backend "${name}" is not available (missing dependencies?)`);
            return false;
        }

        // Dispose current adapter
        if (this.currentAdapter) {
            try {
                await this.currentAdapter.dispose();
            } catch (e) {
                console.error('[Storage] Error disposing previous adapter:', e);
            }
        }

        // Initialize new adapter
        const config = this.getConfig();
        try {
            const success = await adapter.initialize(config);
            if (success) {
                this.currentAdapter = adapter;
                this.initialized = true;
                this.sendMessage(entity ?? null, `[Storage] Switched to backend: ${name}`);
                return true;
            } else {
                this.sendMessage(entity ?? null, `[Storage] Backend "${name}" failed to initialize`);
                return false;
            }
        } catch (error) {
            this.sendMessage(entity ?? null, `[Storage] Backend "${name}" initialization error: ${error}`);
            return false;
        }
    }

    /**
     * Get the current adapter
     */
    getAdapter(): StorageAdapter | null {
        return this.currentAdapter;
    }

    /**
     * Check if initialized
     */
    isInitialized(): boolean {
        return this.initialized && this.currentAdapter !== null;
    }

    // Proxy methods to current adapter

    async load(): Promise<StorageEntry[]> {
        if (!this.currentAdapter) {
            throw new Error('Storage not initialized');
        }
        return this.currentAdapter.load();
    }

    async save(entry: StorageEntry): Promise<boolean> {
        if (!this.currentAdapter) {
            throw new Error('Storage not initialized');
        }
        return this.currentAdapter.save(entry);
    }

    async exists(key: string, value: string): Promise<boolean> {
        if (!this.currentAdapter) {
            throw new Error('Storage not initialized');
        }
        return this.currentAdapter.exists(key, value);
    }

    async clear(): Promise<boolean> {
        if (!this.currentAdapter) {
            throw new Error('Storage not initialized');
        }
        return this.currentAdapter.clear();
    }
}

// Export singleton registry
export const storage = new StorageRegistry();
