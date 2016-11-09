
/*****************************************
 * A kinda warm, but not really warm enough to be comfortable clipping program. featuring tesselation
 * 
 * Robert Colvin (Dr.Doom)
 * Mercer University
 * Fall 2016
 * 
 */

using namespace std;

#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "structs.h"
#include "globals.h"
#include "prototypes.h"
#include "sutherlandHodgman.cpp"

#define WINDOW_MAX 1000
#define WINDOW_MIN 0
#define VIEWPORT_MAX 900
#define VIEWPORT_MIN 100
#define MAX 100

//vertex lb,lt,rt,rb;

float SCALE_UNIFORM = 1.0; 	//a single scale variable for all dimensions
float reflected = -1; 		//-1 for no reflect, 1 for reflect

/* Define these two variables to have a global scope */
float DELTA_SPIN = 0.0;
float SPIN  = 0.0;


list<vertex*> verts;
//matrix stuff
/*
void buildEdges(void)
{
	lb = {VIEWPORT_MIN,VIEWPORT_*/

void vmatm (int SIZE, float *pA, float *pB)

// Matrix-vector multiplication
// pA is a pointer to the first element of matrix A
// pB is a pointer to the first element of vector B
// On return, B will contain transformed coordinates
{
   int i, j;
   float temp[4];

   for (i=0; i<SIZE; i++)
             temp[i] = 0.0;

   for (i=0; i < SIZE; i++)
     for (j = 0; j < SIZE; j++)
           temp[i] += *(pA+(i*SIZE+j)) * *(pB+j);
   
   for (i=0; i<SIZE; i++)
         *(pB+i) = temp[i];

}


void buildTranslate( float xshift, float yshift, float zshift, float *pA )
// Constructs tranlation matrix to translate along x, y, and z axes
{
     pA[ 0] = 1.0; pA[ 1] = 0.0; pA[ 2] = 0.0; pA[ 3] =   xshift;
     pA[ 4] = 0.0; pA[ 5] = 1.0; pA[ 6] = 0.0; pA[ 7] =   yshift;
     pA[ 8] = 0.0; pA[ 9] = 0.0; pA[10] = 1.0; pA[11] =   zshift;
     pA[12] = 0.0; pA[13] = 0.0; pA[14] = 0.0; pA[15] =      1.0;
}

