#include <stdio.h>
#include <unistd.h>
#include "../include/metrics.h"

int completed_processes = 0;

void *metrics_thread(void *arg)
{

    while (1)
    {
        sleep(5);
        printf("\n===== METRICAS =====\n");
        printf("Procesos completados: %d\n", completed_processes);
        printf("====================\n\n");
    }
    return NULL;
}