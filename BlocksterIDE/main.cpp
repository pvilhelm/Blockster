#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    try{
        return a.exec();
    }
    catch(std::exception e){
        qDebug() << "Application chrashed: " << e.what();
        return -1;
    }

}
