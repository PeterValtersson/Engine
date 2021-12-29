#ifndef _DLL_EXPORT_HMI_H_
#define _DLL_EXPORT_HMI_H_
#ifdef _HMI_EXPORT_
#define DECLSPEC_HMI __declspec(dllexport)
#else
#define DECLSPEC_HMI __declspec(dllimport)
#endif
#endif