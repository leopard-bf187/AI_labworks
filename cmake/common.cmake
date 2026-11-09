function(SetWarningsOld target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        target_compile_options(${target} PRIVATE /wd4100)
        target_compile_options(${target} PRIVATE /wd4706)
        target_compile_options(${target} PRIVATE /wd4201)
        if(KRYSTALLIC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        #target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(${target} PRIVATE -Wall -Wextra)
        target_compile_options(${target} PRIVATE -Wno-pedantic)
        target_compile_options(${target} PRIVATE -Wno-unused-parameter)
        target_compile_options(${target} PRIVATE -Wno-parentheses)
        target_compile_options(${target} PRIVATE -Wno-missing-braces)
        if(KRYSTALLIC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()


function(SetWarnings target)
    if(MSVC)
        # MSVC или clang-cl
        target_compile_options(${target} PRIVATE
            /W4
            /wd4100
            /wd4201
            /wd4706
        )

        if(KRYSTALLIC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wno-unused-parameter
            -Wno-parentheses
            -Wno-pedantic
            -Wno-missing-braces
        )

        if(KRYSTALLIC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang с GNU-интерфейсом
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wno-unused-parameter
            -Wno-parentheses
            -Wno-gnu-anonymous-struct
            -Wno-nested-anon-types
            -Wno-missing-braces
        )

        if(KRYSTALLIC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()

    else()
        message(WARNING "SetWarnings: unsupported compiler '${CMAKE_CXX_COMPILER_ID}' for target '${target}'")
    endif()
endfunction()


function(EnablePch target)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.16" AND EXISTS "${CMAKE_SOURCE_DIR}/include/pch.h")
        target_precompile_headers(${target} PRIVATE "${CMAKE_SOURCE_DIR}/include/pch.h")
    endif()
endfunction()

function(SetOutputName target name)
    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${name}")
endfunction()

function(SetPublicIncludes target)
    target_include_directories(${target} PUBLIC "${CMAKE_SOURCE_DIR}/public")
endfunction()

function(SetCppMode target)
    target_compile_features(${target} PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(
        ${target}
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/Zc:wchar_t>
                $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
                $<$<COMPILE_LANGUAGE:CXX>:/permissive->
                #$<$<COMPILE_LANGUAGE:CXX>:/Zc:char16_t>
                $<$<COMPILE_LANGUAGE:CXX>:/EHsc>)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # clang-cl, Windows MSVC ABI
        target_compile_options(
        ${target}
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/Zc:wchar_t>
                $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
                $<$<COMPILE_LANGUAGE:CXX>:/permissive->
                $<$<COMPILE_LANGUAGE:CXX>:/EHsc>)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Android/Linux clang++ uses GNU-style flags. Do not pass /std, /EHsc, /Zc
    # here.
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fexceptions> $<$<COMPILE_LANGUAGE:CXX>:-frtti>)
    endif()
endfunction()
