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
npm run build:examples

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

    # Copy plugins from examples package (src, dist, config files)
    cp -r packages/examples/src "$out_dir/addons/nodemod/plugins/"
    cp -r packages/examples/dist "$out_dir/addons/nodemod/plugins/"
    cp packages/examples/tsconfig.json "$out_dir/addons/nodemod/plugins/"
    cp packages/examples/.gitignore "$out_dir/addons/nodemod/plugins/"
    cp packages/examples/README.md "$out_dir/addons/nodemod/plugins/"

    # Remove examples folder (standalone examples, not needed in release)
    rm -rf "$out_dir/addons/nodemod/plugins/src/examples"
    rm -rf "$out_dir/addons/nodemod/plugins/dist/examples"

    # Copy admin package as local package (entire folder)
    mkdir -p "$out_dir/addons/nodemod/plugins/packages"
    cp -r packages/admin "$out_dir/addons/nodemod/plugins/packages/"
    rm -f "$out_dir/addons/nodemod/plugins/packages/admin/.git"

    # Copy package.json with local admin reference and npm core version
    local core_version=$(node -p "require('./packages/core/package.json').version")
    node -e "
        const pkg = require('./packages/examples/package.json');
        pkg.dependencies['@nodemod/core'] = '^${core_version}';
        pkg.dependencies['@nodemod/admin'] = 'file:./packages/admin';
        console.log(JSON.stringify(pkg, null, 2));
    " > "$out_dir/addons/nodemod/plugins/package.json"

    # Install production dependencies
    echo "Installing production dependencies (@nodemod/core@$core_version, @nodemod/admin from local)..."
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
