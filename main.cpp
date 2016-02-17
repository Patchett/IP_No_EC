#include "common.h"
#include "point.h"
#include "MST.h"
#include "Minmatching/PerfectMatching.h"
// extern "C" {
// #include "LKHmain.h"
// }

bool debug = false;
bool verbose = false;
bool last_vertex = false;
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
                cout << i << " " << j << " = " << mst->odd_vertex_matrix[i][j].adj_row_index << " " << mst->odd_vertex_matrix[i][j].adj_col_index << endl;
            edges[2 * e] = i;
            edges[2 * e + 1] = j;
            weights[e] = mst->odd_vertex_matrix[i][j].weight;
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
        mst->MSTMatrix[(mst->odd_vertex_matrix[i][j].adj_row_index)][(mst->odd_vertex_matrix[i][j].adj_col_index)]++;
        if (debug)
            cout << i << " " << j << endl;
    }
}
void RunSolution(int W, int H, int N, int runs, int i,
                 vector<int> &MST_Costs,
                 vector<float> &TSP2_Costs,
                 vector<float> &TSP1p5_Costs,
                 vector<double> &MST_Times,
                 vector<double> &TSP2_Times,
                 vector<double> &TSP1p5_Times)
{

    set<pair<int, int>> generatedPointset;
    float **adjacentMatrix;
    Point *pointset = new Point(N);

    pointset->generatePoint(W, H, N); //max(W,H,N) should be < 20000 because of memory limitation
    //pointset.printPointset();

    generatedPointset = pointset->getPointset();
    adjacentMatrix = pointset->getAdjacentMatrix();

    //Deliverable A: From pointset and adjacentMatrix, you should construct MST with Prim or Kruskal
    MST *mst = new MST(adjacentMatrix, N, runs);
    double start_MST = (double) clock() / CLOCKS_PER_SEC;
    MST_Costs.push_back(mst->makeTree());
    MST_Times.push_back(((double) clock() / CLOCKS_PER_SEC - start_MST));

    if (verbose) {
        cout << "MST cost on run: " << i << ": " << MST_Costs[i - 1] << endl;
        cout << "MST run time on run " << i << ": " << MST_Times[i - 1] << endl;
    }
    if (debug) {
        mst->printMST();
        mst->printMSTMatrix();
    }


    //Deliverable B: Find TSP2 path from the constructed MST
    double start_TSP2 = (double) clock() / CLOCKS_PER_SEC;
    TSP2_Costs.push_back(mst->makeTSP2());
    TSP2_Times.push_back(((double) clock() / CLOCKS_PER_SEC - start_TSP2));
    if (verbose) {
        cout << "TSP2 cost on run " << i << ": " << TSP2_Costs[i - 1] << endl;
        cout << "TSP2 run time on run " << i << ": " << TSP2_Times[i - 1] << endl;
    }


    //Deliverable C: Find TSP1.5 path from the constructed MST

    //Find the perfect minimum-weight matching
    if (debug)
        cout << "Calculating odd degree perfect matching..." << endl;
    double start_TSP1p5 = (double) clock() / CLOCKS_PER_SEC;
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
    TSP1p5_Costs.push_back(mst->makeTSP1_5());
    TSP1p5_Times.push_back(((double) clock() / CLOCKS_PER_SEC - start_TSP1p5));
    if (verbose) {
        cout << "TSP1p5 cost on run: " << i << ": " << TSP1p5_Costs[i - 1] << endl;
        cout << "TSP1p5 run time on run " << i << ": " << TSP1p5_Times[i - 1] << endl;
    }

    delete pm;
    delete mst;
    delete pointset;
    delete[] edges;
    delete[] weights;

    cout << "Run " << i << " complete " << endl;
    //PrintMatching(node_num, pm);
    return;
}

