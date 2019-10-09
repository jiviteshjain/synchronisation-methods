#include "main.h"
#include "rider.h"
#include "cab.h"
#include "server.h"

void* rider_run(void* args) {
    Rider* self = (Rider*) args;
    self->tid = pthread_self(); // rest of the things are set by rider_init

    pthread_mutex_t* protect_ptr = &(self->protect);
    pthread_cond_t* cv_cab_ptr = &(self->cv_cab);
    pthread_cond_t* cv_server_ptr = &(self->cv_server);

    int premature_time = rand() % 10;
    sleep(premature_time);

    pthread_mutex_lock(protect_ptr);
    
    time_t wait_start_time = time(NULL);
    self->state = RIDER_ST_READY;

    while (true) {
        if (time(NULL) - wait_start_time > self->wait_time) {
            return NULL;
        }

        if (self->state != RIDER_ST_READY) {
            break;
        } else {
            pthread_cond_wait(cv_cab_ptr, protect_ptr);
            // It will check for wait time in next iteration before accepting ride
        }
    }

    pthread_mutex_unlock(protect_ptr);

    // NOW STATE IS RIDING

    sleep(self->ride_time);
    sem_post(&(self->riding)); //TODO: Doing this before changing state - race? - it shouldn't - because servers and cabs don't interact anyways and that cab should be free to pull others and nobody will ever pull this rider now

    // NOW STATE IS WAITING FOR PAYMENT
    pthread_mutex_lock(protect_ptr);
    
    self->state = RIDER_ST_REACHED;

    while (true) {
        if (self->state == RIDER_ST_PAYING) {
            break;
        } else {
            pthread_cond_wait(cv_cab_ptr, protect_ptr);
        }
    }

    pthread_mutex_unlock(protect_ptr);

    sleep(PAYMENT_TIME);
    sem_post(&(self->paying));

    pthread_mutex_lock(protect_ptr);
    self->state = RIDER_ST_DONE;
    pthread_mutex_unlock(protect_ptr);
}
