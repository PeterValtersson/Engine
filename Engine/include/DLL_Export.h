#ifndef _DLL_EXPORT_ENGINE_H_
#define _DLL_EXPORT_ENGINE_H_
#ifdef _ENGINE_EXPORT_
#define DECLSPEC_ENGINE __declspec(dllexport)
#else
#define DECLSPEC_ENGINE __declspec(dllimport)
#endif
#endif