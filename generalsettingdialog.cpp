#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);
    acceptOnDeactilationEn = true;
    ui->emailSettingWidget->setDisabled(true);
    qRegisterMetaTypeStreamOperators<EmailSettings>("EmailSettings");


    setStyleSheet(QString(("* {color: #ABEFF6;"
                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
                           "selection-color: yellow;"
                           "selection-background-color: blue;"
                           "font: 14px bold italic large \"Times New Roman\"}"
                           "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                           "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8080A0, stop: 0.8 #606070,stop: 1.0 #202030);}")));

    connect(ui->pButtonLockUnlockEmail, SIGNAL(clicked(bool)), this, SLOT(lockUnlockEmail()));
    connect(ui->pButtonAccept, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pButtonShowPassword, SIGNAL(clicked(bool)), this, SLOT(hideShowPassword()));
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}

void GeneralSettingDialog::setEmailSettings(EmailSettings emailSett)
{
    ui->editPassword->setText(emailSett.senderPassword);
    ui->editReceiver->setText(emailSett.receiverAdress);
    ui->editSender->setText(emailSett.senderAdress);
    ui->editSubject->setText(emailSett.emailSubject);
}

void GeneralSettingDialog::accept()
{
    EmailSettings emailSett;
    emailSett.senderAdress = ui->editSender->text();
    emailSett.senderPassword = ui->editPassword->text();
    emailSett.receiverAdress = ui->editReceiver->text();
    emailSett.emailSubject = ui->editSubject->text();
    emit this->emailSettingsChanged(emailSett);

    this->hide();
}

void GeneralSettingDialog::reject()
{
    this->hide();
}

void GeneralSettingDialog::lockUnlockEmail()
{
    if(ui->pButtonLockUnlockEmail->isChecked()){
        ui->emailSettingWidget->setEnabled(true);
        ui->pButtonLockUnlockEmail->setText("Lock");
//        QByteArray passwordBArr;
//        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
//        if((CalculateCRC16(0xFFFF, passwordBArr) == ->value("PASSWORD_EMAIL"))){

//        }
    }
    else{
        ui->emailSettingWidget->setDisabled(true);
        ui->pButtonLockUnlockEmail->setText("Unlock");
    }
}

void GeneralSettingDialog::hideShowPassword()
{
    if(ui->pButtonShowPassword->isChecked())
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    if(!ui->pButtonShowPassword->isChecked())
        ui->editPassword->setEchoMode(QLineEdit::Password);
}

bool GeneralSettingDialog::event(QEvent *e)
{
    if(e->type()==QEvent::WindowDeactivate)
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}
