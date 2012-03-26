/* 
 * File:   UnicityTest.cpp
 * Author: troll
 *
 * Created on Mar 26, 2012, 11:34:33 PM
 */

#include <stdlib.h>
#include <iostream>
#include "set"

/*
 * Simple C++ Test Suite
 */
using namespace std;


void test1() {
	std::cout << "UnicityTest test 1" << std::endl;
	const unsigned MAX = 1000;
	
	set<unsigned int> myset;
	for(int i = 2; i < MAX*3; i++) {
		unsigned int tmp = i/MAX + i%MAX;
		if(!(myset.insert(tmp).second)) {
			cout << "i=" << i << "; tmp=" << tmp << " >> VALEUR DEJA PRISE\n";
		} else {
			cout << "i=" << i << "; tmp=" << tmp << " >> VALEUR OK\n";
		}
	}
	cout << endl;
}

void test2() {
	std::cout << "UnicityTest test 2" << std::endl;
	std::cout << "%TEST_FAILED% time=0 testname=test2 (UnicityTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << "%SUITE_STARTING% UnicityTest" << std::endl;
	std::cout << "%SUITE_STARTED%" << std::endl;

	std::cout << "%TEST_STARTED% test1 (UnicityTest)" << std::endl;
	test1();
	std::cout << "%TEST_FINISHED% time=0 test1 (UnicityTest)" << std::endl;

	std::cout << "%TEST_STARTED% test2 (UnicityTest)\n" << std::endl;
	test2();
	std::cout << "%TEST_FINISHED% time=0 test2 (UnicityTest)" << std::endl;

	std::cout << "%SUITE_FINISHED% time=0" << std::endl;

	return (EXIT_SUCCESS);
}

