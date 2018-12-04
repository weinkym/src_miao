#ifndef PROHIBITOR_H
#define PROHIBITOR_H

#include <Windows.h>
#include <QSettings>

class Prohibitor
{
public:
    Prohibitor();
    ~Prohibitor();

    void hook(bool flag); //功能键
    void hideTaskWindow(bool flag); //显示任务栏
    void disableTaskManager(bool flag); //任务管理器

private:
    HWND task;
};

#endif // PROHIBITOR_H
