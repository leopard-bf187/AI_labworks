add_library(stdlib SHARED
    std_dllmain.cpp
    std_Allocator.cpp
    std_String.cpp
    std_StringU16.cpp
    std_StringU32.cpp
    std_StringView.cpp
    std_string_utils.cpp
    std_AtomicCounter_linux.cpp
    std_AtomicCounter_win32.cpp
    std_dig_to_str.cpp
    std_hex_to_str.cpp
    std_str_to_dig.cpp
)

set_target_properties(stdlib PROPERTIES FOLDER "Engine")

SetWarnings(stdlib)
SetCppMode(stdlib)
SetOutputName(stdlib "stdlib")
SetEngineTargetLayout(stdlib)

# target_compile_definitions(stdlib PRIVATE KRYSTALLIC_API_EXPORT)


SetTargetDependencies(stdlib
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_SOURCE_DIR}/src_engine/stdlib
        ${CMAKE_SOURCE_DIR}/src_engine/__shared

    PUBLIC_DEPS

    PRIVATE_DEPS

    PRIVATE_3DPARTY

    BUILD_DEPS
)