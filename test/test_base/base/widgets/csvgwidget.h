#ifndef CSVGWIDGET_H
#define CSVGWIDGET_H
#include "cclickwidget.h"

class QSvgRenderer;
class CSvgWidget : public CClickWidget
{
    Q_OBJECT
public:
    explicit CSvgWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CSvgWidget();
    void setSvgFile(const QString &filePath);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onRepaintNeeded();

private:
    QSvgRenderer *m_svgRenderer;
};

#endif // CSVGWIDGET_H
