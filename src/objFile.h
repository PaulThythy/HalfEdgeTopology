#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct
{
    int m_vIndex ;
    int m_heTwinIndex ;
    int m_faceIndex ;
    int m_heNextIndex ;
    int m_hePrevIndex ;
    string m_heName;
} HeStruct;

typedef struct
{
    double m_x,m_y,m_z ;
    int m_heIndex ;
    string m_vName ;
} VStruct;

typedef struct
{
    vector<int> m_fVertices;
    int m_heIndex ;
    string m_fName;
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

                    this->printVertex(vertex); 
                }
                
                if(carLine == 'f'){
                    FStruct* face = new FStruct();
                    int vertexIndex;
                    while(istr >> vertexIndex){
                        face->m_fVertices.push_back(vertexIndex);
                        
                        //each time we find a new vertex in a face, we create an he
                        HeStruct* he = new HeStruct();
                        //he->m_faceIndex = face->m_fVertices.at(facesCounter);
                        he->m_vIndex = vertexIndex;
                        m_tabHalfEdges.push_back(he);

                        this->printHe(he);
                    }
                    m_tabFaces.push_back(face);

                    /*cout << "f" << facesCounter << " : ";
                    for(int i = 0; i < face->m_fVertices.size(); i++){
                        cout << face->m_fVertices.at(i) << " ";
                    }
                    cout << endl;*/
                    facesCounter++;
                }
            }
            myfile.close();
        }
        else cout << "Unable to open file" << endl;

        this->constructTopology();
    }

    inline void constructTopology(){
        /*for(size_t i = 0; i < m_tabFaces.size(); i++){

            //browse all vertices of a face
            for(size_t j = 1; j <= m_tabFaces[i]->m_fVertices.size(); j++){
                
                int vertexIndex = m_tabFaces[i]->m_fVertices[j];

                HeStruct* he = new HeStruct();

                he1->m_vIndex = indexV1;

                //associate face index to the he
                he1->m_faceIndex = i;
                he2->m_faceIndex = i;

                //associate he next and prev to the he
                he1->m_heNextIndex = 
            }
        }*/
    }

    inline void printHe(const HeStruct* he){
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
            cout << "OneHeIndex: "     << face->m_heIndex       << endl;
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
};