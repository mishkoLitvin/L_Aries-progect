#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QSettings>
#include <QPalette>
#include <QGradient>
#include <QStyle>
#include <QInputDialog>
#include <QByteArray>
#include <QFileDialog>
#include <QEvent>
#include <QMessageBox>
#include <QTime>
#include <QTimer>

#include "headform.h"
#include "headsettingdialog.h"
#include "indexerwidget.h"
#include "indexersettingdialog.h"
#include "generalsettingdialog.h"
#include "numpaddialog.h"
#include "serialport.h"
#include "mailsender.h"
#include "serialsettingsdialog.h"

#include "settings.h"

//#define DEBUG_BUILD

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QList<HeadForm*> headButton;
    QList<HeadSettingButton*> headSettButton;
    QSettings *settings;
    HeadSetting headSettings;
    IndexerLiftSettings indexerLiftSettings;
    MachineSettings machineSettings;
    GeneralSettingDialog *generalSettingDialog;
    SettingDialog *headSettingDialog;
    IndexerSettingDialog *indexerLiftSetDialog;
    SerialSettingsDialog *serialSettingsDialog;
    IndexerWidget *indexer;
    MailSender *mailSender;

    SerialPort *comPort;
    QTimer *timerMain;

    QTime timeProgramStart;
    QTime timeProgramEnd;
    QTime timeWorking;

    QString truePassword;
    bool logedInHeadSettings;
    bool logedInIndexer;
    bool logedInGeneral;
    bool logedInService;
    int headsCount;

    int ragSessionCount;
    int ragAllCount;



private slots:
    void headSettingRequest(int index);
    void indexerLiftSettingRequest();
    void generalSettingDialogRequest();
    void changeHeadNo(int index);
    void getHeadParam(int index, QByteArray hParamArr);
    void getAllHeadParam(int index, QByteArray hParamArr);
    void getHeadCommand(int index, QByteArray commandArr);
    void getIndexerParam(QByteArray indexerParamArr);
    void getMachineParam(QByteArray machineParamArr);
    void getLiftParam(QByteArray liftParamArr);
    void getIndexLiftSettComm(QByteArray commandArr);
    void getIndexLiftCommand(QByteArray commandArr);
    void getSerialSetting(ComSettings comSett);
    void getEmailSettings(EmailSettings emailSett);
    void getVeiwSettings(int stSheetIndex);
    void serviceStateChange();
    void exitProgram();
    void saveJob();
    void loadJob();
    void setButtonPoss();
    void timerTimeout();
    void startPrintProcess(bool autoPrint);
    void stopPrintProcess();



protected:
    virtual void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *ev);

};

#endif // MAINWINDOW_H
