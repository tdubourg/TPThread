#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "arbre.c"
//#define MAP

#define MAX_NUMBERS MAX_NUMBERS
#define EoT -42

static pthread_mutex_t mid;
static t_arbre* MEM_TREE;

/************************* CORE FUNCTIONS ************************/
int is_prime(unsigned int p) {
	if (p <= 3) {
		return 1;
	} else if (!p) {
		return 0;
	} else if (p <= 7) {
		switch (p) {
			case 5:
			case 7:
				return 1;
				break;
			default:
				return 0;
				break;

		}
	}
	double max = sqrt(p) + 1; //* +1 for the lulz
	int j;
	for (j = 2; p % j && j <= max; j++); //* bloc vide
	return (j >= max) ? 1 : 0;
}

int print_prime_factors_nodisp(unsigned n, int startresearch, int pas_i) {
#ifdef MAP
	printf("\n%d :", n);
#endif
	if (!(n % 2)) {
		printf(" %d", 2);
		return print_prime_factors_nodisp(n / 2, 2, pas_i);
	}
	if (!(n % 3)) {
		printf(" %d", 3);
		return print_prime_factors_nodisp(n / 3, 3, pas_i);
	}
	if (!(n % 5)) {
		printf(" %d", 5);
		return print_prime_factors_nodisp(n / 5, 5, pas_i);
	}

	pas_i = (pas_i == 0) ? 4 : pas_i;
	int i;
	startresearch = (startresearch >= 7) ? startresearch : 7;
	//printf(" //startresearch=%d// ", startresearch);
	for (i = startresearch; i <= n; i += pas_i, pas_i = 6 - pas_i) {//* utilisation d'un pas alternatif
		if (!(n % i) && is_prime(i)) {
			printf(" %d", i);
			if (n > i) {
				return print_prime_factors_nodisp(n / i, i, pas_i);
			}
		}
#ifdef MAP
		else {
			printf("(n%%i)=%d\nis_prime(i)=%d", (n % i), is_prime(i));
		}
#endif
	}
	return pas_i;
}

void print_prime_factors(unsigned n) {
	printf("%u:", n);
	int pas_i = 0;

	if (is_prime(n)) {
		printf(" %u\n", n);
		return;
	} else if (!(n % 2)) {
		printf(" %d", 2);
		pas_i = print_prime_factors_nodisp(n / 2, 2, 0);
	} else if (!(n % 3)) {
		printf(" %d", 3);
		pas_i = print_prime_factors_nodisp(n / 3, 3, 0);
	} else if (!(n % 5)) {
		printf(" %d", 5);
		pas_i = print_prime_factors_nodisp(n / 5, 5, 0);
	} else {

		int pas_j = 4;
		int i;
		for (i = 7; i < n; i += pas_j, pas_j = 6 - pas_j) {//* utilisation d'un pas alternatif
			if (!(n % i) && is_prime(i)) {
				printf(" %d", i);
				print_prime_factors_nodisp(n / i, i, 0);
				break;
			}
		}
	}

	printf("\n");
}

unsigned get_prime_factors(unsigned n, unsigned* factors) {
	t_noeud* already;

	unsigned index = 0;
	unsigned pas_i = 4; //* important de garder la première assignation ici, pr pas réinitialiser le pas en cours de recherche
	unsigned lastone = 0;
	unsigned n_prev = n;
	factors[0] = EoT;
	while (n > 1) {//* lorsque n = 1 ça veut dire qu'on a effectué l'opération n /= n donc c'est fini
		if (n != n_prev) {
			already = rechercher_arbre(MEM_TREE, n);
			if (already != NULL) {//* Déjà calculé et stocké, on renvoit directement
				unsigned index2 = index;
				while (already->valeur[index2] != NULL) {
					factors[index++] = already->valeur[index2++];
				}
				return index;
			}
		}
		if (!lastone) {
			if (!(n % 2)) {
				factors[index++] = 2;
				factors[index] = EoT;
				n /= 2;
			} else if (!(n % 3)) {
				factors[index++] = 3;
				factors[index] = EoT;
				n /= 3;
			} else if (!(n % 5)) {
				factors[index++] = 5;
				factors[index] = EoT;
				n /= 5;
			} else {
				lastone = 3; // 3 (here) + 4 (pas_i added at the begining of the loop at the bottom) = 7 (the default beginning value)
			}
			continue;
		}
		//* Memoization of the current decomposition for current n value :
		t_element* factors_cpy = (t_element*) malloc(sizeof(t_element)*MAX_FACTORS);
		memcpy(factors_cpy, factors, sizeof(t_element)*MAX_FACTORS);//* copy the factors[] array into factors_cpy[] array
		MEM_TREE = inserer_arbre(MEM_TREE, n, factors_cpy);
		

		//lastone = 7;
		unsigned i;
		//unsigned stop = 0;
		for (i = lastone + pas_i; i <= n;) {//* utilisation d'un pas alternatif
			if (!(n % i) && is_prime(i)) {
				factors[index++] = i;
				factors[index] = EoT;
				n /= i;
				lastone = i;
			} else {//* on ne met à jour la variable de parcours qu'une fois qu'on a "épuisé" un possible facteur on crée une sous-boucle artificielle en moins lourd
				i += pas_i;
				pas_i = 6 - pas_i;
			}
		}
	}
	return index;
}

