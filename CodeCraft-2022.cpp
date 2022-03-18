#include "utils/FileIO.h"
#include <iostream>

int main() {
  auto IO = new FileIO();
  auto outputData = new OutputData();
  OutputDataSiteMap siteMap;
  siteMap.insert(std::make_pair("s1", 9700));
  siteMap.insert(std::make_pair("SB", 6));
  OutputDataClientMap clientMap;
  clientMap.insert(std::make_pair("CB", siteMap));
  siteMap.insert(std::make_pair("SD", 3353));
  clientMap.insert(std::make_pair("CA", siteMap));
  outputData->vec.push_back(clientMap);
  outputData->vec.push_back(clientMap);
  IO->setOutputData(outputData);
  IO->outputToFile();
  return 0;
}
