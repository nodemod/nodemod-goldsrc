// File Storage Backend
// Stores admin data in configs/users.ini (AMX Mod X compatible format)

import fs from 'fs';
import path from 'path';
import { StorageAdapter, StorageConfig, StorageEntry, storage } from './index';

/**
 * File-based storage adapter
 * Uses configs/users.ini format compatible with AMX Mod X
 */
export class FileStorageAdapter implements StorageAdapter {
    readonly name = 'file';
    readonly description = 'File-based storage (configs/users.ini)';

    private configPath: string = '';

    isAvailable(): boolean {
        // File storage is always available
        return true;
    }

    async initialize(config: StorageConfig): Promise<boolean> {
        // Go up from plugins/ to nodemod/, then into configs/
        this.configPath = path.join(process.cwd(), '..', 'configs', 'users.ini');

        // Create default file if it doesn't exist
        if (!fs.existsSync(this.configPath)) {
            this.createDefaultFile();
        }

        return true;
    }

    async load(): Promise<StorageEntry[]> {
        if (!fs.existsSync(this.configPath)) {
            return [];
        }

        const content = fs.readFileSync(this.configPath, 'utf8');
        return this.parseUsersFile(content);
    }

    async save(entry: StorageEntry): Promise<boolean> {
        try {
            if (!fs.existsSync(this.configPath)) {
                this.createDefaultFile();
            }

            // Check if entry already exists
            const existing = await this.load();
            if (existing.some(e => e.auth === entry.auth)) {
                return false;
            }

            // Format: "auth" "password" "access" "flags"
            const line = `\n"${entry.auth}" "${entry.password || ''}" "${entry.access || ''}" "${entry.flags || ''}"`;
            fs.appendFileSync(this.configPath, line);
            return true;
        } catch (error) {
            console.error('[FileStorage] Failed to save entry:', error);
            return false;
        }
    }

    async exists(key: string, value: string): Promise<boolean> {
        const entries = await this.load();
        return entries.some(e => e[key] === value);
    }

    async clear(): Promise<boolean> {
        try {
            this.createDefaultFile();
            return true;
        } catch (error) {
            console.error('[FileStorage] Failed to clear:', error);
            return false;
        }
    }

    async dispose(): Promise<void> {
        // Nothing to cleanup for file storage
    }

    private parseUsersFile(content: string): StorageEntry[] {
        const entries: StorageEntry[] = [];
        const lines = content.split('\n');

        for (const line of lines) {
            const trimmed = line.trim();

            // Skip comments and empty lines
            if (!trimmed || trimmed.startsWith(';')) {
                continue;
            }

            // Parse line: "auth" "password" "access" "flags"
            const matches = trimmed.match(/"([^"]+)"\s+"([^"]*)"\s+"([^"]+)"\s+"([^"]+)"/);
            if (matches && matches.length >= 5) {
                const [, auth, password, access, flags] = matches;
                entries.push({ auth, password, access, flags });
            }
        }

        return entries;
    }

    private createDefaultFile(): void {
        const dir = path.dirname(this.configPath);
        if (!fs.existsSync(dir)) {
            fs.mkdirSync(dir, { recursive: true });
        }

        const defaultContent = `; Users configuration file
; File location: configs/users.ini
; Line format: "auth" "password" "access flags" "account flags"
;
; Access flags:
; a - immunity (can't be kicked/banned/slayed/slaped and affected by other commmands)
; b - reservation (can join on reserved slots)
; c - amx_kick command
; d - amx_ban and amx_unban commands
; e - amx_slay and amx_slap commands
; f - amx_map command
; g - amx_cvar command (not all cvars will be available)
; h - amx_cfg command
; i - amx_chat and other chat commands
; j - amx_vote and other vote commands
; k - access to sv_password cvar (by amx_cvar command)
; l - access to amx_rcon command and rcon_password cvar (by amx_cvar command)
; m - custom level A (for additional plugins)
; n - custom level B (for additional plugins)
; o - custom level C (for additional plugins)
; p - custom level D (for additional plugins)
; q - custom level E (for additional plugins)
; r - custom level F (for additional plugins)
; s - custom level G (for additional plugins)
; t - custom level H (for additional plugins)
; u - menu access
; z - user (no admin)
;
; Account flags:
; a - disconnect player on invalid password
; b - clan tag
; c - this is steamid/wonid
; d - this is ip
; e - password not checked (only name/ip/steamid needed)
; k - name or tag is case sensitive
;
; Examples of admin accounts:
; "STEAM_0:0:123456" "" "abcdefghijklmnopqrstu" "ce"
; "ID_deadbeefdeadbeef" "" "abcdefghijklmnopqrstu" "ce"  ; Xash3D ID format
; "123.45.67.89" "" "abcdefghijklmnopqrstu" "de"
; "My Name" "my_password" "abcdefghijklmnopqrstu" "a"

; Add your admins below
`;
        fs.writeFileSync(this.configPath, defaultContent);
    }
}

// Create and register the file storage adapter
const fileAdapter = new FileStorageAdapter();
storage.register(fileAdapter);

export default fileAdapter;
