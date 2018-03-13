#include "protocolstructbuilder.h"

QString insertTab(const int &count){
//    int spaceCount = QSettings::value("tab_space_count",2).toInt();
    QString resultTabSpacing = "";
    for (int i=0;i<count;i++)
        resultTabSpacing+= "  ";
    return resultTabSpacing;
}

void ProtocolStructBuilder::loadXmlData()
{
    QFile f(_ruleFile);
    if (f.exists(_ruleFile))
        f.open(QIODevice::ReadOnly);
    else {
        qDebug() << "incorrect path to meteoDecodeRules.xml";
        return;
    }

    structBuilderObject *builder;;
    QString dbTableName;
    QXmlStreamReader xml(&f);
//    QDomDocument
    bool ignoreReadNext = false;

    while (!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token;

        if (!ignoreReadNext)
            token = xml.readNext();
        else
            ignoreReadNext = false;

        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name().toString() == "meteo"){
                QXmlStreamAttributes attributes = xml.attributes();

                structBuilderObject meteoProtocollClass;
                meteoProtocollClass._structName = attributes.value("struct").toString();
                meteoProtocollClass._structParent = attributes.value("parent").toString();
                dbTableName = attributes.value("name").toString();

// TODO: CREATE DB DATA SCTRUCT FILL UP
// TODO: MORE CODE REUSEMENT

//                mainDbData mainDb;
//                mainDb._db_table = attributes.value("name").toString();

                _protocolData.append(meteoProtocollClass);
//                _db_data.append(mainDb);

                builder = &_protocolData[_protocolData.size()-1];
//                mDb = &_db_data[_db_data.size()-1];
            }
            if (xml.name().toString() == "state"){
                QXmlStreamAttributes attributes = xml.attributes();
                QString curStateName = attributes.value("field").toString();

                token = xml.readNext();
                while (xml.name().toString().isEmpty())
                    token = xml.readNext();
                if (xml.name().toString() == "state"){
                    ignoreReadNext = true;
                    continue;
                }
                QString curValueName;

                QHash<QString , containerObject *>  containtersInState;
                QHash<QString , structObject *>     structsInState;

                while (xml.name() != "state" /*&& token != QXmlStreamReader::EndElement*/){
                    if (xml.name().toString() == "value"){
                        QStringList valuesInSection;
                        if (xml.attributes().hasAttribute("field"))
                            curValueName = xml.attributes().value("field").toString();

                        token = xml.readNext();
                        while (/*token != QXmlStreamReader::EndElement &&*/ xml.name() != "value"){

                            if (xml.name().toString() == "subvalue")
                                if (xml.attributes().hasAttribute("field"))
                                    curValueName = xml.attributes().value("field").toString();

                            if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                fieldObject * fieldObj = new fieldObject;
                                fieldObj->_hashKey = curStateName;
                                fieldObj->_resultHashListKey = curValueName;

                                if (attributes.hasAttribute("required"))
                                    fieldObj->required = attributes.value("required").toString() == "true";
                                else
                                    fieldObj->required = false;

                                if (attributes.hasAttribute("multiValue")){
                                    fieldObj->isMultiValue = true;
                                    fieldObj->_multiHashKeys.append(curValueName);
                                    fieldObj->_concatStr = attributes.hasAttribute("concat") ? attributes.value("concat").toString() : " ";
                                }
                                else
                                    fieldObj->isMultiValue = false;

                                fieldObj->_fieldName = xml.attributes().value("structName").toString();
                                fieldObj->_fieldType = xml.attributes().value("structType").toString();

                                if (xml.attributes().hasAttribute("format"))
                                    fieldObj->_fieldFormat = xml.attributes().value("format").toString();
                                if (xml.attributes().hasAttribute("scale"))
                                    fieldObj->_fieldScale = xml.attributes().value("scale").toString();

                                if (!builder->_fieldsNames.contains(fieldObj->_fieldName)){
                                    builder->_structFields.append(fieldObj);
                                    builder->_fieldsNames << fieldObj->_fieldName;

                                    valuesInSection << xml.attributes().value("structName").toString();
                                }
                            }
                            else if (xml.name() == "listdata" && token != QXmlStreamReader::EndElement){
                                QString curContainterName;
                                QStringList sl = xml.attributes().value("structType").toString().split(" ");
                                curContainterName = sl.last();

                                if (!containtersInState.keys().contains(curContainterName)){
                                    containerObject * cObj = new containerObject;
                                    cObj->_fieldNameAsField = xml.attributes().value("structName").toString();
                                    if (sl.first().contains("list",Qt::CaseInsensitive))
                                        cObj->_containerType = typeList;
                                    cObj->_objectNameAsType = curContainterName;
                                    cObj->_containerHashKey = curStateName;

                                    containtersInState[curContainterName] = cObj;
                                    builder->_structFields.append(cObj);
                                }
                                token = xml.readNext();
                                while (/*token != QXmlStreamReader::EndElement &&*/ xml.name() != "listdata"){

                                    if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                        fieldObject * fieldObj = new fieldObject;
                                        fieldObj->_hashKey = curStateName;
                                        fieldObj->_resultHashListKey = curValueName;
                                        if (attributes.hasAttribute("reqired"))
                                            fieldObj->required = attributes.value("reqired").toString() == "true";
                                        else
                                            fieldObj->required = false;

                                        if (attributes.hasAttribute("multiValue")){
                                            fieldObj->isMultiValue = true;
                                            fieldObj->_multiHashKeys.append(curValueName);
                                            fieldObj->_concatStr = attributes.hasAttribute("concat") ? attributes.value("concat").toString() : " ";
                                        }
                                        else
                                            fieldObj->isMultiValue = false;

                                        fieldObj->_fieldName = xml.attributes().value("structName").toString();
                                        fieldObj->_fieldType = xml.attributes().value("structType").toString();
                                        if (xml.attributes().hasAttribute("format"))
                                            fieldObj->_fieldFormat = xml.attributes().value("format").toString();
                                        if (xml.attributes().hasAttribute("scale"))
                                            fieldObj->_fieldScale = xml.attributes().value("scale").toString();
                                        containtersInState[curContainterName]->_inlineData.append(fieldObj);

                                        valuesInSection << xml.attributes().value("structName").toString();
                                    }
                                    else if (xml.name() == "structdata" && token != QXmlStreamReader::EndElement){
                                        QString curStructName = xml.attributes().value("structType").toString();
                                        if (!structsInState.keys().contains(curStructName)){
                                            structObject * sObj = new structObject;
                                            sObj->_objectNameAsType = curStructName;
                                            sObj->_fieldNameAsField = xml.attributes().value("structName").toString();
                                            structsInState[sObj->_objectNameAsType] = sObj;
                                            containtersInState[curContainterName]->_inlineData.append(sObj);
                                        }

                                        token = xml.readNext();
                                        while (token != QXmlStreamReader::EndElement && xml.name() != "structdata"){

                                            if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                                fieldObject * fieldObj = new fieldObject;
                                                fieldObj->_hashKey = curStateName;
                                                fieldObj->_resultHashListKey = curValueName;
                                                if (attributes.hasAttribute("reqired"))
                                                    fieldObj->required = attributes.value("reqired").toString() == "true";
                                                else
                                                    fieldObj->required = false;

                                                if (attributes.hasAttribute("multiValue")){
                                                    fieldObj->isMultiValue = true;
                                                    fieldObj->_multiHashKeys.append(curValueName);
                                                    fieldObj->_concatStr = attributes.hasAttribute("concat") ? attributes.value("concat").toString() : " ";
                                                }
                                                else
                                                    fieldObj->isMultiValue = false;

                                                fieldObj->_fieldName = xml.attributes().value("structName").toString();
                                                fieldObj->_fieldType = xml.attributes().value("structType").toString();
                                                if (xml.attributes().hasAttribute("format"))
                                                    fieldObj->_fieldFormat = xml.attributes().value("format").toString();
                                                if (xml.attributes().hasAttribute("scale"))
                                                    fieldObj->_fieldScale = xml.attributes().value("scale").toString();
                                                structsInState[curStructName]->_structFields.append(fieldObj);

                                                valuesInSection << xml.attributes().value("structName").toString();
                                            }
                                            token = xml.readNext();
                                        }
                                    }
                                    token = xml.readNext();
                                }

                            }
                            else if (xml.name() == "structdata" && token != QXmlStreamReader::EndElement){

                                QString curStructName = xml.attributes().value("structType").toString();
                                if (!structsInState.keys().contains(curStructName)){
                                    structObject * sObj = new structObject;
                                    sObj->_objectNameAsType = curStructName;
                                    sObj->_fieldNameAsField = xml.attributes().value("structName").toString();
                                    builder->_structFields.append(sObj);
                                    structsInState[sObj->_objectNameAsType] = sObj;
                                }

                                token = xml.readNext();
                                while (/*token != QXmlStreamReader::EndElement && */xml.name() != "structdata"){

                                    if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                        fieldObject * fieldObj = new fieldObject;
                                        fieldObj->_hashKey = curStateName;
                                        fieldObj->_resultHashListKey = curValueName;
                                        if (attributes.hasAttribute("reqired"))
                                            fieldObj->required = attributes.value("reqired").toString() == "true";
                                        else
                                            fieldObj->required = false;

                                        if (attributes.hasAttribute("multiValue")){
                                            fieldObj->isMultiValue = true;
                                            fieldObj->_multiHashKeys.append(curValueName);
                                            fieldObj->_concatStr = attributes.hasAttribute("concat") ? attributes.value("concat").toString() : " ";
                                        }
                                        else
                                            fieldObj->isMultiValue = false;

                                        fieldObj->_fieldName = xml.attributes().value("structName").toString();
                                        fieldObj->_fieldType = xml.attributes().value("structType").toString();
                                        if (xml.attributes().hasAttribute("format"))
                                            fieldObj->_fieldFormat = xml.attributes().value("format").toString();
                                        if (xml.attributes().hasAttribute("scale"))
                                            fieldObj->_fieldScale = xml.attributes().value("scale").toString();
                                        structsInState[curStructName]->_structFields.append(fieldObj);

                                        valuesInSection << xml.attributes().value("structName").toString();
                                    }
                                    token = xml.readNext();
                                }

                            }
                            else if (xml.name() == "db" && token != QXmlStreamReader::EndElement){
                                QXmlStreamAttributes db_attr = xml.attributes();
                                QString curTableName = dbTableName ;
                                if (db_attr.hasAttribute("table"))
                                    curTableName = db_attr.value("table").toString();
                                fieldDbData fdb;
                                fdb._db_fieldName = db_attr.value("dbfield").toString();
                                if (db_attr.hasAttribute("value"))
                                    fdb._value = db_attr.value("value").toString();
                                if (db_attr.hasAttribute("multiValue"))
                                    fdb.multiValue = db_attr.value("multiValue").toString() == "true";
                                if (db_attr.hasAttribute("concat"))
                                    fdb.concat  = db_attr.value("concat").toString();

                                if (valuesInSection.count() == 1){

                                    if (!structsInState.isEmpty() && valuesInSection.first().contains(":")){
                                        QStringList sl = valuesInSection.first().split(":");
                                        if (fdb._value.isEmpty())
                                            fdb._value = sl.last();
                                        structsInState[sl.first()]->dbData[curTableName].append(fdb);
                                    }
                                    else {
                                        if (fdb._value.isEmpty())
                                            fdb._value = valuesInSection.first();
                                        builder->dbData[curTableName].append(fdb);
                                    }
                                }
                                else if (!fdb._value.isEmpty()){
                                    foreach (const QString &value, valuesInSection) {
                                        if (!structsInState.isEmpty() && value.contains(":")){
                                            QStringList sl = value.split(":");
                                            structsInState[sl.first()]->dbData[curTableName].append(fdb);
                                        }
                                        else {
                                            builder->dbData[curTableName].append(fdb);
                                        }
                                    }
                                }
                                else
                                    throw std::invalid_argument("More than one data in value section without db value specification: " +
                                                                dbTableName.toStdString() + " " + valuesInSection.join(" ").toStdString());
                            }
                            token = xml.readNext();
                        }
                    }
                    token = xml.readNext();
                }
