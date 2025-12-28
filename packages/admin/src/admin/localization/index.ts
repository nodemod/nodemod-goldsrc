// Localization System
// Provides multi-language support similar to AMX Mod X's dictionary system
// Language files are stored in data/lang/<plugin>.txt (AMXX standard location)

import * as fs from 'fs';
import * as path from 'path';

// Dictionary entry with language variants
interface DictionaryEntry {
    [lang: string]: string;
}

// Full dictionary for a plugin
interface Dictionary {
    [key: string]: DictionaryEntry;
}

// Client language preferences (index -> lang code)
const clientLang: { [index: number]: string } = {};

// Default language
let defaultLang = 'en';

// Loaded dictionaries (plugin name -> dictionary)
const dictionaries: { [plugin: string]: Dictionary } = {};

// Server language (LANG_SERVER constant)
export const LANG_SERVER = 0;

// Get the data/lang directory path (AMXX stores language files in data/lang/)
function getLangPath(): string {
    return path.join(process.cwd(), '..', 'data', 'lang');
}

/**
 * Set the default language for the server
 */
export function setDefaultLang(lang: string): void {
    defaultLang = lang.toLowerCase();
}

/**
 * Get the default language
 */
export function getDefaultLang(): string {
    return defaultLang;
}

/**
 * Set a client's preferred language
 */
export function setClientLang(entity: nodemod.Entity, lang: string): void {
    const index = nodemod.eng.indexOfEdict(entity);
    if (index > 0) {
        clientLang[index] = lang.toLowerCase();
    }
}

/**
 * Get a client's preferred language
 * Checks in order: 1) Explicitly set language, 2) Client's "lang" info key, 3) Default
 */
export function getClientLang(entity: nodemod.Entity | null): string {
    if (!entity) {
        return defaultLang;
    }
    const index = nodemod.eng.indexOfEdict(entity);

    // Check if language was explicitly set
    if (clientLang[index]) {
        return clientLang[index];
    }

    // Try to read from client's info buffer (Half-Life client "lang" setting)
    try {
        const buffer = nodemod.eng.getInfoKeyBuffer(entity);
        const lang = nodemod.eng.infoKeyValue(buffer, 'lang');
        if (lang && lang.length >= 2) {
            // Cache it for future lookups
            clientLang[index] = lang.toLowerCase();
            return clientLang[index];
        }
    } catch (e) {
        // Info key reading not available, fall through to default
    }

    return defaultLang;
}

/**
 * Clear client language on disconnect
 */
export function clearClientLang(entity: nodemod.Entity): void {
    const index = nodemod.eng.indexOfEdict(entity);
    delete clientLang[index];
}

/**
 * Parse a language file in AMX format:
 * [en]
 * KEY = Value with %s placeholders
 *
 * [de]
 * KEY = German value
 */
function parseLanguageFile(content: string): Dictionary {
    const dictionary: Dictionary = {};
    let currentLang = '';

    const lines = content.split('\n');

    for (const line of lines) {
        const trimmed = line.trim();

        // Skip empty lines and comments
        if (!trimmed || trimmed.startsWith('//') || trimmed.startsWith(';')) {
            continue;
        }

        // Language section header
        const langMatch = trimmed.match(/^\[([a-z]{2})\]$/i);
        if (langMatch) {
            currentLang = langMatch[1].toLowerCase();
            continue;
        }

        // Key = Value pair
        if (currentLang) {
            const eqIndex = trimmed.indexOf('=');
            if (eqIndex > 0) {
                const key = trimmed.substring(0, eqIndex).trim().toUpperCase();
                let value = trimmed.substring(eqIndex + 1).trim();

                // Remove surrounding quotes if present
                if ((value.startsWith('"') && value.endsWith('"')) ||
                    (value.startsWith("'") && value.endsWith("'"))) {
                    value = value.slice(1, -1);
                }

                // Unescape common escape sequences (both AMX ^x and standard \x formats)
                value = value
                    .replace(/\^n/g, '\n')
                    .replace(/\^t/g, '\t')
                    .replace(/\\n/g, '\n')
                    .replace(/\\t/g, '\t')
                    .replace(/\\\\/g, '\\');

                if (!dictionary[key]) {
                    dictionary[key] = {};
                }
                dictionary[key][currentLang] = value;
            }
        }
    }

    return dictionary;
}

/**
 * Load a dictionary from file
 */
export function loadDictionary(pluginName: string): boolean {
    const langPath = getLangPath();
    const filePath = path.join(langPath, `${pluginName}.txt`);

    try {
        if (!fs.existsSync(filePath)) {
            return false;
        }

        const content = fs.readFileSync(filePath, 'utf-8');
        dictionaries[pluginName] = parseLanguageFile(content);

        // Language file loaded successfully
        return true;
    } catch (error) {
        console.error(`[Lang] Error loading ${pluginName}: ${error}`);
        return false;
    }
}

/**
 * Register dictionary entries programmatically
 */
export function registerDictionary(pluginName: string, dictionary: Dictionary): void {
    dictionaries[pluginName] = { ...dictionaries[pluginName], ...dictionary };
}

