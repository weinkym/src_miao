#include "cxmlreader.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QDir>

CXmlReader::CXmlReader()
{
    m_exceptKeyList.append("CheckBox");
    m_exceptKeyList.append("TextLabel");

    m_exceptKeyList.append(".MainWindow");
    m_exceptKeyList.append("0%");
    m_exceptKeyList.append("10%");
    m_exceptKeyList.append("Del");
    m_exceptKeyList.append("Ctrl+F");
    m_exceptKeyList.append("Ctrl+S");
    m_exceptKeyList.append("Ctrl+R");
    m_exceptKeyList.append("Ctrl+D");
    m_exceptKeyList.append("Ctrl+Up");
    m_exceptKeyList.append("Ctrl+Down");
    m_exceptKeyList.append("Ctrl+Home");
    m_exceptKeyList.append("Ctrl+End");
    m_exceptKeyList.append("copySource");
    m_exceptKeyList.append("PasteRef");
    m_exceptKeyList.append("PasteDup");
    m_exceptKeyList.append("Configure");
    m_exceptKeyList.append("CopySource");
    m_exceptKeyList.append("PasteFilters");
    m_exceptKeyList.append("autoConfigure");
    m_exceptKeyList.append("addIconSmall");
    m_exceptKeyList.append("removeIconSmall");
    m_exceptKeyList.append("upArrowIconSmall");
    m_exceptKeyList.append("downArrowIconSmall");
    m_exceptKeyList.append("44.1khz");
    m_exceptKeyList.append("48khz");
    m_exceptKeyList.append("24 NTSC");
    m_exceptKeyList.append("I444");
}

bool CXmlReader::read(const QString &sourceFilePath)
{
    QFile sourceFile(sourceFilePath);
    qDebug()<<__LINE__;
    if(!sourceFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    m_contextList.clear();
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(&sourceFile, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug()<<QString("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr);
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "TS") {
        qDebug()<<QString("The file is not an TS file.");
        return false;
    }

    QDomElement child = root.firstChildElement("context");
    while (!child.isNull()) {
        m_contextList.append(parseContextElement(child));
        child = child.nextSiblingElement();
    }
    return true;
}


QSharedPointer<TS_DATA::Context> CXmlReader::parseContextElement(const QDomElement &element)
{
    QSharedPointer<TS_DATA::Context> obj(new TS_DATA::Context);
    if(element.isNull())
    {
        return  obj;
    }
//    context.name = element.firstChildElement("name").text();

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "name") {
            obj.data()->name = child.text();
        }
        else if (child.tagName() == "message")
        {
           obj.data()->messages.append(parseMessageElement(child));
        }
        child = child.nextSiblingElement();
    }
    return  obj;
}

QSharedPointer<TS_DATA::Message> CXmlReader::parseMessageElement(const QDomElement &element)
{
    QSharedPointer<TS_DATA::Message> obj(new TS_DATA::Message);
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "location") {
            obj.data()->location.filename = child.attribute("filename");
            obj.data()->location.line = child.attribute("line");
        }
        else if (child.tagName() == "source")
        {
            obj.data()->source = child.text();
        }
        else if (child.tagName() == "translation")
        {
            obj.data()->translation = child.text();
            obj.data()->translation_type = child.attribute("type");
        }
        child = child.nextSiblingElement();
    }
    return obj;
}

bool CXmlReader::convert(const QString &sourceFilePath, const QString &languageDataFilePath,
                         const QString outPath, const QString &obsFilePath)
{
    QDir dir(outPath);
    if(!dir.exists())
    {
        log(QString("dir.exists() path=%1").arg(outPath));
        return false;
    }
    dir.setPath(obsFilePath);
    if(!dir.exists())
    {
        log(QString("dir.exists() path=%1").arg(obsFilePath));
        return false;
    }
    bool isOk = false;
    isOk = initLanguageMap(languageDataFilePath);
    if(!isOk)
    {
        log(QString("initLanguageMap is false path=%1").arg(languageDataFilePath));
        return false;
    }
    isOk = initLanguageOBSMap(obsFilePath);
    if(!isOk)
    {
        log(QString("initLanguageOBSMap is false path=%1").arg(obsFilePath));
        return false;
    }
//    return false;
    //
    isOk = read(sourceFilePath);
    if(!isOk)
    {
        log(QString("read is false path=%1").arg(sourceFilePath));
        return false;
    }
    //todo
//    testCheck(outPath);
    QList<TS_DATA::LanguageType> typeList;
    typeList.append(TS_DATA::CN);
    typeList.append(TS_DATA::TW);
    typeList.append(TS_DATA::US);
    for(auto type:typeList)
    {
        m_languageType = type;
        if(!save(QString("%1/ljlive_%2.ts").arg(outPath).arg(getCurrentLanguage())))
        {
            log(QString("save false outPath=%1").arg(outPath));
            isOk = false;
            break;
        }
    }
    return isOk;
}

