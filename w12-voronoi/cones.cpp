// Spring 2009 - Closest Post office:  A. Danner
//-----------------------------------------------------------------------------
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

typedef struct {
 GLfloat r, g, b, x, y;
} cone;

vector<cone> cones;

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc( );
void displayFunc( );
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void initialize( );
void addCone(int x, int y);

// width and height of the window
GLsizei g_width = 400;
GLsizei g_height = 400;

GLfloat world_h = 100;
GLfloat world_w = 100;

//-----------------------------------------------------------------------------
// Name: main( )
// Desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    srand(time(NULL));
    // initialize GLUT
    glutInit( &argc, argv );
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "Voronoi Sample" );

    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );

    // do our own initialization
    initialize();

    // let GLUT handle the current thread from here
    glutMainLoop();

    return 0;
}




//-----------------------------------------------------------------------------
// Name: initialize( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize()
{
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( 0.0f, 0.0f,0.0f, 1.0f );
    glShadeModel( GL_FLAT );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    addCone(200,200);
    addCone(300,200);
    addCone(300,300);
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    if(h > w){
       world_w = 100.;
       world_h = 100.*h/w;
    }
    else{
      world_h = 100.;
      world_w = 100.*w/h;
    }
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing box
    glOrtho(0,world_w,0,world_h,0,500);
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point
    gluLookAt(0., 0., 200., 0, 0., 0.0f, 0.0f, 1.0f, 0.0f );
    glutPostRedisplay();
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 'Q':
    case 'q':
	exit( 1 );
	break;
    }
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON ){
        if( state == GLUT_DOWN ){
	  addCone(x,y);
	}
    }
    
    glutPostRedisplay( );
}


void addCone(int x,int y){
   cone c;
   c.r = (rand()%256)/255.;
   c.g = (rand()%256)/255.;
   c.b = (rand()%256)/255.;
   c.x = world_w*x/g_width;
   c.y = world_h*(g_height-y)/g_height;
   cout << c.x << " " << c.y << endl;
   cones.push_back(c);
}

void drawCone(const cone & c){
   glPushMatrix();
   glColor3f(c.r, c.g, c.b);
   glTranslatef(c.x, c.y,0);
   glutSolidCone(150,150,100,100);
   glColor3f(0.,0.,0.);
   glTranslatef(0,0,160.);
   glutSolidSphere(0.6,20,20);
   glPopMatrix();
}


//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(unsigned int i=0; i<cones.size(); i++){
     drawCone(cones[i]);
  }
  glFlush( );
  glutSwapBuffers();
}