void buildRotateZ( float theta, float *pA )
{
// Constructs rotation matrix about Z axis

     float phi;

     // Convert degrees to radians

     phi = theta * M_PI / 180.0;

     pA[ 0] =  cos(phi); pA[ 1] = -sin(phi); pA[ 2] = 0.0; pA[ 3] = 0.0;
     pA[ 4] = sin(phi); pA[ 5] = cos(phi); pA[ 6] = 0.0; pA[ 7] = 0.0;
     pA[ 8] = 0.0;       pA[ 9] = 0.0;      pA[10] = 1.0; pA[11] = 0.0;
     pA[12] = 0.0;       pA[13] = 0.0;      pA[14] = 0.0; pA[15] = 1.0;
}      
void buildScale(float scaleX,float scaleY,float scaleZ, float *pA)
//Creates matrix to scale polygon according to a scale factor for each axis
{
	
     pA[ 0] = scaleX; 	pA[ 1] = 0.0; 	 pA[ 2] = 0.0; 	  pA[ 3] =   0.0;
     pA[ 4] = 0.0; 	pA[ 5] = scaleY; pA[ 6] = 0.0; 	  pA[ 7] =   0.0;
     pA[ 8] = 0.0;	pA[ 9] = 0.0; 	 pA[10] = scaleZ; pA[11] =   0.0;
     pA[12] = 0.0; 	pA[13] = 0.0; 	 pA[14] = 0.0; 	  pA[15] =   1.0;
}
void buildReflect2d(float reflect, float *pA)
{//reflector variable set to -1 creates an identity matrix; set to 1, it reflects about vertical axis
     pA[ 0] = (reflect * -1.0); pA[ 1] = 0.0; pA[ 2] = 0.0; pA[ 3] = 0.0;
     pA[ 4] =  0.0; 		pA[ 5] = 1.0; pA[ 6] = 0.0; pA[ 7] = 0.0;
     pA[ 8] =  0.0; 		pA[ 9] = 0.0; pA[10] = 1.0; pA[11] = 0.0;
     pA[12] =  0.0; 		pA[13] = 0.0; pA[14] = 0.0; pA[15] = 1.0;
}
void applyTransformation( float *vp, int vpts, float *TM/*, float *pA, list<vertex*> l*/ ) 
// Applies the given transformation matrix TM to the vector vp containing
// all of the homegenous coordinates to define the object
{
	float temp[4];
	float *tmp;
        int i;

	tmp = &temp[0];

	for (i=0;i<vpts;i++)
	{
		*(tmp+0)= *(vp+(i*4)+0);
		*(tmp+1)= *(vp+(i*4)+1);
		*(tmp+2)= *(vp+(i*4)+2);
		*(tmp+3)= *(vp+(i*4)+3);
		vmatm( 4, TM, tmp );
		*(vp+(i*4)+0) = *(tmp+0); 
		*(vp+(i*4)+1) = *(tmp+1); 
		*(vp+(i*4)+2) = *(tmp+2); 
		*(vp+(i*4)+3) = *(tmp+3); 
        }/*
	for (list<vertex*>::iterator it = l.begin();it!=l.end();++it)
	{
		(*it)->x = ((*it)->x * pA[0]) + ((*it)->y * pA[4]) + ((*it)->z * pA[ 8]) + ((*it)->w * pA[12]);
		(*it)->y = ((*it)->x * pA[1]) + ((*it)->y * pA[5]) + ((*it)->z * pA[ 9]) + ((*it)->w * pA[13]);
		(*it)->z = ((*it)->x * pA[2]) + ((*it)->y * pA[6]) + ((*it)->z * pA[10]) + ((*it)->w * pA[14]);
		(*it)->w = ((*it)->x * pA[3]) + ((*it)->y * pA[7]) + ((*it)->z * pA[11]) + ((*it)->w * pA[15]);
	}*/
		
}

	
void PipeLine( float *vp, int vpts/*, list<vertex*> l*/ )
{
    /*  This routine will run the graphics transformation pipeline. 
     *  It is operating on x,y,z homogenous coordinates defined in a linear
     *  array.  These are converted to vertex points later for possible
     *  clipping before final display.
     */ 

    /* Set up useful temporary variables */
    float TransformationMatrix[16];
    float *TM;

    /* Don't forget to initialize the ponter! */
    TM = &TransformationMatrix[0];

    // Translate to origin  
    buildTranslate( -WINDOW_MAX/2, -WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM/*, l*/ );  
    //reflect operation
    buildReflect2d(reflected, TM);
    applyTransformation(vp, vpts, TM); 
    //scale operation
    buildScale(SCALE_UNIFORM, SCALE_UNIFORM, SCALE_UNIFORM, TM);
    applyTransformation(vp, vpts, TM/*, l*/);
    	
    // Perform the rotation operation
    buildRotateZ( SPIN, TM );	
    applyTransformation( vp, vpts, TM/*, l*/ );
    // Translate back to point
    buildTranslate( WINDOW_MAX/2, WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM/*, l*/ );   	

}


