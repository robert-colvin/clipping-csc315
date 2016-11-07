#ifndef MY_VERTEX_MATH_H
#define MY_VERTEX_MATH_H

GLfloat dotProduct(struct vertex *p1, struct vertex *p2, struct vertex *p3);

GLfloat magnitudeOf(vertex *p1, vertex *p2);

GLfloat angleBetween(vertex *p1, vertex *p2, vertex *p3);

bool intersect(vertex *p1, vertex *p2, vertex *p3, vertex *p4);

bool noIntersects(singly linkedlist, vertex *lastVertex, vertex *newVertex);

int numIntersects4Interior(singly linkedlist, GLfloat vertexX, GLfloat vertexY, int max_x);

bool inThatThang(singly linkedlist, vertex *p1, vertex *p2);

GLfloat crossProduct(struct vertex *p1, struct vertex *p2, struct vertex *p3);

#endif
