#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

volatile int in_cs = 0;

void* thread_work(void *arg) {
	while (1) {
//		pthread_t *t = (pthread_t *)arg; 
//		printf("I am thread: %li\n", *t);
		assert (in_cs==0);
		in_cs++;
		assert (in_cs==1);
		in_cs++;
		assert (in_cs==2);
		in_cs++;
		assert (in_cs==3);
		in_cs=0;
	}
	return arg;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("This program requires two arguments. Exiting.\n");
		exit(1);
	}
    int num_threads = atoi(argv[1]);
    int sec = atoi(argv[2]);
	clock_t start = clock();
	int term = 1;	
	pthread_t threads[num_threads]; 
	int created_threads	= 0;
	int i = 0;

	// Used to ignore unused error
	(void)argc;

	// Dispatch the threads.
	for (; i < num_threads; i++) {
		int rc = pthread_create(&threads[i], NULL, thread_work, &threads[i]);
		assert(rc == 0);
		created_threads++;
	}

	i = 0;

	// Run for sec seconds.
	while (term) {
		clock_t next = clock();
		if (difftime(next, start) / CLOCKS_PER_SEC >= sec) {
			// Clean up the threads.
			for (; i < num_threads; i++) {
				pthread_kill(threads[i], 9);
			}
			term = 0;
		}
	}




    return 0;
}

