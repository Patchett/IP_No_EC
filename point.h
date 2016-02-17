#include "common.h"

#pragma once

class Point {
public:
    float **adjacentMatrix;
    int pointNum; //the number of generated pointset
    set<pair<int, int>> pointset;

    Point(int N);
    ~Point();

    void generatePoint(unsigned int H, unsigned int W, unsigned int N);
    set<pair<int, int>> getPointset();
    void setPointset();
    float **getAdjacentMatrix();
    int getPointNum();
    void printPointset();

    // added for EC2
    void generateProblemFile();
    int width;
    int height;



private:
    float getEuclideanDistance(int x1, int y1, int x2, int y2);
};
