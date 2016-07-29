#include <QStandardItemModel>
#include <QFileDialog>
#include <QSettings>
#include "zrenamefiledlg.h"
#include "zfiletool.h"

ZRenameFileDlg::ZRenameFileDlg(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    ui.setupUi(this);
    m_model = new QStandardItemModel(this);
    initSetting();
    // "文件"<<"修改后"<<"类型"<<"状态"<<"路径"
    m_labels<<tr("file")<<tr("modified")<<tr("type")<<tr("status")<<tr("patch");//与enum对齐
}

ZRenameFileDlg::~ZRenameFileDlg()
{
    QSettings settings("ZTOOLKIT", "ZRenameFileDlg");
    settings.beginGroup("ZRenameFileDlg");
    settings.setValue("lineEdit_type",ui.lineEdit_type->text());
    settings.setValue("lineEdit_path",ui.lineEdit_path->text());
    settings.setValue("lineEdit_del",ui.lineEdit_del->text());
    settings.endGroup();
}

void ZRenameFileDlg::initSetting()
{
    QSettings settings("ZTOOLKIT", "ZRenameFileDlg");
    settings.beginGroup("ZRenameFileDlg");
    QString lineEdit_type = settings.value("lineEdit_type").toString();
    ui.lineEdit_type->setText(lineEdit_type);

    QString lineEdit_path = settings.value("lineEdit_path").toString();
    ui.lineEdit_path->setText(lineEdit_path);

    QString lineEdit_del = settings.value("lineEdit_del").toString();
    ui.lineEdit_del->setText(lineEdit_del);
    settings.endGroup();
}

QString ZRenameFileDlg::rename(const QStringList &delList, const QString &fileName)
{
    QString newName = fileName;
    foreach (QString del, delList)
    {
        newName.remove(del,Qt::CaseInsensitive);
    }
   return newName;
}

void ZRenameFileDlg::on_btn_update_clicked()
{
    DEBUG_CURRENT_LINE;
    ui.tableView->setModel(NULL);
    m_model->clear();
    QStringList types = ui.lineEdit_type->text().trimmed().split(";",QString::SkipEmptyParts);
    QList<QFileInfo> allNames;
    ZFileTool::findFile(allNames,ui.lineEdit_path->text(),QStringList());
    qDebug()<<types.join("-");
    QList<QFileInfo> delNames;
    foreach (QFileInfo fileInfo, allNames)
    {
        bool isFound = types.isEmpty();
        foreach (QString filter , types)
        {
            if(fileInfo.suffix().toUpper() == filter.toUpper())
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

    m_model->setHorizontalHeaderLabels(m_labels);
    m_model->setRowCount(delNames.count());
    m_model->setColumnCount(m_labels.count());
    int row = 0;
    QHash<QString,QColor> typeColor;
    QColor emptyColor("#12a813");
    QStringList delList = ui.lineEdit_del->text().split(";",QString::SkipEmptyParts);
    qsrand(33);
    foreach (QFileInfo fileInfo, delNames)
    {
        QString fileName = fileInfo.fileName();
        QString newName = rename(delList,fileName);
        bool isSame = (fileName.toUpper() == newName.toUpper());
        {//文件
            int col = E_NAME;
            QModelIndex index = m_model->index(row,col);
            m_model->setData(index,fileName);
            QStandardItem* item = m_model->item(row,col);
            item->setCheckable(true);
            item->setCheckState(isSame ? Qt::Unchecked : Qt::Checked);
            item->setEditable(false);
        }
        {//修改后
            int col = E_NEWNAME;
            QModelIndex index = m_model->index(row,col);
            m_model->setData(index,newName);
            QStandardItem* item = m_model->item(row,col);
            item->setEditable(false);
            if(isSame)
            {
                item->setBackground(QColor("#6E8B3D"));
            }
            else
            {
                item->setBackground(QColor("#8B5A00"));
            }
        }
        {//类型
            int col = E_TYPE;
//            QString suffix = fileInfo.completeSuffix();
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
        {//状态
            int col = E_STATE;
            QModelIndex index = m_model->index(row,col);
            // "无需修改" : "待修改"
            m_model->setData(index,isSame ? tr("unneed modified") : tr("wait modified"));
            if(!isSame)
            {
                QStandardItem* item = m_model->item(row,col);
                item->setBackground(QColor("#8B5A00"));
            }
        }
        {//路径
            int col = E_PATH;
            QModelIndex index = m_model->index(row,col);
            m_model->setData(index,fileInfo.path());
        }
       row++;
    }
    m_model->setSortRole(Qt::EditRole);
    ui.tableView->setModel(m_model);
    ui.tableView->setSortingEnabled(true);
    ui.tableView->resizeColumnsToContents();
}

void ZRenameFileDlg::on_btn_path_clicked()
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

void ZRenameFileDlg::on_btn_rename_clicked()
{
    DEBUG_CURRENT_LINE;
    for(int row = 0; row < m_model->rowCount(); ++row)
    {
        if(m_model->item(row,0)->checkState()  == Qt::Checked)
        {
            QModelIndex index_name = m_model->index(row,E_NAME);
            QModelIndex index_newname = m_model->index(row,E_NEWNAME);
            QString fileName = m_model->data(index_name).toString();
            QString newName = m_model->data(index_newname).toString();
            if(fileName.toUpper() == newName.toUpper())
            {
                continue;
            }
            QModelIndex index_path = m_model->index(row,E_PATH);
            QString path = m_model->data(index_path).toString();
            QFileInfo fileInfo(path,fileName);
            QFileInfo newFileInfo(path,newName);
            QString oldFp = QDir::toNativeSeparators(fileInfo.absoluteFilePath());
            QString newFp = QDir::toNativeSeparators(newFileInfo.absoluteFilePath());
            qDebug()<<"oldFp"<<oldFp;
            qDebug()<<"newFp"<<newFp;
            bool isOk = QFile::rename(oldFp,newFp);
            qDebug()<<QString("修改文件<%1>%2").arg(fileName).arg(isOk ?"成功":"失败");
            {//修改状态
                QModelIndex index = m_model->index(row,E_STATE);
                m_model->setData(index,QString("修改<%1>").arg(isOk ?"成功":"失败"));
                QColor color = isOk ? QColor(111,255,0) : QColor(255,111,0);
                QBrush brush(color);
                QStandardItem* item = m_model->item(row,E_STATE);
                item->setBackground(brush);
            }
        }
    }
}
