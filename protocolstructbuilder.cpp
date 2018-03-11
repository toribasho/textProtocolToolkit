#include "protocolstructbuilder.h"

QString instertTab(const int &count){
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

                            if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                fieldObject * fieldObj = new fieldObject;
                                fieldObj->_hashKey = curStateName;
                                fieldObj->_resultHashListKey = curValueName;
                                if (attributes.hasAttribute("required"))
                                    fieldObj->required = attributes.value("required").toString() == "true";
                                else
                                    fieldObj->required = false;
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
                                    cObj->_containerAsFieldName = xml.attributes().value("structName").toString();
                                    if (sl.first().contains("list",Qt::CaseInsensitive))
                                        cObj->_containerType = typeList;
                                    cObj->_exContainerName = curContainterName;

                                    containtersInState[curContainterName] = cObj;
                                    builder->_structFields.append(cObj);
                                }
                                token = xml.readNext();
                                while (/*token != QXmlStreamReader::EndElement &&*/ xml.name() != "listdata"){

                                    if (xml.name() == "data" && token != QXmlStreamReader::EndElement){
                                        fieldObject * fieldObj = new fieldObject;
                                        fieldObj->_hashKey = curStateName;
                                        fieldObj->_resultHashListKey = attributes.value("field").toString();
                                        if (attributes.hasAttribute("reqired"))
                                            fieldObj->required = attributes.value("reqired").toString() == "true";
                                        else
                                            fieldObj->required = false;
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
                                                fieldObj->_resultHashListKey = attributes.value("field").toString();
                                                if (attributes.hasAttribute("reqired"))
                                                    fieldObj->required = attributes.value("reqired").toString() == "true";
                                                else
                                                    fieldObj->required = false;
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
                                        fieldObj->_resultHashListKey = attributes.value("field").toString();
                                        if (attributes.hasAttribute("reqired"))
                                            fieldObj->required = attributes.value("reqired").toString() == "true";
                                        else
                                            fieldObj->required = false;
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

QByteArray ProtocolStructBuilder::makeStructDeclaration(structObject * sObj)
{
    QByteArray result;

    QTextStream hStream (&result, QIODevice::WriteOnly);
    hStream.setCodec(QTextCodec::codecForName("UTF-8"));

    Q_UNUSED(sObj)
    hStream << "struct " << sObj->_objectNameAsType;
    hStream << " {\n\n";
    for (auto * sFieled : sObj->_structFields){
        switch (sFieled->getObjectType()) {
        case _objectType::SIMPLE:{
            fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
            if (fObj){
                if (fObj->_fieldType == "int"){
                    hStream << "\t" << "int " << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "string"){
                    hStream << "\t" << "QString " << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "datetime"){
                    hStream << "\t" << "QDateTime " << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "date"){
                    hStream << "\t" << "QDate" << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "time"){
                    hStream << "\t" << "QTime " << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "double"){
                    hStream << "\t" << "double " << fObj->_fieldName << ";" << "\n";
                }
                else if (fObj->_fieldType == "bool"){
                    hStream << "\t" << "bool " << fObj->_fieldName << ";" << "\n";
                }
                else{
                    hStream << "\t" << fObj->_fieldType << " " << fObj->_fieldName << ";" << "\n";
                }
            }
            break;
        }
        default:
            break;
        }
    }
    hStream << "};" << "\n\n";
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

    QByteArray externalStructDeclaration;

    for (auto &pStruct: _protocolData){
        tStream << "struct " << pStruct._structName;
        tStream << QString(pStruct._structParent.isEmpty() ? "" : QString(": public %1").arg(pStruct._structParent));
        tStream << " {\n\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    if (fObj->_fieldType == "int"){
                        hStream << instertTab(1) << "int " << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "string"){
                        hStream << instertTab(1) << "QString " << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "datetime"){
                        hStream << instertTab(1) << "QDateTime " << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "date"){
                        hStream << instertTab(1) << "QDate" << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "time"){
                        hStream << instertTab(1) << "QTime " << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "double"){
                        hStream << instertTab(1) << "double " << fObj->_fieldName << ";" << "\n";
                    }
                    else if (fObj->_fieldType == "bool"){
                        hStream << instertTab(1) << "bool " << fObj->_fieldName << ";" << "\n";
                    }
                    else{
                        hStream << instertTab(1) << fObj->_fieldType << " " << fObj->_fieldName << ";" << "\n";
                    }
                break;
            }
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    hStream << instertTab(1) << sObj->_exStructName << " " << sObj->_structAsFieldName << ";" << "\n";
                    externalStructDeclaration.append(makeStructDeclaration(sObj));
                }
                break;
            }
            case _objectType::CONTAINER:{
                containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
                if (cObj){
                    hStream << instertTab(1);
                    switch (cObj->_containerType) {
                    case containerType::typeList:{
                        tStream << "QList<" << cObj->_exContainerName << "> " << cObj->_containerAsFieldName;
                        break;
                    }
                    }
                    tStream << ";" << "\n";
                    if (!cObj->_inlineData.isEmpty()){
                        for (auto * sFieled : cObj->_inlineData){
                            if (sFieled->getObjectType() == STRUCT)
                                externalStructDeclaration.append(makeStructDeclaration(dynamic_cast<structObject *>(sFieled)));
                        }
                    }
                }
                break;
            }
            }
        }
        hStream << "\n";
        hStream << instertTab(1) << "bool loadFromVariantHash(QVariantHash &hash);" << "\n";
        hStream << instertTab(1) << "QVariantHash toVariantHash() const;" << "\n";
        hStream << instertTab(1) << "bool isValid() const;" << "\n";
        hStream << "};" << "\n" << "\n";
        tStream.flush();
    }

    hStream << externalStructDeclaration;
    hStream << mainData;

    hStream << "#endif" << "\n";

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

