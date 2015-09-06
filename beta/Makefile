CFLAGS = -Wall -O2 -ansi -pedantic -Wno-unused-result
CC = gcc
RM = rm
###########################################################################

fila: util.o queue.o filaTeste.o
	$(CC) util.o queue.o filaTeste.o -o fila  

filaTeste.o: filaTeste.c queue.h util.h item.h
	$(CC) $(CFLAGS) -c filaTeste.c

queue.o: queue.c queue.h util.h
	$(CC) $(CFLAGS) -c queue.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	$(RM) *~ *.o fila

