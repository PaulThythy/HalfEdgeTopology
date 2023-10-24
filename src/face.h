#pragma once

#include "halfEdge.h"

#include <string>
#include <iostream>
//include <GL/glut.h>

struct HalfEdge;

using namespace std;

struct Face {
    HalfEdge* m_oneHe;
    string m_name;

    inline Face(string name, HalfEdge* oneHe): m_name(name), m_oneHe(oneHe) {}
    inline Face() {}
    inline ~Face() {}
};