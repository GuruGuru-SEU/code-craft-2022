//
// Created by tth37 on 2022/3/20.
//

#include "Max5per.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <set>
#include <map>

using namespace std;


int remain[9000][40];
int siteRemain[9000][200];
bool mark95[9000][200];
int used[200];  // 每个ISP在时域上被尽量使用的次数 T*5%

void max5per(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
    pair<int, int> demand[200];
    memset(used, 0, sizeof used); // 每个ISP在时域上被尽量使用的次数 T*5%
    memcpy(remain, D, sizeof remain);
    bool flag = true;
    int threshold = floor((double)T * 0.05); // 后5%
    for (int t = 1; t <= T; t++)
        for (int i = 1; i <= M; i++)
            for (int j = 1; j <= N; j++)
                if (Y[i][j] < Q)
                    siteRemain[t][j] += remain[t][i];
    while (flag) { // 如果全局能找到符合条件的继续循环
        flag = false;
        int maxn = 0, maxj, maxt;
        for (int t = 1; t <= T; t++)
            for (int j = 1; j <= N; j++)
                if (used[j] < threshold && !mark95[t][j] && siteRemain[t][j] > maxn) {
                    maxn = siteRemain[t][j];
                    maxj = j;
                    maxt = t;
                }
        if (maxn) {
            flag = true;
            mark95[maxt][maxj] = true;
            used[maxj]++;
            int isp = C[maxj];
            for (int i = 1; i <= M; i++)
                demand[i] = make_pair(remain[maxt][i], i);
            sort(demand + 1, demand + M + 1, [](pair<int, int> x, pair<int, int> y) { return x.first > y.first; });
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (Y[i][j] < Q) {
                        if (j == maxj) continue;
                        siteRemain[maxt][j] -= remain[maxt][i];
                    }
            for (int i = 1; i <= M; i++) {
                int nd = demand[i].first;// 客户的需求
                int ci = demand[i].second;// 客户id=j
                if (Y[ci][maxj] >= Q) continue;
                if (!nd) break;
                if (isp > nd) {
                    X[maxt][ci][maxj] += nd;
                    isp -= nd;
                    remain[maxt][ci] = 0;
                }
                else {
                    X[maxt][ci][maxj] += isp;
                    remain[maxt][ci] -= isp;
                    isp = 0;
                    break; // 全部用完
                }
            }
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (Y[i][j] < Q) {
                        if (j == maxj) continue;
                        siteRemain[maxt][j] += remain[maxt][i];
                    }
        }
    }
}

void max5perPart1(int X[][40][200], int D[][40], const int C[], int Y[][200],
                  int T, int M, int N, int Q) {
    for (int t = 1; t <= T; ++t) {
        for (int j = 1; j <= N; ++j) {
            siteRemain[t][j] = C[j];
            for (int i = 1; i <= M; ++i) {
                siteRemain[t][j] -= X[t][i][j];
            }
        }
        for (int i = 1; i <= M; ++i) {
            remain[t][i] = D[t][i];
            for (int j = 1; j <= N; ++j)
                remain[t][i] -= X[t][i][j];
        }
    }

    for (int j = 1; j <= N; ++j) {
        int minn = 1e9, cur = 0;
        for (int t = 1; t <= T; ++t) {
            if (siteRemain[t][j] == 0)
                continue;
            cur = 0;
            for (int i = 1; i <= M; ++i) {
                if (Y[i][j] >= Q)
                    continue;
                cur += remain[t][i];
            }
            minn = min(minn, min(cur, siteRemain[t][j]));
        }

        for (int t = 1; t <= T; ++t) {
            if (siteRemain[t][j] == 0)
                continue;
            for (int i = M; i >= 1; --i) {
                if (Y[i][j] >= Q)
                    continue;
                if (remain[t][i] >= minn) {
                    siteRemain[t][j] -= minn;
                    remain[t][i] -= minn;
                    X[t][i][j] += minn;
                    minn = 0;
                    break;
                } else {
                    siteRemain[t][j] -= remain[t][i];
                    X[t][i][j] += remain[t][i];
                    minn -= remain[t][i];
                    remain[t][i] = 0;
                }
            }
        }
    }
}

long long baseLine[200], cap[9000][200];

void max5perPart2(int X[][40][200], int D[][40], const int C[], int Y[][200],
                  int T, int M, int N, int Q) {
    for (int t = 1; t <= T; ++t) {
        for (int j = 1; j <= N; ++j) {
            siteRemain[t][j] = C[j];
            for (int i = 1; i <= M; ++i)
                siteRemain[t][j] -= X[t][i][j];
        }
        for (int i = 1; i <= M; ++i) {
            remain[t][i] = D[t][i];
            for (int j = 1; j <= N; ++j)
                remain[t][i] -= X[t][i][j];
        }
    }

    for (int t = 1; t <= T; ++t) {

        for (int i = M; i >= 1; --i) {
            if (remain[t][i] == 0)
                continue;
            int cnt = 0;
            for (int j = 1; j <= N; ++j) {
                if (Y[i][j] >= Q)
                    continue;
                if (siteRemain[t][j])
                    cnt++;
            }
            if (cnt == 0) { // This means the algorithm failed, but I don't want to
                continue;   // get a Runtime Error.
            }
            int rem = remain[t][i];

            for (int j = N; j >= 1; --j) {
                if (Y[i][j] >= Q)
                    continue;
                if (!siteRemain[t][j])
                    continue;
                if (siteRemain[t][j] >= rem / cnt) {
                    siteRemain[t][j] -= rem / cnt;
                    X[t][i][j] += rem / cnt;
                    rem -= rem / cnt;
                    --cnt;
                } else {
                    siteRemain[t][j] = 0;
                    X[t][i][j] += siteRemain[t][j];
                    rem -= siteRemain[t][j];
                    --cnt;
                }
            }
        }
    }
    double alpha = 0.6;
    memset(cap, 0, sizeof cap);
    memset(baseLine, 0, sizeof baseLine);
    for (int t = 1; t <= T; t++)
        for (int i = 1; i <= M; i++)
            for (int j = 1; j <= N; j++) {
                cap[t][j] += X[t][i][j];
                baseLine[j] += X[t][i][j];
            }
    for (int j = 1; j <= N; j++)
        baseLine[j] = (double)baseLine[j] * alpha / T;
    for (int t = 1; t <= T; t++) {
        for (int jh = 1; jh <= N; jh++)
            if (cap[t][jh] > baseLine[jh] && !mark95[t][jh]) {
                for (int i = 1; i <= M; i++)
                    if (Y[i][jh] < Q) {
                        for (int jt = 1; jt <= N; jt++)
                            if (Y[i][jt] < Q) {
                                if (cap[t][jt] < baseLine[jt]) {
                                    int delta = min(min((long long)X[t][i][jh], baseLine[jt] - cap[t][jt]), cap[t][jh] - baseLine[jh]);
                                    cap[t][jh] -= delta;
                                    cap[t][jt] += delta;
                                    X[t][i][jh] -= delta;
                                    X[t][i][jt] += delta;
                                }
                                if (mark95[t][jt]) {
                                    int delta = min(min((long long)X[t][i][jh], C[jt] - cap[t][jt]), cap[t][jh] - baseLine[jh]);
                                    cap[t][jh] -= delta;
                                    cap[t][jt] += delta;
                                    X[t][i][jh] -= delta;
                                    X[t][i][jt] += delta;
                                }
                            }
                    }
            }
    }
}