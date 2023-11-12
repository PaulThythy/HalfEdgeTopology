#include "halfEdge.h"
#include "vertex.h"
#include "face.h"
#include "mesh.h"
#include "objFile.h"

#include <iostream>
#include <algorithm>
#include <array>
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

void initHalfEdge(int, int, int, int, int, int);

#define NBFACES 6
#define NBVERTICES 7
#define NBHALFEDGES 24
Mesh* ExMesh;
OBJFile* objFile;


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
    objFile = new OBJFile("ressources/test.obj");
    objFile->readData();

    // create vertices (vertices numbered from 1 to NBVERTICES !)
    Vertex* nullV = nullptr;
    ExMesh->m_vertices.push_back(nullV);

    for(int i = 0; i <= NBVERTICES; i++){  
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

    initHalfEdge(0, 1, 18, 0, 1, 2);
    initHalfEdge(1, 3, 11, 0, 2, 0);
    initHalfEdge(2, 4, 3, 0, 0, 1);
    initHalfEdge(3, 1, 2, 1, 4, 5);
    initHalfEdge(4, 4, 6, 1, 5, 3);
    initHalfEdge(5, 2, 19, 1, 3, 4);
    initHalfEdge(6, 2, 4, 2, 7, 8);
    initHalfEdge(7, 4, 17, 2, 8, 6);
    initHalfEdge(8, 5, 20, 2, 6, 7);
    initHalfEdge(9, 3, 21, 3, 10, 11);
    initHalfEdge(10, 6, 12, 3, 11, 9);
    initHalfEdge(11, 4, 1, 3, 9, 10);
    initHalfEdge(12, 4, 10, 4, 13, 14);
    initHalfEdge(13, 6, 22, 4, 14, 12);
    initHalfEdge(14, 7, 15, 4, 12, 13);
    initHalfEdge(15, 4, 14, 5, 16, 17);
    initHalfEdge(16, 7, 23, 5, 17, 15);
    initHalfEdge(17, 5, 7, 5, 15, 16);
    initHalfEdge(18, 3, 0, -1, 19, 21);
    initHalfEdge(19, 1, 5, -1, 20, 18);
    initHalfEdge(20, 2, 8, -1, 23, 19);
    initHalfEdge(21, 6, 9, -1, 18, 22);
    initHalfEdge(22, 7, 13, -1, 21, 23);
    initHalfEdge(23, 5, 16, -1, 22, 20);

}

//----------------------------------------------------------------------------------
void initHalfEdge(int heIndex, int vertexIndex, int heTwinIndex, int faceIndex, int heNextIndex, int hePrevIndex){
    //init he with the vertex
    ExMesh->m_hedges.at(heIndex)->m_vertex = ExMesh->m_vertices.at(vertexIndex);
    //init vertex with the he 
    ExMesh->m_vertices.at(vertexIndex)->m_oneHe = ExMesh->m_hedges.at(heIndex);

    //no need to init the twin because all he will be visited
    ExMesh->m_hedges.at(heIndex)->m_heTwin = ExMesh->m_hedges.at(heTwinIndex);


    if(faceIndex == -1){
        ExMesh->m_hedges.at(heIndex)->m_face = nullptr;
    }
    else{
        //init face with face index
        ExMesh->m_hedges.at(heIndex)->m_face = ExMesh->m_faces.at(faceIndex);
        //init face with the he
        ExMesh->m_faces.at(faceIndex)->m_oneHe = ExMesh->m_hedges.at(heIndex);
    } 

    //no need to init the next and prev because all he will be visited
    ExMesh->m_hedges.at(heIndex)->m_heNext = ExMesh->m_hedges.at(heNextIndex);
    ExMesh->m_hedges.at(heIndex)->m_hePrev = ExMesh->m_hedges.at(hePrevIndex);


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
    std::vector<HalfEdge*> visitedHes;

    for(const Face* face : ExMesh->m_faces){
        if(face != nullptr){
            HalfEdge* startHe = face->m_oneHe;
            HalfEdge* currentHe = face->m_oneHe;

            do{
                //if the twin of the current he has not been visited, we draw the current he
                if( std::find(visitedHes.begin(), visitedHes.end(), currentHe->m_heTwin) == visitedHes.end() ){
                    
                    //add current he into the list
                    visitedHes.push_back(currentHe);

                    glBegin(GL_LINES);
                    glVertex3d(currentHe->m_vertex->m_x, currentHe->m_vertex->m_y, currentHe->m_vertex->m_z);
                    glVertex3d(currentHe->m_heNext->m_vertex->m_x, currentHe->m_heNext->m_vertex->m_y, currentHe->m_heNext->m_vertex->m_z);
                    glEnd();
                }

                currentHe = currentHe->m_heNext;
            }while(currentHe!=startHe);                 //while current he different from start he
        }
        
    }
}

//------------------------------------------------------
void displayMeshEdges(void){
    for(int i = 0; i < NBHALFEDGES; i++){
        HalfEdge* he = ExMesh->m_hedges[i];
        if(he->m_face == nullptr){
            Vertex* v1 = he->m_vertex;
            Vertex* v2 = he->m_heNext->m_vertex;

            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(v1->m_x, v1->m_y, v1->m_z);
            glVertex3f(v2->m_x, v2->m_y, v2->m_z);
            glEnd();
        }
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
    //displayHalfEdge();
    //--------------------------------
    //displayMeshEdges();
    //--------------------------------
    //objFile->displayMeshEdges();
    //--------------------------------
    //objFile->displayHalfEdges();

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