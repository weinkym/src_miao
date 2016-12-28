#include "cwbclearcommand.h"
#include "cdrawitem.h"

CWBClearCommand::CWBClearCommand(const QList<CDrawItem *> itemList, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_itemList(itemList)
{

}

void CWBClearCommand::undo()
{
    for(auto obj:m_itemList)
    {
        obj->setVisible(true);
    }
}

void CWBClearCommand::redo()
{
    for(auto obj:m_itemList)
    {
        obj->setVisible(false);
    }
}
