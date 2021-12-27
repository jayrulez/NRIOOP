#if defined(_WIN32)
#include <windows.h>
#undef LoadLibrary

#include "CommonDLL.h"
#include "SharedLibrary.h"

Library* LoadSharedLibrary(const char* path)
{
	return (Library*)LoadLibraryA(path);
}

void* GetSharedLibraryFunction(Library& library, const char* name)
{
	return (void*)GetProcAddress((HMODULE)&library, name);
}

void UnloadSharedLibrary(Library& library)
{
	FreeLibrary((HMODULE)&library);
}
#elif defined(__linux__)
#include <dlfcn.h>

Library* LoadSharedLibrary(const char* path)
{
	return (Library*)dlopen(path, RTLD_NOW);
}

void* GetSharedLibraryFunction(Library& library, const char* name)
{
	return dlsym((void*)&library, name);
}

void UnloadSharedLibrary(Library& library)
{
	dlclose((void*)&library);
}
#else 
#error unknown platform
#endif