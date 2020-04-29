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
        STATUS_FINISHED,
    };

    struct DownloadInfo
    {
        DownloadInfo();
        DownloadInfo(const QString &settingFilePath);
        void save();
        QString filePath;
        quint64 complete;
        quint64 fileSize;
        QDateTime lastTime;
        QByteArray timestamp;
    };

    CDownloadObject(const QString &saveFilePath, const QString &url, QNetworkAccessManager *networkAccessManager = 0, QObject *parent = 0);
    ~CDownloadObject();
    void start();
    void downloadFile(const QString &filePath, const QString &url, bool fullDownload = false);
    void cancel();

protected:
    void setStatus(Status status);
    void sendHttpPartialGetRequest();
    void sendHttpAllGetRequest();

    QString getLocalTmpFile();
    QString getSettingsFile();
    void touchFile(const QString &filename);
    void saveToFile(const QByteArray &dataArray, const QString &fileName, quint64 pos);
    void doRequestFinished(bool ok, const CBaseRequestAction::Data &data);

private:
    void doError(const QString &error);
    void requestPartial();
    void requestHead();

private slots:
    void onHttpFinished();
    void onFullDownloadFinished();
    void onRequestTimeout();
    void onDownloadNetworkError(QNetworkReply::NetworkError code);
    void onDownloadFile(QString filePath, QString url, bool fullDownload);

    void onHeadRequestFinished();
    void onRequestFinished();

signals:
    void sigProcessUpdated(quint64 complete, quint64 fileSize);
    void sigStateUpdated(int status);

public:
    static const int CHUNK_SIZE = 1024 * 512;
    static const int m_cacheSize = 4 * 1024 * 1024;
    static const int m_maxRetryTime = 1;
    static const QString m_tmpSuffix;
    QNetworkAccessManager *m_networkAccessMgr;
    QNetworkReply *m_reply;
    QTimer *m_timeoutTimer;
    CDownloadSettings *m_downloadSettings;
    Status m_status;
    QString m_url;
    QString m_filePath;
    QString m_lastError;
    DownloadInfo m_downloadInfo;
    QByteArray m_dataCache;
    QUuid m_headRequestUuid;
    QList<QUuid> m_requestUuidList;
    QHash<QUuid, int> m_partUuidMap;
};

#endif // CDOWNLOADOBJECT_H
