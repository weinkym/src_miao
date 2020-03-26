#ifndef ZWLABELTEXT_H
#define ZWLABELTEXT_H

#include <QLabel>
#include "zwgui_global.h"

class ZWGUISHARED_EXPORT ZWLabelText : public QLabel
{
    Q_OBJECT

public:
    ZWLabelText(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ZWLabelText();
    void setMaxRow(uint maxRow);
    void setMaxRowText(const QString &text,int width,uint maxRow,const QString &subffix = "...");

protected:
    uint m_maxRow;
    QString m_subffix;
};

#endif // ZWLABELTEXT_H
