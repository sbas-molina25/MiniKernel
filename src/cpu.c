#include <stdio.h>
#include <unistd.h>
#include "../include/queue.h"
#include "../include/sync.h"
#include "../include/metrics.h"

#define QUANTUM 2 // tiempo que ejecuta cada proceso

extern queue_t readyQueue; // cola compartida con procesos listos

// se simula la ejecucion de los procesos, y se implementa el scheduling round robin
void *cpu_run(void *arg)
{
    int idCPU = *(int *)arg;

    while (1)
    {
        pthread_mutex_lock(&mutexQueue); // bloquea la cola para que otra CPU no pueda acceder a ella mientras se saca un proceso

        while (isEmpty(&readyQueue))
        {
            pthread_cond_wait(&condicionQueue, &mutexQueue); // si no hay procesos listos, espera
        }

        pcb_t proceso = dequeue(&readyQueue); // saca un proceso de la cola
        pthread_mutex_unlock(&mutexQueue);    // quita el bloqueo del mutex
        proceso.state = RUNNING;              // cambia el estado del proceso a RUNNING
        printf("CPU: %d Ejecutando proceso %d\n", idCPU, proceso.pid);
        sleep(QUANTUM);                    // aqui se simula la ejecucion del proceso
        proceso.remaining_time -= QUANTUM; // reduce el tiempo restante

        if (proceso.remaining_time <= 0) // si el proceso ya termino
        {
            proceso.state = FINISHED; // cambia el estado del proceso a FINISHED
            procesosTerminados++;     // suma al contador de procesos terminados
            printf("CPU: %d termino el proceso %d\n", idCPU, proceso.pid);
        }
        else // si todavia no termina, lo vuelve a poner en readyQueue
        {
            proceso.state = READY;
            printf("CPU: %d Proceso %d restante: %d\n", idCPU, proceso.pid, proceso.remaining_time);
            pthread_mutex_lock(&mutexQueue);
            enqueue(&readyQueue, proceso);        // el proceso se vuelve a ingrear en la cola
            pthread_cond_signal(&condicionQueue); // despierta a otra CPU que estaba esperando
            pthread_mutex_unlock(&mutexQueue);    // se desbloquea la cola para otra cpu
        }
    }
    return NULL;
}