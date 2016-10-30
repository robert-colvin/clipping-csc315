#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "structs.h"
#include "singly.h"
#include "globals.h"

#define WINDOW_MAX 1000
#define WINDOW_MIN 0
#define VIEWPORT_MAX 900
#define VIEWPORT_MIN 100
#define MAX 100

/* Define these two variables to have a global scope */
float DELTA_SPIN = 0.0;
float SPIN  = 0.0;

float SCALE_UNIFORM = 1.0; 	//a single scale variable for all dimensions
float reflected = -1; 		//-1 for no reflect, 1 for reflect
singly linkedList;//list of points
vector<vertex*> PolyVec;//for filled polygon after tesselation
vector<triangle> triangles;//vector of triangle
struct vertex *lastPoint=NULL;//these 2 ptrs help with tesselation
struct vertex *twoPointsAgo=NULL;

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


void buildTranslate( float x, float y, float z, float *pA )
// Constructs tranlation matrix to translate along x, y, and z axes
{
     pA[ 0] = 1.0; pA[ 1] = 0.0; pA[ 2] = 0.0; pA[ 3] =   x;
     pA[ 4] = 0.0; pA[ 5] = 1.0; pA[ 6] = 0.0; pA[ 7] =   y;
     pA[ 8] = 0.0; pA[ 9] = 0.0; pA[10] = 1.0; pA[11] =   z;
     pA[12] = 0.0; pA[13] = 0.0; pA[14] = 0.0; pA[15] = 1.0;
}
void buildScale(float scaleX,float scaleY,float scaleZ, float *pA)
//Creates matrix to scale polygon according to a scale factor for each axis
{
	
     pA[ 0] = scaleX; 	pA[ 1] = 0.0; 	 pA[ 2] = 0.0; 	  pA[ 3] =   0.0;
     pA[ 4] = 0.0; 	pA[ 5] = scaleY; pA[ 6] = 0.0; 	  pA[ 7] =   0.0;
     pA[ 8] = 0.0;	pA[ 9] = 0.0; 	 pA[10] = scaleZ; pA[11] =   0.0;
     pA[12] = 0.0; 	pA[13] = 0.0; 	 pA[14] = 0.0; 	  pA[15] =   1.0;
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

void buildReflect2d(float reflect, float *pA)
{//reflector variable set to -1 creates an identity matrix; set to 1, it reflects about vertical axis
     pA[ 0] = (reflect * -1.0); pA[ 1] = 0.0; pA[ 2] = 0.0; pA[ 3] = 0.0;
     pA[ 4] =  0.0; 		pA[ 5] = 1.0; pA[ 6] = 0.0; pA[ 7] = 0.0;
     pA[ 8] =  0.0; 		pA[ 9] = 0.0; pA[10] = 1.0; pA[11] = 0.0;
     pA[12] =  0.0; 		pA[13] = 0.0; pA[14] = 0.0; pA[15] = 1.0;
}

void applyTransformation( float *vp, int vpts, float *TM ) 
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
        }
}

	
void PipeLine( float *vp, int vpts )
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
    applyTransformation( vp, vpts, TM );   	

    //reflect operation
    buildReflect2d(reflected, TM);
    applyTransformation(vp, vpts, TM);
    //scale operation
    buildScale(SCALE_UNIFORM, SCALE_UNIFORM, SCALE_UNIFORM, TM);
    applyTransformation(vp, vpts, TM);
    
    // Perform the rotation operation
    buildRotateZ( SPIN, TM );	
    applyTransformation( vp, vpts, TM );
    // Translate back to point
    buildTranslate( WINDOW_MAX/2, WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM );   	

}

void defineArrow( float *apts )
{//	    X		       Y		 Z       MYSTERY MEAT
   apts[ 0] = 350.0;  apts[ 1] = 450.0; apts[ 2] = 0.0; apts[ 3] = 1.0; vertex *p1 = linkedList.createVertex((float)350.0,(float)450.0,(float)0.0,(float)1.0); 
   apts[ 4] = 550.0;  apts[ 5] = 450.0; apts[ 6] = 0.0; apts[ 7] = 1.0; vertex *p2 = linkedList.createVertex((float)550.0,(float)450.0,(float)0.0,(float)1.0);
   apts[ 8] = 550.0;  apts[ 9] = 350.0; apts[10] = 0.0; apts[11] = 1.0; vertex *p3 = linkedList.createVertex((float)550.0,(float)350.0,(float)0.0,(float)1.0);
   apts[12] = 650.0;  apts[13] = 500.0; apts[14] = 0.0; apts[15] = 1.0; vertex *p4 = linkedList.createVertex((float)650.0,(float)500.0,(float)0.0,(float)1.0);
   apts[16] = 550.0;  apts[17] = 650.0; apts[18] = 0.0; apts[19] = 1.0; vertex *p5 = linkedList.createVertex((float)550.0,(float)650.0,(float)0.0,(float)1.0);
   apts[20] = 550.0;  apts[21] = 550.0; apts[22] = 0.0; apts[23] = 1.0; vertex *p6 = linkedList.createVertex((float)550.0,(float)550.0,(float)0.0,(float)1.0);
   apts[24] = 350.0;  apts[25] = 550.0; apts[26] = 0.0; apts[27] = 1.0; vertex *p7 = linkedList.createVertex((float)350.0,(float)550.0,(float)0.0,(float)1.0);

   linkedList.append(p1);
   linkedList.append(p2);
   linkedList.append(p3);
   linkedList.append(p4);
   linkedList.append(p5);
   linkedList.append(p6);
   linkedList.append(p7);


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


void drawArrow( vertex *vp, int points )
{
    int i;

    glBegin(GL_LINE_LOOP);
    for (i=0;i<points;i++)
        glVertex2f( (vp+i)->x, (vp+i)->y );
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

    int inPoints;             // The number of points in the arrow 
    float point[MAX];         // Array to hold homogenous coordinates for arrow
    float *apts;              // Pointer for point array 

    struct vertex inVertexArray[MAX/4];	/* array of vertices to hold points */
    struct vertex *invp;                /* Pointer for inVertexArray  */  

    /* Make sure to initialize the pointers! */ 
    apts = &point[0];         // the pointer to the array of points 
    invp = &inVertexArray[0]; // the pointer to the array of vertices

    inPoints = 7;             // the actual number of points in the arrow
    
    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    /* Define Viewport */
    glColor3f(1.0, 1.0, 1.0);
    glRecti(VIEWPORT_MIN,VIEWPORT_MIN,VIEWPORT_MAX,VIEWPORT_MAX);

    /* Define the arrow points */
    defineArrow( apts );

    /* Now start the process of rotating */
    PipeLine( apts, inPoints);
    toVertex( apts, invp, inPoints );

    glColor3f(1.0, 0.0, 0.0);

    /* Draw Scaled and Rotated Arrow */
    drawArrow( invp, inPoints );
    glutSwapBuffers();


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
