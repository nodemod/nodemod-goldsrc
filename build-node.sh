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
    "${IMAGE_NAME}:${IMAGE_TAG}" \
    bash -c "
        echo 'Initializing and updating submodules...'
        git submodule update --init --recursive
        
        echo 'Building Node.js v24 from source...'
        cd /app
        ./scripts/build-node-from-source.sh

        echo 'Converting thin archives to fat archives...'
        # Use our fat_archive.sh script from the scripts directory
        if [ -f './scripts/fat_archive.sh' ]; then
            echo 'Using fat_archive.sh script from scripts directory...'
            ./scripts/fat_archive.sh deps/node/out/Release/obj.target
        else
            echo 'Error: fat_archive.sh not found in scripts directory'
            exit 1
        fi
    "