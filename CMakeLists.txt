cmake_minimum_required(VERSION 3.10)

project("Krystallic Engine MSU" LANGUAGES C CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(KRYSTALLIC_BUILD_ENGINE "" ON)
option(KRYSTALLIC_BUILD_SANDBOX "" ON)
option(KRYSTALLIC_BUILD_SANDBOX_ANDROID_APK "" ON)
option(KRYSTALLIC_USE_TIMESTAMP_IN_INSTALL_DIR "" OFF)
option(KRYSTALLIC_BUILD_TESTS "" OFF)

option(KRYSTALLIC_CORE_ASSET_SYSTEM "Build Core Asset System" ON)
option(KRYSTALLIC_CORE_FONT_API "Build Core Font API" ON)
option(KRYSTALLIC_CORE_GUI "Build Core GUI System" ON)
option(KRYSTALLIC_CORE_LOG_API "Build Core Log API" ON)
option(KRYSTALLIC_CORE_MATERIAL_SYSTEM "Build Core Material System" ON)
option(KRYSTALLIC_CORE_MESH_API "Build Core Mesh API" ON)
option(KRYSTALLIC_CORE_PARTICLE_SYSTEM "Build Core Particle System" ON)
option(KRYSTALLIC_CORE_PREFAB_SYSTEM "Build Core Prefab System" ON)
option(KRYSTALLIC_CORE_PROFILER_API "Build Core Profiler API" ON)
option(KRYSTALLIC_CORE_RENDER_GRAPH "Build Core Render Graph" ON)
option(KRYSTALLIC_CORE_SCENE_MANAGER "Build Core Scene Manager" ON)
option(KRYSTALLIC_CORE_SCRIPT_SYSTEM "Build Core Script System" ON)
option(KRYSTALLIC_CORE_SOUND_API "Build Core Sound API" ON)
option(KRYSTALLIC_CORE_SPRITE_API "Build Core Sprite API" ON)
option(KRYSTALLIC_CORE_TEXTURE_API "Build Core Texture API" ON)
option(KRYSTALLIC_CORE_VIDEO_API "Build Core Video API" ON)

include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/platform.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/krystallic_version.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_target_layout.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_settings.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/engine_project_defines.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/android_apk.cmake")


if(KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_LINUX" OR KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_ANDROID")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()


add_subdirectory(src_engine)

if(KRYSTALLIC_BUILD_SANDBOX)
	add_subdirectory(src_sbox)
endif()

if(KRYSTALLIC_BUILD_TESTS)
	add_subdirectory(third_party/datadriven)
	add_subdirectory(src_tests)
endif()
