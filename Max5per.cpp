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

double sqrt_C[1000010];

int MMask[40], NMask[200], TMask[9000];

inline void shuffleTMask(int TMask[], int T) { shuffle(TMask + 1, TMask + T + 1, rng); }

inline void shuffleMMask(int MMask[], int M) { shuffle(MMask + 1, MMask + M + 1, rng); }

inline void shuffleNMask(int NMask[], int N) { shuffle(NMask + 1, NMask + N + 1, rng); }

inline int LOG2(unsigned int x) {
    static const int tb[32] = {0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31};
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return tb[x * 0x07C4ACDDu >> 27];
}

void max5per(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
    int demand[200];
    for (int i = 0; i <= 1e6; i++)
        sqrt_C[i] = sqrt((double)i + 2.0);
    memset(used, 0, sizeof used); // T*5%
    memcpy(remain, D, sizeof remain);
    bool flag = true;
    int threshold = T - ceil((double)T * 0.95); // last 5%
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
                if (used[j] < threshold && !mark95[t][j] && siteRemain[t][j] * sqrt_C[C[j]] > maxn) {
                    maxn = siteRemain[t][j] * sqrt_C[C[j]];
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
                MMask[i] = i;
            shuffleMMask(MMask, M);
            for (int ii = 1; ii <= M; ii++) {
                int i = MMask[ii];
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
            }
            for (int ii = 1; ii <= M; ii++) {
                int i = MMask[ii];
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
            }
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (Y[i][j] < Q && j != maxj)
                        siteRemain[maxt][j] += remain[maxt][i];
        }
    }
}

long long siteAvail[9000][200], sum[9000][40];
int his[200];

int beta = 128;

void avg95perPart1(int X[][40][200], int D[][40], const int C[], int Y[][200],
                   int T, int M, int N, int Q) {
    for (int t = 1; t <= T; t++) {
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

    memset(siteAvail, 0, sizeof siteAvail);
    memset(sum, 0, sizeof sum);
    for (int t = 1; t <= T; t++)
        for (int j = 1; j <= N; j++)
            if (!mark95[t][j])
                for (int i = 1; i <= M; i++)
                    if (Y[i][j] < Q)
                        siteAvail[t][j] += remain[t][i];
    for (int t = 1; t <= T; t++)
        for (int j = 1; j <= N; j++)
            if (!mark95[t][j])
                for (int i = 1; i <= M; i++)
                    if (Y[i][j] < Q)
                        sum[t][i] += siteRemain[t][j] * LOG2(siteAvail[t][j] + 2);
    for (int i = 1; i <= M; i++)
        for (int t = 1; t <= T; t++)
            for (int j = 1; j <= N; j++)
                if (!mark95[t][j] && Y[i][j] < Q)
                    X[t][i][j] += (long long)remain[t][i] * (siteRemain[t][j] * LOG2(siteAvail[t][j] + 2)) / sum[t][i];
}

double alpha_0 = 0.95;

long long baseLine[200], cap[9000][200];
int cnt[200];

int randInt(long long upper) { return rng() % (upper + 1); }

void avg95perPart2(int X[][40][200], int D[][40], const int C[], int Y[][200],
                   int T, int M, int N, int Q) {
    for (int t = 1; t <= T; t++)
        TMask[t] = t;
    for (int i = 1; i <= M; ++i)
        MMask[i] = i;
    for (int j = 1; j <= N; ++j)
        NMask[j] = j;

    int Shis[200];
    for (int j = 1; j <= N; ++j)
        Shis[j] = C[j] / 8192 + 1ll;

    shuffleTMask(TMask, T);
    for (int tt = 1; tt <= T; ++tt) {
        int t = TMask[tt];
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
        int Scap[200];
        long long Ssum = 0;
        for (int j = 1; j <= N; ++j) {
            Scap[j] = 0;
            Ssum += Shis[j];
        }
        shuffleMMask(MMask, M);
        for (int imask = 1; imask <= M; ++imask) {
            int i = MMask[imask];
            int w = remain[t][i];
            shuffleNMask(NMask, N);
            for (int jmask = 1; jmask <= N; ++jmask) {
                int j = NMask[jmask];
                if (Y[i][j] < Q) {
                    int x = min(siteRemain[t][j] - Scap[j], min(w, randInt(2ll * Shis[j] * w / Ssum)));
                    X[t][i][j] += x;
                    w -= x;
                    Scap[j] += x;
                }
            }
            while (w) {
                for (int j = 1; j <= N; ++j) {
                    if (Y[i][j] < Q) {
                        int x = min(siteRemain[t][j] - Scap[j],
                                    min(w, randInt(64ll * Shis[j] * w / Ssum + w)));
                        X[t][i][j] += x;
                        w -= x;
                        Scap[j] += x;
                    }
                }
            }
        }
        for (int j = 1; j <= N; ++j)
            if (Shis[j] < Scap[j]) Shis[j] = Scap[j];
    }

    double alpha = alpha_0;
    for (int TT = 0; TT < 3; TT++) {
        memset(his, 0, sizeof his);
        for (int t = 1; t <= T; t++)
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    if (!mark95[t][j]) {
                        int sum = 0;
                        for (int i = 1; i <= M; ++i)
                            sum += X[t][i][j];
                        if (his[j] < sum) his[j] = sum;
                    }
        memset(cap, 0, sizeof cap);
        memset(baseLine, 0, sizeof baseLine);
        for (int t = 1; t <= T; t++)
            for (int i = 1; i <= M; i++)
                for (int j = 1; j <= N; j++)
                    cap[t][j] += X[t][i][j];
        for (int j = 1; j <= N; j++)
            for (int t = 1; t <= T; t++)
                if (cap[t][j])
                    cnt[j]++;
        for (int j = 1; j <= N; j++)
            baseLine[j] = his[j] * alpha;
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
        alpha *= alpha_0;
    }
}