set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR ARM64)

foreach(v LLVM_ROOT SDK_ROOT MSVC_ROOT SDK_VER)
    if(NOT DEFINED ENV{${v}})
        message(FATAL_ERROR "${v} env var is not set. Call localdevenv.bat first.")
    endif()
endforeach()

function(to_cmake_path in_var out_var)
    file(TO_CMAKE_PATH "${in_var}" _p)
    set(${out_var} "${_p}" PARENT_SCOPE)
endfunction()

to_cmake_path("$ENV{LLVM_ROOT}" LLVM_ROOT)
to_cmake_path("$ENV{SDK_ROOT}" SDK_ROOT)
to_cmake_path("$ENV{MSVC_ROOT}" MSVC_ROOT)

set(SDK_VER "$ENV{SDK_VER}")

set(CMAKE_C_COMPILER "${LLVM_ROOT}/bin/clang-cl.exe")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang-cl.exe")
set(CMAKE_LINKER "${LLVM_ROOT}/bin/lld-link.exe")
set(CMAKE_MT "${LLVM_ROOT}/bin/llvm-mt.exe")

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64|x64")
    set(HOST_TOOLS_ARCH "x64")
else()
    set(HOST_TOOLS_ARCH "x86")
endif()

set(CMAKE_RC_COMPILER
    "${SDK_ROOT}/bin/${SDK_VER}/${HOST_TOOLS_ARCH}/rc.exe"
    CACHE FILEPATH "Windows resource compiler" FORCE
)

# ВАЖНО: заставляем clang-cl собирать именно Windows ARM64
set(CMAKE_C_COMPILER_TARGET aarch64-pc-windows-msvc)
set(CMAKE_CXX_COMPILER_TARGET aarch64-pc-windows-msvc)

set(WINSDK_INC "${SDK_ROOT}/Include/${SDK_VER}")
set(WINSDK_LIB "${SDK_ROOT}/Lib/${SDK_VER}")

include_directories(
    SYSTEM
        "${MSVC_ROOT}/include"
        "${WINSDK_INC}/ucrt"
        "${WINSDK_INC}/um"
        "${WINSDK_INC}/shared"
        "${WINSDK_INC}/winrt"
)

link_directories(
    "${MSVC_ROOT}/lib/arm64"
    "${WINSDK_LIB}/ucrt/arm64"
    "${WINSDK_LIB}/um/arm64"
)

set(CMAKE_EXE_LINKER_FLAGS_INIT "/machine:ARM64")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "/machine:ARM64")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "/machine:ARM64")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM$|arm$")
    add_compile_options(/EHs- /EHc- -clang:-fno-exceptions)
else()
    add_compile_options(/EHsc)
endif()

add_compile_options(/Zc:__cplusplus /permissive-)