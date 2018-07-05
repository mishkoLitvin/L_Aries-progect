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
#include <QProgressDialog>
#include <QTime>
#include <QTimer>
#include <QDate>
#include <QStringList>
#include <QTranslator>

#include "headform.h"
#include "headsettingdialog.h"
#include "indexerwidget.h"
#include "indexersettingdialog.h"
#include "generalsettingdialog.h"
#include "numpaddialog.h"
#include "keyboarddialog.h"
#include "serialport.h"
#include "mailsender.h"
#include "serialsettingsdialog.h"
#include "infowidget.h"
#include "usersetting.h"
#include "logindialog.h"
#include "maintancedialog.h"
#include "exitdialog.h"
#include "cyclesdialog.h"
#include "udpsocket.h"
#include "headactivationdialog.h"
#include "reprogramdialog.h"

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

    void masterCodeCheck();
    void userLogin();
    ExitDialog::ExitCode exitCode;

private:
    Ui::MainWindow *ui;

    QList<HeadForm*> headButton;
    QList<HeadSettingButton*> headSettButton;
    InfoWidget* infoWidget;
    QSettings *settings;
    QSettings *masterCodes;
    HeadSetting headSettings;
    IndexerLiftSettings indexerLiftSettings;
    MachineSettings machineSettings;
    GeneralSettingDialog *generalSettingDialog;
    SettingDialog *headSettingDialog;
    IndexerSettingDialog *indexerLiftSetDialog;
    SerialSettingsDialog *serialSettingsDialog;
    IndexerWidget *indexer;
    MailSender *mailSender;
    UserSettingDialog *usersSettingDialog;
    MaintanceDialog* maintanceDialog;
    CyclesDialog* cycleDialog;
    HeadActivationDialog *headActDialog;
    ReprogramDialog *reprogramDialog;

    SerialPort *comPort;
    UdpSocket *udpHandler;

    QTimer *timerMain;
    QTimer *watchDog;

    QTime timeProgramStart;
    QTime timeProgramEnd;
    QTime timeWorking;

    QString truePassword;
    bool logedInHeadSettings;
    bool logedInIndexer;
    bool logedInGeneral;
    bool logedInService;
    int headsCount;
    bool needCompleteReset;

    int indexerCyclesSession;
    int indexerCyclesAll;
    int ragSessionCount;
    int ragAllCount;
    uint32_t ragAllCountReg;
    int ragAtHeadCount;
    bool autoPrintEnabled;
    QString userName;
    Register *registers;

    uint8_t serviceCounter;

    QTranslator translator;


private slots:
    void zeroStart();
    void headsInit();
    void watchDogTimeout();
    void headSettingRequest(int index);
    void indexerLiftSettingRequest();
    void generalSettingDialogRequest();
    void changeHeadNo(int index);
    void resetMachine();
    void getSerialData(ModData modData);
    void getUdpData(QByteArray data);
    void getHeadParam(int index, QByteArray hParamArr);
    void getAllHeadParam(int index, QByteArray hParamArr);
    void getHeadCommand(int index, QByteArray commandArr);
    void getHeadActivCommand(QByteArray commandArr);
    void getCyclesCommand(QByteArray commandArr);
    void getLoadState(LoadState stase);
    void getIndexerParam(QByteArray indexerParamArr);
    void getMachineParam(QByteArray machineParamArr);
    void getStepDelayTime(double arg1);
    void getDirection(int direction);
    void setUnloadState(bool state);
    void getLiftParam(QByteArray liftParamArr);
    void getLiftOffcet(double arg1);
    void getIndexLiftSettComm(QByteArray commandArr);
    void getIndexLiftCommand(QByteArray commandArr);
    void getMachineCommand(QByteArray commandArr);
    void getSerialSetting(ComSettings comSett);
    void getEmailSettings(EmailSettings emailSett);
    void getVeiwSettings(int stSheetIndex);
    void getLangFile(int langIndex);
    void serviceStateChange();
    void exitProgram(bool restart = false);
    void saveJob();
    void loadJob();
    void setHeadsPosition();
    void indexerStepFinish();
    void startPrintProcess(bool autoPrint);
    void stopPrintProcess();
    void maintanceWorkSlot(bool enable);
    void setIconFolder(int index);



protected:
    virtual void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *ev);
    bool eventFilter(QObject *obj, QEvent *ev);
    void changeEvent(QEvent *event);
};

#endif // MAINWINDOW_H
