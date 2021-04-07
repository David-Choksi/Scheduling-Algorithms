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
struct node *next_node;

Task *pickNextTask();

int waitingTimeCalc(int totalCPUtime, int burst);

void printCalc(int totalWaitingTime, int totalResponseTime, int totalTurnAroundTime);

Task *pickNextTask() {
    struct node *listPtr;
    listPtr = listOfTasksHead;

    // need to return task at the end of list, as list order is P4 -> P3 -> P2 -> P1
    while (listPtr->next) {
        listPtr = listPtr->next;
    }

    Task *currentTask = listPtr->task;
    return currentTask;
}

void schedule() {
    Task *currentTask = NULL;
    Task *tempTask = NULL;
    int currentTaskBurst = 0;
    int currentTaskRemainingTime = 0;
    int waitingTime = 0;
    int totalWaitingTime = 0;
    int totalTurnAroundTime = 0;
    int totalCPUtime = 0;
    int totalResponseTime = 0;
    int cpuTIME = 0;

    next_node = listOfTasksHead;

    while(totalTasks >= 1) {
        currentTask = pickNextTask();
        currentTaskRemainingTime = currentTask->burst;
        currentTaskBurst = currentTask->burstTotal;

        if (currentTaskRemainingTime > QUANTUM) {
            cpuTIME = QUANTUM;
            currentTaskRemainingTime = currentTaskRemainingTime - cpuTIME;
        } else {
            cpuTIME = currentTaskRemainingTime;
            currentTaskRemainingTime = 0;
            totalTasks -= 1;
        }

        if (currentTask->firstTime == true) {
            currentTask->firstTime = false;
            totalResponseTime += totalCPUtime;
        }

        totalCPUtime += cpuTIME;

        run(currentTask, cpuTIME);

        currentTask->burst = currentTaskRemainingTime;

        tempTask = currentTask;

        if (currentTaskRemainingTime <= 0) {
            waitingTime = waitingTimeCalc(totalCPUtime, currentTaskBurst);
            totalWaitingTime += waitingTime;

            totalTurnAroundTime += totalCPUtime;

            delete(&listOfTasksHead, currentTask);
        } else {
            delete(&listOfTasksHead, currentTask);
            insert(&listOfTasksHead, tempTask);
        }
    }

    printCalc(totalWaitingTime, totalResponseTime, totalTurnAroundTime);
}

void printCalc(int totalWaitingTime, int totalResponseTime, int totalTurnAroundTime) {
    double avgWaitingTime = 0;
    double avgResponseTime = 0;
    double avgTAT = 0;

    avgWaitingTime = totalWaitingTime / (double) maxTasks;
    avgResponseTime = totalResponseTime / (double) maxTasks;
    avgTAT = totalTurnAroundTime / (double) maxTasks;

    printf("\n");
    printf("Average waiting time = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTAT);
    printf("Average response time = %.2f\n", avgResponseTime);
}

int waitingTimeCalc(int totalCPUtime, int burst) {
    totalCPUtime -= burst;
    return totalCPUtime;
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
    createTask->burstTotal = burst;
    createTask->firstTime = true;

    totalTasks += 1;
    maxTasks += 1;

    insert(&listOfTasksHead, createTask);
}