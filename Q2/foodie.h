#ifndef __FOODIE_H
#define __FOODIE_H

void foodie_init(Foodie* f, int i, Table** tables, int num_tables);
void* foodie_run(void* args);
void wait_for_slot(Foodie* self);
void student_in_slot(Foodie* self);

#endif
