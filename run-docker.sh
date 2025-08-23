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
        
        echo 'Bootstrapping vcpkg...'
        cd /app/deps/vcpkg && ./bootstrap-vcpkg.sh
        
        echo 'Creating build directory...'
        cd /app
        cmake -E make_directory build
        cd build
        
        echo 'Configuring project...'
        cmake .. --preset linux-x86-debug
        
        echo 'Building xash3d-nodemod...'
        cmake --build . --config Debug
    "

echo "Container run completed. Built files are available in ./output/"
echo "The compiled libnodemod.so should be in ./output/Debug/bin/"