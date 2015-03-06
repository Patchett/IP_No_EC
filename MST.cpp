#include "MST.h"

MST::MST(float **input, int size)
{
    adjacentMatrix = input;
    key = new int[size];
    mstSet = new bool[size];
    parent = new int[size];
    MSTSize = 0;

    TSP1p5_Total = 0;
    shortcutVertex = -1;

    visited.resize(size);
//     for (int i = 0; i < N; i++)
//         visited[i] = false;

    N = size;
}

MST::~MST()
{

}

//use Prim's algorithm or Kruskal algorithm. Copied from 'http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/'
void MST::makeTree()
{
    // Initialize all keys as INFINITE
    for (int i = 0; i < N; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    // Always include first 1st vertex in MST.
    key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST

    // The MST will have V vertices
    for (int count = 0; count < N - 1; count++) {
        // Pick the minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, mstSet);

        // Add the picked vertex to the MST Set
        mstSet[u] = true;

        // Update key value and parent index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (int v = 0; v < N; v++)
            // u is the parent.
            // v is a node in u's adj list. edge u -> v is the edge we update
            // mstSet[v] is false for vertices not yet included in MST
            // Update the key only if adjacentMatrix[u][v] is smaller than key[v]
            if (adjacentMatrix[u][v] && mstSet[v] == false && adjacentMatrix[u][v] <  key[v])
                parent[v]  = u, key[v] = adjacentMatrix[u][v];
    }

    MSTMatrix = (bool **)calloc(N, sizeof(int *));

    for (int i = 0; i < N; ++i)
        MSTMatrix[i] = (bool *)calloc(N, sizeof(int));

    for (int i = 1; i < N; i++) {
        MSTMatrix[parent[i]][i] = true;
        MSTSize += adjacentMatrix[parent[i]][i];
    }

    cout << "MST Size: " << MSTSize << endl;
}

// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int MST::minKey(int key[], bool mstSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < N; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed MST stored in parent[]
void MST::printMST()
{
    cout << endl;
    cout << "Minimum spanning tree from the adjacency matrix" << endl;
    cout << "Edge   Weight" << endl;
    for (int i = 1; i < N; i++)
        cout << parent[i] << " - " << i << "  " << adjacentMatrix[i][parent[i]] << endl;
}

void MST::printMSTMatrix()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (MSTMatrix[i][j])
                cout << MSTMatrix[i][j] << "," << adjacentMatrix[i][j] << " ";
            else
                cout << MSTMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

//calculate mean of all edges in the MST
float MST::calMean(int option)
{
    float mean = 0.0;

    if (option == MST_1) {
        //calculate
    } else if (option == TSP2) {

    } else if (option == TSP1_5) {

    }

    return mean;
}

//calculate standard deviation of all edges in the MST
float MST::calStd(int option)
{
    float std = 0.0;

    if (option == MST_1) {
        //calculate
    } else if (option == TSP2) {

    } else if (option == TSP1_5) {

    }

    return std;
}

//make a Eulerian tour by DFS
//add shortcuts if a vertex has no detours.
//calculate heuristic TSP cost

// pass in current_index, shortcut_flag, coordinates_of_vertex_to_shortcut
void MST::makeTSPHelper(int currentVertex, int parentVertex)
{
    bool shortcutFlag = true;
    visited[currentVertex] = true;
    if (shortcutVertex == -1)
        TSP1p5_Total += adjacentMatrix[parentVertex][currentVertex];
    else {
        TSP1p5_Total += adjacentMatrix[shortcutVertex][currentVertex];
        shortcutVertex = -1;
    }

    for (int i = 0; i < N; i++) {
        if (MSTMatrix[currentVertex][i] && !visited[i]) {
            shortcutFlag = false;
            makeTSPHelper(i, currentVertex);
        }
    }

    if (shortcutFlag)
        shortcutVertex = currentVertex;
}

void MST::makeTSP2()
{
    makeTSPHelper(0, 0);
}

void MST::makeTSP1_5()
{

    //construct minimum-weight-matching for the given MST
    minimumMatching();

    //make all edges has even degree by combining minimum-weight matching and MST
    combine();

    //calculate heuristic TSP cost
}

void MST::minimumMatching()   //if you choose O(n^2)
{
    //find minimum-weight matching for the MST.

    //you should carefully choose a matching algorithm to optimize the TSP cost.
}

void MST::combine()
{
    //combine minimum-weight matching with the MST to get a multigraph which has vertices with even degree
}
