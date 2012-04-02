#include "lib.h"

static pthread_mutex_t mid, mid2;
static t_arbre* MEM_TREE;
static long long unsigned shift_pos = 0;

/************************* CORE FUNCTIONS ************************/
long long is_prime(long long unsigned p) {
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
	long long j;
	for (j = 2; p % j && j <= max; j++); //* bloc vide
	return (j >= max) ? 1 : 0;
}

long long print_prime_factors_nodisp(long long unsigned n, long long startresearch, long long pas_i) {
#ifdef MAP
	printf("\n%ll :", n);
#endif
	if (!(n % 2)) {
		printf(" %ll", 2);
		return print_prime_factors_nodisp(n / 2, 2, pas_i);
	}
	if (!(n % 3)) {
		printf(" %ll", 3);
		return print_prime_factors_nodisp(n / 3, 3, pas_i);
	}
	if (!(n % 5)) {
		printf(" %ll", 5);
		return print_prime_factors_nodisp(n / 5, 5, pas_i);
	}

	pas_i = (pas_i == 0) ? 4 : pas_i;
	long long i;
	startresearch = (startresearch >= 7) ? startresearch : 7;
	//printf(" //startresearch=%ll// ", startresearch);
	for (i = startresearch; i <= n; i += pas_i, pas_i = 6 - pas_i) {//* utilisation d'un pas alternatif
		if (!(n % i) && is_prime(i)) {
			printf(" %ll", i);
			if (n > i) {
				return print_prime_factors_nodisp(n / i, i, pas_i);
			}
		}
#ifdef MAP
		else {
			printf("(n%%i)=%ll\nis_prime(i)=%ll", (n % i), is_prime(i));
		}
#endif
	}
	return pas_i;
}

void print_prime_factors(long long unsigned n) {
	printf("%llu:", n);
	long long pas_i = 0;

	if (is_prime(n)) {
		printf(" %llu\n", n);
		return;
	} else if (!(n % 2)) {
		printf(" %ll", 2);
		pas_i = print_prime_factors_nodisp(n / 2, 2, 0);
	} else if (!(n % 3)) {
		printf(" %ll", 3);
		pas_i = print_prime_factors_nodisp(n / 3, 3, 0);
	} else if (!(n % 5)) {
		printf(" %ll", 5);
		pas_i = print_prime_factors_nodisp(n / 5, 5, 0);
	} else {

		long long pas_j = 4;
		long long i;
		for (i = 7; i < n; i += pas_j, pas_j = 6 - pas_j) {//* utilisation d'un pas alternatif
			if (!(n % i) && is_prime(i)) {
				printf(" %ll", i);
				print_prime_factors_nodisp(n / i, i, 0);
				break;
			}
		}
	}

	printf("\n");
}

long long unsigned get_prime_factors(long long unsigned n, long long unsigned* factors) {
	t_arbre* already;

	long long unsigned index = 0;
	long long unsigned pas_i = 4; //* important de garder la première assignation ici, pr pas réinitialiser le pas en cours de recherche
	long long unsigned lastone = 0;
	long long unsigned n_prev = n;
	long long unsigned orig = n;
	long long unsigned max = sqrt(n);
	unsigned char first_loop = 1;
	//factors[0] = EoT;
	while (n > 1) {//* lorsque n = 1 ça veut dire qu'on a effectué l'opération n /= n donc c'est fini
		if (!first_loop && IN_RANGE(n)) {
#ifdef MAP
			printf("Looking in the tree for %llu\n", n);
#endif
			already = rechercher_arbre(MEM_TREE, n);
			if (already != NULL) {//* Déjà calculé et stocké, on renvoit directement
#ifdef MAP
				printf("%llu has already been memoized, using the values\n");
#endif
				long long unsigned index2 = 0;
				for (index2 = 0; index2 < already->val_size; index2++) {
					factors[index++] = already->valeur[index2];
				}
				if(IN_RANGE(n_prev)) {
					t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * MAX_FACTORS);
					memcpy(factors_cpy, factors, sizeof (t_element) * (index)); //* no plus one because index++ just before //* copy the factors[] array into factors_cpy[] array
					pthread_mutex_lock(&mid2);
					MEM_TREE = inserer_arbre(MEM_TREE, n_prev, factors_cpy, index); //* no plus one because index++ just before
					pthread_mutex_unlock(&mid2);
				}
				return index;
			}
		}
		first_loop = 0;
		if (!lastone) {
			if (!(n % 2)) {
				factors[index++] = 2;
				factors[index] = EoT;
				n_prev = n;
				n /= 2;
				continue;
			} else if (!(n % 3)) {
				factors[index++] = 3;
				factors[index] = EoT;
				n_prev = n;
				n /= 3;
				continue;
			} else if (!(n % 5)) {
				factors[index++] = 5;
				factors[index] = EoT;
				n_prev = n;
				n /= 5;
				continue;
			} else {
				lastone = 3; // 3 (here) + 4 (pas_i added at the begining of the loop at the bottom) = 7 (the default beginning value)
			}
		}

		long long unsigned i;
		n_prev = n;
		for (i = lastone + pas_i; i <= max;) {//* utilisation d'un pas alternatif
			if (!(n % i)) {
				factors[index++] = i;
				//factors[index] = EoT;
				n_prev = n;
				n /= i;
				//lastone = i;
				already = rechercher_arbre(MEM_TREE, n);
				if (already != NULL) {//* Déjà calculé et stocké, on renvoit directement
					printf("%llu has already been memoized, using the values\n");
#ifdef MAP
					printf("%llu has already been memoized, using the values\n");
#endif
					long long unsigned index2 = 0;
					for (index2 = 0; index2 < already->val_size; index2++) {
						factors[index++] = already->valeur[index2];
					}
					if(IN_RANGE(n_prev)) {
						t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * MAX_FACTORS);
						memcpy(factors_cpy, factors, sizeof (t_element) * (index + 1)); //* copy the factors[] array into factors_cpy[] array
						pthread_mutex_lock(&mid2);
						MEM_TREE = inserer_arbre(MEM_TREE, n_prev, factors_cpy, index + 1);
						pthread_mutex_unlock(&mid2);
					}
					return index;
				}
			} else {//* on ne met à jour la variable de parcours qu'une fois qu'on a "épuisé" un possible facteur on crée une sous-boucle artificielle en moins lourd
				i += pas_i;
				pas_i = 6 - pas_i;
			}
		}
		if(i >= max && n != 1) {
			factors[index++] = n;
		}
		break;
	}
	//* Memoization of the intermediary results
	long long memo;
	long long curr = factors[0];
	for (memo = 1; memo < index; memo++) {
		curr *= factors[memo];
#ifdef MAP
		printf("Memoizing factors decomposition of %llu\n", curr);
#endif
		//* Memoization of the current decomposition for current n value :
		if(IN_RANGE(curr)) {
			t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * MAX_FACTORS);
			memcpy(factors_cpy, factors, sizeof (t_element) * (memo + 1)); //* copy the factors[] array into factors_cpy[] array

	#ifdef MAP
			printf("We are going to insert in the tree :\n");
			long long ijk = 0;
			for (; ijk < (memo + 1); ijk++) {
				printf("%llu ", factors_cpy[ijk]);
			}
			printf("\nover\n");
	#endif
			pthread_mutex_lock(&mid2);
			MEM_TREE = inserer_arbre(MEM_TREE, curr, factors_cpy, memo + 1);
			pthread_mutex_unlock(&mid2);
		}
	}
	return index;
}

