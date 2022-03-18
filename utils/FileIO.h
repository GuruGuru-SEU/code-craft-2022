//
// Created by tth37 on 2022/3/18.
//

#ifndef CODECRAFT_2021_FILEIO_H
#define CODECRAFT_2021_FILEIO_H

#include <string>
#include <vector>

typedef std::vector<std::vector<int>> DemandDataBandwidths;
typedef std::vector<std::string> DemandDataClients;

typedef struct {
  unsigned int cnt;
  DemandDataBandwidths bandwidths;
  DemandDataClients clients;
} DemandData;

class FileIO {
private:
  DemandData *ptrDemandData;

  void readDemandData();

  int transStrToInt(std::string);

public:
  FileIO();
  ~FileIO();
  DemandData *getDemandData() const;
};

#endif // CODECRAFT_2021_FILEIO_H
