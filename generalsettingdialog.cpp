#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"
#include "crc16.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);

    ui->listWidgetStyle->setStyleSheet("font: 16px bold italic large \"Times New Roman\"");
    ui->listWidgetIcons->setStyleSheet("font: 16px bold italic large \"Times New Roman\"");

    ui->editPassword->setStyleSheet("font: 20px bold italic large \"Times New Roman\"");
    ui->editReceiver->setStyleSheet("font: 20px bold italic large \"Times New Roman\"");
    ui->editSender->setStyleSheet("font: 20px bold italic large \"Times New Roman\"");
    ui->editSubject->setStyleSheet("font: 20px bold italic large \"Times New Roman\"");

    acceptOnDeactilationEn = true;
    logedInSerial = false;
    logedInMail = false;
    logedInUserSett = false;
    ui->emailSettingWidget->setDisabled(true);
    qRegisterMetaTypeStreamOperators<EmailSettings>("EmailSettings");
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");
    this->eventFilterSetup();
    connect(ui->pButtonLockUnlockEmail, SIGNAL(clicked(bool)), this, SLOT(lockUnlockEmail()));
    connect(ui->pButtonAccept, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pButtonReject, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pButtonShowPassword, SIGNAL(clicked(bool)), this, SLOT(hideShowPassword()));
    connect(ui->pButtonChangeSerialSettings, SIGNAL(clicked(bool)), this, SLOT(changeSerialPortSettingsClicked()));
    connect(ui->listWidgetStyle, SIGNAL(currentRowChanged(int)), this, SLOT(styleChanged(int)));
    connect(ui->listWidgetIcons, SIGNAL(currentRowChanged(int)), this, SLOT(iconChanged(int)));
    connect(ui->pButtonServiceState, SIGNAL(clicked(bool)), this, SLOT(changeServiceStateClicked()));
    connect(ui->pButtonUserSetup, SIGNAL(clicked(bool)), this, SLOT(userSettingClicked()));
    connect(ui->pButtonDirection, SIGNAL(clicked(bool)), this, SLOT(changeDirection()));

    MachineSettings::MachineType t;
    t++;
    qDebug()<< "type"<<(int)t;

}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}

void GeneralSettingDialog::setIconFolder(QString path)
{
    pathIcon = path;
    ui->pButtonAccept->setIcon(QIcon(pathIcon+"/check.png"));
    ui->pButtonReject->setIcon(QIcon(pathIcon+"/multip.png"));

    if(ui->pButtonDirection->isChecked())
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateRight.png"));
    else
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateLeft.png"));
    ui->pButtonServiceState->setIcon(QIcon(pathIcon+"/maintanse.png"));
    ui->pButtonUserSetup->setIcon(QIcon(pathIcon+"/user.png"));



    ui->tabWidget->setTabIcon(0, QIcon(pathIcon+"/color.png"));
    ui->tabWidget->setTabIcon(1, QIcon(pathIcon+"/stopHand.png"));
    ui->tabWidget->setTabIcon(2, QIcon(pathIcon+"/mail.png"));
    ui->tabWidget->setTabIcon(3, QIcon(pathIcon+"/connect.png"));


}

void GeneralSettingDialog::setMachineSetting(MachineSettings::MachineParameters machineParam)
{
    ui->spinBoxHeadsCount->setValue(machineParam.headCount);
    ui->dSpinBoxWarningTime->setValue(machineParam.warningTime/10.);
    if(machineParam.direction == -1)
    {
        ui->pButtonDirection->setChecked(true);
        ui->pButtonDirection->setText("Direction\nclockwise");

    }
    else
    {
        ui->pButtonDirection->setChecked(false);
        ui->pButtonDirection->setText("Direction\nanticlockwise");

    }
}

void GeneralSettingDialog::setFocusLossAccept(bool flag)
{
    acceptOnDeactilationEn = flag;
}

void GeneralSettingDialog::setPasswords(uint16_t serialPass, uint16_t mailPass, uint16_t userPass)
{
    this->serialPassword = serialPass;
    this->mailPassword = mailPass;
    this->usersPassword = userPass;
}

void GeneralSettingDialog::setStyleList(QStringList stList, int curSelect, QStringList iconList, int iconSel)
{
    qDebug()<<curSelect;
    ui->listWidgetStyle->addItems(stList);
    ui->listWidgetStyle->setCurrentRow(curSelect);
    ui->listWidgetIcons->addItems(iconList);
    ui->listWidgetIcons->setCurrentRow(iconSel);
}

