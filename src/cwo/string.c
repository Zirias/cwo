#include "string.h"
#include "object.h"
#include "internal/errors.h"
#include "internal/utils.h"

#include <string.h>

struct cwo_String_s
{
    cwo_Object object;
    size_t len;
    char *content;
};

static const cwo_Type *type = 0;

SOEXPORT int
cwo_String_create(cwo_String **self, const char *content)
{
    int err;

    err = cwo_Object_create(self, sizeof(cwo_String),
	    type, cwo_Object_instance());
    if (err) return err;

    (*self)->len = strlen(content);
    
    err = cwoint_alloc(&((*self)->content), (*self)->len + 1);
    if (err)
    {
	free(*self);
	return err;
    }
    
    memcpy((*self)->content, content, (*self)->len + 1);
    return CWO_SUCCESS;
}

SOEXPORT const char *
cwo_String_cstr(const cwo_String *self)
{
    return self->content;
}

SOEXPORT size_t
cwo_String_len(const cwo_String *self)
{
    return self->len;
}

SOEXPORT void
cwo_String_destroy(cwo_String *self)
{
    free(self->content);
    free(self);
}

