#ifndef CRANGEDATA_H
#define CRANGEDATA_H

#include <QList>

class CRangeData
{
public:
    CRangeData(quint64 min, quint64 max);
    CRangeData(const QString &formatString);
    ~CRangeData();
    void remove(quint64 index);
    bool isFinished() const;
    bool isValid() const;
    QString toFormatString() const;
    quint64 getMax() const;

private:
    QList<QPair<quint64, quint64> > removePairOne(const QPair<quint64, quint64> &obj, quint64 index);

private:
    bool m_isValid;
    QList<QPair<quint64, quint64> > m_dataList;
};

#endif // CRANGEDATA_H
