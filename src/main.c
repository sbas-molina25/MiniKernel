#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/sync.h"
#include "../include/metrics.h"

#define numCPUS 2

queue_t readyQueue;
pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicionQueue = PTHREAD_COND_INITIALIZER;

int pidGlobal = 1;

void *cpu_run(void *arg);

// genera procesos aleatorios y los agrega a la cola de listos
void *generadorProcesos(void *arg)
{
    while (1)
    {
        sleep(rand() % 3 + 1); // espera un tiempo random
        pcb_t proceso;
        proceso.pid = pidGlobal++;
        proceso.burst_time = rand() % 10 + 1;
        proceso.remaining_time = proceso.burst_time;
        proceso.priority = rand() % 5;
        proceso.arrival_time = time(NULL);
        proceso.state = READY;
        pthread_mutex_lock(&mutexQueue); // bloquea la cola
        enqueue(&readyQueue, proceso);
        printf("GENERADOR: Proceso %d creado con burst = %d\n", proceso.pid, proceso.burst_time);
        pthread_cond_signal(&condicionQueue); // despierta una cpu
        pthread_mutex_unlock(&mutexQueue);    // desbloquea la cola
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    iniciarCola(&readyQueue);
    pthread_t threadGenerador;
    pthread_t threadCpu[numCPUS];
    pthread_t threadMetrics;
    pthread_create(&threadGenerador, NULL, generadorProcesos, NULL); // crea un hilo generador de procesos

    for (int i = 0; i < numCPUS; i++) // se crean los cpus
    {
        int *idCPU = malloc(sizeof(int));
        *idCPU = i + 1;
        pthread_create(&threadCpu[i], NULL, cpu_run, idCPU);
    }
    pthread_create(&threadMetrics, NULL, metrics_thread, NULL); // crea hilos de metricas
    pthread_join(threadGenerador, NULL);                        // espera a que termine el generador

    for (int i = 0; i < numCPUS; i++) // espera a que terminen los cpus
    {
        pthread_join(threadCpu[i], NULL);
    }
    pthread_join(threadMetrics, NULL); // espera a que terminen las metricas
    return 0;
}