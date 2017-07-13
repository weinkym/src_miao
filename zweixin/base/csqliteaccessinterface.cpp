#include "csqliteaccessinterface.h"
#include "zpublic.h"
#include "zgolbal.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QDir>

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
dateTime UNSIGNED BIGINT NOT NULL,\
content VARCHAR (512) NOT NULL, \
body TEXT DEFAULT \"\"  NOT NULL,\
uuid VARCHAR (64) NOT NULL, \
PRIMARY KEY (uuid)\
);";


int type;
qlonglong dateTime;
QString content;
QString toUserName;
QString uuid;
QVariantMap body;

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

bool CSqliteAccessInterface::isValid()
{
    return m_db.isValid() && m_db.isOpen();
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
            LOG_WARNING(QString("execSql=%1,errorString=%2").arg(lastQuery).arg(lastError));
            return false;
        }
        if(result)
        {
            while(query.next())
            {
                result->append(query.record());
            }
            LOG_INFO(QString("result->count()=%1").arg(result->count()));
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
        LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
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
            LOG_INFO(QString("executedQuery = %1").arg(lastQuery));
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
            LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
        }
        return retValue;
    }
    catch(...)
    {
        QString lastError = m_db.lastError().text();
        if(!m_db.rollback())
        {
            LOG_WARNING(QString("rollback errorString=%2").arg(m_db.lastError().text()));
        }
        if(errorString)
        {
            *errorString = lastError;
        }
        LOG_WARNING(QString("execSql=%1  errorString=%2").arg(lastQuery).arg(lastError));
//        doError(needResetDb);
        return false;
    }
}

