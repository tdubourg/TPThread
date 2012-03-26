#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

void *thread1(void* i) {
	cout << "Le thread tid " << pthread_self() << " a pour pid " << getpid() << endl;
}



int mainpouetpouet(int argc, char** argv) {
	cout << "Le pid du main est : " << getpid() << endl;
	
	pthread_t* tid = NULL;
	
	pthread_create(tid, NULL, thread1, NULL);
	
	pthread_exit(NULL);
	return 0;
}
