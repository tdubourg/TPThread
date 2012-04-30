#include "lib.h"

#define STD_EXIT_THREAD_STATUS (void*)NULL

static pthread_mutex_t mid, mid2, m_screen;
static t_arbre* MEM_TREE;
static unsigned shift_pos = 0;

/************************* CORE FUNCTIONS ************************/

//* Test on a single number
int is_prime(unsigned p) {
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
	double max = sqrt(p) + 1;//* +1 to handle the rounded numbers
	int j;
	for (j = 2; p % j && j <= max; j++); //* bloc vide
	return (j >= max) ? 1 : 0;//* Si j a dépassé ou est égal à max, alors le nombre est premier : nous n'avons pas trouvé de diviseurs. Sinon, il n'est pas premier
}

//* Inner function, used by print_prime_factors(). Do nearly the same but don't do display at the start
static void print_prime_factors_nodisp(unsigned n, int startresearch) {
	if (!(n % 2)) {
		printf(" %u", 2);
		return print_prime_factors_nodisp(n / 2, 2);
	}
	if (!(n % 3)) {
		printf(" %u", 3);
		return print_prime_factors_nodisp(n / 3, 3);
	}
	if (!(n % 5)) {
		printf(" %u", 5);
		return print_prime_factors_nodisp(n / 5, 5);
	}

	int pas_i = 4;
	int i;
	startresearch = (startresearch >= 7) ? startresearch : 7;
	int max = sqrt(n) + 1;
	
	for (i = startresearch; i < max; i += pas_i, pas_i = 6 - pas_i) {//* utilisation d'un pas alternatif
		if (!(n % i)) {
			printf(" %u", i);
			if (i < n) {
				return print_prime_factors_nodisp(n / i, i);
			}
		}
#ifdef MAP
		else {
			printf("(n%%i)=%u\nis_prime(i)=%u", (n % i), is_prime(i));
		}
#endif
	}
	printf(" %u", n);
}

//* Prints the prime factors decomposition of a given unsigned 32bits number (displays the decomposition)
void print_prime_factors(unsigned n) {
	printf("%u:", n);
	if (is_prime(n)) {
		printf(" %u\n", n);
		return;
	} else if (!(n % 2)) {
		printf(" %u", 2);
		print_prime_factors_nodisp(n / 2, 2);
	} else if (!(n % 3)) {
		printf(" %u", 3);
		print_prime_factors_nodisp(n / 3, 3);
	} else if (!(n % 5)) {
		printf(" %u", 5);
		print_prime_factors_nodisp(n / 5, 5);
	} else {
		int pas_j = 4;
		int i;
		for (i = 7; i < n; i += pas_j, pas_j = 6 - pas_j) {//* use of an alternative step
			if (!(n % i)) {
				printf(" %u", i);
				print_prime_factors_nodisp(n / i, i);
				break;
			}
		}
	}

	printf("\n");
}

