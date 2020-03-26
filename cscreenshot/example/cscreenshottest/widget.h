#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QStandardPaths>
#include <QUuid>
#include <QMimeData>
#include <QClipboard>
#include <QFileInfo>
#include <QUrl>

namespace Ui {
class Widget;
}

class QProcess;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void onScreenShotPixmapChanged(const QPixmap &pixmap);
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_btnShot_clicked();

private:
    Ui::Widget *ui;
    QProcess *m_proess;
};

#endif // WIDGET_H
