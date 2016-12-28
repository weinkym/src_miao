#ifndef CWBADDCOMMAND_H
#define CWBADDCOMMAND_H
#include <QUndoCommand>

class CDrawItem;
class CWBAddCommand : public QUndoCommand
{
public:
    CWBAddCommand(const QList<CDrawItem*> itemList,QUndoCommand *parent = Q_NULLPTR);
    void undo();
    void redo();

private:
    QList<CDrawItem*> m_itemList;
};

#endif // CWBADDCOMMAND_H
