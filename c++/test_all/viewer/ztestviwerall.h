#ifndef ZTESTVIWERALL_H
#define ZTESTVIWERALL_H

#include <QString>

class ZTestViwerAll
{
public:
    static void outIntList(int *a,int len);
    //字符串拷贝
    static char *strcopy(const char *src,char *des);
    //快速排序
    static void quickSort(int src[],int l,int r);
    static qulonglong cn2Num(const QString &cn);
};

#endif // ZTESTVIWERALL_H
