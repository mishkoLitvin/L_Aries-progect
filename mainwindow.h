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
#include <QDate>

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

    int indexerCiclesSession;
    int indexerCiclesAll;
    int ragSessionCount;
    int ragAllCount;
    int ragAtHeadCount;
    bool autoPrintEnabled;

    QString userName;



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
    void getDirection(int direction);
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
    void setHeadsPosition();
    void timerTimeout();
    void startPrintProcess(bool autoPrint);
    void stopPrintProcess();
    void maintanceWorkSlot(bool enable);
    void setIconFolder(int index);



protected:
    virtual void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *ev);

};

#endif // MAINWINDOW_H
