vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO rehlds/Metamod-R
    REF 603a2574e937171aa5e737bd5e7b4a07f74e88ee
    SHA512 118967cab0489be00878100c7ec5e04da84e5f43b6f6f6ffc580f8eb9684587ecd42559a164eaaf6f2e3b2ea5f4f273ad9f5b40022696988b0ce4fbedbfc3a87
    HEAD_REF 1.3.0.149
)

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
file(INSTALL 
    DIRECTORY "${SOURCE_PATH}/metamod/include/" 
    DESTINATION "${CURRENT_PACKAGES_DIR}/include/${PORT}/"
    FILES_MATCHING PATTERN "*.h")

# Install main metamod headers needed for plugin development
file(INSTALL 
    "${SOURCE_PATH}/metamod/src/meta_api.h"
    "${SOURCE_PATH}/metamod/src/dllapi.h"
    "${SOURCE_PATH}/metamod/src/engine_api.h"
    "${SOURCE_PATH}/metamod/src/h_export.h"
    DESTINATION "${CURRENT_PACKAGES_DIR}/include/${PORT}/")

# Install example headers that contain required definitions
file(INSTALL 
    DIRECTORY "${SOURCE_PATH}/metamod/extra/example/include/metamod/" 
    DESTINATION "${CURRENT_PACKAGES_DIR}/include/${PORT}/"
    FILES_MATCHING PATTERN "*.h")

file(REMOVE_RECURSE 
    "${CURRENT_PACKAGES_DIR}/include/${PORT}/metamod/msvc")
