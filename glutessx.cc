
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define WINDOW_MAX 700
#define WINDOW_MIN 0
#define VIEWPORT_MAX 600
#define VIEWPORT_MIN 100
#define MAX 100
//#define TESS 


typedef struct vertex { 
	float x;
        float y;
        float z;
	float w;
} vertex;

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


	
void defineArrow( float *apts )
{
   *(apts+0)=250.0;  *(apts+1)=300.0; *(apts+2)=0.0; *(apts+3)=1.0;
   *(apts+4)=450.0;  *(apts+5)=300.0; *(apts+6)=0.0; *(apts+7)=1.0;
   *(apts+8)=400.0;  *(apts+9)=250.0; *(apts+10)=0.0; *(apts+11)=1.0;
   *(apts+12)=550.0; *(apts+13)=350.0; *(apts+14)=0.0; *(apts+15)=1.0;
   *(apts+16)=400.0; *(apts+17)=450.0; *(apts+18)=0.0; *(apts+19)=1.0;
   *(apts+20)=450.0; *(apts+21)=400.0; *(apts+22)=0.0; *(apts+23)=1.0;
   *(apts+24)=250.0; *(apts+25)=400.0; *(apts+26)=0.0; *(apts+27)=1.0;

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

#ifdef TESS

    // Fill coord vector with positions and colors.
    // This must be done for all coordinates outside of 
    // the tesselator.  The tesselator will automatically 
    // increase the size of the array to account for other
    // vertices that it needs to add to tesselate properly.

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
    glFlush();

#else

    glColor3f(1.0,0.0,0.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glBegin(GL_POLYGON);
    for (i=0;i<points;i++)
        glVertex2f( (vp+i)->x, (vp+i)->y );
    glEnd();

    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

#endif

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

    // Make a local frame for the viewport 
    glPushMatrix();
    glLoadIdentity();
    glRecti(VIEWPORT_MIN,VIEWPORT_MIN,VIEWPORT_MAX,VIEWPORT_MAX); 
    glScissor(VIEWPORT_MIN,VIEWPORT_MIN,500,500);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glEnable(GL_SCISSOR_TEST);
    defineArrow( apts );           // Define the points for the arrow
    toVertex( apts, invp, inPoints ); 

//    uncomment the following two lines to see how the polygons are tesselated
//    glPolygonMode(GL_FRONT, GL_LINE);
//    glPolygonMode(GL_BACK, GL_LINE);

    drawArrow( invp, inPoints );

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_SCISSOR_TEST);
    glFlush();

}



void keyboard( unsigned char key, int x, int y )
{ 
  if ( key == 'q' || key == 'Q') exit(0);

}


int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(WINDOW_MAX,WINDOW_MAX); 
    glutInitWindowPosition(0,0); 
    glutCreateWindow("The Arrow"); 
    myinit(); 
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display); 
    glutMainLoop();
}

