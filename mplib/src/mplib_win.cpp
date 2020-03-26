#include "mplib.h"
#include <QString>
#include <QDir>

#ifdef Q_CC_MSVC
#include <atlconv.h>
#include <WinSpool.h>
#include <shellapi.h>
#include <windows.h>
#include <shlobj.h>
#include <cstring>

void Mplib::MpStaticMethod::openFinder(const QString &filePath)
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

QStringList Mplib::MpStaticMethod::getPrinters()
{
    DWORD dwSize,dwPrinters;
    ::EnumPrinters(PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL,NULL,5,NULL,0,&dwSize,&dwPrinters);
    BYTE *pBuffer=new BYTE[dwSize];
   ::EnumPrinters(PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL,NULL,5,pBuffer,dwSize,&dwSize,&dwPrinters);
    QStringList list;
    if(dwPrinters != 0)
    {
        PRINTER_INFO_5 *pPrnInfo=(PRINTER_INFO_5 *)pBuffer;
        for(int i = 0;i < dwPrinters; i++)
        {
            USES_CONVERSION;
            std::string  str( W2A(pPrnInfo-> pPrinterName)) ;
            list.append(str.c_str());
            pPrnInfo++;//指针后移
        }
    }
    delete []pBuffer;
    return list;
}

QString Mplib::MpStaticMethod::getDefaultPrinter()
{
    QString result;
    DWORD dwSize = 0;
    if(!GetDefaultPrinter(NULL, &dwSize)
            && GetLastError()==ERROR_INSUFFICIENT_BUFFER)
    {
        TCHAR *szPrinter = new TCHAR[dwSize+1];
        if(GetDefaultPrinter(szPrinter, &dwSize))
        {
            USES_CONVERSION;
            std::string  str( W2A(szPrinter)) ;
            result = str.c_str();
        }
        delete []szPrinter;
    }
    return result;
}

bool Mplib::MpStaticMethod::setDefaultPrinter(const QString &name)
{
    USES_CONVERSION;
    LPWSTR szPrinterName = A2W(name.toLocal8Bit().data());

    BOOL ret = SetDefaultPrinter(szPrinterName);
    bool result = true;
    if(ret == FALSE)
    {
        result = false;
    }
//    qDebug()<<"设置默认打印机"<< (result ? "成功":"失败");
    return result;
}

