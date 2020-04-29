#ifndef CDOWNLOADOBJECT_H
#define CDOWNLOADOBJECT_H
#include "cbaserequester.h"
#include <QNetworkReply>

class QTimer;
class QNetworkReply;
class QNetworkAccessManager;
class CDownloadSettings;

class CDownloadObject : public CBaseRequester
{
    Q_OBJECT
public:
    enum HttpStatesCode
    {
        OK = 200, /*!< 成功 */
        PartialContent = 206 /*!< 部分完成 */
    };
    enum Status
    {
        STATUS_NORMAL,
        STATUS_STARTINGT,
        STATUS_REQUEST_HEAD,
        STATUS_REQUEST_PARTIAL,
        STATUS_REQUEST_ALL,
        STATUS_CANCEL,
        STATUS_FAIL,
        STATUS_FINISHED,
    };

    CDownloadObject(const QString &saveFilePath, const QString &url, QObject *parent = 0);
    ~CDownloadObject();
    void start();
    void cancel();

protected:
    void setStatus(Status status);

    void touchFile(const QString &filename);
    void doRequestFinished(bool ok, const CBaseRequestAction::Data &data);

private:
    void doError(const QString &error);
    void requestPartial();
    void requestHead();

signals:
    void sigProcessUpdated(quint64 complete, quint64 fileSize);
    void sigStateUpdated(int status);

public:
    CDownloadSettings *m_downloadSettings;
    bool m_cancel;
    Status m_status;
    QString m_url;
    QString m_filePath;
    QString m_lastError;
    QUuid m_headRequestUuid;
    QHash<QUuid, int> m_partUuidMap;
};

#endif // CDOWNLOADOBJECT_H
