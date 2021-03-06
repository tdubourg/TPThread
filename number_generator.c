#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int i, x;

	if (argc != 2 || ((x = atoi(argv[1]))) < 1) {
		printf("usage: %s NUM\n", argv[0]);
		return -1;
	}
	for (i = 0; i < x; i++) {
		unsigned int l;

		// Caveat Emptor: the RNG is intentionally left unseeded, so
		// as to get reproducible results.
		l = random();

		printf("%u\n", l);
	}

	return 0;
}
