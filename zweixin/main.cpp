#include "mainwindow.h"
#include <QApplication>
#include <QTextEdit>
#include "csqliteaccessinterface.h"
#include "cmessageinterface.h"
#include "zinputmessagedialog.h"
#include <QMetaEnum>
#include "ccontactmanager.h"
#include "zcontactlistwidget.h"

QTextEdit* g_log_textEdit = NULL;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
    if(g_log_textEdit/* && type == QtWarningMsg*/)
    {
        g_log_textEdit->append(QString("Debug: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function));
    }
//     QByteArray localMsg = msg.toLocal8Bit();
//     switch (type) {
//     case QtDebugMsg:
//         fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//         break;
//     case QtInfoMsg:
//         fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//         break;
//     case QtWarningMsg:
//         fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//         break;
//     case QtCriticalMsg:
//         fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//         break;
//     case QtFatalMsg:
//         fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//         abort();
//     }
 }

int main(int argc, char *argv[])
{
//   qInstallMessageHandler(myMessageOutput);
   QApplication a(argc, argv);
   {
//       QMetaEnum me = QMetaEnum::fromType<Zpublic::AutoSendEventType>();
//       LOG_INFO(QString("me count = %1").arg(me.keyCount()))
//        return -1;
//       CMessageInterface::getInstance()->init();
//       ZInputMessageDialog dlg;
//       dlg.resetContacts(CContactManager::getInstance()->m_contackMap.values());
//       dlg.show();

//       return a.exec();
   }
   qsrand(QTime::currentTime().msec());
   g_log_textEdit = new QTextEdit;
   g_log_textEdit->show();

    MainWindow w;
    w.hide();
    w.showLogin();

    return a.exec();
}
