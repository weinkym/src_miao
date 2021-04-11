#ifndef ZTESTALL_H
#define ZTESTALL_H


class ZTestAll
{
public:
    ZTestAll();
    static void outIntList(int *a,int len);
    static int* createIntList(int n,int max = 1000);
    static void testCopy();
    static void testSort();
};

#endif // ZTESTALL_H
