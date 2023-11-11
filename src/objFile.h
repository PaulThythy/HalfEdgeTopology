#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct
{
    int m_vIndex; // done
    int m_heTwinIndex;
    int m_faceIndex; // done
    int m_heNextIndex;
    int m_hePrevIndex;
    string m_heName; // done
} HeStruct;

typedef struct
{
    double m_x, m_y, m_z; // done
    int m_heIndex;        // done
    string m_vName;       // done
} VStruct;

typedef struct
{
    vector<int> m_fVertices; // done
    int m_oneHeIndex;        // done
    string m_fName;          // done
} FStruct;

struct OBJFile
{
    vector<HeStruct *> m_tabHalfEdges;
    vector<FStruct *> m_tabFaces;
    vector<VStruct *> m_tabVertices;
    string m_filePath;

    inline OBJFile(string filePath) : m_filePath(filePath) {}
    inline ~OBJFile() {}

    inline void readData()
    {
        string line;
        int verticesCounter = 0;
        int facesCounter = 0;

        // adding an empty vertex to the index 0
        VStruct *nullV = nullptr;
        m_tabVertices.push_back(nullV);

        ifstream myfile(this->m_filePath);

        if (myfile.is_open())
        {
            while (getline(myfile, line))
            {
                char carLine;

                istringstream istr(line);
                istr >> carLine;

                if (carLine == 'v')
                {
                    verticesCounter++;

                    VStruct *vertex = new VStruct();
                    istr >> vertex->m_x >> vertex->m_y >> vertex->m_z;
                    vertex->m_vName = "v" + to_string(verticesCounter);
                    vertex->m_heIndex = -1; // to be update later
                    m_tabVertices.push_back(vertex);
                }

                if (carLine == 'f')
                {
                    FStruct *face = new FStruct();
                    face->m_fName = "f" + to_string(facesCounter);
                    face->m_oneHeIndex = -1; // to be update later
                    m_tabFaces.push_back(face);

                    int vertexIndex;
                    while (istr >> vertexIndex)
                    {
                        face->m_fVertices.push_back(vertexIndex);
                    }
                    facesCounter++;
                }
            }
            myfile.close();
        }
        else
            cout << "Unable to open file" << endl;

        this->constructTopology();
    }

    inline void constructTopology()
    {
        int numHe = 0;
        int numFaces = 0;
        // Iterate over faces to create half-edges and update vertex information
        for (FStruct *face : m_tabFaces)
        {
            vector<int> &vertices = face->m_fVertices;
            int numVertices = static_cast<int>(vertices.size());

            // Iterate over each pair of consecutive vertices in the face
            for (int i = 0; i < numVertices; i++)
            {
                int startVertexIndex = vertices[i];
                int endVertexIndex = vertices[(i + 1) % numVertices];

                HeStruct *halfEdge = new HeStruct();
                halfEdge->m_vIndex = startVertexIndex;
                halfEdge->m_heName = "e" + to_string(numHe);
                halfEdge->m_faceIndex = numFaces;
                // to be update later
                halfEdge->m_heNextIndex = -1;
                halfEdge->m_hePrevIndex = -1;
                halfEdge->m_heTwinIndex = -1;

                // the he index of a face is always the first he of a face
                if (i == 0)
                    face->m_oneHeIndex = numHe;

                // initialize the he of the start vertex
                // the value is the first he that starts from this vertex
                // if statement to not overwrite the value of the heIndex
                if (this->m_tabVertices.at(startVertexIndex)->m_heIndex == -1)
                {
                    this->m_tabVertices.at(startVertexIndex)->m_heIndex = numHe;
                }

                // Add the half-edge to the vector of half-edges
                m_tabHalfEdges.push_back(halfEdge);

                numHe++;
            }
            numFaces++;
        }

        // Initialize next and previous by iterating over faces
        for (FStruct *face : m_tabFaces)
        {
            int numVertices = static_cast<int>(face->m_fVertices.size());

            // Iterate over each triplets of consecutive vertices in the face
            for (int i = 0; i < numVertices; i++)
            {
                int currentVertexIndex = face->m_fVertices[i];
                int nextVertexIndex = face->m_fVertices[(i + 1) % numVertices];
                int prevVertexIndex = face->m_fVertices[(i - 1 + numVertices) % numVertices];

                
            }
        }

        this->printVertices();
        this->printFaces();
        this->printHalfEdges();
    }

    inline void printHalfEdge(const HeStruct *he)
    {
        if (he)
        {
            cout << "--------" << he->m_heName << "--------" << endl;
            cout << "vertexIndex: " << he->m_vIndex << endl;
            cout << "faceIndex: " << he->m_faceIndex << endl;
            cout << "nextHeIndex: " << he->m_heTwinIndex << endl;
            cout << "previousHeIndex: " << he->m_hePrevIndex << endl;
            cout << "twinHeIndex: " << he->m_heTwinIndex << endl;
            cout << "--------------------------------------" << endl;
            cout << endl;
        }
        else
        {
            cout << "HalfEdge pointer is nullptr" << endl;
        }
    }

    inline void printFace(const FStruct *face)
    {
        if (face)
        {

            cout << "--------" << face->m_fName << "--------" << endl;
            cout << "OneHeIndex: " << face->m_oneHeIndex << endl;
            cout << "Vertices: " << endl;
            for (int vertexIndex : face->m_fVertices)
            {
                cout << vertexIndex << " ";
            }
            cout << endl;
            cout << "--------------------------------------" << endl;
            cout << endl;
        }
        else
        {
            cout << "Face pointer is nullptr" << endl;
        }
    }

    inline void printVertex(const VStruct *vertex)
    {
        if (vertex)
        {

            cout << "--------" << vertex->m_vName << "--------" << endl;
            cout << "coords: " << vertex->m_x << " " << vertex->m_y << " " << vertex->m_z << endl;
            cout << "heIndex: " << vertex->m_heIndex << endl;
            cout << "--------------------------------------" << endl;
            cout << endl;
        }
        else
        {
            cout << "Vertex pointer is nullptr" << endl;
        }
    }

    inline void printHalfEdges()
    {
        for (HeStruct *he : this->m_tabHalfEdges)
        {
            printHalfEdge(he);
        }
    }

    inline void printFaces()
    {
        for (FStruct *face : this->m_tabFaces)
        {
            printFace(face);
        }
    }

    inline void printVertices()
    {
        for (VStruct *vertex : this->m_tabVertices)
        {
            printVertex(vertex);
        }
    }
};