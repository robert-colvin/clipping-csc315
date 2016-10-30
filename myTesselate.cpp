
#ifndef MY_TESSELATE
#define MY_TESSELATE

using namespace std;

#include <vector>
#include "structs.h"
#include "singly.h"
#include "prototypes.h"

//to draw a filled, untesselated polygon
void makeThePolygonSucka(/*singly linkedlist, */vector<vertex*> PolyVec)
{//loop through list of points and feed it to the glBegin
//	struct vertex *pointy = linkedlist.head;
	vector<vertex*>::iterator it;

	glBegin(GL_POLYGON);
		for(it=PolyVec.begin();it!=PolyVec.end();++it)
		{
			glVertex2f((*it)->x,(*it)->y);
			
		}
	glEnd();
	glFlush();
}
//post tesselation polygon goodness
void theresANewPolygonInTown(vector<triangle> triangles)
{//loop through vector of triangle structs and feed each point to glBegin as TRIANGLES
	vector<triangle>::iterator it;

	glBegin(GL_TRIANGLES);
		for(it=triangles.begin(); it!=triangles.end(); ++it)
		{
			vertex *p1=(*it).p1; vertex *p2=(*it).p2; vertex *p3=(*it).p3;
		
			glVertex2f(p1->x, p1->y);
			glVertex2f(p2->x, p2->y);
			glVertex2f(p3->x, p3->y);
		}
	glEnd();
	glFlush();
}
//clear the screen
void whiteItOut(vector<vertex*> polyvec)
{
	glColor3f(1.0, 1.0, 1.0);
	makeThePolygonSucka(polyvec);
	glColor3f(1.0, 0.0, 0.0);
}
//to connect the dots in the order they were added
void lineEmUpSucka(singly linkedlist)
{
	//kick you out if you try to connect just 1 point
	if (linkedlist.head==NULL || linkedlist.head->next==NULL)
	{
		cout << "can't draw lines with fewer than 2 points" << endl;
		exit(0);
	}
	struct vertex *firstPoint=linkedlist.head;
	struct vertex *secondPoint=linkedlist.head->next;
	//loop through list and connect each point one by one in the order it was put in list
	while (secondPoint!=NULL)
	{
		glBegin(GL_LINES);
			glVertex2f(firstPoint->x, firstPoint->y);
			glVertex2f(secondPoint->x, secondPoint->y);
		glEnd();
		firstPoint=secondPoint;
		secondPoint=secondPoint->next;
	}
	//connect final point to head
	glBegin(GL_LINES);
		glVertex2f(firstPoint->x, firstPoint->y);
		glVertex2f(linkedlist.head->x, linkedlist.head->y);
	glEnd();
	glFlush();
}
//sexy recursive tesselator function
void commenceTesselation(singly linkedlist, vector<triangle> triangles, struct vertex *p1, struct vertex *p2, struct vertex *p3)
{
	
	if (p1==NULL || p2 ==NULL || p3==NULL)//basically an error check
		return;
	if(crossProduct(p1, p2, p3) == 0.0)//if the 3 points we're working on are colinear, delete the second point and get out
	{
		linkedlist.deleteVertex(p2);
		return;
	}
	else if(crossProduct(p1, p2, p3) < 0.0 && 
		noIntersects(linkedlist, p1, p3) && inThatThang(linkedlist,p1, p3)) //if 3 points are countersclockwise, don't intersect anything if triangulated, and
	{//will draw a line within the shape
		//draw it
		glBegin(GL_LINES);
			glVertex2f(p1->x, p1->y);
			glVertex2f(p3->x, p3->y);
		glEnd();
		glFlush();
		//add a triangle struct of these points to the vector
		struct triangle TDawg = {p1, p2, p3};
		triangles.push_back(TDawg);

		//delete point from list	
		linkedlist.deleteVertex(p2);

		return;
	}
	else//if it fails the tests
	{//MOVE THE VERTICES DOWN THE LIST
		p1=p2;		
		p2=p3;	
		if (p3==linkedlist.last()){
			p3=linkedlist.head;	
		}
		else{
			p3=p3->next;
		}
		//recursion incoming w/ new points
		commenceTesselation(linkedlist,triangles, p1, p2 , p3);
	}
}

