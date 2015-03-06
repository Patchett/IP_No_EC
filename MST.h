#include "common.h"

#pragma once

class MST {
public:
    float **adjacentMatrix;
    bool **MSTMatrix;


    int *parent; //Array to store constructed MST
    int *key; //Key values used to pick minimum weight edge in cut
    bool *mstSet; //To represent set of vertices not yet included in MST
    int N; //the size of pointset

    MST(float **adjacentMatrix, int size);
    ~MST();

    //deliverable a
    int MSTSize;
    void makeTree();
    void printMST();

    //deliverable b
    void makeTSPHelper(int currentVertex, int parentVertex);
    vector<bool> visited;
    float TSP1p5_Total;
    int shortcutVertex;
    void makeTSP2();
    void printMSTMatrix();

    //deliverable c
    void makeTSP1_5();

    float calMean(int option);
    float calStd(int option);

private:
    void minimumMatching();
    void combine();
    int minKey(int key[], bool mstSet[]);

};
