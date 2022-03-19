#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void readDemand();
void readCap();
void readY();
void readQ();
void printAns();

int randInt(int);

void runJudger();

int T, M, N, Q, D[9000][40], C[200], Y[40][200], X[9000][40][200];
string clientName[40], siteName[200];

int main() {
  srand(time(nullptr));
  readDemand();
  readCap();
  readY();
  readQ();
  for (int t = 1; t <= T; ++t) {
    int cap[200];
    for (int j = 1; j <= N; ++j)
      cap[j] = C[j];
    for (int i = 1; i <= M; ++i) {
      int w = D[t][i];
      int Scap = 0;
      for (int j = 1; j <= N; ++j)
        Scap += cap[j];
      for (int j = 1; j <= N; ++j) {
        if (Y[i][j] < Q) {
          int x = min(cap[j], min(w, randInt(2.0 * cap[j] / Scap * w)));
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

  runJudger();

  printAns();
  return 0;
}

void readDemand() {
  fstream fin;
  stringstream ss;
  fin.open("./data/demand.csv", ios::in);
  string line, element;
  getline(fin, line);
  ss.str(line);
  getline(ss, element, ',');
  while (getline(ss, element, ',')) {
    ++M;
    clientName[M] = element;
  }
  while (getline(fin, line)) {
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
  fin.open("./data/site_bandwidth.csv", ios::in);
  string line, element;
  getline(fin, line);
  while (getline(fin, line)) {
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
  fin.open("./data/qos.csv", ios::in);
  string line, element;
  getline(fin, line);
  for (int j = 1; j <= N; ++j) {
    getline(fin, line);
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
  fin.open("./data/config.ini", ios::in);
  string line, element;
  getline(fin, line);
  getline(fin, element, '=');
  getline(fin, element, '=');
  Q = stoi(element);
  fin.close();
}

int randInt(int upper) { return rand() % (upper + 1); }

void printAns() {
  fstream fout;
  fout.open("./output/solution.txt", ios::out);
  for (int t = 1; t <= T; ++t) {
    for (int i = 1; i <= M; ++i) {
      fout << clientName[i] << ":";
      bool isFirst = true;
      for (int j = 1; j <= N; ++j) {
        if (X[t][i][j] == 0)
          continue;
        if (isFirst)
          isFirst = false;
        else
          fout << ",";
        fout << "<" << siteName[j] << "," << X[t][i][j] << ">";
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
