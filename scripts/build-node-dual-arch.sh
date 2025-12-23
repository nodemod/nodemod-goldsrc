#!/bin/bash

# Script to build Node.js v24.6.0 for both 32-bit and 64-bit from a single source tree
# Outputs go to out-ia32/ and out-x64/ respectively
# Patches are applied once (idempotent) and work for both architectures

set -e

PROJECT_ROOT="$(dirname "$0")/.."
NODE_DIR="$PROJECT_ROOT/deps/node"

# Parse arguments
BUILD_32=false
BUILD_64=false
PATCHES_ONLY=false

for arg in "$@"; do
    case $arg in
        --ia32|--x86|--32)
            BUILD_32=true
            ;;
        --x64|--x86_64|--64)
            BUILD_64=true
            ;;
        --all)
            BUILD_32=true
            BUILD_64=true
            ;;
        --patches-only)
            PATCHES_ONLY=true
            ;;
        *)
            echo "Usage: $0 [--ia32|--x64|--all] [--patches-only]"
            echo "  --ia32, --x86, --32   Build 32-bit version"
            echo "  --x64, --x86_64, --64 Build 64-bit version"
            echo "  --all                 Build both architectures"
            echo "  --patches-only        Only apply patches, don't build"
            exit 1
            ;;
    esac
done

if [ "$BUILD_32" = false ] && [ "$BUILD_64" = false ] && [ "$PATCHES_ONLY" = false ]; then
    BUILD_32=true
fi

cd "$NODE_DIR"

