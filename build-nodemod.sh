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

echo "Container run completed. Built files are available in ./build/"
echo "The compiled libnodemod.so should be in ./build/Debug/bin/"

# Copy the built .so file to the HLDS installation
if [ -f "./build/Debug/bin/libnodemod.so" ]; then
    echo "Copying libnodemod.so to HLDS installation..."
    mkdir -p "/home/stevenlafl/Containers/hlds/hlds/ts/addons/nodemod/dlls/"
    cp "./build/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds/ts/addons/nodemod/dlls/"
    cp "./build/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds-new/ts/addons/nodemod/dlls/"
    cp "./build/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds-notloggedin/ts/addons/nodemod/dlls/"
    echo "✓ libnodemod.so copied successfully to /home/stevenlafl/Containers/hlds/hlds/ts/addons/nodemod/dlls/"
else
    echo "✗ libnodemod.so not found in ./build/Debug/bin/"
fi