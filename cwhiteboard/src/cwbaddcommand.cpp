#include "cwbaddcommand.h"
#include "cdrawitem.h"

CWBAddCommand::CWBAddCommand(const QList<CDrawItem *> itemList, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_itemList(itemList)
{

}

void CWBAddCommand::undo()
{
    for(auto obj:m_itemList)
    {
        if(obj)
        {
            obj->setVisible(false);
        }
    }
}

void CWBAddCommand::redo()
{
    for(auto obj:m_itemList)
    {
        if(obj)
        {
            obj->setVisible(true);
        }
    }
}
