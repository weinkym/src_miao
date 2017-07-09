#ifndef CMESSAGEINTERFACE_H
#define CMESSAGEINTERFACE_H
#include <QObject>
#include <QTimer>

class CMessageInterface : QObject
{
    Q_OBJECT

public:
    static CMessageInterface *getInstance();

protected:
    CMessageInterface(QObject *parent = NULL);

protected slots:
    void onTimerout();

private:
    static CMessageInterface *m_instance;
    QTimer *m_timer;
};

#endif // CMESSAGEINTERFACE_H
