function(SetTargetDependencies target)

    set(argOptions)

    set(argOneValueArgs)

    set(argMultiValueArgs
        PRIVATE_INCLUDES
        PUBLIC_INCLUDES

        PRIVATE_DEPS
        PUBLIC_DEPS

        PRIVATE_3DPARTY
        PUBLIC_3DPARTY

        BUILD_DEPS
    )

    cmake_parse_arguments(PROJ
        "${argOptions}"
        "${argOneValueArgs}"
        "${argMultiValueArgs}"
        ${ARGN}
    )

    if(NOT TARGET ${target})
        message(FATAL_ERROR "Target '${target}' does not exist")
    endif()

    set(localPrivateDeps)
    set(localPublicDeps)
    set(localBuildDeps)

    foreach(dep ${PROJ_PRIVATE_DEPS})
        if(TARGET ${dep})
            list(APPEND localPrivateDeps ${dep})
        else()
            message(STATUS "Target '${target}': private dependency '${dep}' skipped")
        endif()
    endforeach()

    foreach(dep ${PROJ_PUBLIC_DEPS})
        if(TARGET ${dep})
            list(APPEND localPublicDeps ${dep})
        else()
            message(STATUS "Target '${target}': public dependency '${dep}' skipped")
        endif()
    endforeach()

    foreach(dep ${PROJ_BUILD_DEPS})
        if(TARGET ${dep})
            list(APPEND localBuildDeps ${dep})
        else()
            message(STATUS "Target '${target}': build dependency '${dep}' skipped")
        endif()
    endforeach()

    if(PROJ_PRIVATE_INCLUDES)
        target_include_directories(${target}
            PRIVATE
                ${PROJ_PRIVATE_INCLUDES}
        )
    endif()

    if(KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_ANDROID")
        target_include_directories(${target} PRIVATE
            ${ANDROID_NDK_ROOT}/sources/android/native_app_glue
        )
    endif()

    if(PROJ_PUBLIC_INCLUDES)
        target_include_directories(${target}
            PUBLIC
                ${PROJ_PUBLIC_INCLUDES}
        )
    endif()

    if(localPrivateDeps)
        target_link_libraries(${target}
            PRIVATE
                ${localPrivateDeps}
        )
    endif()

    if(localPublicDeps)
        target_link_libraries(${target}
            PUBLIC
                ${localPublicDeps}
        )
    endif()

    if(PROJ_PRIVATE_3DPARTY)
        target_link_libraries(${target}
            PRIVATE
                ${PROJ_PRIVATE_3DPARTY}
        )
    endif()

    if(PROJ_PUBLIC_3DPARTY)
        target_link_libraries(${target}
            PUBLIC
                ${PROJ_PUBLIC_3DPARTY}
        )
    endif()

    if(localBuildDeps)
        add_dependencies(${target}
            ${localBuildDeps}
        )
    endif()

    set(localTrackedDeps ${localPrivateDeps} ${localPublicDeps} ${localBuildDeps})

    foreach(dep ${PROJ_PRIVATE_3DPARTY} ${PROJ_PUBLIC_3DPARTY})
        if(TARGET ${dep})
            list(APPEND localTrackedDeps ${dep})
        endif()
    endforeach()

    if(localTrackedDeps)
        set_property(TARGET ${target} APPEND PROPERTY KRYSTALLIC_TARGET_DEPENDENCIES ${localTrackedDeps})
    endif()

endfunction()



function(SetImportDependicies target)
    set(platformName "${KRYSTALLIC_PLATFORM}$<$<CONFIG:Debug>:d>")

    target_include_directories(${target} PRIVATE "${CMAKE_SOURCE_DIR}/import/inc")
    target_link_directories(${target} PRIVATE "${CMAKE_SOURCE_DIR}/import/lib/${platformName}")
endfunction()



set(ENGINE_SIMD "AUTO" CACHE STRING "SIMD instruction set")

set(allowedSimdValues
    AUTO
    NONE
    SSE42
    AVX
    AVX2
    NEON
)


set_property(CACHE ENGINE_SIMD PROPERTY STRINGS ${allowedSimdValues})


if(NOT ENGINE_SIMD IN_LIST allowedSimdValues)
    message(FATAL_ERROR "Invalid ENGINE_SIMD value: ${ENGINE_SIMD}")
endif()



function(DetectSimd outSimd)

    if(NOT ENGINE_SIMD STREQUAL "AUTO")
        set(${outSimd} "${ENGINE_SIMD}" PARENT_SCOPE)
        return()
    endif()

    if(KRYSTALLIC_PLATFORM STREQUAL "Win32")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "Win64")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "WinArm")
        set(${outSimd} "NEON" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "WinArm64")
        set(${outSimd} "NEON" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "Linux32")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "Linux64")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "LinuxArm64")
        set(${outSimd} "NEON" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "AndroidArm64")
        set(${outSimd} "NEON" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "AndroidArm7")
        set(${outSimd} "NEON" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "AndroidX64")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    elseif(KRYSTALLIC_PLATFORM STREQUAL "AndroidX86")
        set(${outSimd} "SSE42" PARENT_SCOPE)

    else()
        set(${outSimd} "NONE" PARENT_SCOPE)
    endif()

