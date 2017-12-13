#include "protocolstructbuilder.h"

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

                mainDbData mainDb;
                mainDb._db_table = attributes.value("name").toString();

                _protocolData.append(meteoProtocollClass);
                _db_data.append(mainDb);

                builder = &_protocolData[_protocolData.size()-1];
                mDb = &_db_data[_db_data.size()-1];
            }
            if (xml.name().toString() == "state"){
                QXmlStreamAttributes attributes = xml.attributes();
                QString curStateName = attributes.value("field").toString();

                token = xml.readNext();
                if (xml.name().toString() == "state"){
                    ignoreReadNext = true;
                    continue;
                }
                QString curValueName;

                QHash<QString , containerObject *>  containtersInState;
                QHash<QString , structObject *>     structsInState;

                while (token != QXmlStreamReader::EndElement && xml.name() == "state"){
                    if (xml.name().toString() == "value"){
                        QStringList valuesInSection;
                        if (xml.attributes().hasAttribute("field"))
                            curValueName = xml.attributes().value("field").toString();

                        while (token != QXmlStreamReader::EndElement && xml.name() == "value"){

                            if (xml.name() == "data"){
                                fieldObject * fieldObj = new fieldObject;
                                fieldObj->_hashKey = curStateName;
                                fieldObj->_resultHashListKey = curValueName;
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
                                builder->_structFields.append(fieldObj);

                                valuesInSection << xml.attributes().value("structName").toString();
                            }
                            else if (xml.name() == "listdata"){
                                QString curContainterName;
                                QStringList sl = xml.attributes().value("structType").toString().split(" ");
                                curContainterName = sl.last();

                                if (!containtersInState.keys().contains(curContainterName)){
                                    containerObject * cObj = new containerObject;
                                    cObj->_containerAsFieldName = xml.attributes().value("structName").toString();
                                    if (sl.first().contains("list",Qt::CaseInsensitive))
                                        cObj->_containerType = containerType::typeList;
                                    cObj->_exContainerName = curContainterName;

                                    containtersInState[curContainterName] = cObj;
                                    builder->_structFields.append(cObj);
                                }
                                else{
                                    while (token != QXmlStreamReader::EndElement && xml.name() != "listdata"){

                                        if (xml.name() == "data"){
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
                                        else if (xml.name() == "structdata"){
                                            QString curStructName = xml.attributes().value("structType").toString();
                                            if (!structsInState.keys().contains(curStructName)){
                                                structObject * sObj = new structObject;
                                                sObj->_exStructName = curStructName;
                                                sObj->_structAsFieldName = xml.attributes().value("structName").toString();
                                                structsInState[sObj->_exStructName] = sObj;
                                                containtersInState[curContainterName]->_inlineData.append(sObj);
                                            }

                                            while (token != QXmlStreamReader::EndElement && xml.name() != "structdata"){

                                                if (xml.name() == "data"){
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
                            }
                            else if (xml.name() == "structdata"){

                                QString curStructName = xml.attributes().value("structType").toString();
                                if (!structsInState.keys().contains(curStructName)){
                                    structObject * sObj = new structObject;
                                    sObj->_exStructName = curStructName;
                                    sObj->_structAsFieldName = xml.attributes().value("structName").toString();
                                    builder->_structFields.append(cObj);
                                    structsInState[sObj->_exStructName] = sObj;
                                }

                                while (token != QXmlStreamReader::EndElement && xml.name() != "structdata"){

                                    if (xml.name() == "data"){
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
                            else if (xml.name() == "db"){
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
                                        structsInState[sl.first()]->dbData[curTableName] = fdb;
                                    }
                                    else {
                                        if (fdb._value.isEmpty())
                                            fdb._value = valuesInSection.first();
                                        builder->dbData[curTableName] = fdb;
                                    }
                                }
                                else if (!fdb._value.isEmpty()){
                                    foreach (const QString &value, valuesInSection) {
                                        if (!structsInState.isEmpty() && value.contains(":")){
                                            QStringList sl = value.split(":");
                                            structsInState[sl.first()]->dbData[curTableName] = fdb;
                                        }
                                        else {
                                            builder->dbData[curTableName] = fdb;
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
                }
// end state section
            }
// end start element secion
        }
// end parese loop
    }
}

QString ProtocolStructBuilder::makeStructDeclaration(structObject * sObj)
{
    QString resultSturctDeclaration;
    Q_UNUSED(sObj)
    return resultSturctDeclaration;
}

bool ProtocolStructBuilder::validate(const QString &ruleFile)
{
    bool res = true;

    QFile f(ruleFile);
    if (f.exists(ruleFile))
        f.open(QIODevice::ReadOnly);
    else {
        qDebug() << "incorrect path to meteoDecodeRules.xml";
        return res;
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
                bool hasUnknown    = false;
                QStringList unknown;

                Q_UNUSED(hasStruct)
                Q_UNUSED(hasFormat)
                Q_UNUSED(hasScale)
                Q_UNUSED(hasFromMain)
                Q_UNUSED(hasFromConst)
                Q_UNUSED(hasMultiValue)

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
    bool res = true;

    QFile *fHeader = new QFile(outputFileName+".h");
    QFile *fSource = new QFile(outputFileName+".cpp");

    fHeader->open(QIODevice::WriteOnly);
    fSource->open(QIODevice::WriteOnly);

    QTextStream hStream (fHeader);
    QTextStream sStream (fSource);

    hStream.setCodec(QTextCodec::codecForName("UTF-8"));
    sStream.setCodec(QTextCodec::codecForName("UTF-8"));

    hStream << "#ifndef " << outputFileName.toUpper() << "_H" << "\n";
    hStream << "#define " << outputFileName.toUpper() << "_H" << "\n" << "\n";
    hStream << "#include \"meteoData_protorype.h\"" << "\n" << "\n";

    sStream << "#include \"" << fHeader->fileName() << "\"" << "\n" << "\n";

    int headPosition = hStream.pos();
    QString externalStructDeclaration = "";

    for (auto &pStruct: _protocolData){
        hStream << "struct " << pStruct._structName << pStruct._structParent.isEmpty() ? "" : QString(": " + pStruct._structParent) << " {" << "\n" << "\n";
        for (auto * sFieled : pStruct._structFields){
            switch (sFieled->getObjectType()) {
            case _objectType::SIMPLE:
                fieldObject * fObj = dynamic_cast<fieldObject *>(sFieled);
                if (fObj){
                    hStream << "\t" << fObj->_fieldType << " " << fObj->_fieldName << ";" << "\n";
                }
                break;
            case _objectType::STRUCT:
                structObject * sObj = dynamic_cast<structObject *>(sFieled);
                if (sObj){
                    hStream << "\t" << sObj->_exStructName << " " << sObj->_structAsFieldName << ";" << "\n";
                    externalStructDeclaration.append(makeStructDeclaration(sObj));
                }
                break;
            case _objectType::CONTAINER:
                containerObject * cObj = dynamic_cast<containerObject *>(sFieled);
                if (cObj){
                    hStream << "\t";
                    switch (cObj->_containerType) {
                    case typeList:
                        hStream << "QList<" << cObj->_exContainerName << "> " << cObj->_containerAsFieldName;
                        break;
                    }
                    hStream << ";" << "\n";
                    if (cObj->_internalField->getObjectType() == _objectType::STRUCT){
                        externalStructDeclaration.append(makeStructDeclaration(dynamic_cast<containerObject *>(cObj->_internalField)));
                    }
                }
                break;
            }
        }

        hStream << "\t" << "bool loadFromVariantHash(QVariantHash &hash);" << "\n";
        hStream << "\t" << "QVariantHash toVariantHash() const;" << "\n";
        hStream << "\t" << "bool isValid() const;" << "\n";
        hStream << "}" << "\n" << "\n";
    }

    hStream << "#endif" << "\n";

    hStream.seek(headPosition);
    hStream << externalStructDeclaration;

    fSource->close();
    fHeader->close();
    return res;
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
