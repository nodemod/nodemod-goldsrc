#!/usr/bin/env python3
"""
Validates Ham function mappings against AMX Mod X implementation.

Usage:
    python3 scripts/validate_ham_mappings.py

Requires:
    - AMX Mod X source at /tmp/amxmodx-master
    - Or set AMXMODX_PATH environment variable
"""

import re
import os
import sys

# Paths
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
AMXMODX_PATH = os.environ.get('AMXMODX_PATH', '/tmp/amxmodx-master')

HOOK_NATIVE_PATH = os.path.join(AMXMODX_PATH, 'modules/hamsandwich/hook_native.cpp')
OUR_CALLBACKS_PATH = os.path.join(PROJECT_ROOT, 'src/ham/hook_callbacks.cpp')
OUR_CONST_PATH = os.path.join(PROJECT_ROOT, 'src/ham/ham_const.h')

# Special name mappings for differences between our naming and AMX Mod X
NAME_FIXES = {
    'dod_weapon_iswatersniping': 'dod_weapon_playeriswatersniping',
    'tfc_db_getitemname': 'tfc_dbgetitemname',
    'esf_sendclientscustommodel': 'esf_sendclientcustommodel',
    'ns_getnameditem': 'ns_givenameditem',
    'ns_weapon_getweapprimetime': 'ns_weapon_getweaponprimetime',
    'sc_isfacings': 'sc_isfacing',
    'sc_checkapplygenericattacks': 'sc_checkandapplygenericattacks',
    'sc_sub_usetargets': 'sc_subusetargets',
}


def ham_to_amx_name(ham_name):
    """Convert Ham_XXX enum name to AMX Mod X lookup name."""
    name = ham_name.replace('Ham_', '')

    # Handle mod prefixes
    if name.startswith('CS_'):
        name = 'cstrike_' + name[3:]
    elif name.startswith('DOD_'):
        name = 'dod_' + name[4:]
    elif name.startswith('TFC_'):
        name = 'tfc_' + name[4:]
    elif name.startswith('NS_'):
        name = 'ns_' + name[3:]
    elif name.startswith('ESF_'):
        name = 'esf_' + name[4:]
    elif name.startswith('SC_'):
        name = 'sc_' + name[3:]
    elif name.startswith('TS_'):
        name = 'ts_' + name[3:]
    elif name.startswith('OPF_'):
        name = 'gearbox_' + name[4:]

    result = name.lower()
    return NAME_FIXES.get(result, result)


def parse_amx_mappings(content):
    """Parse AMX Mod X hook_native.cpp for function type mappings."""
    mappings = {}

    # Match: { V("name", Type_Type) }
    pattern = r'\{\s*V\(\s*"(\w+)"\s*,\s*(\w+)\s*\)'
    for match in re.finditer(pattern, content):
        name = match.group(1).lower()
        type_name = match.group(2)
        mappings[name] = f'Hook_{type_name}'

    # Match: { V_REMOVED("name") }
    removed_pattern = r'\{\s*V_REMOVED\(\s*"(\w+)"\s*\)'
    for match in re.finditer(removed_pattern, content):
        name = match.group(1).lower()
        mappings[name] = 'Hook_Removed'

    return mappings


def parse_our_ham_names(content):
    """Parse our ham_const.h for Ham_ enum names."""
    names = []
    pattern = r'^\s*(Ham_\w+)\s*[,=]'
    for line in content.split('\n'):
        match = re.match(pattern, line)
        if match:
            name = match.group(1)
            if name != 'Ham_EndMarker':
                names.append(name)
    return names


def parse_our_mappings(content):
    """Parse our hook_callbacks.cpp for case mappings."""
    mappings = {}
    duplicates = []
    current_cases = []

    for line in content.split('\n'):
        case_match = re.search(r'case (Ham_\w+):', line)
        if case_match:
            current_cases.append(case_match.group(1))

        handler_match = re.search(r'return reinterpret_cast<void\*>\((Hook_\w+)\)', line)
        if handler_match and current_cases:
            handler = handler_match.group(1)
            for case in current_cases:
                if case in mappings:
                    duplicates.append((case, mappings[case], handler))
                mappings[case] = handler
            current_cases = []

    return mappings, duplicates


def normalize_type(type_name):
    """Normalize type names for comparison."""
    if type_name and ('Deprecated' in type_name or 'Removed' in type_name):
        return 'Hook_Deprecated'
    return type_name


