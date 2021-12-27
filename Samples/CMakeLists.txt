set (NRI_SAMPLE_COMPILE_DEFINITIONS ${NRI_COMPILE_DEFINITIONS})
set (NRI_SAMPLE_COMPILE_OPTIONS ${NRI_COMPILE_OPTIONS})

option(DISABLE_SHADER_COMPILATION "disable compilation of shaders" OFF)
option(USE_MINIMAL_DATA "use minimal _Data (90MB)" OFF)
option(USE_DXC_FROM_PACKMAN_ON_AARCH64 "download DXC for aarch64 using Packman" ON)

# Choose data package
if (USE_MINIMAL_DATA)
    set(NRI_DATA_VERSION 2.0_minimal)
    message(STATUS "Using minimal _Data.")
else()
    set(NRI_DATA_VERSION 2.1)
endif()

# Loading dependcies using packman
execute_process(COMMAND "${NRI_PACKMAN}" pull "${NRI_ROOT_DIR}/Dependencies_Samples.xml" -t nri_data_version=${NRI_DATA_VERSION}
    -p ${NRI_PACKMAN_PLATFORM} RESULT_VARIABLE PACKMAN_RESULT)
if ("${PACKMAN_RESULT}" STREQUAL "0")
    message(STATUS "Packman result: success (platform: '${NRI_PACKMAN_PLATFORM}')")
else()
    message(FATAL_ERROR "Packman result: ${PACKMAN_RESULT} (platform: '${NRI_PACKMAN_PLATFORM}')")
endif()

# Packman downloads some dependencies on aarch64
if ((CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64"))
    if (("${DXC_CUSTOM_PATH}" STREQUAL "") AND USE_DXC_FROM_PACKMAN_ON_AARCH64)
        set(DXC_CUSTOM_PATH "${NRI_ROOT_DIR}/External/DXC/bin/dxc")
    endif()
endif()

# Shader compilation
if (NOT DISABLE_SHADER_COMPILATION)
    # Include path for DXC and FXC (used in ShaderCompilation.cmake)
    set(SHADER_INCLUDE_PATH "${NRI_ROOT_DIR}/Source/Shaders")
    set(MATHLIB_INCLUDE_PATH "${NRI_ROOT_DIR}/External/NRIFramework/External/MathLib")
    set(SHADER_OUTPUT_PATH "${NRI_ROOT_DIR}/_Data/Shaders")

    # List HLSL shaders and headers
    file(GLOB_RECURSE HLSL_FILES "Samples/Shaders/*.hlsli" "Samples/Shaders/*.hlsl")
    file(GLOB_RECURSE HLSL_MATHLIB_HEADER_FILES "${MATHLIB_INCLUDE_PATH}/*.hlsli")

    include(${NRI_ROOT_DIR}/ShaderCompilation.cmake)

    # Create "_Data/Shaders"
    add_custom_target(SampleCreateFolderForShaders ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_PATH})
    set_property(TARGET SampleCreateFolderForShaders PROPERTY FOLDER "Shaders")

    # Define build rules for the shaders
    list_hlsl_headers("${HLSL_FILES}" HEADER_FILES)
    list_hlsl_headers("${HLSL_MATHLIB_HEADER_FILES}" HEADER_FILES)
    list_hlsl_shaders("${HLSL_FILES}" "${HEADER_FILES}" SHADER_FILES)
    add_custom_target(SampleShaders ALL DEPENDS ${SHADER_FILES} SOURCES "${HEADER_FILES}")
    add_dependencies(SampleShaders SampleCreateFolderForShaders)
    set_property(TARGET SampleShaders PROPERTY FOLDER "Shaders")
endif()

# MathLib requires SSE4.1
if ((CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64"))
    set(ARCHITECTURE_COMPILE_OPTIONS -msse4.1)
endif()

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -Wno-pragma-pack: assimp causes a number of these warnings
    # warning: the current #pragma pack alignment value is modified in the included file [-Wpragma-pack]
    add_compile_options(${ARCHITECTURE_COMPILE_OPTIONS} -Wno-missing-braces -Wno-return-type-c-linkage -Wno-pragma-pack)
    set(DETEX_COMPILE_OPTIONS -Wno-unused-function)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(${ARCHITECTURE_COMPILE_OPTIONS})
endif()

if(MSVC)
    # On Windows, GLFW requires /MT
    add_compile_options($<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> $<$<CONFIG:RelWithDebInfo>:/MT> $<$<CONFIG:MinSizeRel>:/MT>)
endif()

# Creates a project for sample and adds dependent operations
function(add_sample SAMPLE_NAME)
    add_executable(${SAMPLE_NAME} "${SAMPLE_NAME}.cpp")
    source_group("" FILES "${SAMPLE_NAME}.cpp")
    if (NOT DISABLE_SHADER_COMPILATION)
        add_dependencies(${SAMPLE_NAME} SampleShaders)
    endif()
    target_include_directories(${SAMPLE_NAME} PRIVATE .)
    target_include_directories(${SAMPLE_NAME} PRIVATE "${NRI_ROOT_DIR}/Base")
    target_compile_definitions(${SAMPLE_NAME} PRIVATE ${NRI_SAMPLE_COMPILE_DEFINITIONS} PROJECT_NAME=${SAMPLE_NAME})
    target_compile_options(${SAMPLE_NAME} PRIVATE ${NRI_SAMPLE_COMPILE_OPTIONS})

    target_link_libraries(${SAMPLE_NAME} PRIVATE NRIFramework NRIBase)

    set_property(TARGET ${SAMPLE_NAME} PROPERTY FOLDER "Samples")
    set_property(TARGET ${SAMPLE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endfunction()

add_sample(01_Triangle)
