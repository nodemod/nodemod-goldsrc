#!/usr/bin/env python3
"""
Validates Ham function keyname mappings in ham_manager.cpp against AMX Mod X.

This ensures:
1. Array indices in g_hamFunctions[] match Ham enum values in ham_const.h
2. Keynames match AMX Mod X's hooklist[] for gamedata lookup

Usage:
    python3 scripts/validate_ham_keynames.py
"""

import re
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
AMXMODX_PATH = os.environ.get('AMXMODX_PATH', '/tmp/amxmodx-master')

HOOK_NATIVE_PATH = os.path.join(AMXMODX_PATH, 'modules/hamsandwich/hook_native.cpp')
OUR_HAM_CONST_PATH = os.path.join(PROJECT_ROOT, 'src/ham/ham_const.h')
OUR_MANAGER_PATH = os.path.join(PROJECT_ROOT, 'src/ham/ham_manager.cpp')


def parse_amx_hooklist(content):
    """Parse AMX Mod X hook_native.cpp hooklist[] for keynames in order."""
    keynames = []
    pattern = r'\{\s*V(?:_REMOVED)?\(\s*"(\w+)"'
    for match in re.finditer(pattern, content):
        keynames.append(match.group(1).lower())
    return keynames


def parse_our_ham_enums(content):
    """Parse our ham_const.h for Ham_ enum names in order."""
    enums = []
    pattern = r'^\s*(Ham_\w+)\s*[,=]'
    for line in content.split('\n'):
        match = re.match(pattern, line)
        if match:
            name = match.group(1)
            if name != 'Ham_EndMarker':
                enums.append(name)
    return enums


def parse_our_g_hamFunctions(content):
    """Parse our ham_manager.cpp g_hamFunctions[] for keynames in order."""
    keynames = []

    # Find the g_hamFunctions array - look for the opening brace
    in_array = False

    for line in content.split('\n'):
        if 'HamFunctionInfo g_hamFunctions[]' in line:
            in_array = True
            continue

        if not in_array:
            continue

        # Match: {"keyname", ...}
        match = re.search(r'\{\s*"(\w+)"', line)
        if match:
            keynames.append(match.group(1).lower())

        # Match nullptr terminator - end of array
        if '{nullptr' in line or '{ nullptr' in line:
            break

    return keynames


def main():
    # Check for files
    if not os.path.exists(HOOK_NATIVE_PATH):
        print(f"ERROR: AMX Mod X source not found at {AMXMODX_PATH}")
        sys.exit(1)

    # Read files
    with open(HOOK_NATIVE_PATH, 'r') as f:
        amx_content = f.read()

    with open(OUR_HAM_CONST_PATH, 'r') as f:
        our_const = f.read()

    with open(OUR_MANAGER_PATH, 'r') as f:
        our_manager = f.read()

    # Parse
    amx_keynames = parse_amx_hooklist(amx_content)
    our_enums = parse_our_ham_enums(our_const)
    our_keynames = parse_our_g_hamFunctions(our_manager)

    print("=" * 70)
    print("HAM KEYNAME MAPPING VALIDATION")
    print("=" * 70)
    print(f"\nAMX Mod X hooklist entries: {len(amx_keynames)}")
    print(f"Our ham_const.h enums:      {len(our_enums)}")
    print(f"Our g_hamFunctions entries: {len(our_keynames)}")

    exit_code = 0

    # Check 1: Array length matches enum count
    if len(our_keynames) != len(our_enums):
        exit_code = 1
        print(f"\n{'=' * 70}")
        print("ERROR: g_hamFunctions array size doesn't match Ham enum count!")
        print("=" * 70)
        print(f"Expected {len(our_enums)} entries, found {len(our_keynames)}")

        if len(our_keynames) < len(our_enums):
            print(f"\nMissing entries for enums {len(our_keynames)} to {len(our_enums)-1}:")
            for i in range(len(our_keynames), min(len(our_enums), len(our_keynames) + 10)):
                print(f"    [{i}] {our_enums[i]}")
            if len(our_enums) - len(our_keynames) > 10:
                print(f"    ... and {len(our_enums) - len(our_keynames) - 10} more")

    # Check 2: Keynames match AMX Mod X
    mismatches = []
    min_len = min(len(amx_keynames), len(our_keynames))

    for i in range(min_len):
        if amx_keynames[i] != our_keynames[i]:
            enum_name = our_enums[i] if i < len(our_enums) else f"index_{i}"
            mismatches.append((i, enum_name, amx_keynames[i], our_keynames[i]))

    if mismatches:
        exit_code = 1
        print(f"\n{'=' * 70}")
        print(f"KEYNAME MISMATCHES ({len(mismatches)})")
        print("=" * 70)
        for idx, enum_name, amx_key, our_key in mismatches[:20]:
            print(f"[{idx}] {enum_name}:")
            print(f"      AMX expects: '{amx_key}'")
            print(f"      We have:     '{our_key}'")
        if len(mismatches) > 20:
            print(f"... and {len(mismatches) - 20} more")

    # Check 3: Report first few mappings for verification
    print(f"\n{'=' * 70}")
    print("SAMPLE MAPPINGS (first 20)")
    print("=" * 70)
    for i in range(min(20, len(our_keynames))):
        enum_name = our_enums[i] if i < len(our_enums) else "???"
        amx_key = amx_keynames[i] if i < len(amx_keynames) else "???"
        our_key = our_keynames[i]
        match = "OK" if amx_key == our_key else "MISMATCH"
        print(f"[{i:3}] {enum_name:40} -> '{our_key}' [{match}]")

    if exit_code == 0:
        print(f"\n{'=' * 70}")
        print("ALL KEYNAME MAPPINGS VALIDATED!")
        print("=" * 70)

    sys.exit(exit_code)


if __name__ == '__main__':
    main()
