#ifndef PROTOTYPES 
#define PROTOTYPES 

using namespace std;

#include <climits>
#include <cmath>
#include "structs.h"

#ifdef OLD

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

//tesselator primed
void tesselateItSucka(singly linkedlist, vector<triangle> triangles);

void commenceTesselation(singly, vector<triangle>, vertex*, vertex*, vertex*);

void lineEmUpSucka(singly)

void whiteItOut(vector<vertex*>);

void theresANewPolygonInTown(vector<triangle>);

void makeThePolygonSucka(vector<vertex*>);

#endif

#ifndef OLD
#include <list>

GLfloat dotProduct(struct vertex*, struct vertex*, struct vertex*);

GLfloat magnitudeOf(vertex*,vertex*);

GLfloat angleBetween(vertex*,vertex*,vertex*);

bool intersect(vertex*,vertex*,vertex*,vertex*);

bool noIntersects(list<vertex*>, vertex*,vertex*);

int numIntersects4Interior(list<vertex*>, GLfloat,GLfloat,int);

bool inThatThang(list<vertex*>,vertex*,vertex*);

GLfloat crossProduct(vertex*,vertex*,vertex*);

//-----------------------------------------------------------
void tesselateItSucka(list<vertex*>, list<triangle>);

void commenceTesselation(list<vertex*>, list<triangle>, vertex*, vertex*,vertex*);

void lineEmUpSucka(list<vertex*>);

void whiteItOut(list<vertex*>);

void theresANewPolygonInTown(list<triangle>);

void makeThePolygonSucka(list<vertex*>);

#endif


#endif

