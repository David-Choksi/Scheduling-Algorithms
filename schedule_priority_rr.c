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
//int duplicateCount = 0;
int total = 0;
struct node *listOfTasksHead;
struct node *next_node;

Task *pickNextTask();

int waitingTimeCalc(int totalCPUtime, int burst);

int turnAroundTimeCalc(int totalTAT, int burst);

void printCalc(int totalWaitingTime, int totalResponseTime, int totalTurnAroundTime);

Task *pickNextTask() {
    struct node *listPtr;
    listPtr = listOfTasksHead->next;

    Task *currentTask;
    currentTask = listOfTasksHead->task;
    currentTask->counter = 1;
    int currentTaskPriority = currentTask->priority;

    int taskPtrPriority = 0;

    //duplicateCount = 0;

    // need to return task at the end of list, as list order is P4 -> P3 -> P2 -> P1

    total = 1;

    if (!listOfTasksHead->next) {
        return currentTask;
    }

    while (listPtr->next) {
        taskPtrPriority = listPtr->task->priority;

        if (taskPtrPriority > currentTaskPriority) {
            currentTask = listPtr->task;
            currentTaskPriority = currentTask->priority;
            total = 1;
            currentTask->counter = total;
            //duplicateCount = 0;
        } else if (taskPtrPriority == currentTaskPriority) {
            currentTask = listPtr->task;
            currentTaskPriority = currentTask->priority;
            total += 1;
            currentTask->counter = total;
            //duplicateCount += 1;
        }
        listPtr = listPtr->next;
    }

    taskPtrPriority = listPtr->task->priority;

    if (taskPtrPriority > currentTaskPriority) {
        currentTask = listPtr->task;
        currentTaskPriority = currentTask->priority;
        total = 1;
        currentTask->counter = total;
        //duplicateCount = 0;
    } else if (taskPtrPriority == currentTaskPriority) {
        currentTask = listPtr->task;
        currentTaskPriority = currentTask->priority;
        total += 1;
        currentTask->counter = total;
        //duplicateCount += 1;
    }

    return currentTask;
}

void schedule() {
    Task *currentTask = NULL;
    Task *tempTask = NULL;
    int currentTaskBurst = 0;
    int currentTaskRemainingTime = 0;
    int waitingTime = 0;
    //int turnAroundTime = 0;
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

//        if (duplicateCount == 0) {
        if (total <= 1) {
            // Priority
            if (currentTask->firstTime == true) {
                currentTask->firstTime = false;
                totalResponseTime += totalCPUtime;
            }
            totalCPUtime += currentTaskRemainingTime;
            totalTurnAroundTime += totalCPUtime;
            waitingTime = totalCPUtime - currentTaskBurst;
            totalWaitingTime += waitingTime;

            run(currentTask, currentTaskRemainingTime);

            totalTasks -= 1;
//            if (currentTask->burstTotal != currentTask->burst) {
//                printf("current task finished RR %s burst is %d remain is %d\n", currentTask->name, currentTask->burstTotal, currentTask->burst);
//            } else {
//                printf("current task finished: %s\n", currentTask->name);
//            }
            delete(&listOfTasksHead, currentTask);
        }

        else {
            // Round Robin
            if (currentTaskRemainingTime > QUANTUM) {
                cpuTIME = QUANTUM;
                currentTaskRemainingTime = currentTaskRemainingTime - cpuTIME;
            } else {
                cpuTIME = currentTaskRemainingTime;
                currentTaskRemainingTime = 0;
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
                totalTurnAroundTime += totalCPUtime;

                waitingTime = totalCPUtime - currentTask->burstTotal;
                totalWaitingTime += waitingTime;

                delete(&listOfTasksHead, currentTask);
                totalTasks -= 1;

            } else {
                delete(&listOfTasksHead, currentTask);
                insert(&listOfTasksHead, tempTask);
            }
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

int turnAroundTimeCalc(int totalTAT, int burst) {
    totalTAT += burst;
    return totalTAT;
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
    createTask->counter = 0;

    totalTasks += 1;
    maxTasks += 1;

    insert(&listOfTasksHead, createTask);
}