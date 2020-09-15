
#include "clogsetting.h"
#include "ctestwidget.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#include <objbase.h>

GUID CreateGuid()
{
    GUID guid;
#ifdef WIN32
    CoCreateGuid(&guid);
#else
    uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif
    return guid;
}

std::string GuidToString(const GUID &guid)
{
    char buf[64] = { 0 };
#ifdef __GNUC__
    snprintf(
#else // MSVC
    _snprintf_s(
#endif
        buf,
        sizeof(buf),
        "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);
    return std::string(buf);
}

void test()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography", QSettings::NativeFormat);
    QString value = settings.value("MachineGuid", "").toString(); //读
    qDebug() << "  -----------  ";
    qDebug() << "" << value;
    std::string uuid = GuidToString(CreateGuid());
    qDebug() << "uuid" << uuid.c_str();
}

int main(int argc, char *argv[])
{
    test();
    return 1;
    QApplication a(argc, argv);
    return a.exec();
}
