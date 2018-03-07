#ifndef DECODERULESVALIDATOR_H
#define DECODERULESVALIDATOR_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QtXml/QtXml>
#include <QXmlStreamReader>
#include <stdexcept>
#include "structbuilderdata.h"

class ProtocolStructBuilder
{
    QList< structBuilderObject >        _protocolData;
    QHash<QString, QList<fieldDbData> > _db_data;
    QString                             _ruleFile;

    void loadXmlData();
//    fieldObject * makeFieldObject(const QXmlStreamReader &xml);
    QByteArray makeStructDeclaration(structObject *);
    void createHeaderFile(const QString &fileName);
    void createSourceFile(const QString &fileName);

    QByteArray createLoadFromHashFunction();
    QByteArray createToHashFunction();
    QByteArray createIsValidFunction();
public:
    ProtocolStructBuilder(const QString &ruleFilePath);
    static bool validate(const QString &ruleFile);
    bool createStructs(const QString &outputFileName);
    bool createPersister(const QString &ruleFile);
    bool createTests(const QString &ruleFile);
};

#endif // DECODERULESVALIDATOR_H
