#include "zw3utils.h"
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include "zlib.h"
#include "zip.h"
#include "zwlog.h"


#define ZW3UTILS_ZIP_WRITE_BUFFER_SIZE (16384)

ZW3Utils::ZW3Utils()
{

}

ZW3Utils::~ZW3Utils()
{

}

bool ZW3Utils::compressedFile(const QStringList &filePathList, const QString &outPath)
{
    if(filePathList.isEmpty() && outPath.isEmpty())
    {
        return false;
    }
    int err=0;
//    zipFile zf;
    int errclose;
    int opt_compress_level = Z_DEFAULT_COMPRESSION;
    zipFile zf = zipOpen64(outPath.toUtf8().data(),APPEND_STATUS_CREATE);
    if (zf == NULL)
    {
        ZW_LOG_CRITICAL(QString("zipOpen64 is error,outPath=%1").arg(outPath));
        err= ZIP_ERRNO;
        return false;
    }
    int size_buf = ZW3UTILS_ZIP_WRITE_BUFFER_SIZE;
    void* buf = (void*)malloc(size_buf);
    for (auto obj:filePathList)
    {
        QFile file(obj);
        if(!file.exists())
        {
            ZW_LOG_WARNING(QString("file.exists=false,path=%1").arg(obj));
            continue;
        }
        QFileInfo fileInfo(obj);
        QDateTime lastTime = fileInfo.lastModified();
        zip_fileinfo zi;
        zi.tmz_date.tm_sec = lastTime.time().second();
        zi.tmz_date.tm_min = lastTime.time().minute();
        zi.tmz_date.tm_hour = lastTime.time().hour();

        zi.tmz_date.tm_mday = lastTime.date().day();
        zi.tmz_date.tm_mon = lastTime.date().month();
        zi.tmz_date.tm_year = lastTime.date().year();

        zi.dosDate = 0;
        zi.internal_fa = 0;
        zi.external_fa = 0;
        QString fileName = fileInfo.fileName();

        int size_read;
        unsigned long crcFile=0;
        int zip64 = 0;

        err = zipOpenNewFileInZip3_64(zf,fileName.toUtf8().data(),&zi,
                                      NULL,0,NULL,0,NULL,
                                      (opt_compress_level != 0) ? Z_DEFLATED : 0,
                                      opt_compress_level,0,
                                      -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                      NULL,crcFile, zip64);

        if(err != ZIP_OK)
        {
            continue;
        }
        if(!file.open(QIODevice::ReadOnly))
        {
            continue;
        }

        do{
            size_read = file.read((char*)buf,size_buf);
            if (size_read>0)
            {
                err = zipWriteInFileInZip (zf,buf,size_read);
                if (err<0)
                {
                    ZW_LOG_WARNING(QString("error in writing %1 in the zipfile").arg(fileName));
                }
            }
        }while((err == ZIP_OK) && (size_read>0));
        file.close();

        if (err<0)
        {
            err=ZIP_ERRNO;
            continue;
        }
        else
        {
            err = zipCloseFileInZip(zf);
            if (err!=ZIP_OK)
            {
                ZW_LOG_WARNING(QString("error in closing %1 in the zipfile").arg(fileName));
                continue;
            }
        }
    }
    errclose = zipClose(zf,NULL);
    if (errclose != ZIP_OK)
    {
        ZW_LOG_CRITICAL(QString("error in closing %1").arg(outPath));
        return false;
    }
    return true;
}