// end state section
            }
// end start element secion
        }
// end parese loop
    }
}

QByteArray ProtocolStructBuilder::makeStructDeclaration(const QString &structName, const QString &structParent,
                                                        const QList<baseFieldObject *> &structFields, bool mainStruct = false)
{
    QByteArray result;
    QTextStream tStream (&result, QIODevice::WriteOnly);
    tStream.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray prevDeclaration;

    tStream << "struct " << structName;
    tStream << QString(structParent.isEmpty() ? "" : QString(": public %1").arg(structParent));
    tStream << " {\n\n";
    for (auto * sFieled : structFields){
        switch (sFieled->getObjectType()) {
        case _objectType::SIMPLE:{
            fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
            if (fObj){
                tStream << makeStructFieldDeclaration(fObj->_fieldType,fObj->_fieldName);
            break;
            }
        }
        case _objectType::STRUCT:{
            structObject * sObj = dynamic_cast<structObject *>(sFieled);
            if (sObj){
                tStream << insertTab(1) << sObj->_objectNameAsType << " " << sObj->_fieldNameAsField << ";" << "\n";
                prevDeclaration.append(makeStructDeclaration(sObj->_objectNameAsType,QString(),sObj->_structFields,false));
            }
            break;
        }
        case _objectType::CONTAINER:{
            containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
            if (cObj){
                tStream << insertTab(1);
                switch (cObj->_containerType) {
                case containerType::typeList:{
                    tStream << "QList<" << cObj->_objectNameAsType << "> " << cObj->_fieldNameAsField;
                    break;
                }
                }
                tStream << ";" << "\n";
                if (!cObj->_inlineData.isEmpty()){
                    for (auto * sFieled : cObj->_inlineData){
                        if (sFieled->getObjectType() == STRUCT){
                            structObject * sObj = dynamic_cast<structObject *>(sFieled);
                            if (sObj){
                                prevDeclaration.append(makeStructDeclaration(sObj->_objectNameAsType,QString(),sObj->_structFields,false));
                            }
                        }
                    }
                }
            }
            break;
        }
        }
    }
    if (mainStruct){
        tStream << "\n";
        tStream << insertTab(1) << "void loadFromVariantHash(const QVariantHash &hash);" << "\n";
        tStream << insertTab(1) << "QVariantHash toVariantHash() const;" << "\n";
        tStream << insertTab(1) << "bool isValid() const;" << "\n";
    }
    tStream << "};" << "\n" << "\n";
    tStream.flush();
    result.insert(0,prevDeclaration);
    return result;
}

