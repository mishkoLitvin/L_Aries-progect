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

signals:
    void accept(int index, QByteArray hParamArr);
    void changeNumber(int newIndex);
    void sendCommand(int index, QByteArray command);
    void setParamsToAll(int index, QByteArray hParamArr);


private:
    Ui::SettingDialog *ui;

    int index;
    bool acceptOnDeactilationEn;

private slots:
    void paramsAccepted();
    void pButtonIncClkd();
    void pButtonDecClkd();

    void eventFilterSetup();

    void on_toolButtonPlast_clicked();
    void on_toolButtonFL_clicked();
    void on_toolButtonMoveRear_clicked();
    void on_toolButtonFL_SQ_clicked();
    void on_toolButtonMoveFront_clicked();
    void on_toolButtonMTPMove_clicked();
    void on_toolButtonSQ_clicked();
    void on_toolButtonMoveTest_clicked();
    void on_toolButtonPressure_clicked();
    void on_toolButtonHoldOn_clicked();
    void on_pushButtonCopyToAll_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_spinBoxRearSpeed_valueChanged(double arg1);
    void on_dSpinBoxRearRange_valueChanged(double arg1);
    void on_spinBoxFrontSpeed_valueChanged(double arg1);
    void on_dSpinBoxFrontRange_valueChanged(double arg1);
    void on_spinBoxStrokCount_valueChanged(double arg1);
    void on_dSpinBoxHeatTime1Q_valueChanged(double arg1);
    void on_dSpinBoxHeatTime2Q_valueChanged(double arg1);
    void on_spinBoxDryPowerQ_valueChanged(double arg1);
    void on_dSpinBoxHeatTime1IR_valueChanged(double arg1);
    void on_dSpinBoxHeatTime2IR_valueChanged(double arg1);
    void on_dSpinBoxDryingRangeIR_valueChanged(double arg1);

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
};



#endif // SETTINGDIALOG_H
