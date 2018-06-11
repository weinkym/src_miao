#ifndef ZCATLOGWIDGET_H
#define ZCATLOGWIDGET_H

#include <QWidget>

namespace Ui {
class ZCatlogWidget;
}

class ZCatlogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZCatlogWidget(QWidget *parent = 0);
    ~ZCatlogWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void updateText();
    bool regExpString(const QRegExp &rx, const QString  &source,QString &dest);
    QString  toHtmlString(const QString &source,const QColor &backgroundColor,const QColor &color);
    void doUpdateFunction();
    QStringList regExpStringList(const QRegExp &rx, const QString  &source);
    void updateFilePathWithLog();

private slots:
    void on_pushButtonUpdate_clicked();

    void on_pushButtonLoad_clicked();

    void on_pushButtonClose_clicked();

    void on_pushButton_clicked();

private:
    Ui::ZCatlogWidget *ui;
//    QMap<int,QString> m_resultLineStringMap;
};

#endif // ZCATLOGWIDGET_H
