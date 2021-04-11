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
            a[i]=x;
        }

        quickSort(a,l,i-1);
        quickSort(a,i+1,r);
    }
}
