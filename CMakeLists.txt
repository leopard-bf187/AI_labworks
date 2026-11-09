cmake_minimum_required(VERSION 3.16)

project(AI_Lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Reuse the public ABI, platform/configuration names and generated __config.h
# from the prebuilt Krystallic Engine. Engine sources are not part of this repo.
include("${CMAKE_SOURCE_DIR}/cmake/platform.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_build_config.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_settings.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/krystallic_version.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_defines.cmake")

# The only application target in this repository.
add_subdirectory(src_lab4)
