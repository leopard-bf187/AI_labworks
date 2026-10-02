add_library(freetype STATIC
    autofit/autofit.c
    base/ftbase.c
    base/ftbbox.c
    base/ftbdf.c
    base/ftbitmap.c
    base/ftcid.c
    base/ftdebug.c
    base/ftfstype.c
    base/ftgasp.c
    base/ftglyph.c
    base/ftgxval.c
    base/ftinit.c
    base/ftmm.c
    base/ftotval.c
    base/ftpatent.c
    base/ftpfr.c
    base/ftstroke.c
    base/ftsynth.c
    base/ftsystem.c
    base/fttype1.c
    base/ftwinfnt.c
    bdf/bdf.c
    cache/ftcache.c
    cff/cff.c
    cid/type1cid.c
    dlg/dlgwrap.c
    gzip/ftgzip.c
    lzw/ftlzw.c
    pcf/pcf.c
    pfr/pfr.c
    psaux/psaux.c
    pshinter/pshinter.c
    psnames/psmodule.c
    raster/raster.c
    sdf/sdf.c
    sfnt/sfnt.c
    smooth/smooth.c
    svg/svg.c
    truetype/truetype.c
    type1/type1.c
    type42/type42.c
    winfonts/winfnt.c
)

set_target_properties(freetype PROPERTIES FOLDER "ThirdParty")

SetWarnings(freetype)
SetCppMode(freetype)
target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)
SetOutputName(freetype "freetype")
SetEngineTargetLayout(freetype)

SetTargetDependencies(freetype
    PUBLIC_INCLUDES
        ${CMAKE_SOURCE_DIR}/public

    PRIVATE_INCLUDES
        ${CMAKE_SOURCE_DIR}/src_engine/freetype
        ${CMAKE_SOURCE_DIR}/src_engine/__shared

    PUBLIC_DEPS

    PRIVATE_DEPS

    PRIVATE_3DPARTY

    BUILD_DEPS
)
