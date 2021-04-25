#include "ztestviwerall.h"

#include "zwlog.h"

void ZTestViwerAll::outIntList(int *a, int len)
{
    QStringList objList;
    for(int i = 0; i < len;++i)
    {
        objList.append(QString::number(a[i]));
    }
    auto value = QString("[%1]").arg(objList.join(","));
    ZW_LOG_WARNING(ZW_LOG_P(value));
}

char *ZTestViwerAll::strcopy(const char *src, char *des)
{
    if(src == nullptr || des == nullptr)
    {
        return nullptr;
    }
    if(des == src)
    {
        return des;
    }
    int len = 0;
    char *temp = des;
    while(*(src+len++) != '\0');
    if(des > src && src + len > des)
    {
        while(len >= 0)
        {
            *(des+len) = *(src+len);
            len--;
        }
    }
    else
    {
        while((*des++ = *src++) != '\0');
    }
    return temp;
}

void ZTestViwerAll::quickSort(int a[], int l, int r)
{
    if(l < r)
    {
        int i = l;
        int j = r;
        int x = a[i];
        while(i < j)
        {
            while(i < j && a[j] >= x)
            {
                j--;
            }
            if(i < j)
            {
                a[i++] = a[j];
            }
            while(i < j && a[i] < x)
            {
                i++;
            }
            if(i < j)
            {
                a[j--] = a[i];
            }
        }
        a[i]=x;
        quickSort(a,l,i-1);
        quickSort(a,i+1,r);
    }
}

qulonglong ZTestViwerAll::cn2Num(const QString &cn)
{
    qulonglong  total = 0;
    qulonglong unit = 1;
    qulonglong unit2 = 1;
    QMap<QString,qulonglong> numMap;
    numMap.insert("零",0);
    numMap.insert("壹",1);
    numMap.insert("贰",2);
    numMap.insert("叁",3);
    numMap.insert("肆",4);
    numMap.insert("伍",5);
    numMap.insert("陆",6);
    numMap.insert("柒",7);
    numMap.insert("捌",8);
    numMap.insert("玖",9);
    numMap.insert("拾",10);

    numMap.insert("佰",100);
    numMap.insert("仟",1000);
    numMap.insert("万",10000);
    numMap.insert("亿",100000000);
    numMap.insert("兆",10000000000000000);
    for(int i = 0; i < cn.length();++i)
    {
        auto key = cn.at(cn.length() - i - 1);
        qulonglong value = numMap.value(key);
        ZW_LOG_WARNING(ZW_LOG_P3(key,value,unit));

        if(value == 10 && i == cn.length() - 1)
        {
            total += value;
        }
        else if(value >= 10)
        {
            if(value > unit)
            {
                unit = value;
                unit2 = 1;
            }
            else
            {
                unit2 = value;
                ZW_LOG_WARNING(ZW_LOG_P3(key,value,unit2));
            }
        }
        else
        {
            auto obj = value * unit * unit2;
            ZW_LOG_WARNING(ZW_LOG_P6(obj,key,value,unit,unit2,total));
            total += obj;
        }
    }
    return total;
}
