#ifndef CWO_OBJECT_H
#define CWO_OBJECT_H

#include <cwo/common.h>
#include <cwo/type.h>

#include <stdlib.h>

struct cwo_Object_s;
typedef struct cwo_Object_s cwo_Object;

struct cwo_Object_s
{
    const char magic[4];
    const cwo_Type * const type;
    const cwo_Object * const base;
};

DECLEXPORT int
cwo_Object_create(void *self_ptr, size_t size,
	const cwo_Type *type, void *base);

DECLEXPORT int
cwo_Object_typeOf(void *self, const cwo_Type **type);

DECLEXPORT int
cwo_Object_destroy(void *self);

#ifdef CWO_NSIMPORT
#define Object cwo_Object
#define Object_create(s,sz,t,b) cwo_Object_create(s,sz,t,b)
#define Object_typeOf(s) cwo_Object_typeOf(s)
#define Object_destroy(s) cwo_Object_destroy(s)
#endif

#endif
