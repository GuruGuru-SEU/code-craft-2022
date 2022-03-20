//
// Created by tth37 on 2022/3/20.
//

#include "Max5per.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <set>

using namespace std;

struct Node {
    int val, t, j;
    bool operator<(const Node &node) const {
        if (val != node.val)
            return val > node.val;
        if (t != node.t)
            return t < node.t;
        return j < node.j;
    }
};

int maxBand[9000][200];
bool deletedFromSet[9000][200];
int used[200];
int remain[9000][40];
int siteRemain[9000][200];
set<Node> st;

void max5per(int X[][40][200], int D[][40], const int C[], int Y[][200], int T,
             int M, int N, int Q) {
    for (int t = 1; t <= T; ++t) {
        for (int j = 1; j <= N; ++j) {
            maxBand[t][j] = 0;
            deletedFromSet[t][j] = false;
            for (int i = 1; i <= M; ++i) {
                if (Y[i][j] < Q)
                    maxBand[t][j] += D[t][i];
            }
        }
    }
    // passed unit test.
    st.clear();
    for (int t = 1; t <= T; ++t) {
        for (int j = 1; j <= N; ++j) {
            struct Node tmp {};
            tmp.val = min(C[j], maxBand[t][j]);
            tmp.t = t;
            tmp.j = j;
            st.insert(tmp);
        }
    }
    // passed unit test.

    // FIXME: Remember to modify limit to 5% point!!
    int pos = ceil(0.95 * T);
    int limit = T - pos;

    for (int j = 1; j <= N; ++j)
        used[j] = 0;
    for (int t = 1; t <= T; ++t)
        for (int i = 1; i <= M; ++i)
            remain[t][i] = D[t][i];
    for (int t = 1; t <= T; ++t)
        for (int j = 1; j <= N; ++j)
            siteRemain[t][j] = C[j];

    while (!st.empty()) {

        auto tp = st.begin();

        if (deletedFromSet[tp->t][tp->j]) {
            st.erase(tp);
            continue;
        }
        deletedFromSet[tp->t][tp->j] = true;

        ++used[tp->j];

        // cout << "ISP No." << tp->j << " has chosen " << used[tp->j] << " times.";
        // cout << "Bandwidth = " << tp->val << endl;

        if (used[tp->j] == limit) {
            for (int t = 1; t <= T; ++t)
                deletedFromSet[t][tp->j] = true;
        }

        for (int i = M; i >= 1; --i) {
            if (Y[i][tp->j] >= Q)
                continue;
            if (siteRemain[tp->t][tp->j] >= remain[tp->t][i]) {
                siteRemain[tp->t][tp->j] -= remain[tp->t][i];
                X[tp->t][i][tp->j] += remain[tp->t][i];
                remain[tp->t][i] = 0;
            } else {
                remain[tp->t][i] -= siteRemain[tp->t][tp->j];
                X[tp->t][i][tp->j] += siteRemain[tp->t][tp->j];
                siteRemain[tp->t][tp->j] = 0;
                break;
            }
        }
        for (int j = 1; j <= N; ++j) {
            if (deletedFromSet[tp->t][j])
                continue;
            struct Node tmp {};
            tmp.val = min(C[j], maxBand[tp->t][j]);
            tmp.t = tp->t;
            tmp.j = j;
            auto cur = st.find(tmp);
            if (cur == st.end())
                continue;
            st.erase(cur);
        }
        for (int j = 1; j <= N; ++j) {
            if (deletedFromSet[tp->t][j])
                continue;
            maxBand[tp->t][j] = 0;
            for (int i = 1; i <= M; ++i) {
                if (Y[i][j] >= Q)
                    continue;
                maxBand[tp->t][j] += remain[tp->t][i];
            }
            struct Node tmp {};
            tmp.val = min(C[j], maxBand[tp->t][j]);
            tmp.t = tp->t;
            tmp.j = j;
            st.insert(tmp);
        }
        st.erase(tp);
    }
    // passed unit test.
}

void max5perPart1(int X[][40][200], int D[][40], const int C[], int Y[][200],
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
bool mark95[9000][200];

void max5perPart2(int X[][40][200], int D[][40], const int C[], int Y[][200],
                  int T, int M, int N, int Q) {
    memset(mark95, false, sizeof mark95);
    for (int t = 1; t <= T; ++t) {
        for (int j = 1; j <= N; ++j) {
            siteRemain[t][j] = C[j];
            for (int i = 1; i <= M; ++i) {
                siteRemain[t][j] -= X[t][i][j];
                if (X[t][i][j]) mark95[t][j] = true;
            }
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

void max5perPart2v2(int X[][40][200], int D[][40], const int C[], int Y[][200],
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
        for (int i = 1; i <= M; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (Y[i][j] >= Q)
                    continue;
                if (siteRemain[t][j] >= remain[t][i]) {
                    siteRemain[t][j] -= remain[t][i];
                    X[t][i][j] += remain[t][i];
                    remain[t][i] = 0;
                    break;
                } else {
                    remain[t][i] -= siteRemain[t][j];
                    X[t][i][j] += siteRemain[t][j];
                    siteRemain[t][j] = 0;
                }
            }
        }
    }
}