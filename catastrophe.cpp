using namespace std;

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "structs.h"

#define WINDOW_MAX 1000
#define WINDOW_MIN 0
#define VIEWPORT_MAX 900
#define VIEWPORT_MIN 100
#define MAX 100

float xshift, yshift;
float SCALE_UNIFORM = 1.0; 	//a single scale variable for all dimensions
float reflected = -1; 		//-1 for no reflect, 1 for reflect
bool showFilled = false;
/* Define these two variables to have a global scope */
float DELTA_SPIN = 0.0;
float SPIN  = 0.0;
//unconditional compile---------------------------------------------------------------------------------------------------------
void display( void );

void defineArrow( float *apts )
{
   apts[ 0] = 350.0;  apts[ 1] = 450.0; apts[ 2] = 0.0; apts[ 3] = 1.0;
   apts[ 4] = 550.0;  apts[ 5] = 450.0; apts[ 6] = 0.0; apts[ 7] = 1.0;
   apts[ 8] = 550.0;  apts[ 9] = 350.0; apts[10] = 0.0; apts[11] = 1.0;
   apts[12] = 650.0;  apts[13] = 500.0; apts[14] = 0.0; apts[15] = 1.0;
   apts[16] = 550.0;  apts[17] = 650.0; apts[18] = 0.0; apts[19] = 1.0;
   apts[20] = 550.0;  apts[21] = 550.0; apts[22] = 0.0; apts[23] = 1.0;
   apts[24] = 350.0;  apts[25] = 550.0; apts[26] = 0.0; apts[27] = 1.0;

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



//GL version (Program 2)-----------------------------------------------------------------------------------------------------------------
//#define GL
#ifdef GL
#define TESS 

GLfloat transCoord = WINDOW_MAX/2.0;

#ifdef TESS         // These have to be defined for the GLU Tesselator

void beginCallback(GLenum which)
{
   glBegin(which);
}

void vertexCallback(void *data)
{
  glVertex3dv((GLdouble *) data); 
}

void endCallback(void)
{
   glEnd();
}

#endif

void drawArrow( vertex *vp, int points )
{
    int i;

#ifdef TESS

    // Fill coord vector with positions and colors.
    // This must be done for all coordinates outside of 
    // the tesselator.  The tesselator will automatically 
    // increase the size of the array to account for other
    // vertices that it needs to add to tesselate properly.
if(showFilled){
    GLdouble coords[points][6];
    for (i=0;i<points;i++) {
        coords[i][0] = (vp+i)->x;        // Coordinates
        coords[i][1] = (vp+i)->y;
        coords[i][2] = 0.0;
        coords[i][3] = 1.0;              // Colors
        coords[i][4] = 0.0;
        coords[i][5] = 0.0;
    }

    // Now start the process of calling the tesselator

    GLUtesselator* tobj = gluNewTess();

    gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*)()) &vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid (*) ())  &beginCallback);
    gluTessCallback(tobj, GLU_TESS_END, endCallback);

    gluTessBeginPolygon(tobj, NULL);
    gluTessBeginContour(tobj);
    for (i=0;i<points;i++) {
        gluTessVertex(tobj, coords[i], coords[i] );
    }
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
    gluDeleteTess(tobj);
    glutSwapBuffers();
}
//#else
else
{
    glColor3f(1.0,0.0,0.0);
    
    glBegin(GL_LINE_LOOP);
    for (i=0;i<points;i++)
        glVertex2f( (vp+i)->x, (vp+i)->y );
    glEnd();

}
#endif

}
void scaleForTheGLadies(GLfloat uniformScale)
{
	glTranslatef(transCoord,transCoord,0.0);
	glScalef(uniformScale,uniformScale,1.0);
	glTranslatef(-transCoord,-transCoord,0.0);
}
void rotateItGLStyle(GLfloat angle)
{
	//glPushMatrix();
	glTranslatef(transCoord,transCoord,0.0);
	glRotatef(angle,0.0,0.0,1.0);
	glTranslatef(-transCoord,-transCoord,0.0);
	//glPopMatrix();
}
void translateItForExtraCredit(GLfloat xmove, GLfloat ymove)
{
	//glTranslatef(transCoord,transCoord,0.0);
	glTranslatef(xmove,ymove,0.0);
	//glTranslatef(-transCoord,-transCoord,0.0);
	//glutPostRedisplay();
}

