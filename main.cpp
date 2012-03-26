#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#define NBMAX 5

using namespace std;

void *thread1(void* i) {
	cout << "Le thread tid " << pthread_self() << " a pour pid " << getpid() << endl;
}

void *thread2(void* i) {
	static unsigned short mynum = 0;
	static unsigned long cpt = 0;
/*	sleep(1);
	if(mynum) {
		pthread_join(*((int*)i), NULL);
	}*/
	mynum++;
	for(int j = 0; j < NBMAX; j++) {
		pthread_mutex_lock(((pthread_mutex_t*)i));
		cpt += mynum;
		cout << "Thread tid " << pthread_self() << " - CPT = " << cpt << endl;
		pthread_mutex_unlock(((pthread_mutex_t*)i));
		
	}
}

int main(int argc, char** argv) {
	cout << "Le pid du main est : " << getpid() << endl;
	
	pthread_t tid;
	
	pthread_mutex_t mid;
	pthread_mutex_init(&mid, NULL);
	for(int i = 0; i < 5; i++) {
		pthread_create(&tid, NULL, thread2, &mid);
		cout << "Main : Je crée le thread " << i << endl;
	}
	pthread_mutex_destroy(&mid);
	
	pthread_exit(NULL);
	return 0;
}
