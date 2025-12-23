#!/bin/bash

# Run script for xash3d-nodemod Docker container
# Builds x86 and/or x64 architectures based on NODEMOD_ARCH env var
# Usage: NODEMOD_ARCH=x86|x64|all ./build-nodemod.sh

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
    -e NODEMOD_ARCH="${NODEMOD_ARCH:-all}" \
    "${IMAGE_NAME}:${IMAGE_TAG}" \
    bash -c "
        echo 'Initializing and updating submodules...'
        git submodule update --init --recursive

        cd /app

        echo 'Bootstrapping vcpkg...'
        cd /app/deps/vcpkg && ./bootstrap-vcpkg.sh
        cd /app

        # Build x86
        if [ \"\$NODEMOD_ARCH\" = \"all\" ] || [ \"\$NODEMOD_ARCH\" = \"x86\" ] || [ \"\$NODEMOD_ARCH\" = \"ia32\" ]; then
            echo ''
            echo '=============================================='
            echo 'Building nodemod for x86...'
            echo '=============================================='
            cmake --preset linux-x86-debug
            cmake --build build-x86 --config Debug
        fi

        # Build x64
        if [ \"\$NODEMOD_ARCH\" = \"all\" ] || [ \"\$NODEMOD_ARCH\" = \"x64\" ]; then
            echo ''
            echo '=============================================='
            echo 'Building nodemod for x64...'
            echo '=============================================='
            cmake --preset linux-x64-debug
            cmake --build build-x64 --config Debug
        fi
    "

echo "Container run completed."

# Show output paths based on what was built
ARCH="${NODEMOD_ARCH:-all}"
if [ "$ARCH" = "all" ] || [ "$ARCH" = "x86" ] || [ "$ARCH" = "ia32" ]; then
    echo "  x86: ./build-x86/Debug/bin/libnodemod.so"
fi
if [ "$ARCH" = "all" ] || [ "$ARCH" = "x64" ]; then
    echo "  x64: ./build-x64/Debug/bin/libnodemod.so"
fi

# Copy the built .so file to the HLDS installation (x86 for Half-Life)
if [ -f "./build-x86/Debug/bin/libnodemod.so" ]; then
    echo "Copying x86 libnodemod.so to HLDS installation..."
    mkdir -p "/home/stevenlafl/Containers/hlds/hlds/ts/addons/nodemod/dlls/"
    cp "./build-x86/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds/ts/addons/nodemod/dlls/"
    cp "./build-x86/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds-new/ts/addons/nodemod/dlls/"
    cp "./build-x86/Debug/bin/libnodemod.so" "/home/stevenlafl/Containers/hlds/hlds-notloggedin/ts/addons/nodemod/dlls/"
    echo "Done"
fi