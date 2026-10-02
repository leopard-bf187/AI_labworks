add_library(simdmath STATIC
    stub.cpp
)

set_target_properties(simdmath PROPERTIES FOLDER "Engine")

SetWarnings(simdmath)
SetCppMode(simdmath)
SetOutputName(simdmath "simdmath")
SetEngineTargetLayout(simdmath)

ApplySimd(simdmath)

SetTargetDependencies(simdmath
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_SOURCE_DIR}/src_engine/simdmath

    PUBLIC_DEPS

    PRIVATE_DEPS

    BUILD_DEPS
)


