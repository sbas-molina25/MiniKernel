#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/sync.h"
#include "../include/metrics.h"

#define N_CPUS 2

queue_t ready_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

int global_pid = 1;

void *cpu_run(void *arg);

void *process_generator(void *arg)
{
    while (1)
    {
        sleep(rand() % 3 + 1);
        pcb_t process;
        process.pid = global_pid++;
        process.burst_time = rand() % 10 + 1;
        process.remaining_time = process.burst_time;
        process.priority = rand() % 5;
        process.arrival_time = time(NULL);
        process.state = READY;
        pthread_mutex_lock(&queue_mutex);
        enqueue(&ready_queue, process);
        printf("(GENERADOR) Proceso %d creado (burst=%d)\n", process.pid, process.burst_time);
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    init_queue(&ready_queue);
    pthread_t generator_thread;
    pthread_t cpu_threads[N_CPUS];
    pthread_t metrics_tid;
    pthread_create(&generator_thread, NULL, process_generator, NULL);

    for (int i = 0; i < N_CPUS; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&cpu_threads[i], NULL, cpu_run, id);
    }
    pthread_create(&metrics_tid, NULL, metrics_thread, NULL);
    pthread_join(generator_thread, NULL);

    for (int i = 0; i < N_CPUS; i++)
    {
        pthread_join(cpu_threads[i], NULL);
    }
    pthread_join(metrics_tid, NULL);
    return 0;
}