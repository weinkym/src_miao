#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zcopyfiledlg.h"
#include "mplib_global.h"
#include "mplib.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);
    //功能
    ui->treeWidget->setHeaderLabel("function");
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        //删除文件
        item->setText(0,tr("delete file"));
        item->setData(0,EV_INDEX,ui->stackedWidget->count());
        m_deleteFileDlg = new ZDeleteFileDlg;
        ui->stackedWidget->addWidget(m_deleteFileDlg);
    }
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        //待复制
        item->setText(0,tr("rename"));
        item->setData(0,EV_INDEX,ui->stackedWidget->count());
        m_renameFileDlg = new ZRenameFileDlg;
        ui->stackedWidget->addWidget(m_renameFileDlg);
//        ui->stackedWidget->setCurrentIndex(1);
    }
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0,"文件复制");
        item->setData(0,EV_INDEX,ui->stackedWidget->count());
        m_copyFileDlg = new ZCopyFileDlg;
        ui->stackedWidget->addWidget(m_copyFileDlg);
//        ui->stackedWidget->setCurrentIndex(1);
    }
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        //自动关机
        item->setText(0,tr("auto shot down"));
        item->setData(0,EV_INDEX,ui->stackedWidget->count());
        m_shutdownDlg = new ZShutdownDlg;
        ui->stackedWidget->addWidget(m_shutdownDlg);
//        ui->stackedWidget->setCurrentIndex(1);
    }

    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        //颜色点
        item->setText(0,tr("color point"));
        item->setData(0,EV_INDEX,ui->stackedWidget->count());
        m_posColorDlg = new ZPosColorDlg;
        ui->stackedWidget->addWidget(m_posColorDlg);
