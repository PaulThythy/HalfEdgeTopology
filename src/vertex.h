#pragma once

#include "halfEdge.h"

#include <string>
#include <iostream>
//#include <GL/glut.h>

struct HalfEdge;

using namespace std;

struct Vertex {
    double m_x, m_y, m_z;
    HalfEdge* m_oneHe;
    string m_name;

    inline Vertex() {}
    inline Vertex(double x, double y, double z): 
                    m_x(x), m_y(y), m_z(z) {}
    inline Vertex(string name, double x, double y, double z, HalfEdge* oneHe):
                    m_name(name), m_x(x), m_y(y), m_z(z), m_oneHe(oneHe) {}
    
    inline ~Vertex() {}
};