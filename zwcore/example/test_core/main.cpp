#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <vector>
#include "zwlog.h"
using namespace std;

int closestTargetValue(int target, vector<int> &array)
{
    // Write your code here
    vector<int> ddata(array.size(),0);
    for(int i = 0; i < array.size();++i)
    {
        ddata[i] = target - array[i];
        ZW_LOG_INFO(ZW_LOG_P2(i,ddata[i]));
    }
    int min = 0;

    for(int i = 0; i < array.size();++i)
    {
        for(int j = i+1; j < ddata.size();++j)
        {
            if(ddata[j] < array[i] || ddata[j] < 0)
            {
                continue;
            }

            if(ddata[j] == array[i])
            {
                ZW_LOG_INFO(ZW_LOG_P2(j,ddata[j]));
                ZW_LOG_INFO_V(array[i]);
                return array[i] + array[j];
            }

            if(ddata[j] - array[i] < min || i == 0)
            {
                min = ddata[j] - array[i];
            }
        }
    }
    if(min == target)
    {
        return -1;
    }
    return target - min;
}

int main(int argc, char *argv[])
{
//    vector<int> data;
//    data.push_back(-12);
//    data.push_back(-5);
//    data.push_back(14);
//    data.push_back(5);
//    data.push_back(0);//-10
//    data.push_back(-16);//-10
////    [-12,-5,14,5,0,-16]
//    auto obj = closestTargetValue(-10,data);
//    qDebug()<<obj;
//    return 1;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
