#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#include <pthread.h>
#include <semaphore.h>

#include <time.h>
#include <wait.h>

#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

typedef struct Rider {
    int id;
    pthread_t tid;

    int type;
    time_t ride_time;
    time_t wait_time;

    int state;

    struct Cab* cab;
    // Server* server;

    pthread_mutex_t protect;  // protects the critical data of the rider

    pthread_cond_t cv_cab;  // conditional variables to use along with protect
    pthread_cond_t cv_server;

    sem_t riding;  // to block cab during ride
    sem_t paying;  // to block server while paying

} Rider;

typedef struct Cab {
    int id;
    pthread_t tid;

    int state;

    struct Rider* rider_a;
    struct Rider* rider_b;

    struct Rider** riders;
    int num_riders;

} Cab;

// PARAMETERS
#define PAYMENT_TIME (int)2

#define RIDE_TIME_OFFSET (int)1
#define RIDE_TIME_LIMIT (int)10

#define MAX_WAIT_TIME_OFFSET (int)50
#define MAX_WAIT_TIME_LIMIT (int)100

pthread_mutex_t num_pool_one_protect;
int num_pool_one;

#endif
