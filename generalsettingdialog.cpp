#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"
#include "crc16.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);
    acceptOnDeactilationEn = true;
    ui->emailSettingWidget->setDisabled(true);
    qRegisterMetaTypeStreamOperators<EmailSettings>("EmailSettings");
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");
    this->eventFilterSetup();
    connect(ui->pButtonLockUnlockEmail, SIGNAL(clicked(bool)), this, SLOT(lockUnlockEmail()));
    connect(ui->pButtonAccept, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pButtonReject, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pButtonShowPassword, SIGNAL(clicked(bool)), this, SLOT(hideShowPassword()));
    connect(ui->pButtonChangeSerialSettings, SIGNAL(clicked(bool)), this, SLOT(changeSerialPortSettingsClicked()));
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}

void GeneralSettingDialog::setMachineSetting(MachineSettings::MachineParameters machineParam)
{
    ui->spinBoxHeadsCount->setValue(machineParam.HeadCount);
    ui->dSpinBoxWarningTime->setValue(machineParam.WarningTime/10.);
}

void GeneralSettingDialog::setFocusLossAccept(bool flag)
{
    acceptOnDeactilationEn = flag;
}

void GeneralSettingDialog::setPasswords(uint16_t serialPass, uint16_t mailPass)
{
    this->serialPassword = serialPass;
    this->mailPassword = mailPass;
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

    MachineSettings::MachineParameters machineParams;
    machineParams.HeadCount = ui->spinBoxHeadsCount->value();
    machineParams.WarningTime = ui->dSpinBoxWarningTime->value()*10;

    emit this->machineParamChanged(machineParams.toByteArray());

    this->hide();
}

void GeneralSettingDialog::reject()
{
    this->hide();
}

void GeneralSettingDialog::lockUnlockEmail()
{
    acceptOnDeactilationEn = false;
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD
    if(!logedInMail){
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
    }
    if(logedInMail || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == this->mailPassword))
#endif
    {
        logedInMail = true;
        if(ui->pButtonLockUnlockEmail->isChecked()){
            ui->emailSettingWidget->setEnabled(true);
            ui->pButtonLockUnlockEmail->setText("Lock");
        }
        else{
            ui->emailSettingWidget->setDisabled(true);
            ui->pButtonLockUnlockEmail->setText("Unlock");
        }
    }
#ifndef DEBUG_BUILD
    else
    {
        ui->pButtonLockUnlockEmail->setChecked(false);
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet());
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
#endif
    acceptOnDeactilationEn = true;


}

void GeneralSettingDialog::hideShowPassword()
{
    if(ui->pButtonShowPassword->isChecked())
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->editPassword->setEchoMode(QLineEdit::Password);
}

void GeneralSettingDialog::eventFilterSetup()
{
    QObjectList objList = ui->dSpinBoxWarningTime->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxHeadsCount->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
}

void GeneralSettingDialog::changeSerialPortSettingsClicked()
{
    acceptOnDeactilationEn = false;
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD

    if(!logedInSerial){
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
    }
    if(logedInSerial || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == this->serialPassword))
#endif
    {
        logedInSerial = true;
        emit this->serialPortSettingsDialogRequested();
    }
#ifndef DEBUG_BUILD
    else
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet());
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
#endif
    acceptOnDeactilationEn = true;

}

void GeneralSettingDialog::showPortInfo(ComSettings comSett)
{
    qDebug() << "riiiiiiiiii";
    ui->lBaudRate->setText(QString("Baud rate: %1").arg(comSett.baudRate));
    ui->lDataBits->setText(QString("Data bits: %1").arg(comSett.stringDataBits));
    ui->lFlowControl->setText(QString("Flow control: %1").arg(comSett.stringFlowControl));
    ui->lParity->setText(QString("Parity: %1").arg(comSett.stringParity));
    ui->lStopBits->setText(QString("Stop bits: %1").arg(comSett.stringStopBits));
}

bool GeneralSettingDialog::event(QEvent *e)
{
    if((e->type()==QEvent::WindowDeactivate)|((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave)))
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}

bool GeneralSettingDialog::eventFilter(QObject *watched, QEvent *event)
{

    if((event->type()==QEvent::MouseButtonDblClick)|((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease)))
    {
        acceptOnDeactilationEn = false;
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
        acceptOnDeactilationEn = true;
    }
    return false;
}
