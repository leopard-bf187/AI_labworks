# AI Lab 4 is a standalone application. Krystallic is used only for
# Platform, GUI and rendering through prebuilt Windows DLLs/import libraries.

if(NOT WIN32)
    message(FATAL_ERROR "AI_Lab4: the included prebuilt Krystallic libraries are Windows-only. Add the appropriate libraries before targeting another OS.")
endif()

# The suffixes match the existing engine output layout:
# Debug -> Win64d, Checked -> Win64c, Release -> Win64 (or Win32/WinArm64).
set(LAB4_KRYSTALLIC_COMPONENTS
    core
    engine
    filesys
    platform
    render_dx11
    render_gl4
    stdlib
)

set(LAB4_KRYSTALLIC_TARGETS)

foreach(component IN LISTS LAB4_KRYSTALLIC_COMPONENTS)
    set(imported_target "Lab4_Krystallic_${component}")
    add_library(${imported_target} SHARED IMPORTED GLOBAL)
    set_property(TARGET ${imported_target} PROPERTY IMPORTED_CONFIGURATIONS Debug Checked Release)

    foreach(config IN ITEMS Debug Checked Release)
        string(TOUPPER "${config}" config_upper)
        KrystallicGetConfigSuffix("${config}" config_suffix)

        set(lib_dir "${CMAKE_SOURCE_DIR}/build/${KRYSTALLIC_PLATFORM}${config_suffix}")
        set(import_lib "${lib_dir}/${component}.lib")
        set(runtime_dll "${lib_dir}/${component}.dll")

        if(NOT EXISTS "${import_lib}" OR NOT EXISTS "${runtime_dll}")
            message(FATAL_ERROR "AI_Lab4: missing prebuilt ${component} (${config}) in ${lib_dir}")
        endif()

        set_target_properties(${imported_target} PROPERTIES
            "IMPORTED_IMPLIB_${config_upper}" "${import_lib}"
            "IMPORTED_LOCATION_${config_upper}" "${runtime_dll}"
        )
    endforeach()

    list(APPEND LAB4_KRYSTALLIC_TARGETS ${imported_target})
endforeach()

# We are still designing the lab; do not invent application source code just
# to make CMake configure. Once main.cpp is added, this becomes an executable.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_custom_target(AI_Lab4
        COMMAND "${CMAKE_COMMAND}" -E echo "AI_Lab4: CMake is ready; add src_lab4/main.cpp to build the executable."
        VERBATIM
    )
    message(STATUS "AI_Lab4: main.cpp is not present; created a placeholder build target.")
    return()
endif()

file(GLOB_RECURSE LAB4_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LAB4_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(AI_Lab4 ${LAB4_SOURCES} ${LAB4_HEADERS})

target_include_directories(AI_Lab4 PRIVATE "${CMAKE_SOURCE_DIR}/public")
target_link_libraries(AI_Lab4 PRIVATE ${LAB4_KRYSTALLIC_TARGETS})

set_target_properties(AI_Lab4 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:AI_Lab4>"
)

# Place the matching configuration's runtime DLLs next to the executable.
foreach(imported_target IN LISTS LAB4_KRYSTALLIC_TARGETS)
    add_custom_command(TARGET AI_Lab4 POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:${imported_target}>"
            "$<TARGET_FILE_DIR:AI_Lab4>"
        VERBATIM
    )
endforeach()
