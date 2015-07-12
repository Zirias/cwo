#ifndef CWOS_OBJECT_H
#define CWOS_OBJECT_H

#include <cwo/object.h>

int
cwo_Object_createEmpty(void *self_ptr, size_t size);

int
cwo_Object_setType(void *self, const cwo_Type *type);

int
cwo_Object_setBase(void *self, void *base);

#endif

