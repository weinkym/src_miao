#ifndef CTESTOBJECT_H
#define CTESTOBJECT_H
#include "cdownloadsettings.h"
#include <QObject>
#include <QTimer>

class CTestObject : public QObject
{
    Q_OBJECT
public:
    CTestObject(const QString &filePath, QObject *parent = Q_NULLPTR);
    ~CTestObject();
    void start();

private slots:
    void onTimeout();

signals:
    void sigFinished(bool);

private:
    CDownloadSettings *m_downloadSettings;
    QString m_filePath;
    QString m_saveFilePath;
    QTimer m_timer;
    QList<quint64> m_indexList;
};

#endif // CTESTOBJECT_H
