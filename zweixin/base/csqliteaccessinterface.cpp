#include "csqliteaccessinterface.h"
#include "zpublic.h"
#include "zgolbal.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

#include <QDir>
#include "zgolbal.h"

const char* const COMMAND_CREATE_TABLE_PREFIX = "CREATE TABLE IF NOT EXISTS ";
const char* const CREATE_DEPARTMENT_TABLE_QT = "message_table (\
        id UNSIGNED BIGINT NOT NULL,\
parent_id BIGINT NOT NULL,\
name VARCHAR (128) DEFAULT \"\" NOT NULL,\
avatar_url VARCHAR (255) DEFAULT \"\" NOT NULL,\
creator_id UNSIGNED BIGINT NOT NULL DEFAULT 0,\
order_field UNSIGNED INTEGER DEFAULT 0 NOT NULL,\
is_full  UNSIGNED TINYINT DEFAULT 0 NOT NULL,\
message_forbidden UNSIGNED TINYINT DEFAULT 0 NOT NULL,\
is_disbanded UNSIGNED TINYINT DEFAULT 0 NOT NULL,\
PRIMARY KEY(id)\
);";

const char* const CREATE_AUTO_SEND_MESSAGE = "AUTO_SEND_MESSAGE (\
        type  UNSIGNED TINYINT DEFAULT 0 NOT NULL,\
dateTime  UNSIGNED TINYINT DEFAULT 0 NOT NULL,\
lastSendDateTime  UNSIGNED TINYINT DEFAULT 0,\
content VARCHAR (4096) NOT NULL, \
uuid VARCHAR (64) NOT NULL, \
PRIMARY KEY (uuid)\
);";


//int type;
//qlonglong dateTime;
//QString content;
//QString toUserName;
//QString uuid;
//QVariantMap body;

CSqliteAccessInterface *CSqliteAccessInterface::m_instance = NULL;

CSqliteAccessInterface::CSqliteAccessInterface()
{
    m_db = QSqlDatabase::database("ZWeiXinLocalStorage", false);
    if(!m_db.isValid())
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "ZWeiXinLocalStorageHost");
        m_db.setHostName("ZWeiXinLocalStorageHost");
        initDb();
    }
}


bool CSqliteAccessInterface::getInsertSql(QList<CPB::BindValueParam> &paramList, const QVariantList &model, const QString &tableName, const QMap<QString, CPB::Field> &fileMap)
{
    QString sqlTemplate = "REPLACE INTO " + tableName + " (%1) values(%2)";
    foreach(QVariant v, model)
    {
        QVariantMap map = v.toMap();
        QStringList sqlField;
        QStringList sqlValue;
        foreach(QString key, map.keys())
        {

            if(!fileMap.contains(key))
            {
                continue;
            }
            CPB::Field f = fileMap.value(key);
            sqlField.push_back(f.keyInDb);
            sqlValue.push_back(":" + f.keyInDb);
        }
        QString statement = sqlTemplate.arg(sqlField.join(",")).arg(sqlValue.join(","));
        CPB::BindValueParam param;
        param.prepareString = statement;
        foreach(QString key, fileMap.keys())
        {
            CPB::Field f = fileMap.value(key);
            param.bindValueMap.insert(":" + f.keyInDb, map.value(f.keyInDb));
        }
        paramList.push_back(param);
    }
    ZW_LOG_INFO(QString("paramList.count=%1").arg(paramList.count()));
    return true;
}

CSqliteAccessInterface *CSqliteAccessInterface::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CSqliteAccessInterface;
    }
    return m_instance;
}

CSqliteAccessInterface::~CSqliteAccessInterface()
{
    close();
}

QString CSqliteAccessInterface::messageTableName()
{
    return QString("AUTO_SEND_MESSAGE");
}

bool CSqliteAccessInterface::isValid()
{
    return m_db.isValid() && m_db.isOpen();
}

bool CSqliteAccessInterface::insertMessage(const CPB::AutoSendEventData &msg)
{
    QVariantList model;
    model.append(msg.toVariantMap());
    QList<CPB::BindValueParam> paramList;
    bool ret = getInsertSql(paramList,model,CSqliteAccessInterface::messageTableName(),CPB::AutoSendEventData::getFieldMap());
    if(!ret)
    {
        ZW_LOG_CRITICAL(QString("getInsertSql fail"));
        return false;
    }
    QString errorString;
    /* m_db.*/execQuery(paramList, &errorString);
}

