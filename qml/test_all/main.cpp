#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

class A
{
public:
    A()
        : m_count(1)
    {
        qDebug() << Q_FUNC_INFO;
    };
    A(int count)
        : m_count(count)
    {
        qDebug() << Q_FUNC_INFO;
    };
    A(const A &a)
        : m_count(a.m_count)
    {
        qDebug() << Q_FUNC_INFO;
    };
    A(A &&a)
        : m_count(a.m_count)
    {
        qDebug() << Q_FUNC_INFO;
    };
    ~A()
    {
        m_count = 0;
        qDebug() << Q_FUNC_INFO;
    };
    int m_count;
};

void test()
{
    std::vector<A> objList;
    A a(8);
    //    qDebug() << __LINE__ << a.m_count;
    objList.push_back(a);
    //    objList.clear();
    //    qDebug() << __LINE__ << a.m_count;
    objList.push_back(std::move(a));
    qDebug() << __LINE__ << a.m_count;

    objList.clear();
}

int main(int argc, char *argv[])
{
    test();
    return 1;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    //    const QUrl url(QStringLiteral("qrc:/login/ZLoginView.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if(!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
