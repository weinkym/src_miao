#include <QFile>
#include <QDir>
#include <QDebug>
#include "zfiletool.h"

ZFileTool::ZFileTool(QObject *parent)
    :QObject(parent)
{
}

ZFileTool::~ZFileTool()
{
}

void ZFileTool::renameFile(const QString &path, const QStringList &nameFilters, const QString &prefix)
{
    DEBUG_CURRENT_LINE;
    QList<QFileInfo> fileInfoList;
    findFile(fileInfoList,path,nameFilters);
    foreach(QFileInfo fileInfo,fileInfoList)
    {
        QString preDirName = fileInfo.absoluteDir().dirName();
        QString path = fileInfo.path();
        QString fileName = fileInfo.fileName();
        QString newName = QString("%1%2%3%4_%5")
                .arg(path).arg(QDir::separator())
                .arg(prefix).arg(preDirName).arg(fileName);
        newName = QDir::toNativeSeparators(newName);
        QString oldName = fileInfo.absoluteFilePath();
        oldName = QDir::toNativeSeparators(oldName);
        QFile::rename(oldName,newName);
        qDebug()<<QString("oldName = %1").arg(oldName);
        qDebug()<<QString("newName = %1").arg(newName);
    }
}

void ZFileTool::renameFile(const QString &fileName, const QString &delStr)
{
    //
}

void ZFileTool::findFile(QStringList &fileList, const QString &path, const QStringList &nameFilters)
{
    DEBUG_CURRENT_LINE;
    QList<QFileInfo> fileInfoList;
    findFile(fileInfoList,path,nameFilters);
    foreach(QFileInfo fileInfo,fileInfoList)
    {
        fileList.append(fileInfo.absoluteFilePath());
    }
}

void ZFileTool::findFile(QList<QFileInfo> &fileInfoList, const QString &path, const QStringList &nameFilters)
{
    DEBUG_CURRENT_LINE;
    QDir rootDir(path);
    if(!rootDir.exists())
    {
        return;
    }
    QFileInfoList list = rootDir.entryInfoList(nameFilters);
    for(int i = 0; i <list.count(); i++)
    {
        QFileInfo tmpFileInfo = list.at( i );
        QString fileName = tmpFileInfo.fileName();
        if ( tmpFileInfo.isDir() )
        {
            if(fileName == "." || fileName == "..")
            {
                continue;
            }
             findFile(fileInfoList,tmpFileInfo.filePath(),nameFilters);
        }
        else
        {
            fileInfoList.append(tmpFileInfo);
        }
    }
}

void ZFileTool::copyFile(const QStringList &fileList, const QString &path)
{
    DEBUG_CURRENT_LINE;
    QList<QFileInfo> fileInfoList;
    foreach(QString fileName,fileList)
    {
        QFileInfo fileInfo(fileName);
        fileInfoList.append(fileInfo);
    }
    copyFile(fileInfoList,path);
}

void ZFileTool::copyFile(const QList<QFileInfo> &fileInfoList, const QString &path)
{
    DEBUG_CURRENT_LINE;
    foreach(QFileInfo fileInfo,fileInfoList)
    {
        bool ok = false;
        QString fileName = fileInfo.absoluteFilePath();
        if(!QFile::exists(fileName))
        {
//            qDebug()<<tr("error:源文件<%1>不存在").arg(fileName);
            continue;
        }
        QString newFileName = QString("%1%2%3").arg(path).arg(QDir::separator()).arg(fileInfo.fileName());
//        newFileName = QDir::toNativeSeparators(newFileName);
//        if(QFile::exists(newFileName))
//        {
//            ok = QFile::remove(newFileName);
//            if(!ok)
//            {
//                qDebug()<<tr("error:清除文件<%1>失败").arg(newFileName);
//                continue;
//            }
//        }
//        ok = QFile::copy(fileName,newFileName);
        ok = copyFile(fileName,newFileName);
        //复制 //成功 //失败
        QString text = QString("%1 (%2 -- %3) %4")
                .arg(tr("copy"))
                .arg(fileName)
                .arg(newFileName)
                .arg(ok?tr("success"):tr("fail"));
        qDebug()<<text;
    }
}

bool ZFileTool::copyFile(const QString &fromFile, const QString &toFile)
{
    DEBUG_CURRENT_LINE;
    QString fromFileName = QDir::toNativeSeparators(fromFile);
    QString toFileName = QDir::toNativeSeparators(toFile);
    if(QFile::exists(toFileName))
    {
        bool ok = QFile::remove(toFileName);
        if(!ok)
        {
            qDebug()<<tr("error:清除文件<%1>失败").arg(toFileName);
            return false;
        }
    }
    return QFile::copy(fromFileName,toFileName);
}
