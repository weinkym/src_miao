#include "zwcore.h"

void ZWCore::openFinder(const QString &filePath)
{
    QString param=QString(" /select, %1").arg(QDir::toNativeSeparators(filePath));
//    QString param=QString(" /select, D:\\kclive\\test.html");
    USES_CONVERSION;
    LPWSTR lpwstr_open = A2W("open");
    LPWSTR lpwstr_explorer = A2W("explorer.exe");
    LPWSTR lpwstr_param = A2W(param.toLocal8Bit().data());
    ShellExecute(NULL, lpwstr_open, lpwstr_explorer, lpwstr_param, NULL, SW_SHOWNORMAL);
    // 采用下面的也可以实现
//    ShellExecute(NULL, TEXT("open"), TEXT("explorer.exe"), lpwstr_param, NULL, SW_SHOWNORMAL);
}
