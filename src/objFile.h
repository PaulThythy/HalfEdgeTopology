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
} HeStruct;

typedef struct
{
    double m_x,m_y,m_z ;
    int m_heIndex ;
} VStruct;

typedef struct
{
    vector<int> m_fVertices;
    int m_heIndex ;
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
        int nbVertices = 0;
        int nbFaces = 0;

        VStruct* nullV = nullptr;
        m_tabVertices.push_back(nullV);

        ifstream myfile(this->m_filePath);
        if(myfile.is_open()){
            while(getline(myfile, line)){
                char carLine;
                
                istringstream istr(line);
                istr >> carLine;

                if(carLine == 'v'){
                    nbVertices++;

                    VStruct* vertex = new VStruct(); 
                    istr >> vertex->m_x >> vertex->m_y >> vertex->m_z; 
                    m_tabVertices.push_back(vertex);

                    //cout << "v" << nbVertices << " : x=" << vertex->m_x << ", y=" << vertex->m_y << ", z=" << vertex->m_z << endl; 
                }

                if(carLine == 'f'){
                    FStruct* face = new FStruct();
                    int vertexIndex;
                    while(istr >> vertexIndex){
                        face->m_fVertices.push_back(vertexIndex);
                    }
                    m_tabFaces.push_back(face);

                    /*cout << "f" << nbFaces << " : ";
                    for(int i = 0; i < face->m_fVertices.size(); i++){
                        cout << face->m_fVertices.at(i) << " ";
                    }
                    cout << endl;*/
                    nbFaces++;
                }
            }
            myfile.close();
        }
        else cout << "Unable to open file" << endl;

        this->constructTopology();
    }

    inline void constructTopology(){
        for(size_t i = 0; i < m_tabFaces.size(); i++){

            //browse all vertices of a face
            for(size_t j = 1; j <= m_tabFaces[i]->m_fVertices.size(); j++){
                //create the two vertices of an he
                int indexV1 = m_tabFaces[i]->m_fVertices[j];
                int indexV2 = m_tabFaces[i]->m_fVertices[(j+1) % m_tabFaces[i]->m_fVertices.size()];

                HeStruct* he1 = new HeStruct();
                HeStruct* he2 = new HeStruct();

                //associate starting vertices to their he
                he1->m_vIndex = indexV1;
                he2->m_vIndex = indexV2;

                //associate face index to the he
                he1->m_faceIndex = i;
                he2->m_faceIndex = i;

                
            }
        }
    }
};