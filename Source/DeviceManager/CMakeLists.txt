file(GLOB NRI_DEVICE_MANAGER_HEADERS "*.h" "*.hpp")
file(GLOB NRI_DEVICE_MANAGER_IMP "*.cpp")

source_group("" FILES ${NRI_DEVICE_MANAGER_HEADERS})
source_group("" FILES ${NRI_DEVICE_MANAGER_IMP})

set (NRI_DEVICE_MANAGER_SOURCES ${NRI_DEVICE_MANAGER_HEADERS} ${NRI_DEVICE_MANAGER_IMP})

set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_COMPILE_DEFINITIONS})
set (NRI_DEVICE_MANAGER_COMPILE_OPTIONS ${NRI_COMPILE_OPTIONS})

set (DM_INCLUDE_DIRS . ${NRI_ROOT_DIR}/Base ${NRI_ROOT_DIR}/Validation)
set (DM_LINK_LIBRARIES "")

if(NOT NRI_D3D11_ENABLED)
    set (DM_INCLUDE_DIRS ${DM_INCLUDE_DIRS} ${NRI_ROOT_DIR}/D3D11)
    set (DM_LINK_LIBRARIES ${DM_LINK_LIBRARIES} NRID3D11)
    set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS} NRI_USE_D3D11=1)
endif()

if(NRI_D3D12_ENABLED)
    set (DM_INCLUDE_DIRS ${DM_INCLUDE_DIRS} ${NRI_ROOT_DIR}/D3D12)
    set (DM_LINK_LIBRARIES ${DM_LINK_LIBRARIES} NRID3D12)
    set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS} NRI_USE_D3D12=1)
endif()

if(NRI_VULKAN_ENABLED)
    set (DM_INCLUDE_DIRS ${DM_INCLUDE_DIRS} ${NRI_ROOT_DIR}/VK)
    set (DM_LINK_LIBRARIES ${DM_LINK_LIBRARIES} NRIVK)
    set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS} NRI_USE_VULKAN=1)
endif()

if (NRI_STATIC_LIBRARY)
    add_library(NRIDeviceManager STATIC ${NRI_DEVICE_MANAGER_SOURCES})
    set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS} NRI_STATIC_LIBRARY NRI_DEVICE_MANAGER_EXPORTS=0)
else()
    add_library(NRIDeviceManager SHARED ${NRI_DEVICE_MANAGER_SOURCES})
    set (NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS} NRI_DEVICE_MANAGER_EXPORTS=1)
endif()

if (UNIX)
    target_link_libraries(NRIDeviceManager PRIVATE ${CMAKE_DL_LIBS})
endif()

target_link_libraries(NRIDeviceManager PUBLIC NRICommon NRIBase NRIValidation ${DM_LINK_LIBRARIES})

target_include_directories(NRIDeviceManager PUBLIC ${DM_INCLUDE_DIRS})
target_compile_definitions(NRIDeviceManager PRIVATE ${NRI_DEVICE_MANAGER_COMPILE_DEFINITIONS})
target_compile_options(NRIDeviceManager PRIVATE ${NRI_DEVICE_MANAGER_COMPILE_OPTIONS})
set_property(TARGET NRIDeviceManager PROPERTY FOLDER "NRI")