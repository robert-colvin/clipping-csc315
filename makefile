OBJS=lukewarm.o singly.o
LIBS=-lglut -lGLU -lGL
CC=g++
CFLAG=-O3
DEPS=structs.h

#p1 : $(OBJS)
#	$(CC) $(OBJS) -o Frisbee

frisbee.o: lukewarm.cpp $(DEPS)
	$(CC) lukewarm.cpp $(LIBS) $(CFLAG) -o Program1

singly.o: singly.cpp 
	$(CC) singly.cpp -c $(LIBS) $(CFLAG)

clean:
	rm lukewarm.o
	rm singly.o
	rm Program1
