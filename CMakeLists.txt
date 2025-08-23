cmake_minimum_required(VERSION 3.21)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(VcpkgIntegration)
project(nodemod)
include(CompilerRuntime)


list(APPEND SOURCES_LIST
	"src/lib/dllapi.cpp"
	"src/lib/engine_api.cpp"
	"src/lib/h_export.cpp"
	"src/lib/meta_api.cpp"
	"src/lib/public.cpp"
	"src/node/nodeimpl.cpp"
	"src/node/uvloop.cpp"
	"src/node/resource.cpp"
	"src/node/events.cpp"
	"src/bindings/functions.cpp"
	"src/bindings/bindings.cpp"
	"src/bindings/players.cpp"
	"src/common/logger.cpp"
	"src/auto/dll_events.cpp"
	"src/auto/dll_functions.cpp"
	"src/auto/engine_events.cpp"
	"src/auto/engine_functions.cpp"
	"src/structures/entity.cpp"
	"src/structures/trace_result.cpp"
	"src/structures/entvars.cpp"
	"src/structures/cvar.cpp"
	"src/structures/usercmd.cpp"
	"src/structures/clientdata.cpp"
	"src/structures/customization.cpp"
	"src/structures/delta.cpp"
	"src/structures/entitystate.cpp"
	"src/structures/keyvaluedata.cpp"
	"src/structures/netadr.cpp"
	"src/structures/playermove.cpp"
	"src/structures/saverestoredata.cpp"
	"src/structures/typedescription.cpp"
	"src/structures/weapondata.cpp"
)

# required for proper plugin loading from metamod, don't touch
if(WIN32)
	list(APPEND SOURCES_LIST "src/nodemod.def")
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES_LIST})

find_path(HLSDK_DIRECTORY "cl_dll/GameStudioModelRenderer.h" PATH_SUFFIXES "hlsdk")
find_path(METAMOD_DIRECTORY "common/BaseSystemModule.h" PATH_SUFFIXES "metamod")

target_include_directories(${PROJECT_NAME} PRIVATE
	"${HLSDK_DIRECTORY}/common"
	"${HLSDK_DIRECTORY}/dlls"
	"${HLSDK_DIRECTORY}/engine"
	"${HLSDK_DIRECTORY}/game_shared"
	"${HLSDK_DIRECTORY}/pm_shared"
	"${HLSDK_DIRECTORY}/public"
	"${METAMOD_DIRECTORY}"
	"src"
	"deps/node/src"
	"deps/node/deps/v8/include"
    "deps/node/deps/uv/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	OUTPUT_NAME "nodemod"
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

# compiler options
if(WIN32)
	target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		_CRT_SECURE_NO_WARNINGS=1 # disable CRT warnings
	)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wfatal-errors)
	target_compile_options(${PROJECT_NAME} PRIVATE -fpermissive)

	# these will break compatibility with other architectures
	# but for now it's required for our purposes
	target_compile_options(${PROJECT_NAME} PRIVATE -m32)
	target_link_options(${PROJECT_NAME} PRIVATE -m32)
endif()

# link dependency libraries
# apply hacky workaround to bypass wrong CMAKE_SIZEOF_VOID_P value
set(BACKUP_CMAKE_SIZEOF_VOID_P "${CMAKE_SIZEOF_VOID_P}")
set(CMAKE_SIZEOF_VOID_P "")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

set(CMAKE_SIZEOF_VOID_P "${BACKUP_CMAKE_SIZEOF_VOID_P}")

# link platform-specific libraries
if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

# link Node.js library statically (in future this should be entirely handled by vcpkg)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/node/lib/Release/win32/libnode.lib")
else()
	# Define base directory for Node.js fat archives
	set(LIBNODE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/node/out/Release/obj.target")
	
	# Link Node.js using Half-Life 1 metamod approach with static groups
	target_link_libraries(${PROJECT_NAME} PRIVATE
		-Wl,--start-group
		# Node.js snapshot object file (contains GetEmbeddedSnapshotData)
		"${LIBNODE_DIR}/node/gen/node_snapshot.o"
		# Following HL1 metamod fat archives order
		"${LIBNODE_DIR}/deps/histogram/libhistogram_fat.a"
		"${LIBNODE_DIR}/deps/ncrypto/libncrypto_fat.a"
		"${LIBNODE_DIR}/deps/ada/libada_fat.a"
		"${LIBNODE_DIR}/deps/nbytes/libnbytes_fat.a"
		"${LIBNODE_DIR}/deps/llhttp/libllhttp_fat.a"
		"${LIBNODE_DIR}/deps/brotli/libbrotli_fat.a"
		"${LIBNODE_DIR}/deps/nghttp2/libnghttp2_fat.a"
		"${LIBNODE_DIR}/deps/cares/libcares_fat.a"
		"${LIBNODE_DIR}/deps/simdjson/libsimdjson_fat.a"
		"${LIBNODE_DIR}/deps/uvwasi/libuvwasi_fat.a"
		"${LIBNODE_DIR}/deps/sqlite/libsqlite_fat.a"
		"${LIBNODE_DIR}/deps/openssl/libopenssl_fat.a"
		"${LIBNODE_DIR}/deps/zlib/libzlib_fat.a"
		"${LIBNODE_DIR}/deps/zlib/libzlib_adler32_simd_fat.a"
		"${LIBNODE_DIR}/deps/zlib/libzlib_data_chunk_simd_fat.a"
		"${LIBNODE_DIR}/deps/uv/libuv_fat.a"
		"${LIBNODE_DIR}/deps/zstd/libzstd_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libsimdutf_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libhighway_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_init_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_initializers_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libtorque_base_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libabseil_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_zlib_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_base_without_compiler_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_initializers_slow_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_snapshot_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_libbase_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_libplatform_fat.a"
		"${LIBNODE_DIR}/tools/v8_gypfiles/libv8_compiler_fat.a"
		"${LIBNODE_DIR}/tools/icu/libicui18n_fat.a"
		"${LIBNODE_DIR}/tools/icu/libicuucx_fat.a"
		"${LIBNODE_DIR}/tools/icu/libicudata_fat.a"
		"${LIBNODE_DIR}/deps/inspector_protocol/libcrdtp_fat.a"
		"${LIBNODE_DIR}/libnode_fat.a"
		-Wl,--end-group
	)
endif()

# enable static runtime linking
set_compiler_runtime(${PROJECT_NAME} STATIC)

# set build output directory
set(DIR_COMMON_OUTPUT 
	${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
	LIBRARY_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
	RUNTIME_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
)

install(TARGETS ${PROJECT_NAME}
	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	PERMISSIONS 
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
	    GROUP_READ GROUP_EXECUTE
	    WORLD_READ WORLD_EXECUTE 
)