void defineArrow( float *apts/*, list<vertex*> l*/ )
{
   apts[ 0] = 350.0;  apts[ 1] = 450.0; apts[ 2] = 0.0; apts[ 3] = 1.0;
   apts[ 4] = 550.0;  apts[ 5] = 450.0; apts[ 6] = 0.0; apts[ 7] = 1.0;
   apts[ 8] = 550.0;  apts[ 9] = 350.0; apts[10] = 0.0; apts[11] = 1.0;
   apts[12] = 650.0;  apts[13] = 500.0; apts[14] = 0.0; apts[15] = 1.0;
   apts[16] = 550.0;  apts[17] = 650.0; apts[18] = 0.0; apts[19] = 1.0;
   apts[20] = 550.0;  apts[21] = 550.0; apts[22] = 0.0; apts[23] = 1.0;
   apts[24] = 350.0;  apts[25] = 550.0; apts[26] = 0.0; apts[27] = 1.0;
 //  apts[28] = 350.0;  apts[29] = 450.0; apts[30] = 0.0; apts[31] = 1.0;
/*
   vertex v6 = {350.0,550.0,0.0,1.0,NULL};
   vertex v5 = {550.0,550.0,0.0,1.0,&v6};
   vertex v4 = {550.0,650.0,0.0,1.0,&v5};
   vertex v3 = {650.0,500.0,0.0,1.0,&v4};
   vertex v2 = {550.0,350.0,0.0,1.0,&v3};
   vertex v1 = {550.0,450.0,0.0,1.0,&v2};
   vertex v0 = {350.0,450.0,0.0,1.0,&v1};

   l.push_back(&v0);
   l.push_back(&v1);
   l.push_back(&v2);
   l.push_back(&v3);
   l.push_back(&v4);
   l.push_back(&v5);
   l.push_back(&v6);
   /*
   verts.push_back(&v0);
   verts.push_back(&v1);
   verts.push_back(&v2);
   verts.push_back(&v3);
   verts.push_back(&v4);
   verts.push_back(&v5);
   verts.push_back(&v6);*/
}

void toVertex ( float *apts, struct vertex *vp, int pts )
{
	int i;

	for (i=0;i<pts;i++)
	{
	   (vp+i)->x = *(apts+(i*4)+0);
	   (vp+i)->y = *(apts+(i*4)+1);
	   (vp+i)->z = *(apts+(i*4)+2);
	   (vp+i)->w = *(apts+(i+4)+3);
	}
}


void drawArrow(/*list<vertex*> l*/vertex *vp, int points )
{
    int i;

//    list<vertex*>::iterator it;
	cout <<"------------------------------\n";
    glBegin(GL_LINE_LOOP);
    for (/*it=l.begin();it!=l.end();++it*/i=0;i<points;i++){
        glVertex2f( /*(*it)->x, (*it)->y*/(vp+i)->x,(vp+i)->y );
	cout<<"point "<<i<<" is "<<(vp+i)->x<<", " << (vp+i)->y <<endl;
    }
    glEnd();

}


void myinit( void )
{
/* attributes */

      glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */
      glPointSize(1.0);

/* set up viewing */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0, (float) WINDOW_MAX, 
                 0.0, (float) WINDOW_MAX);
      glMatrixMode(GL_MODELVIEW);
}



