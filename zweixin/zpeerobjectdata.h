#ifndef ZPEEROBJECTDATA_H
#define ZPEEROBJECTDATA_H

#include <QObject>
#include "zpublicaction.h"
#include <QImage>

class ZPeerObjectData : public QObject
{
    Q_OBJECT

public:
    ZPeerObjectData();
    void requestAvatar();

protected slots:
    void onFinished(const ZRequestAction::ZRequestResponse &response);

signals:
    void sigUpdate();

public:
    bool m_isRepeat;
    QString m_userName;
    QString m_url;
    QImage m_image;
};

#endif // ZPEEROBJECTDATA_H
