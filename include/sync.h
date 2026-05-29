#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

extern pthread_mutex_t mutexQueue;
extern pthread_cond_t condicionQueue;

#endif