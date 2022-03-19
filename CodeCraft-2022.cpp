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

int randInt(int);

void shuffleMMask();
void shuffleNMask();

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

  for (int i = 1; i <= 10; ++i) {
    memset(X, 0, sizeof(X));
    assign(X, D, C, Y, T, M, N, Q);
    int curBenchmark = runBenchmark(X, T, M, N);
    cout << "Round " << i << " Benchmark: " << curBenchmark << endl;
    if (curBenchmark < ansBenchmark) {
      ansBenchmark = curBenchmark;
      overwriteAns(X, Xans, T, M, N);
    }
  }

  /*
   * Run rand-cus-x-arr Algorithm.
   */
  memset(X, 0, sizeof(X));
  assign(X, D, C, Y, T, M, N, Q);

  runJudger(X, D, C, Y, T, M, N, Q);
  cout << "\nAns Benchmark: " << ansBenchmark << endl;

  printAns(clientName, siteName, Xans, T, M, N);
  return 0;
}
