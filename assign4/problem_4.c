#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>

#define MAX_THREADS 10000

volatile int total = 0;
volatile int totals[MAX_THREADS];
volatile int in_cs = 0;
volatile int running = 1;
volatile int choosing[MAX_THREADS];
volatile int tickets[MAX_THREADS];
struct spin_lock_t s;

typedef struct __t_inf {
	int i;
	int num_thr;
} t_inf;

struct spin_lock_t {
	int flag;
};

static inline int atomic_cmpxchg (volatile int *ptr, int old, int new)
{
  int ret;
  asm volatile ("lock cmpxchgl %2,%1"
		      : "=a" (ret), "+m" (*ptr)     
			      : "r" (new), "0" (old)      
				      : "memory");         
  return ret;                            
}

void init_lock(struct spin_lock_t *t) {
	t->flag = 0;
}

void spin_lock (struct spin_lock_t *s) {
	while (atomic_cmpxchg(&s->flag, 0, 1) == 1)
		;
}

void spin_unlock (struct spin_lock_t *s) {
	s->flag = 0;
}

void mfence (void) {
  asm volatile ("mfence" : : : "memory");
}

void lock(int i, int num_threads) {
	choosing[i] = 1;
	mfence();
	int j = 1;
	int max = tickets[0];
	for (; j < num_threads; j++) {
		max = (tickets[j] > max) ? tickets[j] : max;
	}
	tickets[i] = 1 + max;
	mfence();
	choosing[i] = 0;
	mfence();
	int k = 0;
	for (; k < num_threads; k++) {
		while (choosing[k])
			;

		mfence();

		while (tickets[k] != 0 
		   && (tickets[k] < tickets[i] 
		   || (tickets[k] == tickets[i] && k < i))) 
			;
	}
}

void unlock(int i) {
	mfence();
	tickets[i] = 0;
}

void* thread_work(void *arg) {
	t_inf t = *((t_inf *)(arg));
	while (running) {
//		lock(t.i, t.num_thr);
		spin_lock(&s);
		
		// Critical section.
		assert (in_cs==0);
		in_cs++;
		assert (in_cs==1);
		in_cs++;
		assert (in_cs==2);
		in_cs++;
		assert (in_cs==3);
		in_cs=0;

		// Thread tracking information.
		++totals[t.i];
		++total;

		spin_unlock(&s);
//		unlock(t.i);
	}
	return arg;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("This program requires two arguments. Exiting.\n");
		exit(1);
	}
	(void)argc; // Used to ignore unused error
	int num_threads = atoi(argv[1]);
	int sec = atoi(argv[2]);
	pthread_t threads[num_threads]; 
	t_inf t_infs[num_threads];
	int i = 0;
	init_lock(&s);

	for (; i < num_threads; ++i) {
		choosing[i] = tickets[i] = totals[i] = 0;
	}

	i = 0;

	// Dispatch the threads.
	for (; i < num_threads; i++) {
		t_infs[i].i = i;
		t_infs[i].num_thr = num_threads;
		int rc = pthread_create(&threads[i], NULL, thread_work, &t_infs[i]);
		assert(rc == 0);
	}

	// zzzzzzzzz
	sleep(sec);

	// Terminate the running threads.
	running = 0;

	// Print out results.
	i = 0;
	for (; i < num_threads; ++i) {
		printf("Thread %d:%10d\n", i, totals[i]);
	}
	printf("   Total:%10d\n", total);

    return 0;
}

