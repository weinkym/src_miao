#ifndef CPUBLICDEFINED_H
#define CPUBLICDEFINED_H
#include <QList>
#include <QObject>

namespace CYDW
{
enum KEY_TYPE{
    KEY_TYPE_INT,
    KEY_TYPE_FLOAT,
    KEY_TYPE_DATE,
    KEY_TYPE_DATE_TIME,
    KEY_TYPE_TIME,
    KEY_TYPE_ENUM,
    KEY_TYPE_STRING,
};
enum ENUM_TYPE_ID
{
    ENUM_TYPE_ID_UNDEFINED,
    ENUM_TYPE_ID_LENDERS,
    ENUM_TYPE_ID_DEADLINE_TYPE,
};

struct KEY_DATA
{
    QString name;
    KEY_TYPE type;
    ENUM_TYPE_ID id;
};
}

//编号
const QString T_SERIAL_NUMBER = QObject::tr("serial number");
//项目名称
const QString T_PROJECT_NAME = QObject::tr("project name");
//出借日期
const QString T_LEND_DATE = QObject::tr("Lend the date");
//期限
const QString T_DEADLINE = QObject::tr("deadline");
//期限类型
const QString T_DEADLINE_TYPE = QObject::tr("deadline type");
//年利率
const QString T_ANNUAL_RATE = QObject::tr("annual rate");
//出借金额
const QString T_LOAN_AMOUNT = QObject::tr("loan amount");
//出借人
const QString T_LENDERS = QObject::tr("lenders");

#endif // CPUBLICDEFINED_H
