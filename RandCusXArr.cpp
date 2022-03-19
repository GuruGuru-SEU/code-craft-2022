//
// Created by tth37 on 2022/3/19.
//

#include "RandCusXArr.h"
#include <algorithm>

using namespace std;

void shuffleMMask(int MMask[], int M) {
  random_shuffle(MMask + 1, MMask + M + 1);
}

void shuffleNMask(int NMask[], int N) {
  random_shuffle(NMask + 1, NMask + N + 1);
}

int randInt(long long upper) {
  return ((rand() << 16ll) + (long long)rand()) % (upper + 1);
}

void overwriteAns(int X[][40][200], int Xans[][40][200], int T, int M, int N) {
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        Xans[t][i][j] = X[t][i][j];
}

void assign(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
            int M, int N, int Q) {
  int MMask[40], NMask[200];
  for (int i = 1; i <= M; ++i)
    MMask[i] = i;
  for (int j = 1; j <= N; ++j)
    NMask[j] = j;

  long long sum[200];
  for (int j = 1; j <= N; ++j)
    sum[j] = C[j] / 10000 + 1ll;
  for (int t = 1; t <= T; ++t) {
    int cap[200];
    long long Ssum = 0;
    bool flag = false;
    for (int j = 1; j <= N; ++j)
      if (sum[j] > (1ll << 24))
        flag = true;
    if (flag)
      for (int j = 1; j <= N; j++)
        sum[j] >>= 1;
    for (int j = 1; j <= N; ++j) {
      cap[j] = 0;
      Ssum += sum[j];
    }
    shuffleMMask(MMask, M);
    for (int imask = 1; imask <= M; ++imask) {
      int i = MMask[imask];
      int w = D[t][i];
      shuffleNMask(NMask, N);
      for (int jmask = 1; jmask <= N; ++jmask) {
        int j = NMask[jmask];
        if (Y[i][j] < Q) {
          int x = min(C[j] - cap[j], min(w, randInt(2ll * sum[j] * w / Ssum)));
          X[t][i][j] += x;
          w -= x;
          cap[j] += x;
        }
      }
      while (w) {
        for (int j = 1; j <= N; ++j) {
          if (Y[i][j] < Q) {
            int x = min(C[j] - cap[j],
                        min(w, randInt(128ll * sum[j] * w / Ssum + w)));
            // int x = min(C[j] - cap[j], min(w, randInt(1000000)));
            X[t][i][j] += x;
            w -= x;
            cap[j] += x;
          }
        }
      }
    }
    for (int j = 1; j <= N; ++j)
      sum[j] += cap[j];
  }
}