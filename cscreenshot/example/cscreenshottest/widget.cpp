#include "widget.h"
#include "ui_widget.h"
#include "cscreenshotmanager.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(CScreenShotManager::getInstance(),SIGNAL(sigScreenShotPixmapChanged(QPixmap)),
            this,SLOT(onScreenShotPixmapChanged(QPixmap)));
    //    ui->labelPixmap->setScaledContents(true);
    //    this->setAttribute(Qt::WA_QuitOnClose,false);
    onScreenShotPixmapChanged(QPixmap());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onScreenShotPixmapChanged(const QPixmap &pixmap)
{
    QPixmap tempPixmap = pixmap;
    tempPixmap.setDevicePixelRatio(ui->labelPixmap->devicePixelRatioF());
    ui->labelPixmap->setPixmap(pixmap);


    QUuid uid = QUuid::createUuid();
    QString  finderName = uid.toString() + ".jpg";
//    QString  finderName =  "{1f027903-fa3a-4f4a-b20c-e0942b99252a}.jpg";
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if(path.isEmpty())
    {
        path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    QString imagePath =QApplication::applicationDirPath() + "/"+finderName;
    bool saveFlag = pixmap.save(imagePath, "JPG",100);
    if(saveFlag)
    {
        QClipboard *cb = QApplication::clipboard();
        // Ownership of the new data is transferred to the clipboard.
        QMimeData* newMimeData = new QMimeData();
        // Copy old mimedata
        const QMimeData* oldMimeData = cb->mimeData();
        for ( const QString &f : oldMimeData->formats())
        {
            qDebug()<<QString("formats=%1").arg(f)<<oldMimeData->data(f);
            newMimeData->setData(f, oldMimeData->data(f));
        }
        for ( const QUrl &url : oldMimeData->urls())
        {
            qDebug()<<QString("oldMimeData url=%1").arg(url.toString());
        }
        // Copy file (gnome)
        QList<QUrl> dataUrls;
        QStringList paths(imagePath);
        for (QString path : paths)
        {
            dataUrls << QUrl(QString("file:///%1").arg(path));
        }
        newMimeData->setUrls(dataUrls);
        cb->setMimeData(newMimeData);
        for ( const QUrl &url : newMimeData->urls())
        {
            qDebug()<<QString("url=%1").arg(url.toString());
        }
    }
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_pushButton_clicked()
{
    //
}

void Widget::on_btnShot_clicked()
{
    CScreenShotManager::getInstance()->startScreenShot();
}
