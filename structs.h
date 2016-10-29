#include <GL/glut.h>

typedef struct vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	vertex *next;
} vertex;
struct triangle
{
	vertex *p1;
	vertex *p2;
	vertex *p3;
	triangle *next;
};
