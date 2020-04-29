#include "crangedata.h"
#include "clogsetting.h"

const char *G_SQUARE_BRACKET_L = "[";
const char *G_SQUARE_BRACKET_R = "]";
const char *G_SEPARATOR = ",";

CRangeData::CRangeData(quint64 min, quint64 max)
    : m_isValid(min <= max)
{
    m_dataList.append(QPair<quint64, quint64>(min, max));
}

CRangeData::CRangeData(const QString &formatString)
    : m_isValid(false)
{
    do
    {
        QStringList objList = formatString.split(G_SEPARATOR);
        if(objList.isEmpty())
        {
            break;
        }
        quint64 count = objList.count();
        if(count % 2 != 0)
        {
            break;
        }
        bool isValid = true;
        quint64 currentMax = 0;
        for(quint64 i = 0; i < count / 2; ++i)
        {
            bool ok = true;
            quint64 min = objList.at(2 * i).toInt(&ok);
            if(!ok)
            {
                isValid = false;
                break;
            }
            quint64 max = objList.at(2 * i + 1).toInt(&ok);
            if(!ok)
            {
                isValid = false;
                break;
            }
            if(max < min)
            {
                isValid = false;
                break;
            }
            if(i != 0)
            {
                if(min <= currentMax)
                {
                    isValid = false;
                    break;
                }
            }
            m_dataList.append(QPair<quint64, quint64>(min, max));
            currentMax = max;
        }
        m_isValid = isValid;
    } while(0);
}

CRangeData::~CRangeData()
{
}

void CRangeData::remove(quint64 index)
{
    if(isFinished())
    {
        return;
    }
    m_doingIndexList.removeAll(index);

    for(quint64 i = 0; i < m_dataList.count(); ++i)
    {
        QPair<quint64, quint64> obj = m_dataList.at(i);
        if(obj.first > index)
        {
            //todo warning
            return;
        }
        if(obj.second < index)
        {
            continue;
        }
        QList<QPair<quint64, quint64> > objList = removePairOne(obj, index);
        m_dataList.removeAt(i);
        for(quint64 k = 0; k < objList.count(); ++k)
        {
            //            m_dataList.indexOf(objList.at(k), i + k);
            m_dataList.insert(i + k, objList.at(k));
            qDebug() << __LINE__ << __FUNCTION__ << k << m_dataList.count();
        }
        break;
    }
}

bool CRangeData::isFinished() const
{
    return m_dataList.isEmpty();
}

bool CRangeData::isValid() const
{
    return m_isValid;
}

QString CRangeData::toFormatString() const
{
    QStringList strList;
    //    strList.append(G_SQUARE_BRACKET_L);
    foreach(auto obj, m_dataList)
    {
        //        strList.append(QString("%1%2%3%4%5").arg(G_SQUARE_BRACKET_L).arg(obj.first).arg(G_SEPARATOR).arg(obj.second).arg(G_SQUARE_BRACKET_R));
        strList.append(QString("%1%2%3").arg(obj.first).arg(G_SEPARATOR).arg(obj.second));
    }
    //    strList.append(G_SQUARE_BRACKET_R);
    return strList.join(G_SEPARATOR);
}

quint64 CRangeData::getMax() const
{
    if(isValid())
    {
        return m_dataList.last().second;
    }
    return 0;
}

quint64 CRangeData::getFreeIndex(bool &ok)
{
    C_LOG_OUT_V2(isValid(), m_dataList.isEmpty());
    ok = false;
    if(!isValid() || m_dataList.isEmpty())
    {
        return 0;
    }
    for(auto obj : m_dataList)
    {
        for(int i = obj.first; i <= obj.second; ++i)
        {
            if(appendDoingIndex(obj.first))
            {
                ok = true;
                break;
            }
        }
        if(ok)
        {
            break;
        }
    }
    C_LOG_OUT_V2(ok, m_doingIndexList.isEmpty());
    if(ok)
    {
        if(!m_doingIndexList.isEmpty())
        {
            return m_doingIndexList.last();
        }
        else
        {
            C_LOG_WARNING("m_doingIndexList is empty");
        }
    }
    return 0;
}

QList<QPair<quint64, quint64> > CRangeData::removePairOne(const QPair<quint64, quint64> &obj, quint64 index)
{
    QList<QPair<quint64, quint64> > objList;
    if(obj.first == index)
    {
        if(obj.second != index)
        {
            objList.append(QPair<quint64, quint64>(obj.first + 1, obj.second));
        }
    }
    else if(obj.second == index)
    {
        if(obj.first != index)
        {
            objList.append(QPair<quint64, quint64>(obj.first, obj.second - 1));
        }
    }
    else if(index > obj.first && index < obj.second)
    {
        objList.append(QPair<quint64, quint64>(obj.first, index - 1));
        objList.append(QPair<quint64, quint64>(index + 1, obj.second));
    }
    qDebug() << __LINE__ << __FUNCTION__ << index << objList.count();
    return objList;
}

bool CRangeData::appendDoingIndex(quint64 index)
{
    if(m_doingIndexList.contains(index))
    {
        return false;
    }
    m_doingIndexList.append(index);
    return true;
}
