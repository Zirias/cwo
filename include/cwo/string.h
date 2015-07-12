#ifndef CWO_STRING_H
#define CWO_STRING_H

#include <cwo/common.h>

struct cwo_String_s;
typedef struct cwo_String_s cwo_String;

DECLEXPORT int
cwo_String_create(cwo_String **self, const char *content);

DECLEXPORT const char *
cwo_String_cstr(const cwo_String *self);

DECLEXPORT void
cwo_String_destroy(cwo_String *self);

#endif
