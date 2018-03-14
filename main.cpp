#include "mainwindow.h"
#include <QApplication>
#include "logodialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    LogoDialog lD;
//    lD.exec();

    w.show();

    return a.exec();
}
