// From Foley et al., Introduction to Computer Graphics, Prog 3.9
/*
typedef struct vertex {
	float x, y;
} vertex;
*/
/*typedef vertex edge[2];
//typedef vertex vertexArray[100];	/* MAX is a declared constant */
#include <iostream>
//#include <vector>
//#include "structs.h"
void Intersect(vertex first, vertex second, vertex *clipBoundary,
			  vertex *intersectPt)
{
	if (clipBoundary[0].y == clipBoundary[1].y) {		/*horizontal*/
		intersectPt->y = clipBoundary[0].y;
		intersectPt->x = first.x + (clipBoundary[0].y - first.y) *
			(second.x - first.x) / (second.y - first.y);
	} else {											/*vertical*/
		intersectPt->x = clipBoundary[0].x;
		intersectPt->y = first.y + (clipBoundary[0].x - first.x) *
			(second.y - first.y) / (second.x - first.x);
	}
}

bool Inside(vertex testVertex, vertex* clipBoundary)
{
	if (clipBoundary[1].x > clipBoundary[0].x)	/*bottom*/
		if (testVertex.y >= clipBoundary[0].y)
			return true;
	if (clipBoundary[1].x < clipBoundary[0].x)	/*top*/
		if (testVertex.y <= clipBoundary[0].y)
			return true;
	if (clipBoundary[1].y > clipBoundary[0].y)	/*right*/
		if (testVertex.x <= clipBoundary[1].x)
			return true;
	if (clipBoundary[1].y < clipBoundary[0].y)	/*left*/
		if (testVertex.x >= clipBoundary[1].x)
			return true;
	return false;
}

void Output(vertex newVertex, int *outLength, vertex *outVertexArray)
{
	(*outLength)++;
	outVertexArray[*outLength - 1].x = newVertex.x;
	outVertexArray[*outLength - 1].y = newVertex.y;
}

void SutherlandHodgmanPolygonClip(vertex *inVertexArray,
	vertex *outVertexArray, int inLength, int *outLength, vertex *clip_boundary)
{
	vertex s, p, i;
	int j;

	*outLength = 0;
	s = inVertexArray[inLength - 1];	/* Start with the last vertex in inVertexArray */
	for (j = 0; j < inLength; j++) {
		p = inVertexArray[j];	/* Now s and p correspond to the vertices in Fig.3.33 */
		cout << "s is " << s.x << ", " << s.y<<endl;
		cout << "p is " << p.x << ", " << p.y<<endl;
		cout << "clip bound is (" << clip_boundary[0].x << ", " << clip_boundary[0].y<< ") and ("<<clip_boundary[1].x<<", "<<clip_boundary[1].y<<")"<<endl;
		if (Inside(p, clip_boundary)) {	/* Cases 1 and 4 */
cout <<63<<endl;
			if (Inside(s, clip_boundary)){
			cout<<65<<endl;
				Output(p, outLength, outVertexArray);	/* Case 1 */
			}
			else {										/* Case 4 */
				Intersect(s, p, clip_boundary, &i);
				Output(i, outLength, outVertexArray);
				Output(p, outLength, outVertexArray);
			}
		} else if(Inside(s, clip_boundary)) {			/* Cases 2 and 3 */
			cout<<74<<endl;
			Intersect(s, p, clip_boundary, &i);			/* Case 2 */
			Output(i, outLength, outVertexArray);
		}												/* No action for case 3 */
		s = p;					/* Advance to next pair of vertices */
	}
}
