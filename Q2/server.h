#ifndef __SERVER_H
#define __SERVER_H

#define SERVER_ST_FREE (int)0
#define SERVER_ST_BUSY (int)1

void server_init(Server* s, int i, Rider** riders, int num_riders);
void* server_run(void* args);

#endif