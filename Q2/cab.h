#ifndef __CAB_H
#define __CAB_H

#define CAB_ST_EMPTY (int)0
#define CAB_ST_PREMIER (int)1
#define CAB_ST_POOL_ONE (int)2
#define CAB_ST_POOL_TWO (int)3



void cab_init(Cab* c, int i, Rider** riders, int num_riders);
void* cab_run(void* args);

#endif