bool CXmlReader::save(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    xml.setDevice(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xbel>");
    xml.writeStartElement("TS");
    xml.writeAttribute("version", "2.1");
    xml.writeAttribute("language", "zh_CN");
    xml.writeAttribute("sourcelanguage", "en");

    for(auto obj:m_contextList)
    {
        writeContext(obj);
    }
    xml.writeEndElement();

    xml.writeEndDocument();
    return true;
}

QString CXmlReader::getCurrentLanguage()
{
    switch (m_languageType)
    {
    case TS_DATA::CN:
        return "zh_CN";
        break;
    case TS_DATA::TW:
        return "zh_TW";
        break;
    case TS_DATA::US:
        return "en_US";
        break;
    default:
        break;
    }
    return "CN";
}

QString CXmlReader::getLanguageString(const QString &key)
{
    if(m_languageDataMap.contains(key))
    {
        switch (m_languageType)
        {
        case TS_DATA::CN:
            return m_languageDataMap.value(key).language_CN;
            break;
        case TS_DATA::TW:
            return m_languageDataMap.value(key).language_TW;
            break;
        case TS_DATA::US:
            return m_languageDataMap.value(key).language_US.simplified();
            break;
        default:
            return m_languageDataMap.value(key).language_CN;
            break;
        }
        return m_languageDataMap.value(key).language_CN;
    }
    if(m_languageType == TS_DATA::CN && m_languageOBS_CNDataMap.contains(key))
    {
        return m_languageOBS_CNDataMap.value(key);
    }
    if(m_languageType == TS_DATA::TW && m_languageOBS_TWDataMap.contains(key))
    {
        return m_languageOBS_TWDataMap.value(key);
    }
    if(m_languageType == TS_DATA::US && m_languageOBS_USDataMap.contains(key))
    {
        return m_languageOBS_USDataMap.value(key);
    }
    {
        bool ok = false;
        key.toInt(&ok);
        if(!ok)
        {
            key.toDouble(&ok);
        }
        if(ok)
        {
            return key;
        }
    }
    if(m_exceptKeyList.contains(key))
    {
        return key;
    }


    if(!m_notFoundString.contains(key))
    {
        m_notFoundString.insert(key,key);
        log(QString("m_notFoundString key=%1 count=%2").arg(key).arg(m_notFoundString.count()));
    }
//    static int count  = 0;
//    log(QString("count=%1").arg(count++));
    //warning
    return "";
}

void CXmlReader::writeContext(const QSharedPointer<TS_DATA::Context> &obj)
{
    xml.writeStartElement("context");
    xml.writeTextElement("name", obj.data()->name);
    for(auto msg:obj.data()->messages)
    {
        writeMessage(msg);
    }
    xml.writeEndElement();
}

void CXmlReader::writeMessage(const QSharedPointer<TS_DATA::Message> &obj)
{
//    if(obj.data()->translation.isEmpty()
//            || obj.data()->translation_type == "vanished")
//    {
//        return;
//    }
    xml.writeStartElement("message");

    xml.writeStartElement("location");
    xml.writeAttribute("filename",obj.data()->location.filename);
    xml.writeAttribute("line",obj.data()->location.line);
    xml.writeEndElement();

    xml.writeTextElement("source", obj.data()->source);


    xml.writeStartElement("translation");
    QString translationLanguage = getLanguageString(obj.data()->source);
//    translationLanguage = ;
    if(translationLanguage.isEmpty())
    {
//        log(QString("key=%1 source=%2").arg(obj.data()->translation).arg(obj.data()->source));
        xml.writeAttribute("type","unfinished");
    }
    //setAutoFormatting =true会在结尾换行
    xml.setAutoFormatting(false);
    xml.writeCharacters(translationLanguage);
    xml.setAutoFormatting(true);

    xml.writeEndElement();

    xml.writeEndElement();
}

void CXmlReader::log(const QString &text)
{
    qDebug()<<text;
}

bool CXmlReader::initLanguageMap(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    int lineNum = 0;
    while(!file.atEnd())
    {
        lineNum++;
        QString lineString = file.readLine();
        QStringList langes = lineString.split("#----#");
        if(langes.count() == 4)
        {
            QString key = langes.at(0);
            if(key.isEmpty())
            {
                log(QString("key is empty line=%1").arg(lineNum));
                continue;
            }
            TS_DATA::LanguageData obj;
            obj.language_CN=langes.at(1);
            obj.language_TW=langes.at(2);
            obj.language_US=langes.at(3);
            if(obj.language_CN.isEmpty()
                    || obj.language_TW.isDetached()
                    || obj.language_US.isEmpty())
            {
                log(QString("language_CN=%1,language_TW=%2，language_US=%3 is empty")
                    .arg(obj.language_CN)
                    .arg(obj.language_TW)
                    .arg(obj.language_US));
            }
            else
            {
                m_languageDataMap.insert(key,obj);
            }
        }
        else
        {
            log(QString("lineString=%1 is invalid").arg(lineString));
        }
    }
    file.close();
//    log(QString("keys=%1").arg(QStringList(m_languageDataMap.keys()).join("----")));
    return true;
}

bool CXmlReader::initLanguageOBSMap(const QString &filePath)
{
    bool ok = false;
    ok = initLanguageOBSMap(filePath+"/zh-CN.ini",m_languageOBS_CNDataMap);
//    log(QString("m_languageOBS_CNDataMap keys=%1").arg(QStringList(m_languageOBS_CNDataMap.keys()).join("----")));
    if(!ok)
    {
        return false;
    }
    ok = initLanguageOBSMap(filePath+"/zh-TW.ini",m_languageOBS_TWDataMap);
//    log(QString("m_languageOBS_TWDataMap keys=%1").arg(QStringList(m_languageOBS_TWDataMap.keys()).join("----")));
    if(!ok)
    {
        return false;
    }
    ok = initLanguageOBSMap(filePath+"/en-US.ini",m_languageOBS_USDataMap);
//    log(QString("m_languageOBS_USDataMap keys=%1").arg(QStringList(m_languageOBS_USDataMap.keys()).join("----")));
    return ok;
}

bool CXmlReader::initLanguageOBSMap(const QString &filePath, QMap<QString, QString> &objMap)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    int lineNum = 0;
    log(QString("initLanguageOBSMap filePath=%1").arg(filePath));

    while(!file.atEnd())
    {
        lineNum++;
        QString lineString = file.readLine();
        lineString = lineString.simplified();
        if(lineString.startsWith("#")
                || lineString.isEmpty())
        {
            continue;
        }

        int index = lineString.indexOf("=");
        if(index < 0)
        {
            log(QString("line=%1,index=%2,lineString=%3").arg(lineNum).arg(index).arg(lineString));
            continue;
        }
        QString source = lineString.left(index);
        if(source.isEmpty())
        {
            log(QString("line=%1,source.isEmpty(),lineString=%2").arg(lineNum).arg(lineString));
            continue;
        }
        QString key = lineString.right(lineString.length() - index - 1);
        if(key.startsWith("\"")&&key.endsWith("\""))
        {
            QString value = key.mid(1,key.length() - 2);
            if(value.isEmpty())
            {
                log(QString("line=%1,value.isEmpty(),lineString=%2").arg(lineNum).arg(lineString));
                continue;
            }
            if(!objMap.contains(source))
            {
                objMap.insert(source,value);
            }
            else
            {
                log(QString("line=%1,source is repetition,source=%2").arg(lineNum).arg(source));
            }
        }
        else
        {
            log(QString("line=%1,key is invalid,key=%2").arg(lineNum).arg(key));
        }
    }
    log(QString("mapcount = %1").arg(objMap.count()));
    return true;
}

void CXmlReader::testCheck(const QString &outPath)
{
    QMap<QString,QStringList> keyList;
    QFile file(outPath+"/ljsourc.txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    for(auto obj:m_contextList)
    {
        for(auto msg:obj.data()->messages)
        {
            if(msg.data()->translation.isEmpty()
                    || msg.data()->translation_type == "vanished")
            {
                continue;
            }
            if(!keyList.contains(msg.data()->source))
            {
                keyList.insert(msg.data()->source,QStringList());
            }
            else
            {
                continue;
            }
            if(keyList.value(msg.data()->source).count() > 0
                    && !keyList.value(msg.data()->source).contains(msg.data()->translation))
            {
                keyList[msg.data()->source].append(msg.data()->translation);
                log(QString("key has mutl %1")+(msg.data()->source)+keyList.value(msg.data()->source).join("====="));
            }
            else
            {
                keyList[msg.data()->source].append(msg.data()->translation);
            }
            QStringList strList;
            strList.append(obj.data()->name);
            strList.append(msg.data()->source);
            strList.append(msg.data()->translation);
            QString lienString = strList.join("#----#");
            file.write(lienString.toLocal8Bit());
            file.write("\n");
        }
    }

}

