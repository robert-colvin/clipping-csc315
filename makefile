OBJS= singly.o myTesselate.o myVertexMath.o #sutherlandHodgman.o
LIBS=-lglut -lGLU -lGL
CC=g++
#CFLAG=-O2
HEADERS=structs.h singly.h globals.h prototypes.h myVertexMath.h


gl ?= n
ifeq ($(gl), y)
	CFLAG += -g -DGL
else
	CFLAG += -O2
endif



all: Pwnage

Pwnage : catastrophe.o $(OBJS)
	$(CC) $(CFLAG) catastrophe.o $(OBJS) -o Pwnage $(LIBS)

catastrophe.o: catastrophe.cpp $(HEADERS)
	$(CC) catastrophe.cpp $(CFLAG) -c

singly.o: singly.cpp $(HEADERS)
	$(CC) singly.cpp $(CLAG) -c

myTesselate.o: myTesselate.cpp $(HEADERS)
	$(CC) myTesselate.cpp $(CFLAG) -c

myVertexMath.o: myVertexMath.cpp $(HEADERS)
	$(CC) myVertexMath.cpp $(CFLAG) -c

#sutherlandHodgman.o: sutherlandHodgman.cpp $(HEADERS)
#	$(CC) sutherlandHodgman.cpp $(CFLAG) -c

clean:
	rm *.o

pristine:
	rm *.o
	touch *
