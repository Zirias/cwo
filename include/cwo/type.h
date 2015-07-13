#ifndef CWO_TYPE_H
#define CWO_TYPE_H

#include <cwo/common.h>
#include <cwo/string.h>

typedef int (*cwo_MethodCall)();
typedef void *(*cwo_PropertyGet)(void *object);
typedef void (*cwo_PropertySet)(void *object, void *value);
typedef void (*cwo_Destructor)(void *object);

#define CWO_METHOD_MAXARGS 16

struct cwo_Method_s;
typedef struct cwo_Method_s cwo_Method;

struct cwo_Property_s;
typedef struct cwo_Property_s cwo_Property;

struct cwo_TypeDescriptor_s;
typedef struct cwo_TypeDescriptor_s cwo_TypeDescriptor;

struct cwo_Type_s;
typedef struct cwo_Type_s cwo_Type;

DECLEXPORT const cwo_Type * const cwo_GenericType;

DECLEXPORT int
cwo_Method_create(cwo_Method **self, const cwo_String *name,
		cwo_MethodCall call, int numArgs, ...);

DECLEXPORT void
cwo_Method_destroy(cwo_Method *self);

DECLEXPORT int
cwo_Property_create(cwo_Property **self, const cwo_String *name,
		const cwo_Type *type,
		cwo_PropertyGet get, cwo_PropertySet set);

DECLEXPORT void
cwo_Property_destroy(cwo_Property *self);

DECLEXPORT int
cwo_TypeDescriptor_create(cwo_TypeDescriptor **self);

DECLEXPORT int
cwo_TypeDescriptor_addMethod(cwo_TypeDescriptor *self, cwo_Method *method);

DECLEXPORT int
cwo_TypeDescriptor_addProperty(cwo_TypeDescriptor *self,
	cwo_Property *property);

DECLEXPORT void
cwo_TypeDescriptor_destroy(cwo_TypeDescriptor *self);

DECLEXPORT int
cwo_Type_register(const cwo_Type **type,
	const cwo_String *name,
	const cwo_Type *base,
	const cwo_TypeDescriptor *overrides);

DECLEXPORT const cwo_String *
cwo_Type_getName(const cwo_Type *self);

DECLEXPORT const cwo_Type *
cwo_Type_getBase(const cwo_Type *self);

DECLEXPORT int
cwo_Type_destroyInstance(const cwo_Type *self, void *instance);

#ifdef CWO_NSIMPORT
#define MethodCall cwo_MethodCall
#define PropertyGet cwo_PropertyGet
#define PropertySet cwo_PropertySet
#define METHOD_MAXARGS CWO_METHOD_MAXARGS
#define Destructor cwo_Destructor
#define Method cwo_Method
#define Property cwo_Property
#define TypeDescriptor cwo_TypeDescriptor
#define Type cwo_Type
#define GenericType cwo_GenericType
#define Method_create cwo_Method_create
#define Method_destroy cwo_Method_destroy
#define Property_create cwo_Property_create
#define Property_destroy cwo_Property_destroy
#define TypeDescriptor_create cwo_TypeDescriptor_create
#define TypeDescriptor_addMethod cwo_TypeDescriptor_addMethod
#define TypeDescriptor_addProperty cwo_TypeDescriptor_addProperty
#define TypeDescriptor_destroy cwo_TypeDescriptor_destroy
#define Type_register cwo_Type_register
#define Type_getBase cwo_Type_getBase
#define Type_destroyInstance cwo_Type_destroyInstance
#endif

#endif
