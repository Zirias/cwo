#ifndef CWO_TYPE_H
#define CWO_TYPE_H

#include <cwo/common.h>
#include <cwo/string.h>

typedef int (*cwo_MethodCall)();
typedef void *(*cwo_PropertyGet)(void *object);
typedef void (*cwo_PropertySet)(void *object, void *value);
typedef void (*cwo_Destructor)(void *object);

struct cwo_Method_s;
typedef struct cwo_Method_s cwo_Method;

struct cwo_Property_s;
typedef struct cwo_Property_s cwo_Property;

struct cwo_TypeDescriptor_s;
typedef struct cwo_TypeDescriptor_s cwo_TypeDescriptor;

struct cwo_Type_s;
typedef struct cwo_Type_s cwo_Type;

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
cwo_Type_register(const cwo_Type **type,
	const cwo_String *name,
	const cwo_Type *base,
	const cwo_TypeDescriptor *overrides);

DECLEXPORT int
cwo_Type_destroyInstance(const cwo_Type *self, void *instance);

#ifdef CWO_NSIMPORT
#define MethodCall cwo_MethodCall
#define PropertyGet cwo_PropertyGet
#define PropertySet cwo_PropertySet
#define Destructor cwo_Destructor
#define Method cwo_Method
#define Property cwo_Property
#define TypeDescriptor cwo_TypeDescriptor
#define Type cwo_Type
#define Method_create cwo_Method_create
#define Method_destroy cwo_Method_destroy
#define Property_create cwo_Property_create
#define Property_destroy cwo_Property_destroy
#define Type_register cwo_Type_register
#define Type_destroyInstance cwo_Type_destroyInstance
#endif

#endif
