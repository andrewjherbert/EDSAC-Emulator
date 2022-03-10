CC = gcc
CFLAGS = -ansi -Wpedantic
SRC=arith.c control.c edsac.c error.c exec_ord.c \
     execute.c fetch.c init.c inout.c main.c  \
     memory.c mult.c shift.c printbits.c
OBJS=arith.o control.o edsac.o error.o exec_ord.o \
     execute.o fetch.o init.o inout.o main.o   \
     memory.o mult.o shift.o printbits.o

all: punch tprint edsac

edsac: proto.h $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) 

$(OBJS): edsac.h proto.h
arith.o init.o: orders.h
inout.o main.o: inout.h

punch.o: punch.h

punch: punch.o

tprint.o: tprint.h

tprint: tprint.o

clean:
	rm *.o

