#ifndef SLICE_H
#define SLICE_H

#include "error.h"
#include "macro.h"

#include <stddef.h>

#ifndef TYPE
    #define TYPE void*
#endif

#ifndef NAME
    #define NAME slice
#endif

struct NAME {
	TYPE  *data;
	size_t size;
};

struct NAME CAT(NAME, _new)() {
	return (struct NAME){ try_malloc(10 * sizeof(TYPE)), 10 };
}

void CAT(NAME, _drop)(struct NAME *slice) {
	free(slice->data);
}

#undef TYPE
#undef NAME

#endif
