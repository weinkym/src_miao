#ifndef ZRENAMEFILEDLG_H
#define ZRENAMEFILEDLG_H
#include <QDialog>
#include "ui_zrenamefiledlg.h"

class QStandardItemModel;
class ZRenameFileDlg : public QDialog
{
    Q_OBJECT

public:
    ZRenameFileDlg(QWidget* parent = 0,Qt::WindowFlags f = 0);
    ~ZRenameFileDlg();

protected:
    void initSetting();
    QString rename(const QStringList& delList,const QString& name);

private slots:
    void on_btn_update_clicked();

    void on_btn_path_clicked();

    void on_btn_rename_clicked();

private:
    enum{E_NAME = 0,E_NEWNAME,E_TYPE,E_STATE,E_PATH};
    QStringList m_labels;

    Ui::ZRenameFileDlg ui;
    QStandardItemModel* m_model;
};
#endif // ZRENAMEFILEDLG_H
