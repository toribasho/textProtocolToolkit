#include "meteodata.h"

void MetarMeteoStruct::loadFromVariantHash(const QVariantHash &hash)
{
  if (hash.keys().contains("METAR_C_VALIDDATE")){
    QVariantHash valuesHash = hash.value("METAR_C_VALIDDATE").toHash();
    if (valuesHash.keys().contains("CANCEL_DATE")){
      cancelDate = QDateTime::fromString(valuesHash.value("CANCEL_DATE").toString(), "yyMMddhhmm");
    }
  }

  if (hash.keys().contains("METAR_HEAD")){
    QVariantHash valuesHash = hash.value("METAR_HEAD").toHash();
    if (valuesHash.keys().contains("METAR_TYPE")){
      type = valuesHash.value("METAR_TYPE").toString();
    }
  }

  if (hash.keys().contains("METAR_HEAD")){
    QVariantHash valuesHash = hash.value("METAR_HEAD").toHash();
    if (valuesHash.keys().contains("METAR_ICAO")){
      icao = valuesHash.value("METAR_ICAO").toString();
    }
  }

  if (hash.keys().contains("METAR_HEAD")){
    QVariantHash valuesHash = hash.value("METAR_HEAD").toHash();
    if (valuesHash.keys().contains("METAR_DT")){
      DT = QDateTime::fromString(valuesHash.value("METAR_DT").toString(), "ddhhmm");
    }
  }

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("SFCWINDDIR")){
      windDirection = valuesHash.value("SFCWINDDIR").toString();
    }
  }

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("SFCWINDSPD")){
      windSpeed = valuesHash.value("SFCWINDSPD").toInt();
    }
    else
      windSpeed = -100;
  }
  else
    windSpeed = -100;

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("SFCWINDGUST")){
      windGust = valuesHash.value("SFCWINDGUST").toInt();
    }
    else
      windGust = -100;
  }
  else
    windGust = -100;

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("SFCWINDMES")){
      windMeasure = valuesHash.value("SFCWINDMES").toString();
    }
  }

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("SFCWINDCHANGE")){
      windChage = valuesHash.value("SFCWINDCHANGE").toString();
    }
  }

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("VISIBILITY")){
      visibility = valuesHash.value("VISIBILITY").toInt();
    }
    else
      visibility = -100;
  }
  else
    visibility = -100;

  if (hash.keys().contains("METAR_SFCWIND_VIS")){
    QVariantHash valuesHash = hash.value("METAR_SFCWIND_VIS").toHash();
    if (valuesHash.keys().contains("MINVISIBILITY")){
      minVIsibility = valuesHash.value("MINVISIBILITY").toString();
    }
  }

  if (hash.keys().contains("METAR_R_RVR")){
    QVariantHash valuesHash = hash.value("METAR_R_RVR").toHash();
    if (valuesHash.keys().contains("R_RVRVPP")){
      rvrRwy = valuesHash.value("R_RVRVPP").toString();
    }
  }

  if (hash.keys().contains("METAR_R_RVR")){
    QVariantHash valuesHash = hash.value("METAR_R_RVR").toHash();
    if (valuesHash.keys().contains("R_RVR")){
      rvr = valuesHash.value("R_RVR").toString();
    }
  }

  if (hash.keys().contains("METAR_R_RVR")){
    QVariantHash valuesHash = hash.value("METAR_R_RVR").toHash();
    if (valuesHash.keys().contains("R_PREV_RVRVPPP")){
      pRvrRwy = valuesHash.value("R_PREV_RVRVPPP").toString();
    }
  }

  if (hash.keys().contains("METAR_R_RVR")){
    QVariantHash valuesHash = hash.value("METAR_R_RVR").toHash();
    if (valuesHash.keys().contains("R_PREV_RVR")){
      pRvr = valuesHash.value("R_PREV_RVR").toString();
    }
  }

  if (hash.keys().contains("METAR_CURWEATHER")){
    QVariantHash valuesHash = hash.value("METAR_CURWEATHER").toHash();
    if (valuesHash.keys().contains("CURWEATHER")){
      weather = valuesHash.value("CURWEATHER").toString();
    }
  }

  if (hash.keys().contains("METAR_CLOUD")){
    QVariantHash valuesHash = hash.value("METAR_CLOUD").toHash();
    if (valuesHash.keys().contains("CLOUD")){
      clouds = valuesHash.value("CLOUD").toString();
    }
  }

  if (hash.keys().contains("METAR_M_TEMP_DEW_QNH")){
    QVariantHash valuesHash = hash.value("METAR_M_TEMP_DEW_QNH").toHash();
    if (valuesHash.keys().contains("M_TEMP")){
      temp = valuesHash.value("M_TEMP").toInt();
    }
    else
      temp = -100;
  }
  else
    temp = -100;

  if (hash.keys().contains("METAR_M_TEMP_DEW_QNH")){
    QVariantHash valuesHash = hash.value("METAR_M_TEMP_DEW_QNH").toHash();
    if (valuesHash.keys().contains("M_DEW_POINT")){
      dewPoint = valuesHash.value("M_DEW_POINT").toInt();
    }
    else
      dewPoint = -100;
  }
  else
    dewPoint = -100;

  if (hash.keys().contains("METAR_M_TEMP_DEW_QNH")){
    QVariantHash valuesHash = hash.value("METAR_M_TEMP_DEW_QNH").toHash();
    if (valuesHash.keys().contains("Q_PRESURE")){
      presure = valuesHash.value("Q_PRESURE").toInt();
    }
    else
      presure = -100;
  }
  else
    presure = -100;

  if (hash.keys().contains("METAR_PREV_WEATHER")){
    QVariantHash valuesHash = hash.value("METAR_PREV_WEATHER").toHash();
    if (valuesHash.keys().contains("PREV_WEATHER")){
      prevWeather = valuesHash.value("PREV_WEATHER").toString();
    }
  }

  if (hash.keys().contains("METAR_WINDSHEAR")){
    QVariantHash valuesHash = hash.value("METAR_WINDSHEAR").toHash();
    if (valuesHash.keys().contains("WINDSHEAR")){
      windShear = valuesHash.value("WINDSHEAR").toString();
    }
  }

  if (hash.keys().contains("METAR_SEATEMP")){
    QVariantHash valuesHash = hash.value("METAR_SEATEMP").toHash();
    if (valuesHash.keys().contains("SEATEMP")){
      seaTemp = valuesHash.value("SEATEMP").toString();
    }
  }

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_RWY")){
      rwyState.rwy = valuesHash.value("VPPSTATE_RWY").toString();
    }
  }

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_SCURF")){
      rwyState.scurf = valuesHash.value("VPPSTATE_SCURF").toInt();
    }
    else
      rwyState.scurf = -100;
  }
  else
    rwyState.scurf = -100;

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_POLLUTION")){
      rwyState.pollution = valuesHash.value("VPPSTATE_POLLUTION").toInt();
    }
    else
      rwyState.pollution = -100;
  }
  else
    rwyState.pollution = -100;

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_SCURF_HEIGHT")){
      rwyState.scurfHeight = valuesHash.value("VPPSTATE_SCURF_HEIGHT").toInt();
    }
    else
      rwyState.scurfHeight = -100;
  }
  else
    rwyState.scurfHeight = -100;

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_CLUTCH")){
      rwyState.clutch = valuesHash.value("VPPSTATE_CLUTCH").toInt();
    }
    else
      rwyState.clutch = -100;
  }
  else
    rwyState.clutch = -100;

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE_CLRD")){
      rwyState.rwyCleared = valuesHash.keys().contains("VPPSTATE_CLRD");
    }
  }

  if (hash.keys().contains("METAR_VPPSTATE")){
    QVariantHash valuesHash = hash.value("METAR_VPPSTATE").toHash();
    if (valuesHash.keys().contains("VPPSTATE")){
      rwyState.rwySNOCLO = valuesHash.keys().contains("VPPSTATE");
    }
  }

  if (hash.keys().contains("METAR_TREND")){
    QVariantHash valuesHash = hash.value("METAR_TREND").toHash();
    if (valuesHash.keys().contains("TREND")){
      joinedTrend = valuesHash.value("TREND").toString();
    }
  }

  int i = 1;
  while (true){
    QString keyModificator = "";
    if (i != 1) keyModificator = QString("_%1").arg(i);
    i++;
    if (hash.keys().contains("METAR_TREND"+keyModificator)){
      Trend trend;
      QVariantHash valuesHash = hash.value("METAR_TREND"+keyModificator).toHash();
      if (valuesHash.keys().contains("TREND")){
        trend.trend = valuesHash.value("TREND").toString();
      }
      if (valuesHash.keys().contains("TREND_PERIOD_START")){
        trend.trendStart = valuesHash.value("TREND_PERIOD_START").toString();
      }
      if (valuesHash.keys().contains("TREND_PERIOD_END")){
        trend.trendEnd = valuesHash.value("TREND_PERIOD_END").toString();
      }
      if (valuesHash.keys().contains("TREND_WINDDIR")){
        trend.windDirection = valuesHash.value("TREND_WINDDIR").toString();
      }
      if (valuesHash.keys().contains("TREND_WINDSPD")){
        trend.windSpeed = valuesHash.value("TREND_WINDSPD").toInt();
      }
      else
        trend.windSpeed = -100;
      if (valuesHash.keys().contains("TREND_WINDGUST")){
        trend.windGust = valuesHash.value("TREND_WINDGUST").toInt();
      }
      else
        trend.windGust = -100;
      if (valuesHash.keys().contains("TREND_WINDMES")){
        trend.windMeasure = valuesHash.value("TREND_WINDMES").toString();
      }
      if (valuesHash.keys().contains("TREND_VISIBILITY")){
        trend.visibility = valuesHash.value("TREND_VISIBILITY").toInt();
      }
      else
        trend.visibility = -100;
      if (valuesHash.keys().contains("TREND_WEATHER")){
        trend.weather = valuesHash.value("TREND_WEATHER").toString();
      }
      if (valuesHash.keys().contains("TREND_CLOUD")){
        trend.clouds = valuesHash.value("TREND_CLOUD").toString();
      }
      trends << trend;
    }
    else break;
  }
}

