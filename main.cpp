#include "common.h"
#include "point.h"
#include "MST.h"
#include "Minmatching/PerfectMatching.h"
/*extern "C" {
#include "LKHmain.h"
}*/

bool debug = false;
bool verbose = false;
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

void LoadInput(int &node_num, int &edge_num, int *&edges, int *&weights, MST *mst, int N)
{
    int e = 0;
    node_num = N;
    edge_num = N * (N - 1) / 2 ; //complete graph

    edges = new int[2 * edge_num];
    weights = new int[edge_num];

    if (debug)
        cout << "Loading Input" << endl;
    for (int i = 0; i < N ; ++i) {
        for (int j = i + 1 ; j < N ; ++j) {
            if (debug)
                cout << i << " " << j << " = " << mst->odd_vertex_matrix[i][j]->adj_row_index << " " << mst->odd_vertex_matrix[i][j]->adj_col_index << endl;
            edges[2 * e] = i;
            edges[2 * e + 1] = j;
            weights[e] = mst->odd_vertex_matrix[i][j]->weight;
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

void CombineMST_Matching(int node_num, PerfectMatching *pm, MST *&mst)
{
    int i, j;
    // i = rows, j = cols
    for (i = 0; i < node_num; i++) {
        j = pm->GetMatch(i);
        mst->MSTMatrix[(mst->odd_vertex_matrix[i][j]->adj_row_index)][(mst->odd_vertex_matrix[i][j]->adj_col_index)]++;
        if (debug)
            cout << i << " " << j << endl;
    }
}

int main()
{


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

//     W = 10000;
//     H = 10000;
//     N = 10000;

//     W = 5000;
//     H = 3000;
//     N = 7000;

    int W = 100;
    int H = 100;
    int N = 8;

//     int W = 12705;
//     int H = 15640;
//     int N = 8146;

    int runs = 1;

    int current_MST_cost = 0;
    int total_MST_cost = 0;

    int current_TSP2_cost = 0;
    int total_TSP2_cost = 0;

    int current_TSP1p5_cost = 0;
    int total_tsp1p5_cost = 0;

    cout << "Please enter number of runs: ";
    cin >> runs;
    cout << "Please enter W: ";
    cin >> W;
    cout << "Please enter H: ";
    cin >> H;
    cout << "Please enter N: ";
    cin >> N;
    cout << "W:" << W << ", H:" << H << ", N:" << N << ", Runs:" << runs << endl;
    cout << "Note: For large inputs, TSP1p5 calulation can take 2-3 minutes per trial." << endl;

    // ============================================================ //

    for (int i = 1; i < runs + 1; i++) {
        set<pair<int, int>> generatedPointset;
        float **adjacentMatrix;
        Point *pointset = new Point();

        pointset->generatePoint(W, H, N); //max(W,H,N) should be < 20000 because of memory limitation
        //pointset.printPointset();

        generatedPointset = pointset->getPointset();
        adjacentMatrix = pointset->getAdjacentMatrix();

        //Deliverable A: From pointset and adjacentMatrix, you should construct MST with Prim or Kruskal
        MST *mst = new MST(adjacentMatrix, N, runs);
        current_MST_cost = mst->makeTree();
        if (verbose)
            cout << "MST cost on run: " << i << ": " << current_MST_cost << endl;
        total_MST_cost += current_MST_cost;
        if (debug) {
            mst->printMST();
            mst->printMSTMatrix();
        }


        //Deliverable B: Find TSP2 path from the constructed MST
        current_TSP2_cost = mst->makeTSP2();
        if (verbose)
            cout << "TSP2 cost on run " << i << ": " << current_TSP2_cost << endl;
        total_TSP2_cost += current_TSP2_cost;


        //Deliverable C: Find TSP1.5 path from the constructed MST

        //Find the perfect minimum-weight matching
        if (debug)
            cout << "Calculating odd degree perfect matching..." << endl;
        mst->FindOddVertices();

        struct PerfectMatching::Options options;
        if (debug)
            cout << "Number of odd vertices: " << mst->GetNumberOfOddVertices() << endl;
        int e, node_num = mst->GetNumberOfOddVertices();
        int edge_num = node_num * (node_num - 1) / 2;
        int *edges;
        int *weights;
        PerfectMatching *pm = new PerfectMatching(node_num, edge_num);

        LoadInput(node_num, edge_num, edges, weights, mst, node_num);

        for (e = 0; e < edge_num; e++)
            pm->AddEdge(edges[2 * e], edges[2 * e + 1], weights[e]);

        pm->options = options;
        pm->Solve();

        double PM_cost = ComputePerfectMatchingCost(node_num, edge_num, edges, weights, pm);

        if (debug)
            printf("Total cost of the perfect min-weight matching = %.1f\n", PM_cost);
        CombineMST_Matching(node_num, pm, mst);

        if (debug)
            mst->printMSTMatrix();
        current_TSP1p5_cost = mst->makeTSP1_5();
        if (verbose)
            cout << "TSP2 cost on run: " << i << ": " << current_TSP1p5_cost << endl;
        total_tsp1p5_cost += current_TSP1p5_cost;

        delete pm;
        delete mst;
        delete pointset;
        delete[] edges;
        delete[] weights;

        cout << "Run " << i << " complete " << endl;
        //PrintMatching(node_num, pm);
    }

    // Timing
//     double startTime = (double) clock() / CLOCKS_PER_SEC;
//     cout << "TSP1p5 Total Runtime: " << (double) clock() / CLOCKS_PER_SEC - startTime << endl;


    cout << "==================== Stats ====================" << endl;

    cout << "W:" << W << ", H:" << H << ", N:" << N << ", Runs:" << runs << endl;

    cout << "==================== Totals ====================" << endl;
    cout << "Total MST cost: " << total_MST_cost << endl;
    cout << "Total TSP2 cost: " << total_TSP2_cost << endl;
    cout << "Total TSP1p5 cost: " << total_tsp1p5_cost << endl;

    cout << "==================== Averages ====================" << endl;
    cout << "Average MST cost: " << total_MST_cost / runs << endl;
    cout << "Average TSP2 cost: " << total_TSP2_cost / runs << endl;
    cout << "Average TSP1p5 cost: " << total_tsp1p5_cost / runs << endl;

    cout << "Standard Deviation results will go here when I discover proper formula" << endl;
//     cout << "=============== Standard Deviations ===============" << endl;
//     cout << "Standard Deviation MST cost: " << total_MST_cost / runs << endl;
//     cout << "Average TSP2 cost: " << total_TSP2_cost / runs << endl;
//     cout << "Average TSP1p5 cost: " << total_tsp1p5_cost / runs << endl;


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


    // Setup parameters for EC2 KOPT-E


    //ofstream Parameter_File_KOPT("Parameter_File_KOPT.par");
    //Parameter_File_KOPT << "PROBLEM_FILE = Problem_File" << endl;
    //Parameter_File_KOPT << "MOVE_TYPE = 5" << endl;
    //Parameter_File_KOPT << "MAX_SWAPS = " << N / 2 << endl;
    //Parameter_File_KOPT << "MAX_TRIALS = " << N  << endl;
    //Parameter_File_KOPT << "PATCHING_C = 2 " << endl;
    //Parameter_File_KOPT << "PATCHING_A = 2 " << endl;
    //Parameter_File_KOPT << "ASCENT_CANDIDATES = 100" << endl;
    //Parameter_File_KOPT << "RUNS = 1" << endl;
    //Parameter_File_KOPT.close();

    //double totalTime = 0;
    //int TRIALS = 1;
    //for (int i = 1; i < TRIALS; i++) {
    //    double startTime = (double) clock() / CLOCKS_PER_SEC;
    //    pointset.generateProblemFile();
    //    LKHMain("Parameter_File_KOPT.par", "Problem_File.tsp");
    //    totalTime += (double) clock() / CLOCKS_PER_SEC - startTime;
    //    //cout << "2OPT Runtime Total: " << (double) clock() / CLOCKS_PER_SEC - startTime << endl;
    //}
    //cout << "Average runtime: " << totalTime / TRIALS << endl;



//     if (remove("Parameter_File_2OPT.par"))
//         cout << "Parameter_File_2OPT.par was successfully deleted" << endl;
//     if (remove("Parameter_File_KOPT.par"))
//         cout << "Parameter_File_KOPT.par was successfully deleted" << endl;
//     if (remove("Problem_File.tsp"))
//         cout << "Problem_File.tsp was successfully deleted" << endl;

//     delete pm;
//     delete mst;
//     delete [] edges;
//     delete [] weights;

    return 0;
}
