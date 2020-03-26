#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include "zfiletool.h"
#include "zdeletefiledlg.h"
#include "zrenamefiledlg.h"
#include "zcopyfiledlg.h"
#include "zshutdowndlg.h"
#include "zposcolordlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void onAppendMsg(const QString& msg);
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
//    void on_pushButton_copyfile_clicked();

//    void on_pushButton_path_clicked();

//    void on_pushButton_file_clicked();

//    void on_pushButton_rename_clicked();

//    void on_pButton_update_clicked();

//    void on_pushButton_delete_clicked();


private:
    enum{EV_INDEX = 1001};
    Ui::MainWindow *ui;
    QString m_bgImage;
    ZDeleteFileDlg* m_deleteFileDlg;
    ZRenameFileDlg* m_renameFileDlg;
    ZCopyFileDlg* m_copyFileDlg;
    ZShutdownDlg* m_shutdownDlg;
    ZPosColorDlg* m_posColorDlg;
//    ZFileTool m_fileTool;
//    QStandardItemModel* m_model;
};

#endif // MAINWINDOW_H
