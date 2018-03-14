#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
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

#include "headform.h"
#include "headsettingdialog.h"
#include "indexerwidget.h"
#include "indexersettingdialog.h"
#include "generalsettingdialog.h"
#include "numpaddialog.h"
#include "serialport.h"
#include "mailsender.h"
#include "serialsettingsdialog.h"
#include "logodialog.h"

#include "settings.h"

#define HEAD_COUNT 12
//#define DEBUG_BUILD
//#define SHOW_LOGO

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

    HeadForm *headButton[HEAD_COUNT];
    HeadSettingButton *headSettButton[HEAD_COUNT];
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

    QString truePassword;
    bool logedInHeadSettings;
    bool logedInIndexer;
    bool logedInGeneral;
    bool logedInService;

    SerialPort *comPort;


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



protected:
    virtual void resizeEvent(QResizeEvent *e);

};

#endif // MAINWINDOW_H