QByteArray ProtocolStructBuilder::createLoadFromHashFunction()
{
    QByteArray result;

    QTextStream sStream (&result, QIODevice::WriteOnly);
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    for (auto &pStruct: _protocolData){
        sStream << "void " << pStruct._structName << "::loadFromVariantHash(const QVariantHash &hash){\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    sStream << instertTab(1) << "if (hash.keys().contains(\"" << fObj->_hashKey << "\")){\n";
                    sStream << instertTab(2) << "QVariantHash valuesHash = hash.value(\"" << fObj->_hashKey << "\").toHash();\n";
                    sStream << instertTab(2) << "if (valuesHash.keys().contains(\"" << fObj->_resultHashListKey << "\")){\n";
                    sStream << instertTab(3) << fObj->_fieldName << " = ";
                    if (fObj->_fieldType == "int"){
                        sStream << "valueHash.value(\"" << fObj->_resultHashListKey << "\").toInt()";
                        if (!fObj->_fieldScale.isEmpty())
                            sStream << " * "<< fObj->_fieldScale;
                    }
                    else if (fObj->_fieldType == "string"){
                        sStream << "valueHash.value(\"" << fObj->_resultHashListKey << "\").toString()";
                    }
                    else if (fObj->_fieldType == "datetime"){
                        sStream << "QDateTime::fromString(valueHash.value(\"" << fObj->_resultHashListKey << "\").toString(), \"" << fObj->_fieldFormat << "\")";
                    }
                    else if (fObj->_fieldType == "date"){
                        sStream << "QDate::fromString(valueHash.value(\"" << fObj->_resultHashListKey << "\").toString(), \"" << fObj->_fieldFormat << "\")";
                    }
                    else if (fObj->_fieldType == "time"){
                        sStream << "QTime::fromString(valueHash.value(\"" << fObj->_resultHashListKey << "\").toString(), \"" << fObj->_fieldFormat << "\")";
                    }
                    else if (fObj->_fieldType == "double"){
                        sStream << "valueHash.value(\"" << fObj->_resultHashListKey << "\").toDouble()";
                        if (!fObj->_fieldScale.isEmpty())
                            sStream << " * "<< fObj->_fieldScale;
                    }
                    else if (fObj->_fieldType == "bool"){
                        sStream << "valuesHash.keys().contains(\"" << fObj->_resultHashListKey << "\")";
                    }
                    else{
                        sStream << "NULL";
                    }
                    sStream << ";\n";
                    sStream << instertTab(2) << "}\n";
                    if (fObj->_fieldType == "int" || fObj->_fieldType == "double"){
                        sStream << instertTab(2) << "else" << "\n";
                        sStream << instertTab(3) << fObj->_fieldName << " = -100;" << "\n";
                    }
                    sStream << instertTab(1) << "}\n";
                    if (fObj->_fieldType == "int" || fObj->_fieldType == "double"){
                        sStream << instertTab(1) << "else" << "\n";
                        sStream << instertTab(2) << fObj->_fieldName << " = -100;" << "\n";
                    }
                    sStream << "\n";
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    continue;
//                    TODO
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
        sStream << "QVariantHash " << pStruct._structName << "::toVariantHash() const{\n";
        sStream << instertTab(1) << "QVariantHash result;" << "\n";
        sStream << instertTab(1) << "QVaraintHash valueHash;\n";
        sStream << "\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    sStream << instertTab(1) << "if (result.keys().contains(\"" << fObj->_hashKey << "\")" << "\n";
                    sStream << instertTab(2) << "valueHash = result.take(\"" << fObj->_hashKey << "\").toHash();" << "\n";
                    sStream << instertTab(1) << "valueHash[\"" << fObj->_resultHashListKey << "\"] = " << fObj->_fieldName << ";\n";
                    sStream << instertTab(1) << "result[\"" << fObj->_hashKey << "\"] = valueHash;\n";
                    sStream << instertTab(1) << "valueHash.clear();\n";
                    sStream << "\n";
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    continue;
//                    TODO
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
        sStream << instertTab(1) << "return result;" << "\n";
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
        sStream << "bool " << pStruct._structName << "::isValid() const{\n";
        sStream << "\tbool result = true;\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:{
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    if (fObj->required){
                        if (fObj->_fieldType == "int" || fObj->_fieldType == "double"){
                            sStream << instertTab(1)<< "if (" << fObj->_fieldName << " == -100) reslut = false;\n";
                        }
                        else if (fObj->_fieldType == "string"){
                            sStream << instertTab(1) << "if (" << fObj->_fieldName << ".isEmpty()) reslut = false;\n";
                        }
                        else if (fObj->_fieldType == "datetime"){
                            sStream << instertTab(1) << "if (" << fObj->_fieldName << ".isValid()) reslut = false;\n";
                        }
                        else if (fObj->_fieldType == "date"){
                            sStream << instertTab(1) << "if (" << fObj->_fieldName << ".isValid()) reslut = false;\n";
                        }
                        else if (fObj->_fieldType == "time"){
                            sStream << instertTab(1) << "if (" << fObj->_fieldName << ".isValid()) reslut = false;\n";
                        }
                        else if (fObj->_fieldType == "bool"){
                            sStream << instertTab(1) << "if (!" << fObj->_fieldName << ") reslut = false;\n";
                        }
                        else{
                            sStream << instertTab(1) << "//NULL" << fObj->_fieldName << "\n";
                        }
                    }
                }
                break;
            }
            case _objectType::STRUCT:{
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    continue;
//                    TODO
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
        sStream << instertTab(1) << "return result;\n";
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
