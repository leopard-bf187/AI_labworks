cmake_minimum_required(VERSION 3.16)

project(AI_Lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Preserve the engine's platform, build configuration, version and output layout.
option(KRYSTALLIC_USE_TIMESTAMP_IN_INSTALL_DIR "Include the build timestamp in the install directory" ON)

include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/platform.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_build_config.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_settings.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/krystallic_version.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_target_layout.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_defines.cmake")

# The engine itself is prebuilt; only the laboratory application is compiled.
add_subdirectory("${CMAKE_SOURCE_DIR}/src_lab4/src")
