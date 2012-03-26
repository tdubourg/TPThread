#include <stdio.h>
#include <math.h>
#include <pthread.h>

//#define MAP

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

void print_prime_factors_nodisp(unsigned n) {
#ifdef MAP
	printf("\n%d :", n);
#endif
	if (!(n % 2)) {
		printf(" %d", 2);
		return print_prime_factors_nodisp(n / 2);
	}
	if (!(n % 3)) {
		printf(" %d", 3);
		return print_prime_factors_nodisp(n / 3);
	}
	if (!(n % 5)) {
		printf(" %d", 5);
		return print_prime_factors_nodisp(n / 5);
	}

	int pas_i = 4;
	int i;
	for (i = 7; i <= n; i += pas_i, pas_i = 6 - pas_i) {//* utilisation d'un pas alternatif
		if (!(n % i) && is_prime(i)) {
			printf(" %d", i);
			if (n > i) {
				return print_prime_factors_nodisp(n / i);
			} else {
				printf("\n");
				return;
			}
		}
#ifdef MAP
		else {
			printf("(n%%i)=%d\nis_prime(i)=%d", (n % i), is_prime(i));
		}
#endif
	}

}

void print_prime_factors(unsigned n) {
	printf("%d:", n);

	if (is_prime(n)) {
		printf(" %d\n", n);
		return;
	}

	if (!(n % 2)) {
		printf(" %d", 2);
		return print_prime_factors_nodisp(n / 2);
	}
	if (!(n % 3)) {
		printf(" %d", 3);
		return print_prime_factors_nodisp(n / 3);
	}
	if (!(n % 5)) {
		printf(" %d", 5);
		return print_prime_factors_nodisp(n / 5);
	}

	int pas_i = 4;
	int i;
	for (i = 7; i < n; i += pas_i, pas_i = 6 - pas_i) {//* utilisation d'un pas alternatif
		if (is_prime(i) && !(n % i)) {
			printf(" %d", i);
			return print_prime_factors_nodisp(n / i);
		}
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

static pthread_mutex_t mid;

void readMyFileSynced(FILE* f) {
	
	char buffer[100];
	for(;;) {
		pthread_mutex_lock(&mid);
		int ret = fscanf(f, "%s\n", buffer);
		pthread_mutex_unlock(&mid);
		if(ret == EOF) {
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
	for(ij = 0; ij < N; ij++) {
		pthread_create(&(tids[ij]), NULL, readMyFileSynced, (void*) f);
	}
	
	for(ij = 0; ij < N; ij++) {
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
	readMyFileThreadedN("numbers2.txt", 4);
	return 0;
}