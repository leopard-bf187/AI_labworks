

if(KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_ANDROID")

    add_library(android_native_app_glue STATIC 
        ${ANDROID_NDK_ROOT}/sources/android/native_app_glue/android_native_app_glue.c
    )

    SetWarnings(android_native_app_glue)
    SetCppMode(android_native_app_glue)
    SetOutputName(android_native_app_glue "android_native_app_glue")
    SetEngineTargetLayout(android_native_app_glue)

    set(SRC
        android/android_CPlatformInputManager.cpp
        android/android_CPlatformManager.cpp
        android/android_CPlatformMemoryManager.cpp
        android/android_CPlatformProcessManager.cpp
        android/android_CPlatformSystemInfo.cpp
        android/android_CPlatformThreadManager.cpp
        android/android_CPlatformTimeManager.cpp
        android/android_CPlatformWindow.cpp
        android/android_CSharedMemory.cpp
        android/android_CSystemTimer.cpp
        android/android_CVirtualMemory.cpp
        android/android_CWindowCallbacks.cpp
    )

    set(Libs3dp 
            android 
            log 
            android_native_app_glue
    )

elseif(KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_LINUX")

    add_library(xdg_inner_library STATIC 
        linux/xdg-shell-client-protocol.c
        linux/xdg-decoration-unstable-v1-client-protocol.c
    )

    SetWarnings(xdg_inner_library)
    SetCppMode(xdg_inner_library)
    SetOutputName(xdg_inner_library "xdg_inner_library")
    SetEngineTargetLayout(xdg_inner_library)

    set(SRC
        linux/linux_CPlatformInputManager.cpp
        linux/linux_CPlatformManager.cpp
        linux/linux_CPlatformMemoryManager.cpp
        linux/linux_CPlatformProcessManager.cpp
        linux/linux_CPlatformSystemInfo.cpp
        linux/linux_CPlatformThreadManager.cpp
        linux/linux_CPlatformTimeManager.cpp
        linux/linux_CPlatformWindow.cpp
        linux/linux_CSharedMemory.cpp
        linux/linux_CSystemTimer.cpp
        linux/linux_CVirtualMemory.cpp
        linux/linux_CWindowCallbacks.cpp
    )

    set(Libs3dp 
            xdg_inner_library
            wayland-client
            wayland-egl
    )

elseif(KRYSTALLIC_OS_DEFINE STREQUAL "KRYSTALLIC_OS_WINNT")

    set(SRC
        win32/win32_CPlatformInputManager.cpp
        win32/win32_CPlatformManager.cpp
        win32/win32_CPlatformMemoryManager.cpp
        win32/win32_CPlatformProcessManager.cpp
        win32/win32_CPlatformSystemInfo.cpp
        win32/win32_CPlatformThreadManager.cpp
        win32/win32_CPlatformTimeManager.cpp
        win32/win32_CPlatformWindow.cpp
        win32/win32_CSharedMemory.cpp
        win32/win32_CSystemTimer.cpp
        win32/win32_CVirtualMemory.cpp
        win32/win32_CWindowCallbacks.cpp
        win32/win32_WindowProc.cpp
    )

    set(Libs3dp 
            )

endif()


add_library(platform SHARED
    cpuinfo.cpp
    mem_smbios.cpp
    ${SRC}
    platform_CError.cpp
    platform_main.cpp
    platform_dll.cpp
)

set_target_properties(platform PROPERTIES
    FOLDER "Engine"
)

SetWarnings(platform)
SetCppMode(platform)
SetOutputName(platform "platform")
SetEngineTargetLayout(platform)


SetTargetDependencies(platform
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_SOURCE_DIR}/src_engine/platform
        ${CMAKE_SOURCE_DIR}/src_engine/__shared

    PUBLIC_DEPS

    PRIVATE_DEPS
        crypto
        simdmath
        stdlib

    PRIVATE_3DPARTY
        ${Libs3dp}

    BUILD_DEPS
)




# set(SRC
#     src/main.cpp
#     src/game.cpp
# )

# set(HDR
#     public/game.h
#     public/player.h
# )

# add_executable(game_client
#     ${SRC}
#     ${HDR}
# )

# source_group("Source Files" FILES ${SRC})
# source_group("Header Files" FILES ${HDR})

#-------------------------------

