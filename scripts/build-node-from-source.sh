#!/bin/bash

# Script to build Node.js v24.6.0 from source for 32-bit architecture
# Based on the approach used in metamod-hl1

set -e

PROJECT_ROOT="$(dirname "$0")/.."
NODE_DIR="$PROJECT_ROOT/deps/node"

echo "Building Node.js v24.6.0 from source for 32-bit architecture..."

cd "$NODE_DIR"

# Apply OpenSSL patches - they are needed for Node.js v24.6.0 32-bit assembly
echo "Applying OpenSSL assembly patches for 32-bit compatibility..."
for f in $(find deps/openssl -type f -name '*.S'); do 
    #echo "Patching $f"
    sed -i "s/%ifdef/#ifdef/" "$f"
    sed -i "s/%endif/#endif/" "$f"
done

# Fix documentation files that might cause build issues
echo "Fixing documentation placeholder issues..."
sed -i 's/REPLACEME/repl/g' doc/api/*.md 2>/dev/null || true

echo "Skipping clean to preserve git state..."
# make clean 2>/dev/null || true

echo "Cleaning any previous ICU build artifacts..."
rm -rf deps/icu-tmp 2>/dev/null || true

echo "Patching Node.js build system to use older C++ ABI for ReHLDS compatibility..."
# Patch common.gypi to set _GLIBCXX_USE_CXX11_ABI=0 instead of =1 (this is the main source)
sed -i "s/'_GLIBCXX_USE_CXX11_ABI=1'/'_GLIBCXX_USE_CXX11_ABI=0'/g" common.gypi

echo "Configuring Node.js for 32-bit static build..."
# Disable git checks and other build checks
export SKIP_GIT_CHECK=1
export GIT_DIR=""
# Let Node.js auto-detect Python instead of forcing a path
./configure --fully-static --dest-cpu=ia32

echo "Fixing documentation placeholder issues post-configure..."
sed -i 's/REPLACEME/repl/g' doc/api/*.md 2>/dev/null || true

echo "Applying V8 Turboshaft compilation fix for 32-bit..."
# Fix V8 template syntax issue in int64-lowering-reducer.h by using proper V<Word32> casting
sed -i 's/return __ Tuple(result, __ Word32Constant(0));/return __ Tuple(V<Word32>::Cast(result), __ Word32Constant(0));/g' deps/v8/src/compiler/turboshaft/int64-lowering-reducer.h

echo "Building Node.js (this will take a while)..."
# Use 'make binary' as per metamod-hl1 approach with forced 32-bit flags
export CC="gcc -m32"
export CXX="g++ -m32"
export AR="ar"
export LINK="g++ -m32"
make -j$(nproc) binary DESTCPU="x86" ARCH="x86" VARIATION="" CFLAGS="-m32 -msse2 -fPIC" CXXFLAGS="-m32 -fPIC" LDFLAGS="-m32" ARFLAGS="rcs"

echo "Creating lib directory structure..."
mkdir -p lib/Release/linux

# Find and copy the Node.js shared library
NODE_LIB=$(find ./out -name "libnode.so.*" | head -1)
if [ -n "$NODE_LIB" ]; then
    echo "Copying Node.js library: $NODE_LIB"
    cp "$NODE_LIB" lib/Release/linux/
    
    # Get the ABI version from the filename
    ABI_VERSION=$(basename "$NODE_LIB" | sed 's/libnode\.so\.//')
    echo "Built Node.js library with ABI version: $ABI_VERSION"
else
    echo "Warning: Could not find libnode.so in build output"
    echo "Available files in out/Release:"
    find ./out -name "*.so" -o -name "libnode*" | head -10
fi

echo ""
echo "Node.js v24.6.0 build completed successfully!"
echo "Library should be available at: $NODE_DIR/lib/Release/linux/"