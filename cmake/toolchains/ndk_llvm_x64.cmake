# Wrapper around the official Android NDK CMake toolchain. Requires
# ANDROID_NDK_ROOT or ANDROID_NDK_HOME.

if(NOT DEFINED ANDROID_NDK_ROOT AND DEFINED ENV{ANDROID_NDK_ROOT})
  set(ANDROID_NDK_ROOT
      "$ENV{ANDROID_NDK_ROOT}"
      CACHE PATH "Android NDK root")
endif()

if(NOT DEFINED ANDROID_NDK_ROOT AND DEFINED ENV{ANDROID_NDK_HOME})
  set(ANDROID_NDK_ROOT
      "$ENV{ANDROID_NDK_HOME}"
      CACHE PATH "Android NDK root")
endif()

if(NOT DEFINED ANDROID_NDK_ROOT)
  message(
    FATAL_ERROR
      "ANDROID_NDK_ROOT is not set. Use scripts/android/flake.nix or export Android NDK env before CMake configure."
  )
endif()

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(ANDROID_ABI
    x86_64
    CACHE STRING "Android ABI")

set(ANDROID_PLATFORM
    android-24
    CACHE STRING "Android API level")

set(ANDROID_STL
    c++_shared
    CACHE STRING "Android STL")

set(ANDROID_CPP_FEATURES
    "rtti exceptions"
    CACHE STRING "Android C++ features")

include("${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake")
