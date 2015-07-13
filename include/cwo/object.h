#ifndef CWO_OBJECT_H
#define CWO_OBJECT_H

#include <cwo/common.h>
#include <cwo/type.h>
#include <cwo/string.h>

#include <stdlib.h>

struct cwo_Object_s;
typedef struct cwo_Object_s cwo_Object;

struct cwo_Object_s
{
    const char magic[4];
    const cwo_Type * const type;
    const cwo_Object * const base;
    const size_t size;
};

DECLEXPORT const cwo_Object *
cwo_Object_instance(void);

DECLEXPORT int
cwo_Object_create(void *self_ptr, size_t size,
	const cwo_Type *type, const void *base);

DECLEXPORT int
cwo_Object_clone(const void *self, void *clone_ptr);

DECLEXPORT int
cwo_Object_toString(void *self, cwo_String **string);

DECLEXPORT int
cwo_Object_isObject(void *self);

DECLEXPORT int
cwo_Object_isInstanceOf(void *self, const cwo_Type *type);

DECLEXPORT int
cwo_Object_typeOf(void *self, const cwo_Type **type);

DECLEXPORT int
cwo_Object_destroy(void *self);

#ifdef CWO_NSIMPORT
#define Object cwo_Object
#define Object_instance cwo_Object_instance
#define Object_create cwo_Object_create
#define Object_clone cwo_Object_clone
#define Object_isObject cwo_Object_isObject
#define Object_isInstanceOf cwo_Object_isInstanceOf
#define Object_typeOf cwo_Object_typeOf
#define Object_destroy cwo_Object_destroy
#endif

#endif
