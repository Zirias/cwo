#ifndef CWOINT_HASHTBL
#define CWOINT_HASHTBL

#include <stdlib.h>

enum cwoint_HashtblSize
{
    CWOINT_HASHTBL_16,
    CWOINT_HASHTBL_64,
    CWOINT_HASHTBL_128,
    CWOINT_HASHTBL_256,
    CWOINT_HASHTBL_NUMSIZES
};

typedef void (*cwoint_Hashtbl_destroyElement)(void *element);

struct cwoint_Hashtbl_s;
typedef struct cwoint_Hashtbl_s cwoint_Hashtbl;

int
cwoint_Hashtbl_create(cwoint_Hashtbl **self, enum cwoint_HashtblSize size,
	cwoint_Hashtbl_destroyElement elementDtor);

void
cwoint_Hashtbl_destroy(cwoint_Hashtbl *self);

int
cwoint_Hashtbl_insert(cwoint_Hashtbl *self,
	const void *key, size_t keyLen, void *element);

void *
cwoint_Hashtbl_get(const cwoint_Hashtbl *self,
	const void *key, size_t keyLen);

int
cwoint_Hashtbl_remove(cwoint_Hashtbl *self, const void *key, size_t keyLen);

#endif

