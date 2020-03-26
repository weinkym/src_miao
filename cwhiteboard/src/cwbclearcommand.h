#ifndef CWBCLEARCOMMAND_H
#define CWBCLEARCOMMAND_H
#include <QUndoCommand>

class CDrawItem;
class CWBClearCommand : public QUndoCommand
{
public:
    CWBClearCommand(const QList<CDrawItem*> itemList,QUndoCommand *parent = Q_NULLPTR);
    void undo();
    void redo();

private:
    QList<CDrawItem*> m_itemList;
};

#endif // CWBCLEARCOMMAND_H
