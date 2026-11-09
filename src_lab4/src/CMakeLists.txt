# Standalone AI Lab 4 application. The Krystallic Engine itself is prebuilt.
# Its public headers are in /public and Windows import libraries/DLLs in /build.

if(NOT WIN32)
    message(FATAL_ERROR "AI_Lab4: the included prebuilt Krystallic libraries are Windows-only.")
endif()

# Debug: Win64d, Checked: Win64c, Release: Win64 (and corresponding architectures).
set(LAB4_KRYSTALLIC_COMPONENTS core engine filesys platform render_dx11 render_gl4 stdlib)
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

        set_target_properties(${imported_target} PROPERTIES "IMPORTED_IMPLIB_${config_upper}" "${import_lib}" "IMPORTED_LOCATION_${config_upper}" "${runtime_dll}")
    endforeach()

    list(APPEND LAB4_KRYSTALLIC_TARGETS ${imported_target})
endforeach()

# Keep the project configurable while the application is still being designed.
# The placeholder becomes a real executable once main.cpp has been added.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_custom_target(AI_Lab4 COMMAND "${CMAKE_COMMAND}" -E echo "AI_Lab4: add src_lab4/main.cpp to build the executable." VERBATIM)
    message(STATUS "AI_Lab4: main.cpp is not present; created a placeholder build target.")
    return()
endif()

file(GLOB_RECURSE LAB4_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LAB4_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(AI_Lab4 ${LAB4_SOURCES} ${LAB4_HEADERS})
SetCppMode(AI_Lab4)
SetWarnings(AI_Lab4)
SetEngineTargetLayout(AI_Lab4)

target_include_directories(AI_Lab4 PRIVATE "${CMAKE_SOURCE_DIR}/public" "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(AI_Lab4 PRIVATE ${LAB4_KRYSTALLIC_TARGETS})

# Both IDE launches and installed builds use resources relative to the executable.
set_target_properties(AI_Lab4 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:AI_Lab4>")

foreach(imported_target IN LISTS LAB4_KRYSTALLIC_TARGETS)
    add_custom_command(TARGET AI_Lab4 POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${imported_target}>" "$<TARGET_FILE_DIR:AI_Lab4>" VERBATIM)
    install(FILES "$<TARGET_FILE:${imported_target}>" DESTINATION "${SRC_FINAL_ENGINE_DIR}")
endforeach()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SBoxData")
    add_custom_command(TARGET AI_Lab4 POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/SBoxData" "$<TARGET_FILE_DIR:AI_Lab4>/SBoxData" VERBATIM)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SBoxData" DESTINATION "${SRC_FINAL_ENGINE_DIR}")
endif()
