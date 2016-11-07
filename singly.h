#ifndef SINGLY_H
#define SINGLY_H

using namespace std;

#include <stdio.h>
#include <iostream>
#include "structs.h"

class singly
{
	public:
		vertex* createVertex(GLfloat xCoord, GLfloat yCoord, GLfloat zCoord, GLfloat wCoord);
		triangle* createTriangle(vertex *vert1, vertex *vert2, vertex *vert3);
		void append(vertex *node);
		void append(triangle *node);
		void deleteVertex(vertex *node);
		void deleteTheWholeDamnThing();
		int getLength();
		void printList();
		void put_atIndex_(vertex *node, int index);
		vertex* last();
		vertex *head;
		triangle *t_head;
		
		singly()
		{
			t_head = new struct triangle;
			head = new struct vertex;
			head=NULL;
			length=0;
		}
	private:
		int length;
};


#endif
