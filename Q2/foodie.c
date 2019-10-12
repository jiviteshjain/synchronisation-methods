#include "main.h"
#include "table.h"
#include "chef.h"
#include "foodie.h"

void foodie_init(Foodie* f, int i, Table** tables, int num_tables) {
    f->id = i;
    // tid set by main

    f->num_tables = num_tables;
    f->tables = tables;
}

void* foodie_run(void* args) {
    Foodie* self = (Foodie*)args;
    
    self->arrival_time = FOODIE_DELAY_OFFSET + rand() % FOODIE_DELAY_LIMIT;
    sleep(self->arrival_time);

    printf(ANSI_GREEN "STUDENT %d ARRIVED AND HUNGRY\n" ANSI_DEFAULT, self->id);

    wait_for_slot(self);
    student_in_slot(self);

    return NULL;
}

void wait_for_slot(Foodie* self) {
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

        printf(ANSI_GREEN "STUDENT %d ASSIGNED SLOT ON TABLE %d AND SERVED BIRYANI\nSTUDENT %d DONE\n" ANSI_DEFAULT, self->id, table->id, self->id);

        pthread_cond_signal(&(table->cv_foodie));
        pthread_mutex_unlock(&(table->protect));
        break;
    }
}

void student_in_slot(Foodie* self) {
    // TODO: print
    return;
}