#include "utils/FileIO.h"
#include <iostream>

using namespace std;

void getDemandDataClientIns(DemandDataClientIns &, DemandData *, int);
void getRestraintDataSiteMap(RestraintDataSiteMap &, RestraintData *,
                             const string &);

int getCapSum(const OfferDataMap &);
int getRandomInt(int upper);

void initOutputData(OutputData *, int);
void modifyOutputData(OutputData *, int, const string &, const string &, int);

int main() {
  srand(time(nullptr));
  auto IO = new FileIO();
  auto ptrOutputData = new OutputData();

  //  auto outputData = new OutputData();
  //  OutputDataSiteMap siteMap;
  //  siteMap.insert(std::make_pair("s1", 9700));
  //  siteMap.insert(std::make_pair("SB", 6));
  //  OutputDataClientMap clientMap;
  //  clientMap.insert(std::make_pair("CB", siteMap));
  //  siteMap.insert(std::make_pair("SD", 3353));
  //  clientMap.insert(std::make_pair("CA", siteMap));
  //  outputData->vec.push_back(clientMap);
  //  outputData->vec.push_back(clientMap);
  //  IO->setOutputData(outputData);
  //  IO->outputToFile();

  DemandData *ptrDemandData = IO->getDemandData();
  OfferData *ptrOfferData = IO->getOfferData();
  RestraintData *ptrRestraintData = IO->getRestraintData();

  int T = (int)ptrDemandData->mp.begin()->second.size();

  initOutputData(ptrOutputData, T);

  for (int t = 0; t < T; ++t) {
    OfferDataMap cap = ptrOfferData->mp;
    DemandDataClientIns demandDataClientIns;
    getDemandDataClientIns(demandDataClientIns, ptrDemandData, t);
    for (auto &i : demandDataClientIns) {
      int w = i.second;
      int scap = getCapSum(cap);
      RestraintDataSiteMap restraintDataSiteMap;
      getRestraintDataSiteMap(restraintDataSiteMap, ptrRestraintData, i.first);
      for (auto &j : restraintDataSiteMap) {
        int Y = j.second;
        int Q = ptrRestraintData->bound;
        if (Y < Q) {

          //  cout << j.first << endl;
          int &capj = cap[j.first];
          int X = min(capj, min(w, getRandomInt(2.0 * capj / scap * w)));
          scap -= capj;
          w -= X;
          capj -= X;
          modifyOutputData(ptrOutputData, t, i.first, j.first, X);
        }
      }
      while (w) {
        scap = getCapSum(cap);
        for (auto &j : restraintDataSiteMap) {
          int Y = j.second;
          int Q = ptrRestraintData->bound;
          if (Y < Q) {
            int &capj = cap[j.first];
            int X = min(capj, min(w, getRandomInt(100000)));
            scap -= capj;
            w -= X;
            capj -= X;
            modifyOutputData(ptrOutputData, t, i.first, j.first, X);
          }
        }
      }
    }
  }

  IO->setOutputData(ptrOutputData);
  IO->outputToFile();
  return 0;
}

void getDemandDataClientIns(DemandDataClientIns &demandDataClientIns,
                            DemandData *ptrDemandData, int t) {
  for (auto &it : ptrDemandData->mp) {
    demandDataClientIns.insert(make_pair(it.first, it.second[t]));
  }
}

int getCapSum(const OfferDataMap &cap) {
  int res = 0;
  for (auto &it : cap) {
    res += it.second;
  }
  return res;
}

void getRestraintDataSiteMap(RestraintDataSiteMap &restraintDataSiteMap,
                             RestraintData *ptrRestraintData,
                             const string &clientName) {
  restraintDataSiteMap = ptrRestraintData->mp[clientName];
}

int getRandomInt(int upper) { return rand() % (upper + 1); }

void initOutputData(OutputData *ptrOutputData, int t) {
  for (int i = 0; i < t; ++i)
    ptrOutputData->vec.push_back(*new OutputDataClientMap());
}

void modifyOutputData(OutputData *ptrOutputData, int t,
                      const string &clientName, const string &siteName,
                      int bandwidth) {
  ptrOutputData->vec[t][clientName][siteName] += bandwidth;
}