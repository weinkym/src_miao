#ifndef TESTZWLABELDIALOG_H
#define TESTZWLABELDIALOG_H

#include <QDialog>
#include "zwlabeltext.h"

namespace Ui {
class TestZWLabelDialog;
}

class TestZWLabelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestZWLabelDialog(QWidget *parent = 0);
    ~TestZWLabelDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestZWLabelDialog *ui;
    ZWLabelText *m_textLabel;
};

#endif // TESTZWLABELDIALOG_H
