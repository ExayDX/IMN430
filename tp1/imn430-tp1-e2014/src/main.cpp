//
//  main.cpp
//  VoronoiDiagram
//
//  Created by Olivier Godin on 2013-05-11.
//
//

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
	#include <GL/glut.h>
	#include <GL/glu.h>
    #include <GL/gl.h>
#endif

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <set>

#include "DCEL.h"
#include "VoronoiDiagram.h"

#define GLUT_DISABLE_ATEXIT_HACK

<<<<<<< HEAD

//Variable definition
GLint windowWidth = 500;
GLint windowHeight = 500;
std::vector<DCEL::Vertex*> displaySites;
VoronoiDiagram diagram;
=======
using namespace std;
>>>>>>> dc9afffdc4133dc7f55576c30ed1efbfc2cecc1c

//Function declarations
void init();
void display();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void drawControlPoints();
void drawVoronoiDiagram();
//FIXME dev test
void drawEdges();

<<<<<<< HEAD
//Function definitions
=======
//Contains the sites' display position
std::vector<DCEL::Vertex*/*, DCEL::Vertex::Compare*/> displaySites;
VoronoiDiagram diagram;

GLint windowWidth = 500;
GLint windowHeight = 500;

>>>>>>> dc9afffdc4133dc7f55576c30ed1efbfc2cecc1c
int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (windowWidth, windowHeight);
	glutInitDisplayMode (GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow ("VoronoiDiagram");
	
	//GL hooks declaration
	glutDisplayFunc(display);
    glutMouseFunc(mouseButton);
	glutReshapeFunc(reshape);
    init();
    
	glutMainLoop();
	
	return 0;
}

void init()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport( 0,0, windowWidth, windowHeight );
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, windowWidth, 0.0, windowHeight, 1.0, -1.0 );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseButton( int button, int state, int x, int y )
{
	if(state == GLUT_DOWN)
	{
        GLfloat pixels[3];
<<<<<<< HEAD
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixels);

        displaySites.push_back(new DCEL::Vertex(x,y));
        diagram.addSite(new DCEL::Vertex(x - windowWidth/2,-y + windowHeight/2));
        diagram.fortuneAlgorithm();
=======
        glReadPixels(x, windowHeight - y, 1, 1, GL_RGB, GL_FLOAT, &pixels);

		//A site will not be created if it's position is the same of another one : 
		//Condition based on color (a site is RGB(1.0,0.0,0.0)). 
        if(!(pixels[0] == 1.0 && pixels[1] == 0.0 && pixels[2] == 0.0))
        {
            displaySites.push_back(new DCEL::Vertex(x,y));
			//The site's coordinate for the Voronoi computation is entered as if it was in a 
			//cartesian plan with the 0.0, 0.0 at the center of the window.
            diagram.addSite(DCEL::Vertex(x - windowWidth/2,-y + windowHeight/2));
//            diagram.addSite(DCEL::Vertex(-50 ,-50));
//            diagram.addSite(DCEL::Vertex(0 ,50));
//            diagram.addSite(DCEL::Vertex(75,-75));
            diagram.fortuneAlgorithm();
        }
>>>>>>> dc9afffdc4133dc7f55576c30ed1efbfc2cecc1c
	}

	glutPostRedisplay();
}

void display (void)
{
    glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-windowWidth/2.0,windowWidth/2.0,-windowHeight/2.0,windowHeight/2.0);
	glMatrixMode(GL_MODELVIEW);
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
    drawControlPoints();
    drawVoronoiDiagram();
    if(diagram.edges.size() >= 1)
        drawEdges();

    glFlush();
	glutSwapBuffers();
}

void reshape (int m_width, int m_height)
{
	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void drawControlPoints()
{
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(5);
    
    glBegin(GL_POINTS);

<<<<<<< HEAD
	for (int i = 0; i < displaySites.size(); ++i)
		glVertex2i((displaySites[i]->x) - windowWidth / 2, (-displaySites[i]->y) + windowHeight / 2); 
=======
		for (int i = 0; i < displaySites.size(); ++i)
			glVertex2i((displaySites[i])->x - windowWidth / 2, -(displaySites[i])->y + windowHeight / 2);
>>>>>>> dc9afffdc4133dc7f55576c30ed1efbfc2cecc1c
    
    glEnd();
    glPopMatrix();
}

void drawVoronoiDiagram()
{
    //TODO: a implanter
    //draw voronoid vertex
    
    if(diagram.getVertices().size() == 0)
        return;
    
    glColor3f(0.0f, 0.0f, 1.0f);
    glPointSize(5);
    
    glBegin(GL_POINTS);
    
    for(auto iter = diagram.getVertices().begin(); iter < diagram.getVertices().end(); iter++)
        glVertex2i(iter->x, iter->y);
    
    glEnd();
}

void drawEdges()
{
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(0.0f, 1.0f, 0.0f);
    
    glBegin(GL_LINES);
    
    for (int i = 0; i < diagram.edges.size(); ++i)
    {
        glVertex2i((diagram.edges[i])->getOrigin()->x, (diagram.edges[i])->getOrigin()->y);
        cout<<"p1: " << (diagram.edges[i])->getOrigin()->x << " " << (diagram.edges[i])->getOrigin()->y;
        glVertex2i((diagram.edges[i])->getTwin()->getOrigin()->x, (diagram.edges[i])->getTwin()->getOrigin()->y);
        cout<<"  p2: " << (diagram.edges[i])->getTwin()->getOrigin()->x << " " << (diagram.edges[i])->getTwin()->getOrigin()->y <<endl;
    }
    
    glEnd();
    glPopMatrix();
}
