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
    int m_faceIndex ;
    int m_heNextIndex ;
    int m_hePrevIndex ;
    string m_heName;                //done
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
    int m_oneHeIndex ;                 
    string m_fName;                 //done
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
                        m_tabHalfEdges.push_back(he);

                        he->m_faceIndex = facesCounter;
                        he->m_vIndex = vertexIndex;

                        m_tabVertices.at(vertexIndex)->m_heIndex = heCounter;

                        heCounter++;
                    }
                    facesCounter++;
                }
            }
            myfile.close();
        }
        else cout << "Unable to open file" << endl;

        this->printVertices();
        this->printFaces();
        this->printHalfEdges();

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
        //to not print the empty vertex at the first index
        for(int i = 1; i <= this->m_tabVertices.size(); i++){
            printVertex(this->m_tabVertices.at(i));
        }
    }
};