QString ProtocolStructBuilder::makeStructFieldDeclaration(const QString &fieldType, const QString &fieldName)
{
    QString result;
    if (fieldType == "int"){
        result = insertTab(1) + "int " + fieldName + ";" + "\n";
    }
    else if (fieldType == "string"){
        result = insertTab(1) + "QString " + fieldName + ";" + "\n";
    }
    else if (fieldType == "datetime"){
        result = insertTab(1) + "QDateTime " + fieldName + ";" + "\n";
    }
    else if (fieldType == "date"){
        result = insertTab(1) + "QDate" + fieldName + ";" + "\n";
    }
    else if (fieldType == "time"){
        result = insertTab(1) + "QTime " + fieldName + ";" + "\n";
    }
    else if (fieldType == "double"){
        result = insertTab(1) + "double " + fieldName + ";" + "\n";
    }
    else if (fieldType == "bool"){
        result = insertTab(1) + "bool " + fieldName + ";" + "\n";
    }
    else{
        result = insertTab(1) + fieldType + " " + fieldName + ";" + "\n";
    }
    return result;
}

void ProtocolStructBuilder::createHeaderFile(const QString &fileName)
{
    QFile *fHeader = new QFile(fileName);
    fHeader->open(QIODevice::WriteOnly);

    QByteArray mainData;
    QTextStream tStream (&mainData, QIODevice::WriteOnly | QIODevice::Append);
    tStream.setCodec(QTextCodec::codecForName("UTF-8"));

    QTextStream hStream (fHeader);
    QString outputFileName = fileName.mid(0,fileName.length()-2);

    hStream.setCodec(QTextCodec::codecForName("UTF-8"));

    hStream << "#ifndef " << outputFileName.toUpper() << "_H" << "\n";
    hStream << "#define " << outputFileName.toUpper() << "_H" << "\n" << "\n";
    hStream << "#include \"meteoData_protorype.h\"" << "\n" << "\n";

    for (auto &pStruct: _protocolData){
        hStream << makeStructDeclaration(pStruct._structName,pStruct._structParent,pStruct._structFields,true);
    }
    hStream << "#endif" << insertTab(1) << "// " << outputFileName.toUpper() << "_H" << "\n";

    fHeader->close();
    delete fHeader;
}