//* Memoization things :
//* compute prime factors decomposition using memoization
//* then puts everything in an array
unsigned get_prime_factors(unsigned n, unsigned* factors) {
	t_arbre* already;

	unsigned index = 0;
	unsigned pas_i = 4; //* important de garder la première assignation ici, pr pas réinitialiser le pas en cours de recherche
	unsigned lastone = 0;
	unsigned n_prev = n;
	unsigned char first_loop = 1;

	while (n > 1) {//* lorsque n = 1 ça veut dire qu'on a effectué l'opération n /= n donc c'est fini
		if (!first_loop && IN_RANGE(n)) {
#ifdef MAP
			printf("Looking in the tree for %u\n", n);
#endif
			already = rechercher_arbre(MEM_TREE, n);
			if (already != NULL) {//* Déjà calculé et stocké, on renvoit directement
#ifdef MAP
				printf("%u has already been memoized, using the values\n");
#endif
				unsigned index2 = 0;
				//* This loop copies the prime factors contained in the tree to the result
				for (index2 = 0; index2 < already->val_size; index2++) {
					factors[index++] = already->valeur[index2];
				}
				//* We insert the n decomposition in the tree, in n_prev is an "interesting number"
				if(IN_RANGE(n_prev)) {
					t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * (index));
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
				lastone = 3; // 3 (here) + 4 (pas_i added at the beginning of the loop at the bottom) = 7 (the default beginning value)
			}
		}

		unsigned i;
		n_prev = n;
		for (i = lastone + pas_i; (i*i) <= n;) {//* utilisation d'un pas alternatif
			if (!(n % i)) {
				factors[index++] = i;
				n_prev = n;
				n /= i;
				already = rechercher_arbre(MEM_TREE, n);
				if (already != NULL) {//* Déjà calculé et stocké, on renvoit directement
#ifdef MAP
					printf("%u has already been memoized, using the values\n", n);
#endif
					unsigned index2 = 0;
					//* Here, we just finish the function, we get the "factors" array ready to be returned
					for (index2 = 0; index2 < already->val_size; index2++) {
						factors[index++] = already->valeur[index2];
					}
					//* If this decomposition (the complete one of n) is to be stored, we store it :
					if(IN_RANGE(n_prev)) {
						t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * (index + 1));
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
		if((i*i) >= n && n != 1) {
			factors[index++] = n;
		}
		break;
	}
	//* Memoization of the intermediary results
	int memo;
	int curr = factors[0];
	for (memo = 1; memo < index; memo++) {
		curr *= factors[memo];
#ifdef MAP
		printf("Memoizing factors decomposition of %u\n", curr);
#endif
		//* Memoization of the current decomposition for current n value :
		if(IN_RANGE(curr)) {
			t_element* factors_cpy = (t_element*) malloc(sizeof (t_element) * (memo + 1));
			memcpy(factors_cpy, factors, sizeof (t_element) * (memo + 1)); //* copy the factors[] array into factors_cpy[] array

	#ifdef MAP
			printf("We are going to insert in the tree :\n");
			int ijk = 0;
			for (; ijk < (memo + 1); ijk++) {
				printf("%u ", factors_cpy[ijk]);
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

//* Prints the prime factors decomposition of a given number, using memoization
void print_prime_factorsMemoized(unsigned n) {
	int j, k;
	unsigned factors[MAX_FACTORS];

	k = get_prime_factors(n, factors);

	pthread_mutex_lock(&m_screen);
	printf("%u: ", n);
	for (j = 0; j < k; j++) {
		printf("%u ", factors[j]);
	}
	printf("\n");
	pthread_mutex_unlock(&m_screen);
}

//* Reads a file, gets numbers from it, prints the prime factors decomposition for each one
void readMyFile(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[50];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		unsigned tmp = (unsigned) atol(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

//* After the file having been read and stored in memory, this functions goes through it like through a real file, but using RAM, so, faster
void *readMyFileFromMemorySynced_And_Memoized(void* aF) {
	char* buffer;
	char* file_in_memory = (char*) aF;

	for (;;) {
		pthread_mutex_lock(&mid);
		//* New way : using memory !
		buffer = strtok(file_in_memory+shift_pos, "\n");
#ifdef MAP
		printf("Buffer=%s\n", buffer);
#endif
		if (buffer == NULL) {//* Over !
			pthread_mutex_unlock(&mid);//* Important line !!!
#ifdef MAP
			printf("Over !\n");
#endif
			return STD_EXIT_THREAD_STATUS;
		}
		shift_pos += strlen(buffer)+1;
		pthread_mutex_unlock(&mid);
		unsigned tmp = (unsigned) atol(buffer);
		print_prime_factorsMemoized(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	return STD_EXIT_THREAD_STATUS;
}

void* readMyFileSynced(void* aF) {
	FILE *f = (FILE*) aF;

	char buffer[100];
	for (;;) {
		pthread_mutex_lock(&mid);
		int ret = fscanf(f, "%s\n", buffer);
		pthread_mutex_unlock(&mid);
		if (ret == EOF) {
			return STD_EXIT_THREAD_STATUS;
		}
		unsigned tmp = (unsigned) atol(buffer);
		print_prime_factors(tmp);
#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
	return STD_EXIT_THREAD_STATUS;
}

void * print_prime_factors_wrapper(void *i) {
	unsigned *p = (unsigned*) i;
	print_prime_factors(*p);
	return STD_EXIT_THREAD_STATUS;
}

void readMyFileThreaded1(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	char buffer[100];
	while (fscanf(f, "%s\n", buffer) != EOF) {
		unsigned tmp = (unsigned) atol(buffer);
		pthread_t tid, tid2;

		pthread_create(&tid, NULL, print_prime_factors_wrapper, (void*) (&tmp));
		if (fscanf(f, "%s\n", buffer) != EOF) {
			unsigned tmp2 = (unsigned) atol(buffer);
			pthread_create(&tid2, NULL, print_prime_factors_wrapper, (void*) (&tmp2));
			pthread_join(tid2, NULL);
		}
		pthread_join(tid, NULL);

#ifdef MAP
		printf("%s\n", buffer);
#endif
	}
	fclose(f);
}

//* Reads a file with mutex sync, to be able to be executed from multiple threads simultaneously
void readMyFileThreaded2(char* fname) {
	FILE *f;
	f = fopen(fname, "r");

	pthread_t tid, tid2;

	pthread_mutex_init(&mid, NULL);

	pthread_create(&tid2, NULL, readMyFileSynced, (void*) f);

	pthread_create(&tid, NULL, readMyFileSynced, (void*) f);

	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&mid);
	fclose(f);
}

//* Does the same thing as readMyFileThreaded2() but with N threads
void readMyFileThreadedN(char* fname, unsigned N) {
	FILE *f;
	f = fopen(fname, "r");

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

//* Reads the file, puts it in RAM, launches the threads with callback = readMyFileFromMemorySynced_And_Memoized and the pointer to the file "in memory'
void readMyFileThreadedN_And_Memoized(char* fname, unsigned N) {

	//* Initialisation de la structure de données :
	MEM_TREE = creer_arbre(6000, NULL, NULL, NULL);

	FILE *f;
	f = fopen(fname, "r");

	//* ----------- Putting the file into memory in order to be faster : -------------------
	fseek(f, 0, SEEK_END);
	unsigned size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char*result = (char *)malloc(sizeof(char)*(size+1));
	if (size != fread(result, sizeof(char), size, f))
	{
		printf("Error reading file !");
		return;// Error in file read, abort !
	}
	fclose(f);
	//*** ----------- END OF PUTTING THE FILE IN MEMORY -------------------
	result[size] = '\0';

	//* ----------- Launching the threads --------------------------------
	pthread_t tids[N];

	pthread_mutex_init(&mid, NULL);
	pthread_mutex_init(&mid2, NULL);
	pthread_mutex_init(&m_screen, NULL);

	int ij;
	for (ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyFileFromMemorySynced_And_Memoized, (void*) result);
	}

	//* ----------- Waiting for all the threads to finish -----------
	for (ij = 0; ij < N; ij++) {
		pthread_join(tids[ij], NULL);
	}

	//* ----------- Garbage collection -----------
	free(result);
	pthread_mutex_destroy(&mid);
	pthread_mutex_destroy(&mid2);
	pthread_mutex_destroy(&m_screen);
	detruire_arbre(MEM_TREE);
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
			printf("%u est premier\n", i);
		} else {
			printf("%u est PAS premier\n", i);
		}
	}
	return 1;
}

int testPrintPrimeFactors() {
	printf("\n");
	print_prime_factors(84);
	print_prime_factors(138);

	return 1;
}
