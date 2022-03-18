//
// Created by tth37 on 2022/3/18.
//

#include "FileIO.h"
#include <fstream>
#include <iostream>

using namespace std;

std::vector<std::vector<int>> *FileIO::readDemandCSV() {
  fstream fin;
  fin.open("./data/demand.csv", ios::in);
  string tmp;
  getline(fin, tmp);
  cerr << tmp << endl;
}