#include "Iterate.h"
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

int T, M, N, Q, C[200], Y[40][200], X[9000][40][200], D[9000][40];
int Xans[9000][40][200];

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
  // assign(X, D, C, Y, T, M, N, Q);

  max5per(X, D, C, Y, T, M, N, Q);

  // iterate5per(X, D, C, Y, T, M, N, Q);

  /*
   * Run Jesus's algorithm.
   */
  max5perPart1(X, D, C, Y, T, M, N, Q);
  max5perPart2(X, D, C, Y, T, M, N, Q);

  int ansScore = runBenchmark(X, T, M, N);
  overwriteAns(X, Xans, T, M, N);

  for (int i = 1; i <= 50; ++i) {
    iterate(X, D, C, Y, T, M, N, Q);
    max5perPart2(X, D, C, Y, T, M, N, Q);
    int curScore = runBenchmark(X, T, M, N);
    if (curScore < ansScore) {
      ansScore = curScore;
      overwriteAns(X, Xans, T, M, N);
    }
  }

  // iterate95per(X, D, C, Y, T, M, N, Q);
  runJudger(Xans, D, C, Y, T, M, N, Q);
  cout << "\nAns Benchmark: " << runBenchmark(X, T, M, N) << endl;

  printAns(clientName, siteName, Xans, T, M, N);
  return 0;
}