endfunction()



function(DetectSimdDefine simd outDefine)

    if("${simd}" STREQUAL "SSE42")
        set(${outDefine} "SIMD_SSE42" PARENT_SCOPE)

    elseif("${simd}" STREQUAL "AVX")
        set(${outDefine} "SIMD_AVX" PARENT_SCOPE)

    elseif("${simd}" STREQUAL "AVX2")
        set(${outDefine} "SIMD_AVX2" PARENT_SCOPE)

    elseif("${simd}" STREQUAL "NEON")
        set(${outDefine} "SIMD_ARM_NEON" PARENT_SCOPE)

    else()
        set(${outDefine} "SIMD_NONE" PARENT_SCOPE)
    endif()

endfunction()


DetectSimd(KRYSTALLIC_SIMD)
DetectSimdDefine("${KRYSTALLIC_SIMD}" KRYSTALLIC_SIMD_DEFINE)

message(STATUS "KRYSTALLIC_PLATFORM=${KRYSTALLIC_PLATFORM}")
message(STATUS "KRYSTALLIC_SIMD=${KRYSTALLIC_SIMD}")
message(STATUS "KRYSTALLIC_SIMD_DEFINE=${KRYSTALLIC_SIMD_DEFINE}")


function(ApplySimd targetName)

    if(NOT TARGET ${targetName})
        message(FATAL_ERROR "Target '${targetName}' does not exist")
    endif()

    if(KRYSTALLIC_SIMD STREQUAL "NONE")
        return()
    endif()

    if(KRYSTALLIC_SIMD STREQUAL "NEON")
        if(KRYSTALLIC_PLATFORM STREQUAL "AndroidArm7")
            if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
                target_compile_options(${targetName} PRIVATE -mfpu=neon)
            endif()
        endif()

        return()
    endif()

    if(MSVC)
        if(KRYSTALLIC_SIMD STREQUAL "SSE42")
            target_compile_options(${targetName} PRIVATE /clang:-msse4.2)

        elseif(KRYSTALLIC_SIMD STREQUAL "AVX")
            target_compile_options(${targetName} PRIVATE /arch:AVX)

        elseif(KRYSTALLIC_SIMD STREQUAL "AVX2")
            target_compile_options(${targetName} PRIVATE /arch:AVX2)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(KRYSTALLIC_SIMD STREQUAL "SSE42")
            target_compile_options(${targetName} PRIVATE -msse4.2)

        elseif(KRYSTALLIC_SIMD STREQUAL "AVX")
            target_compile_options(${targetName} PRIVATE -mavx)

        elseif(KRYSTALLIC_SIMD STREQUAL "AVX2")
            target_compile_options(${targetName} PRIVATE -mavx2)
        endif()
    endif()

endfunction()