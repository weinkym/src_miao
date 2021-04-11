#include "ztestall.h"
#include "ztestviwerall.h"
#include <QString>
#include <QDebug>
#include "zwlog.h"
#include <memory>

ZTestAll::ZTestAll()
{

}

void ZTestAll::outIntList(int *a, int len)
{
    QStringList objList;
    for(int i = 0; i < len;++i)
    {
        objList.append(QString::number(a[i]));
    }
    auto value = QString("[%1]").arg(objList.join(","));
    ZW_LOG_WARNING(ZW_LOG_P(value));
}

int *ZTestAll::createIntList(int n,int max)
{
    int *a = (int*)malloc(sizeof(int) * n);
    for(int i =0; i < n;++i)
    {
        a[i] = qrand() % max;
    }
    return a;
}

void ZTestAll::testCopy()
{
    QString str;
    int n = 10;
    for(int i = 0; i < n;++i)
    {
        str.append(QString::number(i));
    }

    char *src = (char*)malloc(2 * n + 1);
    memset(src,'\0',2 * n + 1);
    memcpy(src+5,str.toLocal8Bit().data(),n);
    QString temp = (src+5);
    ZW_LOG(ZW_LOG_P(temp),LOG_LEVEL_WARNING);
//    for(int i = 0; i < n;++i)
    {
        auto obj = QString(ZTestViwerAll::strcopy(src+5,src+9));
        ZW_LOG(ZW_LOG_P(obj),LOG_LEVEL_WARNING);
    }

}

void ZTestAll::testSort()
{
//    int a[10];
    int n = QTime::currentTime().msec() % 5 + 5;
    for(int i = 0; i < n;++i)
    {
        int n = qrand() % 10 + 9;
        int *a = createIntList(n,50);
        outIntList(a,n);
        ZTestViwerAll::quickSort(a,0,n-1);
        outIntList(a,n);
        for(int j = 1; j < n;++j)
        {
            if(a[j - 1] > a[j])
            {
                ZW_LOG_FATAL("=======sort is error");
                exit(111);
                break;
            }
        }
    }
//    int a[] ={41,31,21,7,47,49,13,39,19,44};
//    outIntList(a,10);
//    ZTestViwerAll::quickSort(a,0,9);
//    outIntList(a,10);
//    for(int j = 1; j < 10;++j)
//    {
//        if(a[j - 1] > a[j])
//        {
//            ZW_LOG_FATAL("=======sort is error");
//            exit(111);
//            break;
//        }
//    }
}
