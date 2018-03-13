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

    void loadXmlData(); //NEED REFACTORING

    QByteArray makeStructDeclaration(const QString &structName, const QString &structParent,
                                     const QList<baseFieldObject *> &structFields, bool mainStruct);

    QString makeStructFieldDeclaration(const QString &fieldType, const QString &fieldName);

    QByteArray makeFieldAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldType,
                                   const QString &fieldName, const QString &fieldFormat, const QString &fieldScale);

    QByteArray makeStructListFieldAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldType,
                                             const QString &fieldName, const QString &fieldFormat, const QString &fieldScale,
                                             const QString &structType, const QString &structName, const QString &containerName,
                                             bool createStruct = false, bool storeToContainer = false);

    QByteArray makeFieldToHashAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldName);

    QString makeFieldValidation(const QString &fieldType, const QString &fieldName);

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
