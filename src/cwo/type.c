#include "type.h"
#include "object.h"
#include "string.h"
#include "internal/hashtbl.h"
#include "internal/errors.h"

#include <stdarg.h>
#include <string.h>

struct cwo_Method_s
{
    cwo_Object object;
    cwo_String *name;
    cwo_MethodCall call;
    int numArgs;
    const cwo_Type *argTypes[1];
};

struct cwo_Property_s
{
    cwo_Object object;
    cwo_String *name;
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
    const cwo_Type *base;
    cwo_TypeDescriptor *desc;
};

static cwoint_Hashtbl *types = 0;

static const cwo_Type generic = { {"cwoG", 0, 0, 0}, 0, 0, 0 };
SOEXPORT const cwo_Type * const cwo_GenericType = &generic;

static const cwo_Type *t_method = 0;
static const cwo_Type *t_property = 0;
static const cwo_Type *t_typeDescriptor = 0;
static const cwo_Type *t_type = 0;

static void
destroyType(void *type)
{
    cwo_Type *t = type;

    if (!t) return;

    if (t->name) cwo_String_destroy(t->name);
    if (t->desc) cwo_TypeDescriptor_destroy(t->desc);

    free(t);
}

/* bootstrap type system */
SOLOCAL int
cwo_Type_bootstrap(void)
{
    int err;
    cwo_Type *lt_object = 0;
    cwo_Type *lt_type = 0;
    cwo_Type *lt_typeDescriptor = 0;
    cwo_Type *lt_method = 0;
    cwo_Type *lt_property = 0;
    cwo_Type *lt_string = 0;
    cwo_TypeDescriptor *desc = 0;
    cwo_Method *method = 0;
    cwo_Property *property = 0;
    cwo_String *tmp = 0;
    const char *tname;

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

    /* inject type of string class */
    cwo_String_setType(lt_string);

    /* base configuration of types */
    err = cwo_Object_setType(lt_object, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_object, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_object->name), "cwo_Object");
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_setType(lt_type, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_type, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_type->name), "cwo_Type");
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_setType(lt_typeDescriptor, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_typeDescriptor, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_typeDescriptor->name), "cwo_TypeDescriptor");
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_setType(lt_method, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_method, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_method->name), "cwo_Method");
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_setType(lt_property, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_property, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_property->name), "cwo_Property");
    if (err) goto s_c_type_init_fail;

    err = cwo_Object_setType(lt_string, lt_type);
    if (err) goto s_c_type_init_fail;
    err = cwo_Object_setBase(lt_string, (void *)cwo_Object_instance());
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&(lt_string->name), "cwo_String");
    if (err) goto s_c_type_init_fail;

    /* set our local types */
    t_method = lt_method;
    t_property = lt_property;
    t_typeDescriptor = lt_typeDescriptor;
    t_type = lt_type;

    /* descriptors for types */
    err = cwo_TypeDescriptor_create(&desc);
    if (err) goto s_c_type_init_fail;
    err = cwo_String_create(&tmp, "clone");
    if (err) goto s_c_type_init_fail;
    err = cwo_Method_create(&method,
	    tmp, &cwo_Object_clone, 1, cwo_GenericType);
    if (err) goto s_c_type_init_fail;
    err = cwo_TypeDescriptor_addMethod(desc, method);
    if (err) goto s_c_type_init_fail;
    tmp = 0;
    method = 0;
    err = cwo_String_create(&tmp, "toString");
    if (err) goto s_c_type_init_fail;
    err = cwo_Method_create(&method,
	    tmp, &cwo_Object_toString, 1, cwo_GenericType);
    if (err) goto s_c_type_init_fail;
    err = cwo_TypeDescriptor_addMethod(desc, method);
    if (err) goto s_c_type_init_fail;
    tmp = 0;
    method = 0;
    lt_object->desc = desc;
    desc = 0;

    /* register core types */
    tname = cwo_String_cstr(lt_object->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_object);
    if (err) goto s_c_type_init_failobject;
    tname = cwo_String_cstr(lt_type->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_type);
    if (err) goto s_c_type_init_failtype;
    tname = cwo_String_cstr(lt_typeDescriptor->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_typeDescriptor);
    if (err) goto s_c_type_init_failtypedescriptor;
    tname = cwo_String_cstr(lt_method->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_method);
    if (err) goto s_c_type_init_failmethod;
    tname = cwo_String_cstr(lt_property->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_property);
    if (err) goto s_c_type_init_failproperty;
    tname = cwo_String_cstr(lt_string->name);
    err = cwoint_Hashtbl_insert(types, tname, strlen(tname), lt_string);
    if (err) goto s_c_type_init_failstring;

    return CWO_SUCCESS;

s_c_type_init_fail:
    cwo_String_destroy(tmp);
    cwo_Method_destroy(method);
    cwo_Property_destroy(property);
    cwo_TypeDescriptor_destroy(desc);
s_c_type_init_failobject:
    destroyType(lt_object);
s_c_type_init_failtype:
    destroyType(lt_type);
s_c_type_init_failtypedescriptor:
    destroyType(lt_typeDescriptor);
