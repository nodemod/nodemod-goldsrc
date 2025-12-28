// Example: In-Memory Storage Adapter
// This is an example of a user-defined storage adapter.
// It stores data in memory only - data is lost on server restart.
//
// To use this adapter:
//   1. Copy this file to your plugin directory
//   2. Import and register it:
//
//      import { MemoryStorageAdapter } from './memory.example';
//      import { storage } from './admin/storage';
//
//      storage.register(new MemoryStorageAdapter());
//
//   3. Select it via CVAR or command:
//      amx_storage_backend memory
//      -- or --
//      amx_storage_use memory

import { StorageAdapter, StorageConfig, StorageEntry } from './index';

/**
 * In-memory storage adapter (ephemeral)
 * Data is lost when the server restarts.
 * Useful for testing or temporary admin access.
 */
export class MemoryStorageAdapter implements StorageAdapter {
    readonly name = 'memory';
    readonly description = 'In-memory storage (ephemeral, lost on restart)';

    private entries: StorageEntry[] = [];

    isAvailable(): boolean {
        // Always available - no dependencies
        return true;
    }

    async initialize(_config: StorageConfig): Promise<boolean> {
        // Nothing to initialize
        console.log('[Memory] Initialized in-memory storage');
        console.log('[Memory] WARNING: Data will be lost on server restart!');
        return true;
    }

    async load(): Promise<StorageEntry[]> {
        return [...this.entries]; // Return a copy
    }

    async save(entry: StorageEntry): Promise<boolean> {
        // Check if entry already exists
        const existingIndex = this.entries.findIndex(e => e.auth === entry.auth);
        if (existingIndex !== -1) {
            // Update existing
            this.entries[existingIndex] = { ...entry };
        } else {
            // Add new
            this.entries.push({ ...entry });
        }
        return true;
    }

    async exists(key: string, value: string): Promise<boolean> {
        return this.entries.some(e => e[key] === value);
    }

    async clear(): Promise<boolean> {
        this.entries = [];
        return true;
    }

    async dispose(): Promise<void> {
        this.entries = [];
    }

    // Additional helper methods for testing

    /**
     * Get entry count (for debugging)
     */
    count(): number {
        return this.entries.length;
    }

    /**
     * Pre-populate with test data
     */
    seed(entries: StorageEntry[]): void {
        this.entries = entries.map(e => ({ ...e }));
    }
}

// NOTE: This adapter is NOT auto-registered.
// Users must explicitly register it if they want to use it.
//
// Example:
//   import { MemoryStorageAdapter } from './storage/memory.example';
//   import { storage } from './storage';
//   storage.register(new MemoryStorageAdapter());

export default MemoryStorageAdapter;
