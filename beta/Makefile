CFLAGS = -Wall -O2 -ansi -pedantic -Wno-unused-result
PFLAGS = -lpthread -pthread
CC = gcc
RM = rm
###########################################################################

ep1: util.o queue.o ep1.o FCFS.o SJF.o SRTN.o RR.o
	$(CC) $(PFLAGS) util.o queue.o ep1.o FCFS.o SJF.o SRTN.o RR.o -o ep1  

ep1.o: ep1.c queue.h util.h item.h ep1.h FCFS.h SJF.h SRTN.h RR.h
	$(CC) $(CFLAGS) $(PFLAGS) -c ep1.c

FCFS.o: FCFS.c FCFS.h queue.h util.h item.h impressao.h
	$(CC) $(CFLAGS) $(PFLAGS) -c FCFS.c

SJF.o: SJF.c SJF.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c SJF.c

SRTN.o: SRTN.c SRTN.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c SRTN.c

RR.o: RR.c RR.h queue.h util.h item.h
	$(CC) $(CFLAGS) -c RR.c

queue.o: queue.c queue.h util.h
	$(CC) $(CFLAGS) -c queue.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

ep1sh: ep1sh.o
	$(CC) -lreadline ep1sh.o -o ep1sh

ep1sh.o: ep1sh.c
	$(CC) -lreadline -c ep1sh.c



clean:
	$(RM) *~ *.o ep1 ep1sh

