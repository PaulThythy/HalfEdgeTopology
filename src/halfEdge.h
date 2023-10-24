#pragma once

#include "vertex.h"
#include "face.h"

#include <string>
#include <iostream>
//#include <GL/glut.h>

struct Face;
struct Vertex;

using namespace std;

struct HalfEdge {
    Vertex* m_vertex;
    HalfEdge* m_heNext;
    HalfEdge* m_hePrev;
    HalfEdge* m_heTwin;
    Face* m_face;
    string m_name;

    inline HalfEdge() {}
    inline HalfEdge(string name, Vertex* vertex,
                    HalfEdge* heNext, HalfEdge* hePrev,
                    HalfEdge* heTwin, Face* face):

                    m_name(name), m_vertex(vertex), 
                    m_heNext(heNext), m_hePrev(hePrev),
                    m_heTwin(heTwin), m_face(face) {}
    inline ~HalfEdge() {}
};