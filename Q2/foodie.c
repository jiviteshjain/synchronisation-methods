#include "main.h"
#include "table.h"
#include "chef.h"
#include "foodie.h"

void* foodie_run(void* args) {
    Foodie* self = (Foodie*)args;
    
    self->arrival_time = FOODIE_DELAY_OFFSET + rand() % FOODIE_DELAY_LIMIT;
    sleep(self->arrival_time);
    
    while (true) {
        int i = rand() % self->num_tables;
        Table* table = self->tables[i];
        pthread_mutex_lock(&(table->protect));

        if (table->state != TABLE_ST_SERVING) {
            pthread_cond_signal(&(table->cv_foodie));
            pthread_mutex_unlock(&(table->protect));
            continue;
        }

        if (table->left_slots <= 0) {
            pthread_cond_signal(&(table->cv_foodie));
            pthread_mutex_unlock(&(table->protect));
            continue;
        }

        table->left_slots--;
        int n = table->total_slots - table->left_slots;
        table->foodies[n - 1] = self;
        // TODO: print slot assigned

        pthread_cond_signal(&(table->cv_foodie));
        pthread_mutex_unlock(&(table->protect));
        break;
    }
    return;
}