#include "common.h"
#include "point.h"
#include "MST.h"
#include "Minmatching/PerfectMatching.h"
// extern "C" {
// #include "LKHmain.h"
// }

/*
This project is a starter code and wrappers for CSE101W15 Implementation project.

point.h - uniform random pointset generator

MST.h - minimum spanning tree

PerfectMatching.h - interface to min cost perfect matching code

-------------------------------------
PerfectMatching is from the paper:

Vladimir Kolmogorov. "Blossom V: A new implementation of a minimum cost perfect matching algorithm."
In Mathematical Programming Computation (MPC), July 2009, 1(1):43-67.

sourcecode : pub.ist.ac.at/~vnk/software/blossom5-v2.05.src.tar.gz

*/

void LoadInput(int &node_num, int &edge_num, int *&edges, int *&weights, float **adjacentMatrix, int N)
{
    int e = 0;
    node_num = N;
    edge_num = N * (N - 1) / 2 ; //complete graph

    edges = new int[2 * edge_num];
    weights = new int[edge_num];

    for (int i = 0; i < N ; ++i) {
        for (int j = i + 1 ; j < N ; ++j) {
            edges[2 * e] = i;
            edges[2 * e + 1] = j;
            weights[e] = adjacentMatrix[i][j];
            e++;
        }
    }

    if (e != edge_num) {
        cout << "the number of edge is wrong" << endl;

        exit(1);
    }
}

void PrintMatching(int node_num, PerfectMatching *pm)
{
    int i, j;

    for (i = 0; i < node_num; i++) {
        j = pm->GetMatch(i);
        if (i < j) printf("%d %d\n", i, j);
    }
}

int main()
{
    set<pair<int, int>> generatedPointset;
    float **adjacentMatrix;
    int W, H, N;
    Point pointset;

//    Answers from TA for inputs W=5000, H=3000, N=7000
//     [mean]
//     MST:  208536
//     TSP2 : 319860
//     TSP1p5 : 262852
//
//     [Standard deviation]
//     MST : 510.87
//     TSP2 : 1279.221
//     TSP1p5 : 1123.846

    W = 5000;
    H = 3000;
    N = 7000;

//     W = 100;
//     H = 100;
//     N = 10;

//     W = 12705;
//     H = 15640;
//     N = 8146;

    cout << "W: " << W << " H: " << H << " N:" << N << endl;

    pointset.generatePoint(W, H, N); //max(W,H,N) should be < 20000 because of memory limitation
    //pointset.printPointset();

    generatedPointset = pointset.getPointset();
    adjacentMatrix = pointset.getAdjacentMatrix();

    //Deliverable A: From pointset and adjacentMatrix, you should construct MST with Prim or Kruskal
    MST *mst = new MST(adjacentMatrix, N);
    mst->makeTree();
//     mst->printMST();
//     mst->printMSTMatrix();


    //Deliverable B: Find TSP2 path from the constructed MST
    //You won't need any wrappers for B.
    mst->makeTSP2();
    cout << "TSP1p5 Total: " << mst->TSP1p5_Total << endl;

    //Deliverable C: Find TSP1.5 path from the constructed MST

    //Find the perfect minimum-weight matching
    struct PerfectMatching::Options options;
    int e, node_num = N;
    int edge_num = N * (N - 1) / 2;
    int *edges;
    int *weights;
    PerfectMatching *pm = new PerfectMatching(node_num, edge_num);

    LoadInput(node_num, edge_num, edges, weights, adjacentMatrix, N);

    for (e = 0; e < edge_num; e++)
        pm->AddEdge(edges[2 * e], edges[2 * e + 1], weights[e]);

    pm->options = options;
    pm->Solve();

    double cost = ComputePerfectMatchingCost(node_num, edge_num, edges, weights, pm);
    printf("Total cost of the perfect min-weight matching = %.1f\n", cost);

    //PrintMatching(node_num, pm);

    // ===================================================== //
    // ==================== BELOW IS EC ==================== //
    // ===================================================== //

//     remove("Parameter_File_2OPT.par");
//     remove("Parameter_File_KOPT.par");
//     remove("Problem_File.tsp");

    //Setup parameters for EC1 2OPT-E

//     double startTime = (double) clock() / CLOCKS_PER_SEC;
//     ofstream Parameter_File_2OPT("Parameter_File_2OPT.par");
//     Parameter_File_2OPT << "PROBLEM_FILE = Problem_File" << endl;
//     Parameter_File_2OPT << "MOVE_TYPE = 2" << endl;
//     Parameter_File_2OPT << "RUNS = 1" << endl;
//     LKHMain("Parameter_File_2OPT.par", "Problem_File.tsp");
//     cout << "2OPT Runtime Total: " << (double) clock() / CLOCKS_PER_SEC - startTime << endl;

    // Setup parameters for EC1 KOPT-E
//     ofstream Parameter_File_KOPT("Parameter_File_KOPT.par");
//     Parameter_File_KOPT << "PROBLEM_FILE = Problem_File" << endl;
//     Parameter_File_KOPT << "MOVE_TYPE = 5" << endl;
//     Parameter_File_KOPT << "MAX_SWAPS = " << N / 2 << endl;
//     Parameter_File_KOPT << "MAX_TRIALS = " << N / 2 << endl;
//     Parameter_File_KOPT << "RUNS = 1" << endl;

//     LKHMain("Parameter_File_KOPT.par", "Problem_File.tsp");

    /*    Parameter_File_2OPT.close();*/

//     if (remove("Parameter_File_2OPT.par"))
//         cout << "Parameter_File_2OPT.par was successfully deleted" << endl;
//     if (remove("Parameter_File_KOPT.par"))
//         cout << "Parameter_File_KOPT.par was successfully deleted" << endl;
//     if (remove("Problem_File.tsp"))
//         cout << "Problem_File.tsp was successfully deleted" << endl;

    delete pm;
    delete [] edges;
    delete [] weights;

    return 0;
}
