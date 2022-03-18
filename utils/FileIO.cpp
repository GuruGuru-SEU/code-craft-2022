//
// Created by tth37 on 2022/3/18.
//

#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>

int FileIO::transStrToInt(std::string s) {
  int len = (int)s.length();
  int res = 0;
  for (int i = len - 1; i >= 0; --i) {
    res = res * 10 + s[i] - '0';
  }
  return res;
}

void FileIO::readDemandData() {
  ptrDemandData->cnt = 0;

  std::fstream fin;
  std::stringstream ss;

  fin.open("./data/demand.csv", std::ios::in);

  std::string line, element;
  std::getline(fin, line);
  std::cout << line << std::endl;

  ss.str(line);
  std::getline(ss, element, ','); // read "mtime"
  while (std::getline(ss, element, ',')) {
    ++ptrDemandData->cnt;
    ptrDemandData->clients.push_back(element);
  }

  while (std::getline(fin, line)) {
    std::vector<int> tempVector;
    ss.clear();
    ss.str(line);
    std::getline(ss, element, ','); // read time
    while (std::getline(ss, element, ',')) {
      tempVector.push_back(transStrToInt(element));
    }
    ptrDemandData->bandwidths.push_back(tempVector);
  }
}

FileIO::FileIO() {
  ptrDemandData = new DemandData();
  readDemandData();
}

FileIO::~FileIO() { delete ptrDemandData; }

DemandData *FileIO::getDemandData() const { return ptrDemandData; }