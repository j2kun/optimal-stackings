CC=g++
CFLAGS=-c -Wall
OMP=-fopenmp
ALL=hillclimb.o poker.o poker 

all: $(ALL)

hillclimb.o: hillclimb.h hillclimb.cpp
	$(CC) $(OMP)  $(CFLAGS) hillclimb.cpp

poker.o: poker.h poker.cpp
	$(CC) $(OMP) $(CFLAGS) poker.cpp

poker: poker.o hillclimb.o
	$(CC) $(OMP) hillclimb.o poker.o -o $@

clean: 
	rm $(ALL)