void print_prime_factorsMemoized(unsigned n) {

	//* Initialisation de la structure de données :
	MEM_TREE = creer_arbre(0, NULL, NULL, NULL);
	int j, k;
	unsigned int factors[MAX_FACTORS];

	if (is_prime(n)) {
		printf(" %u\n", n);
		return;
	}

	k = get_prime_factors(n, factors);

	printf("%u: ", n);
	for (j = 0; j < k; j++) {
		printf("%u ", factors[j]);
	}

	printf("\n");
}

void readMyFile(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		unsigned tmp = (unsigned) atoi(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void readMyreadMyFileSynced_And_Memoized(FILE* f) {

	char buffer[100];
	for (;;) {
		pthread_mutex_lock(&mid);
		int ret = fscanf(f, "%s\n", buffer);
		pthread_mutex_unlock(&mid);
		if (ret == EOF) {
			return;
		}
		unsigned tmp = (unsigned) atoi(buffer);
		print_prime_factorsMemoized(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void readMyFileSynced(FILE* f) {

	char buffer[100];
	for (;;) {
		pthread_mutex_lock(&mid);
		int ret = fscanf(f, "%s\n", buffer);
		pthread_mutex_unlock(&mid);
		if (ret == EOF) {
			return;
		}
		unsigned tmp = (unsigned) atoi(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void print_prime_factors_wrapper(void *i) {
	unsigned *p = (unsigned*) i;
	print_prime_factors(*p);
}

void readMyFileThreaded1(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		unsigned tmp = (unsigned) atoi(buffer);
		pthread_t tid, tid2;

		pthread_create(&tid, NULL, print_prime_factors_wrapper, (void*) &tmp);
		if (fscanf(f, "%s\n", buffer) != EOF) {
			unsigned tmp2 = (unsigned) atoi(buffer);
			pthread_create(&tid2, NULL, print_prime_factors_wrapper, (void*) &tmp2);
			pthread_join(tid2, NULL);
		}
		pthread_join(tid, NULL);

#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void readMyFileThreaded2(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];

	pthread_t tid, tid2;

	pthread_mutex_init(&mid, NULL);

	pthread_create(&tid2, NULL, readMyFileSynced, (void*) f);

	pthread_create(&tid, NULL, readMyFileSynced, (void*) f);

	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&mid);
	fclose(f);
}

void readMyFileThreadedN(char* fname, unsigned int N) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];

	pthread_t tids[N];

	pthread_mutex_init(&mid, NULL);

	int ij;
	for (ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyFileSynced, (void*) f);
	}

	for (ij = 0; ij < N; ij++) {
		pthread_join(tids[ij], NULL);
	}

	pthread_mutex_destroy(&mid);
	fclose(f);
}

void readMyFileThreadedN_And_Memoized(char* fname, unsigned int N) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];

	pthread_t tids[N];

	pthread_mutex_init(&mid, NULL);

	int ij;
	for (ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyreadMyFileSynced_And_Memoized, (void*) f);
	}

	for (ij = 0; ij < N; ij++) {
		pthread_join(tids[ij], NULL);
	}

	pthread_mutex_destroy(&mid);
	fclose(f);
}

/******************* TEST FUNCTIONS **************** */

int testIsPrime(int max) {
	if (!is_prime(1)) {
		return 0;
	}
	if (!is_prime(2)) {
		return 0;
	}
	if (!is_prime(3)) {
		return 0;
	}
	if (!is_prime(5)) {
		return 0;
	}
	if (!is_prime(7)) {
		return 0;
	}
	if (!is_prime(1)) {
		return 0;
	}
	if (!is_prime(11)) {
		return 0;
	}
	if (!is_prime(13)) {
		return 0;
	}
	int i;
	for (i = 16; i < max; i++) {
		if (is_prime(i)) {
			printf("%d est premier\n", i);
		} else {
			printf("%d est PAS premier\n", i);
		}
	}
	return 1;
}

int testPrintPrimeFactors() {
	print_prime_factors(84);
	print_prime_factors(138);

	return 1;
}

int main(int argc, char** argv) {
	/*	if(testIsPrime(100)) {
			printf("Test succeeded\n");
		} else {
			printf("Test failed\n");
		}
	
		testPrintPrimeFactors();
	
		readMyFile("numbers.txt");
	 */
	//print_prime_factors(27166);
	readMyFileThreadedN_And_Memoized("numbers2.txt", 1);
	pthread_exit(NULL);
	return 0;
}