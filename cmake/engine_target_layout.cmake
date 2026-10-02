set(SRC_ROOT_DIR "${CMAKE_SOURCE_DIR}")

set(SRC_BUILD_DATA_DIR "${SRC_ROOT_DIR}/__buildData")

if(CMAKE_CONFIGURATION_TYPES)
    # Visual Studio / Xcode / Ninja Multi-Config
    set(SRC_CONFIG_NAME "$<CONFIG>")
    set(SRC_DEBUG_SUFFIX "$<$<CONFIG:Debug>:d>")
else()
    # Ninja / Makefiles
    set(SRC_CONFIG_NAME "${CMAKE_BUILD_TYPE}")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SRC_DEBUG_SUFFIX "d")
    else()
        set(SRC_DEBUG_SUFFIX "")
    endif()
endif()

set(SRC_PLATFORM "${KRYSTALLIC_PLATFORM}${SRC_DEBUG_SUFFIX}")

set(SRC_ENG_VER_V1 "v${KRYSTALLIC_STAGE_MAJOR}.${KRYSTALLIC_STAGE_MINOR}.${KRYSTALLIC_VERSION_MAJ}.${KRYSTALLIC_VERSION_MIN}.${KRYSTALLIC_VERSION_REV}_b${KRYSTALLIC_BUILD_NUMBER}")
set(SRC_ENG_VER_V2 "v${KRYSTALLIC_STAGE_MAJOR}.${KRYSTALLIC_STAGE_MINOR}.${KRYSTALLIC_VERSION_MAJ}.${KRYSTALLIC_VERSION_MIN}.${KRYSTALLIC_VERSION_REV}_b${KRYSTALLIC_BUILD_NUMBER}.${KRYSTALLIC_BUILD_TIMESTAMP_SHORT}")

if(KRYSTALLIC_USE_TIMESTAMP_IN_INSTALL_DIR)
    set(SRC_ENG_VER ${SRC_ENG_VER_V2})
else()
    set(SRC_ENG_VER ${SRC_ENG_VER_V1})
endif()


set(SRC_FINAL_ENGINE_DIR "${SRC_ROOT_DIR}/build/${SRC_ENG_VER}/${SRC_PLATFORM}")

message(STATUS "SRC_PLATFORM=${SRC_PLATFORM}")
message(STATUS "SRC_ENG_VER=${SRC_ENG_VER}")
message(STATUS "SRC_FINAL_ENGINE_DIR=${SRC_FINAL_ENGINE_DIR}")

# function(SetupTargetLayout TARGET_NAME)

# set(TARGET_BUILD_DIR
# "${SRC_BUILD_DATA_DIR}/${TARGET_NAME}_$<CONFIG>_${SRC_PLATFORM}" )

# set_target_properties(${TARGET_NAME} PROPERTIES

# RUNTIME_OUTPUT_DIRECTORY "${TARGET_BUILD_DIR}"

# LIBRARY_OUTPUT_DIRECTORY "${TARGET_BUILD_DIR}"

# ARCHIVE_OUTPUT_DIRECTORY "${TARGET_BUILD_DIR}"

# PDB_OUTPUT_DIRECTORY "${TARGET_BUILD_DIR}"

# COMPILE_PDB_OUTPUT_DIRECTORY "${TARGET_BUILD_DIR}" )

# if(MSVC) set_target_properties(${TARGET_NAME} PROPERTIES VS_GLOBAL_IntDir
# "${SRC_BUILD_DATA_DIR}/${TARGET_NAME}_$(Configuration)_$(PlatformShortName)/objs/"
# ) endif()

# endfunction()

option(KRYSTALLIC_ENGINE_INSTALL_LAYOUT
        "Install targets into Krystallic engine runtime layout" ON)

function(SetEngineTargetLayout target)
    set(TARGET_TEMP_DIR
        "${SRC_BUILD_DATA_DIR}/${target}_${CMAKE_CXX_COMPILER_ID}_${SRC_CONFIG_NAME}_${SRC_PLATFORM}")

    set_target_properties(
        ${target}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_TEMP_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${TARGET_TEMP_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY "${TARGET_TEMP_DIR}"
            PDB_OUTPUT_DIRECTORY "${TARGET_TEMP_DIR}"
            COMPILE_PDB_OUTPUT_DIRECTORY "${TARGET_TEMP_DIR}"
    )

    if(MSVC)
        set_target_properties(
            ${target}
            PROPERTIES
                VS_GLOBAL_IntDir
                "${SRC_BUILD_DATA_DIR}/${target}_${CMAKE_CXX_COMPILER_ID}_$(Configuration)_$(PlatformShortName)/objs/"
        )
    endif()

    if(KRYSTALLIC_ENGINE_INSTALL_LAYOUT)
        install(
            TARGETS ${target}
            RUNTIME DESTINATION "${SRC_FINAL_ENGINE_DIR}"
            LIBRARY DESTINATION "${SRC_FINAL_ENGINE_DIR}"
            ARCHIVE DESTINATION "${SRC_FINAL_ENGINE_DIR}")
    else()
        install(
            TARGETS ${target}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
    endif()

endfunction()
