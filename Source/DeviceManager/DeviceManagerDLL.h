#pragma once

#include "BaseDLL.h"

#if _WIN32
#if defined(_MSC_VER)
#if defined(NRI_STATIC_LIBRARY)
#define NRI_DEVICE_MANAGER_API
#else
#if (NRI_DEVICE_MANAGER_EXPORTS == 1)
#define NRI_DEVICE_MANAGER_API __declspec(dllexport)
#else
#define NRI_DEVICE_MANAGER_API __declspec(dllexport)
#endif
#endif
#else
#if defined(NRI_STATIC_LIBRARY)
#define NRI_DEVICE_MANAGER_API
#else
#if (NRI_DEVICE_MANAGER_EXPORTS == 1)
#define NRI_DEVICE_MANAGER_API __attribute__ ((dllexport))
#else
#define NRI_DEVICE_MANAGER_API __attribute__ ((dllimport))
#endif
#endif
#endif
#else
#define NRI_DEVICE_MANAGER_API __attribute__ ((visibility ("default")))
#endif