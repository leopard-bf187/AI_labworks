include("${CMAKE_SOURCE_DIR}/cmake/engine_build_config.cmake")

add_compile_definitions(${KRYSTALLIC_PLATFORM_DEFINE} ${KRYSTALLIC_OS_DEFINE} ${KRYSTALLIC_ARCH_DEFINE})

add_compile_definitions("$<$<CONFIG:Debug>:BUILD_CONF_DBG>" "$<$<CONFIG:Checked>:BUILD_CONF_CHK>" "$<$<CONFIG:Release>:BUILD_CONF_REL>")

if(KRYSTALLIC_SIMD_DEFINE)
    add_compile_definitions(${KRYSTALLIC_SIMD_DEFINE})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()
