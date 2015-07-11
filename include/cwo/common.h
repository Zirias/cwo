#ifndef CWO_COMMON_H
#define CWO_COMMON_H

#ifdef __cplusplus
#define cwo___cdecl extern "C"
#else
#define cwo___cdecl
#endif

#ifdef WIN32
#define SOEXPORT cwo___cdecl __declspec(dllexport)
#ifdef BUILDING_cwo
#define DECLEXPORT __declspec(dllexport)
#else
#define DECLEXPORT cwo___cdecl __declspec(dllimport)
#endif
#define SOLOCAL
#else
#define DECLEXPORT cwo___cdecl
#if __GNUC__ >= 4
#define SOEXPORT cwo___cdecl __attribute__ ((visibility ("default")))
#define SOLOCAL __attribute__ ((visibility ("hidden")))
#else
#define SOEXPORT cwo___cdecl
#define SOLOCAL
#endif
#endif


#endif
