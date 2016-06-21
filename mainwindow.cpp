#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipaddrdelegate.h"
#include "domaindelegate.h"
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QProgressDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lblStatus = new QLabel(this);
    ui->statusBar->addPermanentWidget(lblStatus);
    //
    ui->treeWidget->setHeaderLabels(QStringList() << "状态" << tr("IP地址") << tr("域名") << tr("备注"));
    ui->treeWidget->setItemDelegateForColumn(1, new IpAddrDelegate(ui->treeWidget));
    ui->treeWidget->setItemDelegateForColumn(2, new DomainDelegate(ui->treeWidget));
    setTreeWidget();
    
    openFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

/**
 * @brief 正则验证字符串
 * @param pattern 正则表达式
 * @param source 字符串
 * @return 结果
 */
bool MainWindow::regexMatch(const QString& pattern, const QString& source)
{
    QRegExp regex(pattern);
    if(regex.exactMatch(source))
        return true;
    return false;
}
/**
 * @brief 正则抓取内容
 * @param pattern
 * @param source
 * @return 
 */
QStringList MainWindow::regexCapture(const QString &pattern, const QString &source)
{
    QStringList result;
    QRegExp regex(pattern);
    int pos = regex.indexIn(source);
    if(pos >= 0)
    {
        for(int i = 0; i <= regex.captureCount(); i++)
        {
            result << regex.cap(i).trimmed();
        }
    }
    return result;
}

void MainWindow::setTreeWidget()
{
    //清空状态
    ui->treeWidget->setCurrentItem(Q_NULLPTR);
    //默认展开所有
    ui->treeWidget->expandAll();
    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
    {
        //ui->treeWidget->resizeColumnToContents(i);
        if(i == 0)
            ui->treeWidget->setColumnWidth(i, 100);
        else
        {
            ui->treeWidget->resizeColumnToContents(i);
        }
    }
    int count = 0;
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) 
    {
        QTreeWidgetItem *itemTop = ui->treeWidget->topLevelItem(i);
        count += itemTop->childCount();
    }
    lblStatus->setText(QString::number(count) + tr(" 条记录"));
}

void MainWindow::setTreeItems(const QStringList& lines)
{
    QProgressDialog progress(this);
    progress.setWindowModality(Qt::WindowModal);  
    progress.setWindowTitle(tr("加载文件"));  
    progress.setLabelText(tr("解析并生成数据结构..."));  
    progress.setCancelButton(Q_NULLPTR);
    progress.setFixedSize(150,70);
    progress.setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint);
    progress.setMinimum(0);  
    progress.setMaximum(lines.count()); 
    //行号
    int number = 0;
    QTreeWidgetItem *itemParent = Q_NULLPTR;
    
    while(number < lines.count())
    {
        //当前行内容
        QString line = lines.at(number);
        
        if(!line.isEmpty())
        {
            if(is_my_file)//内置格式区分处理
            {
                bool temp = regexMatch("^#*\\s*\\d+.\\d+.\\d+.\\d+.*$", line); //ipv4
                bool tempRemark = regexMatch("^#+.*", line);
                if(number >= 2)
                {
                    if(temp)
                    {
                        if(itemParent == 0)
                             itemParent = addTreeItem(QStringList() << "未分组");
                        QStringList result = regexCapture("^(#*)\\s*(\\d+.\\d+.\\d+.\\d+)\\s+(\\S+)\\s+(.*)", line);
                        QTreeWidgetItem* item = addTreeItem(QStringList() << "" << result.at(2) << result.at(3) << result.takeAt(4).replace("#", ""), itemParent);
                        //是否是注释的项目
                        item->setCheckState(0, (result.at(1) == "#") ? Qt::Unchecked : Qt::Checked);
                    }
                    if(tempRemark && !temp)
                    {
                        QStringList result = regexCapture("^#+([^#]*)", line);
                        QString groupName = result.at(1);

                        itemParent = addTreeItem(QStringList() << groupName);
                    }
                }
            }
            else
            {
                bool temp = regexMatch("^#*\\s*\\d+.\\d+.\\d+.\\d+.*$", line); //ipv4
                if(temp)
                {
                    if(!itemParent)
                         itemParent = addTreeItem(QStringList() << "未分组");
                    QStringList result = regexCapture("^(#*)\\s*(\\d+.\\d+.\\d+.\\d+)\\s+(\\S+)\\s+(.*)", line);
                    QTreeWidgetItem* item = addTreeItem(QStringList() << "" << result.at(2) << result.at(3) << result.takeAt(4).replace("#", ""), itemParent);
                    //是否是注释的项目
                    item->setCheckState(0, (result.at(1) == "#") ? Qt::Unchecked : Qt::Checked);
                }
            }
            
        }
        
        number ++;
    }
    progress.wasCanceled();
}
/**
 * @brief 将树转换为文本
 * @return 
 */
QStringList MainWindow::getHostContent()
{
    QStringList result;
    result << "# hosts file editor - v1.0.0 (by 叶流痕 - Buckeye.Conroy, buckeye@live.cn)";
    result << "# last saved: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    
    for(int i =0; i < ui->treeWidget->topLevelItemCount(); i++) 
    {
        result << "# " + ui->treeWidget->topLevelItem(i)->text(0).trimmed();
        for(int j = 0; j < ui->treeWidget->topLevelItem(i)->childCount(); j++)
        {
            QTreeWidgetItem *child = ui->treeWidget->topLevelItem(i)->child(j);
            QString remark = child->checkState(0) == Qt::Checked ? "  " : "# ";
            result << remark + child->text(1).trimmed() + " " + child->text(2).trimmed() + " # " + child->text(3).trimmed();
        }
    }
    return result;
}

