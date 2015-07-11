#include "utils.h"
#include "errors.h"

int
cwoint_alloc(void *ptr_object, size_t size)
{
    void **object = ptr_object;

    if (size <= 0)
    {
	return CWOERR_INVARG;
    }

    if (!object)
    {
	return CWOERR_NULL;
    }

    *object = malloc(size);
    if (*object)
    {
	return CWO_SUCCESS;
    }
    else
    {
	return CWOERR_NOMEM;
    }
}