bool CSqliteAccessInterface::queryAllMessage(QVariantList &model, QString *errorString)
{
    ZW_LOG_FUNCTION;
    if(!isValid())
    {
        return false;
    }
    QString sql = "SELECT * FROM " + CSqliteAccessInterface::messageTableName();
    ZW_LOG_INFO(QString("execQuerySql, sql = %1").arg(sql));
    QList<QSqlRecord> record;
    QString tempErrorString;
    bool ret = this->execQuery(sql, record, &tempErrorString);
    if(errorString)
    {
        *errorString = tempErrorString;
    }
    if(!ret)
    {
        ZW_LOG_CRITICAL(QString("execQuerySql fail, sql = %1, err = %2").arg(sql).arg(tempErrorString));
        return false;
    }

    bool isFirst = true;
    foreach(auto r, record)
    {
        QVariantMap map;
        for(int i = 0; i < r.count(); ++i)
        {
            map.insert(r.field(i).name(), r.value(i));
            if(isFirst)
            {
                ZW_LOG_INFO(QString("r.field(i).name() = %1").arg(r.field(i).name()));
            }
        }
        isFirst = true;
        model.push_back(map);
    }
    return true;
}

bool CSqliteAccessInterface::deleteMessage(const QString &uuid)
{
    ZW_LOG_FUNCTION;
    if(!isValid())
    {
        return false;
    }
    QString sql = QString("DELETE FROM %1 where uuid=\"%2\"").arg(CSqliteAccessInterface::messageTableName()).arg(uuid);

    QString tempErrorString;
    bool ret = this->execQuery(sql, &tempErrorString);
    if(!ret)
    {
        ZW_LOG_CRITICAL(QString("execQuerySql fail, sql = %1, err = %2").arg(sql).arg(tempErrorString));
        return false;
    }
    return true;
}

bool CSqliteAccessInterface::deleteAllMessage()
{
    QString sql = QString("DELETE FROM %1").arg(CSqliteAccessInterface::messageTableName());

    QString tempErrorString;
    bool ret = this->execQuery(sql, &tempErrorString);
    if(!ret)
    {
        ZW_LOG_CRITICAL(QString("execQuerySql fail, sql = %1, err = %2").arg(sql).arg(tempErrorString));
        return false;
    }
    return true;
}

void CSqliteAccessInterface::close()
{
    if(m_db.isValid())
    {
        if(m_db.isOpen())
            m_db.close();
    }
}

void CSqliteAccessInterface::initDb()
{
    QString fileName = "backup.db";

    QString dbFile = Zpublic::getDataPath("datadb",true)+"/"+(fileName);
    m_db.setDatabaseName(dbFile);
    bool ok = m_db.open();
    qDebug()<<"CSqliteAccessInterface::open:"<<ok<<"dbFile"<<dbFile;
    if(!ok)
    {
        return;
    }

    QStringList list;
    list.push_back(COMMAND_CREATE_TABLE_PREFIX + QString(CREATE_DEPARTMENT_TABLE_QT));
    list.push_back(COMMAND_CREATE_TABLE_PREFIX + QString(CREATE_AUTO_SEND_MESSAGE));
    bool result = this->execQuery(list);
}

bool CSqliteAccessInterface::execQuery(const QString &sql, QList<QSqlRecord> &result, QString *errorString)
{
    return execStringQuery(sql,&result,errorString);
}

bool CSqliteAccessInterface::execQuery(const QString &sql, QString *errorString)
{
    return execStringQuery(sql,0,errorString);
}

bool CSqliteAccessInterface::execQuery(const QStringList &sqlList, QString *errorString)
{
    QList<QSqlQuery> sqlQueryList;
    foreach(QString sql, sqlList)
    {
        sqlQueryList.append(QSqlQuery(sql,m_db));
    }
    return execQueryList(sqlQueryList,errorString);
}