void ProtocolStructBuilder::createSourceFile(const QString &fileName)
{
    QFile *fSource = new QFile(fileName);
    fSource->open(QIODevice::WriteOnly);

    QTextStream sStream (fSource);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    QString headerFile = fileName.mid(0,fileName.length() - 4);

    sStream << "#include \"" << headerFile << ".h" << "\"" << "\n" << "\n";

    sStream << createLoadFromHashFunction();
    sStream << createToHashFunction();
    sStream << createIsValidFunction();

    fSource->close();
    delete fSource;
}

QByteArray ProtocolStructBuilder::makeFieldAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldType,
                                                      const QString &fieldName, const QString &fieldFormat, const QString &fieldScale)
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    sStream << insertTab(1) << "if (hash.keys().contains(\"" << hashKey << "\")){\n";
    sStream << insertTab(2) << "QVariantHash valuesHash = hash.value(\"" << hashKey << "\").toHash();\n";
    sStream << insertTab(2) << "if (valuesHash.keys().contains(\"" << internalHashKey << "\")){\n";

    sStream << insertTab(3) << fieldName << " = ";

    if (fieldType == "int"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toInt()";
        if (!fieldScale.isEmpty())
            sStream << " * "<< fieldScale;
    }
    else if (fieldType == "string"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toString()";
    }
    else if (fieldType == "datetime"){
        sStream << "QDateTime::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "date"){
        sStream << "QDate::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "time"){
        sStream << "QTime::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "double"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toDouble()";
        if (!fieldScale.isEmpty())
            sStream << " * "<< fieldScale;
    }
    else if (fieldType == "bool"){
        sStream << "valuesHash.keys().contains(\"" << internalHashKey << "\")";
    }
    else{
        sStream << "NULL";
    }
    sStream << ";\n";
    sStream << insertTab(2) << "}\n";
    if (fieldType == "int" || fieldType == "double"){
        sStream << insertTab(2) << "else" << "\n";
        sStream << insertTab(3) << fieldName << " = -100;" << "\n";
    }
    sStream << insertTab(1) << "}\n";
    if (fieldType == "int" || fieldType == "double"){
        sStream << insertTab(1) << "else" << "\n";
        sStream << insertTab(2) << fieldName << " = -100;" << "\n";
    }
    sStream << "\n";

    return result;
}

QByteArray ProtocolStructBuilder::makeStructListFieldAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldType,
                                                                const QString &fieldName, const QString &fieldFormat, const QString &fieldScale,
                                                                const QString &structType, const QString &structName, const QString &containerName,
                                                                bool createStruct, bool storeToContainer)
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    if (createStruct){
        sStream << insertTab(2) << "if (hash.keys().contains(\"" << hashKey << "\"+keyModificator)){\n";
        sStream << insertTab(3) << structType << " " << structName << ";\n";
        sStream << insertTab(3) << "QVariantHash valuesHash = hash.value(\"" << hashKey << "\"+keyModificator).toHash();\n";
    }
    sStream << insertTab(3) << "if (valuesHash.keys().contains(\"" << internalHashKey << "\")){\n";
    sStream << insertTab(4) << fieldName << " = ";

    if (fieldType == "int"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toInt()";
        if (!fieldScale.isEmpty())
            sStream << " * "<< fieldScale;
    }
    else if (fieldType == "string"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toString()";
    }
    else if (fieldType == "datetime"){
        sStream << "QDateTime::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "date"){
        sStream << "QDate::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "time"){
        sStream << "QTime::fromString(valuesHash.value(\"" << internalHashKey << "\").toString(), \"" << fieldFormat << "\")";
    }
    else if (fieldType == "double"){
        sStream << "valuesHash.value(\"" << internalHashKey << "\").toDouble()";
        if (!fieldScale.isEmpty())
            sStream << " * "<< fieldScale;
    }
    else if (fieldType == "bool"){
        sStream << "valuesHash.keys().contains(\"" << internalHashKey << "\")";
    }
    else{
        sStream << "NULL";
    }
    sStream << ";\n";
    sStream << insertTab(3) << "}\n";
    if (fieldType == "int" || fieldType == "double"){
        sStream << insertTab(3) << "else" << "\n";
        sStream << insertTab(4) << fieldName << " = -100;" << "\n";
    }

    if (storeToContainer){
        sStream << insertTab(3) << containerName << " << " << structName << ";\n";
        sStream << insertTab(2) << "}\n";
    }

    return result;
}

