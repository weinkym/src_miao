#ifndef ZWVIDEOTHREAD_H
#define ZWVIDEOTHREAD_H

#include <QThread>

class ZWVideoThread : public QThread
{
    Q_OBJECT

public:
    ZWVideoThread(QObject *parent = Q_NULLPTR);
    ~ZWVideoThread();
    void run();
    void pause();
    void play();

signals:
    void sigErrorHappened(int type,const QString &errorString);
    void sigImageChanged(const QImage &image);

protected:
    bool m_pause;
    QString m_filePath;
};

#endif // ZWVIDEOTHREAD_H
