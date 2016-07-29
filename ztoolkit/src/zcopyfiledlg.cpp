#include <QStandardItemModel>
#include <QFileDialog>
#include <QSettings>
#include "ZCopyFileDlg.h"
#include "zfiletool.h"
#include "mplib.h"

ZCopyFileDlg::ZCopyFileDlg(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    ui.setupUi(this);
    m_model = new QStandardItemModel(this);
    initSetting();
    //"文件"<<"路径"<<"复制路径"<<"类型"<<"状态"<<"大小"
    m_labels<<tr("file")<<tr("path")<<tr("copy path")<<tr("type")<<tr("status")<<tr("size");//与enum对齐
}

ZCopyFileDlg::~ZCopyFileDlg()
{
    QSettings settings("ZTOOLKIT", "ZCopyFileDlg");
    settings.beginGroup("ZCopyFileDlg");

    settings.setValue("lineEdit_path",ui.lineEdit_path->text());
    settings.setValue("lineEdit_file",ui.lineEdit_file->text());
    settings.setValue("lineEdit_except",ui.lineEdit_except->text());
    settings.endGroup();
}

void ZCopyFileDlg::initSetting()
{
    QSettings settings("ZTOOLKIT", "ZCopyFileDlg");
    settings.beginGroup("ZCopyFileDlg");

    QString lineEdit_path = settings.value("lineEdit_path").toString();
    ui.lineEdit_path->setText(lineEdit_path);

    QString lineEdit_file = settings.value("lineEdit_file").toString();
    ui.lineEdit_file->setText(lineEdit_file);

    QString lineEdit_except = settings.value("lineEdit_except").toString();
    ui.lineEdit_except->setText(lineEdit_except);

    settings.endGroup();
}

QMap<QString, QStringList> ZCopyFileDlg::getCopyFileInfo(int &count)
{
    QMap<QString, QStringList> result;
    QStringList fileNames = ui.lineEdit_file->text().trimmed().split(";",QString::SkipEmptyParts);
    QStringList exceptDirs = ui.lineEdit_except->text().trimmed().split(";",QString::SkipEmptyParts);
    foreach (QString name, fileNames)
    {
        QFile file(name);
        QStringList fileList;
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream doc(&file);
            while(!doc.atEnd())
            {
                QString fileName = doc.readLine();
                if(foundExistDir(exceptDirs,fileName))
                {
                    continue;
                }
                fileList.append(fileName);
                count++;
            }
        }
        else
        {
            qDebug()<<(QString("打开文件<%1>失败").arg(ui.lineEdit_file->text()));
        }
        if(fileList.count() > 0)
        {
            result.insert(QFileInfo(name).baseName(),fileList);
        }
    }
    return result;
}

