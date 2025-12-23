#!/bin/bash

# Run script for xash3d-nodemod Docker container

set -e

IMAGE_NAME="xash3d-nodemod"
IMAGE_TAG="latest"
CONTAINER_NAME="xash3d-nodemod-build"

echo "Running Docker container: ${CONTAINER_NAME}"

# Remove existing container if it exists
docker rm -f "${CONTAINER_NAME}" 2>/dev/null || true

# Run the container and build the code
docker run --name "${CONTAINER_NAME}" \
    -v "$(pwd):/app" \
    -e NODE_ARCH="${NODE_ARCH:-all}" \
    "${IMAGE_NAME}:${IMAGE_TAG}" \
    bash -c "
        echo 'Initializing and updating submodules...'
        git submodule update --init --recursive
        
        echo 'Building Node.js v24 from source...'
        cd /app

        # Build both architectures (or just one if NODE_ARCH is set)
        ./scripts/build-node-dual-arch.sh --\$NODE_ARCH

        echo 'Converting thin archives to fat archives...'
        if [ -f './scripts/fat_archive.sh' ]; then
            if [ \"\$NODE_ARCH\" = \"all\" ] || [ \"\$NODE_ARCH\" = \"ia32\" ] || [ \"\$NODE_ARCH\" = \"x86\" ]; then
                if ls deps/node/out-ia32/Release/obj.target/*_fat.a >/dev/null 2>&1; then
                    echo 'ia32 fat archives already exist, skipping...'
                else
                    echo 'Converting ia32 archives...'
                    ./scripts/fat_archive.sh deps/node/out-ia32/Release/obj.target
                fi
            fi
            if [ \"\$NODE_ARCH\" = \"all\" ] || [ \"\$NODE_ARCH\" = \"x64\" ]; then
                if ls deps/node/out-x64/Release/obj.target/*_fat.a >/dev/null 2>&1; then
                    echo 'x64 fat archives already exist, skipping...'
                else
                    echo 'Converting x64 archives...'
                    ./scripts/fat_archive.sh deps/node/out-x64/Release/obj.target
                fi
            fi
        else
            echo 'Error: fat_archive.sh not found in scripts directory'
            exit 1
        fi
    "