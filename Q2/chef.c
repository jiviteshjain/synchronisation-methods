#include "main.h"
#include "chef.h"
#include "table.h"
#include "foodie.h"

void chef_init(Chef* c, int i) {
    c->id = i;
    // TID SET WHILE CREATING THREAD

    pthread_mutex_init(&(c->protect), NULL);
    pthread_cond_init(&(c->cv_table), NULL);
}

void* chef_run(void* args) {
    Chef* self = (Chef*)args;

    while (true) {
        
        if (done) {
            break;
        }

        printf(ANSI_CYAN "CHEF %d COOKING\n" ANSI_DEFAULT, self->id);

        self->vessel_cap = VESSEL_CAPACITY_OFFSET + rand() % VESSEL_CAPACITY_LIMIT;

        self->cook_time = COOKING_TIME_OFFSET + rand() % COOKING_TIME_LIMIT;
        sleep(self->cook_time);

        pthread_mutex_lock(&(self->protect));
        
        self->left_vessels = CHEF_CAPACITY_OFFSET + rand() % CHEF_CAPACITY_LIMIT;

        printf(ANSI_CYAN "CHEF %d READY WITH %d VESSELS OF BIRYANI WITH A CAPACITY OF %d STUDENTS EACH\n" ANSI_DEFAULT, self->id, self->left_vessels, self->vessel_cap); // written inside to protect left_vessels

        // mutex unlocked by cond_wait in biryani ready

        biryani_ready(self);
    }
}

void biryani_ready(Chef* self) {
    
    while (true) {
        if (done) {
            break;
        }

        if (self->left_vessels <= 0) {
            break;
        } else{
            pthread_cond_wait(&(self->cv_table), &(self->protect));
        }
    }
    self->left_vessels = 0;
    
    pthread_mutex_unlock(&(self->protect));
    
    if (!done) {
        printf(ANSI_CYAN "CHEF %d OUT OF BIRYANI\n" ANSI_DEFAULT, self->id);
    }
}