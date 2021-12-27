#pragma once

#if _WIN32
#if defined(_MSC_VER)
#if defined(NRI_STATIC_LIBRARY)
#define NRI_COMMON_API
#else
#if (NRI_COMMON_EXPORTS == 1)
#define NRI_COMMON_API __declspec(dllexport)
#else
#define NRI_COMMON_API __declspec(dllexport)
#endif
#endif
#else
#if defined(NRI_STATIC_LIBRARY)
#define NRI_COMMON_API
#else
#if (NRI_COMMON_EXPORTS == 1)
#define NRI_COMMON_API __attribute__ ((dllexport))
#else
#define NRI_COMMON_API __attribute__ ((dllimport))
#endif
#endif
#endif
#else
#define NRI_COMMON_API __attribute__ ((visibility ("default")))
#endif

struct MemoryAllocatorInterface
{
	void* (*Allocate)(void* userArg, size_t size, size_t alignment);
	void* (*Reallocate)(void* userArg, void* memory, size_t size, size_t alignment);
	void (*Free)(void* userArg, void* memory);
	void* userArg;
};