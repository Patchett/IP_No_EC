#include "point.h"
extern bool debug;

Point::Point()
{
    pointNum = 0;
}

Point::~Point()
{
    for (int i = 0; i < pointNum; i++)
        free(adjacentMatrix[i]);
    free(adjacentMatrix);
}

//Added for EC2
void Point::generateProblemFile()
{
    pointset.clear();

    int xp = 0;
    int yp = 0;

    ofstream Problem_File("Problem_File");
    Problem_File << "NAME : Problem_File" << endl;
    Problem_File << "COMMENT : Initial randomly generated pointset" << endl;
    Problem_File << "TYPE : TSP" << endl;
    Problem_File << "DIMENSION : " << pointNum << endl;
    Problem_File << "EDGE_WEIGHT_TYPE : EUC_2D" << endl;
    Problem_File << "NODE_COORD_SECTION" << endl;

    //uniform distribution generation
    std::default_random_engine x_generator;
    std::default_random_engine y_generator;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> x_distribution(0, height);
    std::uniform_int_distribution<int> y_distribution(0, width);

    int count = 1;
    for (int i = 0; i < pointNum; i++) {
        xp = x_distribution(generator);
        yp = y_distribution(generator);
        // Print points to file for LKH
        Problem_File << count << " " << xp << " " << yp << endl;
        pair<int, int> point(xp, yp);
        pointset.insert(point);
        count++;
    }

    Problem_File.close();
}

void Point::generatePoint(unsigned int H, unsigned int W, unsigned int N)
{
    int row = 0, col = 0;
    int xp = 0;
    int yp = 0;

    if (N > MAX_N || W > MAX_N || H > MAX_N) {
        cout << "W,H,N should be less than " << MAX_N << endl;

        return;
    } else if (N > W + H) {
        cout << "N should be less than H or W" << endl;

        return;
    }

    pointNum = N;
    this->width = W;
    this->height = H;

    // Open file and write points/parameters for LKH
    ofstream Problem_File("Problem_File");
    Problem_File << "NAME : Problem_File" << endl;
    Problem_File << "COMMENT : Initial randomly generated pointset" << endl;
    Problem_File << "TYPE : TSP" << endl;
    Problem_File << "DIMENSION : " << N << endl;
    Problem_File << "EDGE_WEIGHT_TYPE : EUC_2D" << endl;
    Problem_File << "NODE_COORD_SECTION" << endl;

    //uniform distribution generation
    std::default_random_engine x_generator;
    std::default_random_engine y_generator;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> x_distribution(0, H);
    std::uniform_int_distribution<int> y_distribution(0, W);

    int count = 1;
    for (unsigned int i = 0; i < N; i++) {
        xp = x_distribution(generator);
        yp = y_distribution(generator);
        // Print points to file for LKH
        Problem_File << count << " " << xp << " " << yp << endl;
        pair<int, int> point(xp, yp);
        pointset.insert(point);
        count++;
    }

    adjacentMatrix = (float **)malloc(N * sizeof(int *));
    for (unsigned int i = 0; i < N ; ++i)
        adjacentMatrix[i] = (float *)malloc(N * sizeof(int));

    for (set<pair<int, int>>::iterator it = pointset.begin() ; it != pointset.end() ; ++it, ++row) {
        int x1 = (*it).first;
        int y1 = (*it).second;

        col = 0;
        for (set<pair<int, int>>::iterator that = pointset.begin() ; that != pointset.end() ; ++that, ++col) {
            int x2 = (*that).first;
            int y2 = (*that).second;

            adjacentMatrix[row][col] = getEuclideanDistance(x1, y1, x2, y2);
        }
        if (debug) {
            if (row % 100 == 0)  //show progress
                cout << ".";
        }
    }
    Problem_File.close();
}

void Point::printPointset()
{
    cout << "Generated pointset list: " << endl;
    for (set<pair<int, int>>::iterator it = pointset.begin() ; it != pointset.end() ; ++it)
        cout << (*it).first << " , " << (*it).second << endl;
    cout << endl;

    cout << "adjacency matrix: " << endl;
    for (int i = 0; i < pointNum ; ++i) {
        for (int j = 0; j < pointNum ; ++j)
            cout << adjacentMatrix[i][j] << "  ";
        cout << endl;
    }
}

float Point::getEuclideanDistance(int x1, int y1, int x2, int y2)
{

    return sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}

set<pair<int, int>> Point::getPointset()
{

    return pointset;
}

float **Point::getAdjacentMatrix()
{

    return adjacentMatrix;
}

int Point::getPointNum()
{

    return pointNum;
}
