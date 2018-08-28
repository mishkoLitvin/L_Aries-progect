#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "logodialog.h"

#include <QProcess>


int main(int argc, char *argv[])
{
    int res = 0;

    QApplication a(argc, argv);
    ExitDialog::ExitCode eCode = ExitDialog::Continue;
//    LogoDialog lD;
//    lD.exec();
    while ((eCode == ExitDialog::Continue)|((eCode == ExitDialog::RestartProgram)|(eCode ==ExitDialog::RestartMachine))) {
        MainWindow w;
        w.show();
        w.masterCodeCheck();
        w.userLogin();
        res = a.exec();
        eCode = w.exitCode;
//#ifdef DEBUG
//        if(eCode ==ExitDialog::Shutdown)
//        {
//            QProcess proc;
//            proc.startDetached("shutdown -P now");
//        }
//        if(eCode ==ExitDialog::RestartMachine)
//        {
//            QProcess proc;
//            proc.startDetached("reboot");
//        }
//        if(eCode ==ExitDialog::ExitFromProgram)
//        {
//            QProcess proc;
//            proc.startDetached("./chDisp");
//        }
    }


//        LogoDialog lD;
//        lD.exec();

    return res;
}