//checks for "special" 4 sided test cases, otherwise calls previous tesselator and manages triangles
void tesselateItSucka(singly linkedlist, vector<triangle> triangles)
{//starts tesselation
	//grab first 3 points
	struct vertex *p1 = linkedlist.head;
	struct vertex *p2 = p1->next;
	struct vertex *p3 = p2->next;
	
	//draw outline of shape
	lineEmUpSucka(linkedlist);
	if (linkedlist.getLength() < 3)//less than 3 points, do nothing
		return;
	if (linkedlist.getLength()==3)//equal to 3 points, already a triangle; add it to vector and leave
	{
		struct triangle TDawg = {p1,p2,p3};
		triangles.push_back(TDawg);
		return;
	}
	else//4 points or more
	{
		
		while (linkedlist.getLength() >= 3)//tessy loop
		{
			if (linkedlist.getLength()==3)//when the list is deleted down to 3 vertices, add that triangle to vector and leave
			{
				p1=linkedlist.head; p2=p1->next; p3=p2->next;
				struct triangle TDawg = {p1, p2, p3};
				triangles.push_back(TDawg);

				break;
			}
			if (linkedlist.getLength() == 4 && crossProduct(p1,p2,p3)>0.0)//if 4 sided and clockwise
			{
				vertex *p4 = p3->next;

				if (angleBetween(p2,p3,linkedlist.head) > angleBetween(p2,p3,p3->next))//if line will not exist within polygon
				{//draw other line in quadrilateral, delete vertex from list, add triangle to vector; rinse repeat for following

					glBegin(GL_LINES);
						glVertex2f(p2->x, p2->y);
						glVertex2f(p4->x, p4->y);
					glEnd();
					glFlush();
					
					linkedlist.deleteVertex(p3);

					struct triangle TDawg = {p2, p3, p4};
					struct triangle TShizzle = {p1, p2, p4};
					triangles.push_back(TDawg);
					triangles.push_back(TShizzle);
					break;
				}
				else
				{
					glBegin(GL_LINES);
						glVertex2f(p1->x, p1->y);
						glVertex2f(p3->x, p3->y);
					glEnd();
					glFlush();
	
					linkedlist.deleteVertex(p2);
					
					struct triangle TDawg = {p1, p2, p3};
					struct triangle TShizzle = {p1, p3, p4};
					triangles.push_back(TShizzle);
					triangles.push_back(TDawg);
					break;
				}
			}
			else if (linkedlist.getLength() == 4 && crossProduct(p1,p2,p3)<0.0)//if 4 sided and counterclockwise
			{
				vertex *p4 = p3->next;

				if(angleBetween(p2,p3,linkedlist.head) < angleBetween(p2,p3,p3->next))
				{
					glBegin(GL_LINES);
						glVertex2f(p1->x, p1->y);
						glVertex2f(p3->x, p3->y);
					glEnd();
					glFlush();

					struct triangle TDawg = {p1, p2, p3};
					struct triangle TShizzle = {p1, p3, p4};
					triangles.push_back(TShizzle);
					triangles.push_back(TDawg);
					break;
				}
				else
				{
					vertex *p4 = linkedlist.last();
					glBegin(GL_LINES);
						glVertex2f(p2->x, p2->y);
						glVertex2f(p4->x, p4->y);
					glEnd();
					glFlush();

					struct triangle TDawg = {p2, p3, p4};
					struct triangle TShizzle = {p1, p2, p4};
					triangles.push_back(TDawg);
					triangles.push_back(TShizzle);
					break;
				}
			}
			else
			{
					//more than 4 sides, get to the sexy stuff
					commenceTesselation(linkedlist,triangles, p1, p2, p3);	
					//reset points to first 3after every run through
					p1 = linkedlist.head;
					p2 = p1->next;
					p3 = p2->next;
			}
			//reset points 
			p1 = linkedlist.head;
			p2 = p1->next;
			p3 = p2->next;
			//for safety i suppose
			struct triangle TDawg = {p1,p2,p3};
			triangles.push_back(TDawg);
		}
	}
}

#endif