void display( void )
{

    int numArrowPoints;             // The number of points in the arrow 
    float point[MAX];         // Array to hold homogenous coordinates for arrow
    float *apts;              // Pointer for point array 

    struct vertex inVertexArray[MAX/4];	/* array of vertices to hold points */
    struct vertex *invp;                /* Pointer for inVertexArray  */  

    /* Make sure to initialize the pointers! */ 
    apts = &point[0];         // the pointer to the array of points 
    invp = &inVertexArray[0]; // the pointer to the array of vertices

    numArrowPoints = 7;             // the actual number of points in the arrow
    
    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    /* Define Viewport */
    glColor3f(1.0, 1.0, 1.0);
    glRecti(VIEWPORT_MIN,VIEWPORT_MIN,VIEWPORT_MAX,VIEWPORT_MAX);

    /* Define the arrow points */
    defineArrow( apts );

    /* Now start the process of rotating */
    PipeLine( apts, numArrowPoints );
    toVertex( apts, invp, numArrowPoints );

	struct vertex l[2] = {{VIEWPORT_MIN,VIEWPORT_MAX,0.0,1.0,NULL},{VIEWPORT_MIN,VIEWPORT_MIN,0.0,1.0,NULL}};
	struct vertex r[2] = {{VIEWPORT_MAX,VIEWPORT_MIN,0.0,1.0,NULL},{VIEWPORT_MAX,VIEWPORT_MAX,0.0,1.0,NULL}};
	struct vertex t[2] = {{VIEWPORT_MAX,VIEWPORT_MAX,0.0,1.0,NULL},{VIEWPORT_MIN,VIEWPORT_MAX,0.0,1.0,NULL}};
	struct vertex b[2] = {{VIEWPORT_MIN,VIEWPORT_MIN,0.0,1.0,NULL},{VIEWPORT_MAX,VIEWPORT_MIN,0.0,1.0,NULL}};

	struct vertex *lclip = &l[0];
	struct vertex *rclip = &r[0];
	struct vertex *bclip = &b[0];
	struct vertex *tclip = &t[0];

	struct vertex *outvp;
	outvp = new vertex[MAX];

	int *outLengthPtr = new int;


	SutherlandHodgmanPolygonClip(invp, outvp, numArrowPoints, outLengthPtr, lclip);
	invp = outvp;
	numArrowPoints = *outLengthPtr;
	//delete outvp;
	outvp = new vertex[MAX];
	SutherlandHodgmanPolygonClip(invp, outvp, numArrowPoints, outLengthPtr, rclip);
	invp = outvp;
	numArrowPoints = *outLengthPtr;
	//delete outvp;
	outvp = new vertex[MAX];
	SutherlandHodgmanPolygonClip(invp, outvp, numArrowPoints, outLengthPtr, bclip);
	invp = outvp;
	numArrowPoints = *outLengthPtr;
	//delete outvp;
	outvp = new vertex[MAX];
	SutherlandHodgmanPolygonClip(invp, outvp, numArrowPoints, outLengthPtr, tclip);
	invp = outvp;
	numArrowPoints = *outLengthPtr;
    
    glColor3f(1.0, 0.0, 0.0);
    /* Draw Scaled and Rotated Arrow */
    drawArrow( invp, numArrowPoints );
    glutSwapBuffers();

	delete outLengthPtr;
	delete outvp;
 }


void SpinDisplay(void)
{
    SPIN = SPIN + DELTA_SPIN;
    if (SPIN > 360.0) SPIN = SPIN - 360.0;
    glutPostRedisplay();
}
bool insideViewport(int x, int y)
{
	bool insideX = (x < VIEWPORT_MAX) && (x > VIEWPORT_MIN);
	bool insideY = (y < VIEWPORT_MAX) && (y > VIEWPORT_MIN);

	return (insideX && insideY);
}


void mouse(int button, int state, int x, int y) 
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN )
            {
		if (insideViewport(x,y))
		{
                	DELTA_SPIN = DELTA_SPIN + 1.0;
                	glutIdleFunc(SpinDisplay);
		}
		else
		{
			SCALE_UNIFORM = SCALE_UNIFORM * 1.05;		//INCREASE SCALE BY 5
			glutPostRedisplay();
		}
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
		if (insideViewport(x,y))
		{
                	DELTA_SPIN = DELTA_SPIN - 1.0;
                	glutIdleFunc(SpinDisplay);
		}
		else
		{
			SCALE_UNIFORM = SCALE_UNIFORM * 0.95;			//DECREASE SCALE BY 5
                	glutPostRedisplay();
            	}
	    }
            break;
        default:
            break;
    }
}
void keyboard( unsigned char key, int x, int y )
{ 
    if ( key == 'q' || key == 'Q') exit(0);
    if ( key == 'i' || key == 'I') 
    {
	reflected = -1.0;
        SPIN = 0.0;
	SCALE_UNIFORM = 1.0; 
   	glutIdleFunc(display);
    }
    if ( key == 'x' || key == 'X') 
    {
        glutIdleFunc(NULL);
    }
    if (key == 's' || key == 'S') 
        DELTA_SPIN=0.0;
    if (key == 'r' || key == 'R')
    {//for reflection, flip sign of reflector variable
	reflected = reflected * -1.0;
	glutPostRedisplay();
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(WINDOW_MAX,WINDOW_MAX); 
    glutInitWindowPosition(0,0); 
    glutCreateWindow("The Arrow"); 
    myinit(); 
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display); 
    glutMainLoop();
}

