#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "logodialog.h"

int main(int argc, char *argv[])
{
    int res = 0;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //    LogoDialog lD;
    //    lD.exec();
    res = a.exec();

    return res;
}
