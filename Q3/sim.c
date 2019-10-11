#include "main.h"
#include "rider.h"
#include "cab.h"
#include "server.h"

int main(void) {
    srand(time(0));
    pthread_mutex_init(&num_pool_one_protect, NULL);
    num_pool_one = 0;

    // pthread_mutex_init(&rider_server_protect, NULL);
    sem_init(&sem_rich_riders, 0, 0);

    int num_riders, num_cabs, num_servers;
    printf("Enter number of riders, cabs and servers: ");
    scanf("%d %d %d", &num_riders, &num_cabs, &num_servers);

    Rider** riders = (Rider**)malloc(sizeof(Rider*) * num_riders);
    Cab** cabs = (Cab**)malloc(sizeof(Cab*) * num_cabs);
    Server** servers = (Server**)malloc(sizeof(Server*) * num_servers);

    printf("\nRunning simulation.\n");

    for (int i = 0; i < num_riders; i++) {
        riders[i] = (Rider*)malloc(sizeof(Rider));
        rider_init(riders[i], i);
        pthread_create(&(riders[i]->tid), NULL, rider_run, (void*)riders[i]);
    }
    for (int i = 0; i < num_cabs; i++) {
        cabs[i] = (Cab*)malloc(sizeof(Cab));
        cab_init(cabs[i], i, riders, num_riders);
        pthread_create(&(cabs[i]->tid), NULL, cab_run, (void*)cabs[i]);
    }
    for (int i = 0; i < num_servers; i++) {
        servers[i] = (Server*)malloc(sizeof(Server));
        server_init(servers[i], i, riders, num_riders);
        pthread_create(&(servers[i]->tid), NULL, server_run, servers[i]);
    }

    for (int i = 0; i < num_riders; i++) {
        pthread_join(riders[i]->tid, NULL);
        }

    printf("\nSimulation done.\n");
}