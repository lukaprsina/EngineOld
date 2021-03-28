#pragma once

#ifdef ENG_PLATFORM_WINDOWS
#ifdef ENG_BUILD_DLL
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif
#elif ENG_PLATFORM_UNIX
#ifdef ENG_BUILD_DLL
#define ENG_API __attribute__((visibility("default")))
#else
#define ENG_API
#endif
#else
#define ENG_API
#endif