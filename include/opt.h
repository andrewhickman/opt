#include "stdbool.h"
#include "stddef.h"

struct opt_iter {
	char        flag;
	char const *arg;

	char const *pos;
	int         index;
};

static inline struct opt_iter
opt_init()
{
	return (struct opt_iter){ 0 };
}

static inline void
opt_incr(struct opt_iter *it)
{
	++it->index;
	it->pos = NULL;
}

static inline void
opt_decr(struct opt_iter *it)
{
	--it->index;
	it->pos = NULL;
}

static inline bool 
opt_next(struct opt_iter *it, int argc, char const *const *argv)
/* Iterate through flags and arguments */
{
	while (!it->pos) {
		if (it->index == argc) {
			return false;
		}
		it->pos = argv[it->index];
		if (*it->pos != '-') {
			it->flag = ':';
			it->arg = it->pos;
			opt_incr(it);
			return true;
		}
		++it->pos;
		if (*it->pos == '\0') {
			opt_incr(it);
		}
	}

	it->flag = *it->pos;
	it->arg = ++it->pos;
	if (*it->pos == '\0') {
		opt_incr(it);
	}
	return true;
}

static inline bool
opt_next_arg(struct opt_iter *it, int argc, char const *const *argv)
/* Ignore flags, parse next argument */
{
	if (it->pos) {
		opt_incr(it);
	}
	if (it->index == argc) {
		return false;
	}
	it->flag = ':';
	it->arg = it->pos = argv[it->index];

	opt_incr(it);
	return true;
}

static inline char const*
opt_parse_arg(struct opt_iter *it, int argc, char const *const *argv)
{
	if (!opt_next_arg(it, argc, argv)) {
		return NULL;
	}
	return it->arg;
}

static inline char const*
opt_parse_arg_strict(struct opt_iter *it, int argc, char const *const *argv)
{
	if (!opt_next_arg(it, argc, argv)) {
		return NULL;
	} else if (it->arg[0] == '-') {
		opt_decr(it);
		return NULL;
	}
	return it->arg;
}


