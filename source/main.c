#include "opt.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char const *argv[])
{
	char const *name = NULL;
	int verbosity = 0;

	struct opt_iter it = opt_init();
	opt_next_arg(&it, argc, argv);
	while (opt_next(&it, argc, argv)) {
		switch (it.flag) {
		case '-':
			// Long args
			if (strcmp("name", it.arg) == 0) {
				name = opt_parse_arg_strict(&it, argc, argv);
				if (!name) {
					printf("Missing arg for --name\n");
				}
			}
			break;
		case 'v':
			++verbosity;
			break;
		case ':':
			printf("Got unexpected arg %s\n", it.arg);
			break;
		default:
			printf("Got unexpected flag -%c\n", it.flag);
			break;
		}
	}
	printf("Name: %s, verbosity: %d\n", name, verbosity);
}
