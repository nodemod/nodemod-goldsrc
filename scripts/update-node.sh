#!/bin/bash

# Script to update Node.js dependencies to v24

set -e

NODE_VERSION="24.6.0"
BASE_URL="https://nodejs.org/dist/v${NODE_VERSION}"
DEPS_DIR="$(dirname "$0")/../deps"

echo "Updating Node.js to version ${NODE_VERSION}..."

# Create temporary directory
TEMP_DIR=$(mktemp -d)
cd "$TEMP_DIR"

echo "Downloading Node.js headers..."
wget -q "${BASE_URL}/node-v${NODE_VERSION}-headers.tar.gz"
tar -xzf "node-v${NODE_VERSION}-headers.tar.gz"

echo "Downloading Node.js Linux x86 binary..."
wget -q "${BASE_URL}/node-v${NODE_VERSION}-linux-x86.tar.gz"
tar -xzf "node-v${NODE_VERSION}-linux-x86.tar.gz"

# Update headers
echo "Updating Node.js headers..."
rm -rf "$DEPS_DIR/node/include"
mkdir -p "$DEPS_DIR/node/include"
cp -r "node-v${NODE_VERSION}/include/node"/* "$DEPS_DIR/node/include/"

# Update library
echo "Updating Node.js library..."
rm -rf "$DEPS_DIR/node/lib"
mkdir -p "$DEPS_DIR/node/lib/Release/linux"
cp "node-v${NODE_VERSION}-linux-x86/lib/libnode.so.138" "$DEPS_DIR/node/lib/Release/linux/"

# Update V8 headers if needed
if [ -d "node-v${NODE_VERSION}/include/node/v8" ]; then
    echo "Updating V8 headers..."
    rm -rf "$DEPS_DIR/v8"
    mkdir -p "$DEPS_DIR/v8"
    cp -r "node-v${NODE_VERSION}/include/node/"v8*.h "$DEPS_DIR/v8/" 2>/dev/null || true
    cp -r "node-v${NODE_VERSION}/include/node/cppgc" "$DEPS_DIR/v8/" 2>/dev/null || true
    cp -r "node-v${NODE_VERSION}/include/node/libplatform" "$DEPS_DIR/v8/" 2>/dev/null || true
fi

# Update libuv headers if needed  
if [ -d "node-v${NODE_VERSION}/include/node/uv" ]; then
    echo "Updating libuv headers..."
    rm -rf "$DEPS_DIR/uv"
    mkdir -p "$DEPS_DIR/uv"
    cp "node-v${NODE_VERSION}/include/node/uv.h" "$DEPS_DIR/uv/"
    cp -r "node-v${NODE_VERSION}/include/node/uv" "$DEPS_DIR/uv/"
fi

# Clean up
cd /
rm -rf "$TEMP_DIR"

echo "Node.js successfully updated to version ${NODE_VERSION}"
echo "Note: You need to update CMakeLists.txt to reference libnode.so.138 instead of libnode.so.93"