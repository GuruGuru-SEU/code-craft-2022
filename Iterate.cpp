//
// Created by tth37 on 2022/3/23.
//

#include "Iterate.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

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
                X[t][i][j] -= (X[t][i][j] >> 2);
        }
    }
}