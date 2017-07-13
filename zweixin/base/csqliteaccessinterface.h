#ifndef CSQLITEACCESSINTERFACE_H
#define CSQLITEACCESSINTERFACE_H

#include <QSqlDatabase>

class CSqliteAccessInterface
{
public:
    static CSqliteAccessInterface *getInstance();
    ~CSqliteAccessInterface();
    bool isValid();

private:
    CSqliteAccessInterface();
    void close();
    void initDb();

    bool execQuery(const QString &sql, QList<QSqlRecord> &result, QString *errorString = NULL);
    bool execQuery(const QString &sql, QString *errorString = NULL);
    bool execQuery(const QStringList &sqlList,QString *errorString = NULL);
    bool execStringQuery(const QString &sql, QList<QSqlRecord> *result,QString *errorString = NULL);
    bool execQueryList(const QList<QSqlQuery> &sqlQueryList, QString *errorString);

private:
    static CSqliteAccessInterface *m_instance;
    QSqlDatabase m_db;
    QMutex m_mutex;

};

#endif // CSQLITEACCESSINTERFACE_H
