#pragma once

#include "CommonDLL.h"

struct Library;
NRI_COMMON_API Library* LoadSharedLibrary(const char* path);
NRI_COMMON_API void* GetSharedLibraryFunction(Library& library, const char* name);
NRI_COMMON_API void UnloadSharedLibrary(Library& library);