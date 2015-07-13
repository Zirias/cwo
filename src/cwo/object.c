#include "object.h"

#include "internal/utils.h"
#include "internal/errors.h"

#include <string.h>

#define MAGIC "cwoO"

struct cwo_MutableObject_s
{
    char magic[4];
    const cwo_Type *type;
    const cwo_Object *base;
    size_t size;
};

static cwo_Object instance = { MAGIC, 0, 0, sizeof(cwo_Object) };

SOLOCAL int
cwo_Object_createEmpty(void *self_ptr, size_t size)
{
    int err;
    struct cwo_MutableObject_s *mo;

    if (size < sizeof(cwo_Object)) return CWOERR_INVARG;

    err = cwoint_alloc(&mo, size);
    if (err) return err;
    memset(&mo, 0, size);
    memcpy(&mo, MAGIC, 4);
    mo->size = size;

    *(void **)self_ptr = mo;
    return CWO_SUCCESS;
}

SOLOCAL int
cwo_Object_setType(void *self, const cwo_Type *type)
{
    struct cwo_MutableObject_s *mo = self;

    if (!mo) return CWOERR_NULLARG;
    if (strncmp(mo->magic, MAGIC, 4)) return CWOERR_INVARG;

    mo->type = type;
    return CWO_SUCCESS;
}

SOLOCAL int
cwo_Object_setBase(void *self, void *base)
{
    struct cwo_MutableObject_s *mo = self;

    if (!mo) return CWOERR_NULLARG;
    if (strncmp(mo->magic, MAGIC, 4)) return CWOERR_INVARG;

    mo->base = base;
    return CWO_SUCCESS;
}

SOEXPORT int
cwo_Object_create(void *self_ptr, size_t size,
	const cwo_Type *type, const void *base)
{
    int err;
    struct cwo_MutableObject_s *mo;

    if (!type || !base) return CWOERR_NULLARG;

    err = cwo_Object_createEmpty(&mo, size);
    if (err) return err;

    mo->type = type;
    mo->base = base;
    *(void **)self_ptr = mo;
    return CWO_SUCCESS;
}

SOEXPORT int
cwo_Object_clone(void *self, void *clone_ptr)
{
    int err;
    cwo_Object *o = self;
    void **clone = clone_ptr;

    if (!o || !clone) return CWOERR_NULLARG;
    if (strncmp(o->magic, MAGIC, 4)) return CWOERR_INVARG;

    err = cwoint_alloc(clone, o->size);
    if (err) return err;

    memcpy(*clone, o, o->size);
    return CWO_SUCCESS;
}

SOEXPORT const cwo_Object *
cwo_Object_instance(void)
{
    return &instance;
}

SOEXPORT int
cwo_Object_isObject(void *self)
{
    cwo_Object *o = self;

    if (!o) return 0;
    if (strncmp(o->magic, MAGIC, 4)) return 0;

    return 1;
}

SOEXPORT int
cwo_Object_isInstanceOf(void *self, const cwo_Type *type)
{
    const cwo_Type *objType;
    int err;

    if (!cwo_Object_isObject(self)) return 0;
    err = cwo_Object_typeOf(self, &objType);
    if (err) return 0;

    while (objType)
    {
	if (objType == type) return 1;
	objType = cwo_Type_getBase(objType);
    }

    return 0;
}

SOEXPORT int
cwo_Object_typeOf(void *self, const cwo_Type **type)
{
    cwo_Object *o = self;

    if (!o) return CWOERR_NULLARG;
    if (strncmp(o->magic, MAGIC, 4)) return CWOERR_INVARG;

    *type = o->type;
    return CWO_SUCCESS;
}

SOEXPORT int
cwo_Object_destroy(void *self)
{
    int err;
    struct cwo_MutableObject_s *mo = self;

    if (!mo) return CWOERR_NULLARG;
    if (strncmp(mo->magic, MAGIC, 4)) return CWOERR_INVARG;

    err = cwo_Type_destroyInstance(mo->type, mo);
    if (err) return err;

    if (mo->base)
    {
	err = cwo_Object_destroy((void *)(mo->base));
	if (err) return err;
    }

    free(mo);

    return CWO_SUCCESS;
}
