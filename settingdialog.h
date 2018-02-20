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

#include "headsetting.h"
#include "numpaddialog.h"

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

private slots:
    void accepted();
    void pButtonIncClkd();
    void pButtonDecClkd();

    void eventFilterSetup();

    void on_pushButtonPlast_clicked();
    void on_pushButtonFL_clicked();
    void on_pushButtonMoveRear_clicked();
    void on_pushButtonFL_SQ_clicked();
    void on_pushButtonMoveFront_clicked();
    void on_pushButtonMTPMove_clicked();
    void on_pushButtonSQ_clicked();
    void on_pushButtonMoveTest_clicked();
    void on_pushButtonPressure_clicked();
    void on_pushButtonHoldOn_clicked();
    void on_pushButtonCopyToAll_clicked();

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
};



#endif // SETTINGDIALOG_H
