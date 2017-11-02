#ifndef ZANALYZER_H
#define ZANALYZER_H
#include <QString>
#include <QMap>

class ZAnalyzer
{
public:
    struct Info
    {
        Info(const QString &name);
        Info();
        QString name;
        int count;
        int time_consuming_all;
        int time_consuming_max;
    };

    ZAnalyzer();
    ~ZAnalyzer();
    void analye(const QString &fileName);

    QStringList getLineMatchContent(const QString &str);

    void printData();
private:
    QMap<QString,Info> m_dataMap;
};

#endif // ZANALYZER_H
