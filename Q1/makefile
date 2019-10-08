CFLAGS = -g -Wall -pthread

all: utils.o normal.o multiproc.o multithread.o quicksort.o
	gcc $(CFLAGS) utils.o normal.o multiproc.o multithread.o quicksort.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
utils.o: utils.c main.h
	gcc $(CFLAGS) -c utils.c

normal.o: normal.c main.h
	gcc $(CFLAGS) -c normal.c

multiproc.o: multiproc.c main.h
	gcc $(CFLAGS) -c multiproc.c

multithread.o: multithread.c main.h
	gcc $(CFLAGS) -c multithread.c

quicksort.o: quicksort.c main.h
	gcc $(CFLAGS) -c quicksort.c