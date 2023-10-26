#include "halfEdge.h"
#include "vertex.h"
#include "face.h"
#include "mesh.h"
#include "objFile.h"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string>
#include <GL/glut.h>

using namespace std;

void display(void);
void display_basis(void);

void keyboard(unsigned char key, int x, int y);
void mouse(int, int, int, int);
void mouseMotion(int, int);
//void reshape(int, int);

//global variables for openGL
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX, cameraAngleY;
float cameraDistance = 0;

#define NBFACES 6
#define NBVERTICES 7
#define NBHALFEDGES 24
Mesh* ExMesh;


//---------------------------------------
void initMesh() {
    int tabHe[NBHALFEDGES][5] = { // vertex, heTwin, face, next, prev for each halfedges
        {1, 18, 0, 1, 2},
        {3, 11, 0, 2, 0},
        {4, 3, 0, 0, 1},
        {1, 2, 1, 4, 5},
        {4, 6, 1, 5, 3},
        {2, 19, 1, 3, 4},
        {2, 4, 2, 7, 8},
        {4, 17, 2, 8, 6},
        {5, 20, 2, 6, 7},
        {3, 21, 3, 10, 11},
        {6, 12, 3, 11, 9},
        {4, 1, 3, 9, 10},
        {4, 10, 4, 13, 14},
        {6, 22, 4, 14, 12},
        {7, 15, 4, 12, 13},
        {4, 14, 5, 16, 17},
        {7, 23, 5, 17, 15},
        {5, 7, 5, 15, 16},
        {3, 0, -1, 19, 21},
        {1, 5, -1, 20, 18},
        {2, 8, -1, 23, 19},
        {6, 9, -1, 18, 22},
        {7, 13, -1, 21, 23},
        {5, 16, -1, 22, 20}};

    int tabFace[NBFACES] ={0,3,6,9,12,15};    // he for each faces
    int tabVertex[NBVERTICES][4] ={           // x,y,z,he for each vertices 
        {1, 4, 0, 0},       
        {3, 4, 0, 5},
        {0, 2, 0, 1},
        {2, 2, 0, 2},
        {4, 2, 0, 8},
        {1, 0, 0, 10},
        {3, 0, 0, 14}};

    ExMesh = new Mesh() ;

    // create vertices (vertices numbered from 1 to NBVERTICES !)
    for(int i = 0; i <= NBVERTICES; i++){
        if(i==0){
            Vertex* nullV = nullptr;
            ExMesh->m_vertices.push_back(nullV);

        }  
        Vertex* vertex = new Vertex("v"+to_string(i), tabVertex[i][0], tabVertex[i][1], tabVertex[i][2]);
        ExMesh->m_vertices.push_back(vertex);
    }

    // create faces
    for(int i = 0; i < NBFACES; i++){
        Face* face = new Face("f"+to_string(i));
        ExMesh->m_faces.push_back(face);
    }

    // create halfEdges
    for(int i = 0; i < NBHALFEDGES; i++){
        HalfEdge* he = new HalfEdge("e"+to_string(i));
        ExMesh->m_hedges.push_back(he);
    }

    ExMesh->m_hedges.at(0)->m_vertex = ExMesh->m_vertices.at(1); 
    ExMesh->m_hedges.at(0)->m_heTwin = ExMesh->m_hedges.at(18);
    ExMesh->m_hedges.at(0)->m_face = ExMesh->m_faces.at(0);
    ExMesh->m_hedges.at(0)->m_heNext = ExMesh->m_hedges.at(1);
    ExMesh->m_hedges.at(0)->m_hePrev = ExMesh->m_hedges.at(2);

    ExMesh->m_hedges.at(1)->m_vertex = ExMesh->m_vertices.at(3); 
    ExMesh->m_hedges.at(1)->m_heTwin = ExMesh->m_hedges.at(11);
    ExMesh->m_hedges.at(1)->m_face = ExMesh->m_faces.at(0);
    ExMesh->m_hedges.at(1)->m_heNext = ExMesh->m_hedges.at(2);
    ExMesh->m_hedges.at(1)->m_hePrev = ExMesh->m_hedges.at(0);

    ExMesh->m_hedges.at(2)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(2)->m_heTwin = ExMesh->m_hedges.at(3);
    ExMesh->m_hedges.at(2)->m_face = ExMesh->m_faces.at(0);
    ExMesh->m_hedges.at(2)->m_heNext = ExMesh->m_hedges.at(0);
    ExMesh->m_hedges.at(2)->m_hePrev = ExMesh->m_hedges.at(1);

    ExMesh->m_hedges.at(3)->m_vertex = ExMesh->m_vertices.at(1); 
    ExMesh->m_hedges.at(3)->m_heTwin = ExMesh->m_hedges.at(2);
    ExMesh->m_hedges.at(3)->m_face = ExMesh->m_faces.at(1);
    ExMesh->m_hedges.at(3)->m_heNext = ExMesh->m_hedges.at(4);
    ExMesh->m_hedges.at(3)->m_hePrev = ExMesh->m_hedges.at(5);

    ExMesh->m_hedges.at(4)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(4)->m_heTwin = ExMesh->m_hedges.at(6);
    ExMesh->m_hedges.at(4)->m_face = ExMesh->m_faces.at(1);
    ExMesh->m_hedges.at(4)->m_heNext = ExMesh->m_hedges.at(5);
    ExMesh->m_hedges.at(4)->m_hePrev = ExMesh->m_hedges.at(3);

    ExMesh->m_hedges.at(5)->m_vertex = ExMesh->m_vertices.at(2); 
    ExMesh->m_hedges.at(5)->m_heTwin = ExMesh->m_hedges.at(19);
    ExMesh->m_hedges.at(5)->m_face = ExMesh->m_faces.at(1);
    ExMesh->m_hedges.at(5)->m_heNext = ExMesh->m_hedges.at(3);
    ExMesh->m_hedges.at(5)->m_hePrev = ExMesh->m_hedges.at(4);

    ExMesh->m_hedges.at(6)->m_vertex = ExMesh->m_vertices.at(2); 
    ExMesh->m_hedges.at(6)->m_heTwin = ExMesh->m_hedges.at(4);
    ExMesh->m_hedges.at(6)->m_face = ExMesh->m_faces.at(2);
    ExMesh->m_hedges.at(6)->m_heNext = ExMesh->m_hedges.at(7);
    ExMesh->m_hedges.at(6)->m_hePrev = ExMesh->m_hedges.at(8);

    ExMesh->m_hedges.at(7)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(7)->m_heTwin = ExMesh->m_hedges.at(17);
    ExMesh->m_hedges.at(7)->m_face = ExMesh->m_faces.at(2);
    ExMesh->m_hedges.at(7)->m_heNext = ExMesh->m_hedges.at(8);
    ExMesh->m_hedges.at(7)->m_hePrev = ExMesh->m_hedges.at(6);

    ExMesh->m_hedges.at(8)->m_vertex = ExMesh->m_vertices.at(5); 
    ExMesh->m_hedges.at(8)->m_heTwin = ExMesh->m_hedges.at(20);
    ExMesh->m_hedges.at(8)->m_face = ExMesh->m_faces.at(2);
    ExMesh->m_hedges.at(8)->m_heNext = ExMesh->m_hedges.at(6);
    ExMesh->m_hedges.at(8)->m_hePrev = ExMesh->m_hedges.at(7);

    ExMesh->m_hedges.at(9)->m_vertex = ExMesh->m_vertices.at(3); 
    ExMesh->m_hedges.at(9)->m_heTwin = ExMesh->m_hedges.at(21);
    ExMesh->m_hedges.at(9)->m_face = ExMesh->m_faces.at(3);
    ExMesh->m_hedges.at(9)->m_heNext = ExMesh->m_hedges.at(10);
    ExMesh->m_hedges.at(9)->m_hePrev = ExMesh->m_hedges.at(11);

    ExMesh->m_hedges.at(10)->m_vertex = ExMesh->m_vertices.at(6); 
    ExMesh->m_hedges.at(10)->m_heTwin = ExMesh->m_hedges.at(12);
    ExMesh->m_hedges.at(10)->m_face = ExMesh->m_faces.at(3);
    ExMesh->m_hedges.at(10)->m_heNext = ExMesh->m_hedges.at(11);
    ExMesh->m_hedges.at(10)->m_hePrev = ExMesh->m_hedges.at(9);

    ExMesh->m_hedges.at(11)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(11)->m_heTwin = ExMesh->m_hedges.at(1);
    ExMesh->m_hedges.at(11)->m_face = ExMesh->m_faces.at(3);
    ExMesh->m_hedges.at(11)->m_heNext = ExMesh->m_hedges.at(9);
    ExMesh->m_hedges.at(11)->m_hePrev = ExMesh->m_hedges.at(10);

    ExMesh->m_hedges.at(12)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(12)->m_heTwin = ExMesh->m_hedges.at(10);
    ExMesh->m_hedges.at(12)->m_face = ExMesh->m_faces.at(4);
    ExMesh->m_hedges.at(12)->m_heNext = ExMesh->m_hedges.at(13);
    ExMesh->m_hedges.at(12)->m_hePrev = ExMesh->m_hedges.at(14);

    ExMesh->m_hedges.at(13)->m_vertex = ExMesh->m_vertices.at(6); 
    ExMesh->m_hedges.at(13)->m_heTwin = ExMesh->m_hedges.at(22);
    ExMesh->m_hedges.at(13)->m_face = ExMesh->m_faces.at(4);
    ExMesh->m_hedges.at(13)->m_heNext = ExMesh->m_hedges.at(14);
    ExMesh->m_hedges.at(13)->m_hePrev = ExMesh->m_hedges.at(12);

    ExMesh->m_hedges.at(14)->m_vertex = ExMesh->m_vertices.at(7); 
    ExMesh->m_hedges.at(14)->m_heTwin = ExMesh->m_hedges.at(15);
    ExMesh->m_hedges.at(14)->m_face = ExMesh->m_faces.at(4);
    ExMesh->m_hedges.at(14)->m_heNext = ExMesh->m_hedges.at(12);
    ExMesh->m_hedges.at(14)->m_hePrev = ExMesh->m_hedges.at(13);

    ExMesh->m_hedges.at(15)->m_vertex = ExMesh->m_vertices.at(4); 
    ExMesh->m_hedges.at(15)->m_heTwin = ExMesh->m_hedges.at(14);
    ExMesh->m_hedges.at(15)->m_face = ExMesh->m_faces.at(5);
    ExMesh->m_hedges.at(15)->m_heNext = ExMesh->m_hedges.at(16);
    ExMesh->m_hedges.at(15)->m_hePrev = ExMesh->m_hedges.at(17);

    ExMesh->m_hedges.at(16)->m_vertex = ExMesh->m_vertices.at(7); 
    ExMesh->m_hedges.at(16)->m_heTwin = ExMesh->m_hedges.at(23);
    ExMesh->m_hedges.at(16)->m_face = ExMesh->m_faces.at(5);
    ExMesh->m_hedges.at(16)->m_heNext = ExMesh->m_hedges.at(17);
    ExMesh->m_hedges.at(16)->m_hePrev = ExMesh->m_hedges.at(15);

    ExMesh->m_hedges.at(17)->m_vertex = ExMesh->m_vertices.at(5); 
    ExMesh->m_hedges.at(17)->m_heTwin = ExMesh->m_hedges.at(7);
    ExMesh->m_hedges.at(17)->m_face = ExMesh->m_faces.at(5);
    ExMesh->m_hedges.at(17)->m_heNext = ExMesh->m_hedges.at(15);
    ExMesh->m_hedges.at(17)->m_hePrev = ExMesh->m_hedges.at(16);

    ExMesh->m_hedges.at(18)->m_vertex = ExMesh->m_vertices.at(3); 
    ExMesh->m_hedges.at(18)->m_heTwin = ExMesh->m_hedges.at(0);
    ExMesh->m_hedges.at(18)->m_face = nullptr;
    ExMesh->m_hedges.at(18)->m_heNext = ExMesh->m_hedges.at(19);
    ExMesh->m_hedges.at(18)->m_hePrev = ExMesh->m_hedges.at(21);

    ExMesh->m_hedges.at(19)->m_vertex = ExMesh->m_vertices.at(1); 
    ExMesh->m_hedges.at(19)->m_heTwin = ExMesh->m_hedges.at(5);
    ExMesh->m_hedges.at(19)->m_face = nullptr;
    ExMesh->m_hedges.at(19)->m_heNext = ExMesh->m_hedges.at(20);
    ExMesh->m_hedges.at(19)->m_hePrev = ExMesh->m_hedges.at(18);

    ExMesh->m_hedges.at(20)->m_vertex = ExMesh->m_vertices.at(2); 
    ExMesh->m_hedges.at(20)->m_heTwin = ExMesh->m_hedges.at(8);
    ExMesh->m_hedges.at(20)->m_face = nullptr;
    ExMesh->m_hedges.at(20)->m_heNext = ExMesh->m_hedges.at(23);
    ExMesh->m_hedges.at(20)->m_hePrev = ExMesh->m_hedges.at(19);

    ExMesh->m_hedges.at(21)->m_vertex = ExMesh->m_vertices.at(6); 
    ExMesh->m_hedges.at(21)->m_heTwin = ExMesh->m_hedges.at(9);
    ExMesh->m_hedges.at(21)->m_face = nullptr;
    ExMesh->m_hedges.at(21)->m_heNext = ExMesh->m_hedges.at(18);
    ExMesh->m_hedges.at(21)->m_hePrev = ExMesh->m_hedges.at(22);

    ExMesh->m_hedges.at(22)->m_vertex = ExMesh->m_vertices.at(7); 
    ExMesh->m_hedges.at(22)->m_heTwin = ExMesh->m_hedges.at(13);
    ExMesh->m_hedges.at(22)->m_face = nullptr;
    ExMesh->m_hedges.at(22)->m_heNext = ExMesh->m_hedges.at(21);
    ExMesh->m_hedges.at(22)->m_hePrev = ExMesh->m_hedges.at(23);

    ExMesh->m_hedges.at(23)->m_vertex = ExMesh->m_vertices.at(5); 
    ExMesh->m_hedges.at(23)->m_heTwin = ExMesh->m_hedges.at(16);
    ExMesh->m_hedges.at(23)->m_face = nullptr;
    ExMesh->m_hedges.at(23)->m_heNext = ExMesh->m_hedges.at(22);
    ExMesh->m_hedges.at(23)->m_hePrev = ExMesh->m_hedges.at(20);


}


