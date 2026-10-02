add_library(crypto STATIC
    #aeslib.c
    #cdkey.c
    #cdkey_dbg.c
    #gamekey.c
    crypto_crc32.c
    crypto_fnv1a.c
    crypto_md4.c
    crypto_md5.c
)

set_target_properties(crypto PROPERTIES FOLDER "Engine")

SetWarnings(crypto)
SetCppMode(crypto)
SetOutputName(crypto "crypto")
SetEngineTargetLayout(crypto)

SetTargetDependencies(crypto
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_SOURCE_DIR}/src_engine/crypto
        ${CMAKE_SOURCE_DIR}/src_engine/__shared

    PUBLIC_DEPS

    PRIVATE_DEPS

    PRIVATE_3DPARTY

    BUILD_DEPS
)

