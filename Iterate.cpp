//
// Created by tth37 on 2022/3/23.
//

#include "Iterate.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

bool lock[9000][200]; // >=95%, lock
int tot[200];

int sortTmp[9000];

int f(int x, int y) { return (x - y) / 2; }

void iterate95per(int X[][40][200], int D[][40], const int C[], int Y[][200],
                  int T, int M, int N, int Q) {
  int pos = ceil(0.95 * T);
  for (int j = 1; j <= N; ++j) {
    for (int t = 1; t <= T; ++t) {
      sortTmp[t] = 0;
      for (int i = 1; i <= M; ++i)
        sortTmp[t] += X[t][i][j];
    }
    sort(sortTmp + 1, sortTmp + T + 1);
    int thres = sortTmp[pos];
    for (int t = 1; t <= T; ++t) {
      int w = 0;
      for (int i = 1; i <= M; ++i)
        w += X[t][i][j];
      if (w >= thres)
        lock[t][j] = true;
    }
  }
  for (int j = 1; j <= N; ++j) {
    tot[j] = 0;
    for (int t = 1; t <= T; ++t)
      for (int i = 1; i <= M; ++i)
        tot[j] += X[t][i][j];
  }

  for (int it = 1; it <= 10000000; ++it) {
    int t = rand() % T + 1;
    int j1 = rand() % N + 1, j2 = rand() % N + 1;
    if (j1 == j2)
      continue;
    int i = rand() % M + 1;
    if (lock[t][j1] || lock[t][j2])
      continue;
    if (!X[t][i][j1] || !X[t][i][j2])
      continue;
    //   ++cnt;
    int w1 = 0, w2 = 0;
    int avg1 = tot[j1] / T, avg2 = tot[j2] / T;
    for (int ii = 1; ii <= M; ++ii)
      w1 += X[t][ii][j1], w2 += X[t][ii][j2];
    int d1 = w1 - avg1, d2 = w2 - avg2;
    //  cout << d1 << ' ' << d2 << endl;
    int trans = f(d1, d2);
    if (trans == 0)
      continue;
    if (trans > 0) {
      trans = rand() * rand() % (trans + 1);
      trans = min(trans, C[j2] - w2);
      trans = min(trans, X[t][i][j1]);
      trans = max(trans, 0);
      X[t][i][j1] -= trans;
      X[t][i][j2] += trans;
      tot[j1] -= trans;
      tot[j2] += trans;
    } else {
      trans = -trans;
      trans = rand() * rand() % (trans + 1);
      trans = min(trans, C[j1] - w1);
      trans = min(trans, X[t][i][j2]);
      trans = max(trans, 0);
      X[t][i][j2] -= trans;
      X[t][i][j1] += trans;
      tot[j2] -= trans;
      tot[j1] += trans;
    }
  }
}

int usedd[9000][40];

void iterate5per(int X[][40][200], int D[][40], const int C[], int Y[][200],
                 int T, int M, int N, int Q) {
  for (int j = 1; j <= N; ++j)
    for (int i = 1; i <= M; ++i)
      for (int t = 1; t <= T; ++t)
        X[t][i][j] = 0.9 * X[t][i][j];
}

pair<int, int> sortPair[9000];

void iterate(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
  int pos = ceil(0.95 * T);
  for (int j = 1; j <= N; ++j) {
    for (int t = 1; t <= T; ++t) {
      sortPair[t].first = 0;
      sortPair[t].second = t;
      for (int i = 1; i <= M; ++i)
        sortPair[t].first += X[t][i][j];
    }
    sort(sortPair + 1, sortPair + T + 1);
    for (int tt = 0.85 * pos; tt <= pos; ++tt) {
      int t = sortPair[tt].second;
      for (int i = 1; i <= M; ++i)
        X[t][i][j] /= 2;
    }
  }
}