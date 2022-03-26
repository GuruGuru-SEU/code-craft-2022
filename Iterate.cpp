//
// Created by tth37 on 2022/3/23.
//

#include "Iterate.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

// bool lock95[9000][200];
// int tot[200];

// int sortTmp[9000];
// int thres[200];
// bool vis[200];

// double gmm[200]; // gamma

// void iterate95per_init(int N, const int C[], int fake_C[]) {for (int j = 1; j <= N; j++) gmm[j] = 0.99, vis[j] = false, fake_C[j] = C[j];}

// void iterate95per(int X[][40][200], int D[][40], int fake_C[], int Y[][200],
//                   int T, int M, int N, int Q) {
//     int pos = ceil(0.95 * T);
//     for (int j = 1; j <= N; ++j) {
//         for (int t = 1; t <= T; ++t) {
//             sortTmp[t] = 0;
//             for (int i = 1; i <= M; ++i)
//                 sortTmp[t] += X[t][i][j];
//         }
//         sort(sortTmp + 1, sortTmp + T + 1);
//         if (vis[j] && sortTmp[j] >= thres[j]) gmm[j] = 1.0;
//         thres[j] = sortTmp[pos]; vis[j] = true;
//         for (int t = 1; t <= T; ++t) {
//             int w = 0;
//             for (int i = 1; i <= M; ++i)
//                 w += X[t][i][j];
//             if (w >= thres[j])
//                 lock95[t][j] = true;
//         }
//     }
//     for (int j = 1; j <= N; j++)
//         fake_C[j] = gmm[j] * thres[j];
//     for (int t = 1; t <= T; t++)
//         for (int j = 1; j <= N; j++)
//             if (!lock95[t][j])
//                 for (int i = 1; i <= M; i++)
//                     if (Y[i][j] < Q)
//                         X[t][i][j] /= 2;
// }

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