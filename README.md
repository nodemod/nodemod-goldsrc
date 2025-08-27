# Nodemod
This is an Metamod plugin for integrating Node.js directly into Half-Life 1 engine. Plugin is intended as a more functional and convenient replacement for AMX Mod X or LuaMod. Also, it is compatible with Xash3D FWGS too.

## Directory structure
```
|-- /
|-- deps                       # 3rdparty dependencies
|-- scripts                    # auxiliary scripts (generating the autocode)
|-- src                        # nodemod source files
|  |-- auto                    # auto-generated code
|  |  |-- dll_events.cpp       # events from gamedll
|  |  |-- engine_events.cpp    # events from engine
|  |  |-- engine_functions.cpp # bindings to engine methods
|  |-- lib                     # nodemod library external api to metamod
|  |-- node                    # v8 and nodejs internal code and api
|  |-- structures              # structures between engine and jscode
|  |-- bindings                # util methods and core functional, passed to JS
```

## Compilation
> Disclaimer: for now, Nodemod locked for only x86 architecture. In future we plans to support other architectures too.

Pre-installing dependencies
```
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install gcc-multilib g++-multilib cmake ninja-build
```
  
Building instruction
```
git clone --recursive https://github.com/TheEVolk/xash3d-nodemod.git
cd xash3d-nodemod
cmake -E make_directory build
cd build
cmake .. --preset linux-x86-debug
cmake --build . --config Debug
```

## Installation and launch
> ⚠️ We are planning to move examples to another repository

1. You should make sure that Metamod is already installed on your server
2. Create a `nodemod` directory in `addons`
3. Move all files from `/example` to `addons/nodemod`
4. Create the `addons/nodemod/dlls` directory
5. Move the compiled *libnodemod.so* to the `addons/nodemod/dlls` directory
6. In plugins.ini from metamod, add the line `linux addons/nodemod/dlls/libnodemod.so`
7. Install npm or yarn and run the command `npm i` for npm or `yarn` for yarn in the `addons/nodemod` directory
8. You can place your scripts in the `addons/nodemod/src` directory.

## TypeScript
You can run your TypeScript code using the [ts-node](https://www.npmjs.com/package/ts-node) library.

1. Install ts-node: `npm i ts-node`
2. Pass there env variable to your start script: `export NODE_OPTIONS="--loader ts-node/esm"`
3. Make sure that the main field is in your package.json leads to a TS file

## RoadMap
- Fix all existing crashes.
- Write a `@types/gs-nodemod` library, for easy coding on typescript.

## Credits
- [iAmir (Amyr Aahmady)](https://github.com/AmyrAhmady) for his [samp-node project](https://github.com/AmyrAhmady/samp-node).
- [TheEVolk (Maksim Nikiforov)](https://github.com/theevolk) that's me.
- [SNMetamorph](https://github.com/SNMetamorph) for moving code to C++17 and build system refactoring


LD_LIBRARY_PATH=. valgrind --tool=memcheck --leak-check=full --error-exitcode=0 ./hlds_linux -game ts +map ts_lobby
LD_LIBRARY_PATH=. gdb --args ./hlds_linux -game ts +map ts_lobby
(gdb) set environment MALLOC_CHECK_ = 1
(gdb) handle SIGBUS stop print
(gdb) handle SIGSEGV stop print
(gdb) set logging file debug.log
(gdb) set logging on