#ifndef __RIDER_H
#define __RIDER_H

#define RIDER_ST_PREMATURE (int)0
#define RIDER_ST_READY (int)1
#define RIDER_ST_RIDING (int)2
#define RIDER_ST_REACHED (int)3
#define RIDER_ST_PAYING (int)4
#define RIDER_ST_DONE (int)5

#define RIDER_TYPE_PREMIER (int)0
#define RIDER_TYPE_POOL (int)1



void rider_init(Rider* r, int i);
void* rider_run(void* args);

#endif
