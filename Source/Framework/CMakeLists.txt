file(GLOB NRI_FRAMEWORK_HEADERS "*.h" "*.hpp")
file(GLOB NRI_FRAMEWORK_IMP "*.cpp")

source_group("" FILES ${NRI_FRAMEWORK_HEADERS})
source_group("" FILES ${NRI_FRAMEWORK_IMP})

set (NRI_FRAMEWORK_SOURCES ${NRI_FRAMEWORK_HEADERS} ${NRI_FRAMEWORK_IMP})

set (NRI_FRAMEWORK_COMPILE_DEFINITIONS ${NRI_COMPILE_DEFINITIONS} )
set (NRI_FRAMEWORK_COMPILE_OPTIONS ${NRI_COMPILE_OPTIONS})

if (WIN32)
    set(NRI_FRAMEWORK_COMPILE_DEFINITIONS ${NRI_FRAMEWORK_COMPILE_DEFINITIONS} GLFW_EXPOSE_NATIVE_WIN32 GLFW_INCLUDE_NONE)
else()
    set(NRI_FRAMEWORK_COMPILE_DEFINITIONS ${NRI_FRAMEWORK_COMPILE_DEFINITIONS} GLFW_EXPOSE_NATIVE_X11 GLFW_INCLUDE_NONE)
endif()

# Loading dependcies using packman
execute_process(COMMAND "${NRI_PACKMAN}" pull "${NRI_ROOT_DIR}/Dependencies_Framework.xml" -t nri_data_version=${NRI_DATA_VERSION}
    -p ${NRI_PACKMAN_PLATFORM} RESULT_VARIABLE NRI_PACKMAN_RESULT)
if ("${NRI_PACKMAN_RESULT}" STREQUAL "0")
    message(STATUS "Packman result: success (platform: '${NRI_PACKMAN_PLATFORM}')")
else()
    message(FATAL_ERROR "Packman result: ${NRI_PACKMAN_RESULT} (platform: '${NRI_PACKMAN_PLATFORM}')")
endif()

# Verify that required static and shared libraries are available
if (WIN32)
    find_library(NRI_FRAMEWORK_INPUT_GLFW_LIB NAMES glfw3 PATHS "${NRI_ROOT_DIR}/External/Glfw/build-${NRI_BUILD_ARCHITECTURE_SUFFIX}/Release/src" REQUIRED)
    find_library(NRI_FRAMEWORK_INPUT_ASSIMP_LIB NAMES assimp-vc142-mt PATHS "${NRI_ROOT_DIR}/External/Assimp/lib/${NRI_BUILD_ARCHITECTURE_SUFFIX}" REQUIRED)
    find_file(NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB NAMES assimp-vc142-mt.dll PATHS "${NRI_ROOT_DIR}/External/Assimp/bin/${NRI_BUILD_ARCHITECTURE_SUFFIX}" REQUIRED)
else()
    find_library(NRI_FRAMEWORK_INPUT_GLFW_LIB NAMES glfw3 PATHS "${NRI_ROOT_DIR}/External/Glfw/build/Release/src" REQUIRED)
    find_library(NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB NAMES assimp PATHS "${NRI_ROOT_DIR}/External/Assimp/bin" REQUIRED)
    set(NRI_FRAMEWORK_INPUT_ASSIMP_LIB ${NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB})
endif()

# Verify that required Linux packages are installed
if (UNIX)
    find_path(NRI_FRAMEWORK_XLIB_HEADERS NAMES "X11/Xlib.h")
    if (NOT NRI_FRAMEWORK_XLIB_HEADERS)
        message(FATAL_ERROR "Can't find 'X11/xlib.h'. (libx11-dev is not installed?)")
    endif()
    find_path(NRI_FRAMEWORK_XRANDR_HEADERS NAMES "X11/extensions/Xrandr.h")
    if (NOT NRI_FRAMEWORK_XRANDR_HEADERS)
        message(FATAL_ERROR "Can't find 'X11/extensions/Xrandr.h'. (libxrandr-dev is not installed?)")
    endif()
endif()

