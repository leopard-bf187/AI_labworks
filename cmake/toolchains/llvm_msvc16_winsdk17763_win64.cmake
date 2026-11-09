set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

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

if(CMAKE_HOST_WIN32)
    set(_TOOL_SUFFIX ".exe")
else()
    set(_TOOL_SUFFIX "")
endif()

set(CMAKE_C_COMPILER "${LLVM_ROOT}/bin/clang-cl${_TOOL_SUFFIX}")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang-cl${_TOOL_SUFFIX}")
set(CMAKE_LINKER "${LLVM_ROOT}/bin/lld-link${_TOOL_SUFFIX}")
set(CMAKE_MT "${LLVM_ROOT}/bin/llvm-mt${_TOOL_SUFFIX}")

# ВАЖНО: заставляем clang-cl собирать именно Win64/x64
set(CMAKE_C_COMPILER_TARGET x86_64-pc-windows-msvc)
set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc)

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
    "${MSVC_ROOT}/lib/x64" 
    "${WINSDK_LIB}/ucrt/x64" 
    "${WINSDK_LIB}/um/x64"
)

set(CMAKE_EXE_LINKER_FLAGS_INIT "/machine:X64")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "/machine:X64")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "/machine:X64")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_compile_options(/EHsc /Zc:__cplusplus /permissive-)
