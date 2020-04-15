#ifndef ZDBMANAGER_H
#define ZDBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class ZDBManager : public QObject
{
    Q_OBJECT

public:
    static ZDBManager* getInstance();
    ~ZDBManager();
    void init();

private:
    ZDBManager();

private:
    static ZDBManager *m_instance;
    QSqlDatabase m_sqlDatabase;
};

#endif // ZDBMANAGER_H
