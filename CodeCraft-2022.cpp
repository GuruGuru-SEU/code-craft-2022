#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

const bool LOCAL_DEBUG = true;

void readDemand();
void readCap();
void readY();
void readQ();

void overwriteAns();
void printAns();

int randInt(int);

void runJudger();
int runBenchmark();

void shuffleMMask();
void shuffleNMask();

int T, M, N, Q, D[9000][40], C[200], Y[40][200], X[9000][40][200],
    Xans[9000][40][200], ansBenchmark = 1e9;
int MMask[40], NMask[200];
string clientName[40], siteName[200];

void assign() {
  for (int t = 1; t <= T; ++t) {
    int cap[200];
    for (int j = 1; j <= N; ++j)
      cap[j] = C[j];
    shuffleMMask();
    for (int imask = 1; imask <= M; ++imask) {
      int i = MMask[imask];
      int w = D[t][i];
      int Scap = 0;
      for (int j = 1; j <= N; ++j)
        Scap += cap[j];
      shuffleNMask();
      for (int jmask = 1; jmask <= N; ++jmask) {
        int j = NMask[jmask];
        if (Y[i][j] < Q) {
          int x = min(cap[j], min(w, randInt(2.0 * cap[j] * w / Scap)));
          X[t][i][j] += x;
          Scap -= cap[j];
          w -= x;
          cap[j] -= x;
        }
      }
      while (w) {
        Scap = 0;
        for (int j = 1; j <= N; ++j)
          Scap += cap[j];
        for (int j = 1; j <= N; ++j) {
          if (Y[i][j] < Q) {
            // FIXME: this is a bug.
            // int x = min(cap[j], min(w, randInt(100.0 * cap[j] / Scap * w)));
            int x = min(cap[j], min(w, randInt(100000)));
            X[t][i][j] += x;
            Scap -= cap[j];
            w -= x;
            cap[j] -= x;
          }
        }
      }
    }
  }
}

int main() {
  srand(time(nullptr));
  readDemand();
  readCap();
  readY();
  readQ();
  for (int i = 1; i <= M; ++i)
    MMask[i] = i;
  for (int j = 1; j <= N; ++j)
    NMask[j] = j;

  for (int i = 1; i <= 100; ++i) {
    memset(X, 0, sizeof(X));
    assign();
    int curBenchmark = runBenchmark();
    cout << "Round " << i << " Benchmark: " << curBenchmark << endl;
    if (curBenchmark < ansBenchmark) {
      ansBenchmark = curBenchmark;
      overwriteAns();
    }
  }

  // runJudger();
  cout << "\nAns Benchmark: " << ansBenchmark << endl;

  printAns();
  return 0;
}

void readDemand() {
  fstream fin;
  stringstream ss;
  if (LOCAL_DEBUG)
    fin.open("./data/demand.csv", ios::in);
  else
    fin.open("/data/demand.csv", ios::in);
  string line, element;
  getline(fin, line);
  if (line[line.size() - 1] == '\r')
    line = line.substr(0, line.size() - 1);
  ss.str(line);
  getline(ss, element, ',');
  while (getline(ss, element, ',')) {
    ++M;
    clientName[M] = element;
  }
  while (getline(fin, line)) {
    if (line[line.size() - 1] == '\r')
      line = line.substr(0, line.size() - 1);
    ++T;
    ss.clear();
    ss.str(line);
    getline(ss, element, ',');
    for (int i = 1; i <= M; ++i) {
      getline(ss, element, ',');
      D[T][i] = stoi(element);
    }
  }
  fin.close();
}

void readCap() {
  fstream fin;
  stringstream ss;
  if (LOCAL_DEBUG)
    fin.open("./data/site_bandwidth.csv", ios::in);
  else
    fin.open("/data/site_bandwidth.csv", ios::in);
  string line, element;
  getline(fin, line);
  if (line[line.size() - 1] == '\r')
    line = line.substr(0, line.size() - 1);
  while (getline(fin, line)) {
    if (line[line.size() - 1] == '\r')
      line = line.substr(0, line.size() - 1);
    ++N;
    ss.clear();
    ss.str(line);
    getline(ss, element, ',');
    siteName[N] = element;
    getline(ss, element, ',');
    C[N] = stoi(element);
  }
  fin.close();
}

void readY() {
  fstream fin;
  stringstream ss;
  if (LOCAL_DEBUG)
    fin.open("./data/qos.csv", ios::in);
  else
    fin.open("/data/qos.csv", ios::in);
  string line, element;
  getline(fin, line);
  if (line[line.size() - 1] == '\r')
    line = line.substr(0, line.size() - 1);
  for (int j = 1; j <= N; ++j) {
    getline(fin, line);
    if (line[line.size() - 1] == '\r')
      line = line.substr(0, line.size() - 1);
    ss.clear();
    ss.str(line);
    getline(ss, element, ',');
    for (int i = 1; i <= M; ++i) {
      getline(ss, element, ',');
      Y[i][j] = stoi(element);
    }
  }
  fin.close();
}

void readQ() {
  fstream fin;
  stringstream ss;
  if (LOCAL_DEBUG)
    fin.open("./data/config.ini", ios::in);
  else
    fin.open("/data/config.ini", ios::in);
  string line, element;
  getline(fin, line);
  getline(fin, line);
  if (line[line.size() - 1] == '\r')
    line = line.substr(0, line.size() - 1);
  ss.str(line);
  getline(ss, element, '=');
  getline(ss, element, '=');
  Q = stoi(element);
  fin.close();
}

void printAns() {
  fstream fout;
  if (LOCAL_DEBUG)
    fout.open("./output/solution.txt", ios::out);
  else
    fout.open("/output/solution.txt", ios::out);
  for (int t = 1; t <= T; ++t) {
    for (int i = 1; i <= M; ++i) {
      fout << clientName[i] << ":";
      bool isFirst = true;
      for (int j = 1; j <= N; ++j) {
        if (Xans[t][i][j] == 0)
          continue;
        if (isFirst)
          isFirst = false;
        else
          fout << ",";
        fout << "<" << siteName[j] << "," << Xans[t][i][j] << ">";
      }
      if (t == T && i == M)
        continue;
      fout << "\n";
    }
  }
  fout.close();
}

void runJudger() {
  cout << "Checking Constraint 1 ..." << endl;
  bool flag = true;
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        if (X[t][i][j] < 0)
          flag = false;
  if (!flag)
    cout << "Failed Constraint 1." << endl;
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
    cout << "Failed Constraint 2." << endl;
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
    cout << "Failed Constraint 3." << endl;
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
    cout << "Failed Constraint 4." << endl;
  else
    cout << "Passed Constraint 4." << endl;
}

int runBenchmark() {
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

void shuffleMMask() { random_shuffle(MMask + 1, MMask + M + 1); }

void shuffleNMask() { random_shuffle(NMask + 1, NMask + N + 1); }

void overwriteAns() {
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        Xans[t][i][j] = X[t][i][j];
}

int randInt(int upper) { return rand() % (upper + 1); }