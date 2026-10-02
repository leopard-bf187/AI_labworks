set(KRYSTALLIC_STAGE_MAJOR "0")
set(KRYSTALLIC_STAGE_MINOR "2")
set(KRYSTALLIC_VERSION_MAJ "11")
set(KRYSTALLIC_VERSION_MIN "20")
set(KRYSTALLIC_VERSION_REV "71")
set(KRYSTALLIC_CODENAME "Neural Emerald")

set(KRYSTALLIC_BUILD_EPOCH "2025-12-05")

set(_SOURCE_DATE_EPOCH_BACKUP "$ENV{SOURCE_DATE_EPOCH}")
unset(ENV{SOURCE_DATE_EPOCH})

string(TIMESTAMP KRYSTALLIC_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
string(TIMESTAMP KRYSTALLIC_BUILD_TIMESTAMP_SHORT "%y%m%d-%H%M")

message(STATUS "KRYSTALLIC_BUILD_TIMESTAMP=${KRYSTALLIC_BUILD_TIMESTAMP}")
message(STATUS "KRYSTALLIC_BUILD_TIMESTAMP_SHORT=${KRYSTALLIC_BUILD_TIMESTAMP_SHORT}")

if(NOT "${_SOURCE_DATE_EPOCH_BACKUP}" STREQUAL "")
    set(ENV{SOURCE_DATE_EPOCH} "${_SOURCE_DATE_EPOCH_BACKUP}")
endif()



set(KRYSTALLIC_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if(KRYSTALLIC_STAGE_MAJOR EQUAL 0)
    if(KRYSTALLIC_STAGE_MINOR LESS_EQUAL 5)
        set(KRYSTALLIC_STAGE_NAME "InDev")
    elseif(KRYSTALLIC_STAGE_MINOR LESS_EQUAL 7)
        set(KRYSTALLIC_STAGE_NAME "Alpha")
    elseif(KRYSTALLIC_STAGE_MINOR EQUAL 8)
        set(KRYSTALLIC_STAGE_NAME "Beta")
    elseif(KRYSTALLIC_STAGE_MINOR EQUAL 9)
        set(KRYSTALLIC_STAGE_NAME "RC")
    else()
        message(FATAL_ERROR "Invalid Krystallic development stage: ${KRYSTALLIC_STAGE_MAJOR}.${KRYSTALLIC_STAGE_MINOR}")
    endif()
elseif(KRYSTALLIC_STAGE_MAJOR GREATER_EQUAL 1)
    set(KRYSTALLIC_STAGE_NAME "RTM")
else()
    message(FATAL_ERROR "Invalid Krystallic development stage: ${KRYSTALLIC_STAGE_MAJOR}.${KRYSTALLIC_STAGE_MINOR}")
endif()

string(TOLOWER "${KRYSTALLIC_STAGE_NAME}" KRYSTALLIC_STAGE_NAME_SHORT)

find_program(
    LUA_EXE
    NAMES 
        lua.exe
        lua
        lua55.exe
        lua55
	lua5.5
        lua54.exe
        lua54
	lua5.4
        lua53.exe
        lua53
	lua5.3
        lua52.exe
        lua52
	lua5.2
        lua51.exe
        lua51
	lua5.1
        luajit.exe
        luajit
    PATHS "${KRYSTALLIC_UTILS_DIR}"
    NO_DEFAULT_PATH)

if(NOT LUA_EXE)
    find_program(
        LUA_EXE
        NAMES 
            lua.exe
            lua
            lua55.exe
            lua55
	    lua5.5
            lua54.exe
            lua54
	    lua5.4
            lua53.exe
            lua53
	    lua5.3
            lua52.exe
            lua52
	    lua5.2
            lua51.exe
            lua51
	    lua5.1
            luajit.exe
            luajit)
endif()

message(STATUS "KRYSTALLIC_UTILS_DIR = ${KRYSTALLIC_UTILS_DIR}")
message(STATUS "LUA_EXE = ${LUA_EXE}")

if(LUA_EXE)
    execute_process(
        COMMAND "${LUA_EXE}" "${CMAKE_SOURCE_DIR}/cmake/krystallic_engine_build_number.lua" "${KRYSTALLIC_BUILD_EPOCH}"
        OUTPUT_VARIABLE KRYSTALLIC_BUILD_NUMBER
        ERROR_VARIABLE KRYSTALLIC_BUILD_ERROR RESULT_VARIABLE
        KRYSTALLIC_BUILD_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
endif()

message(STATUS "KRYSTALLIC_BUILD_RESULT = ${KRYSTALLIC_BUILD_RESULT}")
message(STATUS "KRYSTALLIC_BUILD_NUMBER = ${KRYSTALLIC_BUILD_NUMBER}")
message(STATUS "KRYSTALLIC_BUILD_ERROR = ${KRYSTALLIC_BUILD_ERROR}")

# if(NOT KRYSTALLIC_BUILD_RESULT EQUAL 0) message(FATAL_ERROR "Lua build number
# script failed: ${KR_BUILD_ERROR}") endif()

# if(KRYSTALLIC_BUILD_NUMBER STREQUAL "") message(FATAL_ERROR "Lua build number
# script returned empty output") set(KRYSTALLIC_BUILD_NUMBER "0") endif()

if(NOT KRYSTALLIC_BUILD_NUMBER)
    set(KRYSTALLIC_BUILD_NUMBER "0")
endif()

configure_file("${CMAKE_SOURCE_DIR}/cmake/__config.h.in" "${CMAKE_SOURCE_DIR}/public/__config.h" @ONLY)
