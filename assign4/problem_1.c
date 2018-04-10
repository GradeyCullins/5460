#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

#define MAX_THREADS 10000

volatile int in_cs = 0;
volatile int running = 1;
volatile int choosing[MAX_THREADS];
volatile int tickets[MAX_THREADS];

typedef struct __t_inf {
	int i;
	int num_thr;
} t_inf;

void lock(int i, int num_threads) {
	choosing[i] = 1;
	int j = 1;
	int max = tickets[0];
	for (; j < num_threads; j++) {
		max = (tickets[j] > max) ? tickets[j] : max;
	}
	tickets[i] = 1 + max;
	choosing[i] = 0;
	int k = 0;
	for (; k < num_threads; k++) {
		while (choosing[k])
			;

		while (tickets[k] != 0 
		   && (tickets[k] < tickets[i] 
		   || (tickets[k] == tickets[i] && k < i))) 
			;
	}
}

void unlock(int i) {
	tickets[i] = 0;
}

void* thread_work(void *arg) {
	t_inf t = *((t_inf *)(arg));
	printf("%d %d\n", t.i, t.num_thr);
	while (running) {
		lock(t.i, t.num_thr);
		
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

		unlock(t.i);
	}
	return arg;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("This program requires two arguments. Exiting.\n");
		exit(1);
	}
	(void)argc; // Used to ignore unused error
	clock_t start = clock();
	int num_threads = atoi(argv[1]);
	int sec = atoi(argv[2]);
	pthread_t threads[num_threads]; 
	int created_threads	= 0;
	t_inf t_infs[num_threads];
	int i = 0;
	for (; i < num_threads; ++i) {
		choosing[i] = tickets[i] = 0;
	}
	i = 0;
	while (running) {
		clock_t next = clock();
		// Only create the threads once.
		if (!created_threads) {
			// Dispatch the threads.
			for (; i < num_threads; i++) {
				t_infs[i].i = i;
				t_infs[i].num_thr = num_threads;
				int rc = pthread_create(&threads[i], NULL, thread_work, &t_infs[i]);
				assert(rc == 0);
			}
			created_threads = 1;
		}
		if (difftime(next, start) / CLOCKS_PER_SEC >= sec) {
			// Signal to the threads that the game is over.
			running = 0;
		}
	}

    return 0;
}

