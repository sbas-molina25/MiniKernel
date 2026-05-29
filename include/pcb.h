#ifndef PCB_H
#define PCB_H

#define READY 0
#define RUNNING 1
#define FINISHED 2

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
    int priority;
    int arrival_time;
    int state;
} pcb_t;

#endif