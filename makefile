OBJS= singly.o myTesselate.o myVertexMath.o
LIBS=-lglut -lGLU -lGL
CC=g++
#CFLAG=-O3
HEADERS=structs.h singly.h globals.h prototypes.h myVertexMath.h

flintstones ?= n
ifeq ($(flintstones), y)
	CFLAG += -g -DOLD
else
	CFLAG += -O2
endif



all: Program1

Program1 : lukewarm.o $(OBJS)
	$(CC) $(CFLAG) lukewarm.o $(OBJS) -o Program1 $(LIBS)

lukewarm.o: lukewarm.cpp $(HEADERS)
	$(CC) lukewarm.cpp $(CFLAG) -c

singly.o: singly.cpp $(HEADERS)
	$(CC) singly.cpp $(CLAG) -c

myTesselate.o: myTesselate.cpp $(HEADERS)
	$(CC) myTesselate.cpp $(CFLAG) -c

myVertexMath.o: myVertexMath.cpp $(HEADERS)
	$(CC) myVertexMath.cpp $(CFLAG) -c

clean:
	rm *.o

pristine:
	rm *.o
	touch *
