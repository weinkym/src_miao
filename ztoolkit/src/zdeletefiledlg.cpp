#include <QStandardItemModel>
#include <QFileDialog>
#include <QSettings>
#include "zdeletefiledlg.h"
#include "zfiletool.h"

ZDeleteFileDlg::ZDeleteFileDlg(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    ui.setupUi(this);
    m_model = new QStandardItemModel(this);
    initSetting();
    m_typeMap.insert(COLUMN_TYPE_FILE_NAME,tr("name"));
    m_typeMap.insert(COLUMN_TYPE_FILE_TYPE,tr("type"));
    m_typeMap.insert(COLUMN_TYPE_FILE_STATUS,tr("status"));
    m_typeMap.insert(COLUMN_TYPE_FILE_PATH,tr("path"));
}

ZDeleteFileDlg::~ZDeleteFileDlg()
{
    QSettings settings("ZTOOLKIT", "ZDeleteFileDlg");
    settings.beginGroup("ZDeleteFileDlg");

    settings.setValue("lineEdit_path",ui.lineEdit_path->text());
    settings.setValue("lineEdit_del_prefix",ui.lineEdit_del_prefix->text());
    settings.setValue("lineEdit_del_subfix",ui.lineEdit_del_subfix->text());
    settings.endGroup();
}

void ZDeleteFileDlg::initSetting()
{
    QSettings settings("ZTOOLKIT", "ZDeleteFileDlg");
    settings.beginGroup("ZDeleteFileDlg");

    QString lineEdit_path = settings.value("lineEdit_path").toString();
    ui.lineEdit_path->setText(lineEdit_path);

    QString lineEdit_del_prefix = settings.value("lineEdit_del_prefix").toString();
    ui.lineEdit_del_prefix->setText(lineEdit_del_prefix);

    QString lineEdit_del_subfix = settings.value("lineEdit_del_subfix").toString();
    ui.lineEdit_del_subfix->setText(lineEdit_del_subfix);

    settings.endGroup();
}

void ZDeleteFileDlg::on_btn_update_clicked()
{
    DEBUG_CURRENT_LINE;
    ui.tableView->setModel(NULL);
    m_model->clear();
    QStringList surfix = ui.lineEdit_del_subfix->text().trimmed().split(";",QString::SkipEmptyParts);
    QStringList prefix = ui.lineEdit_del_prefix->text().trimmed().split(";",QString::SkipEmptyParts);
    QList<QFileInfo> allNames;
    ZFileTool::findFile(allNames,ui.lineEdit_path->text(),QStringList());
    qDebug()<<"surfix"<<surfix.join("-");
    qDebug()<<"prefix"<<prefix.join("-");
    QList<QFileInfo> delNames;
    foreach (QFileInfo fileInfo, allNames)
    {
        QString fileName = fileInfo.fileName();
        bool isFound =false;
        foreach (QString filter , surfix)
        {
            if( fileName.endsWith(filter,Qt::CaseInsensitive)/* || fileName.toUpper() == filter.toUpper()*/)
            {
                isFound = true;
                break;
            }
        }
        foreach (QString filter , prefix)
        {
            if( fileName.startsWith(filter,Qt::CaseInsensitive)/* || fileName.toUpper() == filter.toUpper()*/)
            {
                isFound = true;
                break;
            }
        }
        if(isFound)
        {
            delNames.append(fileInfo);
        }
    }
    QStringList labels;
    QMapIterator<COLUMN_TYPE,QString>iter(m_typeMap);
    while(iter.hasNext())
    {
        iter.next();
        labels.append(iter.value());
    }

    m_model->setHorizontalHeaderLabels(labels);
    m_model->setRowCount(delNames.count());
    m_model->setColumnCount(labels.count());
    int row = 0;
    QHash<QString,QColor> typeColor;
    QColor emptyColor("#12a813");
    foreach (QFileInfo fileInfo, delNames)
    {
        QString path = fileInfo.absoluteFilePath();
        QString name = fileInfo.fileName();
        {
            QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_NAME);
            m_model->setData(index,name);
            QStandardItem* item = m_model->item(row,COLUMN_TYPE_FILE_NAME);
            item->setCheckable(true);
            item->setCheckState(Qt::Checked);
        }
        {
            QString suffix = fileInfo.completeSuffix();
            QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_TYPE);
            m_model->setData(index,suffix);
            QStandardItem* item = m_model->item(row,COLUMN_TYPE_FILE_TYPE);
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
            QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_STATUS);
            //未删除
            m_model->setData(index,tr("undeleted"));
        }
        {
            QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_PATH);
            m_model->setData(index,path);
            QStandardItem* item = m_model->item(row,COLUMN_TYPE_FILE_PATH);
//            item->setCheckable(true);
//            item->setCheckState(Qt::Checked);
        }
       row++;
    }

    m_model->setSortRole(Qt::EditRole);
    ui.tableView->setModel(m_model);
    ui.tableView->setSortingEnabled(true);
    ui.tableView->resizeColumnsToContents();
}

void ZDeleteFileDlg::on_btn_path_clicked()
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

void ZDeleteFileDlg::on_btn_delete_clicked()
{
    DEBUG_CURRENT_LINE;
    for(int row = 0; row < m_model->rowCount(); ++row)
    {
        if(m_model->item(row,0)->checkState()  == Qt::Checked)
        {
            QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_PATH);
            QString fileName = m_model->data(index).toString();
            bool isOk = QFile::remove(fileName);
            qDebug()<<QString("删除文件<%1>%2").arg(fileName).arg(isOk ?"成功":"失败");
            {//修改状态
                QModelIndex index = m_model->index(row,COLUMN_TYPE_FILE_STATUS);
                m_model->setData(index,QString("删除<%1>").arg(isOk ?"成功":"失败"));
                QColor color = isOk ? QColor(56,120,0) : QColor(255,111,0);
                QBrush brush(color);
                QStandardItem* item = m_model->item(row,COLUMN_TYPE_FILE_STATUS);
                item->setBackground(brush);
            }
        }
    }
}
