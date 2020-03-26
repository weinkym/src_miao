#ifndef ZCOPYFILEDLG_H
#define ZCOPYFILEDLG_H
#include <QDialog>
#include <QFileInfo>
#include "ui_zcopyfiledlg.h"

class QStandardItemModel;
class ZCopyFileDlg : public QDialog
{
    Q_OBJECT

public:
    ZCopyFileDlg(QWidget* parent = 0,Qt::WindowFlags f = 0);
    ~ZCopyFileDlg();

protected:
    void initSetting();
    QMap<QString,QStringList > getCopyFileInfo(int& count);
    bool foundExistDir(const QStringList& dirs,const QString filePath);

private slots:
    void on_btn_update_clicked();
    void on_btn_path_clicked();
    void on_btn_copy_clicked();

    void on_btn_file_clicked();

private:
    enum{E_NAME = 0,E_FROM_PATH,E_TO_PATH,E_TYPE,E_STATE,E_SIZE};
    QStringList m_labels;
    Ui::ZCopyFileDlg ui;
    QStandardItemModel* m_model;
};

#endif // ZCOPYFILEDLG_H
