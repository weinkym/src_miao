#ifndef ZWWEBENGINEPAGE_H
#define ZWWEBENGINEPAGE_H
#include <QWebEnginePage>

class ZWWebEnginePage : public QWebEnginePage
{
    Q_OBJECT

public:
    ZWWebEnginePage(QObject *parent = Q_NULLPTR);
    ~ZWWebEnginePage();

private:

};

#endif // ZWWEBENGINEPAGE_H
