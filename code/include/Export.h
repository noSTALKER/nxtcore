#pragma once

#ifdef NXT_CORE_DEFINE
#ifdef _WIN32
#define NXTCORE_DLL_EXPORT __declspec(dllexport)
#else
#define NXTCORE_DLL_EXPORT
#endif

#else
#ifdef _WIN32
#define NXTCORE_DLL_EXPORT __declspec(dllimport)
#else
#define NXTCORE_DLL_EXPORT
#endif

#endif
