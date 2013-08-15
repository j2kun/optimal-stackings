CC=g++
CFLAGS=-O3 -Wall

ALL=poker.o allfive allseven 

all: $(ALL)

allfive: allfive.cpp poker.o
	${CC} ${CFLAGS} allfive.cpp poker.o -o allfive

allseven: allseven.cpp poker.o
	${CC} ${CFLAGS} allseven.cpp poker.o -o allseven

poker.o: poker.cpp arrays.h
	${CC} -c ${CFLAGS} poker.cpp -o poker.o


clean: 
	rm $(ALL)

