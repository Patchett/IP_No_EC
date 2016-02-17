#include "MST.h"

extern bool debug;
extern bool verbose;
extern bool last_vertex;

MST::MST(float **input, int size, int runs)
{
    adjacentMatrix = input;
    num_of_runs = runs;
    key = new int[size];
    mstSet = new bool[size];
    parent = new int[size];
    Last_TSP1p5_Vertex = 0;
    Last_TSP2_Vertex = 0;
    MSTSize = 0;

    MSTMatrix = (int **)calloc(size, sizeof(int *));
    for (int i = 0; i < size; i++)
        MSTMatrix[i] = (int *)calloc(size, sizeof(int));

    TSP1p5_size = 0;
    TSP2_size = 0;
    shortcutVertex = -1;
    shortcut_vertex_euler = -1;

    visited.resize(size);
    visited_euler_tour.resize(size);
    fill(visited_euler_tour.begin(), visited_euler_tour.end(), false);
    fill(visited.begin(), visited.end(), false);

    N = size;
}

MST::~MST()
{
    delete []parent;
    delete []key;
    delete []mstSet;
    for (int i = 0; i < N; i++)
        free(MSTMatrix[i]);
    free(MSTMatrix);
    visited.clear();
    visited_euler_tour.clear();

//     for (int i = 0; i < odd_vertex_matrix.size(); i++)
//         for (int j = 0; j < odd_vertex_matrix.size(); j++)
//             delete odd_vertex_matrix[i][j];

//     for each(vector<MappedGraphItem *> vec in odd_vertex_matrix)
//         for each(MappedGraphItem * oPtr in vec)
//             delete oPtr;
}

//use Prim's algorithm or Kruskal algorithm. Copied from 'http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/'
int MST::makeTree()
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

    for (int i = 1; i < N; i++) {
        MSTMatrix[parent[i]][i] = 1;
        MSTMatrix[i][parent[i]] = 1;
        MSTSize += adjacentMatrix[parent[i]][i];
    }

    return MSTSize;
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
                cout << MSTMatrix[i][j] /*<< "," << adjacentMatrix[i][j]*/ << " ";
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

void MST::makeTSP2Helper(int currentVertex, int parentVertex)
{
    bool shortcutFlag = true;
    visited[currentVertex] = true;
    if (shortcutVertex == -1) {
        TSP2_size += adjacentMatrix[parentVertex][currentVertex];
        if (last_vertex)
            cout << parentVertex << " " << currentVertex << endl;
    } else {
        TSP2_size += adjacentMatrix[shortcutVertex][currentVertex];
        if (last_vertex)
            cout << shortcutVertex << " " << currentVertex << endl;
        shortcutVertex = -1;
    }
    Last_TSP2_Vertex = currentVertex;

    for (int i = 0; i < N; i++) {
        if (MSTMatrix[currentVertex][i] > 0 && !visited[i]) {
            shortcutFlag = false;
            makeTSP2Helper(i, currentVertex);
        }
    }

    if (shortcutFlag) {
        shortcutVertex = currentVertex;
        Last_TSP2_Vertex = shortcutVertex;
    }
}

float MST::makeTSP2()
{
    makeTSP2Helper(0, 0);
    if (verbose) {
        cout << Last_TSP2_Vertex << " 0" << endl;
        cout << "TSP2 done " << endl;
    }
    TSP2_size += adjacentMatrix[0][Last_TSP2_Vertex];
    return TSP2_size;
}

// Uses DFS to count the number of vertices reachable from the passed in vertex
int MST::countReachable(int currentVertex)
{
    visited[currentVertex] = true;
    int count = 1;

    for (int i = 0; i < N; i++) {
        if (MSTMatrix[currentVertex][i] > 0 && !visited[i])
            count += countReachable(i);
    }
    return count;
}

void MST::RemoveEdge(int i, int j)
{
    MSTMatrix[i][j]--;
    MSTMatrix[j][i]--;
}

void MST::AddEdge(int i, int j)
{
    MSTMatrix[i][j]++;
    MSTMatrix[j][i]++;
}

