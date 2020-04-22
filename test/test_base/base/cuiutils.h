#ifndef CUIUTILS_H
#define CUIUTILS_H
#include <QFont>
#include <QLabel>
#include <QStringList>

class CUIUtils
{
public:
    CUIUtils();
    ~CUIUtils();
    static QStringList elidedTextList(const QString &source, const QFont &font, int width, uint maxRow, const QString &subffix = "...");
    static QStringList elidedTextListWidthlineBreak(const QString &source, const QFont &font, int width, uint maxRow, const QString &subffix = "...");

    static void setFullChildWidget(QWidget *parent, QWidget *childWidget, const QMargins &margins = QMargins());
};

#endif // CUIUTILS_H
