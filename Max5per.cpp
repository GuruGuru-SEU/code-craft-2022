//
// Created by tth37 on 2022/3/20.
//

#include "Max5per.h"
#include <cstring>
#include <iostream>

using namespace std;

void max5per(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
  int maxBand[9000][200];
  for (int t = 1; t <= T; ++t) {
    for (int j = 1; j <= N; ++j) {
      maxBand[t][j] = 0;
      for (int i = 1; i <= M; ++i) {
        if (Y[i][j] < Q)
          maxBand[t][j] += D[t][i];
      }
      cout << t << " " << j << " " << maxBand[t][j] << endl;
    }
  }
}