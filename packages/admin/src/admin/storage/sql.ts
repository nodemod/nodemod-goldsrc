// SQL Storage Backend
// NodeMod enhancement - provides database-backed admin storage
//
// This is a significant enhancement over AMX Mod X's SQL support:
// - Uses Knex.js for cross-database compatibility
// - Supports MySQL, PostgreSQL, SQLite, and MSSQL
// - Auto-registers if knex package is installed
// - Provides automatic table creation with proper schema
// - Uses upsert (INSERT ... ON CONFLICT) for safe updates
//
// Configuration via CVARs (set in server.cfg or amxx.cfg):
//   amx_sql_type "mysql"     - Database type (mysql, postgres, sqlite, mssql)
//   amx_sql_host "localhost" - Database host
//   amx_sql_user "root"      - Database username
//   amx_sql_pass ""          - Database password
//   amx_sql_db "amx"         - Database name
//   amx_sql_table "admins"   - Table name for admin entries
//
// Installation:
//   npm install knex mysql2    (for MySQL)
//   npm install knex pg        (for PostgreSQL)
//   npm install knex better-sqlite3 (for SQLite)
//
// Auto-registers if knex is available

import { StorageAdapter, StorageConfig, StorageEntry, storage } from './index';

// Dynamic types - we don't import knex types directly to avoid build errors
// when knex isn't installed
type KnexInstance = any;

// Check if knex is available at runtime
function getKnex(): any | null {
    try {
        return require('knex');
    } catch {
        return null;
    }
}

/**
 * Knex-based SQL storage adapter
 * Supports multiple database types based on amx_sql_type CVAR
 */
export class KnexStorageAdapter implements StorageAdapter {
    readonly name = 'sql';
    readonly description = 'SQL database via Knex (MySQL, PostgreSQL, SQLite)';

    private db: KnexInstance | null = null;
    private tableName: string = 'admins';

    isAvailable(): boolean {
        return getKnex() !== null;
    }

    async initialize(config: StorageConfig): Promise<boolean> {
        const knex = getKnex();
        if (!knex) {
            console.error('[SQL] Knex not installed. Run: npm install knex <driver>');
            return false;
        }

        this.tableName = config.table || 'admins';

        // Map config type to knex client
        const clientMap: Record<string, string> = {
            'mysql': 'mysql2',
            'mysql2': 'mysql2',
            'mariadb': 'mysql2',
            'pg': 'pg',
            'postgres': 'pg',
            'postgresql': 'pg',
            'sqlite': 'better-sqlite3',
            'sqlite3': 'better-sqlite3',
            'mssql': 'mssql'
        };

        const client = clientMap[config.type.toLowerCase()];
        if (!client) {
            console.error(`[SQL] Unknown database type: ${config.type}`);
            console.log('[SQL] Supported types: mysql, postgres, sqlite, mssql');
            return false;
        }

        try {
            // Check if the driver is installed
            try {
                require(client);
            } catch {
                console.error(`[SQL] Database driver not installed. Run: npm install ${client}`);
                return false;
            }

            // Build knex config
            const knexConfig: any = {
                client,
                useNullAsDefault: client === 'better-sqlite3'
            };

            if (client === 'better-sqlite3') {
                const path = require('path');
                knexConfig.connection = {
                    filename: path.join(process.cwd(), 'configs', `${config.database}.sqlite`)
                };
            } else {
                knexConfig.connection = {
                    host: config.host,
                    user: config.user,
                    password: config.password,
                    database: config.database
                };
            }

            this.db = knex.default ? knex.default(knexConfig) : knex(knexConfig);

            // Test connection
            await this.db.raw('SELECT 1');
            console.log(`[SQL] Connected to ${config.type} database`);

            // Ensure table exists
            await this.ensureTable();

            return true;
        } catch (error) {
            console.error('[SQL] Connection failed:', error);
            return false;
        }
    }

    private async ensureTable(): Promise<void> {
        if (!this.db) return;

        const exists = await this.db.schema.hasTable(this.tableName);
        if (!exists) {
            await this.db.schema.createTable(this.tableName, (table: any) => {
                table.string('auth', 64).primary();
                table.string('password', 64).defaultTo('');
                table.string('access', 32).notNullable();
                table.string('flags', 32).notNullable();
            });
            console.log(`[SQL] Created table: ${this.tableName}`);
        }
    }

    async load(): Promise<StorageEntry[]> {
        if (!this.db) return [];

        try {
            const rows = await this.db(this.tableName)
                .select('auth', 'password', 'access', 'flags');
            return rows as StorageEntry[];
        } catch (error) {
            console.error('[SQL] Load failed:', error);
            return [];
        }
    }

    async save(entry: StorageEntry): Promise<boolean> {
        if (!this.db) return false;

        try {
            await this.db(this.tableName)
                .insert({
                    auth: entry.auth,
                    password: entry.password || '',
                    access: entry.access,
                    flags: entry.flags
                })
                .onConflict('auth')
                .merge();
            return true;
        } catch (error) {
            console.error('[SQL] Save failed:', error);
            return false;
        }
    }

    async exists(key: string, value: string): Promise<boolean> {
        if (!this.db) return false;

        try {
            const row = await this.db(this.tableName)
                .where(key, value)
                .first();
            return !!row;
        } catch (error) {
            console.error('[SQL] Exists check failed:', error);
            return false;
        }
    }

    async clear(): Promise<boolean> {
        if (!this.db) return false;

        try {
            await this.db(this.tableName).truncate();
            return true;
        } catch (error) {
            console.error('[SQL] Clear failed:', error);
            return false;
        }
    }

    async dispose(): Promise<void> {
        if (this.db) {
            await this.db.destroy();
            this.db = null;
            console.log('[SQL] Disconnected');
        }
    }
}

// Auto-register if knex is available
const sqlAdapter = new KnexStorageAdapter();
if (sqlAdapter.isAvailable()) {
    storage.register(sqlAdapter);
}

// Export for extension
export { KnexStorageAdapter as SqlStorageAdapter };
export default sqlAdapter;
