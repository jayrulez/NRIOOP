cmake_minimum_required(VERSION 3.10)

set (NRI_BUILD_SAMPLES TRUE)

set (NRI_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (NRI_SOURCE_DIR ${NRI_ROOT_DIR}/Source)
set (NRI_SAMPLES_DIR ${NRI_ROOT_DIR}/Samples)

option(NRI_DISABLE_INTERPROCEDURAL_OPTIMIZATION "disable interprocedural optimization" OFF)
option(NRI_DISABLE_XLIB_SUPPORT "disable xlib support" OFF)
option(NRI_DISABLE_WAYLAND_SUPPORT "disable wayland support" OFF)
option(NRI_STATIC_LIBRARY "build static lib" OFF)

set (NRI_VERSION_MAJOR 1)
set (NRI_VERSION_MINOR 0)
set (NRI_VERSION ${NRI_VERSION_MAJOR}.${NRI_VERSION_MINOR})

project (NRI VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_POSITION_INDEPENDENT_BINARIES ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if (NOT NRI_DISABLE_INTERPROCEDURAL_OPTIMIZATION)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -Wmissing-braces
    # warning: suggest braces around initialization of subobject [-Wmissing-braces]
    # D:\git\nri\Source\D3D12\CommandBufferD3D12.cpp(344,75) initialization of anonymous union
    add_compile_options(-Wno-switch -Wno-missing-braces)
endif()

message(STATUS "${PROJECT_NAME} version: ${NRI_VERSION}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(NRI_BUILD_ARCHITECTURE_SUFFIX "x86_64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(NRI_BUILD_ARCHITECTURE_SUFFIX "x86")
endif()

# Detecting target architecture
if ((CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64"))
    set(NRI_PACKMAN_PLATFORM_SUFFIX "x86_64")
elseif ((CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64"))
    set(NRI_PACKMAN_PLATFORM_SUFFIX "aarch64")
else()
    message(FATAL_ERROR "Unknown architecture: '${CMAKE_SYSTEM_PROCESSOR}'")
endif()

# Use different scripts on different platforms
if (WIN32)
    set(NRI_PACKMAN_SCRIPT "packman.cmd")
    set(NRI_PACKMAN_PLATFORM "windows-${NRI_PACKMAN_PLATFORM_SUFFIX}")
else()
    set(NRI_PACKMAN_SCRIPT "packman")
    set(NRI_PACKMAN_PLATFORM "linux-${NRI_PACKMAN_PLATFORM_SUFFIX}")
endif()

# Verify that packman is available
find_file(NRI_PACKMAN NAMES ${NRI_PACKMAN_SCRIPT} PATHS "${NRI_ROOT_DIR}/External/Packman")
if (NOT NRI_PACKMAN)
    message(FATAL_ERROR "Can't find packman: '${NRI_ROOT_DIR}/External/Packman/${NRI_PACKMAN_SCRIPT}' (did you init submodules?)")
endif()

if (WIN32)
    execute_process(COMMAND "${NRI_PACKMAN}" pull "${NRI_ROOT_DIR}/Dependencies_Base.xml" -p ${NRI_PACKMAN_PLATFORM}
        WORKING_DIRECTORY "${NRI_ROOT_DIR}"
        RESULT_VARIABLE NRI_PACKMAN_RESULT)

    if (${NRI_PACKMAN_RESULT} EQUAL 0)
        message(STATUS "NRI: Packman result: success")
    else()
        message(FATAL_ERROR "NRI: Packman result: ${NRI_PACKMAN_RESULT}")
    endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (NRI_D3D11_ENABLED TRUE)
set (NRI_D3D12_ENABLED TRUE)
set (NRI_VULKAN_ENABLED TRUE)

# defines
if (WIN32)
    set(NRI_COMPILE_DEFINITIONS WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS _UNICODE UNICODE _ENFORCE_MATCHING_ALLOCATORS=0)
    
    # 4251 = class 'type' needs to have dll-interface to be used by clients of class 'type2' -> dll export / import issues (mostly with templates) *
    set(NRI_COMPILE_OPTIONS /wd4251)
endif()

# Dependencies
if (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(NRI_WINDOWS_SDK_BIN_ARCHITECTURE "x64")
        set(NRI_NVAPI_BIN_ARCHITECTURE "amd64")
        set(NRI_AGS_BIN_ARCHITECTURE "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(NRI_WINDOWS_SDK_BIN_ARCHITECTURE "x86")
        set(NRI_NVAPI_BIN_ARCHITECTURE "x86")
        set(NRI_AGS_BIN_ARCHITECTURE "x86")
    endif()

    if ("$ENV{WIN10_SDK_PATH}$ENV{WIN10_SDK_VERSION}" STREQUAL "")
        get_filename_component(NRI_WINDOWS_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE CACHE)
        set(NRI_WINDOWS_SDK_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
        if ("${NRI_WINDOWS_SDK_VERSION}" STREQUAL "")
            set(NRI_WINDOWS_SDK_VERSION ${CMAKE_SYSTEM_VERSION})
            message(STATUS "NRI: Using CMAKE_SYSTEM_VERSION as WindowsSDK version. (${CMAKE_SYSTEM_VERSION})")
        endif()
    else()
        set(NRI_WINDOWS_SDK_PATH $ENV{WIN10_SDK_PATH})
        set(NRI_WINDOWS_SDK_VERSION $ENV{WIN10_SDK_VERSION})
    endif()

    if ("${NRI_WINDOWS_SDK_PATH}" STREQUAL "")
        message(FATAL_ERROR "NRI: Failed to detect WindowsSDK path. Please add enviroment variables WIN10_SDK_PATH and WIN10_SDK_VERSION or use MSVC.")
    endif()
    if ("${NRI_WINDOWS_SDK_VERSION}" STREQUAL "")
        message(FATAL_ERROR "NRI: Failed to detect WindowsSDK version. Please add enviroment variables WIN10_SDK_PATH and WIN10_SDK_VERSION or use MSVC.")
    endif()
    if (IS_DIRECTORY "${NRI_WINDOWS_SDK_PATH}/Include/${NRI_WINDOWS_SDK_VERSION}.0")
        set(NRI_WINDOWS_SDK_VERSION "${NRI_WINDOWS_SDK_VERSION}.0")
    endif()
    set(NRI_WINDOWS_SDK_LIB_PATH "${NRI_WINDOWS_SDK_PATH}/Lib/${NRI_WINDOWS_SDK_VERSION}/um/${NRI_WINDOWS_SDK_BIN_ARCHITECTURE}")

    find_library(NRI_INPUT_LIB_D3D11 NAMES d3d11 HINTS ${NRI_WINDOWS_SDK_LIB_PATH} REQUIRED)
    find_library(NRI_INPUT_LIB_D3D12 NAMES d3d12 HINTS ${NRI_WINDOWS_SDK_LIB_PATH} REQUIRED)
    find_library(NRI_INPUT_LIB_DXGI NAMES dxgi HINTS ${NRI_WINDOWS_SDK_LIB_PATH} REQUIRED)
    find_library(NRI_INPUT_LIB_DXGUID NAMES dxguid HINTS ${NRI_WINDOWS_SDK_LIB_PATH} REQUIRED)
    find_library(NRI_INPUT_LIB_NVAPI NAMES nvapi64 nvapi PATHS "External/NVAPI/${NRI_NVAPI_BIN_ARCHITECTURE}" REQUIRED)
    find_library(NRI_INPUT_LIB_AGS NAMES amd_ags_${NRI_AGS_BIN_ARCHITECTURE} PATHS "External/AGS/lib" REQUIRED)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Source)
if(NRI_BUILD_SAMPLES)
	add_subdirectory(Samples)
endif()