#include "hashtbl.h"
#include "errors.h"
#include "utils.h"

#include <string.h>

struct entry
{
    void *key;
    size_t keyLen;
    void *element;
    struct entry *next;
};

struct tblsize
{
    const size_t entries;
    const unsigned int hashmask;
};

struct cwoint_Hashtbl_s
{
    cwoint_Hashtbl_destroyElement elementDtor;
    const struct tblsize *size;
    struct entry* entries[1];
};

static const struct tblsize tblsizes[CWOINT_HASHTBL_NUMSIZES] = {
    { 16, 0xf },
    { 64, 0x3f },
    { 128, 0x7f },
    { 256, 0xff }
};

static unsigned int
hash(const void *key, size_t keyLen, unsigned int hashmask)
{
    size_t i;
    unsigned int h = 5381;

    for (i=0; i<keyLen; ++i)
    {
	h += (h << 5) + ((const unsigned char *)key)[i];
    }

    return h & hashmask;
}

static int
entryHasKey(const struct entry *entry, const void *key, size_t keyLen)
{
    if (entry->keyLen != keyLen) return 0;
    return (!strncmp((const char *)(entry->key), (const char *)key, keyLen));
}

int
cwoint_Hashtbl_create(cwoint_Hashtbl **self, enum cwoint_HashtblSize size,
	cwoint_Hashtbl_destroyElement elementDtor)
{
    int err = CWO_SUCCESS;

    if ((int)size < 0 || size >= CWOINT_HASHTBL_NUMSIZES)
    {
	return CWOIERR_HTSIZE;
    }

    err = cwoint_alloc(self, sizeof(struct cwoint_Hashtbl_s)
	    + (tblsizes[size].entries-1) * sizeof(struct entry *));
    if (err) return err;

    memset((*self)->entries, 0,
	    tblsizes[size].entries * sizeof(struct entry *));
    (*self)->elementDtor = elementDtor;
    (*self)->size = &(tblsizes[size]);

    return CWO_SUCCESS;
}

void
cwoint_Hashtbl_destroy(cwoint_Hashtbl *self)
{
    struct entry *current, *last;
    size_t i;

    for (i = 0; i < self->size->entries; ++i)
    {
	current = self->entries[i];
	while (current)
	{
	    last = current;
	    current = current->next;
	    free(last->key);
	    if (self->elementDtor) self->elementDtor(last->element);
	    free(last);
	}
    }

    free(self);
}

int
cwoint_Hashtbl_insert(cwoint_Hashtbl *self, const void *key,
	size_t keyLen, void *element)
{
    struct entry *current;
    struct entry *myEntry = 0;
    int err = CWO_SUCCESS;
    unsigned int pos = hash(key, keyLen, self->size->hashmask);

    err = cwoint_alloc(&myEntry, sizeof(struct entry));
    if (err) return err;

    err = cwoint_alloc(&(myEntry->key), keyLen);
    if (err)
    {
	free(myEntry);
	return err;
    }

    memcpy(myEntry->key, key, keyLen);
    myEntry->keyLen = keyLen;
    myEntry->element = element;
    myEntry->next = 0;

    if ((current = self->entries[pos]))
    {
	while (current->next) current = current->next;
	current->next = myEntry;
    }
    else
    {
	self->entries[pos] = myEntry;
    }

    return CWO_SUCCESS;
}

void *
cwoint_Hashtbl_get(const cwoint_Hashtbl *self, const void *key, size_t keyLen)
{
    unsigned int pos = hash(key, keyLen, self->size->hashmask);
    struct entry *current = self->entries[pos];

    while (current && !entryHasKey(current, key, keyLen))
    {
	current = current->next;
    }
    return current ? current->element : 0;
}

int
cwoint_Hashtbl_remove(cwoint_Hashtbl *self, const void *key, size_t keyLen)
{ 
    struct entry *current, *last;
    unsigned int pos = hash(key, keyLen, self->size->hashmask);

    last = 0;
    current = self->entries[pos];

    while (current && !entryHasKey(current, key, keyLen))
    {
	last = current;
	current = current->next;
    }

    if (!current) return CWOERR_NOENT;

    if (last)
    {
	last->next = current->next;
    }
    else
    {
	self->entries[pos] = current->next;
    }

    free(current->key);
    if (self->elementDtor) self->elementDtor(current->element);
    free(current);

    return CWO_SUCCESS;
}

