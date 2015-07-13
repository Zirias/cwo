#include "type.h"
#include "object.h"
#include "internal/hashtbl.h"
#include "internal/errors.h"

struct cwo_Method_s
{
    cwo_Object object;
    cwo_String *name;
    int (*call)();
    int numArgs;
    cwo_Type **argTypes;
};

struct cwo_Property_s
{
    cwo_Object object;
    cwo_String *name;
    void *(*get)(void);
    void (*set)(void *);
    cwo_Type *type;
};

struct cwo_TypeDescriptor_s
{
    cwo_Object object;
    cwo_Method *constructor;
    void (*destructor)(void *object);
    cwoint_Hashtbl *methods;
    cwoint_Hashtbl *properties;
};

struct cwo_Type_s
{
    cwo_Object object;
    cwo_String *name;
    cwo_Type *base;
    cwo_TypeDescriptor *desc;
};

static cwoint_Hashtbl *types = 0;

static void
destroyType(void *type)
{
    cwo_Type *t = type;

    if (!t) return;

    free(t);
}

/* bootstrap type system */
static int
init(void)
{
    int err;
    cwo_Type *t_object = 0;
    cwo_Type *t_type = 0;
    cwo_Type *t_typeDescriptor = 0;
    cwo_Type *t_method = 0;
    cwo_Type *t_property = 0;
    cwo_Type *t_string = 0;

    err = cwoint_Hashtbl_create(&types, CWOINT_HASHTBL_256, &destroyType);
    if (err) return err;
    
    err = cwo_Object_createEmpty(&t_object, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&t_type, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&t_typeDescriptor, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&t_method, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&t_property, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&t_string, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_String_create(&(t_object->name), "cwo_object");
    if (err) goto s_c_type_init_fail;

s_c_type_init_fail:
    destroyType(t_string);
    destroyType(t_property);
    destroyType(t_method);
    destroyType(t_typeDescriptor);
    destroyType(t_type);
    destroyType(t_object);
    cwoint_Hashtbl_destroy(types);
    types = 0;
    return err;
}

SOEXPORT int
cwo_Type_register(const cwo_Type **type, const cwo_String *name,
		const cwo_Type *base, const cwo_TypeDescriptor *overrides)
{
    return CWOERR_NOTIMP;
}

SOEXPORT int
cwo_Type_destroyInstance(const cwo_Type *self, void *instance)
{
    return CWOERR_NOTIMP;
}

