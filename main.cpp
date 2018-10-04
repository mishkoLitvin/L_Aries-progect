#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "logodialog.h"

#include <QtDebug>
#include <QFile>
#include <QTextStream>

#include <QProcess>

void crashMessageOutput(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(str);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(str);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(str);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(str);
        abort();
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    int res = 0;

    QApplication a(argc, argv);
//    qInstallMessageHandler(crashMessageOutput);
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
