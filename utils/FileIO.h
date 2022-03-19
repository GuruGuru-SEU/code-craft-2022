//
// Created by tth37 on 2022/3/18.
//

#ifndef CODECRAFT_2021_FILEIO_H
#define CODECRAFT_2021_FILEIO_H

#include <string>
#include <unordered_map>
#include <vector>

typedef std::pair<std::string, std::vector<int>> DemandDataClient;
typedef std::unordered_map<std::string, std::vector<int>> DemandDataMap;
typedef std::unordered_map<std::string, int> DemandDataClientIns;

typedef struct {
  unsigned int cnt;
  DemandDataMap mp;
} DemandData;

typedef std::pair<std::string, int> OfferDataSite;
typedef std::unordered_map<std::string, int> OfferDataMap;

typedef struct {
  unsigned int cnt;
  OfferDataMap mp;
} OfferData;

typedef int RestraintDataBound;
typedef std::pair<std::string, int> RestraintDataSiteQos;
typedef std::unordered_map<std::string, int> RestraintDataSiteMap;
typedef std::pair<std::string, RestraintDataSiteMap> RestraintQos;
typedef std::unordered_map<std::string, RestraintDataSiteMap> RestraintQosMap;

typedef struct {
  unsigned int cnt;
  RestraintDataBound bound;
  RestraintQosMap mp;
} RestraintData;

typedef std::pair<std::string, int> OutputDataSiteBandwidth;
typedef std::unordered_map<std::string, int> OutputDataSiteMap;
typedef std::pair<std::string, OutputDataSiteMap> OutputDataClientBandwidth;
typedef std::unordered_map<std::string, OutputDataSiteMap> OutputDataClientMap;
typedef std::vector<OutputDataClientMap> OutputDataVector;

typedef struct {
  OutputDataVector vec;
} OutputData;

class FileIO {
private:
  DemandData *ptrDemandData;
  void readDemandData();
  void outputDemandData() const;

  OfferData *ptrOfferData;
  void readOfferData();
  void outputOfferData() const;

  RestraintData *ptrRestraintData;
  void readRestraintData();
  void outputRestraintData() const;

  OutputData *ptrOutputData;

  static int transStrToInt(std::string);

public:
  FileIO();
  ~FileIO();
  DemandData *getDemandData() const;
  OfferData *getOfferData() const;
  RestraintData *getRestraintData() const;

  void setOutputData(OutputData *);
  void outputToFile() const;
};

#endif // CODECRAFT_2021_FILEIO_H
