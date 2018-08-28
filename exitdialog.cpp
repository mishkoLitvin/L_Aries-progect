#include "exitdialog.h"
#include "ui_exitdialog.h"

ExitDialog::ExitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExitDialog)
{
    ui->setupUi(this);
}

ExitDialog::~ExitDialog()
{
    delete ui;
}

ExitDialog::ExitCode ExitDialog::tryExit(QWidget *parent, bool logOutEn)
{
    ExitDialog dialog(parent);
    dialog.setLogOutEn(logOutEn);
    dialog.move(parent->width()/2-dialog.width()/2, parent->height()/2-dialog.height()/2);
    dialog.exec();
    return dialog.exitCode;
}

void ExitDialog::setLogOutEn(bool en)
{
    ui->pButtonLogOut->setVisible(en);
}

void ExitDialog::on_pButtonTurnOff_clicked()
{
    this->exitCode = Shutdown;
    this->accept();
}

void ExitDialog::on_pButtonRestartApp_clicked()
{
    this->exitCode = RestartProgram;
    this->accept();
}

void ExitDialog::on_pButtonRastartAll_clicked()
{
    this->exitCode = RestartMachine;
    this->accept();
}

void ExitDialog::on_pButtonServiceMode_clicked()
{
    this->exitCode = ServiceMode;
    this->accept();
}

void ExitDialog::on_pButtonExitApp_clicked()
{
    this->exitCode = ExitFromProgram;
    this->accept();
}

void ExitDialog::on_pButtonReprogram_clicked()
{
    this->exitCode = ReprogramMachine;
    this->accept();
}

void ExitDialog::on_pushButtonCancel_clicked()
{
    this->exitCode = Continue;
    this->accept();
}

void ExitDialog::on_pButtonLogOut_clicked()
{
    this->exitCode = LogOut;
    this->accept();
}


void ExitDialog::showEvent(QShowEvent *ev)
{
    ui->widget->setVisible(MachineSettings::getServiceWidgEn());
    ev->accept();
}

void ExitDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

