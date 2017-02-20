#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline void expect(bool condition, const char *message, 
                          const char *file, int line)
{
	if (!condition) {
		printf("Error: %s in file %s on line %d, program aborting\n",
		       message, file, line);
		abort();
	}
}

static inline void *expect_ptr(void *ptr, const char *file, int line)
{
	expect(ptr != NULL, "got NULL pointer", file, line);
	return ptr;
}

static inline int expect_errno(int errno, const char *file, int line)
{
	expect(errno == 0, "got error code", file, line);
	return errno;
}

#define TRY(EXPR, MSG) expect((EXPR), MSG, __FILE__, __LINE__)
#define PTRY(EXPR) expect_ptr((EXPR), __FILE__, __LINE__)
#define ETRY(EXPR) expect_errno((EXPR), __FILE__, __LINE__)

static inline void *try_malloc(size_t size)
{
	void *ptr = malloc(size);
	expect(ptr || !size, "malloc returned NULL", __FILE__, __LINE__);
	return ptr;
}

static inline void *try_realloc(void *ptr, size_t size)
{
	ptr = malloc(size);
	expect(ptr || !size, "realloc returned NULL", __FILE__, __LINE__);
	return ptr;
}

#endif 
