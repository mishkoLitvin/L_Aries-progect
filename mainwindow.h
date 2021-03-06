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
#include "warmingwidget.h"
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
#include "countersdialog.h"

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

    QWidget* lastOpened;

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
    WarmingWidget *warming;
    MailSender *mailSender;
    UserSettingDialog *usersSettingDialog;
    MaintanceDialog* maintanceDialog;
    MaintanceWidget* maintanceWidget;
    CyclesDialog* cycleDialog;
    HeadActivationDialog *headActDialog;
    ReprogramDialog *reprogramDialog;
    CountersDialog *counterDialog;

    SerialPort *comPort;
    UdpSocket *udpHandler;

    QTimer *watchDog;
    QTimer *updateTime;

    QTime timeProgramStart;
    QTime timeProgramEnd;
    QTime timeWorking;

    QString truePassword;
    bool logedInHeadSettings;
    bool logedInIndexer;
    bool logedInGeneral;
    bool logedInService;
    uint16_t headsCount;
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
    QTranslator translator;

    QString pathIcon;

    QStringList headStylesStr;

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
    void getHeadColor(int index, QColor color);
    void getHeadParam(int index, QByteArray hParamArr);
    void getAllHeadParam(int index, QByteArray hParamArr);
    void getHeadCommand(int index, QByteArray commandArr);
    void getHeadActivCommand(QByteArray commandArr);
    void getHeadActiveState(int index, bool state);
    void getCyclesCommand(QByteArray commandArr);
    void getLoadState(int index, LoadState state);
    void getIndexerParam(QByteArray indexerParamArr);
    void getMachineParam(QByteArray machineParamArr);
    void getStepDelayTime(double arg1);
    void getDirection(int direction);
    void setUnloadState(bool state);
    void getLiftParam(QByteArray liftParamArr);
    void getLiftOffcet(double arg1);
    void getLiftDistance(float distance);
    void getIndexLiftSettComm(QByteArray commandArr);
    void getIndexLiftCommand(QByteArray commandArr);
    void getMachineCommand(QByteArray commandArr);
    void getWarmingCommand(QByteArray commandArr);
    void getWarmingEnable(bool enable);
    void getSerialSetting(ComSettings comSett);
    void getEmailSettings(EmailSettings emailSett);
    void getVeiwSettings(int stSheetIndex);
    void getLangFile(int langIndex);
    void serviceStateChange();
    void warmingStateChange(bool state);
    void exitProgram(bool restart = false);
    void saveJob();
    void loadJob();
    void setHeadsPosition();
    void indexerStepFinish();
    void startPrintProcess(bool autoPrint);
    void stopPrintProcess();
    void maintanceWorkSlot(bool enable);
    void showMaintananceWidget();
    void setIconFolder(int index);
    void setBackGround(bool enable, bool request = false);
    void updateTimeSlot();
    void resetSkippedRequest();
    void resetRemainingRequest();
    void remainingValChangedRequest(int val);
    void counterDialogRequest();
    void skipStateUpdate();

    void on_pButtonWarming_clicked();

protected:
    virtual void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *ev);
    bool eventFilter(QObject *obj, QEvent *ev);
    void changeEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MAINWINDOW_H