QVariantHash MetarMeteoStruct::toVariantHash() const
{
  QVariantHash result;
  QVariantHash valueHash;

  if (result.keys().contains("METAR_C_VALIDDATE") )
    valueHash = result.take("METAR_C_VALIDDATE").toHash();
  valueHash["CANCEL_DATE"] = cancelDate;
  result["METAR_C_VALIDDATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_HEAD") )
    valueHash = result.take("METAR_HEAD").toHash();
  valueHash["METAR_TYPE"] = type;
  result["METAR_HEAD"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_HEAD") )
    valueHash = result.take("METAR_HEAD").toHash();
  valueHash["METAR_ICAO"] = icao;
  result["METAR_HEAD"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_HEAD") )
    valueHash = result.take("METAR_HEAD").toHash();
  valueHash["METAR_DT"] = DT;
  result["METAR_HEAD"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["SFCWINDDIR"] = windDirection;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["SFCWINDSPD"] = windSpeed;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["SFCWINDGUST"] = windGust;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["SFCWINDMES"] = windMeasure;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["SFCWINDCHANGE"] = windChage;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["VISIBILITY"] = visibility;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SFCWIND_VIS") )
    valueHash = result.take("METAR_SFCWIND_VIS").toHash();
  valueHash["MINVISIBILITY"] = minVIsibility;
  result["METAR_SFCWIND_VIS"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_R_RVR") )
    valueHash = result.take("METAR_R_RVR").toHash();
  valueHash["R_RVRVPP"] = rvrRwy;
  result["METAR_R_RVR"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_R_RVR") )
    valueHash = result.take("METAR_R_RVR").toHash();
  valueHash["R_RVR"] = rvr;
  result["METAR_R_RVR"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_R_RVR") )
    valueHash = result.take("METAR_R_RVR").toHash();
  valueHash["R_PREV_RVRVPPP"] = pRvrRwy;
  result["METAR_R_RVR"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_R_RVR") )
    valueHash = result.take("METAR_R_RVR").toHash();
  valueHash["R_PREV_RVR"] = pRvr;
  result["METAR_R_RVR"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_CURWEATHER") )
    valueHash = result.take("METAR_CURWEATHER").toHash();
  valueHash["CURWEATHER"] = weather;
  result["METAR_CURWEATHER"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_CLOUD") )
    valueHash = result.take("METAR_CLOUD").toHash();
  valueHash["CLOUD"] = clouds;
  result["METAR_CLOUD"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_M_TEMP_DEW_QNH") )
    valueHash = result.take("METAR_M_TEMP_DEW_QNH").toHash();
  valueHash["M_TEMP"] = temp;
  result["METAR_M_TEMP_DEW_QNH"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_M_TEMP_DEW_QNH") )
    valueHash = result.take("METAR_M_TEMP_DEW_QNH").toHash();
  valueHash["M_DEW_POINT"] = dewPoint;
  result["METAR_M_TEMP_DEW_QNH"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_M_TEMP_DEW_QNH") )
    valueHash = result.take("METAR_M_TEMP_DEW_QNH").toHash();
  valueHash["Q_PRESURE"] = presure;
  result["METAR_M_TEMP_DEW_QNH"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_PREV_WEATHER") )
    valueHash = result.take("METAR_PREV_WEATHER").toHash();
  valueHash["PREV_WEATHER"] = prevWeather;
  result["METAR_PREV_WEATHER"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_WINDSHEAR") )
    valueHash = result.take("METAR_WINDSHEAR").toHash();
  valueHash["WINDSHEAR"] = windShear;
  result["METAR_WINDSHEAR"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_SEATEMP") )
    valueHash = result.take("METAR_SEATEMP").toHash();
  valueHash["SEATEMP"] = seaTemp;
  result["METAR_SEATEMP"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_RWY"] = rwyState.rwy;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_SCURF"] = rwyState.scurf;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_POLLUTION"] = rwyState.pollution;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_SCURF_HEIGHT"] = rwyState.scurfHeight;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_CLUTCH"] = rwyState.clutch;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE_CLRD"] = rwyState.rwyCleared;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_VPPSTATE") )
    valueHash = result.take("METAR_VPPSTATE").toHash();
  valueHash["VPPSTATE"] = rwyState.rwySNOCLO;
  result["METAR_VPPSTATE"] = valueHash;
  valueHash.clear();

  if (result.keys().contains("METAR_TREND") )
    valueHash = result.take("METAR_TREND").toHash();
  valueHash["TREND"] = joinedTrend;
  result["METAR_TREND"] = valueHash;
  valueHash.clear();

  QString keyModificator = "";
  for (int i = 0 ; i < trends.count() ; i++){
    if (i != 0) keyModificator = QString("_%1").arg(i+1);
    {
      QVariantHash valuesHash;
      valuesHash["TREND"] = trends.at(i).trend;
      valuesHash["TREND_PERIOD_START"] = trends.at(i).trendStart;
      valuesHash["TREND_PERIOD_END"] = trends.at(i).trendEnd;
      valuesHash["TREND_WINDDIR"] = trends.at(i).windDirection;
      valuesHash["TREND_WINDSPD"] = trends.at(i).windSpeed;
      valuesHash["TREND_WINDGUST"] = trends.at(i).windGust;
      valuesHash["TREND_WINDMES"] = trends.at(i).windMeasure;
      valuesHash["TREND_VISIBILITY"] = trends.at(i).visibility;
      valuesHash["TREND_WEATHER"] = trends.at(i).weather;
      valuesHash["TREND_CLOUD"] = trends.at(i).clouds;
      result["METAR_TREND"+keyModificator] = valuesHash;
    }
  return result;
}

bool MetarMeteoStruct::isValid() const
{
  bool result = true;
  if (type.isEmpty()) result = false;
  if (icao.isEmpty()) result = false;
  if (DT.isValid()) result = false;
  if (rwyState.rwy.isEmpty()) result = false;
  if (rwyState.scurf == -100) result = false;
  if (rwyState.pollution == -100) result = false;
  if (rwyState.scurfHeight == -100) result = false;
  if (rwyState.clutch == -100) result = false;
  if (!rwyState.rwyCleared) result = false;
  if (!rwyState.rwySNOCLO) result = false;
  result = !trends.isEmpty();
  return result;
}

