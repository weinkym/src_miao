#ifndef ZFILETOOL_H
#define ZFILETOOL_H
#include <QObject>
#include <QPair>
#include <QFileInfo>
#include "mplib_global.h"

class ZFileTool : public QObject
{
    Q_OBJECT

public:
    ZFileTool(QObject* parent = 0);
    ~ZFileTool();
    static void renameFile(const QString& path,const QStringList & nameFilters,const QString& prefix = "");
//    void renameFile(const QList<QFileInfo>& fileInfoList,const QString& delStr);
    static void renameFile(const QString& fileName,const QString& delStr);
//    bool renameFile(const QString& path,int start,const QString& prefix = "");
    static void findFile(QStringList& fileList,const QString& path,const QStringList & nameFilters);
    static void findFile(QList<QFileInfo>& fileInfoList,const QString& path,const QStringList & nameFilters);
    static void copyFile(const QStringList& fileList,const QString& path);
    static void copyFile(const QList<QFileInfo>& fileInfoList,const QString& path);

    static bool copyFile(const QString& fromFile,const QString& toFile);

signals:

private:
    //
};

#endif // ZFILETOOL_H
