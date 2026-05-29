#include <stdio.h>
#include <unistd.h>
#include "../include/queue.h"
#include "../include/sync.h"
#include "../include/metrics.h"

#define QUANTUM 2

extern queue_t ready_queue;

void* cpu_run(void* arg) {
    int cpu_id = *(int*)arg;

    while(1) {
        pthread_mutex_lock(&queue_mutex);

        while(is_empty(&ready_queue)) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        pcb_t process = dequeue(&ready_queue);
        pthread_mutex_unlock(&queue_mutex);
        process.state = RUNNING;
        printf("(CPU %d) Ejecutando proceso %d\n", cpu_id, process.pid);
        sleep(QUANTUM);
        process.remaining_time -= QUANTUM;

        if(process.remaining_time <= 0) {
            process.state = FINISHED;
            completed_processes++;
            printf("(CPU %d) Proceso %d TERMINADO\n", cpu_id, process.pid);
        }
        else {
            process.state = READY;
            printf("(CPU %d) Proceso %d restante: %d\n", cpu_id, process.pid, process.remaining_time);
            pthread_mutex_lock(&queue_mutex);
            enqueue(&ready_queue, process);
            pthread_cond_signal(&queue_cond);
            pthread_mutex_unlock(&queue_mutex);
        }
    }
    return NULL;
}