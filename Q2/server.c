#include "main.h"
#include "rider.h"
#include "cab.h"
#include "server.h"


void server_init(Server* s, int i, Rider** riders, int num_riders) {
    s->id = i;
    // tid will be set by thread
    s->state = SERVER_ST_FREE;
    s->rider = NULL;
    s->num_riders = num_riders;
    s->riders = riders;
}

void* server_run(void* args) {
    Server* self = (Server*)args;

    while(true) {
        
        sem_wait(&sem_rich_riders);
        // 1 rider is surely available for grabs. This is because semaphore would allow the exact number of servers to proceed
        self->state = SERVER_ST_BUSY;
        
        // pthread_mutex_lock(&rider_server_protect);
        for (int i = 0; i < self->num_riders; i++) {
            
            pthread_mutex_lock(&(self->riders[i]->protect));
            
            if (self->riders[i]->state == RIDER_ST_REACHED) {
                
                self->rider = self->riders[i];
                // pthread_mutex_lock(&(self->rider->protect));
                self->rider->state = RIDER_ST_PAYING;
                
                pthread_mutex_unlock(&(self->rider->protect));
                break;
            
            } else {
                pthread_mutex_unlock(&(self->riders[i]->protect));
            }
        }
        // pthread_mutex_unlock(&rider_server_protect);

        if (self->rider == NULL) {
            continue;
        }
        printf(ANSI_CYAN "RIDER %d PAYING ON SERVER %d\n" ANSI_DEFAULT, self->rider->id, self->id);
        
        sleep(PAYMENT_TIME);
        sem_post(&(self->rider->paying));
        
        printf(ANSI_GREEN "RIDER %d DONE\n" ANSI_DEFAULT, self->rider->id);
        
        self->state = SERVER_ST_FREE;
        self->rider = NULL;
    }
}