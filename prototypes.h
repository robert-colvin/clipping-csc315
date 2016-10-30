#ifndef PROTOTYPES 
#define PROTOTYPES 

#include <climits>
#include <cmath>
#include "singly.h"

//dot product function for 2 line segments that share an endpoint
GLfloat dotProduct(struct vertex*, struct vertex*, struct vertex*);

//to find the magnitude of a vector
GLfloat magnitudeOf(vertex*, vertex*);

//using dot product and magnitude to find angle between 2 vectors
GLfloat angleBetween(vertex*, vertex*, vertex*);

//uses algorithm from class to determine if two line segments intersect
bool intersect(vertex*, vertex*, vertex*, vertex*);

//does the above function for all lines in list
bool noIntersects(singly,vertex*, vertex*);

//a check for if a line exists in a polygon
int numIntersects4Interior(singly ,GLfloat, GLfloat, int  );

//checks if proposed line will exists in side polygon by checking how many times it intersects as it goes to the edge of the window
bool inThatThang(singly , vertex*, vertex*);


//crossproduct function for 2 line segments that share a point
GLfloat crossProduct(struct vertex*, struct vertex*, struct vertex*);

#endif

