//
// Created by Dawood Choksi on 2021-03-02.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"
#include "cpu.h"

int totalTasks = 0;
int maxTasks = 0;
struct node *listOfTasksHead;

Task *pickNextTask();

int turnAroundTimeCalc();

int waitingTimeCalc(int totalTAT, int burst);

void schedule() {
    Task *currentTask = NULL;
    int currentTaskBurst = 0;
    int waitingTime = 0;
    int totalWaitingTime = 0;
    int turnAroundTime = 0;
    int totalTurnAroundTime = 0;
    double avgWaitingTime = 0;
    double avgResponseTime = 0;
    double avgTAT = 0;

    while(totalTasks >= 1) {
        currentTask = pickNextTask();
        currentTaskBurst = currentTask->burst;

        turnAroundTime = turnAroundTimeCalc(turnAroundTime, currentTaskBurst);
        totalTurnAroundTime += turnAroundTime;
        waitingTime = waitingTimeCalc(turnAroundTime, currentTaskBurst);
        totalWaitingTime += waitingTime;

        run(currentTask, currentTaskBurst);

        totalTasks -= 1;
        delete(&listOfTasksHead, currentTask);
    }

    avgWaitingTime = totalWaitingTime / (double) maxTasks;
    avgResponseTime = avgWaitingTime;
    avgTAT = totalTurnAroundTime / (double) maxTasks;

    printf("\n");
    printf("Average waiting time = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTAT);
    printf("Average response time = %.2f\n", avgResponseTime);
}

int waitingTimeCalc(int totalTAT, int burst) {
    totalTAT -= burst;
    return totalTAT;
}

int turnAroundTimeCalc(int totalTAT, int burst) {
    totalTAT += burst;
    return totalTAT;
}

Task *pickNextTask() {
    struct node *listPtr;
    listPtr = listOfTasksHead;

    Task *currentTask;
    currentTask = listPtr->task;
    int currentTaskPriority = currentTask->priority;

    int taskPtrPriority = 0;

    // need to return task at the end of list, as list order is P4 -> P3 -> P2 -> P1

    while (listPtr->next) {
        taskPtrPriority = listPtr->task->priority;

        if (taskPtrPriority >= currentTaskPriority) {
            currentTask = listPtr->task;
            currentTaskPriority = currentTask->priority;
        }
        listPtr = listPtr->next;
    }

    taskPtrPriority = listPtr->task->priority;

    if (taskPtrPriority >= currentTaskPriority) {
        currentTask = listPtr->task;
        currentTaskPriority = currentTask->priority;
    }

    return currentTask;
}

void add(char *name, int priority, int burst) {
    struct task *createTask;

    createTask = (struct task *) malloc(sizeof(struct task));

    if (createTask == NULL) {
        printf("Did not allocate memory correctly for adding task.");
        EXIT_FAILURE;
    }

    createTask->burst = burst;
    createTask->name = name;
    createTask->priority = priority;

    totalTasks += 1;
    maxTasks += 1;

    insert(&listOfTasksHead, createTask);
}