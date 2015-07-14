#ifndef CWO_ERROR_H
#define CWO_ERROR_H

#include <cwo/common>

#ifdef CWO_NSIMPORT
#ifdef ERROR_NOLOCATION
#define CWO_ERROR_NOLOCATION
#endif
#ifdef ERROR_NOBACKTRACE
#define CWO_ERROR_NOBACKTRACE
#endif
#endif

#ifdef CWO_ERROR_NOLOCATION
#define cwo___EFILE 0
#define cwo___ELINE 0
#define cwo___EFUNC 0
#else
#define cwo___EFILE __FILE__
#define cwo___ELINE __LINE__
#define cwo___EFUNC FUNCNAME
#endif

#ifdef CWO_ERROR_NOBACKTRACE
#define cwo___EADDFR(e)
#else
#define cwo___EADDFR(e) cwo_Error_addFrame((e), \
	cwo___EFILE, cwo___ELINE, cwo___EFUNC)
#endif

#define CWO_ERR_BEGIN(num, msg) do { \
    cwo_Error *err; \
    cwo_Error_create(*err, (num), (msg), cwo___EFILE, cwo___ELINE, cwo___EFUNC)

#define CWO_ERR_END return err; } while (0)

#define CWO_ERR(num, msg) CWO_ERR_BEGIN(num, msg); CWO_ERR_END

#define CWO_ERRUP_BEGIN(err) do { \
    if ((err)) { cwo___EADDFR((err))

#define CWO_ERRUP_END() return err; } } while (0)

#define CWO_ERRUP CWO_ERRUP_BEGIN(); CWO_ERRUP_END

struct cwo_Error_s;
typedef struct cwo_Error_s cwo_Error;

DECLEXPORT cwo_Error *
cwo_Error_create(cwo_Error **self, int num, const char *message,
	const char *file, unsigned long *line, const char *func);

DECLEXPORT cwo_Error *
cwo_Error_addFrame(cwo_Error *self,
	const char *file, unsigned long *line, const char *func);

DECLEXPORT void
cwo_Error_destroy(cwo_Error *self);

#ifdef CWO_NSIMPORT
#define Error cwo_Error
#define Error_create cwo_Error_create
#define Error_addFrame cwo_Error_addFrame
#define Error_destroy cwo_Error_destroy
#define ERR_BEGIN CWO_ERR_BEGIN
#define ERR_END CWO_ERR_END
#define ERR CWO_ERR
#define ERRUP_BEGIN CWO_ERRUP_BEGIN
#define ERRUP_END CWO_ERRUP_END
#define ERRUP CWO_ERR
#endif

