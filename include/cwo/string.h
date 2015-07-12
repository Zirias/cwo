#ifndef CWO_STRING_H
#define CWO_STRING_H

#include <stdlib.h>
#include <cwo/common.h>

struct cwo_String_s;
typedef struct cwo_String_s cwo_String;

DECLEXPORT int
cwo_String_create(cwo_String **self, const char *content);

DECLEXPORT const char *
cwo_String_cstr(const cwo_String *self);

DECLEXPORT size_t
cwo_String_len(const cwo_String *self);

DECLEXPORT void
cwo_String_destroy(cwo_String *self);

#ifdef CWO_NSIMPORT
#define String cwo_String
#define String_create cwo_String_create
#define String_cstr cwo_String_cstr
#define String_len cwo_String_len
#define String_destroy cwo_String_destroy
#endif

#endif
