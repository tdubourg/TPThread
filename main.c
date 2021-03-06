//#define MAP
//#define MAPTREE
#include "lib.h"

#define TESTFILE "numbers.txt"
#define N_THREAD 2


void exercice1() {
	printf("\n ------------ Exercice 1 ------------ \n");
	if (!testIsPrime(50)) {
		printf("isPrime() went wrong.\n");
		return;
	}
	testPrintPrimeFactors();
	printf("\n");
	readMyFile(TESTFILE);
}


void exercice2() {
	printf("\n ------------ Exercice 2 ------------ \n");
	readMyFileThreaded1(TESTFILE);
}

void exercice3() {
	printf("\n ------------ Exercice 3 ------------ \n");
	readMyFileThreaded2(TESTFILE);
}

void exercice4() {
	printf("\n ------------ Exercice 4 ------------ \n");
	readMyFileThreadedN_And_Memoized(TESTFILE, N_THREAD);
}

int main(int argc, char** argv) {
	//* *************** Exercice 1 ************
	exercice1();

	//* *************** Exercice 2 ************
	exercice2();

	//* *************** Exercice 3 ************
	exercice3();

	//* *************** Exercice 4 ************
	exercice4();

	pthread_exit(NULL);
	return 0;
}
