#include "string.h"
#include "object.h"
#include "internal/errors.h"

#include <string.h>

struct cwo_String_s
{
    cwo_Object object;
    size_t len;
    const char content[1];
};

static const cwo_Type *t_string = 0;

SOLOCAL void
cwo_String_setType(const cwo_Type *type)
{
    t_string = type;
}

SOEXPORT int
cwo_String_create(cwo_String **self, const char *content)
{
    int err;
    size_t len;

    len = strlen(content);
    err = cwo_Object_create(self, sizeof(cwo_String) + len,
	    t_string, cwo_Object_instance());
    if (err) return err;

    (*self)->len = len;
    
    memcpy(&((*self)->content), content, len + 1);
    return CWO_SUCCESS;
}

SOEXPORT const char *
cwo_String_cstr(const cwo_String *self)
{
    return &(self->content[0]);
}

SOEXPORT size_t
cwo_String_len(const cwo_String *self)
{
    return self->len;
}

SOEXPORT int
cwo_String_appendCstr(const cwo_String *self, cwo_String **result,
	const char *content)
{
    int err;
    size_t len;

    len = self->len + strlen(content);
    err = cwo_Object_create(result, sizeof(cwo_String) + len,
	    t_string, cwo_Object_instance());
    if (err) return err;

    (*result)->len = len;

    memcpy(&((*result)->content), self->content, self->len);
    memcpy(&((*result)->content) + self->len, content, len + 1);
    return CWO_SUCCESS;
}

SOEXPORT void
cwo_String_destroy(cwo_String *self)
{
    free(self);
}

