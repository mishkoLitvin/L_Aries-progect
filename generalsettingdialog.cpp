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

    MachineSettings mSett;
    int i;
    for(i = 0; i<mSett.machineTypeList.length(); i++)
    {
        ui->comboBoxMacineType->addItem(mSett.machineTypeList[i], mSett.machineTypeData[i]);
    }
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
    connect(ui->pButtonUseUnload, SIGNAL(clicked(bool)), this, SLOT(useUnloadStateChanged()));
    connect(ui->spinBoxHeadsCount, SIGNAL(valueChanged(double)), this, SLOT(headCountChanged(double)));
    connect(ui->dSpinBoxWarningTime, SIGNAL(valueChanged(double)), this, SLOT(warningTimeChanged(double)));
    connect(ui->comboBoxMacineType, SIGNAL(currentIndexChanged(int)), this, SLOT(machineTypeChanget(int)));
    connect(ui->pButtonCyclesEnable, SIGNAL(clicked(bool)), this, SLOT(changeCyclesState()));
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
    machineParamGl = machineParam;
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

    if(machineParam.lastRevWarm.field.revolver)
    {
        ui->pButtonCyclesEnable->setChecked(true);
        ui->pButtonCyclesEnable->setText("Cycles\nDisable");
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cyclesDis.png"));
    }
    else
    {
        ui->pButtonCyclesEnable->setChecked(false);
        ui->pButtonCyclesEnable->setText("Cycles\nEnable");
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cycles.png"));
    }

    ui->comboBoxMacineType->setCurrentIndex(machineParam.machineType);

    ui->comboBoxCarriageT->setCurrentIndex(machineParam.headType.field.carriageType-1);
    ui->comboBoxHeadServoT->setCurrentIndex(machineParam.headType.field.servoDriveType);
    ui->comboBoxSqFlT->setCurrentIndex(machineParam.headType.field.sqFlType-1);

    ui->comboBoxIndexerT->setCurrentIndex(machineParam.indexeLiftType.field.indexerType-1);
    ui->comboBoxLiftT->setCurrentIndex(machineParam.indexeLiftType.field.liftType-1);
    ui->comboBoxLockT->setCurrentIndex(machineParam.indexeLiftType.field.lockType);
    ui->comboBoxMainServoT->setCurrentIndex(machineParam.indexeLiftType.field.mainServoDriveType);

    ui->checkBoxHMIConnected->setChecked(machineParam.indexeLiftType.field.hmiIsConnected);
    ui->checkBoxKeyPadConnected->setChecked(machineParam.indexeLiftType.field.keypadIsConnected);

    ui->dSpinBoxHeadMaxRange->setValue(machineParam.headMaxRange/10);
    ui->dSpinBoxIndexerScrew->setValue(machineParam.indexerScrewPinch);
    ui->dSpinBoxLiftGear->setValue(machineParam.liftGearRatio);
    ui->pButtonUseUnload->setChecked(machineParam.useUnloadHead);
    if(machineParam.useUnloadHead)
    {
        ui->pButtonUseUnload->setText("Don't use\nunload palett");
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt.png"));
    }
    else
    {
        ui->pButtonUseUnload->setText("Use\nunload palett");
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt3.png"));
    }

//    =============================================
    MachineSettings::setHeadMaxRange(machineParam.headMaxRange);
    MachineSettings::setHeadType(machineParam.headType.all);
    MachineSettings::setIndexLiftType(machineParam.indexeLiftType.all);
