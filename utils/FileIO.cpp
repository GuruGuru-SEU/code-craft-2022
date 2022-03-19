#include "FileIO.h"
#include <fstream>
#include <sstream>

using namespace std;

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
    siteName[N] = element;
    getline(ss, element, ',');
    C[N] = stoi(element);
  }
  fin.close();
}

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