void print_prime_factorsMemoized(long long unsigned n) {
	long long j, k;
	long long unsigned factors[MAX_FACTORS];

	/*if (is_prime(n)) {
		printf(" %llu\n", n);
		return;
	}*/

	k = get_prime_factors(n, factors);

	printf("%llu: ", n);
	for (j = 0; j < k; j++) {
		printf("%llu ", factors[j]);
	}

	printf("\n");
}

void readMyFile(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		long long unsigned tmp = (long long unsigned) atoll(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void readMyreadMyFileSynced_And_Memoized(char* f) {

	char* buffer;

	for (;;) {
		pthread_mutex_lock(&mid);
		//long long ret = fscanf(f, "%s\n", buffer);
		//* New way : using memory !
		buffer = strtok(f+shift_pos, "\n");
#ifdef MAP
		printf("Buffer=%s\n", buffer);
#endif
		if (buffer == NULL) {//* Over !
			pthread_mutex_unlock(&mid);//* Important line !!!
#ifdef MAP
			printf("Over !\n");
#endif
			return;
		}
		shift_pos += strlen(buffer)+1;
		pthread_mutex_unlock(&mid);
		long long unsigned tmp = (long long unsigned) atoll(buffer);
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
		long long ret = fscanf(f, "%s\n", buffer);
		pthread_mutex_unlock(&mid);
		if (ret == EOF) {
			return;
		}
		long long unsigned tmp = (long long unsigned) atoll(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

void print_prime_factors_wrapper(void *i) {
	long long unsigned *p = (long long unsigned*) i;
	print_prime_factors(*p);
}

void readMyFileThreaded1(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		long long unsigned tmp = (long long unsigned) atoll(buffer);
		pthread_t tid, tid2;

		pthread_create(&tid, NULL, print_prime_factors_wrapper, (void*) &tmp);
		if (fscanf(f, "%s\n", buffer) != EOF) {
			long long unsigned tmp2 = (long long unsigned) atoll(buffer);
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

void readMyFileThreadedN(char* fname, long long unsigned N) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];

	pthread_t tids[N];

	pthread_mutex_init(&mid, NULL);

	long long ij;
	for (ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyFileSynced, (void*) f);
	}

	for (ij = 0; ij < N; ij++) {
		pthread_join(tids[ij], NULL);
	}

	pthread_mutex_destroy(&mid);
	fclose(f);
}

void readMyFileThreadedN_And_Memoized(char* fname, long long unsigned N) {

	//* Initialisation de la structure de données :
	MEM_TREE = creer_arbre(6000, NULL, NULL, NULL);

	FILE *f;
	f = fopen(fname, "r");

	//* Putting the file into memory in order to be faster :

	fseek(f, 0, SEEK_END);
	long long unsigned size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char*result = (char *)malloc(sizeof(char)*(size+1));
	if (size != fread(result, sizeof(char), size, f))
	{
		printf("Error reading file !");
		return;// Error in file read, avort !
	}
	fclose(f);
	result[size] = '\0';

	pthread_t tids[N];

	pthread_mutex_init(&mid, NULL);
	pthread_mutex_init(&mid2, NULL);

	long long ij;
	for (ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyreadMyFileSynced_And_Memoized, (void*) result);
	}

	for (ij = 0; ij < N; ij++) {
		pthread_join(tids[ij], NULL);
	}

	free(result);
	pthread_mutex_destroy(&mid);
	pthread_mutex_destroy(&mid2);
	detruire_arbre(MEM_TREE);
}

/******************* TEST FUNCTIONS **************** */

long long testIsPrime(long long max) {
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
	long long i;
	for (i = 16; i < max; i++) {
		if (is_prime(i)) {
			printf("%ll est premier\n", i);
		} else {
			printf("%ll est PAS premier\n", i);
		}
	}
	return 1;
}

long long testPrintPrimeFactors() {
	print_prime_factors(84);
	print_prime_factors(138);

	return 1;
}
