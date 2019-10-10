#include "main.h"
#include "rider.h"
#include "cab.h"
#include "server.h"

void cab_init(Cab* c, int i, Rider** riders, int num_riders) {
    c->id = i;
    // tid will be set by thread

    c->state = CAB_ST_EMPTY;
    c->rider_a = NULL;
    c->rider_b = NULL;
    c->riders = riders;
    c->num_riders = num_riders;
}

void* cab_run(void* args) {
    Cab* self = (Cab*)args;
    self->tid = pthread_self();

    while (true) {
        if (self->state == CAB_ST_EMPTY) {
            int i = rand() % self->num_riders;
            Rider* rider = self->riders[i];
            pthread_mutex_lock(&(rider->protect));

            if (rider->state != RIDER_ST_READY) {
                pthread_cond_signal(&(rider->cv_cab));
                pthread_mutex_unlock(&(rider->protect));  // TODO: requires cond_signal
                continue;
            }

            if (rider->type == RIDER_TYPE_PREMIER) {
                self->state = CAB_ST_PREMIER;
                self->rider_a = rider;

                rider->state = RIDER_ST_RIDING;
                rider->cab = self;

                pthread_cond_signal(&(rider->cv_cab));
                pthread_mutex_unlock(&(rider->protect));

                sem_wait(&(rider->riding));
                self->state = CAB_ST_EMPTY;
                self->rider_a = NULL;
                continue;

            } else if (rider->type == RIDER_TYPE_POOL) { // HAS TO BE ONE OF THESE TWO TYPES
                pthread_mutex_lock(&num_pool_one_protect);
                if (num_pool_one > 0) {
                    pthread_mutex_unlock(&num_pool_one_protect);

                    pthread_cond_signal(&(rider->cv_cab));
                    pthread_mutex_unlock(&(rider->protect));

                    continue;
                } else {
                    num_pool_one++;
                    self->state = CAB_ST_POOL_ONE;
                    pthread_mutex_unlock(&num_pool_one_protect);

                    
                    self->rider_a = rider;

                    rider->state = RIDER_ST_RIDING;
                    rider->cab = self;

                    pthread_cond_signal(&(rider->cv_cab));
                    pthread_mutex_unlock(&(rider->protect));

                    continue;
                }
            }
        } else if (self->state == CAB_ST_POOL_ONE) {
            if (sem_trywait(&(self->rider_a->riding)) == 0) {
                pthread_mutex_lock(&num_pool_one_protect);
                self->state = CAB_ST_EMPTY;
                self->rider_a = NULL;
                pthread_mutex_unlock(&num_pool_one_protect);
                continue;
            }
            
            int i = rand() % self->num_riders;
            Rider* rider = self->riders[i];
            pthread_mutex_lock(&(rider->protect));

            if (rider->state != RIDER_ST_READY) {
                pthread_cond_signal(&(rider->cv_cab));
                pthread_mutex_unlock(&(rider->protect));  // TODO: requires cond_signal
                continue;
            }

            if (rider->type != RIDER_TYPE_POOL) {
                pthread_cond_signal(&(rider->cv_cab));
                pthread_mutex_unlock(&(rider->protect));  // TODO: requires cond_signal
                continue;
            }

            pthread_mutex_lock(&num_pool_one_protect);
            num_pool_one--;
            self->state = CAB_ST_POOL_TWO;
            pthread_mutex_unlock(&num_pool_one_protect);

            self->rider_b = rider;

            rider->state = RIDER_ST_RIDING;
            rider->cab = self;

            pthread_cond_signal(&(rider->cv_cab));
            pthread_mutex_unlock(&(rider->protect));

            continue;
        } else if (self->state == CAB_ST_POOL_TWO) {
            if (sem_trywait(&(self->rider_a->riding)) == 0) {
                pthread_mutex_lock(&num_pool_one_protect);
                num_pool_one++;
                self->state = CAB_ST_POOL_ONE;
                self->rider_a = self->rider_b;
                self->rider_b = NULL;
                pthread_mutex_unlock(&num_pool_one_protect);
                continue;
            }
            if (sem_trywait(&(self->rider_b->riding)) == 0) {
                pthread_mutex_lock(&num_pool_one_protect);
                num_pool_one++;
                self->state = CAB_ST_POOL_ONE;
                self->rider_b = NULL;
                pthread_mutex_unlock(&num_pool_one_protect);
                continue;
            }
        }
    }


}