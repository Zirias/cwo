#include "type.h"
#include "object.h"
#include "internal/hashtbl.h"
#include "internal/errors.h"

#include <stdarg.h>

struct cwo_Method_s
{
    cwo_Object object;
    const cwo_String *name;
    cwo_MethodCall call;
    int numArgs;
    const cwo_Type *argTypes[1];
};

struct cwo_Property_s
{
    cwo_Object object;
    const cwo_String *name;
    cwo_PropertyGet get;
    cwo_PropertySet set;
    const cwo_Type *type;
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

static const cwo_Type *t_method = 0;
static const cwo_Type *t_property = 0;
static const cwo_Type *t_typeDescriptor = 0;
static const cwo_Type *t_type = 0;

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
    cwo_Type *lt_object = 0;
    cwo_Type *lt_type = 0;
    cwo_Type *lt_typeDescriptor = 0;
    cwo_Type *lt_method = 0;
    cwo_Type *lt_property = 0;
    cwo_Type *lt_string = 0;

    err = cwoint_Hashtbl_create(&types, CWOINT_HASHTBL_256, &destroyType);
    if (err) return err;
    
    err = cwo_Object_createEmpty(&lt_object, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&lt_type, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&lt_typeDescriptor, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&lt_method, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&lt_property, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_createEmpty(&lt_string, sizeof(cwo_Type));
    if (err) goto s_c_type_init_fail;

    /* inject type of root object */
    err = cwo_Object_setType((void *)cwo_Object_instance(), lt_object);
    if (err) goto s_c_type_init_fail;

    /* set our types */
    t_method = lt_method;
    t_property = lt_property;
    t_typeDescriptor = lt_typeDescriptor;
    t_type = lt_type;

    err = cwo_String_create(&(lt_object->name), "cwo_object");
    if (err) goto s_c_type_init_fail;

s_c_type_init_fail:
    destroyType(lt_string);
    destroyType(lt_property);
    destroyType(lt_method);
    destroyType(lt_typeDescriptor);
    destroyType(lt_type);
    destroyType(lt_object);
    cwoint_Hashtbl_destroy(types);
    types = 0;
    return err;
}

SOEXPORT int
cwo_Method_create(cwo_Method **self, const cwo_String *name,
	cwo_MethodCall call, int numArgs, ...)
{
    va_list ap;
    int err;
    int i;

    if (numArgs < 0) return CWOERR_INVARG;

    err = cwo_Object_create(self, sizeof(cwo_Method) - sizeof(cwo_Type *)
	    + (size_t)numArgs * sizeof(cwo_Type *),
	    t_method, cwo_Object_instance());
    if (err) return err;

    (*self)->name = name;
    (*self)->call = call;
    (*self)->numArgs = numArgs;

    va_start(ap, numArgs);
    for (i = 0; i < numArgs; ++i)
    {
	(*self)->argTypes[i] = va_arg(ap, const cwo_Type *);
    }
    va_end(ap);

    return CWO_SUCCESS;
}

SOEXPORT void
cwo_Method_destroy(cwo_Method *self)
{
    free(self);
}

SOEXPORT int
cwo_Property_create(cwo_Property **self, const cwo_String *name,
	const cwo_Type *type, cwo_PropertyGet get, cwo_PropertySet set)
{
    int err;

    err = cwo_Object_create(self, sizeof(cwo_Property),
	    t_property, cwo_Object_instance());
    if (err) return err;

    (*self)->name = name;
    (*self)->type = type;
    (*self)->get = get;
    (*self)->set = set;

    return CWO_SUCCESS;
}

SOEXPORT void
cwo_Property_destroy(cwo_Property *self)
{
    free(self);
}

SOEXPORT int
cwo_TypeDescriptor_create(cwo_TypeDescriptor **self)
{
    int err;
    err = cwo_Object_create(self, sizeof(cwo_TypeDescriptor),
	    t_typeDescriptor, cwo_Object_instance());
    if (err) return err;

    err = cwoint_Hashtbl_create(&((*self)->methods), CWOINT_HASHTBL_16,
	    (cwo_Destructor)&cwo_Method_destroy);
    if (err) return err;

    err = cwoint_Hashtbl_create(&((*self)->properties), CWOINT_HASHTBL_16,
	    (cwo_Destructor)&cwo_Property_destroy);
    if (err) return err;

    return CWO_SUCCESS;
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

