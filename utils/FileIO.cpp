//
// Created by tth37 on 2022/3/18.
//

#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>

int FileIO::transStrToInt(std::string s) {
  size_t len = s.length();
  int res = 0;
  for (size_t i = 0; i < len; ++i) {
    res = res * 10 + s[i] - '0';
  }
  return res;
}

void FileIO::readDemandData() {
  ptrDemandData->cnt = 0;

  std::fstream fin;
  std::stringstream ss;

  std::vector<std::string> clientNames;
  std::vector<std::vector<int>> clientBandwidths;

  fin.open("./data/demand.csv", std::ios::in);

  std::string line, element;
  std::getline(fin, line);

  ss.str(line);
  std::getline(ss, element, ','); // read "mtime"
  while (std::getline(ss, element, ',')) {
    ++ptrDemandData->cnt;
    clientNames.push_back(element);
    clientBandwidths.push_back(*new std::vector<int>());
  }

  while (std::getline(fin, line)) {
    std::vector<int> tempVector;
    ss.clear();
    ss.str(line);
    std::getline(ss, element, ','); // read time
    int cnt = 0;
    while (std::getline(ss, element, ',')) {
      clientBandwidths[cnt++].push_back(transStrToInt(element));
    }
  }

  auto it1 = clientNames.begin();
  auto it2 = clientBandwidths.begin();

  while (it1 != clientNames.end()) {
    DemandDataClient client = std::make_pair(*it1, *it2);
    ptrDemandData->mp.insert(client);
    ++it1, ++it2;
  }

  fin.close();
}

void FileIO::outputDemandData() const {
  std::cout << "Demand Data -----" << std::endl;
  int cnt = 0;
  for (auto &it : ptrDemandData->mp) {
    std::cout << it.first << ": [" << it.second[0] << ", " << it.second[1]
              << ", ...]\n";
    if (++cnt > 10)
      break;
  }
  if (cnt == 11)
    std::cout << "..." << std::endl;
  std::cout << "Demand Data -----" << std::endl << std::endl;
}

void FileIO::readOfferData() {
  ptrOfferData->cnt = 0;

  std::fstream fin;
  std::stringstream ss;

  fin.open("./data/site_bandwidth.csv");

  std::string line, element;
  std::getline(fin, line);

  while (std::getline(fin, line)) {
    ++ptrOfferData->cnt;

    std::string siteName;
    int siteBandwidth;

    ss.clear();
    ss.str(line);
    std::getline(ss, element, ','); // read site
    siteName = element;

    std::getline(ss, element, ','); // read bandwidth
    siteBandwidth = transStrToInt(element);

    OfferDataSite site = std::make_pair(siteName, siteBandwidth);
    ptrOfferData->mp.insert(site);
  }

  fin.close();
}

void FileIO::outputOfferData() const {
  std::cout << "Offer Data -----" << std::endl;
  int cnt = 0;
  for (auto &it : ptrOfferData->mp) {
    std::cout << it.first << ": " << it.second << "\n";
    if (++cnt > 10)
      break;
  }
  if (cnt == 11)
    std::cout << "..." << std::endl;
  std::cout << "Offer Data -----" << std::endl << std::endl;
}

void FileIO::readRestraintData() {
  ptrRestraintData->cnt = 0;

  std::fstream fin;
  std::stringstream ss;

  fin.open("./data/qos.csv", std::ios::in);

  std::string line, element;
  std::vector<std::string> clientNames, siteNames;
  std::vector<std::vector<int>> clientQos;

  std::getline(fin, line);
  ss.str(line);
  std::getline(ss, element, ','); // read "site_name"
  while (std::getline(ss, element, ',')) {
    ++ptrRestraintData->cnt;
    clientNames.push_back(element);
    clientQos.push_back(*new std::vector<int>());
  }

  while (std::getline(fin, line)) {
    ss.clear();
    ss.str(line);
    std::getline(ss, element, ',');
    siteNames.push_back(element);
    int cnt = 0;
    while (std::getline(ss, element, ',')) {
      clientQos[cnt++].push_back(transStrToInt(element));
    }
  }

  auto it1 = clientNames.begin();
  auto it2 = clientQos.begin();
  while (it1 != clientNames.end()) {
    auto it3 = siteNames.begin();
    auto it4 = it2->begin();
    RestraintDataSiteMap siteMap;
    while (it3 != siteNames.end()) {
      RestraintDataSiteQos siteQos = std::make_pair(*it3, *it4);
      siteMap.insert(siteQos);
      ++it3, ++it4;
    }
    RestraintQos qos = std::make_pair(*it1, siteMap);
    ptrRestraintData->mp.insert(qos);
    ++it1, ++it2;
  }

  fin.close();

  fin.open("./data/config.ini", std::ios::in);
  std::getline(fin, line);
  std::getline(fin, element, '=');
  std::getline(fin, element, '=');
  ptrRestraintData->bound = transStrToInt(element);

  fin.close();
}

void FileIO::outputRestraintData() const {
  std::cout << "Restraint Data -----" << std::endl;
  int cnt = 0;
  for (auto &it : ptrRestraintData->mp) {
    std::cout << it.first << ": {";
    int cnt2 = 0;
    for (auto &it2 : it.second) {
      std::cout << it2.first << ": " << it2.second << ", ";
      if (++cnt2 > 3)
        break;
    }
    if (cnt2 == 4)
      std::cout << "...}" << std::endl;
    else
      std::cout << "}" << std::endl;
    if (++cnt > 10)
      break;
  }
  if (cnt == 11)
    std::cout << "..." << std::endl;
  std::cout << "Restraint Data -----" << std::endl << std::endl;
}

FileIO::FileIO() {
  ptrDemandData = new DemandData();
  readDemandData();
  outputDemandData();

  ptrOfferData = new OfferData();
  readOfferData();
  outputOfferData();

  ptrRestraintData = new RestraintData();
  readRestraintData();
  outputRestraintData();
}

FileIO::~FileIO() {
  delete ptrDemandData;
  delete ptrOfferData;
  delete ptrRestraintData;
}

DemandData *FileIO::getDemandData() const { return ptrDemandData; }

OfferData *FileIO::getOfferData() const { return ptrOfferData; }

RestraintData *FileIO::getRestraintData() const { return ptrRestraintData; }

void FileIO::setOutputData(OutputData *ptr) { ptrOutputData = ptr; }

void FileIO::outputToFile() const {
  std::fstream fout;

  fout.open("./output/solution.txt", std::ios::out);

  for (auto &it1 : ptrOutputData->vec) {
    for (auto &it2 : it1) {
      fout << it2.first << ':';
      bool isFirst = true;
      for (auto &it3 : it2.second) {
        if (!it3.second)
          continue;
        if (isFirst)
          isFirst = false;
        else
          fout << ",";
        fout << '<' << it3.first << ',' << it3.second << '>';
      }
      fout << '\n';
    }
  }
}