bool ZCopyFileDlg::foundExistDir(const QStringList &dirs, const QString filePath)
{
    foreach (QString dir, dirs)
    {
        if(filePath.contains(dir,Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

void ZCopyFileDlg::on_btn_update_clicked()
{
    DEBUG_CURRENT_LINE;
    int rows = 0;
    QMap<QString, QStringList> result = getCopyFileInfo(rows);
    ui.tableView->setModel(NULL);
    m_model->clear();
    m_model->setHorizontalHeaderLabels(m_labels);
    m_model->setRowCount(rows);
    m_model->setColumnCount(m_labels.count());
    int row = 0;

    QHash<QString,QColor> typeColor;
    QColor emptyColor("#12a813");
    QString path = ui.lineEdit_path->text();

    QMapIterator<QString,QStringList> iter(result);
    while(iter.hasNext())
    {
        iter.next();
        QDir toDir(path);
        toDir.mkdir(iter.key());
        toDir.cd(iter.key());
        foreach (QString name, iter.value())
        {
//            enum{E_NAME = 0,E_FROM_PATH,E_TO_PATH,E_TYPE,E_STATE,E_SIZE};
            QFileInfo fileInfo(name);
            {
                int col = E_NAME;
                QModelIndex index = m_model->index(row,col);
                m_model->setData(index,fileInfo.fileName());
                QStandardItem* item = m_model->item(row,col);
                item->setCheckable(true);
                item->setCheckState(Qt::Checked);
            }
            {
                int col = E_FROM_PATH;
                QModelIndex index = m_model->index(row,col);
                m_model->setData(index,fileInfo.absolutePath());
            }
            {
                int col = E_TO_PATH;
                QModelIndex index = m_model->index(row,col);
                m_model->setData(index,toDir.path());
            }
            {
                int col = E_TYPE;
                QString suffix = fileInfo.suffix();
                QModelIndex index = m_model->index(row,col);
                m_model->setData(index,suffix);
                QStandardItem* item = m_model->item(row,col);
                QColor color(emptyColor);
                if(!suffix.isEmpty())
                {
                    if(typeColor.contains(suffix))
                    {
                        color = typeColor.value(suffix);
                    }
                    else
                    {
                        color = QColor(qrand()%256,qrand()%256,qrand()%256);
                        typeColor.insert(suffix,color);
                    }
                }
                QBrush brush(color);
                item->setBackground(brush);
            }
            {
                int col = E_STATE;
                QModelIndex index = m_model->index(row,col);
                //待复制
                m_model->setData(index,tr("wait copy"));
            }
            {
                int col = E_SIZE;
                QModelIndex index = m_model->index(row,col);
                m_model->setData(index,Mplib::MpStaticMethod::getSizeString(fileInfo.size(),3));
            }
           row++;
        }
    }

    m_model->setSortRole(Qt::EditRole);
    ui.tableView->setModel(m_model);
    ui.tableView->setSortingEnabled(true);
    ui.tableView->resizeColumnsToContents();
}

void ZCopyFileDlg::on_btn_path_clicked()
{
    DEBUG_CURRENT_LINE;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ui.lineEdit_path->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui.lineEdit_path->setText(dir);
    }
}

void ZCopyFileDlg::on_btn_copy_clicked()
{
    DEBUG_CURRENT_LINE;
    for(int row = 0; row < m_model->rowCount(); ++row)
    {
        if(m_model->item(row,E_NAME)->checkState()  == Qt::Checked)
        {
            QModelIndex indexName = m_model->index(row,E_NAME);
            QString name = m_model->data(indexName).toString();

            QModelIndex indexFromPath = m_model->index(row,E_FROM_PATH);
            QString fromPath = m_model->data(indexFromPath).toString();

            QModelIndex indexToPath = m_model->index(row,E_TO_PATH);
            QString toPath = m_model->data(indexToPath).toString();

            QFileInfo fInfo(fromPath,name);
            QFileInfo tInfo(toPath,name);
            QString fromName = fInfo.absoluteFilePath();
            QString toName = tInfo.absoluteFilePath();

            bool isOk = ZFileTool::copyFile(fromName,toName);
            qDebug()<<QString("复制文件<%1>到<%2>%3").arg(fromName).arg(toName).arg(isOk ?"成功":"失败");
            {//修改状态
                QModelIndex index = m_model->index(row,E_STATE);
                m_model->setData(index,QString("复制<%1>").arg(isOk ?"成功":"失败"));
                QColor color = isOk ? QColor(111,255,0).dark() : QColor(255,111,0);
                QBrush brush(color);
                QStandardItem* item = m_model->item(row,E_STATE);
                item->setBackground(brush);
            }
        }
    }
    ui.tableView->resizeColumnsToContents();
}

void ZCopyFileDlg::on_btn_file_clicked()
{
    DEBUG_CURRENT_LINE;
    QStringList files = QFileDialog::getOpenFileNames(
                             this,
                             "选择文件",
                             ui.lineEdit_path->text(),
                             "所有 (*.*)");
    if(!files.isEmpty())
    {
        ui.lineEdit_file->setText(files.join(";"));
    }
}
