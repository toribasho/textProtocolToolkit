#ifndef DECODERULESVALIDATOR_H
#define DECODERULESVALIDATOR_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QtXml/QtXml>
#include <QXmlStreamReader>

enum containerType { typeStruct = 0, typeList = 1} ;
enum _fieldType { SIMPLE, STRUCT, CONTAINER};
//enum _fieldType { typeBool, typeInt, typeString, typeDate, typeTime, typeDateTime};

struct baseFieldObject{
    _fieldType getObjectType() = 0;
};

struct fieldDbData{
    QString _db_fieldName;
    QString _value;
    bool multiValue;
    QString concat;
};

struct fieldObject: public baseFieldObject{

    _fieldType getObjectType(){return _fieldType::SIMPLE;}

    QString _fieldType;
    QString _fieldName;
    QString _fieldFormat;
    QString _fieldScale;
    bool    required;

    QString _hashKey;                           // key for state hash
    QString _resultHashListKey;                 // key for value hash from state hash
};

struct structObject: public baseFieldObject{

    _fieldType getObjectType(){return _fieldType::STRUCT;}

    QString _exStructName;                      // name as Type
    QString _structAsFieldName;                 // variableName

    QList< baseFieldObject *> _structFields;

    QHash<QString, QList<fieldDbData> > dbData;
};

struct containerObject: public baseFieldObject{

    _fieldType getObjectType(){return _fieldType::CONTAINER;}

    containerType _containerType;               // enum?
    QString _exContainerName;                   // name as Type
    QString _containerAsFieldName;              // variableName

    QList< baseFieldObject *> _inlineData;      // structData or fieldData
};

struct structBuilderObject{
    QString _structName;
    QString _structParent;
    QList< baseFieldObject *> _structFields;

    QHash<QString, QList<fieldDbData> > dbData;
};

class ProtocolStructBuilder
{
    QList< structBuilderObject >        _protocolData;
    QHash<QString, QList<fieldDbData> > _db_data;
    QString                             _ruleFile;

    void loadXmlData();
//    fieldObject * makeFieldObject(const QXmlStreamReader &xml);

public:
    static bool validate(const QString &ruleFile);
    bool createStructs(const QString &ruleFile, const QString &outputFileName);
    bool createPersister(const QString &ruleFile);
    bool createTests(const QString &ruleFile);
};

#endif // DECODERULESVALIDATOR_H
