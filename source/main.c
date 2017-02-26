#include "opt.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char const *argv[])
{
	char const *name = NULL;
	bool verbose = false;

	struct opt_iter it = { 0 };
	opt_next_arg(&it, argc, argv);
	while (opt_next(&it, argc, argv)) {
		switch (it.flag) {
		case '-':
			// Long args
			if (strcmp("name", it.arg) == 0) {
				if (!opt_next_arg(&it, argc, argv)) {
					printf("Missing arg for --name\n");
					goto end;
				}
				name = it.arg;	
			}
			break;
		case 'v':
			verbose = true;
			break;
		case ':':
			printf("Got unexpected arg %s\n", it.arg);
			break;
		default:
			printf("Got unexpected flag -%c\n", it.flag);
			break;
		}
	}
end:
	printf("Name: %s, verbose: %s\n", name, verbose ? "true" : "false");
}
