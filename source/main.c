#include "opt.h"

int main(int argc, char const *argv[])
{
	struct opt_iter it = { 0 };
	opt_next(&it, argc, argv, "c:abd:");
	while (opt_next(&it, argc, argv, "d:abc:")) {
		switch (it.flag) {
		case 'a':
		case 'b':
			printf("Got arg -%c\n", it.flag);
			break;
		case 'c':
		case 'd':
			printf("Got arg -%c with val %s\n", it.flag, it.val);
			break;
		case '?':
			printf("Error: ");
			opt_print_error(it.error);
			break;
		}
	}
}
