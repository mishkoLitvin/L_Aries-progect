#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QFocusEvent>
#include <QEvent>
#include <QApplicationStateChangeEvent>
#include <QDebug>
#include <QMessageBox>
#include <QByteArray>
#include <QLineEdit>

#include "settings.h"
#include "numpaddialog.h"

#include "crc16.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SettingDialog(HeadSetting hSttg, int index = 0, QWidget *parent = 0);
    ~SettingDialog();

    void setHeadParams(HeadSetting hSttg, int index = 0);
    void setIconFolder(QString path);


signals:
    void accept(int index, QByteArray hParamArr);
    void changeNumber(int newIndex);
    void sendCommand(int index, QByteArray command);
    void setParamsToAll(int index, QByteArray hParamArr);


private:
    Ui::SettingDialog *ui;

    int index;
    bool acceptOnDeactilationEn;
    bool acceptEnable;

private slots:
    void accept();
    void reject();
    void pButtonIncClkd();
    void pButtonDecClkd();

    void connectAll();
    void disconnectAll();

    void eventFilterSetup();

    void on_toolButtonPlast_clicked();
    void on_toolButtonFL_clicked();
    void on_toolButtonMoveRear_clicked();
    void on_toolButtonFL_SQup_clicked();
    void on_toolButtonMoveFront_clicked();
    void on_toolButtonMTPMove_clicked();
    void on_toolButtonSQ_clicked();
    void on_toolButtonMoveTest_clicked();
    void on_toolButtonPressure_clicked();
    void on_toolButtonHoldOn_clicked();
    void on_pushButtonCopyToAll_clicked();
    void on_toolButtonFL_SQ_clicked(bool checked);
    void on_toolButtonStepBack_clicked();
    void on_toolButtonIndexHere_clicked();
    void on_toolButtonInkColor_clicked();
    void on_toolButtonPressureAir_clicked();

    void tabWidget_currentChanged(int index);
    void spinBoxRearSpeed_valueChanged(double arg1);
    void dSpinBoxRearRange_valueChanged(double arg1);
    void spinBoxFrontSpeed_valueChanged(double arg1);
    void dSpinBoxFrontRange_valueChanged(double arg1);
    void spinBoxStrokCount_valueChanged(double arg1);
    void spinBoxSBStrokCount_valueChanged(double arg1);
    void dSpinBoxHeatTime1Q_valueChanged(double arg1);
    void dSpinBoxHeatTime2Q_valueChanged(double arg1);
    void spinBoxDryPowerQ_valueChanged(double arg1);
    void dSpinBoxHeatTime1IR_valueChanged(double arg1);
    void dSpinBoxHeatTime2IR_valueChanged(double arg1);
    void dSpinBoxDryingRangeIR_valueChanged(double arg1);
    void dSpinBoxFlDwellTime_valueChanged(double arg1);
    void dSpinBoxSqDwellTime_valueChanged(double arg1);



protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *ev);


};



#endif // SETTINGDIALOG_H
