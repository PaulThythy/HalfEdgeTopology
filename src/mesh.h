#pragma once

#include "vertex.h"
#include "halfEdge.h"
#include "face.h"

#include <string>
#include <iostream>
#include <vector>
//#include <GL/glut.h>

using namespace std;

struct Mesh {
    vector <Vertex*> m_vertices;
    vector <HalfEdge*> m_hedges;
    vector <Face*> m_faces;

    inline Mesh() {}
    inline ~Mesh() {}
};