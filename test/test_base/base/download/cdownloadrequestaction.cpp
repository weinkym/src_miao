#include "cdownloadrequestaction.h"
#include <QNetworkRequest>

CDownloadRequestAction::CDownloadRequestAction(Type type, QObject *parent)
    : CBaseRequestAction(parent)
    , m_actionType(type)
{
}

CDownloadRequestAction::~CDownloadRequestAction()
{
}

CDownloadRequestAction *CDownloadRequestAction::createHeadAction(const QString &url)
{
    CDownloadRequestAction *action = new CDownloadRequestAction(TYPE_HEAD);
    action->m_url = url;
    return action;
}

CDownloadRequestAction *CDownloadRequestAction::createPartAction(const QString &url, quint64 start, quint64 end)
{
    CDownloadRequestAction *action = new CDownloadRequestAction(TYPE_PART);
    action->m_url = url;
    action->m_start = start;
    action->m_end = end;
    return action;
}

QString CDownloadRequestAction::getActionName() const
{
    switch(m_actionType)
    {
    case TYPE_HEAD:
        return "TYPE_HEAD";
        break;
    case TYPE_PART:
        return "TYPE_PART";
        break;
    default:
        break;
    }
    return "TYPE_HEAD";
}

CBaseRequestAction::RequestType CDownloadRequestAction::getRequestType() const
{
    switch(m_actionType)
    {
    case TYPE_HEAD:
        return REQUEST_TYPE_HEAD;
        break;
    case TYPE_PART:
        return REQUEST_TYPE_GET;
        break;
    default:
        break;
    }
    return REQUEST_TYPE_UNDEFINED;
}

QNetworkRequest CDownloadRequestAction::getRequest() const
{
    switch(m_actionType)
    {
    case TYPE_HEAD:
    {
        QUrl url(m_url);
        QNetworkRequest request(url);
        request.setRawHeader("Accept-Encoding", "none");
        return request;
        break;
    }
    case TYPE_PART:
    {
        QUrl url(m_url);
        QNetworkRequest request(url);
        request.setRawHeader("Accept-Encoding", "none");
        //        req.setRawHeader("Range", QString("bytes=%1-%2").arg(finishedDownloadSize).arg(QString("")));
        request.setRawHeader("Range", QString("bytes=%1-%2").arg(m_start).arg(m_end).toLatin1());
        return request;
        break;
    }
    default:
        break;
    }

    return QNetworkRequest();
}
