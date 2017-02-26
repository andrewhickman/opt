#include "stdbool.h"
#include "stddef.h"

struct opt_iter {
	char        flag;
	char const *arg;
	char const *pos;
	int         index;
};

static inline void
_opt_incr(struct opt_iter *it)
{
	++it->index;
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
			_opt_incr(it);
			return true;
		}
		++it->pos;
		if (*it->pos == '\0') {
			_opt_incr(it);
		}
	}

	it->flag = *it->pos;
	it->arg = NULL;

	++it->pos;
	if (*it->pos == '\0') {
		_opt_incr(it);
	}
	return true;
}

static inline bool
opt_next_arg(struct opt_iter *it, int argc, char const *const *argv)
/* Force parsing as argument */
{
	if (!it->pos) {
		if (it->index == argc) {
			return false;
		}
		it->pos = argv[it->index];
	}
	it->flag = ':';
	it->arg = it->pos;

	_opt_incr(it);
	return true;
}

