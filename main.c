#define MAP
#include "lib.h"

int main(int argc, char** argv) {
#ifdef MAP
	printf("Beginning\n");
#endif
	readMyFileThreadedN_And_Memoized("numbers2.txt", 1);
#ifdef MAP
	printf("Terminé1\n");
#endif
	detruire_arbre(MEM_TREE);
#ifdef MAP
	printf("Terminé2\n");
#endif
	pthread_exit(NULL);
#ifdef MAP
	printf("Test....\n");
#endif
	return 0;
}