void display( void )
{

    int inPoints;                       // The number of points in the arro
    float point[100];                   // Array to hold homogenous coordinates for arrow
    float *apts;                        // Pointer for point array

    struct vertex inVertexArray[MAX/4];	/* array of vertices to hold points */
    struct vertex *invp;                /* pointer for inVertexArray   */

    /* Make sure to initialize the pointers! */

    apts = &point[0];                  // the pointer to the array of points
    invp = &inVertexArray[0];          // the pointer to the array of vertices

    inPoints = 7;                      // the actual number of points in the arrow

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    glLoadIdentity();              /* New - prior to any GL Operations, load identity matrix */           


    /* Draw Arrow in Viewport */
    glColor3f(1.0, 1.0, 1.0);

	GLsizei vwidth = WINDOW_MAX - (WINDOW_MAX - (VIEWPORT_MAX - VIEWPORT_MIN));

    // Make a local frame for the viewport 
    glPushMatrix();
    glLoadIdentity();
    glRecti(VIEWPORT_MIN,VIEWPORT_MIN,VIEWPORT_MAX,VIEWPORT_MAX); 
    glScissor(VIEWPORT_MIN, VIEWPORT_MIN, VIEWPORT_MAX-VIEWPORT_MIN, VIEWPORT_MAX-VIEWPORT_MIN);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
    glColor3f(0.6, 0.0, 1.0);
    glEnable(GL_SCISSOR_TEST);
    defineArrow( apts );           // Define the points for the arrow
    toVertex( apts, invp, inPoints ); 


	translateItForExtraCredit(xshift,yshift);
	rotateItGLStyle(SPIN);
	scaleForTheGLadies(SCALE_UNIFORM);



    drawArrow( invp, inPoints );

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_SCISSOR_TEST);
    glutSwapBuffers();

}
//my version (Program 1)-------------------------------------------------------------------------------------------------------------------
#else
#include "globals.h"
#include "prototypes.h"
#include "sutherlandHodgman.cpp"
#include <list>
list<vertex*> outline;
list<triangle> triangles;
//matrix stuff
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
void arrayToList(vertex *invp, int inLength, list<vertex*> &list)
{
	list.clear();

	for (int i=0;i<inLength;i++){
		vertex *vert = new vertex;
		vert = &invp[i];
		list.push_back(vert);
//		cout << list.back()->x<<"  "<<list.back()->y<<endl;
		//cout << "&   "<<vert.x<<"   "<<vert.y<<endl;
	}

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

    //for moving that thang
    buildTranslate(xshift,yshift,0.0,TM);
    applyTransformation(vp,vpts,TM);


    // Translate back to point
    buildTranslate( WINDOW_MAX/2, WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM/*, l*/ );   	

}
//*void drawArrow(list<vertex*> l, list<triangle> tris/*vertex *vp, int points*/ )
///{
	//if(!showFilled)
	//{
    	//	list<vertex*>::iterator it;
		//cout <<"------------------------------\n";
    
    		/*glBegin(GL_LINE_LOOP);
    		for (it=l.begin();it!=l.end();++it/*i=0;i<points;i++*)
		//{
        	//	glVertex2f( (*it)->x, (*it)->y/*(vp+i)->x,(vp+i)->y*/// );
		//	cout<<"point "<<i<<" is "<<it->x/*(vp+i)->x*/<<", " << /*(vp+i)->y*/it->y <<endl;
    		//}
    		//glEnd();
	//}
	//cout << l.size()<<" length"<<endl;
	//else
	//	theresANewPolygonInTown(tris);
//}

#define TESS 

GLfloat transCoord = WINDOW_MAX/2.0;

#ifdef TESS         // These have to be defined for the GLU Tesselator

void beginCallback(GLenum which)
{
   glBegin(which);
}

void vertexCallback(void *data)
{
  glVertex3dv((GLdouble *) data); 
}

void endCallback(void)
{
   glEnd();
}

#endif

void drawArrow( vertex *vp, int points )
{
    int i;

#ifdef TESS

    // Fill coord vector with positions and colors.
    // This must be done for all coordinates outside of 
    // the tesselator.  The tesselator will automatically 
    // increase the size of the array to account for other
    // vertices that it needs to add to tesselate properly.
if(showFilled){
    GLdouble coords[points][6];
    for (i=0;i<points;i++) {
        coords[i][0] = (vp+i)->x;        // Coordinates
        coords[i][1] = (vp+i)->y;
        coords[i][2] = 0.0;
        coords[i][3] = 1.0;              // Colors
        coords[i][4] = 0.0;
        coords[i][5] = 0.0;
    }

    // Now start the process of calling the tesselator

    GLUtesselator* tobj = gluNewTess();

    gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*)()) &vertexCallback);
    gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid (*) ())  &beginCallback);
    gluTessCallback(tobj, GLU_TESS_END, endCallback);

    gluTessBeginPolygon(tobj, NULL);
    gluTessBeginContour(tobj);
    for (i=0;i<points;i++) {
        gluTessVertex(tobj, coords[i], coords[i] );
    }
    gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
    gluDeleteTess(tobj);
    glutSwapBuffers();
}
//#else
else
{
    glColor3f(1.0,0.0,0.0);
    
    glBegin(GL_LINE_LOOP);
    for (i=0;i<points;i++)
        glVertex2f( (vp+i)->x, (vp+i)->y );
    glEnd();

}
}
#endif
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

	list<vertex*> verts;
	arrayToList(invp,numArrowPoints,verts);	
	
	outline = verts;
	
	list<triangle> tris;

	//tesselateItSucka(verts,tris);

	triangles = tris;

//   	cout<<verts.size()<<endl; 
    glColor3f(1.0, 0.0, 1.0);
    /* Draw Scaled and Rotated Arrow */
    drawArrow(invp, numArrowPoints );
    glutSwapBuffers();

	delete outLengthPtr;
	delete outvp;
 }
#endif

//unconditional compile----------------------------------------------------------------------------------------------------------------
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
    if ( key == 'f' || key == 'F'){
	    //draw that filled sumbitch
	showFilled = !showFilled;
	glutPostRedisplay();
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
void greatSpecsBro(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {yshift+=5; glutPostRedisplay();}
	if (key == GLUT_KEY_DOWN) {yshift-=5; glutPostRedisplay();}
	if (key == GLUT_KEY_LEFT) {xshift-=5; glutPostRedisplay();}
	if (key == GLUT_KEY_RIGHT) {xshift+=5; glutPostRedisplay();}
}
/*
void reshape (int width, int height)
{
	glViewport(100.0,100.0,(GLsizei)(width-100),(GLsizei)(height-100));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    	gluOrtho2D(0.0, (float) width, 
          	  0.0, (float) height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}*/
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
    glutSpecialFunc(greatSpecsBro);
    glutDisplayFunc(display); 

//	glutReshapeFunc(reshape);

    glutMainLoop();
}

