#include "main.h"
#include "rider.h"
#include "cab.h"
#include "server.h"

void rider_init(Rider* r, int i) {
    r->id = i;
    // tid set by thread itself

    if (rand() % 2 == 0) {
        r->type = RIDER_TYPE_POOL;
    } else {
        r->type = RIDER_TYPE_PREMIER;
    }

    r->ride_time = RIDE_TIME_OFFSET + rand() % RIDE_TIME_LIMIT;
    r->wait_time = MAX_WAIT_TIME_OFFSET + rand() % MAX_WAIT_TIME_LIMIT;

    r->state = RIDER_ST_PREMATURE;

    r->cab = NULL;
    // r->server = NULL;

    pthread_mutex_init(&(r->protect), NULL);

    pthread_cond_init(&(r->cv_cab), NULL);
    // pthread_cond_init(&(r->cv_server), NULL);

    sem_init(&(r->riding), 0, 0);
    sem_init(&(r->paying), 0, 0);
}

void* rider_run(void* args) {
    Rider* self = (Rider*)args;
    self->tid = pthread_self();  // rest of the things are set by rider_init

    pthread_mutex_t* protect_ptr = &(self->protect);
    pthread_cond_t* cv_cab_ptr = &(self->cv_cab);
    // pthread_cond_t* cv_server_ptr = &(self->cv_server);

    int premature_time = rand() % 10;
    sleep(premature_time);

    if (self->type == RIDER_TYPE_PREMIER) {
        printf(ANSI_RED "RIDER %d WAITING FOR PREMIER\n" ANSI_DEFAULT, self->id);
    } else {
        printf(ANSI_RED "RIDER %d WAITING FOR POOL\n" ANSI_DEFAULT, self->id);
    }

    pthread_mutex_lock(protect_ptr);
    
    self->state = RIDER_ST_READY;

    struct timespec timer;
    clock_gettime(CLOCK_REALTIME, &timer);
    timer.tv_sec = timer.tv_sec + self->wait_time;

    while (true) {

        if (self->state != RIDER_ST_READY) {
            break;
        } else {
            if (pthread_cond_timedwait(cv_cab_ptr, protect_ptr, &timer) == ETIMEDOUT){
                self->state = RIDER_ST_DONE;
                sem_post(&(self->riding));
                sem_post(&(self->paying)); // TODO: Not needed, because server posts
                printf(ANSI_RED_BOLD "RIDER %d TIMED OUT\n" ANSI_DEFAULT, self->id);
                pthread_mutex_unlock(protect_ptr);
                return NULL;
            }
        }
    }

    pthread_mutex_unlock(protect_ptr);
    
    // NOW STATE IS RIDING
    if (self->type == RIDER_TYPE_PREMIER) {
        printf(ANSI_YELLOW "RIDER %d RIDING PREMIER IN CAB %d\n" ANSI_DEFAULT, self->id, self->cab->id);
    } else {
        if (self->cab->state == CAB_ST_POOL_ONE) {
            printf(ANSI_YELLOW "RIDER %d RIDING POOL ALONE IN CAB %d\n" ANSI_DEFAULT, self->id, self->cab->id);
        } else {
            printf(ANSI_YELLOW "RIDER %d RIDING POOL SHARED WITH RIDER %d IN CAB %d\n" ANSI_DEFAULT, self->id, (self->cab->rider_a->id != self->id ? self->cab->rider_a->id : self->cab->rider_b->id), self->cab->id);
        }
    }

    sleep(self->ride_time);
    sem_post(&(self->riding)); //TODO: Doing this before changing state - race? - it shouldn't - because servers and cabs don't interact anyways and that cab should be free to pull others and nobody will ever pull this rider now

    // NOW STATE IS WAITING FOR PAYMENT
    pthread_mutex_lock(protect_ptr);
    self->state = RIDER_ST_REACHED;
    self->cab = NULL;
    pthread_mutex_unlock(protect_ptr);

    printf(ANSI_CYAN "RIDER %d REACHED AND WAITING FOR PAYMENT\n" ANSI_DEFAULT, self->id);

    sem_post(&sem_rich_riders);
    // SOME SERVER CATCHES THIS AND MAKES IT'S STATE PAYING
    sem_wait(&(self->paying));
    // DONE
    pthread_mutex_lock(protect_ptr);
    self->state = RIDER_ST_DONE;
    pthread_mutex_unlock(protect_ptr);

    // printf(ANSI_GREEN "RIDER %d DONE\n" ANSI_DEFAULT, self->id);
    // while (true) {
    //     if (self->state == RIDER_ST_PAYING) {
    //         break;
    //     } else {
    //         pthread_cond_wait(cv_cab_ptr, protect_ptr);
    //     }
    // }

    // sem_post(&(self->paying));

    
    return NULL;
}
