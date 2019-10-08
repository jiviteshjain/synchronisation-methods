#ifndef __MAIN_H
#define __MAIN_H

#define _POSIX_C_SOURCE 199309L  //required for clock
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

typedef struct {
    int start;
    int end;
    int* arr;
} args;

int get_pivot_index(int start, int end);

void swap(int arr[], int i, int j);

void normal_quick_sort(int arr[], int start, int end);

void multiproc_quick_sort(int arr[], int start, int end);

void* multithread_quick_sort(void* inp);

int main(void);

#endif
