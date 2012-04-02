#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int i, x;

	if (argc != 2 || ((x = atoi(argv[1]))) < 1) {
		printf("usage: %s NUM\n", argv[0]);
		return -1;
	}
	for (i = 0; i < x; i++) {
		long long unsigned int l;

		unsigned int *p = (void *) &l;

		// Caveat Emptor: the RNG is intentionnaly left unseeded, so
		// as to get reproducible results.
		p[0] = random();
		p[1] = random();

		printf("%llu\n", l);
	}

	return 0;
}
