CFLAGS = -Wall -O2 -ansi -pedantic -Wno-unused-result
CC = gcc
RM = rm
###########################################################################

ep1: util.o queue.o ep1.o FCFS.o SJF.o RR.o
	$(CC) util.o queue.o ep1.o FCFS.o SJF.o RR.o -o ep1  

ep1.o: ep1.c queue.h util.h item.h ep1.h FCFS.h SJF.h RR.h
	$(CC) $(CFLAGS) -c ep1.c

FCFS.o: FCFS.c FCFS.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c FCFS.c

SJF.o: SJF.c SJF.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c SJF.c

RR.o: RR.c RR.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c RR.c

queue.o: queue.c queue.h util.h
	$(CC) $(CFLAGS) -c queue.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	$(RM) *~ *.o ep1

