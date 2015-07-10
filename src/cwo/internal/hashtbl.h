#ifndef CWOINT_HASHTBL
#define CWOINT_HASHTBL

#include <stdlib.h>

enum cwoint_hashtbl_size
{
    CWOINT_HASHTBL_16,
    CWOINT_HASHTBL_64,
    CWOINT_HASHTBL_128,
    CWOINT_HASHTBL_256,
    CWOINT_HASHTBL_NUMSIZES
};

typedef void (*cwoint_hashtbl_destroy_element)(void *element);

struct cwoint_hashtbl_s;
typedef struct cwoint_hashtbl_s cwoint_hashtbl;

int
cwoint_hashtbl_create(cwoint_hashtbl **self, enum cwoint_hashtbl_size size,
	cwoint_hashtbl_destroy_element elementDtor);

void
cwoint_hashtbl_destroy(cwoint_hashtbl *self);

int
cwoint_hashtbl_insert(cwoint_hashtbl *self,
	const void *key, size_t keyLen, void *element);

void *
cwoint_hashtbl_get(const cwoint_hashtbl *self,
	const void *key, size_t keyLen);

int
cwoint_hashtbl_remove(cwoint_hashtbl *self, const void *key, size_t keyLen);

#endif

