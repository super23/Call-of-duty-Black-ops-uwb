cmake_minimum_required(VERSION 3.16)
project(KisakBlack)

set(BIN_NAME "KisakBlack")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR_PLATFORM "WIN32")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TL_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/tl")
set(DW_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/DemonWare")

include(cmake_files.cmake)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT  /O2 /MP /FS /W3 ${MSVC_WARNING_DISABLES} /permissive")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MTd /Od /MP /FS /W3 ${MSVC_WARNING_DISABLES} /permissive")

if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} /SAFESEH:NO")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
endif()

function(kisak_configure_game_target target kisak_mode_define)
    # phys_gjk.cpp specific flag (experimental)
    set_source_files_properties(
        "${SRC_DIR}/physics/phys_gjk.cpp"
        PROPERTIES
        COMPILE_OPTIONS "/arch:IA32"
    )

    # ----- Preprocessor Defines -----
    target_compile_definitions(${target} PUBLIC
        WIN32
        _CONSOLE
        ${kisak_mode_define}
        _CRT_SECURE_NO_WARNINGS
        _WINSOCK_NO_DEPRECATED_NO_WARNINGS
    )
    target_compile_definitions(${target} PUBLIC
        $<$<CONFIG:Debug>:_DEBUG>
    )
    target_compile_definitions(${target} PUBLIC
        $<$<CONFIG:Release>:NDEBUG>
    )

    # ----- Include directories -----
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/libtomcrypt-1.17/src/headers")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/libtommath-1.0")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/libvpx-1.5.0/include")
    target_include_directories(${target} PUBLIC "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")

    # ----- Library directories -----
    target_link_directories(${target} PUBLIC "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")
    target_link_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/libvpx-1.5.0")
    target_link_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/steam/redistributable_bin")
    target_link_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/binklib")
    target_link_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/nvapi")

    if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
        target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
        target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:/ZI>)
    endif()

    # ----- Links -----
    target_link_libraries(${target} PUBLIC
        steam_api.lib
        binkw32.lib
        nvapi.lib
        d3d9.lib
        ws2_32.lib
        dxguid.lib
        dsound.lib
        winmm.lib
        xinput.lib
        xapobase.lib
        dxerr.lib
        libvpx.lib
        legacy_stdio_definitions.lib
    )
    target_link_libraries(${target} PUBLIC $<$<CONFIG:Debug>:d3dx9d.lib>)
    target_link_libraries(${target} PUBLIC $<$<CONFIG:Release>:d3dx9.lib>)
    target_link_options(${target} PRIVATE /machine:x86)

    # /DYNAMICBASE:NO - Turning this off causes directx to crash "directx has encountered an unrecoverable error".
    # /LARGEADDRESSAWARE - for convenience (4gb)
    # /DEBUG - PDB for both configs
    target_link_options(${target} PRIVATE /DYNAMICBASE:NO /LARGEADDRESSAWARE /DEBUG)

    set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(${target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
    )
endfunction()

kisak_configure_game_target(KisakBlack KISAK_MP)
set_target_properties(KisakBlack PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/main_mp"
)

option(KISAK_BUILD_SP ON)
if(KISAK_BUILD_SP)
    include(cmake_files_sp.cmake)

    # SP-specific sources are placeholders until decomp lands; skip missing paths at configure time.
    set(_kisak_sp_sources_existing)
    foreach(_src IN LISTS KISAK_SP_SOURCES)
        if(EXISTS "${_src}")
            list(APPEND _kisak_sp_sources_existing "${_src}")
        endif()
    endforeach()

    add_executable(KisakBlackSP ${_kisak_sp_sources_existing})
    set_target_properties(KisakBlackSP PROPERTIES OUTPUT_NAME KisakBlackSP)

    kisak_configure_game_target(KisakBlackSP KISAK_SP)
    target_include_directories(KisakBlackSP BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src_sp_shims")
    set_target_properties(KisakBlackSP PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/main_sp"
    )
endif()

# ----- cod2rad lighting compiler (x64 tool, optional) -----
# Built in an isolated CMake tree so it cannot override the Win32 game platform.
option(KISAK_BUILD_COD2RAD "Build cod2rad lighting compiler (x64)" OFF)
if(KISAK_BUILD_COD2RAD)
    include(ExternalProject)
    set(_cod2rad_binary_dir "${CMAKE_BINARY_DIR}/cod2rad_x64")
    ExternalProject_Add(cod2rad_ep
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/tools/cod2rad"
        BINARY_DIR "${_cod2rad_binary_dir}"
        CMAKE_GENERATOR "${CMAKE_GENERATOR}"
        CMAKE_GENERATOR_PLATFORM "x64"
        CMAKE_GENERATOR_TOOLSET "${CMAKE_GENERATOR_TOOLSET}"
        INSTALL_COMMAND ""
        BUILD_ALWAYS TRUE
    )
    add_custom_target(cod2rad DEPENDS cod2rad_ep)
endif()
