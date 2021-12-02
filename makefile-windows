CC=gcc
SRC=arith.c control.c edsac.c error.c exec_ord.c \
     execute.c fetch.c init.c inout.c main.c  \
     memory.c mult.c shift.c printbits.c
OBJS=arith.o control.o edsac.o error.o exec_ord.o \
     execute.o fetch.o init.o inout.o main.o   \
     memory.o mult.o shift.o printbits.o

all: edsac.exe punch.exe tprint.exe
.PHONY: all

edsac.exe: proto.h $(OBJS)
	$(CC) -o $@ $(OBJS)

proto.h: $(SRC)
	cproto -o $@ $(SRC) 

$(OBJS): edsac.h  $(SRC)
arith.o init.o: orders.h

inout.o main.o: inout.h

punch.h: punch.c
	cproto -o $@ punch.c

punch.o: punch.h

punc.exeh: punch.o
	$(CC) -o $@ punch.o

tprint.h: tprint.c
	cproto -o $@ tprint.c

tprint.o: tprint.h

tprint.exe: tprint.o
	$(CC) -o $@ tprint.o

.PHONY: clean

clean:
	rm -f *.o proto.h punch.h tprint.h