void GeneralSettingDialog::setEmailSettings(EmailSettings emailSett)
{
    ui->editPassword->setText(emailSett.senderPassword);
    ui->editReceiver->setText(emailSett.receiverAdress);
    ui->editSender->setText(emailSett.senderAdress);
    ui->editSubject->setText(emailSett.emailSubject);
    ui->checkBoxMailSendEnable->setChecked(emailSett.mailEnable);
}

void GeneralSettingDialog::accept()
{
    EmailSettings emailSett;
    emailSett.senderAdress = ui->editSender->text();
    emailSett.senderPassword = ui->editPassword->text();
    emailSett.receiverAdress = ui->editReceiver->text();
    if(ui->editSubject->text().isEmpty())
        emailSett.emailSubject = "LiQt machine interface autosend message";
    else
        emailSett.emailSubject = ui->editSubject->text();
    emailSett.mailEnable = ui->checkBoxMailSendEnable->isChecked();
    emit this->emailSettingsChanged(emailSett);

    MachineSettings::MachineParameters machineParams;
    machineParams.headCount = ui->spinBoxHeadsCount->value();
    machineParams.warningTime = ui->dSpinBoxWarningTime->value()*10;

    if(ui->pButtonDirection->isChecked())
        machineParams.direction = -1;
    else
        machineParams.direction = 1;

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
            ui->checkBoxMailSendEnable->setEnabled(true);
            ui->pButtonLockUnlockEmail->setText("Lock");
        }
        else{
            ui->emailSettingWidget->setEnabled(false);
            ui->checkBoxMailSendEnable->setEnabled(false);
            ui->pButtonLockUnlockEmail->setText("Unlock");
        }
    }
#ifndef DEBUG_BUILD
    else
    {
        ui->pButtonLockUnlockEmail->setChecked(false);
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
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
    ui->editPassword->installEventFilter(this);
    ui->editReceiver->installEventFilter(this);
    ui->editSender->installEventFilter(this);
    ui->editSubject->installEventFilter(this);
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
        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
#endif
    acceptOnDeactilationEn = true;

}

void GeneralSettingDialog::changeServiceStateClicked()
{
    acceptOnDeactilationEn = false;
    if(ui->pButtonServiceState->isChecked())
    {
        emit this->serviceSettingRequest();
        this->hide();
        this->show();
    }
    else
    {
        MachineSettings::setServiceWidgEn(false);
        this->hide();
        this->show();
    }

}

void GeneralSettingDialog::userSettingClicked()
{
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD
    if(!logedInUserSett){
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
    }
    if(logedInUserSett || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == this->usersPassword))
#endif
    {
        logedInUserSett = true;
        emit this->usersSettingRequest();
    }
#ifndef DEBUG_BUILD
    else
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
#endif
}

void GeneralSettingDialog::styleChanged(int index)
{
    emit this->styleChangedIndex(index);
}

void GeneralSettingDialog::iconChanged(int index)
{
    emit this->iconsChangedIndex(index);
}

void GeneralSettingDialog::changeDirection()
{
    if(ui->pButtonDirection->isChecked())
    {
        emit this->directionChanged(-1);
        ui->pButtonDirection->setText("Direction\nclockwise");
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateRight.png"));
    }
    else
    {
        emit this->directionChanged(1);
        ui->pButtonDirection->setText("Direction\nanticlockwise");
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateLeft.png"));
    }
}

void GeneralSettingDialog::showPortInfo(ComSettings comSett)
{
    ui->lPortName->setText(QString("Port name: %1").arg(comSett.name));
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
        if((watched == ui->editPassword)
                |(watched == ui->editReceiver)
                |(watched == ui->editSender)
                |(watched == ui->editSubject))
        {
            if(ui->pButtonLockUnlockEmail->isChecked())
            {
                QString text = KeyboardDialog::getText(this);
                if(!text.isEmpty())
                {
                    qobject_cast<QLineEdit*>(watched)->setText(text);
                    qobject_cast<QLineEdit*>(watched)->clearFocus();
                }
            }
        }
        else
        {
            qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
            qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
        }
        acceptOnDeactilationEn = true;
    }
    return false;
}

void GeneralSettingDialog::showEvent(QShowEvent *ev)
{
    ui->spinBoxHeadsCount->setVisible(MachineSettings::getServiceWidgEn());
    ui->labelH1->setVisible(MachineSettings::getServiceWidgEn());
    ui->tabWidget->setTabEnabled(3, MachineSettings::getServiceWidgEn());
    ui->pButtonServiceState->setChecked(MachineSettings::getServiceWidgEn());
    ev->accept();
    acceptOnDeactilationEn = true;


}
