#ifndef STRUCTBUILDERDATA_H
#define STRUCTBUILDERDATA_H

enum containerType {typeList = 0};
enum _objectType { SIMPLE = 0, STRUCT = 1, CONTAINER = 2};
//enum _fieldType { typeBool, typeInt, typeString, typeDate, typeTime, typeDateTime};

struct baseFieldObject{
    virtual _objectType getObjectType() = 0;
};

struct fieldDbData{
    QString _db_fieldName;
    QString _value;
    bool multiValue;
    QString concat;
};

struct multiObject{
    QString     _concatStr;
    QStringList _multiHashKeys;
};

struct fieldObject: public baseFieldObject, public multiObject{

    virtual _objectType getObjectType(){return SIMPLE;}

    QString _fieldType;
    QString _fieldName;
    QString _fieldFormat;
    QString _fieldScale;
    bool    required;
    bool    isMultiValue;

    QString _hashKey;                           // key for state hash
    QString _resultHashListKey;                 // key for value hash from state hash
};

struct complexObject{

    QString _objectNameAsType;                // name as Type
    QString _fieldNameAsField;                 // variableName
};

struct dbObject{
    QHash<QString, QList<fieldDbData> > dbData;
};

struct structObject: public baseFieldObject , public complexObject, public dbObject{

    virtual _objectType getObjectType(){return STRUCT;}

    QList< baseFieldObject *> _structFields;
};

struct containerObject: public baseFieldObject, public complexObject{

    virtual _objectType getObjectType(){return CONTAINER;}

    containerType   _containerType;               // enum?
    QString         _containerHashKey;                           // key for state hash

    QList< baseFieldObject *> _inlineData;      // structData or fieldData. todo: fix
};

struct structBuilderObject: public dbObject{

    QString _structName;
    QString _structParent;
    QStringList _fieldsNames;
    QList< baseFieldObject *> _structFields;
};


#endif // STRUCTBUILDERDATA_H
