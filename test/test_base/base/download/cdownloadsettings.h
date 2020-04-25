#ifndef CDOWNLOADSETTINGS_H
#define CDOWNLOADSETTINGS_H

#include "crangedata.h"
#include <QString>

class CDownloadSettings
{
public:
    CDownloadSettings(const QString &filePath);
    CDownloadSettings(const QString &saveFilePath, quint64 fileSize, const QByteArray &timestamp);

    ~CDownloadSettings();
    bool isValid() const;
    static quint64 getChunkCount(quint64 fileSize, quint64 chunkSize);
    qint64 writeToFile(quint64 index, const QByteArray &data);
    bool isFinished() const;
    int getChunkSize() const;
    quint64 getMax() const;
    quint64 getIndexChunkSize(quint64 index) const;
    quint64 getIndexPos(quint64 index) const;

private:
    void saveSettings();

private:
    bool m_isValid;
    quint64 m_fileSize;
    int m_threadCount;
    int m_chunkSize;
    CRangeData m_rangeData;
    QString m_filePath;
    QString m_saveFilePath;
    QByteArray m_timestamp;
};

#endif // CDOWNLOADSETTINGS_H
