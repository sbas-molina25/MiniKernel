#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"

//inicializa la cola
void iniciarCola(queue_t *cola)
{
    cola->inicio = NULL;
    cola->final = NULL;
    cola->size = 0;
}

// Inserta un proceso al final de la cola 
void enqueue(queue_t *cola, pcb_t proceso)
{
    nodo_t *nuevoNodo = (nodo_t *)malloc(sizeof(nodo_t));
    nuevoNodo->proceso = proceso;
    nuevoNodo->siguiente = NULL;

    if (cola->final == NULL)
    {
        cola->inicio = nuevoNodo;
        cola->final = nuevoNodo;
    }
    else
    {
        cola->final->siguiente = nuevoNodo;
        cola->final = nuevoNodo;
    }
    cola->size++;
}

// saca el proceso al inicio de la cola
pcb_t dequeue(queue_t *cola)
{
    pcb_t emptyProcess;
    emptyProcess.pid = -1;

    if (cola->inicio == NULL) // revisa si la cola esta vacia
    {
        return emptyProcess;
    }
    nodo_t *temp = cola->inicio;
    pcb_t process = temp->proceso;
    cola->inicio = cola->inicio->siguiente;

    if (cola->inicio == NULL) // revisa si ya no quedan nodos
    {
        cola->final = NULL;
    }

    free(temp);
    cola->size--;
    return process;
}

int isEmpty(queue_t *cola)
{
    return cola->inicio == NULL;
}