QByteArray ProtocolStructBuilder::makeFieldToHashAssignment(const QString &hashKey, const QString &internalHashKey, const QString &fieldName)
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    sStream << insertTab(1) << "if (result.keys().contains(\"" << hashKey << "\") )" << "\n";
    sStream << insertTab(2) << "valueHash = result.take(\"" << hashKey << "\").toHash();" << "\n";
    sStream << insertTab(1) << "valueHash[\"" << internalHashKey << "\"] = " << fieldName << ";\n";
    sStream << insertTab(1) << "result[\"" << hashKey << "\"] = valueHash;\n";
    sStream << insertTab(1) << "valueHash.clear();\n";
    sStream << "\n";

    return result;
}

QString ProtocolStructBuilder::makeFieldValidation(const QString &fieldType, const QString &fieldName)
{
    QString result;
    if (fieldType == "int" || fieldType == "double"){
        result = insertTab(1)+ "if (" + fieldName + " == -100) result = false;\n";
    }
    else if (fieldType == "string"){
        result = insertTab(1) + "if (" + fieldName + ".isEmpty()) result = false;\n";
    }
    else if (fieldType == "datetime"){
        result = insertTab(1) + "if (" + fieldName + ".isValid()) result = false;\n";
    }
    else if (fieldType == "date"){
        result = insertTab(1) + "if (" + fieldName + ".isValid()) result = false;\n";
    }
    else if (fieldType == "time"){
        result = insertTab(1) + "if (" + fieldName + ".isValid()) result = false;\n";
    }
    else if (fieldType == "bool"){
        result = insertTab(1) + "if (!" + fieldName + ") result = false;\n";
    }
    else{
        result = insertTab(1) + "//NULL" + fieldName + "\n";
    }
    return result;
}

QByteArray ProtocolStructBuilder::createLoadFromHashFunction()
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    for (auto &pStruct: _protocolData){
        sStream << "void " << pStruct._structName << "::loadFromVariantHash(const QVariantHash &hash){\n\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    sStream << makeFieldAssignment(fObj->_hashKey,fObj->_resultHashListKey,fObj->_fieldType,
                                                   fObj->_fieldName,fObj->_fieldFormat,fObj->_fieldScale);
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    for (auto * fieldObj : sObj->_structFields ){
                        fieldObject * fObj = dynamic_cast<fieldObject *>(fieldObj);
                        if (fObj){
                            sStream << makeFieldAssignment(fObj->_hashKey,fObj->_resultHashListKey,fObj->_fieldType,
                                                           sObj->_fieldNameAsField + "." + fObj->_fieldName,
                                                           fObj->_fieldFormat,fObj->_fieldScale);
                        }
                    }
                }
                break;
            }
            case _objectType::CONTAINER:{
                containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
                sStream << insertTab(1) << "int i = 1;" << "\n";
                sStream << insertTab(1) << "while (true){" << "\n";
                sStream << insertTab(2) << "QString keyModificator = \"\";" << "\n";
                sStream << insertTab(2) << "if (i != 1) keyModificator = QString(\"_%1\").arg(i);" << "\n";
                sStream << insertTab(2) << "i++;" << "\n";
                if (cObj){
                    if (!cObj->_inlineData.isEmpty()){
                        for (auto * sFieled : cObj->_inlineData){
                            if (sFieled->getObjectType() == STRUCT){
                                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                                if (sObj){
                                    int i = 1;
                                    bool createStruct = true;
                                    bool store = false;
                                    for (auto * fieldObj : sObj->_structFields ){
                                        fieldObject * fObj = dynamic_cast<fieldObject *>(fieldObj);
                                        if (fObj){
                                            if (i > 1)
                                                createStruct = false;
                                            if (i++ == sObj->_structFields.count())
                                                store = true;
                                            sStream << makeStructListFieldAssignment(fObj->_hashKey,fObj->_resultHashListKey,fObj->_fieldType,
                                                                                     sObj->_fieldNameAsField + "." + fObj->_fieldName,fObj->_fieldFormat,
                                                                                     fObj->_fieldScale,sObj->_objectNameAsType,
                                                                                     sObj->_fieldNameAsField, cObj->_fieldNameAsField, createStruct,store);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                sStream << insertTab(2) << "else break;" << "\n";
                sStream << insertTab(1) << "}" << "\n";
                break;
            }
            }
        }
        sStream << "}" << "\n" << "\n";
    }
    return result;
}

QByteArray ProtocolStructBuilder::createToHashFunction()
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    for (auto &pStruct: _protocolData){
        sStream << "QVariantHash " << pStruct._structName << "::toVariantHash() const{\n\n";
        sStream << insertTab(1) << "QVariantHash result;" << "\n";
        sStream << insertTab(1) << "QVariantHash valueHash;\n";
        sStream << "\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    sStream << makeFieldToHashAssignment(fObj->_hashKey,fObj->_resultHashListKey,fObj->_fieldName);
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    for (auto * fieldObj : sObj->_structFields ){
                        fieldObject * fObj = dynamic_cast<fieldObject *>(fieldObj);
                        if (fObj){
                            sStream << makeFieldToHashAssignment(fObj->_hashKey,fObj->_resultHashListKey,sObj->_fieldNameAsField + "." + fObj->_fieldName);
                        }
                    }
                }
                break;
            }
            case _objectType::CONTAINER:{
                containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
                if (cObj){
                    continue;
//                    TODO
                }
                break;
            }
            }
        }
        sStream << insertTab(1) << "return result;" << "\n";
        sStream << "}" << "\n" << "\n";
    }
    return result;
}

