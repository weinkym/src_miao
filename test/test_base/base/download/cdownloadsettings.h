#ifndef CDOWNLOADSETTINGS_H
#define CDOWNLOADSETTINGS_H

#include "crangedata.h"
#include <QString>

class CDownloadSettings
{
public:
    CDownloadSettings(const QString &filePath);
    //    CDownloadSettings(const QString &saveFilePath, quint64 fileSize, const QByteArray &timestamp);

    void update(const QString &filePath, quint64 fileSize, const QByteArray &timestamp);

    ~CDownloadSettings();
    bool isValid() const;
    static quint64 getChunkCount(quint64 fileSize, quint64 chunkSize);
    qint64 writeToFile(quint64 index, const QByteArray &data);
    bool isFinished() const;
    int getChunkSize() const;
    quint64 getMax() const;
    quint64 getIndexChunkSize(quint64 index) const;
    quint64 getIndexPos(quint64 index, bool &ok) const;
    quint64 getFreeIndex(bool &ok);

    static QString getSettingsFile(const QString &filePath);

private:
    void saveSettings();
    void updateRangeData();

private:
    bool m_isValid;
    quint64 m_fileSize;
    int m_chunkSize;
    int m_threadCount;
    CRangeData m_rangeData;
    QString m_filePath;
    QByteArray m_timestamp;
};

#endif // CDOWNLOADSETTINGS_H
