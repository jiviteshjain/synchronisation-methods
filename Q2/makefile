CFLAGS = -g -Wall -pthread

all: sim.o
	gcc $(CFLAGS) sim.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
sim.o: sim.c sim.h
	gcc $(CFLAGS) -c sim.c