QByteArray ProtocolStructBuilder::createIsValidFunction()
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    for (auto &pStruct: _protocolData){
        sStream << "bool " << pStruct._structName << "::isValid() const{\n\n";
        sStream << insertTab(1) << "bool result = true;\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    if (fObj->required){
                        sStream << makeFieldValidation(fObj->_fieldType, fObj->_fieldName);
                    }
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    if (sObj){
                        for (auto * fieldObj : sObj->_structFields ){
                            fieldObject * fObj = dynamic_cast<fieldObject *>(fieldObj);
                            if (fObj){
                                sStream << makeFieldValidation(fObj->_fieldType,sObj->_fieldNameAsField + "." + fObj->_fieldName);
                            }
                        }
                    }
                }
                break;
            }
            case _objectType::CONTAINER:{
                containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
                if (cObj){
                    sStream << insertTab(1)<< "result = !" << cObj->_fieldNameAsField << ".isEmpty();\n";
// TODO: IS THAT ENOUGH?
                }
                break;
            }
            }
        }
        sStream << insertTab(1) << "return result;\n";
        sStream << "}" << "\n" << "\n";
    }

    return result;
}

ProtocolStructBuilder::ProtocolStructBuilder(const QString &ruleFilePath):
    _ruleFile(ruleFilePath)
{
    loadXmlData();
}

bool ProtocolStructBuilder::validate(const QString &ruleFile)
{
    bool res = true;

    QFile f(ruleFile);
    if (f.exists(ruleFile))
        f.open(QIODevice::ReadOnly);
    else {
        qDebug() << "incorrect path to meteoDecodeRules.xml";
        return false;
    }

    QXmlStreamReader xml(&f);

    QHash<int, QString>         meteoTypes;
    int _currentMeteo = -1;
    QHash<int, QHash<QString, QString> > connectedStates;

    while (!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name().toString() == "meteo"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasType = false;
                bool hasName = false;
                bool hasStruct = false;
                bool hasParent = false;
                bool hasUnknown = false;
                QStringList unknown;

                _currentMeteo = attributes.value("type").toString().toInt();

                if (attributes.hasAttribute("type") && attributes.hasAttribute("name"))
                    meteoTypes[attributes.value("type").toString().toInt()] = attributes.value("name").toString();

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "type")
                        hasType = true;
                    else if (attributes.at(i).name().toString() == "name")
                        hasName = true;
                    else if (attributes.at(i).name().toString() == "struct")
                        hasStruct = true;
                    else if (attributes.at(i).name().toString() == "parent")
                        hasParent = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasType){
                    qDebug() << "Error syntax in meteo section" << xml.text().toString() << xml.lineNumber() << "no Type";
                    res = false;
                }
                if (!hasName){
                    qDebug() << "Error syntax in meteo section " << xml.text().toString() << xml.lineNumber() << "no Name";
                    res = false;
                }
                if (!hasStruct){
                    qDebug() << "Error syntax in meteo section " << xml.text().toString() << xml.lineNumber() << "no Struct";
                    res = false;
                }
                if (!hasParent){
                    qDebug() << "Error syntax in meteo section " << xml.text().toString() << xml.lineNumber() << "no Parent";
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Warning! Unknown attribute in meteo section " << xml.text().toString() << xml.lineNumber() << unknown;
                }
            }
            if (xml.name().toString() == "state"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasName = false;
                bool hasField = false;
                bool hasPattern = false;
                bool hasRepeated = false;
                bool hasConcat = false;
                bool hasRequired = false;
                bool hasNext = false;
                bool hasUnknown = false;
                QStringList unknown;

                Q_UNUSED(hasRequired)
                Q_UNUSED(hasRepeated)
                Q_UNUSED(hasConcat)

                if (attributes.hasAttribute("name") && attributes.hasAttribute("next"))
                    connectedStates[_currentMeteo][attributes.value("name").toString()].append(attributes.value("next").toString());

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "field")
                        hasField = true;
                    else if (attributes.at(i).name().toString() == "name")
                        hasName = true;
                    else if (attributes.at(i).name().toString() == "pattern")
                        hasPattern = true;
                    else if (attributes.at(i).name().toString() == "concat")
                        hasConcat = true;
                    else if (attributes.at(i).name().toString() == "repeated")
                        hasRepeated = true;
                    else if (attributes.at(i).name().toString() == "required")
                        hasRequired = true;
                    else if (attributes.at(i).name().toString() == "next")
                        hasNext = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasField){
                    qDebug() << "Error syntax in state section " << xml.text().toString() << xml.lineNumber() << "no Field";
                    res = false;
                }
                if (!hasName){
                    qDebug() << "Error syntax in state section " << xml.text().toString() << xml.lineNumber() << "no Name";
                    res = false;
                }
                if (!hasPattern){
                    qDebug() << "Error syntax in state section " << xml.text().toString() << xml.lineNumber() << "no Pattern";
                    res = false;
                }
                if (!hasNext){
                    qDebug() << "Error syntax in state section " << xml.text().toString() << xml.lineNumber() << "no Next";
                    res = false;
                }
