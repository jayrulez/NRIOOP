file(GLOB NRI_VK_HEADERS "*.h" "*.hpp")
file(GLOB NRI_VK_IMP "*.cpp")

source_group("" FILES ${NRI_VK_HEADERS})
source_group("" FILES ${NRI_VK_IMP})

set (NRI_VK_SOURCES ${NRI_VK_HEADERS} ${NRI_VK_IMP})

set (NRI_VK_COMPILE_DEFINITIONS ${NRI_COMPILE_DEFINITIONS} NRI_USE_VULKAN=1)
set (NRI_VK_COMPILE_OPTIONS ${NRI_COMPILE_OPTIONS})

if (NRI_STATIC_LIBRARY)
    add_library(NRIVK STATIC ${NRI_VK_SOURCES})
    set (NRI_VK_COMPILE_DEFINITIONS ${NRI_VK_COMPILE_DEFINITIONS} NRI_STATIC_LIBRARY NRI_VK_EXPORTS=0)
else()
    add_library(NRIVK SHARED ${NRI_VK_SOURCES})
    set (NRI_VK_COMPILE_DEFINITIONS ${NRI_VK_COMPILE_DEFINITIONS} NRI_VK_EXPORTS=1)
endif()

if (UNIX)
    target_link_libraries(NRIVK PRIVATE ${CMAKE_DL_LIBS})
endif()

target_link_libraries(NRIVK PUBLIC NRICommon NRIBase ${NRI_INPUT_LIB_VK} ${NRI_INPUT_LIB_DXGI})

target_include_directories(NRIVK PUBLIC . ${NRI_ROOT_DIR}/External/vulkan/include)
target_compile_definitions(NRIVK PRIVATE ${NRI_VK_COMPILE_DEFINITIONS})
target_compile_options(NRIVK PRIVATE ${NRI_VK_COMPILE_OPTIONS})
set_property(TARGET NRIVK PROPERTY FOLDER "NRI")

# Window system defines
if (WIN32)
    target_compile_definitions(NRIVK PRIVATE VK_USE_PLATFORM_WIN32_KHR)
else()
    if (NOT NRI_DISABLE_XLIB_SUPPORT)
        find_path(NRI_XLIB_HEADERS NAMES "X11/Xlib.h")
        if (NOT NRI_XLIB_HEADERS)
            message(FATAL_ERROR "NRI: Can't find 'X11/xlib.h'. (libx11-dev is not installed?)")
        endif()
        target_compile_definitions(NRIVK PRIVATE VK_USE_PLATFORM_XLIB_KHR)
    endif()
    if (NOT NRI_DISABLE_WAYLAND_SUPPORT)
        find_path(NRI_WAYLAND_HEADERS NAMES "wayland-client.h")
        if (NOT NRI_WAYLAND_HEADERS)
            message(FATAL_ERROR "NRI: Can't find 'wayland-client.h'. (libwayland-dev is not installed?)")
        endif()
        target_compile_definitions(NRIVK PRIVATE VK_USE_PLATFORM_WAYLAND_KHR)
    endif()
endif()