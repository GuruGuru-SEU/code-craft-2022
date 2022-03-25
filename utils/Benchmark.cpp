//
// Created by tth37 on 2022/3/19.
//

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

void runJudger(const int X[][40][200], const int D[][40], const int C[],
               const int Y[][200], int T, int M, int N, int Q) {
  cout << "Checking Constraint 1 ..." << endl;
  bool flag = true;
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        if (X[t][i][j] < 0)
          flag = false;
  if (!flag)
    cout << "Failed Constraint 1." << endl, exit(-1);
  else
    cout << "Passed Constraint 1." << endl;

  cout << "Checking Constraint 2 ..." << endl;
  flag = true;
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        if (Y[i][j] >= Q)
          if (X[t][i][j] != 0)
            flag = false;
  if (!flag)
    cout << "Failed Constraint 2." << endl, exit(-1);
  else
    cout << "Passed Constraint 2." << endl;

  cout << "Checking Constraint 3 ..." << endl;
  flag = true;
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i) {
      int sumX = 0;
      for (int j = 1; j <= N; ++j)
        sumX += X[t][i][j];
      if (sumX != D[t][i])
        flag = false;
    }
  if (!flag)
    cout << "Failed Constraint 3." << endl, exit(-1);
  else
    cout << "Passed Constraint 3." << endl;

  cout << "Checking Constraint 4 ..." << endl;
  flag = true;
  for (int t = 1; t <= T; ++t)
    for (int j = 1; j <= N; ++j) {
      int sumX = 0;
      for (int i = 1; i <= M; ++i)
        sumX += X[t][i][j];
      if (sumX > C[j])
        flag = false;
    }
  if (!flag)
    cout << "Failed Constraint 4." << endl, exit(-1);
  else
    cout << "Passed Constraint 4." << endl;
}

int runBenchmark(int X[][40][200], int T, int M, int N) {
  int res = 0, w[9000], pos = ceil(0.95 * T);
  for (int j = 1; j <= N; ++j) {
    for (int t = 1; t <= T; ++t) {
      w[t] = 0;
      for (int i = 1; i <= M; ++i)
        w[t] += X[t][i][j];
    }
    sort(w + 1, w + T + 1);
    res += w[pos];
  }
  return res;
}