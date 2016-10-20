
//dot product function for 2 line segments that share an endpoint
GLfloat dotProduct(struct vertex *point1, struct vertex *point2, struct vertex *point3)
{
	if(point1==NULL || point2==NULL)
		return 0;
	
	GLfloat line1x, line1y, line1z, line2x, line2y, line2z, dotProduct;
	
	line1x = (point1->x) - (point2->x);
	line1y = (point1->y) - (point2->y);
	line1z = 0.0;

	line2x = (point3->x) - (point2->x);
	line2y = (point3->y) - (point2->y);
	line2z = 0.0;

	dotProduct = (line1x * line2x) + (line2y * line1y) + (line1z * line2z);

	return dotProduct;
}


//to find the magnitude of a vector
GLfloat magnitudeOf(vertex *point1, vertex *point2)
{
	GLfloat lineX = (point1->x) - (point2->x);
	GLfloat lineY = (point1->y) - (point2->y);
	return sqrt (	pow(lineX, 2.0) + pow(lineY, 2.0)  );
}


//using dot product and magnitude to find angle between 2 vectors
GLfloat angleBetween(vertex *p1, vertex *p2, vertex *p3)
{
	GLfloat cosTheta, theta;
	cosTheta = dotProduct(p1,p2,p3)/(  magnitudeOf(p1,p2) * magnitudeOf(p2,p3)  );
	theta = acos(cosTheta);

	return theta;
}


//uses algorithm from class to determine if two line segments intersect
bool intersect(vertex *point1, vertex *point2, vertex *point3, vertex *point4)
{
	bool intersects = false;

	GLfloat ua, ub, x1, x2, x3, x4, y1, y2, y3, y4, numerator_a, numerator_b, denominator;

	x1 = point1->x; y1 = point1->y;
	x2 = point2->x; y2 = point2->y;
	x3 = point3->x; y3 = point3->y;
	x4 = point4->x; y4 = point4->y;
	
	numerator_a = ((x3-x1) * -(y4-y3)) - (-(x4-x3) * (y3-y1));
	numerator_b = ((x2-x1) *  (y3-y1)) - ( (x3-x1) * (y2-y1));
	denominator = ((x2-x1) * -(y4-y3)) - (-(x4-x3) * (y2-y1));

	ua = numerator_a/denominator;
	ub = numerator_b/denominator;

	if (0 < ua && ua < 1 &&
	    0 < ub && ub < 1)
	{
		intersects = true;
	}
	return intersects;
}


//does the above function for all lines in list
bool noIntersects(singly linkedlist,vertex *lastVertex, vertex *newVertex)
{
	if (linkedlist.getLength() < 3)
		return true;

	struct vertex *headChecker = linkedlist.head;
	struct vertex *anotherLineVertex1=linkedlist.head;
	struct vertex *anotherLineVertex2=linkedlist.head->next;
	while (anotherLineVertex2!=NULL)
	{
		if(intersect(lastVertex, newVertex, anotherLineVertex1, anotherLineVertex2) || 
		   intersect(newVertex, headChecker,anotherLineVertex1, anotherLineVertex2))	
			return false;

		anotherLineVertex1=anotherLineVertex2;
		anotherLineVertex2=anotherLineVertex2->next;
	}

	return true;
}


//a check for if a line exists in a polygon
int numIntersects4Interior(singly linkedlist,GLfloat vertexX, GLfloat vertexY)
{
	int county = 0;
	struct vertex *midpoint = linkedList.createVertex(vertexX, vertexY);
	struct vertex *boundaryPoint = linkedList.createVertex(WINDOW_MAX_X, vertexY);
	struct vertex *anotherLineVertex1=linkedlist.head;
	struct vertex *anotherLineVertex2=linkedlist.head->next;
	while (anotherLineVertex2!=NULL)
	{
		if(intersect(midpoint, boundaryPoint, anotherLineVertex1, anotherLineVertex2))	{
			county++;
			cout<<county<<endl;
		}

		anotherLineVertex1=anotherLineVertex2;
		anotherLineVertex2=anotherLineVertex2->next;
	}
	cout<<"county is "<< county <<endl;
	return county;
}


//checks if proposed line will exists in side polygon by checking how many times it intersects as it goes to the edge of the window
bool inThatThang(vertex *p1, vertex *p2)
{

	GLfloat line1x=p1->x, line1y=p1->y;
	GLfloat line2x=p2->x, line2y=p2->y;

	GLfloat middyBoyX = (line1x + line2x)/2.0; 
	GLfloat middyBoyY= (line1y + line2y)/2.0;
	
	int crossy = numIntersects4Interior(linkedList, middyBoyX, middyBoyY);

	if (crossy%2==1)//if it crosses an odd number of lines, it will exist inside the polygon
		return true;//this is good
	else
		return false;//no bueno
}


//crossproduct function for 2 line segments that share a point
GLfloat crossProduct(struct vertex *point1, struct vertex *point2, struct vertex *point3)
{
	if(point1==NULL || point2==NULL)
		return 0;
	
	GLfloat line1x, line1y, line1z, line2x, line2y, line2z, crossProductZcomponent;
	
	line1x = (point1->x) - (point2->x);
	line1y = (point1->y) - (point2->y);
	line1z = 0.0;

	line2x = (point3->x) - (point2->x);
	line2y = (point3->y) - (point2->y);
	line2z = 0.0;

	crossProductZcomponent = (line1x * line2y) - (line2x * line1y);

	return crossProductZcomponent;
}


