#ifndef METEODATA_PROTORYPE_H
#define METEODATA_PROTORYPE_H

#include <QString>
#include <QVariantHash>
#include <QDateTime>

struct MetarMeteoPrototype
{
  virtual void loadFromVariantHash(const QVariantHash &hash) = 0;
  virtual QVariantHash toVariantHash() const = 0;
  virtual bool isValid() const = 0;
};

#endif  // METEODATA_PROTORYPE_H
