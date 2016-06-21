#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QTranslator translator;
    translator.load(QString(":/statics/ts/qt_zh_CN.qm"));
    a.installTranslator(&translator);
    
    QFile file(":/statics/qss/green.qss");
    if(file.open(QFile::ReadOnly))
    {
        a.setStyleSheet(QLatin1String(file.readAll()));
    }
    file.close();
   
    MainWindow w;
    w.show();
    
    return a.exec();
}
