OBJS=lukewarm.o singly.o myTesselate.o myVertexMath.o
LIBS=-lglut -lGLU -lGL
CC=g++
CFLAG=-O3
DEPS=structs.h


lukewarm.o: lukewarm.cpp $(DEPS)
	$(CC) lukewarm.cpp $(LIBS) $(CFLAG) -o Program1

myTesselate.o: myTesselate.cpp 
	$(CC) myTesselate.cpp -c $(LIBS) $(CFLAG)

myVertexMath.o: myVertexMath.cpp 
	$(CC) myVertexMath.cpp -c $(LIBS) $(CFLAG)

singly.o: singly.cpp 
	$(CC) singly.cpp -c $(CFLAG)

clean:
	rm myTesselate.o
	rm myVertexMath.o
	rm lukewarm.o
	rm singly.o
	rm Program1
