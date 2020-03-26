#include <QCoreApplication>
#include "csqliteaccessinterface.h"
#include "cmessageinterface.h"
#include <QMetaEnum>
#include "ccontactmanager.h"
#include "cloginmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    ZW_LOG::initLog(Zpublic::getDataPath("log",true)+"/"+(QDateTime::currentDateTime().toString("yyyyMMddhhmmss.log")));
    CLoginManager::getInstance()->start();

    return a.exec();
}
