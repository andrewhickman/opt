#include "assert.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

struct opt_error {
	enum opt_error_type {
		OPT_NONE,
		OPT_FINISHED_ITER,
		OPT_UNEXPECTED_ARG,
		OPT_UNEXPECTED_FLAG,
		OPT_MISSING_ARG,
		OPT_MISSING_FLAG,
	} type;
	union opt_error_data {
		char const *arg;    // used by UNEXPECTED_ARG
		char        flag;   // used by UNEXPECTED_FLAG and MISSING_ARG
	} data;
};

struct opt_iter {
	char             flag;
	char const      *val;
	char const      *pos;
	int              index;
	struct opt_error error;
};

static inline void 
opt_set_error(struct opt_iter *it, enum opt_error_type type) 
{
	it->error.type = type;
	switch (type) {
	case OPT_NONE:
		return;
	case OPT_UNEXPECTED_ARG:
		it->error.data.arg = it->pos;
		break;
	case OPT_UNEXPECTED_FLAG:
	case OPT_MISSING_ARG:
		it->error.data.flag = *it->pos;
		break;
	case OPT_MISSING_FLAG:
	case OPT_FINISHED_ITER:
		break;
	}
	it->flag = '?';
}

static inline void 
opt_print_error(struct opt_error error) 
{
	switch (error.type) {
	case OPT_NONE:
		printf("No errors\n");
		break;
	case OPT_FINISHED_ITER:
		printf("Parsed all arguments\n");
		break;
	case OPT_UNEXPECTED_ARG:
		printf("Got unexpected argument: %s\n", error.data.arg);
		break;
	case OPT_UNEXPECTED_FLAG:
		printf("Got unexpected flag: -%c\n", error.data.flag);
		break;
	case OPT_MISSING_ARG:
		printf("Missing argument for flag: -%c\n", error.data.flag);
		break;
	case OPT_MISSING_FLAG:
		printf("Missing flag\n");
		break;
	}
}

static inline char const*
opt_get_arg(struct opt_iter *it, int argc, char const *const *argv)
{
	if (it->pos[1] != '\0') {    // val is the rest of the arg
		return ++it->pos;
	}    // val is the next arg
	if (it->index == argc - 1) {    // end of arg list
		opt_set_error(it, OPT_MISSING_ARG);
		return NULL;
	}
	++it->index;
	it->pos = argv[it->index];
	if (it->pos[0] == '-') {    // found more flags
		opt_set_error(it, OPT_MISSING_ARG);
		return NULL;
	}
	return it->pos;
}

static inline bool 
opt_next(struct opt_iter *it, int argc, char const *const *argv,
         char const *flags) 
{
	assert(it);
	assert(argc >= 0);
	assert(argv);
	assert(flags);

	opt_set_error(it, OPT_NONE);
	if (!it->pos) {    // go to the next argument
		if (it->index == argc) {    // end of argument list
			opt_set_error(it, OPT_FINISHED_ITER);
			return false;
		}
		it->pos = argv[it->index];
		if (*it->pos != '-') {
			opt_set_error(it, OPT_UNEXPECTED_ARG);
			goto next_arg;
		}
		++it->pos;
		if (*it->pos == '\0') {    // '-' char with no flags
			opt_set_error(it, OPT_MISSING_FLAG);
			goto next_arg;
		}
	}

	it->flag = *it->pos;
	it->val = NULL;
	char const *flag_pos = strchr(flags, it->flag);
	if (!flag_pos) {
		opt_set_error(it, OPT_UNEXPECTED_FLAG);
		goto next_flag;
	}
	if (flag_pos[1] == ':') {    // flag has argument
		it->val = opt_get_arg(it, argc, argv);
		goto next_arg;
	}

next_flag:    // go to the next flag in the current argument (if any)
	++it->pos;
	if (*it->pos == '\0') {
next_arg:    // go to the next argument
		++it->index;
		it->pos = NULL;
	}
	return true;
}
