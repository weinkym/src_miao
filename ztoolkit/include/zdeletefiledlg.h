#ifndef ZDELETEFILEDLG_H
#define ZDELETEFILEDLG_H
#include <QDialog>
#include "ui_zdeletefiledlg.h"

class QStandardItemModel;
class ZDeleteFileDlg : public QDialog
{
    Q_OBJECT

public:
    enum COLUMN_TYPE
    {
        COLUMN_TYPE_FILE_NAME,
        COLUMN_TYPE_FILE_TYPE,
        COLUMN_TYPE_FILE_STATUS,
        COLUMN_TYPE_FILE_PATH,
    };

    ZDeleteFileDlg(QWidget* parent = 0,Qt::WindowFlags f = 0);
    ~ZDeleteFileDlg();

protected:
    void initSetting();

private slots:
    void on_btn_update_clicked();
    void on_btn_path_clicked();
    void on_btn_delete_clicked();

private:
    Ui::ZDeleteFileDlg ui;
    QStandardItemModel* m_model;
    QMap<COLUMN_TYPE,QString> m_typeMap;
};

#endif // ZDELETEFILEDLG_H
