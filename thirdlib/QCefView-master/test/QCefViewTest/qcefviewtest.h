#ifndef QCEFVIEWTEST_H
#define QCEFVIEWTEST_H

#include "customcefview.h"
#include "ui_qcefviewtest.h"
#include <QtWidgets/QMainWindow>

class QCefViewTest : public QMainWindow
{
    Q_OBJECT

public:
    QCefViewTest(QWidget *parent = 0);
    ~QCefViewTest();

protected slots:
    void onBtnChangeColorClicked();

private:
    Ui::QCefViewTestClass ui;
    CustomCefView *cefview;
};

#endif // QCEFVIEWTEST_H
