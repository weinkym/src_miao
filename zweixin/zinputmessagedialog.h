#ifndef ZINPUTMESSAGEDIALOG_H
#define ZINPUTMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ZInputMessageDialog;
}

class ZInputMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZInputMessageDialog(QWidget *parent = 0);
    ~ZInputMessageDialog();

private:
    Ui::ZInputMessageDialog *ui;
};

#endif // ZINPUTMESSAGEDIALOG_H
