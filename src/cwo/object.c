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
};

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
	const cwo_Type *type, void *base)
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
