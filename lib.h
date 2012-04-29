/*
 * File:   lib.h
 * Author: troll
 *
 * Created on April 2, 2012, 5:02 PM
 */

#ifndef LIB_H
#define	LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "arbre.h"

#define MAX_NUMBERS MAX_NUMBERS
#define EoT (-42)

#define MAX_VALUE_IN_TREE 64000
#define MIN_VALUE_IN_TREE 8

#define IN_RANGE(n) ((n) < MAX_VALUE_IN_TREE && (n) > MIN_VALUE_IN_TREE)

int testPrintPrimeFactors();

int testIsPrime(int max);

//* Use of 2 threads, doing pthread_join() after each computation
void readMyFileThreaded1(char* fname);


//* Reads a file with mutex sync, to be able to be executed from multiple threads simultaneously
void readMyFileThreaded2(char* fname);

//* Does the same thing as readMyFileThreaded2() but with N threads
void readMyFileThreadedN(char* fname, unsigned N);


void *print_prime_factors_wrapper(void *i);

//* Reads a file with multi-thread synchronization using mutex, in order to be called from multiple thread
void *readMyFileSynced(void* aF);

//* Reads the file, puts it in RAM, launches the threads with callback = readMyFileFromMemorySynced_And_Memoized and the pointer to the file "in memory'
void readMyFileThreadedN_And_Memoized(char* fname, unsigned N);

//* After the file having been read and stored in memory, this functions goes through it like through a real file, but using RAM, so, faster
void *readMyFileFromMemorySynced_And_Memoized(void* aF);

//* Reads a file with a int32 on each line. Displays the prime factors decomposition of each one
void readMyFile(char* fname);

void print_prime_factorsMemoized(unsigned n);

unsigned get_prime_factors(unsigned n, unsigned* factors);

//* Prints the prime factors decomposition of a given unsigned 32bits number (displays the decomposition)
void print_prime_factors(unsigned n);

//* Test on a single number
int is_prime(unsigned p);



#ifdef	__cplusplus
}
#endif

#endif	/* LIB_H */

