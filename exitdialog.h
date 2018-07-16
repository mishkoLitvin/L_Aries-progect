#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QDialog>
#include <QShowEvent>

#include "settings.h"

namespace Ui {
class ExitDialog;
}

class ExitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExitDialog(QWidget *parent = 0);
    ~ExitDialog();

    typedef enum ExitCode_{
        Continue,
        Shutdown,
        ExitFromProgram,
        ReprogramMachine,
        ServiceMode,
        RestartProgram,
        RestartMachine
    }ExitCode;

    static ExitCode tryExit(QWidget *parent);

private:
    ExitCode exitCode;

private slots:
    void on_pButtonTurnOff_clicked();
    void on_pButtonRestartApp_clicked();
    void on_pButtonRastartAll_clicked();
    void on_pButtonServiceMode_clicked();
    void on_pButtonExitApp_clicked();
    void on_pButtonReprogram_clicked();



    void on_pushButton_clicked();

private:
    Ui::ExitDialog *ui;

protected:
    void showEvent(QShowEvent *ev);
    void changeEvent(QEvent *event);
};

#endif // EXITDIALOG_H
