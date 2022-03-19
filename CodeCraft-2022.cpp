#include "RandCusXArr.h"
#include "utils/Benchmark.h"
#include "utils/FileIO.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

using namespace std;

int T, M, N, Q, C[200], D[9000][40], Y[40][200], X[9000][40][200],
    Xans[9000][40][200], ansBenchmark = 1e9;

string clientName[40], siteName[200];

int getIPSBandwidth(int jIPS, int t) {
  int res = 0;
  for (int i = 1; i <= M; ++i)
    res += X[t][i][jIPS];
  return res;
}

int getIPS95t(int jIPS) {
  int pos = ceil(0.95 * T);
  pair<int, int> ts[9000];
  for (int t = 1; t <= T; ++t) {
    ts[t].first = getIPSBandwidth(jIPS, t);
    ts[t].second = t;
  }
  sort(ts + 1, ts + T + 1);
  return ts[pos].second;
}

int getIPS95tRough(int jIPS) {
  int pos = ceil(0.95 * T);
  pair<int, int> ts[9000];
  for (int t = 1; t <= T; ++t) {
    ts[t].first = getIPSBandwidth(jIPS, t);
    ts[t].second = t;
  }
  sort(ts + 1, ts + T + 1);
  if (rand() % 5)
    return ts[pos].second;
  int l = pos, r = T;
  return l + rand() % (r - l + 1);
}

int getFreeFlow(int jIPS, int t) {
  int t95 = getIPS95t(jIPS);
  int stdBandwidth = getIPSBandwidth(jIPS, t95);
  int curBandwidth = getIPSBandwidth(jIPS, t);
  if (curBandwidth > stdBandwidth)
    return C[jIPS] - curBandwidth;
  else
    return stdBandwidth - curBandwidth;
}

void looseIPS(int jIPS) {
  int t = getIPS95tRough(jIPS);
  int MMask[40], NMask[200];
  for (int i = 1; i <= M; ++i)
    MMask[i] = i;
  for (int j = 1; j <= N; ++j)
    NMask[j] = j;
  random_shuffle(MMask + 1, MMask + M + 1);
  random_shuffle(NMask + 1, NMask + N + 1);
  for (int imask = 1; imask <= M; ++imask) {
    int i = MMask[imask];
    if (!X[t][i][jIPS])
      continue;
    for (int jmask = 1; jmask <= N; ++jmask) {
      int jTAR = NMask[jmask];
      if (Y[i][jTAR] >= Q)
        continue;
      int freeFlow = getFreeFlow(jTAR, t);
      int flow = min(freeFlow, X[t][i][jIPS]);
      if (!flow)
        continue;
      X[t][i][jTAR] += flow;
      X[t][i][jIPS] -= flow;
    }
  }
}

int main() {
  srand(time(nullptr));

  readDemand(D, clientName, M, T);
  readCap(C, siteName, N);
  readY(Y, M, N);
  readQ(Q);

  //  for (int i = 1; i <= 10; ++i) {
  //    memset(X, 0, sizeof(X));
  //    assign(X, D, C, Y, T, M, N, Q);
  //    int curBenchmark = runBenchmark(X, T, M, N);
  //    cout << "Round " << i << " Benchmark: " << curBenchmark << endl;
  //    if (curBenchmark < ansBenchmark) {
  //      ansBenchmark = curBenchmark;
  //      overwriteAns(X, Xans, T, M, N);
  //    }
  //  }

  /*
   * Run rand-cus-x-arr Algorithm.
   */
  memset(X, 0, sizeof(X));
  assign(X, D, C, Y, T, M, N, Q);

  for (int j = 1; j <= 80000; ++j) {

    looseIPS(rand() % N + 1);
    cout << runBenchmark(X, T, M, N) << endl;
  }

  runJudger(X, D, C, Y, T, M, N, Q);
  cout << "\nAns Benchmark: " << runBenchmark(X, T, M, N) << endl;

  printAns(clientName, siteName, Xans, T, M, N);
  return 0;
}
