#include "zwlabeltext.h"
#include "zwgui.h"
#include "zwlog.h"
#include <QLayout>

ZWLabelText::ZWLabelText(QWidget *parent, Qt::WindowFlags f)
    :QLabel(parent,f)
    ,m_maxRow(1)
    ,m_subffix("...")
{

}

ZWLabelText::~ZWLabelText()
{

}

void ZWLabelText::setMaxRow(uint maxRow)
{
    if(m_maxRow == maxRow)
    {
        return;
    }
    m_maxRow = maxRow;
}

void ZWLabelText::setMaxRowText(const QString &text, int width, uint maxRow, const QString &subffix)
{
    if(maxRow <= 0 || width <= 0)
    {
        return;
    }

    QStringList stringList = ZWGui::elidedTextListWidthlineBreak(text,this->font(),width,maxRow,subffix);
    ZW_LOG_INFO(QString("row = %1").arg(stringList.count()));
    this->setText(stringList.join("\n"));
}

