#ifndef _DLL_EXPORT_WINDOW_H_
#define _DLL_EXPORT_WINDOW_H_
#ifdef _WINDOW_EXPORT_
#define DECLSPEC_WINDOW __declspec(dllexport)
#else
#define DECLSPEC_WINDOW __declspec(dllimport)
#endif
#endif