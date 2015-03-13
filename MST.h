#include "common.h"
#include "MappedGraphItem.h"


class MST {
public:
    float **adjacentMatrix;
    int **MSTMatrix;
    int *parent; //Array to store constructed MST
    int *key; //Key values used to pick minimum weight edge in cut
    bool *mstSet; //To represent set of vertices not yet included in MST
    int N; //the size of pointset
    int num_of_runs;

    MST(float **adjacentMatrix, int size, int runs);
    ~MST();

    //deliverable a
    int MSTSize;
    int makeTree();
    void printMST();

    //deliverable b
    void makeTSP2Helper(int currentVertex, int parentVertex);
    vector<bool> visited;
    float TSP2_size;
    int shortcutVertex;
    int Last_TSP2_Vertex;
    float makeTSP2();
    void printMSTMatrix();

    //deliverable c
    vector<bool> visited_euler_tour; // keeps track of visited vertices in the Euler tour
    float TSP1p5_size;
    int Last_TSP1p5_Vertex;
    // Allows us to map vertices from the perfect matching to vertices in the
    // original Adjacency Matrix
    vector<vector<MappedGraphItem *>> odd_vertex_matrix;
    // Set of odd vertices from the MST
    set<int> odd_vertices;
    // Keeps track of which vertex to shortcut in the Euler Tour during TSP1p5 calculation
    int shortcut_vertex_euler;
    // Implements Fleury's Algorithm and allows us to shortcut the Euler Tour that it generates
    void TSP1_5_Start(int currentVertex);
    // Counts the number of vertices that are reachable from the passed in vertex
    // using a DFS traversal
    int countReachable(int currentVertex);
    // Checks if the passed in edge (currentVertex->testVertex) is a bridge. Tells us
    // if it is safe to delete the edge in Fleury's algorithm
    bool isValidEdge(int currentVertex, int testVertex);
    // Call this to start TSP1p5
    float makeTSP1_5();
    // Finds the odd vertices in the MST so we can pass it to Blossom V for perfect matching
    void FindOddVertices();
    // Create an adj matrix out of the odd vertices. We pass this adj matrix to Blossom V
    void MakeOddVertexMatrix(int size);
    // Return number of odd vertices found in MST
    int GetNumberOfOddVertices();
    // Remove an edge from the Eulerian Circuit (Fleury's Algorithm)
    void RemoveEdge(int i, int j);
    // Add an edge from the Eulerian Circuit (Fleury's Algorithm)
    void AddEdge(int i, int j);

    float calMean(int option);
    float calStd(int option);

private:
    int minKey(int key[], bool mstSet[]);

};
