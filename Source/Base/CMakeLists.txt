file(GLOB NRI_BASE_HEADERS "*.h" "*.hpp")
file(GLOB NRI_BASE_IMP "*.cpp")

file(GLOB NRI_BASE_HELPERS_HEADERS "Helpers/*.h" "Helpers/*.hpp")
file(GLOB NRI_BASE_HELPERS_IMP "Helpers/*.cpp")

source_group("" FILES ${NRI_BASE_HEADERS})
source_group("" FILES ${NRI_BASE_IMP})
source_group("Helpers" FILES ${NRI_BASE_HELPERS_HEADERS})
source_group("Helpers" FILES ${NRI_BASE_HELPERS_IMP})

set (NRI_BASE_SOURCES ${NRI_BASE_HEADERS} ${NRI_BASE_IMP} ${NRI_BASE_HELPERS_HEADERS} ${NRI_BASE_HELPERS_IMP})

set (NRI_BASE_COMPILE_DEFINITIONS ${NRI_COMPILE_DEFINITIONS})
set (NRI_BASE_COMPILE_OPTIONS ${NRI_COMPILE_OPTIONS})

if (NRI_STATIC_LIBRARY)
    add_library(NRIBase STATIC ${NRI_BASE_SOURCES})
    set (NRI_BASE_COMPILE_DEFINITIONS ${NRI_BASE_COMPILE_DEFINITIONS} NRI_STATIC_LIBRARY NRI_BASE_EXPORTS=0)
else()
    add_library(NRIBase SHARED ${NRI_BASE_SOURCES})
    set (NRI_BASE_COMPILE_DEFINITIONS ${NRI_BASE_COMPILE_DEFINITIONS} NRI_BASE_EXPORTS=1)
endif()

if (UNIX)
    target_link_libraries(NRIBase PRIVATE ${CMAKE_DL_LIBS})
endif()

target_link_libraries(NRIBase PUBLIC NRICommon)

target_include_directories(NRIBase PUBLIC .)
target_compile_definitions(NRIBase PRIVATE ${NRI_BASE_COMPILE_DEFINITIONS})
target_compile_options(NRIBase PRIVATE ${NRI_BASE_COMPILE_OPTIONS})
set_property(TARGET NRIBase PROPERTY FOLDER "NRI") 