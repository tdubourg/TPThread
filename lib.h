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
//#define MAP

#define MAX_NUMBERS MAX_NUMBERS
#define EoT (-42)

#define MAX_VALUE_IN_TREE 64000
#define MIN_VALUE_IN_TREE 8

#define IN_RANGE(n) ((n) < MAX_VALUE_IN_TREE && (n) > MIN_VALUE_IN_TREE)

long long testPrintPrimeFactors();
long long testIsPrime(long long max);
void readMyFileThreadedN_And_Memoized(char* fname, long long unsigned N);
void readMyFileThreadedN(char* fname, long long unsigned N);
void readMyFileThreaded2(char* fname);
void readMyFileThreaded1(char* fname);
void print_prime_factors_wrapper(void *i);
void readMyFileSynced(FILE* f);
void readMyreadMyFileSynced_And_Memoized(char* f);
void readMyFile(char* fname);
void print_prime_factorsMemoized(long long unsigned n);
long long unsigned get_prime_factors(long long unsigned n, long long unsigned* factors);
void print_prime_factors(long long unsigned n);
long long print_prime_factors_nodisp(long long unsigned n, long long startresearch, long long pas_i);
long long is_prime(long long unsigned p);



#ifdef	__cplusplus
}
#endif

#endif	/* LIB_H */

