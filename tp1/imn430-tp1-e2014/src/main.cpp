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


//Variable definition
GLint windowWidth = 500;
GLint windowHeight = 500;
std::vector<DCEL::Vertex*> displaySites;
VoronoiDiagram diagram;

//Function declarations
void init();
void display();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void drawControlPoints();
void drawVoronoiDiagram();
void drawEdges();

//Function definitions
int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (windowWidth, windowHeight);
	glutInitDisplayMode (GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow ("VoronoiDiagram");
	
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
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixels);

        displaySites.push_back(new DCEL::Vertex(x,y));
        diagram.addSite(new DCEL::Vertex(x - windowWidth/2,-y + windowHeight/2));
        diagram.fortuneAlgorithm();
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

	for (int i = 0; i < displaySites.size(); ++i)
		glVertex2i((displaySites[i]->x) - windowWidth / 2, (-displaySites[i]->y) + windowHeight / 2); 
    
    glEnd();
    
    glPopMatrix();
}

void drawVoronoiDiagram()
{
    //TODO: a implanter
}

void drawEdges()
{
    //TODO: a implanter
}
