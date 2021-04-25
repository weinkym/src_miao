#ifndef ZWCLICKEDWIDGET_H
#define ZWCLICKEDWIDGET_H
#include <QWidget>
#include <QUuid>
#include <QVariant>
#include "zwgui_global.h"

class ZWGUISHARED_EXPORT ZWClickedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZWClickedWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ZWClickedWidget();
    int getId() const;
    QUuid getUuid() const;
    void setId(int id);
    QVariant getUserData() const;
    void setUserData(const QVariant &value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void sigClicked(int,QUuid,QVariant);

private:
    int m_id;
    bool m_isLeftPressed;
    QUuid m_uuid;
    QVariant m_userData;
};

#endif // ZWCLICKEDWIDGET_H
