#ifndef ZWSINGLETON_H
#define ZWSINGLETON_H
#include <QMutex>
#include <QMutexLocker>

template <typename T>
class ZWSingleton
{
public:
    static T *getInstance()
    {
        static T* instance = nullptr;
        if(instance == nullptr)
        {
            QMutex s_mutex;
            QMutexLocker locker(&s_mutex);
            if(instance == nullptr)
            {
                instance = new T;
            }
        }
        return instance;
    };
    ZWSingleton(const ZWSingleton&)=delete;
    ZWSingleton& operator =(const ZWSingleton&)=delete;
    ~ZWSingleton(){};
    virtual void uninitialize(){};

protected:
    ZWSingleton(){};

private:
};
#endif // ZWSINGLETON_H