//        ui->stackedWidget->setCurrentIndex(1);
    }

    connect(ui->treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    qsrand(QTime::currentTime().second());
    m_bgImage = QString(":/skin/%1.jpg").arg(qrand() % 3);
    ZSetMsgHandlerDevice(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppendMsg(const QString &msg)
{
    DEBUG_CURRENT_LINE;
    ui->textEdit->append(msg);
    qApp->processEvents();
}

void MainWindow::onCurrentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *)
{
int index = item->data(0,EV_INDEX).toInt();
ui->stackedWidget->setCurrentIndex(index);
}

//void MainWindow::on_pushButton_copyfile_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    QFile file(ui->lineEdit_file->text());
//    if(file.open(QIODevice::ReadOnly))
//    {
//        QTextStream doc(&file);
//        QString filters = ui->lineEdit_filters->text();
//        QStringList fileList;
//        while(!doc.atEnd())
//        {
//            QString fileName = doc.readLine();
//            if(fileName.contains(filters))
//            {
//                continue;
//            }
//            fileList.append(fileName);
//        }
//        m_fileTool.copyFile(fileList,ui->lineEdit_path->text());
//    }
//    else
//    {
//        onAppendMsg(QString("打开文件<%1>失败").arg(ui->lineEdit_file->text()));
//    }
//}

//void MainWindow::on_pushButton_path_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//                                                    ui->lineEdit_path->text(),
//                                                    QFileDialog::ShowDirsOnly
//                                                    | QFileDialog::DontResolveSymlinks);
//    if(!dir.isEmpty())
//    {
//        ui->lineEdit_path->setText(dir);
//    }
//}

//void MainWindow::on_pushButton_file_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                     ui->lineEdit_file->text(),
//                                                     tr("Images (*.*)"));
//    if(!fileName.isEmpty())
//    {
//        ui->lineEdit_file->setText(fileName);
//    }
//}

//void MainWindow::on_pushButton_rename_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    m_fileTool.renameFile(ui->lineEdit_path->text(),QStringList(),"");
//}

//void MainWindow::on_pButton_update_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    ui->tableView->setModel(NULL);
//    m_model->clear();
//    QStringList filters = ui->lineEdit_del_filters->text().trimmed().split(";",QString::SkipEmptyParts);
//    QList<QFileInfo> allNames;
//    m_fileTool.findFile(allNames,ui->lineEdit_path->text(),QStringList());
//    onAppendMsg(filters.join("-"));
//    QList<QFileInfo> delNames;
//    foreach (QFileInfo fileInfo, allNames)
//    {
//        QString fileName = fileInfo.fileName();
//        bool isFound =false;
//        foreach (QString filter , filters)
//        {
//            if(/*fileName.contains(QRegExp(filter,Qt::CaseInsensitive)) ||*/ fileName.endsWith(filter,Qt::CaseInsensitive) || fileName.toUpper() == filter.toUpper())
//            {
//                isFound = true;
//                break;
//            }
//        }
//        if(isFound)
//        {
//            delNames.append(fileInfo);
//        }
//    }
//    QStringList labels;
//    labels<<"文件"<<"类型"<<"状态";
//    m_model->setHorizontalHeaderLabels(labels);
//    m_model->setRowCount(delNames.count());
//    m_model->setColumnCount(labels.count());
//    int row = 0;
//    QHash<QString,QColor> typeColor;
//    QColor emptyColor("#12a813");
//    foreach (QFileInfo fileInfo, delNames)
//    {
//        QString name = fileInfo.absoluteFilePath();
//        {
//            QModelIndex index = m_model->index(row,0);
//            m_model->setData(index,name);
//            QStandardItem* item = m_model->item(row,0);
//            item->setCheckable(true);
//            item->setCheckState(Qt::Checked);
//        }
//        {
//            QString suffix = fileInfo.completeSuffix();
//            QModelIndex index = m_model->index(row,1);
//            m_model->setData(index,suffix);
//            QStandardItem* item = m_model->item(row,1);
//            QColor color(emptyColor);
//            if(!suffix.isEmpty())
//            {
//                if(typeColor.contains(suffix))
//                {
//                    color = typeColor.value(suffix);
//                }
//                else
//                {
//                    color = QColor(qrand()%256,qrand()%256,qrand()%256);
//                    typeColor.insert(suffix,color);
//                }
//            }
//            QBrush brush(color);
//            item->setBackground(brush);
//        }
//        {
//            QModelIndex index = m_model->index(row,2);
//            m_model->setData(index,"未删除");
//        }
//       row++;
//    }

//    m_model->setSortRole(Qt::EditRole);
//    ui->tableView->setModel(m_model);
//    ui->tableView->setSortingEnabled(true);
//    ui->tableView->resizeColumnsToContents();
//}

//void MainWindow::on_pushButton_delete_clicked()
//{
//    DEBUG_CURRENT_LINE;
//    for(int row = 0; row < m_model->rowCount(); ++row)
//    {
//        if(m_model->item(row,0)->checkState()  == Qt::Checked)
//        {
//            QModelIndex index = m_model->index(row,0);
//            QString fileName = m_model->data(index).toString();
//            bool isOk = QFile::remove(fileName);
//            onAppendMsg(QString("删除文件<%1>%2").arg(fileName).arg(isOk ?"成功":"失败"));
//            {//修改状态
//                QModelIndex index = m_model->index(row,2);
//                m_model->setData(index,QString("删除<%1>").arg(isOk ?"成功":"失败"));
//                QColor color = isOk ? QColor(111,255,0) : QColor(255,111,0);
//                QBrush brush(color);
//                QStandardItem* item = m_model->item(row,2);
//                item->setBackground(brush);
//            }
//        }
//    }
//}

void MainWindow::resizeEvent(QResizeEvent *)
{
    DEBUG_CURRENT_LINE;
    QPixmap pixmap(m_bgImage);
    //判断图片是否为空
    if(pixmap.isNull()){
        qDebug() << tr("illege arguments") << "m_bgImage = "<<m_bgImage;
        return;
    }
    //设置窗口的背景
    QPalette    palette = this->palette();
    palette.setBrush(this->backgroundRole(),
                     QBrush(pixmap.scaled(this->size(),
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation)));
    this->setPalette(palette);
}
