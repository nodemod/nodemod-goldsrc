#!/usr/bin/env python3
"""
Extracts Ham function keyname mappings from AMX Mod X.

This creates the mapping from Ham_XXX enum to gamedata keyname,
which is needed to look up VTable indices from offsets-common.txt files.

Usage:
    python3 scripts/extract_ham_keynames.py
"""

import re
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
AMXMODX_PATH = os.environ.get('AMXMODX_PATH', '/tmp/amxmodx-master')

HOOK_NATIVE_PATH = os.path.join(AMXMODX_PATH, 'modules/hamsandwich/hook_native.cpp')
HAM_CONST_PATH = os.path.join(AMXMODX_PATH, 'modules/hamsandwich/ham_const.h')
GAMEDATA_PATH = os.path.join(AMXMODX_PATH, 'gamedata/common.games/virtual.games')


def parse_hooklist(content):
    """Parse hook_native.cpp hooklist[] array to get keynames in order."""
    keynames = []

    # Match: { V("keyname", Type) } or { V_REMOVED("keyname") }
    pattern = r'\{\s*V(?:_REMOVED)?\(\s*"(\w+)"'

    for match in re.finditer(pattern, content):
        keynames.append(match.group(1))

    return keynames


def parse_ham_enums(content):
    """Parse ham_const.h for Ham_ enum names in order."""
    enums = []
    pattern = r'^\s*(Ham_\w+)\s*[,=]'

    for line in content.split('\n'):
        match = re.match(pattern, line)
        if match:
            name = match.group(1)
            if name != 'HAM_LAST_ENTRY_DONT_USE_ME_LOL':
                enums.append(name)

    return enums


def parse_gamedata_offsets(filepath):
    """Parse an offsets-common.txt file for offset values."""
    offsets = {}

    with open(filepath, 'r') as f:
        content = f.read()

    # Simple pattern matching for offset entries
    current_name = None
    for line in content.split('\n'):
        # Match offset name
        name_match = re.match(r'\s*"(\w+)"\s*$', line)
        if name_match:
            current_name = name_match.group(1).lower()
            continue

        # Match platform value
        if current_name:
            value_match = re.match(r'\s*"(windows|linux|mac)"\s+"(-?\d+|0x[0-9a-fA-F]+)"', line)
            if value_match:
                platform = value_match.group(1)
                value_str = value_match.group(2)
                value = int(value_str, 16) if value_str.startswith('0x') else int(value_str)

                if current_name not in offsets:
                    offsets[current_name] = {}
                offsets[current_name][platform] = value

    return offsets


def main():
    # Read AMX Mod X files
    with open(HOOK_NATIVE_PATH, 'r') as f:
        hooklist_content = f.read()

    with open(HAM_CONST_PATH, 'r') as f:
        ham_const_content = f.read()

    # Parse
    keynames = parse_hooklist(hooklist_content)
    enums = parse_ham_enums(ham_const_content)

    print("=" * 70)
    print("HAM ENUM TO KEYNAME MAPPING")
    print("=" * 70)
    print(f"\nTotal enums: {len(enums)}")
    print(f"Total keynames: {len(keynames)}")

    if len(enums) != len(keynames):
        print(f"\nWARNING: Mismatch in count!")

    # Generate C++ mapping
    print("\n// C++ keyname mapping array (index = Ham enum value)")
    print("static const char* ham_keynames[] = {")
    for i, keyname in enumerate(keynames):
        enum_name = enums[i] if i < len(enums) else f"// UNKNOWN_{i}"
        print(f'    "{keyname}",  // {enum_name}')
    print("};")

    # Load valve gamedata for verification
    valve_gamedata = os.path.join(GAMEDATA_PATH, 'valve/offsets-common.txt')
    if os.path.exists(valve_gamedata):
        offsets = parse_gamedata_offsets(valve_gamedata)

        print("\n" + "=" * 70)
        print("SAMPLE VTABLE INDICES (valve/offsets-common.txt)")
        print("=" * 70)

        for i, keyname in enumerate(keynames[:20]):
            enum_name = enums[i] if i < len(enums) else "UNKNOWN"
            offset_info = offsets.get(keyname.lower(), {})
            win = offset_info.get('windows', '?')
            lin = offset_info.get('linux', '?')
            print(f"{enum_name:40} -> '{keyname}' -> win:{win} lin:{lin}")

        if len(keynames) > 20:
            print(f"... and {len(keynames) - 20} more")


if __name__ == '__main__':
    main()