/**
 * Format a localized string with placeholder substitution
 * Supports %s, %d, %i, %f with optional width/padding (e.g., %02d, %10s)
 * Also supports precision (e.g., %.0f, %.2f) and positional %1$s %2$d etc.
 * If no placeholders are found but args are provided, appends them space-separated.
 */
function formatString(template: string, args: any[]): string {
    if (args.length === 0) {
        return template;
    }

    let result = template;
    let argIndex = 0;
    let usedArgs = new Set<number>();

    // Handle positional placeholders first (%1$s, %1$02d, %1$.2f, etc.)
    result = result.replace(/%(\d+)\$(0?)(\d*)(?:\.(\d+))?([sdif])/g, (_, pos, zero, width, precision, type) => {
        const idx = parseInt(pos) - 1;
        if (idx >= 0 && idx < args.length) {
            usedArgs.add(idx);
            return formatArg(args[idx], type, zero === '0', parseInt(width) || 0, parseInt(precision));
        }
        return '';
    });

    // Handle sequential placeholders (%s, %d, %02d, %10s, %.0f, %.2f, etc.)
    result = result.replace(/%(0?)(\d*)(?:\.(\d+))?([sdif])/g, (_, zero, width, precision, type) => {
        if (argIndex < args.length) {
            usedArgs.add(argIndex);
            return formatArg(args[argIndex++], type, zero === '0', parseInt(width) || 0, parseInt(precision));
        }
        return '';
    });

    // If no placeholders were used, append remaining args space-separated
    if (usedArgs.size === 0 && args.length > 0) {
        const appendArgs = args.map(a => String(a)).join(' ');
        result = result + ' ' + appendArgs;
    }

    return result;
}

function formatArg(value: any, type: string, zeroPad: boolean = false, width: number = 0, precision?: number): string {
    let result: string;
    switch (type) {
        case 's':
            result = String(value);
            break;
        case 'd':
        case 'i':
            result = Math.floor(Number(value)).toString();
            break;
        case 'f':
            const num = Number(value);
            // If precision is specified, use toFixed; otherwise just convert
            if (precision !== undefined && !isNaN(precision)) {
                result = num.toFixed(precision);
            } else {
                result = num.toString();
            }
            break;
        default:
            result = String(value);
    }

    // Apply width padding
    if (width > 0 && result.length < width) {
        const padChar = zeroPad ? '0' : ' ';
        result = padChar.repeat(width - result.length) + result;
    }

    return result;
}

/**
 * Get a localized string for a player
 *
 * @param entity Target player (null for server language)
 * @param plugin Plugin name (dictionary to use)
 * @param key Translation key
 * @param args Placeholder values
 */
export function getLang(
    entity: nodemod.Entity | null,
    plugin: string,
    key: string,
    ...args: any[]
): string {
    const dict = dictionaries[plugin];
    if (!dict) {
        return key; // No dictionary loaded
    }

    const entry = dict[key.toUpperCase()];
    if (!entry) {
        return key; // Key not found
    }

    // Get the language to use
    const lang = getClientLang(entity);

    // Try requested language, then default, then any available
    let template = entry[lang] || entry[defaultLang];
    if (!template) {
        // Use first available language
        const availableLangs = Object.keys(entry);
        if (availableLangs.length > 0) {
            template = entry[availableLangs[0]];
        } else {
            return key;
        }
    }

    return formatString(template, args);
}

/**
 * Shorthand for getting a localized string using server language
 */
export function getServerLang(plugin: string, key: string, ...args: any[]): string {
    return getLang(null, plugin, key, ...args);
}

/**
 * Get a translation for a specific language code directly.
 * Unlike getLang(), this takes a language code string instead of an entity.
 * Returns null if the translation doesn't exist for that language.
 */
export function getLangForCode(
    langCode: string,
    plugin: string,
    key: string,
    ...args: any[]
): string | null {
    const dict = dictionaries[plugin];
    if (!dict) {
        return null;
    }

    const entry = dict[key.toUpperCase()];
    if (!entry) {
        return null;
    }

    const template = entry[langCode];
    if (!template) {
        return null;
    }

    return formatString(template, args);
}

/**
 * Create language file directory if it doesn't exist
 */
export function ensureLangDir(): void {
    const langPath = getLangPath();
    if (!fs.existsSync(langPath)) {
        fs.mkdirSync(langPath, { recursive: true });
    }
}

/**
 * List available languages for a plugin
 */
export function getAvailableLanguages(plugin: string): string[] {
    const dict = dictionaries[plugin];
    if (!dict) {
        return [];
    }

    const langs = new Set<string>();
    for (const key in dict) {
        for (const lang in dict[key]) {
            langs.add(lang);
        }
    }
    return Array.from(langs);
}

// Initialize
ensureLangDir();

export default {
    setDefaultLang,
    getDefaultLang,
    setClientLang,
    getClientLang,
    clearClientLang,
    loadDictionary,
    registerDictionary,
    getLang,
    getLangForCode,
    getServerLang,
    getAvailableLanguages,
    LANG_SERVER
};
