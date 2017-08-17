#ifndef CXMLREADER_H
#define CXMLREADER_H
#include <QDebug>

#include <QDomDocument>
#include <QXmlStreamWriter>

namespace TS_DATA {

struct Location
{
    QString filename;
    QString line;
};

struct Message
{
    Location location;
    QString source;
    QString translation;
    QString translation_type;
};

struct Context
{
    QString name;
    QList<QSharedPointer<TS_DATA::Message> > messages;
};

struct LanguageData
{
    QString language_CN;
    QString language_TW;
    QString language_US;
};
enum LanguageType
{
    CN,
    TW,
    US
};
}

class CXmlReader
{
public:
    CXmlReader();
    bool read(const QString &sourceFilePath);
    QSharedPointer<TS_DATA::Context> parseContextElement(const QDomElement &element);
    QSharedPointer<TS_DATA::Message> parseMessageElement(const QDomElement &element);
    bool convert(const QString &sourceFilePath,const QString &languageDataFilePath,const QString outPath);
    bool save(const QString &filePath);
    void setRepalceMap(const QString &filePath);

private:
    QString getCurrentLanguage();
    QString getLanguageString(const QString &key);
    void writeContext(const QSharedPointer<TS_DATA::Context> &obj);
    void writeMessage(const QSharedPointer<TS_DATA::Message> &obj);
    void log(const QString &text);
    bool initLanguageMap(const QString &filePath);

private:
    QDomDocument domDocument;
    QXmlStreamWriter xml;
    QList<QSharedPointer<TS_DATA::Context> >m_contextList;
    QMap<QString,TS_DATA::LanguageData> m_languageDataMap;
    TS_DATA::LanguageType m_languageType;
    QMap<QString,QString> m_notFoundString;
};

#endif // CXMLREADER_H
