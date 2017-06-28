#include "mainwindow.h"
#include <QApplication>
#include <QTextEdit>

QTextEdit* g_log_textEdit = NULL;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
    if(g_log_textEdit)
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
   qInstallMessageHandler(myMessageOutput);
   QApplication a(argc, argv);
   g_log_textEdit = new QTextEdit;
   g_log_textEdit->show();
    MainWindow w;
    w.hide();
    w.showLogin();

    return a.exec();
}
