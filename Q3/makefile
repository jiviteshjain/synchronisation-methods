CFLAGS = -g -Wall -pthread

all: sim.o cab.o rider.o server.o
	gcc $(CFLAGS) sim.o cab.o rider.o server.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
sim.o: sim.c main.h
	gcc $(CFLAGS) -c sim.c

server.o: server.c server.h
	gcc $(CFLAGS) -c server.c

rider.o: rider.c rider.h
	gcc $(CFLAGS) -c rider.c

cab.o: cab.c cab.h
	gcc $(CFLAGS) -c cab.c