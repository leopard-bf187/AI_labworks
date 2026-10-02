option(KRYSTALLIC_CORE_ENABLE_ASSET_SYSTEM     "Enable Core Asset System"     ON)
option(KRYSTALLIC_CORE_ENABLE_FONT_API         "Enable Core Font API"         ON)
option(KRYSTALLIC_CORE_ENABLE_GUI              "Enable Core GUI"              ON)
option(KRYSTALLIC_CORE_ENABLE_DEBUG_API        "Enable Core Debug API"        ON)
option(KRYSTALLIC_CORE_ENABLE_MATERIAL_SYSTEM  "Enable Core Material System"  ON)
option(KRYSTALLIC_CORE_ENABLE_MESH_API         "Enable Core Mesh API"         ON)
option(KRYSTALLIC_CORE_ENABLE_PARTICLE_SYSTEM  "Enable Core Particle System"  ON)
option(KRYSTALLIC_CORE_ENABLE_PREFAB_SYSTEM    "Enable Core Prefab System"    ON)
option(KRYSTALLIC_CORE_ENABLE_RENDER_GRAPH     "Enable Core Render Graph"     ON)
option(KRYSTALLIC_CORE_ENABLE_SCENE_MANAGER    "Enable Core Scene Manager"    ON)
option(KRYSTALLIC_CORE_ENABLE_SCRIPT_SYSTEM    "Enable Core Script System"    ON)
option(KRYSTALLIC_CORE_ENABLE_SOUND_API        "Enable Core Sound API"        ON)
option(KRYSTALLIC_CORE_ENABLE_SPRITE_API       "Enable Core Sprite API"       ON)
option(KRYSTALLIC_CORE_ENABLE_TEXTURE_API      "Enable Core Texture API"      ON)
option(KRYSTALLIC_CORE_ENABLE_VIDEO_API        "Enable Core Video API"        ON)


set(CORE_SOURCES
    core_main.cpp
    core_dll.cpp
)

set(CORE_COMPILE_DEFINITIONS)


macro(AddCoreComponent optionName componentDirectory definitionName)
    if(${optionName})
        set(componentPath "${CMAKE_CURRENT_SOURCE_DIR}/${componentDirectory}")

        if(NOT IS_DIRECTORY "${componentPath}")
            message(FATAL_ERROR "Core component directory does not exist: ${componentPath}")
        endif()

        file(GLOB_RECURSE componentSources CONFIGURE_DEPENDS
            "${componentPath}/*.c"
            "${componentPath}/*.cc"
            "${componentPath}/*.cpp"
            "${componentPath}/*.cxx"
            "${componentPath}/*.h"
            "${componentPath}/*.hh"
            "${componentPath}/*.hpp"
            "${componentPath}/*.hxx"
            "${componentPath}/*.inl"
        )

        if(NOT componentSources)
            message(WARNING "Core component '${componentDirectory}' does not contain source files")
        endif()

        list(APPEND CORE_SOURCES ${componentSources})
        list(APPEND CORE_COMPILE_DEFINITIONS "${definitionName}=1")
    else()
        list(APPEND CORE_COMPILE_DEFINITIONS "${definitionName}=0")
    endif()
endmacro()


AddCoreComponent(KRYSTALLIC_CORE_ENABLE_ASSET_SYSTEM assetsys KRYSTALLIC_CORE_ASSET_SYSTEM_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_FONT_API fontapi KRYSTALLIC_CORE_FONT_API_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_GUI gui KRYSTALLIC_CORE_GUI_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_DEBUG_API debugapi KRYSTALLIC_CORE_ENABLE_DEBUG_API)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_MATERIAL_SYSTEM materialsys KRYSTALLIC_CORE_MATERIAL_SYSTEM_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_MESH_API meshapi KRYSTALLIC_CORE_MESH_API_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_PARTICLE_SYSTEM particlesys KRYSTALLIC_CORE_PARTICLE_SYSTEM_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_PREFAB_SYSTEM prefabsys KRYSTALLIC_CORE_PREFAB_SYSTEM_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_RENDER_GRAPH rendergraph KRYSTALLIC_CORE_RENDER_GRAPH_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_SCENE_MANAGER scenemgr KRYSTALLIC_CORE_SCENE_MANAGER_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_SCRIPT_SYSTEM scriptsys KRYSTALLIC_CORE_SCRIPT_SYSTEM_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_SOUND_API soundapi KRYSTALLIC_CORE_SOUND_API_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_SPRITE_API spriteapi KRYSTALLIC_CORE_SPRITE_API_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_TEXTURE_API textureapi KRYSTALLIC_CORE_TEXTURE_API_ENABLED)
AddCoreComponent(KRYSTALLIC_CORE_ENABLE_VIDEO_API videoapi KRYSTALLIC_CORE_VIDEO_API_ENABLED)



add_library(core SHARED ${CORE_SOURCES})

target_compile_definitions(core PRIVATE ${CORE_COMPILE_DEFINITIONS})

set_target_properties(core PROPERTIES FOLDER "Engine")

SetWarnings(core)
SetCppMode(core)
SetOutputName(core "core")
SetEngineTargetLayout(core)


set(CORE_PRIVATE_DEPS
    simdmath
    stdlib
    crypto
)

if(KRYSTALLIC_CORE_ENABLE_FONT_API)
    list(APPEND CORE_PRIVATE_DEPS freetype)
endif()

if(KRYSTALLIC_CORE_ENABLE_SCRIPT_SYSTEM)
    list(APPEND CORE_PRIVATE_DEPS lua)
endif()

if(KRYSTALLIC_CORE_ENABLE_ASSET_SYSTEM OR KRYSTALLIC_CORE_ENABLE_MATERIAL_SYSTEM OR KRYSTALLIC_CORE_ENABLE_PREFAB_SYSTEM)
    list(APPEND CORE_PRIVATE_DEPS tinyxml2)
endif()


SetTargetDependencies(core
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src_engine/__shared

    PUBLIC_DEPS

    PRIVATE_DEPS
        ${CORE_PRIVATE_DEPS}
        filesys

    PRIVATE_3DPARTY

    BUILD_DEPS
        render_dx11
        render_gl4
        render_gles3
)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CORE_SOURCES})