//==可扩充,修改pi2->pDevMode对应的参数即可,但需pDevMode->dmFields所支持
bool Mplib::MpStaticMethod::setPrinterParam(const QString &pName, short pagesize, short width, short height, short orientation, short copies)
{
    HANDLE hPrinter = NULL;
    DWORD dwNeeded = 0;
    PRINTER_INFO_2 *pi2 = NULL;
    DEVMODE *pDevMode = NULL;
    PRINTER_DEFAULTS pd;
    BOOL bFlag;
    LONG lFlag;
    // Open printer handle (on Windows NT, you need full-access because you
    // will eventually use SetPrinter)...
    ZeroMemory(&pd, sizeof(pd));
    pd.DesiredAccess = PRINTER_ALL_ACCESS;
#ifdef UNICODE
    USES_CONVERSION;
    LPWSTR pPrinterName = A2W(pName.toLocal8Bit().data());
#else
//    LPSTR pPrinterName = A2W(name.toLocal8Bit().data());
    LPSTR pPrinterName = name.toLocal8Bit().data();
#endif // !UNICODE

    bFlag = OpenPrinter(A2W(pName.toLocal8Bit().data()), &hPrinter, &pd);
    if (!bFlag || (hPrinter == NULL))
    {
        qDebug()<<QString("打印机<%1>打开失败").arg(pName);
        return false;
    }
    // The first GetPrinter tells you how big the buffer should be in
    // order to hold all of PRINTER_INFO_2. Note that this should fail with
    // ERROR_INSUFFICIENT_BUFFER.  If GetPrinter fails for any other reason
    // or dwNeeded isn't set for some reason, then there is a problem...
    SetLastError(0);
    bFlag = GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
         if ((!bFlag) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER) ||
         (dwNeeded == 0))
    {
        ClosePrinter(hPrinter);
//        qDebug()<<QString("获取打印机<%1>信息失败<%2>").arg(pName).arg(GetLastError());
        return false;
    }
    // Allocate enough space for PRINTER_INFO_2...
    pi2 = (PRINTER_INFO_2 *)GlobalAlloc(GPTR, dwNeeded);
    if (pi2 == NULL)
    {
        ClosePrinter(hPrinter);
//        qDebug()<<QString("获取打印机<%1>信息时，申请不到足够的空间").arg(pName);
        return false;
    }
    // The second GetPrinter fills in all the current settings, so all you
    // need to do is modify what you're interested in...
    bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
    if (!bFlag)
    {
        GlobalFree(pi2);
        ClosePrinter(hPrinter);
//        qDebug()<<QString("获取打印机<%1>默认信息失败").arg(pName);
        return false;
    }
    // If GetPrinter didn't fill in the DEVMODE, try to get it by calling
    // DocumentProperties...
    if (pi2->pDevMode == NULL)
    {
        dwNeeded = DocumentProperties(NULL, hPrinter,
                        pPrinterName,
                        NULL, NULL, 0);
        if (dwNeeded <= 0)
        {
            GlobalFree(pi2);
            ClosePrinter(hPrinter);
            qDebug()<<QString("获取打印机<%1>信息失败,error<429>").arg(pName);
            return false;
        }

        pDevMode = (DEVMODE *)GlobalAlloc(GPTR, dwNeeded);
        if (pDevMode == NULL)
        {
            GlobalFree(pi2);
            ClosePrinter(hPrinter);
            qDebug()<<QString("获取打印机<%1>信息失败,error<438>").arg(pName);
            return false;
        }

        lFlag = DocumentProperties(NULL, hPrinter,
                        pPrinterName,
                        pDevMode, NULL,
                        DM_OUT_BUFFER);
        if (lFlag != IDOK || pDevMode == NULL)
        {
            GlobalFree(pDevMode);
            GlobalFree(pi2);
            ClosePrinter(hPrinter);
            qDebug()<<QString("获取打印机<%1>信息失败,error<451>").arg(pName);
            return false;
        }

        pi2->pDevMode = pDevMode;
    }

    //=======================================================
    bool result = true;
    // Driver is reporting that it doesn't support this change...
    // Specify exactly what we are attempting to change...
    //判断此熟悉是否支持
    if (!(pi2->pDevMode->dmFields & DM_ORIENTATION))
    {
        qDebug()<<QString("打印机<%1>不支持修改<纵横向>").arg(pName);
        result = false;
    }
    else
    {
        pi2->pDevMode->dmOrientation = orientation;
    }

    if(pagesize > 0)
    {
        if (!(pi2->pDevMode->dmFields & DM_PAPERSIZE))
        {
            qDebug()<<QString("打印机<%1>不支持修改<纸张规格>").arg(pName);
            result = false;
        }
        else
        {
            pi2->pDevMode->dmPaperSize = pagesize;
        }
    }
    else
    {
        if (!(pi2->pDevMode->dmFields & DM_PAPERWIDTH))
        {
            qDebug()<<QString("打印机<%1>不支持修改<纸张宽度>").arg(pName);
            result = false;
        }
        else
        {
            pi2->pDevMode->dmPaperLength = height;
        }

        if (!(pi2->pDevMode->dmFields & DM_PAPERLENGTH))
        {
            qDebug()<<QString("打印机<%1>不支持修改<纸张长度>").arg(pName);
            result = false;
        }
        else
        {
            pi2->pDevMode->dmPaperWidth = width;
        }
    }

    if (!(pi2->pDevMode->dmFields & DM_COPIES))
    {
        qDebug()<<QString("打印机<%1>不支持修改<打印份数>").arg(pName);
        result = false;
    }
    else
    {
        pi2->pDevMode->dmCopies = copies;
    }
    //=======================================================
    // Do not attempt to set security descriptor...
    pi2->pSecurityDescriptor = NULL;
    // Make sure the driver-dependent part of devmode is updated...
    lFlag = DocumentProperties(NULL, hPrinter,
          pPrinterName,
          pi2->pDevMode, pi2->pDevMode,
          DM_IN_BUFFER | DM_OUT_BUFFER);
    if (lFlag != IDOK)
    {
        GlobalFree(pi2);
        ClosePrinter(hPrinter);
        if (pDevMode)
            GlobalFree(pDevMode);
        return false;
    }
    // Update printer information...
    bFlag = SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
    if (!bFlag)
    // The driver doesn't support, or it is unable to make the change...
    {
        GlobalFree(pi2);
        ClosePrinter(hPrinter);
        if (pDevMode)
            GlobalFree(pDevMode);
        qDebug()<<"TTTTTTTTTT<276>";
        return false;
    }
    // Tell other apps that there was a change...
//    SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L,
//              (LPARAM)(LPCSTR)pPrinterName,
//              SMTO_NORMAL, 1000, NULL);

    // Clean up...
    if (pi2)
        GlobalFree(pi2);
    if (hPrinter)
        ClosePrinter(hPrinter);
    if (pDevMode)
        GlobalFree(pDevMode);

    return result;
}
#endif
