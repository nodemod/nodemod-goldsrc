#!/bin/bash
set -e

# Package release script for nodemod
# Usage: ./scripts/package-release.sh [x86|x64|both]

ARCH="${1:-both}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
RELEASE_DIR="$PROJECT_ROOT/release"

cd "$PROJECT_ROOT"

# Build types and packages first
echo "Building TypeScript packages..."
npm install
npm run build:types
npm run build:core
npm run build:admin

package_arch() {
    local arch=$1
    local so_path

    if [ "$arch" = "x86" ]; then
        so_path="build-x86/Debug/bin/libnodemod.so"
    else
        so_path="build-x64/Debug/bin/libnodemod.so"
    fi

    if [ ! -f "$so_path" ]; then
        echo "Error: $so_path not found. Build the native plugin first with:"
        echo "  NODEMOD_ARCH=$arch ./build-nodemod.sh"
        return 1
    fi

    local out_dir="$RELEASE_DIR/nodemod_$arch"
    echo "Packaging $arch release to $out_dir..."

    # Clean and create structure
    rm -rf "$out_dir"
    mkdir -p "$out_dir/addons/nodemod/dlls"
    mkdir -p "$out_dir/addons/nodemod/plugins"
    mkdir -p "$out_dir/addons/nodemod/logs"

    # Copy native plugin
    cp "$so_path" "$out_dir/addons/nodemod/dlls/"

    # Copy configs and data from admin package
    cp -r packages/admin/configs "$out_dir/addons/nodemod/"
    cp -r packages/admin/data "$out_dir/addons/nodemod/"

    # Copy plugins (src, dist, config files)
    cp -r packages/admin/src "$out_dir/addons/nodemod/plugins/"
    cp -r packages/admin/dist "$out_dir/addons/nodemod/plugins/"
    cp packages/admin/tsconfig.json "$out_dir/addons/nodemod/plugins/"
    cp packages/admin/.gitignore "$out_dir/addons/nodemod/plugins/"

    # Copy package.json and replace workspace dependency with npm version
    local core_version=$(node -p "require('./packages/core/package.json').version")
    sed "s/\"@nodemod\/core\": \"\\*\"/\"@nodemod\/core\": \"^${core_version}\"/" \
        packages/admin/package.json > "$out_dir/addons/nodemod/plugins/package.json"

    # Install production dependencies only
    echo "Installing production dependencies (@nodemod/core@$core_version)..."
    cd "$out_dir/addons/nodemod/plugins"
    npm install --omit=dev
    cd "$PROJECT_ROOT"

    # Create tarball
    cd "$out_dir"
    tar -czvf "../nodemod_$arch.tar.gz" addons
    cd "$PROJECT_ROOT"

    echo "Created: $RELEASE_DIR/nodemod_$arch.tar.gz"
}

case "$ARCH" in
    x86|ia32)
        package_arch x86
        ;;
    x64)
        package_arch x64
        ;;
    both|all)
        package_arch x86
        package_arch x64
        ;;
    *)
        echo "Usage: $0 [x86|x64|both]"
        exit 1
        ;;
esac

echo "Done!"
