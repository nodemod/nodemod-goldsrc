// Admin System Constants
// Equivalent to amxconst.inc from AMX Mod X
// These are shared across all admin plugins

// ============================================================================
// Access Level Flags (from amxconst.inc)
// These represent what actions an admin can perform
// ============================================================================

export const ADMIN_IMMUNITY   = (1 << 0);   // flag "a" - immune to kicks/bans
export const ADMIN_RESERVATION = (1 << 1);  // flag "b" - reserved slot
export const ADMIN_KICK       = (1 << 2);   // flag "c" - kick players
export const ADMIN_BAN        = (1 << 3);   // flag "d" - ban players
export const ADMIN_SLAY       = (1 << 4);   // flag "e" - slay/slap players
export const ADMIN_MAP        = (1 << 5);   // flag "f" - change map
export const ADMIN_CVAR       = (1 << 6);   // flag "g" - change cvars
export const ADMIN_CFG        = (1 << 7);   // flag "h" - execute configs
export const ADMIN_CHAT       = (1 << 8);   // flag "i" - admin chat
export const ADMIN_VOTE       = (1 << 9);   // flag "j" - vote commands
export const ADMIN_PASSWORD   = (1 << 10);  // flag "k" - password command
export const ADMIN_RCON       = (1 << 11);  // flag "l" - rcon access
export const ADMIN_LEVEL_A    = (1 << 12);  // flag "m" - custom level A
export const ADMIN_LEVEL_B    = (1 << 13);  // flag "n" - custom level B
export const ADMIN_LEVEL_C    = (1 << 14);  // flag "o" - custom level C
export const ADMIN_LEVEL_D    = (1 << 15);  // flag "p" - custom level D
export const ADMIN_LEVEL_E    = (1 << 16);  // flag "q" - custom level E
export const ADMIN_LEVEL_F    = (1 << 17);  // flag "r" - custom level F
export const ADMIN_LEVEL_G    = (1 << 18);  // flag "s" - custom level G
export const ADMIN_LEVEL_H    = (1 << 19);  // flag "t" - custom level H
export const ADMIN_MENU       = (1 << 20);  // flag "u" - menu access
export const ADMIN_USER       = (1 << 25);  // flag "z" - user flag (non-admin)

// Special pseudo-flags for access checking
export const ADMIN_ALL        = 0;          // everyone has access
export const ADMIN_ADMIN      = (1 << 24);  // any admin flag

// ============================================================================
// Admin Auth Type Flags (internal to admin.sma)
// These determine how an admin is identified
// ============================================================================

export const ADMIN_LOOKUP     = (1 << 0);   // Lookup from connected player
export const ADMIN_NORMAL     = (1 << 1);   // Normal auth
export const ADMIN_STEAM      = (1 << 2);   // SteamID auth
export const ADMIN_IPADDR     = (1 << 3);   // IP address auth
export const ADMIN_NAME       = (1 << 4);   // Name auth

// ============================================================================
// Account Flags (from users.ini - column 4)
// These control how the admin account authenticates/behaves
// NOT to be confused with ADMIN_* flags which control what admins can DO
// ============================================================================

export const FLAG_DISCONNECT_ON_BAD_PASS = (1 << 0);  // 'a' - kick player if wrong password
export const FLAG_TAG             = (1 << 1);          // 'b' - clan tag (partial name match)
export const FLAG_AUTHID          = (1 << 2);          // 'c' - auth is steamid/wonid
export const FLAG_IP              = (1 << 3);          // 'd' - auth is ip address
export const FLAG_NOPASS          = (1 << 4);          // 'e' - password not checked
export const FLAG_CASE_SENSITIVE  = (1 << 10);         // 'k' - name or tag is case sensitive

// Legacy alias (AMX uses FLAG_KICK but it's confusing with ADMIN_KICK)
export const FLAG_KICK = FLAG_DISCONNECT_ON_BAD_PASS;

// ============================================================================
// cmd_target Flags (from amxmisc.inc)
// These control player targeting behavior
// ============================================================================

export const CMDTARGET_OBEY_IMMUNITY = (1 << 0);  // Obey immunity flag
export const CMDTARGET_ALLOW_SELF    = (1 << 1);  // Allow targeting yourself
export const CMDTARGET_ONLY_ALIVE    = (1 << 2);  // Only target alive players
export const CMDTARGET_NO_BOTS       = (1 << 3);  // Exclude bots from targeting

// ============================================================================
// Flag Conversion Utilities
// ============================================================================

/**
 * Convert flag string (e.g., "abcde") to numeric flags
 * @param flagString String of flag characters a-z
 * @returns Numeric flag value
 */
export function readFlags(flagString: string): number {
    let flags = 0;
    for (const char of flagString) {
        const code = char.toLowerCase().charCodeAt(0);
        if (code >= 97 && code <= 122) { // a-z
            flags |= (1 << (code - 97));
        }
    }
    return flags;
}

/**
 * Convert numeric flags to flag string
 * @param flags Numeric flag value
 * @returns String of flag characters a-z
 */
export function getFlags(flags: number): string {
    let result = '';
    for (let i = 0; i < 26; i++) {
        if (flags & (1 << i)) {
            result += String.fromCharCode(97 + i);
        }
    }
    return result;
}
