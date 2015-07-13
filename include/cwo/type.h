#ifndef CWO_TYPE_H
#define CWO_TYPE_H

#include <cwo/common.h>
#include <cwo/string.h>

struct cwo_Method_s;
typedef struct cwo_Method_s cwo_Method;

struct cwo_Property_s;
typedef struct cwo_Property_s cwo_Property;

struct cwo_TypeDescriptor_s;
typedef struct cwo_TypeDescriptor_s cwo_TypeDescriptor;

struct cwo_Type_s;
typedef struct cwo_Type_s cwo_Type;

DECLEXPORT int
cwo_Type_register(const cwo_Type **type,
	const cwo_String *name,
	const cwo_Type *base,
	const cwo_TypeDescriptor *overrides);

DECLEXPORT int
cwo_Type_destroyInstance(const cwo_Type *self, void *instance);

#ifdef CWO_NSIMPORT
#define Method cwo_Method
#define Property cwo_Property
#define TypeDescriptor cwo_TypeDescriptor
#define Type cwo_Type
#define Type_register(n,b,o) cwo_Type_register(n,b,o)
#define Type_destroyInstance(s,i) cwo_Type_destroyInstance(s,i);
#endif

#endif
