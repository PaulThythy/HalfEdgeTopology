#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <GL/glut.h>

using namespace std;

typedef struct
{
    int m_vIndex ;
    int m_heTwinIndex ;
    int m_faceIndex ; 
    int m_heNextIndex ;
    int m_hePrevIndex ;
    string m_heName ;
} HeStruct;

typedef struct
{
    double m_x, m_y, m_z ; 
    int m_heIndex ;        
    string m_vName ;      
} VStruct;

typedef struct
{
    vector<int> m_fVertices ;
    int m_oneHeIndex ;
    string m_fName ;
} FStruct;

struct OBJFile
{
    vector<HeStruct*> m_tabHalfEdges;
    vector<FStruct*> m_tabFaces;
    vector<VStruct*> m_tabVertices;
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

                    VStruct* vertex = new VStruct();
                    istr >> vertex->m_x >> vertex->m_y >> vertex->m_z;
                    vertex->m_vName = "v" + to_string(verticesCounter);
                    vertex->m_heIndex = -1; // to be update later
                    m_tabVertices.push_back(vertex);
                }

                if (carLine == 'f')
                {
                    FStruct* face = new FStruct();
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
        for (FStruct* face : m_tabFaces)
        {
            vector<int> &vertices = face->m_fVertices;
            int numVertices = static_cast<int>(vertices.size());

            // Iterate over each pair of consecutive vertices in the face
            for (int i = 0; i < numVertices; i++)
            {
                int startVertexIndex = vertices[i];
                int endVertexIndex = vertices[(i + 1) % numVertices];

                HeStruct* halfEdge = new HeStruct();
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

        // Set next and previous by iterating over halfedges of a face
        for(int i = 0; i < m_tabFaces.size(); i++){
            int numberHalfEdges = static_cast<int>(getHalfEdgesFromFace(i).size());

            for(int j = 0; j < numberHalfEdges; j++){
                int currentHeIndex  = getHalfEdgesFromFace(i).at(j);
                int nextHeIndex     = getHalfEdgesFromFace(i).at((j + 1) % numberHalfEdges);
                int prevHeIndex     = getHalfEdgesFromFace(i).at((j - 1 + numberHalfEdges) % numberHalfEdges);

                HeStruct* currentHe         = m_tabHalfEdges.at(currentHeIndex);
                currentHe->m_heNextIndex    = nextHeIndex;
                currentHe->m_hePrevIndex    = prevHeIndex;
            }
        }

        // Set twins by iterating over halfedges
        for(int i = 0; i < m_tabHalfEdges.size(); i++){
            HeStruct* currentHe = m_tabHalfEdges.at(i);

            int startVertexIndex = currentHe->m_vIndex;
            int endVertexIndex   = m_tabHalfEdges.at(currentHe->m_heNextIndex)->m_vIndex; 
        
            for(int j = 0; j < m_tabHalfEdges.size(); j++){
                HeStruct* potentialTwin = m_tabHalfEdges.at(j);

                if( potentialTwin->m_vIndex == endVertexIndex && 
                    m_tabHalfEdges.at(potentialTwin->m_heNextIndex)->m_vIndex == startVertexIndex &&
                    potentialTwin->m_heTwinIndex == -1){

                    currentHe->m_heTwinIndex = j;
                    potentialTwin->m_heTwinIndex = i;
                }
            }
        }

        int numGhostHe = 0;
        int numHalfEdges = static_cast<int>(m_tabHalfEdges.size());
        
        // Set ghost halfedges by iterating over halfedges
        for(int i = 0; i < numHalfEdges; i++){
            HeStruct* currentHe = m_tabHalfEdges.at(i);

            // Check if the he doesn't have a twin (i.e., it's a boundary edge)
            if(currentHe->m_heTwinIndex == -1){
                HeStruct* ghostHe = new HeStruct();

                ghostHe->m_heName       = "e" + to_string(numHalfEdges + numGhostHe);
                ghostHe->m_faceIndex    = -1;
                ghostHe->m_vIndex       = m_tabHalfEdges.at(currentHe->m_heNextIndex)->m_vIndex;
                // to be update later
                ghostHe->m_heNextIndex  = -1;
                ghostHe->m_hePrevIndex  = -1;
                ghostHe->m_heTwinIndex  = m_tabHalfEdges.at(currentHe->m_heNextIndex)->m_hePrevIndex;
                m_tabHalfEdges.at(m_tabHalfEdges.at(currentHe->m_heNextIndex)->m_hePrevIndex)->m_heTwinIndex = numHalfEdges + numGhostHe; 

                m_tabHalfEdges.push_back(ghostHe);
                numGhostHe++;
            }
        }

        // Set next and prev for ghost halfedges
        // I wanted to use the function getHalfEdgesFromFace(-1) to loop over ghost he, 
        // but I think it will be a problem for indexes
        for(HeStruct* currentHe : m_tabHalfEdges){
            if(currentHe->m_faceIndex == -1){
                int startVertexIndex    = currentHe->m_vIndex;
                int endVertexIndex      = m_tabHalfEdges.at(currentHe->m_heTwinIndex)->m_vIndex;

                for(int i = 0; i < numHalfEdges; i++){
                    HeStruct* potentialNext = m_tabHalfEdges.at(i);

                    if(potentialNext->m_faceIndex == -1 && currentHe != potentialNext){
                        
                        if( potentialNext->m_vIndex == endVertexIndex &&
                            currentHe->m_heNextIndex == -1){
                            
                            currentHe->m_heNextIndex = i;
                        }
                    }
                }

                for(int i = 0; i < numHalfEdges; i++){
                    HeStruct* potentialPrev = m_tabHalfEdges.at(i);

                    if(potentialPrev->m_faceIndex == -1 && currentHe != potentialPrev){

                        int endVertexIndexPotentialPrev = m_tabHalfEdges.at(potentialPrev->m_heTwinIndex)->m_vIndex;

                        if( endVertexIndexPotentialPrev == startVertexIndex && 
                            currentHe->m_hePrevIndex == -1){
                            
                            currentHe->m_hePrevIndex = i;
                        }
                    }
                }
            }
        }

        this->printVertices();
        this->printFaces();
        this->printHalfEdges();
    }

    inline void displayMeshEdges(){
        
    }

    inline void displayHalfEdges(){
        
    }

    inline vector<int> getHalfEdgesFromFace(int faceIndex)
    {
        vector<int> listHe;
        for(int i = 0; i < m_tabHalfEdges.size(); i++){
            if(m_tabHalfEdges.at(i)->m_faceIndex == faceIndex){
                listHe.push_back(i);
            }
        }
        return listHe;
    }

    inline void printHalfEdge(const HeStruct* he)
    {
        if (he)
        {
            cout << "--------" << he->m_heName << "--------" << endl;
            cout << "vertexIndex: " << he->m_vIndex << endl;
            cout << "faceIndex: " << he->m_faceIndex << endl;
            cout << "nextHeIndex: " << he->m_heNextIndex << endl;
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

    inline void printFace(const FStruct* face)
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

    inline void printVertex(const VStruct* vertex)
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