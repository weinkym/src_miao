#include "zdbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

ZDBManager *ZDBManager::m_instance = NULL;

ZDBManager* ZDBManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new ZDBManager();
    }
    return m_instance;
}

ZDBManager::ZDBManager()
    :QObject(nullptr)
{
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
}

ZDBManager::~ZDBManager()
{

}

void ZDBManager::init()
{
    if(m_sqlDatabase.isOpen())
    {
        return;
    }
    m_sqlDatabase.setDatabaseName("MyDataBase.db");
    if(!m_sqlDatabase.open())
    {
        return;
    }
    {
        QSqlQuery sqlQuery = m_sqlDatabase.exec("select * from tables");
        if(sqlQuery.isValid())
        {
            int index = 0;
            while(sqlQuery.next())
            {
                qDebug()<<sqlQuery.value(index++);
            }
        }
        else
        {
            qDebug() << "sqlQuerylastError "<< sqlQuery.lastError();
        }
    }
    //创建表格
    QSqlQuery sql_query;
    if(!sql_query.exec("create table student(id int primary key, name text, age int)"))
    {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }

    //插入数据
    if(!sql_query.exec("INSERT INTO student VALUES(1, \"Wang\", 23)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Wang!";
    }
    if(!sql_query.exec("INSERT INTO student VALUES(2, \"Li\", 23)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Li!";
    }

}

