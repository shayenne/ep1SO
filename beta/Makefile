CFLAGS = -Wall -O2 -ansi -pedantic -Wno-unused-result
CC = gcc
RM = rm
###########################################################################

ep1: util.o queue.o ep1.o
	$(CC) util.o queue.o ep1.o -o ep1  

ep1.o: ep1.c queue.h util.h item.h ep1.h
	$(CC) $(CFLAGS) -c ep1.c

queue.o: queue.c queue.h util.h
	$(CC) $(CFLAGS) -c queue.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	$(RM) *~ *.o ep1

