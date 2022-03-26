#include "Max5per.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <set>

using namespace std;

mt19937 rng(time(nullptr));

int remain[9000][40];
int siteRemain[9000][200];
bool mark95[9000][200];
int used[200]; // T*5%

double log_C[1000010];

void max5per(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
    int demand[200];
    for (int i = 1; i <= 1e6; i++)
        log_C[i] = log((double)i + 2.0);
    memset(used, 0, sizeof used); // T*5%
    memcpy(remain, D, sizeof remain);
    bool flag = true;
    int threshold = floor((double)T * 0.05); // last 5%
    for (int t = 1; t <= T; t++)
        for (int i = 1; i <= M; i++)
            for (int j = 1; j <= N; j++)
                if (Y[i][j] < Q)
                    siteRemain[t][j] += remain[t][i];
    while (flag) {
        flag = false;
        int maxn = 0, maxj = 1, maxt = 1;
        for (int t = 1; t <= T; t++)
            for (int j = 1; j <= N; j++)
                if (used[j] < threshold && !mark95[t][j] && siteRemain[t][j] * log_C[C[j]] > maxn) {
                    maxn = siteRemain[t][j] * log_C[C[j]];
                    maxj = j;
                    maxt = t;
                }
        if (maxn) {
            flag = true;
            mark95[maxt][maxj] = true;
            used[maxj]++;
            int isp = C[maxj];
            for (int i = 1; i <= M; i++)
                demand[i] = remain[maxt][i];
            int l = -1, r = 1e6;
            while (l + 1 < r) {
                int mid = (l + r) >> 1;
                int tmp_isp = isp;
                for (int i = 1; i <= M; i++)
                    if (Y[i][maxj] < Q && demand[i] > mid)
                        tmp_isp -= demand[i] - mid;
                if (tmp_isp >= 0)
                    r = mid;
                else
                    l = mid;
            }
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (Y[i][j] < Q) {
                        if (j == maxj)
                            continue;
                        siteRemain[maxt][j] -= remain[maxt][i];
                    }
            for (int i = 1; i <= M; i++)
                if (Y[i][maxj] < Q && demand[i] > r) {
                    int nd = demand[i] - r;
                    demand[i] = r;
                    if (isp > nd) {
                        X[maxt][i][maxj] += nd;
                        isp -= nd;
                        remain[maxt][i] -= nd;
                    } else {
                        X[maxt][i][maxj] += isp;
                        remain[maxt][i] -= isp;
                        isp = 0;
                        break; // used up
                    }
                }
            for (int i = 1; i <= M; i++)
                if (Y[i][maxj] < Q && demand[i]) {
                    if (isp > demand[i]) {
                        X[maxt][i][maxj] += demand[i];
                        isp -= demand[i];
                        remain[maxt][i] = 0;
                    } else {
                        X[maxt][i][maxj] += isp;
                        remain[maxt][i] -= isp;
                        isp = 0;
                        break; // used up
                    }
                }
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (Y[i][j] < Q && j != maxj)
                        siteRemain[maxt][j] += remain[maxt][i];
        }
    }
}

void max95per(int X[][40][200], int D[][40], const int C[], int Y[][200],
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
}

long long baseLine[200], cap[9000][200];

inline void shuffleMMask(int MMask[], int M) { shuffle(MMask + 1, MMask + M + 1, rng); }

inline void shuffleNMask(int NMask[], int N) { shuffle(NMask + 1, NMask + N + 1, rng); }

int randInt(long long upper) { return rng() % (upper + 1); }

void avg95per(int X[][40][200], int D[][40], const int C[], int Y[][200],
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

    int MMask[40], NMask[200];
    for (int i = 1; i <= M; ++i)
        MMask[i] = i;
    for (int j = 1; j <= N; ++j)
        NMask[j] = j;

    long long sum[200];
    for (int j = 1; j <= N; ++j)
        sum[j] = C[j] / 10000 + 1ll;
    for (int t = 1; t <= T; ++t) {
        int Scap[200];
        long long Ssum = 0;
        bool flag = false;
        for (int j = 1; j <= N; ++j)
            if (sum[j] > (1ll << 24))
                flag = true;
        if (flag)
            for (int j = 1; j <= N; j++)
                sum[j] >>= 1;
        for (int j = 1; j <= N; ++j) {
            Scap[j] = 0;
            Ssum += sum[j];
        }
        shuffleMMask(MMask, M);
        for (int imask = 1; imask <= M; ++imask) {
            int i = MMask[imask];
            int w = remain[t][i];
            shuffleNMask(NMask, N);
            for (int jmask = 1; jmask <= N; ++jmask) {
                int j = NMask[jmask];
                if (Y[i][j] < Q) {
                    int x = min(siteRemain[t][j] - Scap[j], min(w, randInt(2ll * sum[j] * w / Ssum)));
                    X[t][i][j] += x;
                    w -= x;
                    Scap[j] += x;
                }
            }
            while (w) {
                for (int j = 1; j <= N; ++j) {
                    if (Y[i][j] < Q) {
                        int x = min(siteRemain[t][j] - Scap[j],
                                    min(w, randInt(128ll * sum[j] * w / Ssum + w)));
                        X[t][i][j] += x;
                        w -= x;
                        Scap[j] += x;
                    }
                }
            }
        }
        for (int j = 1; j <= N; ++j)
            sum[j] += Scap[j];
    }

    double alpha = 0.8;
    memset(cap, 0, sizeof cap);
    memset(baseLine, 0, sizeof baseLine);
    for (int t = 1; t <= T; t++)
        for (int i = 1; i <= M; i++)
            for (int j = 1; j <= N; j++) {
                cap[t][j] += X[t][i][j];
                baseLine[j] += X[t][i][j];
            }
    for (int j = 1; j <= N; j++) {
        baseLine[j] = (double)baseLine[j] * alpha / T;
        if (baseLine[j] > C[j]) baseLine[j] = C[j];
        if (baseLine[j] < 0) baseLine[j] = 0;
    }
    for (int t = 1; t <= T; t++) {
        for (int jh = 1; jh <= N; jh++)
            if (cap[t][jh] > baseLine[jh] && !mark95[t][jh]) {
                for (int i = 1; i <= M; i++)
                    if (Y[i][jh] < Q) {
                        for (int jt = 1; jt <= N; jt++)
                            if (Y[i][jt] < Q) {
                                if (cap[t][jt] < baseLine[jt]) {
                                    int delta = min(
                                        min((long long)X[t][i][jh], baseLine[jt] - cap[t][jt]),
                                        cap[t][jh] - baseLine[jh]);
                                    cap[t][jh] -= delta;
                                    cap[t][jt] += delta;
                                    X[t][i][jh] -= delta;
                                    X[t][i][jt] += delta;
                                }
                                if (mark95[t][jt]) {
                                    int delta =
                                        min(min((long long)X[t][i][jh], C[jt] - cap[t][jt]),
                                            cap[t][jh] - baseLine[jh]);
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