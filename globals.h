#ifndef GLOBALS
#define GLOBALS

#include <vector>
#include "structs.h"

/* Define these two variables to have a global scope */
extern float DELTA_SPIN;
extern float SPIN;

extern float SCALE_UNIFORM; 	//a single scale variable for all dimensions
extern float reflected; 		//-1 for no reflect, 1 for reflect
extern singly linkedList;//list of points
extern vector<vertex*> PolyVec;//for filled polygon after tesselation
extern vector<triangle> triangles;//vector of triangle
extern struct vertex *lastPoint;//these 2 ptrs help with tesselation
extern struct vertex *twoPointsAgo;

#endif
