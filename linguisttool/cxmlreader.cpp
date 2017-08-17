#include "cxmlreader.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QDir>

CXmlReader::CXmlReader()
{

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

bool CXmlReader::convert(const QString &sourceFilePath, const QString &languageDataFilePath, const QString outPath)
{
    QDir dir(outPath);
    if(!dir.exists())
    {
        return false;
    }
    bool isOk = false;
    isOk = initLanguageMap(languageDataFilePath);
    if(!isOk)
    {
        return false;
    }
    isOk = read(sourceFilePath);
    if(!isOk)
    {
        return false;
    }
    QList<TS_DATA::LanguageType> typeList;
//    typeList.append(TS_DATA::CN);
    typeList.append(TS_DATA::TW);
    typeList.append(TS_DATA::US);
    for(auto type:typeList)
    {
        m_languageType = type;
        if(!save(QString("%1/ljlive_%2.ts").arg(outPath).arg(getCurrentLanguage())))
        {
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

    if(!m_notFoundString.contains(key))
    {
        m_notFoundString.insert(key,key);
        log(QString("key=%1 count=%2").arg(key).arg(m_notFoundString.count()));
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
    if(obj.data()->translation.isEmpty()
            || obj.data()->translation_type == "vanished")
    {
        return;
    }
    xml.writeStartElement("message");

    xml.writeStartElement("location");
    xml.writeAttribute("filename",obj.data()->location.filename);
    xml.writeAttribute("line",obj.data()->location.line);
    xml.writeEndElement();

    xml.writeTextElement("source", obj.data()->source);


    xml.writeStartElement("translation");
    QString translationLanguage = getLanguageString(obj.data()->translation);
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
    while(!file.atEnd())
    {
        QString lineString = file.readLine();
        QStringList langes = lineString.split("#----#");
        if(langes.count() == 3)
        {
            TS_DATA::LanguageData obj;
            obj.language_CN=langes.at(0);
            obj.language_TW=langes.at(1);
            obj.language_US=langes.at(2);
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
                m_languageDataMap.insert(obj.language_CN,obj);
            }
        }
        else
        {
            log(QString("lineString=%1 is invalid").arg(lineString));
        }
    }
    file.close();
    log(QString("keys=%1").arg(QStringList(m_languageDataMap.keys()).join("----")));
    return true;
}