//----------------------------------------------------------------------------------
void initOpenGl()
{
    initMesh();

    // light
    glClearColor(.5, .5, 0.5, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat l_pos[] = {3., 3.5, 3.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, l_pos);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_pos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l_pos);

    // glDepthFunc(GL_LESS);
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)200 / (GLfloat)200, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glScalef(.7,.7,.7);
    gluLookAt(0., 0., 4., 0., 0., 0., 0., 1., 0.);
    // glTranslatef(0.0,0.0,-5.0);
}

//------------------------------------------------------
void displayHalfEdge(void)
{
    for(int i = 0;i < NBHALFEDGES; i++){
        HalfEdge* he = ExMesh->m_hedges[i];
        Vertex* v1 = he->m_vertex;
        Vertex* v2 = he->m_heNext->m_vertex;

        glBegin(GL_LINES);
        glVertex3f(v1->m_x, v1->m_y, v1->m_z);
        glVertex3f(v2->m_x, v2->m_y, v2->m_z);
        glEnd();
    }
}

int main(int argc, char **argv)
{
    /* glut init and window creation */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);
    glutCreateWindow("ifs");

    /* OpenGL init */

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    initOpenGl();

    /* Enter glut's main loop */
    glutMainLoop();
    return 0;
}

//------------------------------------------------------
void display_basis(void)
{
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0., 0.);
    glVertex2f(1., 0.);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0., 0.);
    glVertex2f(0., 1.);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 0., 1.);
    glEnd();
}

//------------------------------------------------------
void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    /* clearing background */
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1., 0., 0.);
    glRotatef(cameraAngleY, 0., 1., 0.);
    //--------------------------------
    display_basis();
    //--------------------------------
    displayHalfEdge();
    //--------------------------------

    glPopMatrix();
    /* force result display */
    glFlush();
}

//------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
    case '+':
        glutPostRedisplay();
        break;
    case '-':
        glutPostRedisplay();
        break;
    case 'f': /* wire mode display */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glutPostRedisplay();
        break;
    case 'p': /* filled mode display */
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glutPostRedisplay();
        break;
    case 's': /* only vertices display mode */
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glutPostRedisplay();
        break;
    case 'q': /* quit program */
        exit(0);
    }
}

void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}


void mouseMotion(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}