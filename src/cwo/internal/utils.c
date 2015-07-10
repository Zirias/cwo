#include "utils.h"
#include "errors.h"

int
cwoint_alloc(void *ptr_object, size_t size)
{
    void **object = ptr_object;
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

