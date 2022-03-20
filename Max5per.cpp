//
// Created by tth37 on 2022/3/20.
//

#include "Max5per.h"
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
      return t > node.t;
    return j > node.j;
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

    //   cout << "ISP No." << tp->j << " has chosen " << used[tp->j] << " times.
    //   "; cout << "Bandwidth = " << tp->val << endl;

    if (used[tp->j] == limit) {
      for (int t = 1; t <= T; ++t)
        deletedFromSet[t][tp->j] = true;
    }
    for (int i = 1; i <= M; ++i) {
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
  //  for (int i = 1; i <= M; ++i)
  //    cout << remain[1][i] << ' ';
  //  cout << endl;
  for (int t = 1; t <= T; ++t) {
    for (int i = 1; i <= M; ++i) {
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
        continue;     // get a Runtime Error.
      }
      int rem = remain[t][i], cntt = 0;
      for (int j = 1; j <= N; ++j) {
        if (Y[i][j] >= Q)
          continue;
        if (!siteRemain[t][j])
          continue;
        ++cntt;
        if (cntt <= (rem % cnt)) {
          X[t][i][j] += rem / cnt + 1;
        } else {
          X[t][i][j] += rem / cnt;
        }
      }
    }
  }
}