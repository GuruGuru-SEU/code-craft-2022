#include "FileIO.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int Dt[9000][40];
string clientNamet[40];
pair<string, int> clientNamest[40];

void readDemand(int D[][40], string clientName[], int &M, int &T) {
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
    clientNamet[M] = element;
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
      Dt[T][i] = stoi(element);
    }
  }
  fin.close();
  for (int i = 1; i <= M; ++i) {
    clientNamest[i] = make_pair(clientNamet[i], i);
  }
  sort(clientNamest + 1, clientNamest + M + 1);
  for (int i = 1; i <= M; ++i) {
    int it = clientNamest[i].second;
    clientName[i] = clientNamest[i].first;
    for (int t = 1; t <= T; ++t)
      D[t][i] = Dt[t][it];
  }
  sort(clientNamet + 1, clientNamet + M + 1);
}

int Ct[200];
string siteNamet[200];
pair<string, int> siteNamest[200];

void readCap(int C[], string siteName[], int &N) {
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
    siteNamet[N] = element;
    getline(ss, element, ',');
    Ct[N] = stoi(element);
  }
  fin.close();
  for (int j = 1; j <= N; ++j) {
    siteNamest[j] = make_pair(siteNamet[j], j);
  }
  sort(siteNamest + 1, siteNamest + N + 1);
  for (int j = 1; j <= N; ++j) {
    int jt = siteNamest[j].second;
    siteName[j] = siteNamest[j].first;
    C[j] = Ct[jt];
  }
  sort(siteNamet + 1, siteNamet + N + 1);
}

string stri[40], strj[200];

void readY(int Y[][200], int M, int N) {
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
  ss.str(line);
  getline(ss, element, ',');
  for (int i = 1; i <= M; ++i) {
    getline(ss, element, ',');
    stri[i] = element;
  }
  for (int j = 1; j <= N; ++j) {
    getline(fin, line);
    if (line[line.size() - 1] == '\r')
      line = line.substr(0, line.size() - 1);
    ss.clear();
    ss.str(line);
    getline(ss, element, ',');
    strj[j] = element;
    for (int i = 1; i <= M; ++i) {
      getline(ss, element, ',');
      int in = lower_bound(clientNamet + 1, clientNamet + M + 1, stri[i]) -
               clientNamet;
      int jn =
          lower_bound(siteNamet + 1, siteNamet + N + 1, strj[j]) - siteNamet;
      Y[in][jn] = stoi(element);
    }
  }
  fin.close();
}

void readQ(int &Q) {
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

void overwriteAns(int X[][40][200], int Xans[][40][200], int T, int M, int N) {
  for (int t = 1; t <= T; ++t)
    for (int i = 1; i <= M; ++i)
      for (int j = 1; j <= N; ++j)
        Xans[t][i][j] = X[t][i][j];
}


void printAns(string clientName[], string siteName[], int Xans[][40][200],
              int T, int M, int N) {
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