CFLAGS = -g -Wall -pthread

all: chef.o table.o foodie.o sim.o
	gcc $(CFLAGS) chef.o table.o foodie.o sim.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
chef.o: chef.c chef.h main.h
	gcc $(CFLAGS) -c chef.c

table.o: table.c table.h main.h
	gcc $(CFLAGS) -c table.c

foodie.o: foodie.c foodie.h main.h
	gcc $(CFLAGS) -c foodie.c

sim.o: sim.c main.h
	gcc $(CFLAGS) -c sim.c