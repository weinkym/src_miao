#ifndef CBASEDIALOG_H
#define CBASEDIALOG_H
#include <QDialog>
#include <QUuid>
#include <QVariant>

class CBaseDialog : public QDialog
{
    Q_OBJECT
public:
    typedef std::function<void(void *param, const QVariant &result)> DIALOG_CALLBACK;
    enum ShowType
    {
        SHOW_TYPE_NORMAL,
        SHOW_TYPE_HALF_MODAL,
        SHOW_TYPE_NORMAL_MODAL,
    };
    struct CallbackObject
    {
        CallbackObject()
            : delayCallbackMS(0)
            , callback(NULL)
            , param(NULL) {};
        int delayCallbackMS;
        DIALOG_CALLBACK callback;
        void *param;
        QVariant result;
    };
    explicit CBaseDialog(ShowType showType, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CBaseDialog();
    QUuid getUuid() const;
    ShowType getShowType() const;
    void startShow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void accept();
    void closeEvent(QCloseEvent *event);
    void setDefaultCallback(CallbackObject obj);

private:
    void doCallback(CallbackObject obj);

signals:
    void sigClosed(const QUuid &uuid, int showType);

private:
    friend class CBaseDialogManager;
    ShowType m_showType;
    QUuid m_uuid;
    CallbackObject m_defaultCallbackObj;
};
#endif // CBASEDIALOG_H
