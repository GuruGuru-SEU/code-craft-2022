#include "Max5per.h"
#include "RandCusXArr.h"
#include "utils/Benchmark.h"
#include "utils/FileIO.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

int T, M, N, Q, C[200], Y[40][200], X[9000][40][200], Xans[9000][40][200],
    ansBenchmark = 1e9;
int D[9000][40];

string clientName[40], siteName[200];

int main() {
  srand(time(nullptr));

  readDemand(D, clientName, M, T);
  readCap(C, siteName, N);
  readY(Y, M, N);
  readQ(Q);

  /*
   * Run Max-5-per algorithm.
   */
  memset(X, 0, sizeof(X));
  max5per(X, D, C, Y, T, M, N, Q);

  /*
   * Run Hao-Zhang algorithm.
   */
  max5perPart2(X, D, C, Y, T, M, N, Q);

  runJudger(X, D, C, Y, T, M, N, Q);
  cout << "\nAns Benchmark: " << runBenchmark(X, T, M, N) << endl;

  printAns(clientName, siteName, X, T, M, N);
  return 0;
}
