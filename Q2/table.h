#ifndef __TABLE_H
#define __TABLE_H

#define TABLE_ST_PREPARING (int)0
#define TABLE_ST_INTERMEDIATE (int)1
#define TABLE_ST_SERVING (int)2

void table_init(Table* t, int i, Chef** chefs, int num_chefs);
void* table_run(void* args);
void ready_to_serve_table(Table* self);

#endif