# Apply patches (idempotent - safe to run multiple times)
apply_patches() {
    echo "Applying patches..."

    # OpenSSL assembly: %ifdef -> #ifdef (only patch files that still have %ifdef)
    echo "  - OpenSSL assembly syntax..."
    local patched=0
    for f in $(find deps/openssl -type f -name '*.S'); do
        if grep -q '%ifdef' "$f" 2>/dev/null; then
            sed -i 's/%ifdef/#ifdef/g; s/%endif/#endif/g' "$f"
            ((patched++)) || true
        fi
    done
    echo "    Patched $patched files"

    # Documentation placeholders
    echo "  - Documentation placeholders..."
    sed -i 's/REPLACEME/repl/g' doc/api/*.md 2>/dev/null || true

    # C++ ABI for ReHLDS compatibility
    echo "  - C++ ABI compatibility..."
    if grep -q "'_GLIBCXX_USE_CXX11_ABI=1'" common.gypi; then
        sed -i "s/'_GLIBCXX_USE_CXX11_ABI=1'/'_GLIBCXX_USE_CXX11_ABI=0'/g" common.gypi
        echo "    Applied ABI patch"
    else
        echo "    Already patched"
    fi

    # V8 Turboshaft 32-bit fix
    echo "  - V8 Turboshaft 32-bit fix..."
    V8_FILE="deps/v8/src/compiler/turboshaft/int64-lowering-reducer.h"
    if [ -f "$V8_FILE" ] && grep -q 'return __ Tuple(result, __ Word32Constant(0));' "$V8_FILE"; then
        sed -i 's/return __ Tuple(result, __ Word32Constant(0));/return __ Tuple(V<Word32>::Cast(result), __ Word32Constant(0));/g' "$V8_FILE"
        echo "    Applied V8 patch"
    else
        echo "    Already patched or not found"
    fi

    # x64 -fPIC and TLS model for embedding in shared libraries
    echo "  - x64 -fPIC and TLS model for shared library embedding..."
    if grep -q "'cflags': \[ '-m64' \]," common.gypi; then
        sed -i "s/'cflags': \[ '-m64' \],/'cflags': [ '-m64', '-fPIC', '-ftls-model=global-dynamic' ],/g" common.gypi
        echo "    Applied -fPIC and -ftls-model patch"
    elif grep -q "'cflags': \[ '-m64', '-fPIC' \]," common.gypi; then
        sed -i "s/'cflags': \[ '-m64', '-fPIC' \],/'cflags': [ '-m64', '-fPIC', '-ftls-model=global-dynamic' ],/g" common.gypi
        echo "    Applied -ftls-model patch (fPIC already present)"
    else
        echo "    Already patched or not needed"
    fi

    # ia32 -lm for static linking (32-bit libm.a is a real archive, not a linker script)
    # This must be in configure.py so it ends up in 'libraries' alongside '-static'
    echo "  - ia32 -lm for static linking in configure.py..."
    if grep -q "o\['libraries'\] += \['-static'\]$" configure.py; then
        sed -i "/o\['libraries'\] += \['-static'\]$/a\\      # 32-bit static linking needs explicit -lm (libm.a is a real archive, not a linker script)\\n      if options.dest_cpu == 'ia32':\\n        o['libraries'] += ['-lm']" configure.py
        echo "    Applied -lm patch to configure.py"
    elif grep -q "32-bit static linking needs explicit -lm" configure.py; then
        echo "    Already patched"
    else
        echo "    Pattern not found or unexpected format"
    fi

    # V8 TLS library mode for shared library embedding (fixes R_X86_64_TPOFF32 relocations)
    echo "  - V8 TLS library mode for shared library embedding..."
    V8_TLS_FILE="deps/v8/src/common/thread-local-storage.h"
    if [ -f "$V8_TLS_FILE" ] && grep -q "^#if V8_TLS_LIBRARY_MODE" "$V8_TLS_FILE"; then
        sed -i 's/^#if V8_TLS_LIBRARY_MODE/#if 1  \/\/ Force library mode for shared lib embedding (was: V8_TLS_LIBRARY_MODE)/' "$V8_TLS_FILE"
        echo "    Applied V8 TLS library mode patch"
    elif grep -q "Force library mode for shared lib embedding" "$V8_TLS_FILE" 2>/dev/null; then
        echo "    Already patched"
    else
        echo "    File not found or unexpected format"
    fi

    echo "Patches complete."
}

build_arch() {
    local ARCH=$1
    local OUT_DIR="out-${ARCH}"

    echo ""
    echo "=============================================="
    echo "Building Node.js for ${ARCH}..."
    echo "=============================================="

    # Create arch-specific output directory if needed
    mkdir -p "${OUT_DIR}"

    # Symlink out -> out-{arch} so Node.js builds into the right cache
    ln -sfn "${OUT_DIR}" out

    # Check if we need to reconfigure (config.gypi is in root, not out/)
    local NEED_CONFIGURE=false
    if [ ! -f "config.gypi" ]; then
        NEED_CONFIGURE=true
    elif [ "$ARCH" = "ia32" ] && ! grep -q "'target_arch': 'ia32'" config.gypi; then
        NEED_CONFIGURE=true
    elif [ "$ARCH" = "x64" ] && ! grep -q "'target_arch': 'x64'" config.gypi; then
        NEED_CONFIGURE=true
    fi

    # For ia32, set 32-bit compiler BEFORE configure so it detects host_arch=ia32
    if [ "$ARCH" = "ia32" ]; then
        export CC="gcc -m32"
        export CXX="g++ -m32"
        export AR="ar"
        export LINK="g++ -m32"
    fi

    if [ "$NEED_CONFIGURE" = true ]; then
        echo "Configuring for ${ARCH}..."
        export SKIP_GIT_CHECK=1
        export GIT_DIR=""

        ./configure --dest-cpu=$ARCH
    else
        echo "Using existing configuration for ${ARCH}"
    fi

    # Fix docs after configure
    sed -i 's/REPLACEME/repl/g' doc/api/*.md 2>/dev/null || true

    echo "Building..."
    if [ "$ARCH" = "ia32" ]; then
        make -j$(nproc) DESTCPU="x86" ARCH="x86" VARIATION="" \
            CFLAGS="-m32 -msse2 -fPIC" CXXFLAGS="-m32 -fPIC" LDFLAGS="-m32 -lm" ARFLAGS="rcs"
    else
        # x64 needs -fPIC for embedding in shared libraries
        make -j$(nproc) DESTCPU="x64" ARCH="x64" VARIATION="" \
            CFLAGS="-fPIC" CXXFLAGS="-fPIC" ARFLAGS="rcs"
    fi

    # Setup lib directory
    mkdir -p "lib/Release/linux-${ARCH}"
    NODE_LIB=$(find "./${OUT_DIR}" -name "libnode.so.*" 2>/dev/null | head -1)
    if [ -n "$NODE_LIB" ]; then
        cp "$NODE_LIB" "lib/Release/linux-${ARCH}/"
        echo "Library: lib/Release/linux-${ARCH}/$(basename $NODE_LIB)"
    fi

    echo "${ARCH} build complete!"
}

# Always apply patches first
apply_patches

if [ "$PATCHES_ONLY" = true ]; then
    echo "Patches applied. Exiting (--patches-only mode)."
    exit 0
fi

# Build requested architectures
if [ "$BUILD_64" = true ]; then
    build_arch "x64"
fi

if [ "$BUILD_32" = true ]; then
    build_arch "ia32"
fi

echo ""
echo "=============================================="
echo "Build complete!"
echo "=============================================="
[ "$BUILD_64" = true ] && echo "  64-bit: out-x64/"
[ "$BUILD_32" = true ] && echo "  32-bit: out-ia32/"
