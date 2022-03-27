#include "Iterate.hpp"
#include "Max5per.hpp"
#include "utils/Benchmark.hpp"
#include "utils/FileIO.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <omp.h>

using namespace std;

int T, M, N, Q, C[200], Y[40][200], X[9000][40][200], D[9000][40];
int Xans[9000][40][200];

string clientName[40], siteName[200];

int main() {
    srand(time(nullptr));

    readDemand(D, clientName, M, T);
    readCap(C, siteName, N);
    readY(Y, M, N);
    readQ(Q);

    memset(X, 0, sizeof(X));

    max5per(X, D, C, Y, T, M, N, Q);
    avg95perPart1(X, D, C, Y, T, M, N, Q);
    avg95perPart2(X, D, C, Y, T, M, N, Q);

    int ansScore = runBenchmark(X, T, M, N);
    overwriteAns(X, Xans, T, M, N);

    for (int iter = 1; iter <= 10; iter++) {
        iterate(X, D, C, Y, T, M, N, Q);
        avg95perPart1(X, D, C, Y, T, M, N, Q);
        avg95perPart2(X, D, C, Y, T, M, N, Q);
        int curScore = runBenchmark(X, T, M, N);
        if (curScore < ansScore) {
            ansScore = curScore;
            overwriteAns(X, Xans, T, M, N);
        }
    }

    runJudger(Xans, D, C, Y, T, M, N, Q);
    cout << "\nAns Benchmark: " << runBenchmark(Xans, T, M, N) << endl;

    printAns(clientName, siteName, Xans, T, M, N);
    return 0;
}