void MainWindow::clearTreeItems()
{
    //首先清空以往数据
    if(ui->treeWidget->topLevelItemCount() > 0)
        ui->treeWidget->clear();
    ui->treeWidget->setCurrentItem(Q_NULLPTR);
}

QTreeWidgetItem *MainWindow::addTreeItem(QStringList& hostValue, QTreeWidgetItem* parent)
{
    if(parent)
    {
        QTreeWidgetItem *itemChild = new QTreeWidgetItem(hostValue);
        itemChild->setFlags(itemChild->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        parent->addChild(itemChild);
        return itemChild;
    }
    else
    {
        QTreeWidgetItem *itemParent = new QTreeWidgetItem(hostValue);
        itemParent->setFlags(itemParent->flags() | Qt::ItemIsEditable);
        ui->treeWidget->addTopLevelItem(itemParent);
        return itemParent;
    }
}

QTreeWidgetItem*MainWindow::insertTreeItem(int position, QStringList& hostValue, QTreeWidgetItem* parent)
{
    if(position < 0 || ( parent && position > parent->childCount()))
        return Q_NULLPTR;
    
    if(parent)
    {
        QTreeWidgetItem *itemChild = new QTreeWidgetItem(hostValue);
        itemChild->setFlags(itemChild->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        parent->insertChild(position, itemChild);
        return itemChild;
    }
    else
    {
        QTreeWidgetItem *itemParent = new QTreeWidgetItem(hostValue);
        itemParent->setFlags(itemParent->flags() | Qt::ItemIsEditable);
        ui->treeWidget->insertTopLevelItem(position, itemParent);
        return itemParent;
    }
}

void MainWindow::openFile()
{
    QString strWindows = QProcessEnvironment::systemEnvironment().value("windir");
    QFile file(strWindows + "\\System32\\drivers\\etc\\hosts");
    if(file.open(QFile::ReadOnly))
    {
        QString fileContent = QString::fromLocal8Bit(file.readAll());
        //开头检测文件格式
        if(regexMatch("# hosts file editor - v.* 叶流痕.*buckeye@live.cn.*",fileContent))
            is_my_file = true;
        else
            is_my_file = false;
        clearTreeItems();
        setTreeItems(fileContent.split(QString("\n")));
        setTreeWidget();
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("未找到 hosts 文件"), QMessageBox::Yes, QMessageBox::Yes);
    }
    
    file.close();
}

bool MainWindow::ipAddrRefresh()
{
    QProcess p(0);
    p.start("cmd", QStringList()<<"/c"<<"ipconfig /flushdns");
    p.waitForStarted();
    p.waitForFinished();
    qDebug()<<QString::fromLocal8Bit(p.readAllStandardOutput());
    return true;
}
/**
 * @brief 删除按钮
 */
void MainWindow::on_actionDelete_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("警告"), tr("确定要删除选中记录么？"));
    if(reply == QMessageBox::Yes)
    {
        QTreeWidgetItem* item = ui->treeWidget->currentItem();
        if(item)
        {
            delete item;
        }
        setTreeWidget();
    }
}
/**
 * @brief 添加组按钮
 */
void MainWindow::on_actionInsertGroup_triggered()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item)
    {
        insertTreeItem(ui->treeWidget->currentIndex().row(), QStringList()<<"未分组");
    }
    else
    {
        addTreeItem(QStringList() << "未分组");
    }
    setTreeWidget();
}
/**
 * @brief 添加记录按钮
 */
void MainWindow::on_actionInsertItem_triggered()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item)
    {
        QTreeWidgetItem* result;
        if(item->parent())
        {
            result = insertTreeItem(ui->treeWidget->currentIndex().row(), QStringList() << "" << "127.0.0.1" << "localhost" << "", item->parent());
        }
        else
        {
            result = addTreeItem(QStringList() << "" << "127.0.0.1" << "localhost" << "", item);
        }
        result->setCheckState(0, Qt::Unchecked);
    }
    setTreeWidget();
}

/**
 * @brief 保存按钮
 */
void MainWindow::on_actionSave_triggered()
{
    QString strWindows = QProcessEnvironment::systemEnvironment().value("windir");
    QFile file(strWindows + "\\System32\\drivers\\etc\\hosts");
    if(file.open(QFile::WriteOnly))
    {
        QStringList result = getHostContent();
        QTextStream in(&file);
        foreach (QString line, result) 
        {
            in << line << "\r\n";
        }
    }
    else
    {
        
    }
    
    
    file.close();
    QFile::remove(strWindows + "\\System32\\drivers\\etc\\hosts.bak");
    QFile::copy(strWindows + "\\System32\\drivers\\etc\\hosts", strWindows + "\\System32\\drivers\\etc\\hosts.bak");
    ipAddrRefresh();
}

void MainWindow::on_actionRefresh_triggered()
{
    ipAddrRefresh();
}

void MainWindow::on_actionNew_triggered()
{
    clearTreeItems();
    setTreeWidget();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("关于"), tr("Host File Editor - v1.0.0\r\n"
                                          "Author: Buckeye Conroy(叶流痕)\r\n"
                                          "Email: buckeye@live.cn\r\n"
                                          "Description:\r\n"
                                          "1. 本程序为了学习体验，未经同意不得转载或用作商业用途。\r\n"
                                          "2. 本程序会操作系统文件，请确保拥有管理员权限 \r\n"));
}
