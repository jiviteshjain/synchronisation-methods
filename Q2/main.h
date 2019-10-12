#ifndef __MAIN_H
#define __MAIN_H

#define _POSIX_C_SOURCE 199309L  //required for clock
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include <errno.h>
#include <time.h>
#include <wait.h>

#include <fcntl.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

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
#define FOODIE_DELAY_OFFSET (int)0
#define FOODIE_DELAY_LIMIT (int)5

#define COOKING_TIME_OFFSET (int)2
#define COOKING_TIME_LIMIT (int)4

#define CHEF_CAPACITY_OFFSET (int)1
#define CHEF_CAPACITY_LIMIT (int)10

#define VESSEL_CAPACITY_OFFSET (int)25
#define VESSEL_CAPACITY_LIMIT (int)26

#define TABLE_SLOTS_OFFSET (int)1
#define TABLE_SLOTS_LIMIT (int)10

#define EATING_TIME (int)2

typedef struct Chef {
    int id;
    pthread_t tid;

    pthread_mutex_t protect;
    pthread_cond_t cv_table;

    time_t cook_time;
    // int total_vessels;
    int left_vessels;
    int vessel_cap;
} Chef;

typedef struct Table {
    int id;
    pthread_t tid;

    int state;

    // struct Chef* chef;
    struct Chef** chefs;
    int num_chefs;

    // int total_vessel_cap;
    int left_vessel_cap;

    int total_slots;
    int left_slots;

    struct Foodie* foodies[TABLE_SLOTS_LIMIT];

    pthread_mutex_t protect;
    pthread_cond_t cv_foodie;

} Table;

typedef struct Foodie {
    int id;
    pthread_t tid;

    time_t arrival_time;

    struct Table** tables;
    int num_tables;
} Foodie;

bool done;

#endif