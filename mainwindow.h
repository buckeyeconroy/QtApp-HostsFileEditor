#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QLabel>

namespace Ui 
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    bool regexMatch(const QString &pattern, const QString &source);
    QStringList regexCapture(const QString &pattern, const QString &source);
    void setTreeWidget();
    void setTreeItems(const QStringList &lines);
    QStringList getHostContent();
    void clearTreeItems();
    QTreeWidgetItem *addTreeItem(QStringList &hostValue, QTreeWidgetItem *parent = Q_NULLPTR);
    QTreeWidgetItem* insertTreeItem(int position, QStringList &hostValue, QTreeWidgetItem *parent = Q_NULLPTR);
    void openFile();
    bool ipAddrRefresh();
private slots:
    void on_actionExit_triggered();
    
    void on_actionDelete_triggered();
    
    void on_actionInsertGroup_triggered();
    
    void on_actionInsertItem_triggered();
    
    void on_actionSave_triggered();
    
    void on_actionRefresh_triggered();
    
    void on_actionNew_triggered();
    
    void on_actionAbout_triggered();
    
private:
    Ui::MainWindow *ui;
    bool is_my_file;
    QLabel *lblStatus;
};

#endif // MAINWINDOW_H
