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
    static QPixmap getSvgFilePixmap(const QString &filePath, const QSize &size, int devicePixelRatio = 1);
};

#endif // CUIUTILS_H