# MathLib requires SSE4.1
if ((CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64"))
    set(NRI_FRAMEWORK_ARCHITECTURE_COMPILE_OPTIONS -msse4.1)
endif()

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -Wno-pragma-pack: assimp causes a number of these warnings
    # warning: the current #pragma pack alignment value is modified in the included file [-Wpragma-pack]
    add_compile_options(${NRI_FRAMEWORK_ARCHITECTURE_COMPILE_OPTIONS} -Wno-missing-braces -Wno-return-type-c-linkage -Wno-pragma-pack)
    set(DETEX_COMPILE_OPTIONS -Wno-unused-function)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(${NRI_FRAMEWORK_ARCHITECTURE_COMPILE_OPTIONS})
endif()

if (WIN32)
    # On Windows, GLFW requires /MT
    add_compile_options($<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> $<$<CONFIG:RelWithDebInfo>:/MT> $<$<CONFIG:MinSizeRel>:/MT>)
endif()

# External static library: ImGui
file(GLOB NRI_FRAMEWORK_IMGUI_SOURCE "${NRI_ROOT_DIR}/External/ImGui/*.cpp" "${NRI_ROOT_DIR}/External/ImGui/*.h")
source_group("" FILES ${NRI_FRAMEWORK_IMGUI_SOURCE})
add_library(ImGui STATIC ${NRI_FRAMEWORK_IMGUI_SOURCE})
target_compile_definitions(ImGui PRIVATE ${NRI_FRAMEWORK_COMPILE_DEFINITIONS})
set_property(TARGET ImGui PROPERTY FOLDER "External")

# External static library: Detex
file(GLOB NRI_FRAMEWORK_DETEX_SOURCE "${NRI_ROOT_DIR}/External/Detex/*.c" "${NRI_ROOT_DIR}/External/Detex/*.h")
source_group("" FILES ${NRI_FRAMEWORK_DETEX_SOURCE})
add_library(Detex STATIC ${NRI_FRAMEWORK_DETEX_SOURCE})
target_compile_definitions(Detex PRIVATE ${NRI_FRAMEWORK_COMPILE_DEFINITIONS})
target_compile_options(Detex PRIVATE ${DETEX_COMPILE_OPTIONS})
set_property(TARGET Detex PROPERTY FOLDER "External")

if (NRI_STATIC_LIBRARY)
    add_library(NRIFramework STATIC ${NRI_FRAMEWORK_SOURCES})
    set (NRI_FRAMEWORK_COMPILE_DEFINITIONS ${NRI_FRAMEWORK_COMPILE_DEFINITIONS} NRI_STATIC_LIBRARY NRI_FRAMEWORK_EXPORTS=0)
else()
    add_library(NRIFramework SHARED ${NRI_FRAMEWORK_SOURCES})
    set (NRI_FRAMEWORK_COMPILE_DEFINITIONS ${NRI_FRAMEWORK_COMPILE_DEFINITIONS} NRI_FRAMEWORK_EXPORTS=1)
endif()

if (UNIX)
    target_link_libraries(NRIFramework PRIVATE ${CMAKE_DL_LIBS} pthread X11)
endif()

target_link_libraries(NRIFramework PUBLIC NRICommon NRIBase NRIDeviceManager ImGui Detex ${NRI_FRAMEWORK_INPUT_GLFW_LIB} ${NRI_FRAMEWORK_INPUT_ASSIMP_LIB})

target_include_directories(NRIFramework PUBLIC . "${NRI_ROOT_DIR}/External" "${NRI_SOURCE_DIR}/Base" "${NRI_ROOT_DIR}/External/Assimp/include")
target_compile_definitions(NRIFramework PRIVATE ${NRI_FRAMEWORK_COMPILE_DEFINITIONS})
target_compile_options(NRIFramework PRIVATE ${NRI_FRAMEWORK_COMPILE_OPTIONS})
set_property(TARGET NRIFramework PROPERTY FOLDER "Framework") 

get_filename_component(NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB_NAME ${NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB} NAME)
set(NRI_FRAMEWORK_OUTPUT_ASSIMP_SHARED_LIB ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB_NAME})
add_custom_command(TARGET NRIFramework POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NRI_FRAMEWORK_INPUT_ASSIMP_SHARED_LIB} ${NRI_FRAMEWORK_OUTPUT_ASSIMP_SHARED_LIB}
    COMMAND_EXPAND_LISTS)