def main():
    # Check for AMX Mod X source
    if not os.path.exists(HOOK_NATIVE_PATH):
        print(f"ERROR: AMX Mod X source not found at {AMXMODX_PATH}")
        print("Please clone AMX Mod X to /tmp/amxmodx-master or set AMXMODX_PATH")
        sys.exit(1)

    # Read files
    with open(HOOK_NATIVE_PATH, 'r') as f:
        amx_content = f.read()

    with open(OUR_CALLBACKS_PATH, 'r') as f:
        our_callbacks = f.read()

    with open(OUR_CONST_PATH, 'r') as f:
        our_const = f.read()

    # Parse
    amx_mappings = parse_amx_mappings(amx_content)
    our_ham_names = parse_our_ham_names(our_const)
    our_mappings, duplicates = parse_our_mappings(our_callbacks)

    # Compare
    matches = 0
    mismatches = []
    not_in_amx = []
    not_mapped = []
    invalid_cases = []

    # Check for case statements that reference non-existent enums
    our_ham_names_set = set(our_ham_names)
    for case_name in our_mappings.keys():
        if case_name not in our_ham_names_set:
            invalid_cases.append(case_name)

    for ham_name in our_ham_names:
        amx_name = ham_to_amx_name(ham_name)
        amx_type = amx_mappings.get(amx_name)
        our_type = our_mappings.get(ham_name)

        if amx_type is None:
            not_in_amx.append((ham_name, amx_name, our_type))
            continue

        if our_type is None:
            not_mapped.append((ham_name, amx_name, amx_type))
            continue

        amx_type_norm = normalize_type(amx_type)
        our_type_norm = normalize_type(our_type)

        if amx_type_norm == our_type_norm:
            matches += 1
        else:
            mismatches.append((ham_name, amx_name, amx_type, our_type))

    # Report
    print("=" * 70)
    print("HAM FUNCTIONS VALIDATION (Against AMX Mod X Implementation)")
    print("=" * 70)
    print(f"\nTotal Ham functions defined: {len(our_ham_names)}")
    print(f"Correctly mapped:            {matches}")
    print(f"MISMATCHES (wrong type):     {len(mismatches)}")
    print(f"INVALID CASES (bad enum):    {len(invalid_cases)}")
    print(f"DUPLICATE CASES:             {len(duplicates)}")
    print(f"Not mapped yet:              {len(not_mapped)}")
    print(f"Not in AMX Mod X:            {len(not_in_amx)}")

    # Exit code based on mismatches or invalid cases
    exit_code = 0

    if duplicates:
        exit_code = 1
        print(f"\n{'=' * 70}")
        print(f"DUPLICATE CASE STATEMENTS ({len(duplicates)}) - WILL NOT COMPILE")
        print("=" * 70)
        for case_name, first_handler, second_handler in duplicates:
            print(f"    {case_name}: {first_handler} vs {second_handler}")

    if invalid_cases:
        exit_code = 1
        print(f"\n{'=' * 70}")
        print(f"INVALID CASE STATEMENTS ({len(invalid_cases)}) - WILL NOT COMPILE")
        print("=" * 70)
        print("These case statements reference enums that don't exist in ham_const.h:")
        for case_name in sorted(invalid_cases):
            print(f"    {case_name}")

    if mismatches:
        exit_code = 1
        print(f"\n{'=' * 70}")
        print(f"MISMATCHES - THESE NEED FIXING ({len(mismatches)})")
        print("=" * 70)
        for ham_name, amx_name, amx_type, our_type in mismatches:
            print(f"\n{ham_name}:")
            print(f"    AMX Mod X expects: {amx_type}")
            print(f"    We have:           {our_type}")
    else:
        print(f"\n{'=' * 70}")
        print("ALL MAPPED FUNCTIONS ARE CORRECT!")
        print("=" * 70)

    # Group unmapped by mod
    if not_mapped:
        print(f"\n{'=' * 70}")
        print(f"UNMAPPED FUNCTIONS BY MOD ({len(not_mapped)} total)")
        print("=" * 70)

        groups = {}
        for ham_name, amx_name, amx_type in not_mapped:
            parts = ham_name.split('_')
            if len(parts) >= 2 and parts[1] in ['SC', 'NS', 'ESF', 'TFC', 'DOD', 'TS', 'CS', 'OPF']:
                prefix = parts[1]
            else:
                prefix = 'Core'
            if prefix not in groups:
                groups[prefix] = []
            groups[prefix].append((ham_name, amx_type))

        for prefix in sorted(groups.keys()):
            print(f"\n{prefix} ({len(groups[prefix])} functions):")
            for ham_name, amx_type in groups[prefix][:5]:
                print(f"    {ham_name} -> {amx_type}")
            if len(groups[prefix]) > 5:
                print(f"    ... and {len(groups[prefix]) - 5} more")

    if not_in_amx:
        print(f"\n{'=' * 70}")
        print(f"NOT IN AMX MOD X ({len(not_in_amx)} total)")
        print("=" * 70)
        for ham_name, amx_name, our_type in not_in_amx:
            print(f"    {ham_name} (looked for: {amx_name})")

    # Generate missing mappings code if requested
    if '--generate' in sys.argv and not_mapped:
        print(f"\n{'=' * 70}")
        print("GENERATED CODE FOR MISSING MAPPINGS")
        print("=" * 70)

        # Group by handler type
        by_handler = {}
        for ham_name, amx_name, amx_type in not_mapped:
            if amx_type not in by_handler:
                by_handler[amx_type] = []
            by_handler[amx_type].append(ham_name)

        for handler in sorted(by_handler.keys()):
            funcs = by_handler[handler]
            print(f"\n        // {handler.replace('Hook_', '')} functions")
            for func in sorted(funcs):
                print(f"        case {func}:")
            print(f"            return reinterpret_cast<void*>({handler});")

    sys.exit(exit_code)


if __name__ == '__main__':
    main()