void MST::TSP1_5_Start(int current_vertex)
{
    // Maybe add some logic here if shortcut_vertex != -1
    visited_euler_tour[current_vertex] = true;
    for (int i = 0; i < N; i++) {
        if (MSTMatrix[current_vertex][i] > 0 && isValidEdge(current_vertex, i)) {
            // Edge we are looking at adding to our tour leads to a vertex that
            // was already visited. Store current_vertex so we can shortcut
            // the already visited vertex later
            if (i != current_vertex && visited_euler_tour[i] && shortcut_vertex_euler == -1) {
                shortcut_vertex_euler = current_vertex;
                Last_TSP1p5_Vertex = shortcut_vertex_euler;
                if (debug)
                    cout << "Next move is shortcut from " << shortcut_vertex_euler << endl;
                RemoveEdge(i, current_vertex);
                TSP1_5_Start(i);
            }
            // We are looking for a vertex in the tour that has not been visited so that
            // we can make our shortcut. The vertex examined in this iteration
            // has already been visited so we do not shortcut to it
            else if (i != current_vertex && visited_euler_tour[i] && shortcut_vertex_euler != -1) {
                RemoveEdge(i, current_vertex);
                TSP1_5_Start(i);
            }
            // Found a valid vertex to shortcut to. Execute the shortcut
            else if (!visited_euler_tour[i] && shortcut_vertex_euler != -1) {
                if (debug)
                    cout << "SHORTCUT: " << shortcut_vertex_euler << " - " << i << endl;
                RemoveEdge(i, current_vertex);
                TSP1p5_size += adjacentMatrix[shortcut_vertex_euler][i];
                Last_TSP1p5_Vertex = i;
                shortcut_vertex_euler = -1;
                TSP1_5_Start(i);
            } else {
                if (debug)
                    cout << current_vertex << " " << i << endl;
                RemoveEdge(i, current_vertex);
                Last_TSP1p5_Vertex = i;
                TSP1p5_size += adjacentMatrix[current_vertex][i];
                TSP1_5_Start(i);
            }
        }
    }
}

float MST::makeTSP1_5()
{
    if (verbose)
        cout << "Running TSP1p5 Calculation..." << endl;
    shortcut_vertex_euler = -1;
    TSP1_5_Start(0);
    TSP1p5_size += adjacentMatrix[Last_TSP1p5_Vertex][0];
    if (verbose)
        cout << Last_TSP1p5_Vertex << " 0" << endl;
    return TSP1p5_size;
}

void MST::FindOddVertices()
{
    int degree;
    for (int i = 0; i < N; i++) {
        degree = 0;
        for (int j = 0; j < N; j++) {
            if (MSTMatrix[i][j] > 0)
                degree++;
        }
        if (degree & 1)
            odd_vertices.insert(i);
    }
    MakeOddVertexMatrix(odd_vertices.size());
}

void MST::MakeOddVertexMatrix(int size)
{
    odd_vertex_matrix.resize(size, vector<MappedGraphItem>(size));

    // Grab the edges formed by all odd vertices in the MST
    // and form a complete graph out of them
    int ori = 0; // odd row index
    int oci = 0; // odd column index
    if (debug)
        cout << "Odd Vertices: " << endl;
    for (auto i : odd_vertices) {
        if (debug)
            cout << i << endl;
        oci = 0;
        for (auto j : odd_vertices) {
            odd_vertex_matrix[ori][oci] = MappedGraphItem(i, j, adjacentMatrix[i][j]);
            odd_vertex_matrix[oci][ori] = MappedGraphItem(j, i, adjacentMatrix[j][i]);
            oci++;
        }
        ori++;
    }
}

bool MST::isValidEdge(int currentVertex, int testVertex)
{
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (MSTMatrix[currentVertex][i] > 0)
            count++;
    }
    if (count == 1) {
        if (debug)
            cout << "Only 1 vertex reachable from " << currentVertex << endl;
        return true;
    }

    fill(visited.begin(), visited.end(), false);
    int reachable = countReachable(currentVertex);
    if (debug)
        cout << "reachable " << currentVertex << " - " << testVertex << ": " << reachable << endl;
    RemoveEdge(testVertex, currentVertex);

    fill(visited.begin(), visited.end(), false);
    int reachable_removed = countReachable(currentVertex);
    if (debug)
        cout << "reachable_removed " << currentVertex << " - " << testVertex << ": " << reachable_removed << endl;
    AddEdge(currentVertex, testVertex);

    return (reachable > reachable_removed) ? false : true;

}

int MST::GetNumberOfOddVertices()
{
    return odd_vertices.size();
}



