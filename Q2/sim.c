#include "main.h"
#include "chef.h"
#include "table.h"
#include "foodie.h"

int main(void) {
    srand(time(0));
    done = false;
    
    int num_chefs, num_tables, num_foodies;
    printf("Enter number of chefs, tables and students: ");
    scanf("%d %d %d", &num_chefs, &num_tables, &num_foodies);

    Chef** chefs = (Chef**)malloc(sizeof(Chef*) * num_chefs);
    Table** tables = (Table**)malloc(sizeof(Table*) * num_tables);
    Foodie** foodies = (Foodie**)malloc(sizeof(Foodie*) * num_foodies);

    printf("Running simulation.\n");

    for (int i = 0; i < num_chefs; i++) {
        chefs[i] = (Chef*)malloc(sizeof(Chef));
        chef_init(chefs[i], i);

        pthread_create(&(chefs[i]->tid), NULL, chef_run, (void*)chefs[i]);
    }
    for (int i = 0; i < num_tables; i++) {
        tables[i] = (Table*)malloc(sizeof(Table));
        table_init(tables[i], i, chefs, num_chefs);

        pthread_create(&(tables[i]->tid), NULL, table_run, (void*)tables[i]);
    }
    for (int i = 0; i < num_foodies; i++) {
        foodies[i] = (Foodie*)malloc(sizeof(Foodie));
        foodie_init(foodies[i], i, tables, num_tables);

        pthread_create(&(foodies[i]->tid), NULL, foodie_run, (void*)foodies[i]);
    }

    for (int i = 0; i < num_foodies; i++) {
        pthread_join(foodies[i]->tid, NULL);
    }

    done = true;
    for (int i = 0; i < num_tables; i++) {
        Table* t = tables[i];

        if (pthread_mutex_trylock(&(t->protect)) == 0) {

            pthread_cond_signal(&(t->cv_foodie));
            pthread_mutex_unlock(&(t->protect));
            // if (t->state == TABLE_ST_SERVING) {
            //     int n = t->total_slots - t->left_slots;

            //     if (n < 0 || n >= TABLE_SLOTS_LIMIT) {
            //         pthread_mutex_unlock(&(t->protect));
            //         continue;
            //     }
                
            //     for (int j = 0; j < n; j++) {
            //         // printf for each foodie in table
            //     }
            // }

        }
    }

    for (int i = 0; i < num_chefs; i++) {
        Chef* c = chefs[i];

        if (pthread_mutex_trylock(&(c->protect)) == 0) {
            pthread_cond_signal(&(c->cv_table));
            pthread_mutex_unlock(&(c->protect));
        }
    }

    for (int i = 0; i < num_tables; i++) {
        pthread_join(tables[i]->tid, NULL);
    }
    for (int i = 0; i < num_chefs; i++) {
        pthread_join(chefs[i]->tid, NULL);
    }

    printf("Simulation Done.\n");

    for (int i = 0; i < num_chefs; i++) {
        pthread_mutex_destroy(&(chefs[i]->protect));
    }
    for (int i = 0; i < num_tables; i++) {
        pthread_mutex_destroy(&(tables[i]->protect));
    }
}