s_c_type_init_failmethod:
    destroyType(lt_method);
s_c_type_init_failproperty:
    destroyType(lt_property);
s_c_type_init_failstring:
    destroyType(lt_string);
    cwoint_Hashtbl_destroy(types);
    types = 0;
    return err;
}

SOLOCAL void
cwo_Type_cleanup(void)
{
    cwoint_Hashtbl_destroy(types);
    types = 0;
}

SOEXPORT int
cwo_Method_create(cwo_Method **self, cwo_String *name,
	cwo_MethodCall call, int numArgs, ...)
{
    va_list ap;
    int err;
    int i;

    if (!self || !name || !call) return CWOERR_NULLARG;
    if (numArgs < 0 || numArgs > CWO_METHOD_MAXARGS) return CWOERR_INVARG;

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

SOEXPORT int
cwo_Method_call(cwo_Method *self, void *object, ...)
{
    va_list ap;
    int i;
    void *argObj;
    void *arg[CWO_METHOD_MAXARGS];

    if (!cwo_Object_isObject(object)) return CWOERR_INVARG;

    va_start(ap, object);
    for (i = 0; i < self->numArgs; ++i)
    {
	argObj = va_arg(ap, void *);
	if (!cwo_Object_isInstanceOf(argObj, self->argTypes[i]))
	{
	    return CWOERR_INCARG;
	}
	arg[i] = argObj;
    }
    va_end(ap);

    /* UGLY hack ... whatever ... nothing to see here */
    switch (self->numArgs)
    {
	case 0: return self->call(object);
	case 1: return self->call(object, arg[0]);
	case 2: return self->call(object, arg[0], arg[1]);
	case 3: return self->call(object, arg[0], arg[1], arg[2]);
	case 4: return self->call(object, arg[0], arg[1], arg[2], arg[3]);
	case 5: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4]);
	case 6: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5]);
	case 7: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6]);
	case 8: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7]);
	case 9: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8]);
	case 10: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	case 11: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10]);
	case 12: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10], arg[11]);
	case 13: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10], arg[11], arg[12]);
	case 14: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10], arg[11], arg[12], arg[13]);
	case 15: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10], arg[11], arg[12], arg[13], arg[14]);
	case 16: return self->call(object, arg[0], arg[1], arg[2], arg[3],
			 arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],
			 arg[10], arg[11], arg[12], arg[13], arg[14], arg[15]);
    }

    return CWOERR_BUG;
}

SOEXPORT void
cwo_Method_destroy(cwo_Method *self)
{
    cwo_String_destroy(self->name);
    free(self);
}

SOEXPORT int
cwo_Property_create(cwo_Property **self, cwo_String *name,
	const cwo_Type *type, cwo_PropertyGet get, cwo_PropertySet set)
{
    int err;

    if (!self || !name || !type) return CWOERR_NULLARG;

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
    cwo_String_destroy(self->name);
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
cwo_TypeDescriptor_addMethod(cwo_TypeDescriptor *self, cwo_Method *method)
{
    cwo_Method *tmp;
    const char *name = cwo_String_cstr(method->name);

    tmp = cwoint_Hashtbl_get(self->methods, name, strlen(name));
    if (tmp) return CWOERR_EXISTS;

    return cwoint_Hashtbl_insert(self->methods, name, strlen(name), method);
}

SOEXPORT int
cwo_TypeDescriptor_addProperty(cwo_TypeDescriptor *self, cwo_Property *property)
{
    cwo_Property *tmp;
    const char *name = cwo_String_cstr(property->name);

    tmp = cwoint_Hashtbl_get(self->properties, name, strlen(name));
    if (tmp) return CWOERR_EXISTS;

    return cwoint_Hashtbl_insert(
	    self->properties, name, strlen(name), property);
}

SOEXPORT void
cwo_TypeDescriptor_destroy(cwo_TypeDescriptor *self)
{
    cwoint_Hashtbl_destroy(self->properties);
    cwoint_Hashtbl_destroy(self->methods);
    free(self);
}

SOEXPORT int
cwo_Type_register(const cwo_Type **type, cwo_String *name,
		const cwo_Type *base, cwo_TypeDescriptor *desc)
{
    int err;
    cwo_Type *t;
    const char *rawName;

    err = cwo_Object_create(&t, sizeof(cwo_Type),
	    t_type, cwo_Object_instance());
    if (err) return err;

    t->name = name;
    t->base = base;
    t->desc = desc;

    rawName = cwo_String_cstr(name);
    err = cwoint_Hashtbl_insert(types, rawName, strlen(rawName), t);
    if (err)
    {
	destroyType(t);
	return err;
    }

    *type = t;
    return CWO_SUCCESS;
}

SOEXPORT const cwo_String *
cwo_Type_getName(const cwo_Type *self)
{
    return self->name;
}

SOEXPORT const cwo_Type *
cwo_Type_getBase(const cwo_Type *self)
{
    return self->base;
}

SOEXPORT int
cwo_Type_destroyInstance(const cwo_Type *self, void *instance)
{
    return CWOERR_NOTIMP;
}


