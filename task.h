/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

#include <stdbool.h>

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int burstTotal;
    bool firstTime;
    int counter;
} Task;

#endif
