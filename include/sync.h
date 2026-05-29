#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

extern pthread_mutex_t queue_mutex;
extern pthread_cond_t queue_cond;

#endif