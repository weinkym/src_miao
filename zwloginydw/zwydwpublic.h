#ifndef ZWYDWPUBLIC_H
#define ZWYDWPUBLIC_H
#include <QString>
#include <QDate>

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

}

class ZWYDWPublic
{
public:
    ZWYDWPublic();
};

#endif // ZWYDWPUBLIC_H