void Calculate_Cost_Stats(vector<int> &MST_Costs,
                          vector<float> &TSP2_Costs,
                          vector<float> &TSP1p5_Costs)
{
    int size = MST_Costs.size();
    int sum_MST = std::accumulate(MST_Costs.begin(), MST_Costs.end(), 0.0);
    float sum_TSP2 = std::accumulate(TSP2_Costs.begin(), TSP2_Costs.end(), 0.0);
    float sum_TSP1p5 = std::accumulate(TSP1p5_Costs.begin(), TSP1p5_Costs.end(), 0.0);

    float MST_Mean = sum_MST / size;
    float TSP2_Mean = sum_TSP2 / size;
    float TSP1p5_Mean = sum_TSP1p5 / size;

    double accum_MST = 0.0;
    double accum_TSP2 = 0.0;
    double accum_TSP1p5 = 0.0;

    for_each(MST_Costs.begin(), MST_Costs.end(), [&](const double d) {
        accum_MST += (d - MST_Mean) * (d - MST_Mean);
    });
    for_each(TSP2_Costs.begin(), TSP2_Costs.end(), [&](const double d) {
        accum_TSP2 += (d - TSP2_Mean) * (d - TSP2_Mean);
    });
    for_each(TSP1p5_Costs.begin(), TSP1p5_Costs.end(), [&](const double d) {
        accum_TSP1p5 += (d - TSP1p5_Mean) * (d - TSP1p5_Mean);
    });

    double stdev_MST = sqrt(accum_MST / (size - 1));
    double stdev_TSP2 = sqrt(accum_TSP2 / (size - 1));
    double stdev_TSP1p5 = sqrt(accum_TSP1p5 / (size - 1));

    cout << "==================== Total Costs ====================" << endl;
    cout << "Total MST cost: " << sum_MST << endl;
    cout << "Total TSP2 cost: " << sum_TSP2 << endl;
    cout << "Total TSP1p5 cost: " << sum_TSP1p5 << endl;

    cout << "==================== Average Costs ====================" << endl;
    cout << "Average MST cost: " << MST_Mean << endl;
    cout << "Average TSP2 cost: " << TSP2_Mean << endl;
    cout << "Average TSP1p5 cost: " << TSP1p5_Mean << endl;

    cout << "=============== Standard Deviation Costs ===============" << endl;
    cout << "Standard Deviation MST cost: " << stdev_MST << endl;
    cout << "Standard Deviation TSP2 cost: " << stdev_TSP2 << endl;
    cout << "Standard Deviation TSP1p5 cost: " << stdev_TSP1p5 << endl;
}
void Calculate_Time_Stats(vector<double> &MST_Times,
                          vector<double> &TSP2_Times,
                          vector<double> &TSP1p5_Times)
{
    int size = MST_Times.size();
    double sum_MST = std::accumulate(MST_Times.begin(), MST_Times.end(), 0.0);
    double sum_TSP2 = std::accumulate(TSP2_Times.begin(), TSP2_Times.end(), 0.0);
    double sum_TSP1p5 = std::accumulate(TSP1p5_Times.begin(), TSP1p5_Times.end(), 0.0);

    double MST_Mean = sum_MST / size;
    double TSP2_Mean = sum_TSP2 / size;
    double TSP1p5_Mean = sum_TSP1p5 / size;

    double accum_MST = 0.0;
    double accum_TSP2 = 0.0;
    double accum_TSP1p5 = 0.0;

    for_each(MST_Times.begin(), MST_Times.end(), [&](const double d) {
        accum_MST += (d - MST_Mean) * (d - MST_Mean);
    });
    for_each(TSP2_Times.begin(), TSP2_Times.end(), [&](const double d) {
        accum_TSP2 += (d - TSP2_Mean) * (d - TSP2_Mean);
    });
    for_each(TSP1p5_Times.begin(), TSP1p5_Times.end(), [&](const double d) {
        accum_TSP1p5 += (d - TSP1p5_Mean) * (d - TSP1p5_Mean);
    });

    double stdev_MST = sqrt(accum_MST / (size - 1));
    double stdev_TSP2 = sqrt(accum_TSP2 / (size - 1));
    double stdev_TSP1p5 = sqrt(accum_TSP1p5 / (size - 1));

    cout << "==================== Total Times ====================" << endl;
    cout << "Total MST run time: " << sum_MST << endl;
    cout << "Total TSP2 run time: " << sum_TSP2 << endl;
    cout << "Total TSP1p5 run time: " << sum_TSP1p5 << endl;

    cout << "==================== Average Times ====================" << endl;
    cout << "Average MST run time: " << MST_Mean << endl;
    cout << "Average TSP2 run time: " << TSP2_Mean << endl;
    cout << "Average TSP1p5 run time: " << TSP1p5_Mean << endl;

    cout << "=============== Standard Deviation Times ===============" << endl;
    cout << "Standard Deviation MST run time: " << stdev_MST << endl;
    cout << "Standard Deviation TSP2 run time: " << stdev_TSP2 << endl;
    cout << "Standard Deviation TSP1p5 run time: " << stdev_TSP1p5 << endl;
}
void Print_Individual_Stats(const vector<int> MST_Costs,
                            const vector<float> TSP2_Costs,
                            const vector<float> TSP1p5_Costs,
                            const vector<double> MST_Times,
                            const vector<double> TSP2_Times,
                            const vector<double> TSP1p5_Times)
{

    cout << "Run          MST_Cost    TSP2_Cost   TSP1p5_Costs MST_Time    TSP2_Time   TSP1p5_Time" << endl;
    for (unsigned int i = 0; i < MST_Costs.size(); i++)
        cout << setw(13) << left << i + 1 << setw(13) << left << MST_Costs[i] << setw(13) << left  << TSP2_Costs[i] << setw(13) << left  << TSP1p5_Costs[i] << setw(13) << left  << MST_Times[i] << setw(13) << left  << TSP2_Times[i] << setw(13) << left  << TSP1p5_Times[i] << endl;
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
    int N = 25;

//     int W = 12705;
//     int H = 15640;
//     int N = 8146;

    int runs = 2;

    cout << "Please enter number of runs: ";
    cin >> runs;
    cout << "Please enter W: ";
    cin >> W;
    cout << "Please enter H: ";
    cin >> H;
    cout << "Please enter N: ";
    cin >> N;
    cout << "W:" << W << ", H:" << H << ", N:" << N << ", Runs:" << runs << endl;
    cout << "Note: For large inputs, TSP1p5 calulation can up to 5 minutes per trial." << endl;

    vector<int> MST_Costs;
    vector<float> TSP2_Costs;
    vector<float> TSP1p5_Costs;

    vector<double> MST_Times;
    vector<double> TSP2_Times;
    vector<double> TSP1p5_Times;

    // ============================================================ //
    /*    goto Extra_Credit;*/
    double startMain = (double) clock() / CLOCKS_PER_SEC;
    for (int i = 1; i < runs + 1; i++)
        RunSolution(W, H, N, runs, i,
                    MST_Costs,
                    TSP2_Costs,
                    TSP1p5_Costs,
                    MST_Times,
                    TSP2_Times,
                    TSP1p5_Times);

    // Timing
//     double startTime = (double) clock() / CLOCKS_PER_SEC;
//     cout << "TSP1p5 Total Runtime: " << (double) clock() / CLOCKS_PER_SEC - startTime << endl;

    cout << endl;
    Print_Individual_Stats(MST_Costs,
                           TSP2_Costs,
                           TSP1p5_Costs,
                           MST_Times,
                           TSP2_Times,
                           TSP1p5_Times);

    cout << "==================== Cost Stats ====================" << endl;
    cout << "W:" << W << ", H:" << H << ", N:" << N << ", Runs:" << runs << endl;
    Calculate_Cost_Stats(MST_Costs, TSP2_Costs, TSP1p5_Costs);
    cout << endl;
    cout << "==================== Time Stats ====================" << endl;
    Calculate_Time_Stats(MST_Times, TSP2_Times, TSP1p5_Times);

    // ===================================================== //
    // ==================== BELOW IS EC ==================== //
    // ===================================================== //

    /*Extra_Credit:*/
//     remove("Parameter_File_2OPT.par");
//     remove("Parameter_File_KOPT.par");
//     remove("Problem_File.tsp");

    // Setup parameters for EC2 KOPT-E

//     ofstream Parameter_File_KOPT("Parameter_File_KOPT.par");
//     Parameter_File_KOPT << "PROBLEM_FILE = Problem_File" << endl;
//     Parameter_File_KOPT << "MOVE_TYPE = 5" << endl;
//     Parameter_File_KOPT << "MAX_SWAPS = " << N / 2 << endl;
//     Parameter_File_KOPT << "MAX_TRIALS = " << N / 2  << endl;
// //     Parameter_File_KOPT << "PATCHING_C = 2 " << endl;
// //     Parameter_File_KOPT << "PATCHING_A = 2 " << endl;
//     Parameter_File_KOPT << "ASCENT_CANDIDATES = 2" << endl;
//     Parameter_File_KOPT << "EXTRA_CANDIDATES = 1" << endl;
//     Parameter_File_KOPT << "POPULATION_SIZE = 1" << endl;
//     Parameter_File_KOPT << "RUNS = 1" << endl;
//     Parameter_File_KOPT << "TRACE_LEVEL = 0" << endl;
//     Parameter_File_KOPT.close();
//
//     double totalTime = 0;
//     int TRIALS = 2;
//     for (int i = 1; i < TRIALS; i++) {
//         double startTime = (double) clock() / CLOCKS_PER_SEC;
//         LKHMain("Parameter_File_KOPT.par", "Problem_File.tsp");
//         totalTime += (double) clock() / CLOCKS_PER_SEC - startTime;
//         //cout << "2OPT Runtime Total: " << (double) clock() / CLOCKS_PER_SEC - startTime << endl;
//     }
//     cout << "Average runtime: " << totalTime / TRIALS << endl;



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
