#ifndef CSQLITEACCESSINTERFACE_H
#define CSQLITEACCESSINTERFACE_H

#include <QSqlDatabase>
#include <QMutex>
#include "zpublic.h"

class CSqliteAccessInterface
{
public:
    static CSqliteAccessInterface *getInstance();
    ~CSqliteAccessInterface();
    static QString messageTableName();
    bool isValid();

    bool insertMessage(const CPB::AutoSendEventData &msg);
    bool queryAllMessage(QVariantList &model, QString *errorString = nullptr);
    bool deleteMessage(const QString &uuid);
    bool deleteAllMessage();

private:
    CSqliteAccessInterface();
    bool getInsertSql(QList<CPB::BindValueParam> &paramList,const QVariantList &model,const QString &tableName,const QMap<QString,CPB::Field> &fileMap);
    void close();
    void initDb();

    bool execQuery(const QString &sql, QList<QSqlRecord> &result, QString *errorString = NULL);
    bool execQuery(const QString &sql, QString *errorString = NULL);
    bool execQuery(const QStringList &sqlList,QString *errorString = NULL);
    bool execStringQuery(const QString &sql, QList<QSqlRecord> *result,QString *errorString = NULL);
    bool execQueryList(const QList<QSqlQuery> &sqlQueryList, QString *errorString);
    bool execQuery(const QList<CPB::BindValueParam> &paramList, QString *errorString = NULL);

private:
    static CSqliteAccessInterface *m_instance;
    QSqlDatabase m_db;
    QMutex m_mutex;

};

#endif // CSQLITEACCESSINTERFACE_H