//    ===============================================
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
    if(acceptEnable)
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
        machineParams.machineType = (MachineSettings::MachineType)ui->comboBoxMacineType->currentIndex();
        MachineSettings::setMachineType(machineParams.machineType);
        machineParams.headMaxRange = ui->dSpinBoxHeadMaxRange->value()*10;
        machineParams.liftGearRatio = ui->dSpinBoxLiftGear->value();
        machineParams.indexerScrewPinch = ui->dSpinBoxIndexerScrew->value();
        machineParams.useUnloadHead = ui->pButtonUseUnload->isChecked();
        machineParams.lastRevWarm.field.revolver = ui->pButtonCyclesEnable->isChecked();
        machineParams.headType.field.carriageType = ui->comboBoxCarriageT->currentIndex()+1;
        machineParams.headType.field.servoDriveType = ui->comboBoxHeadServoT->currentIndex();
        machineParams.headType.field.sqFlType = ui->comboBoxSqFlT->currentIndex()+1;
        machineParams.indexeLiftType.field.hmiIsConnected = ui->checkBoxHMIConnected->isChecked();
        machineParams.indexeLiftType.field.keypadIsConnected = ui->checkBoxKeyPadConnected->isChecked();
        machineParams.indexeLiftType.field.indexerType = ui->comboBoxIndexerT->currentIndex()+1;
        machineParams.indexeLiftType.field.liftType = ui->comboBoxLiftT->currentIndex()+1;
        machineParams.indexeLiftType.field.lockType = ui->comboBoxLockT->currentIndex();
        machineParams.indexeLiftType.field.mainServoDriveType = ui->comboBoxMainServoT->currentIndex();
        //    =============================================
        MachineSettings::setHeadMaxRange(machineParams.headMaxRange/10);
        MachineSettings::setHeadType(machineParams.headType.all);
        MachineSettings::setIndexLiftType(machineParams.indexeLiftType.all);
        //    ===============================================
        if(ui->pButtonDirection->isChecked())
            machineParams.direction = -1;
        else
            machineParams.direction = 1;
        acceptEnable = false;
        this->hide();
        emit this->machineParamChanged(machineParams.toByteArray());
    }
    else
        acceptEnable = true;


}

void GeneralSettingDialog::reject()
{
    acceptEnable = false;
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
    objList = ui->dSpinBoxHeadMaxRange->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxIndexerScrew->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxLiftGear->children();
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
        emit this->directionChanged(1);
        ui->pButtonDirection->setText("Direction\nanticlockwise");
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateLeft.png"));
    }
    else
    {
        emit this->directionChanged(-1);
        ui->pButtonDirection->setText("Direction\nclockwise");
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateRight.png"));
    }
    QByteArray cmdArr;
    int data;
    cmdArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append((char)(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::IndexDirChange;
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::changeCyclesState()
{
    if(ui->pButtonCyclesEnable->isChecked())
    {
        ui->pButtonCyclesEnable->setText("Disable\ncycles");
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cyclesDis.png"));
    }
    else
    {
        ui->pButtonCyclesEnable->setText("Enable\ncycles");
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cycles.png"));
    }
    QByteArray cmdArr;
    int data;
    cmdArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append((char)(IndexerLiftSettings::IndexLastRevolvWarm&0x00FF));
    data = this->machineParamGl.lastRevWarm.all;
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::useUnloadStateChanged()
{
    emit this->unloadStateChanged(ui->pButtonUseUnload->isChecked());
    if(ui->pButtonUseUnload->isChecked())
    {

        ui->pButtonUseUnload->setText("Don't use\nunload palett");
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt.png"));
    }
    else
    {
        ui->pButtonUseUnload->setText("Use\nunload palett");
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt3.png"));
    }
}

void GeneralSettingDialog::headCountChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append((char)(MachineSettings::MasterHeadCount&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::machineTypeChanget(int index)
{
    MachineSettings mStt;
    QByteArray cmdArr;
    int data = mStt.machineTypeData[index];
    cmdArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append((char)(MachineSettings::MasterMachineType&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::warningTimeChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append((char)(IndexerLiftSettings::WarningTime&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);
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
    if((e->type()==QEvent::WindowDeactivate)
            |((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave))
            |((QApplication::platformName() == "linuxfb")&(e->type()==QEvent::Leave)))
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}

bool GeneralSettingDialog::eventFilter(QObject *watched, QEvent *event)
{

    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease))
            |((QApplication::platformName() == "linuxfb")&(event->type()==QEvent::MouseButtonRelease)))
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
    ui->pButtonUseUnload->setVisible(MachineSettings::getServiceWidgEn());
    ui->tabWidget->setTabEnabled(3, MachineSettings::getServiceWidgEn());
    ui->pButtonServiceState->setChecked(MachineSettings::getServiceWidgEn());
//    ui->labelH2->setVisible(MachineSettings::getServiceWidgEn());
//    ui->comboBoxMacineType->setVisible(MachineSettings::getServiceWidgEn());
    ui->widgetServiceSettings->setVisible(MachineSettings::getServiceWidgEn());
    ev->accept();
    acceptOnDeactilationEn = true;
    acceptEnable = true;

}
