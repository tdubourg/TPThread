#define MAP
#include "lib.h"

int main(int argc, char** argv) {
#ifdef MAP
	printf("Beginning\n");
#endif
	readMyFileThreadedN_And_Memoized("numbers2-64b.txt", 4);
#ifdef MAP
	printf("Terminé1\n");
#endif
	pthread_exit(NULL);
#ifdef MAP
	printf("Test....\n");
#endif
	return 0;
}
