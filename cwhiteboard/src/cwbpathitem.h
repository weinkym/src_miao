#ifndef CWBPATHITEM_H
#define CWBPATHITEM_H
#include <QGraphicsPathItem>
#include "cwhiteboard_global.h"

class CWHITEBOARDSHARED_EXPORT CWBPathItem : public QGraphicsPathItem
{
public:
    CWBPathItem(QGraphicsItem *parent = Q_NULLPTR);
    void appendLine(const QPointF &startPoint, const QPointF &endPoint);

private:
    bool m_isFirst;
};

#endif // CWBPATHITEM_H
