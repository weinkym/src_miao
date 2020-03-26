#include "cutilinterface.h"

CUtilInterface::CUtilInterface()
{

}

QList<CYDW::KEY_DATA> CUtilInterface::getTableInfo()
{
    QList<CYDW::KEY_DATA> objList;
    //编号
    objList.append({T_SERIAL_NUMBER,CYDW::KEY_TYPE_INT,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //项目名称
    objList.append({T_PROJECT_NAME,CYDW::KEY_TYPE_STRING,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //出借日期
    objList.append({T_LEND_DATE,CYDW::KEY_TYPE_DATE,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //期限
    objList.append({T_DEADLINE,CYDW::KEY_TYPE_INT,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //期限类型
    objList.append({T_DEADLINE_TYPE,CYDW::KEY_TYPE_ENUM,CYDW::ENUM_TYPE_ID_DEADLINE_TYPE});
    //年利率
    objList.append({T_ANNUAL_RATE,CYDW::KEY_TYPE_FLOAT,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //出借金额
    objList.append({T_LOAN_AMOUNT,CYDW::KEY_TYPE_INT,CYDW::ENUM_TYPE_ID_UNDEFINED});
    //出借人
    objList.append({T_LENDERS,CYDW::KEY_TYPE_ENUM,CYDW::ENUM_TYPE_ID_LENDERS});
}
