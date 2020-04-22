#ifndef CTESTDIALOG_H
#define CTESTDIALOG_H

#include "cbasedialog.h"

namespace Ui
{
class CTestDialog;
}

class CTestDialog : public CBaseDialog
{
    Q_OBJECT

public:
    explicit CTestDialog(ShowType showType, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CTestDialog();
    void setText(const QString &text);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CTestDialog *ui;
};

#endif // CTESTDIALOG_H
