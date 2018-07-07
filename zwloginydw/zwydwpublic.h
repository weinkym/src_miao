#ifndef ZWYDWPUBLIC_H
#define ZWYDWPUBLIC_H
#include <QString>
#include <QDate>
#include <QVariantMap>


namespace ZWYDW {
enum DeadlineType{
    DEADLINE_TYPE_DAY,
    DEADLINE_TYPE_MONTH,
};

struct YXData{
    QString title;
    QString code;
    QString status;
    qreal rate;
    qreal dRate;
    qreal money;
    qreal repaymentMoney;
    qreal interimRepaymentMoney;
    int deadlineType;
    int deadline;
    QDate repaymentDate;
};

struct MoneyData
{
    MoneyData()
        :amount(0)
        ,account_balance(0)
        ,recent_receivable_amount(0)
        ,recent_refund_amount(0){};
    static double getDoubleDataFun(const QVariant &value,bool &ok);
    static QDate getDateFun(QVariant &value,bool &ok);
    qreal amount;
    qreal account_balance;
    qreal recent_receivable_amount;
    qreal recent_refund_amount;
    QDate recent_receivable_amount_date;
    QDate recent_refund_amount_date;
    QString errorString;
};

}

class ZWYDWPublic
{
public:
    static QDate getAmountDate(const QString &src);
    static QStringList getMatchList(const QString &src,const QRegExp &re);
    static double conertStringToDouble(const QString &src,bool &ok);
    static ZWYDW::MoneyData parseMoneyData(const QVariantMap &objMap,bool &ok);

    ZWYDWPublic();
};

#endif // ZWYDWPUBLIC_H
