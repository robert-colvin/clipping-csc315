
#include <GL/glut.h>

#ifndef STRUCTS
#define STRUCTS

typedef struct vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	vertex *next;
} vertex;
typedef struct triangle
{
	vertex *p1;
	vertex *p2;
	vertex *p3;
	//triangle *next;
} triangle;
typedef struct line
{
	vertex *p1;
	vertex *p2;
	line *next;
} line;
typedef struct vArray
{
	int length;
	int *lengthPtr;
	vertex* vertices[100];
//	vertex *head;
} vArray;
#endif

