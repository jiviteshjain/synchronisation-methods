#ifndef __RIDER_H
#define __RIDER_H

#define RIDER_ST_PREMATURE (int)0
#define RIDER_ST_READY (int)1
#define RIDER_ST_RIDING (int)2
#define RIDER_ST_REACHED (int)3
#define RIDER_ST_PAYING (int)4
#define RIDER_ST_DONE (int)5

#define RIDER_TYPE_PREMIER (int)0
#define RIDER_TYPE_POOL (int)1

typedef struct {
    int id;
    pthread_t tid;
    
    int type;
    time_t ride_time;
    time_t wait_time;

    int state;

    Cab* cab;
    Server* server;

    pthread_mutex_t protect; // protects the critical data of the rider

    pthread_cond_t cv_cab; // conditional variables to use along with protect
    pthread_cond_t cv_server;

    sem_t riding; // to block cab during ride
    sem_t paying; // to block server while paying

} Rider;

#endif