//                if (hasRepeated && !hasConcat){
//                    qDebug() << "Error in syntax " << xml.text().toString() << xml.lineNumber() << "no concat in Repeated field";
//                    res = false;
//                }
//                if (!hasRepeated && hasConcat){
//                    qDebug() << "Error in syntax " << xml.text().toString() << xml.lineNumber() << "no Repeated in concat field";
//                    res = false;
//                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in state section " << xml.text().toString() << xml.lineNumber() << unknown;
                }
            }
            if (xml.name().toString() == "key"){
                QXmlStreamAttributes attributes = xml.attributes();
                if (!attributes.hasAttribute("pattern")){
                    qDebug() << "Error syntax in key section " << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
            }
            if (xml.name().toString() == "value"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasPattern = false;
                bool hasField = false;
                bool hasRepeated = false;
                bool hasConcat = false;
                bool hasDel = false;
                bool hasReplace = false;
                bool hasMinimalCap = false;
                bool hasUnknown = false;
                QStringList unknown;

                Q_UNUSED(hasDel)
                Q_UNUSED(hasReplace)
                Q_UNUSED(hasMinimalCap)

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "field")
                        hasField = true;
                    else if (attributes.at(i).name().toString() == "pattern")
                        hasPattern = true;
                    else if (attributes.at(i).name().toString() == "repeated")
                        hasRepeated = true;
                    else if (attributes.at(i).name().toString() == "del")
                        hasDel = true;
                    else if (attributes.at(i).name().toString() == "concat")
                        hasConcat = true;
                    else if (attributes.at(i).name().toString() == "repeated")
                        hasRepeated = true;
                    else if (attributes.at(i).name().toString() == "replace")
                        hasReplace = true;
                    else if (attributes.at(i).name().toString() == "minimalCap")
                        hasMinimalCap = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasField){
                    qDebug() << "Warning syntax in value section " << xml.text().toString() << xml.lineNumber() << " no Field";
                }
                if (!hasPattern){
                    qDebug() << "Error syntax in key section " << xml.text().toString() << xml.lineNumber() << " no Pattern";
                    res = false;
                }
                if (hasRepeated && !hasConcat){
                    qDebug() << "Warinig syntax in value section " << xml.text().toString() << xml.lineNumber() << " no concat in Repeated field";
                }
                if (!hasRepeated && hasConcat){
                    qDebug() << "Error syntax in value section " << xml.text().toString() << xml.lineNumber() << " no Repeated in concat field";
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in value section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                }
            }
            if (xml.name().toString() == "listdata"){
                QXmlStreamAttributes attributes = xml.attributes();

                bool hasStructType = false;
                bool hasStructName = false;
                bool hasCreate = false;
                bool hasUnknown = false;
                QStringList unknown;

                Q_UNUSED(hasCreate)

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "structType")
                        hasStructType = true;
                    else if (attributes.at(i).name().toString() == "structName")
                        hasStructName = true;
                    else if (attributes.at(i).name().toString() == "create")
                        hasCreate = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasStructType
                        || !attributes.value("structType").toString().contains("list ",Qt::CaseInsensitive)){
                    res = false;
                    qDebug() << "Error syntax in listdata section " << xml.text().toString() << xml.lineNumber();
                }
                if (!hasStructName){
                    qDebug() << "Error syntax in listdata section" << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in listdata section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                }
            }
            if (xml.name().toString() == "structdata"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasStructType = false;
                bool hasStructName = false;
                bool hasUnknown = false;
                QStringList unknown;

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "structType")
                        hasStructType = true;
                    else if (attributes.at(i).name().toString() == "structName")
                        hasStructName = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasStructType){
                    qDebug() << "Error syntax in structdata section " << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                if (!hasStructName){
                    qDebug() << "Error syntax in structdata section" << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in structdata section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                }
            }
            if (xml.name().toString() == "subvalue"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasPattern = false;
                bool hasField = false;
                bool hasUnknown = false;
                bool hasDel = false;
                bool hasReplace = false;
                QStringList unknown;

                Q_UNUSED(hasDel)
                Q_UNUSED(hasReplace)

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "field")
                        hasField = true;
                    else if (attributes.at(i).name().toString() == "pattern")
                        hasPattern = true;
                    else if (attributes.at(i).name().toString() == "replace")
                        hasReplace = true;
                    else if (attributes.at(i).name().toString() == "del")
                        hasDel = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                    if (!hasField){
                        qDebug() << "Error syntax in subvalue section " << xml.text().toString() << xml.lineNumber() << " no Field";
                        res = false;
                    }
                    if (!hasPattern){
                        qDebug() << "Error syntax in subvalue section " << xml.text().toString() << xml.lineNumber() << " no Pattern";
                        res = false;
                    }
                    if (hasUnknown){
                        qDebug() << "Unknown attribute in subvalue section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                    }
            }
            if (xml.name().toString() == "data"){
                QXmlStreamAttributes attributes = xml.attributes();
                QStringList inlineFormats;
                inlineFormats << "int" << "date" << "time" << "datetime" << "float" << "string" << "bool" << "awosType" << "string *" << "RWY_SIDE";

                bool hasStructType = false;
                bool hasStructName = false;
                bool hasStruct     = false;
                bool hasFormat     = false;
                bool hasScale      = false;
                bool hasFromMain   = false;
                bool hasFromConst  = false;
                bool hasMultiValue = false;
                bool hasConcat     = false;
                bool hasUnknown    = false;
                QStringList unknown;

                Q_UNUSED(hasStruct)
                Q_UNUSED(hasFormat)
                Q_UNUSED(hasScale)
                Q_UNUSED(hasFromMain)
                Q_UNUSED(hasFromConst)
                Q_UNUSED(hasMultiValue)
                Q_UNUSED(hasConcat)

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name().toString() == "structType")
                        hasStructType = true;
                    else if (attributes.at(i).name().toString() == "structName")
                        hasStructName = true;
                    else if (attributes.at(i).name().toString() == "struct")
                        hasStruct = true;
                    else if (attributes.at(i).name().toString() == "format")
                        hasFormat = true;
                    else if (attributes.at(i).name().toString() == "scale")
                        hasScale = true;
                    else if (attributes.at(i).name().toString() == "fromMain")
                        hasFromMain = true;
                    else if (attributes.at(i).name().toString() == "multiValue")
                        hasMultiValue = true;
                    else if (attributes.at(i).name().toString() == "concat")
                        hasConcat = true;
                    else if (attributes.at(i).name().toString() == "fromConst")
                        hasFromConst = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }
                if (!hasStructType){
                    qDebug() << "Error syntax in data section " << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                else if (!inlineFormats.contains(attributes.value("structType").toString())){
                    qDebug() << "Error syntax in data section " << xml.text().toString() << xml.lineNumber() << "invalid inlineType" << attributes.value("structType").toString();
                    res = false;
                }
                if (!hasStructName){
                    qDebug() << "Error syntax in data section" << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in data section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                }

            }
            if (xml.name().toString() == "db"){
                QXmlStreamAttributes attributes = xml.attributes();
                bool hasMulti   = false;
                bool hasConcat  = false;
                bool hasDbField = false;
                bool hasTable   = false;
                bool hasValue   = false;
                bool hasUnknown    = false;
                QStringList unknown;

                Q_UNUSED(hasTable)
                Q_UNUSED(hasValue)

                for (int i=0;i<attributes.count();i++){
                    if (attributes.at(i).name() == "multiValue")
                        hasMulti = true;
                    else if (attributes.at(i).name() == "dbfield")
                        hasDbField = true;
                    else if (attributes.at(i).name() == "table")
                        hasTable = true;
                    else if (attributes.at(i).name() == "concat")
                        hasConcat = true;
                    else if (attributes.at(i).name() == "value")
                        hasValue = true;
                    else{
                        hasUnknown = true;
                        unknown << attributes.at(i).name().toString();
                    }
                }

                if (hasMulti && !hasConcat){
                    qDebug() << "Warning syntax in db section " << xml.text().toString() << xml.lineNumber() << "multiValue without concat";
                }
                if (!hasMulti && hasConcat){
                    qDebug() << "Warning syntax in db section " << xml.text().toString() << xml.lineNumber() << "concat without multiValue";
                }
                if (!hasDbField){
                    qDebug() << "Error syntax in db section " << xml.text().toString() << xml.lineNumber();
                    res = false;
                }
                if (hasUnknown){
                    qDebug() << "Unknown attribute in db section " << xml.text().toString() << xml.lineNumber() << ":\n" << unknown;
                }
            }
        }
    }

    foreach (const int& meteo, connectedStates.keys()) {
        QStringList objectsList = connectedStates[meteo].keys();
        QHash<QString, QString> hObj = connectedStates[meteo];

        foreach (const QString &key, hObj.keys()) {
            foreach (const QString &sKey, hObj.keys()) {
                if (key == hObj.value(sKey)){
                    objectsList.removeOne(key);
                    break;
                }
            }
        }
        if (objectsList.count() > 2){
            qDebug() << "In meteo section " << QString::number(meteo);
            qDebug() << "Missed next values for keys:\n" << objectsList;
        }
    }

    return res;
}

bool ProtocolStructBuilder::createStructs(const QString &outputFileName)
{
    createHeaderFile(outputFileName+".h");
    createSourceFile(outputFileName+".cpp");
    return true;
}

bool ProtocolStructBuilder::createPersister(const QString &ruleFile)
{
    Q_UNUSED(ruleFile)
    bool res = false;
    return res;
}

bool ProtocolStructBuilder::createTests(const QString &ruleFile)
{
    Q_UNUSED(ruleFile)
    bool res = false;
    return res;
}
