#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct
{
    int m_vIndex ;                  //done
    int m_heTwinIndex ;
    int m_faceIndex ;               //done
    int m_heNextIndex ;
    int m_hePrevIndex ;
    string m_heName ;               //done
} HeStruct;

typedef struct
{
    double m_x,m_y,m_z ;            //done
    int m_heIndex ;                 //done        
    string m_vName ;                //done
} VStruct;

typedef struct
{
    vector<int> m_fVertices ;       //done
    int m_oneHeIndex ;              //done
    string m_fName ;                //done
} FStruct;

struct OBJFile{
    vector<HeStruct*> m_tabHalfEdges;
    vector<FStruct*> m_tabFaces;
    vector<VStruct*> m_tabVertices;
    string m_filePath;

    inline OBJFile(string filePath): m_filePath(filePath) {}
    inline ~OBJFile() {}

    inline void readData() {
        string line;
        int verticesCounter = 0;
        int facesCounter = 0;
        int heCounter = 0;

        //adding an empty vertex to the index 0
        VStruct* nullV = nullptr;
        m_tabVertices.push_back(nullV);

        ifstream myfile(this->m_filePath);

        if(myfile.is_open()){
            while(getline(myfile, line)){
                char carLine;
                
                istringstream istr(line);
                istr >> carLine;

                if(carLine == 'v'){
                    verticesCounter++;

                    VStruct* vertex = new VStruct(); 
                    istr >> vertex->m_x >> vertex->m_y >> vertex->m_z; 
                    vertex->m_vName = "v" + to_string(verticesCounter);
                    m_tabVertices.push_back(vertex);
                }
                
                if(carLine == 'f'){
                    FStruct* face = new FStruct();
                    face->m_fName = "f" + to_string(facesCounter);
                    m_tabFaces.push_back(face);

                    int vertexIndex;
                    while(istr >> vertexIndex){
                        face->m_fVertices.push_back(vertexIndex);
                        
                        //each time we find a new vertex in a face, we create an he
                        HeStruct* he = new HeStruct();
                        he->m_heName = "e" + to_string(heCounter);
                        he->m_heTwinIndex = -1;
                        he->m_heNextIndex = -1;
                        he->m_hePrevIndex = -1;
                        m_tabHalfEdges.push_back(he);

                        he->m_faceIndex = facesCounter;
                        he->m_vIndex = vertexIndex;

                        face->m_oneHeIndex = heCounter;

                        m_tabVertices.at(vertexIndex)->m_heIndex = heCounter;

                        heCounter++;
                    }
                    facesCounter++;
                }
            }
            myfile.close();
        }
        else cout << "Unable to open file" << endl;

        //this->printVertices();
        //this->printFaces();
        //this->printHalfEdges();

        this->constructTopology();
    }

    inline void constructTopology() {
        // Initialize next and previous he
        for (FStruct* face : m_tabFaces) {
            vector<int>& vertices = face->m_fVertices;
            int numVertices = static_cast<int>(vertices.size());

            for (int i = 0; i < numVertices; i++) {
                int currentVertexIndex = vertices[i];
                int nextVertexIndex = vertices[(i + 1) % numVertices];

                cout << "-----------------" << endl;
                cout << currentVertexIndex << endl;
                cout << nextVertexIndex << endl;
                cout << "-----------------" << endl;

                int currentHeIndex = m_tabVertices.at(currentVertexIndex)->m_heIndex;
                int nextHeIndex = m_tabVertices.at(nextVertexIndex)->m_heIndex;

                cout << "-----------------" << endl;
                cout << currentHeIndex << endl;
                cout << nextHeIndex << endl;
                cout << "-----------------" << endl;

                HeStruct* currentHe = m_tabHalfEdges.at(currentHeIndex);
                HeStruct* nextHe = m_tabHalfEdges.at(nextHeIndex);

                currentHe->m_heNextIndex = nextHeIndex;
                nextHe->m_hePrevIndex = currentHeIndex;
            }
        }

        // Initialize twin he
        /*for (FStruct* face : m_tabFaces) {
            vector<int>& vertices = face->m_fVertices;
            int numVertices = static_cast<int>(vertices.size());

            for (int i = 0; i < numVertices; i++) {
                int currentVertexIndex = vertices[i];
                int nextVertexIndex = vertices[(i + 1) % numVertices];

                int currentHeIndex = m_tabVertices.at(currentVertexIndex)->m_heIndex;
                int nextHeIndex = m_tabVertices.at(nextVertexIndex)->m_heIndex;

                HeStruct* currentHe = m_tabHalfEdges.at(currentHeIndex);
                HeStruct* nextHe = m_tabHalfEdges.at(nextHeIndex);

                if (currentHe->m_heTwinIndex == -1) {
                    for (int j = 0; j < numVertices; j++) {
                        if (i != j) {
                            int potentialTwinIndex = m_tabVertices.at(vertices[j])->m_heIndex;
                            HeStruct* potentialTwinHe = m_tabHalfEdges.at(potentialTwinIndex);

                            if (potentialTwinHe->m_vIndex == nextVertexIndex) {
                                currentHe->m_heTwinIndex = potentialTwinIndex;
                                potentialTwinHe->m_heTwinIndex = currentHeIndex;
                                break;
                            }
                        }
                    }
                }
            }
        }*/

        this->printVertices();
        this->printFaces();
        this->printHalfEdges();
    }

    

    inline void printHalfEdge(const HeStruct* he){
        if(he){
            cout << "--------" << he->m_heName << "--------" << endl;
            cout << "vertexIndex: "     << he->m_vIndex         << endl;
            cout << "faceIndex: "       << he->m_faceIndex      << endl;
            cout << "nextHeIndex: "     << he->m_heTwinIndex    << endl;
            cout << "previousHeIndex: " << he->m_hePrevIndex    << endl;
            cout << "twinHeIndex: "     << he->m_heTwinIndex    << endl;
            cout << "--------------------------------------"    << endl;
            cout << endl;

        }else{
            cout << "HalfEdge pointer is nullptr" << endl;
        }
    }

    inline void printFace(const FStruct* face){
        if(face){

            cout << "--------" << face->m_fName << "--------"   << endl;
            cout << "OneHeIndex: "     << face->m_oneHeIndex       << endl;
            cout << "Vertices: "                                << endl;
            for(int vertexIndex : face->m_fVertices){
                cout << vertexIndex << " "; 
            }
            cout << endl;
            cout << "--------------------------------------"    << endl;
            cout << endl;

        }else{
            cout << "Face pointer is nullptr" << endl;
        }
    }

    inline void printVertex(const VStruct* vertex){
        if(vertex){

            cout << "--------" << vertex->m_vName << "--------" << endl;
            cout << "coords: " << vertex->m_x << " " << vertex->m_y << " " << vertex->m_z << endl;
            cout << "heIndex: "<< vertex->m_heIndex             << endl;
            cout << "--------------------------------------"    << endl;
            cout << endl;

        }else{
            cout << "Vertex pointer is nullptr" << endl;
        }
    }

    inline void printHalfEdges(){
        for(HeStruct* he : this->m_tabHalfEdges){
            printHalfEdge(he);
        }
    }

    inline void printFaces(){
        for(FStruct* face : this->m_tabFaces){
            printFace(face);
        }
    }

    inline void printVertices(){
        for(VStruct* vertex : this->m_tabVertices){
            printVertex(vertex);
        }
    }
};