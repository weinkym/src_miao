#ifndef CDOWNLOADREQUESTACTION_H
#define CDOWNLOADREQUESTACTION_H

#include "cbaserequestaction.h"

class CDownloadRequestAction : public CBaseRequestAction
{
public:
    enum Type
    {
        TYPE_HEAD,
        TYPE_PART
    };

    ~CDownloadRequestAction();
    static CDownloadRequestAction *createHeadAction(const QString &url);
    static CDownloadRequestAction *createPartAction(const QString &url, quint64 start, quint64 end);

protected:
    QString getActionName() const;
    RequestType getRequestType() const;
    QNetworkRequest getRequest() const;

private:
    explicit CDownloadRequestAction(Type type, QObject *parent = 0);

private:
    Type m_actionType;
    quint64 m_start;
    quint64 m_end;
    QString m_url;
};

#endif // CDOWNLOADREQUESTACTION_H
