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

    while ((eCode == ExitDialog::Continue)|((eCode == ExitDialog::RestartProgram)|(eCode ==ExitDialog::RestartMachine))) {
        MainWindow w;
        w.masterCodeCheck();
        w.userLogin();
        w.show();

        res = a.exec();
        eCode = w.exitCode;
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
//            proc.startDetached("chvt 7");
//        }
    }


//        LogoDialog lD;
//        lD.exec();

    return res;
}
