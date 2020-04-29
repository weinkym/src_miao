#include "cbaseutils.h"
#include "clogsetting.h"
#include <QFile>

CBaseUtils::CBaseUtils()
{
}

CBaseUtils::~CBaseUtils()
{
}

bool CBaseUtils::createTempile(const QString &filePath, quint64 size)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QByteArray data(size, '\0');
        qint64 writeSize = file.write(data);
        file.close();
        C_LOG_OUT_V3(filePath, size, writeSize);
        return writeSize == size;
    }
    return false;
}
