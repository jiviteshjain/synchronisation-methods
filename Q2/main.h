#ifndef __MAIN_H
#define __MAIN_H

#define _POSIX_C_SOURCE 199309L  //required for clock
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
    // pthread_cond_t cv_server;

    sem_t riding;  // to block cab during ride
    sem_t paying;  // to block thread while paying

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

typedef struct Server {
    int id;
    pthread_t tid;

    int state;

    struct Rider* rider;

    struct Rider** riders;
    int num_riders;
} Server;

pthread_mutex_t num_pool_one_protect;
int num_pool_one;

// pthread_mutex_t rider_server_protect;
sem_t sem_rich_riders;

// COLOURS
#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RED_BOLD "\033[1;31m"
#define ANSI_GREEN_BOLD "\033[1;32m"
#define ANSI_YELLOW_BOLD "\033[1;33m"
#define ANSI_CYAN_BOLD "\033[1;36m"
#define ANSI_DEFAULT "\033[0m"
#define ANSI_CLEAR "\033[2J\033[1;1H"

// PARAMETERS
#define PAYMENT_TIME (int)2

#define RIDE_TIME_OFFSET (int)1
#define RIDE_TIME_LIMIT (int)10

#define MAX_WAIT_TIME_OFFSET (int)1
#define MAX_WAIT_TIME_LIMIT (int)5



#endif
