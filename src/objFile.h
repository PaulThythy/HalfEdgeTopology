#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct
{
    int vIndex ;
    int heTwinIndex ;
    int faceIndex ;
    int heNextIndex ;
    int hePrevIndex ;
} HeStruct;

typedef struct
{
    double x,y,z ;
    int heIndex ;
} VStruct;

typedef struct
{
    int v0, v1, v2 ;
    int heIndex ;
} FStruct;

struct OBJFile{
    vector<HeStruct*> tabHalfEdges;
    vector<FStruct*> tabFaces;
    vector<VStruct*> tabVertices;
    string m_filename;

    inline OBJFile() {}

    inline OBJFile(string fileName): m_filename(fileName) {}
    inline ~OBJFile() {}

    inline void readData() {
        string line;
        int nbVertices(0), nbFaces(0);

        ifstream myfile("test.obj");
        if(myfile.is_open()){
            while(getline(myfile, line)){
                float x, y, z;
                char carLine;
                
                cout << line << '\n';
                istringstream istr(line);
                istr >> carLine >> x >> y >> z ;

                //*************************************
                // TODO
                // stock data into data a structure
                //*************************************
            }
            myfile.close();
        }
        else cout << "Unable to open file";

        this->constructTopology();
    }

    inline void constructTopology(){
        //************************************************
        // TODO
        // exploit obj file datas to build the index tab
        //************************************************
    }
};