bool CSqliteAccessInterface::execStringQuery(const QString &sql, QList<QSqlRecord> *result, QString *errorString)
{
    QMutexLocker lock(&m_mutex);
    ZW_LOG_WARNING(QString("execSql=%1").arg(sql));
    if(!isValid())
    {
        return false;
    }
    QSqlQuery query(m_db);
    bool retValue = false;
    QString lastQuery;
    try
    {
        query.setForwardOnly(true);
        retValue = query.exec(sql);
        lastQuery = sql;
        if(!retValue)
        {
//           doError(needResetDb);
           QString lastError = m_db.lastError().text();
            if(errorString)
            {
                *errorString = lastError;
            }
            ZW_LOG_WARNING(QString("execSql=%1,errorString=%2").arg(lastQuery).arg(lastError));
            return false;
        }
        if(result)
        {
            while(query.next())
            {
                result->append(query.record());
            }
            ZW_LOG_INFO(QString("result->count()=%1").arg(result->count()));
        }
        return true;
    }
    catch(...)
    {
        QString lastError = m_db.lastError().text();
        if(errorString)
        {
            *errorString = lastError;
        }
        ZW_LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
//        doError(needResetDb);
        return false;
    }
}

bool CSqliteAccessInterface::execQueryList(const QList<QSqlQuery> &sqlQueryList, QString *errorString)
{
    QMutexLocker lock(&m_mutex);
    if(!isValid())
    {
        return false;
    }

    m_db.transaction();
    bool retValue = false;
    QString lastQuery;
    try
    {
        QString lastError;
        foreach(QSqlQuery query, sqlQueryList)
        {
            lastQuery = query.lastQuery();
            ZW_LOG_INFO(QString("executedQuery = %1").arg(lastQuery));
            retValue = query.exec();
            if(!retValue)
            {
//                doError(needResetDb);
                lastError = query.lastError().text();
                break;
            }
        }

        if(retValue)
        {
            if(!m_db.commit())
            {
                m_db.rollback();
                retValue = false;
//                doError(needResetDb);
                lastError = m_db.lastError().text();
            }
        }
        if(!retValue)
        {
            if(errorString)
            {
                *errorString = lastError;
            }
            ZW_LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
        }
        return retValue;
    }
    catch(...)
    {
        QString lastError = m_db.lastError().text();
        if(!m_db.rollback())
        {
            ZW_LOG_WARNING(QString("rollback errorString=%2").arg(m_db.lastError().text()));
        }
        if(errorString)
        {
            *errorString = lastError;
        }
        ZW_LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
//        doError(needResetDb);
        return false;
    }
}

bool CSqliteAccessInterface::execQuery(const QList<CPB::BindValueParam> &paramList, QString *errorString)
{
    QList<QSqlQuery> sqlQueryList;
    foreach(const CPB::BindValueParam &param, paramList)
    {
        QSqlQuery query(m_db);
        ZW_LOG_INFO(QString("prepare sql = %1").arg(param.prepareString));
        if(!query.prepare(param.prepareString))
        {
            ZW_LOG_WARNING(QString("prepare sql fail, sql = %1").arg(param.prepareString));
            ZW_LOG_WARNING(QString("m_db.lastError() = %1").arg(query.lastError().text()));
            ZW_LOG_WARNING(QString("m_db.lastError() = %1").arg(query.lastError().driverText()));
            ZW_LOG_WARNING(QString("m_db.lastError() = %1").arg(query.lastError().databaseText()));
            ZW_LOG_WARNING(QString("m_db.lastError() = %1").arg(query.lastError().nativeErrorCode()));
            ZW_LOG_WARNING(QString("m_db.lastError() = %1").arg(query.lastError().type()));
//            doError(needResetDb);
//            QString driverText() const;
//            QString databaseText() const;
//            ErrorType type() const;
//        #if QT_DEPRECATED_SINCE(5, 3)
//            QT_DEPRECATED int number() const;
//        #endif
//            QString nativeErrorCode() const;
//            QString text() const;
            continue;
        }
        query.setForwardOnly(true);
        QMapIterator<QString,QVariant>iter(param.bindValueMap);
        int count = 0;
        while(iter.hasNext())
        {
            count++;
            iter.next();
            query.bindValue(iter.key(),iter.value());
        }
        ZW_LOG_DEBUG(QString("bindValue Count=%1").arg(count));
        sqlQueryList.append(query);
    }

    return execQueryList(sqlQueryList,errorString);
}

