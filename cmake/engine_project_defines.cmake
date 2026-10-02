add_compile_definitions(${KRYSTALLIC_PLATFORM_DEFINE} ${KRYSTALLIC_OS_DEFINE} ${KRYSTALLIC_ARCH_DEFINE})

add_compile_definitions("$<$<CONFIG:Debug>:KRYSTALLIC_BUILD_CONFIG=\"Debug\">" "$<$<CONFIG:Debug>:KRYSTALLIC_BUILD_CONFIG_SHORT=\"dbg\">" "$<$<CONFIG:Release>:KRYSTALLIC_BUILD_CONFIG=\"Release\">" "$<$<CONFIG:Release>:KRYSTALLIC_BUILD_CONFIG_SHORT=\"rel\">" "$<$<CONFIG:RelWithDebInfo>:KRYSTALLIC_BUILD_CONFIG=\"RelWithDebInfo\">" "$<$<CONFIG:RelWithDebInfo>:KRYSTALLIC_BUILD_CONFIG_SHORT=\"rdi\">" "$<$<CONFIG:MinSizeRel>:KRYSTALLIC_BUILD_CONFIG=\"MinSizeRel\">" "$<$<CONFIG:MinSizeRel>:KRYSTALLIC_BUILD_CONFIG_SHORT=\"msr\">")

if(KRYSTALLIC_SIMD_DEFINE)
    add_compile_definitions(${KRYSTALLIC_SIMD_DEFINE})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()
