#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int in_cs = 0;

void* thread_work(void *arg) {
    in_cs++;
}

int main(int argc, char *argv[]) {
    int num_threads = atoi(argv[1]);
    int sec = atoi(argv[2]);

    pthread_t t;

    if (pthread_create(&t, NULL, thread_work, NULL)) {
        printf("Failed!");
    }

    return 0;
}