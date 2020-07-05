#ifndef METEODATA_H
#define METEODATA_H

#include "meteoData_protorype.h"

struct rwyStateStruct
{
  QString rwy;
  int scurf;
  int pollution;
  int scurfHeight;
  int clutch;
  bool rwyCleared;
  bool rwySNOCLO;
};

struct Trend
{
  QString trend;
  QString trendStart;
  QString trendEnd;
  QString windDirection;
  int windSpeed;
  int windGust;
  QString windMeasure;
  int visibility;
  QString weather;
  QString clouds;
};

struct MetarMeteoStruct: public MeteoStruct
{
  QDateTime cancelDate;
  QString type;
  QString icao;
  QDateTime DT;
  QString windDirection;
  int windSpeed;
  int windGust;
  QString windMeasure;
  QString windChage;
  int visibility;
  QString minVIsibility;
  QString rvrRwy;
  QString rvr;
  QString pRvrRwy;
  QString pRvr;
  QString weather;
  QString clouds;
  int temp;
  int dewPoint;
  int presure;
  QString prevWeather;
  QString windShear;
  QString seaTemp;
  rwyStateStruct rwyState;
  QString joinedTrend;
  QList<Trend> trends;

  void loadFromVariantHash(const QVariantHash &hash);
  QVariantHash toVariantHash() const;
  bool isValid() const;
};

#endif  // METEODATA_H
