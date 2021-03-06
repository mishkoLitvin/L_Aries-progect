#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"
#include "crc16.h"
#include "generalsettingdialog.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QWidget(parent),
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
    connect(ui->pButtonShowPassword, SIGNAL(clicked(bool)), this, SLOT(hideShowPassword()));
    connect(ui->pButtonChangeSerialSettings, SIGNAL(clicked(bool)), this, SLOT(changeSerialPortSettingsClicked()));
    connect(ui->listWidgetStyle, SIGNAL(currentRowChanged(int)), this, SLOT(styleChanged(int)));
    connect(ui->listWidgetIcons, SIGNAL(currentRowChanged(int)), this, SLOT(iconChanged(int)));
    connect(ui->listWidgetLang, SIGNAL(currentRowChanged(int)), this, SLOT(langChanged(int)));
    connect(ui->pButtonServiceState, SIGNAL(clicked(bool)), this, SLOT(changeServiceStateClicked()));
    connect(ui->pButtonUserSetup, SIGNAL(clicked(bool)), this, SLOT(userSettingClicked()));
    connect(ui->pButtonDirection, SIGNAL(clicked(bool)), this, SLOT(changeDirection()));
    connect(ui->pButtonUseUnload, SIGNAL(clicked(bool)), this, SLOT(useUnloadStateChanged()));
    connect(ui->spinBoxHeadsCount, SIGNAL(valueChanged(double)), this, SLOT(headCountChanged(double)));
    connect(ui->dSpinBoxWarningTime, SIGNAL(valueChanged(double)), this, SLOT(warningTimeChanged(double)));
    connect(ui->comboBoxMacineType, SIGNAL(currentIndexChanged(int)), this, SLOT(machineTypeChanget(int)));
    connect(ui->pButtonCyclesEnable, SIGNAL(clicked(bool)), this, SLOT(changeCyclesState()));

    ui->widgetServiceSettings->setVisible(false);
    this->setMaximumSize(400, 700);
    this->resize(this->maximumSize());
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}

void GeneralSettingDialog::setIconFolder(QString path)
{
    pathIcon = path;
    ui->pButtonAccept->setIcon(QIcon(pathIcon+"/check.png"));

    if(ui->pButtonDirection->isChecked())
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateRight.png"));
    else
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateLeft.png"));
    ui->pButtonServiceState->setIcon(QIcon(pathIcon+"/maintanse.png"));
    ui->pButtonUserSetup->setIcon(QIcon(pathIcon+"/user.png"));
    ui->pButtonCounters->setIcon(QIcon(pathIcon+"/counter.png"));


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
    this->machineParamGl.lastRevWarm.all = registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_TM);
    if(machineParam.direction == -1)
    {
        ui->pButtonDirection->setChecked(false);
        ui->pButtonDirection->setText(tr("Direction\nclockwise"));
    }
    else
    {
        ui->pButtonDirection->setChecked(true);
        ui->pButtonDirection->setText(tr("Direction\nanticlockwise"));
    }

    if(machineParam.lastRevWarm.field.revolver)
    {
        ui->pButtonCyclesEnable->setChecked(true);
        ui->pButtonCyclesEnable->setText(tr("Cycles\nDisable"));
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cyclesDis.png"));
    }
    else
    {
        ui->pButtonCyclesEnable->setChecked(false);
        ui->pButtonCyclesEnable->setText(tr("Cycles\nEnable"));
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
        ui->pButtonUseUnload->setText(tr("Don't use\nunload palett"));
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt.png"));
    }
    else
    {
        ui->pButtonUseUnload->setText(tr("Use\nunload palett"));
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt3.png"));
    }

    ui->pButtonWarming->setChecked(machineParam.lastRevWarm.field.warm);
    if(machineParam.lastRevWarm.field.warm)
    {
        ui->pButtonWarming->setText(tr("Disable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOff.png"));
    }
    else
    {
        ui->pButtonWarming->setText(tr("Enable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOn.png"));
    }

    ui->pButtonShowSkip->setChecked(MachineSettings::getSoftwartSkipEn());
    if(ui->pButtonShowSkip->isChecked())
    {
        ui->pButtonShowSkip->setText(tr("Hide skip\nbutton"));
    }
    else
        ui->pButtonShowSkip->setText(tr("Show skip\nbutton"));

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

void GeneralSettingDialog::setStyle(QStringList stList, int curSelect, QStringList iconList, int iconSel, bool backGrEn)
{
    ui->listWidgetStyle->addItems(stList);
    ui->listWidgetStyle->setCurrentRow(curSelect);
    ui->listWidgetIcons->addItems(iconList);
    ui->listWidgetIcons->setCurrentRow(iconSel);
    ui->checkBoxUseBackgr->setChecked(backGrEn);
}

void GeneralSettingDialog::setLangList(QStringList langList, int curSelect)
{
    ui->listWidgetLang->addItems(langList);
    ui->listWidgetLang->setCurrentRow(curSelect);
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
            machineParams.direction = 1;
        else
            machineParams.direction = -1;
        acceptEnable = false;
        machineParams.lastRevWarm = machineParamGl.lastRevWarm;
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
    if(ui->pButtonLockUnlockEmail->isChecked())
    {
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
        if(CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == this->mailPassword)
        {
            ui->emailSettingWidget->setEnabled(true);
            ui->checkBoxMailSendEnable->setEnabled(true);
            ui->pButtonLockUnlockEmail->setText(tr("Lock"));
            ui->editSender->setEchoMode(QLineEdit::Normal);
            ui->editReceiver->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            ui->pButtonLockUnlockEmail->setChecked(false);
            ui->emailSettingWidget->setEnabled(false);
            ui->checkBoxMailSendEnable->setEnabled(false);
            ui->pButtonLockUnlockEmail->setText(tr("Unlock"));
            ui->editSender->setEchoMode(QLineEdit::Password);
            ui->editReceiver->setEchoMode(QLineEdit::Password);
            QMessageBox msgBox;
            msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
            msgBox.setText("Wrong password!");
            msgBox.setWindowTitle("Password");
            msgBox.exec();
        }
    }
    else
    {
        ui->emailSettingWidget->setEnabled(false);
        ui->checkBoxMailSendEnable->setEnabled(false);
        ui->editSender->setEchoMode(QLineEdit::Password);
        ui->editPassword->setEchoMode(QLineEdit::Password);
        ui->pButtonShowPassword->setChecked(false);
        ui->pButtonShowPassword->setText(tr("Show"));
        ui->editReceiver->setEchoMode(QLineEdit::Password);
        ui->pButtonLockUnlockEmail->setText(tr("Unlock"));
    }
    acceptOnDeactilationEn = true;


}

void GeneralSettingDialog::hideShowPassword()
{
    if(ui->pButtonShowPassword->isChecked())
    {
        ui->pButtonShowPassword->setText(tr("Hide"));
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pButtonShowPassword->setText(tr("Show"));
        ui->editPassword->setEchoMode(QLineEdit::Password);
    }
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
    if(ui->pButtonServiceState->isChecked())
    {
        emit this->serviceSettingRequest();
        this->hide();
        ui->pButtonServiceState->setText("DISABLE\nSERVICE");
    }
    else
    {
        emit this->serviceSettingRequest();
        this->hide();
        ui->pButtonServiceState->setText("ENABLE\nSERVICE");
        ui->widgetServiceSettings->setVisible(false);
        this->setMaximumSize(400, 700);
    }

}

void GeneralSettingDialog::userSettingClicked()
{
    QByteArray passwordBArr;
    passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
    if((CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == this->usersPassword))
        emit this->usersSettingRequest();
    else
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
}

void GeneralSettingDialog::styleChanged(int index)
{
    emit this->styleChangedIndex(index);
}

void GeneralSettingDialog::iconChanged(int index)
{
    emit this->iconsChangedIndex(index);
}

void GeneralSettingDialog::langChanged(int index)
{
    emit this->langChangedIndex(index);
}

void GeneralSettingDialog::changeDirection()
{
    if(ui->pButtonDirection->isChecked())
    {
        emit this->directionChanged(1);
        ui->pButtonDirection->setText(tr("Direction\nanticlockwise"));
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateLeft.png"));
    }
    else
    {
        emit this->directionChanged(-1);
        ui->pButtonDirection->setText(tr("Direction\nclockwise"));
        ui->pButtonDirection->setIcon(QIcon(pathIcon+"/rotateRight.png"));
    }
    QByteArray cmdArr;
    int data;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::IndexDirChange;
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::changeCyclesState()
{
    this->machineParamGl.lastRevWarm.field.revolver = ui->pButtonCyclesEnable->isChecked();
    this->machineParamGl.lastRevWarm.field.last = ui->pButtonCyclesEnable->isChecked();

    if(ui->pButtonCyclesEnable->isChecked())
    {
        ui->pButtonCyclesEnable->setText(tr("Disable\ncycles"));
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cyclesDis.png"));
    }
    else
    {
        ui->pButtonCyclesEnable->setText(tr("Enable\ncycles"));
        ui->pButtonCyclesEnable->setIcon(QIcon(pathIcon+"/cycles.png"));
    }
    QByteArray cmdArr;
    int data;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexLastRevolvWarm&0x00FF));
    data = this->machineParamGl.lastRevWarm.all;
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::useUnloadStateChanged()
{
    emit this->unloadStateChanged(ui->pButtonUseUnload->isChecked());
    if(ui->pButtonUseUnload->isChecked())
    {

        ui->pButtonUseUnload->setText(tr("Don't use\nunload palett"));
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt.png"));
    }
    else
    {
        ui->pButtonUseUnload->setText(tr("Use\nunload palett"));
        ui->pButtonUseUnload->setIcon(QIcon(pathIcon+"/tt3.png"));
    }
}

void GeneralSettingDialog::headCountChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterHeadCount&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::machineTypeChanget(int index)
{
//    MachineSettings mStt;
//    QByteArray cmdArr;
//    int data = mStt.machineTypeData[index];
//    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
//    cmdArr.append(static_cast<char>(MachineSettings::MasterMachineType&0x00FF));
//    cmdArr.append(static_cast<char>(data>>8));
//    cmdArr.append(static_cast<char>(data&0x00FF));
//    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
//    cmdArr.append(static_cast<char>(data>>8));
//    cmdArr.append(static_cast<char>(data&0x00FF));
//    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::warningTimeChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::WarningTime&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void GeneralSettingDialog::showPortInfo(ComSettings comSett)
{
    ui->lPortName->setText(QString(tr("Port name: %1")).arg(comSett.name));
    ui->lBaudRate->setText(QString(tr("Baud rate: %1")).arg(comSett.baudRate));
    ui->lDataBits->setText(QString(tr("Data bits: %1")).arg(comSett.stringDataBits));
    ui->lFlowControl->setText(QString(tr("Flow control: %1")).arg(comSett.stringFlowControl));
    ui->lParity->setText(QString(tr("Parity: %1")).arg(comSett.stringParity));
    ui->lStopBits->setText(QString(tr("Stop bits: %1")).arg(comSett.stringStopBits));
}

bool GeneralSettingDialog::event(QEvent *e)
{
//    if((e->type()==QEvent::WindowDeactivate)
//            |((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave))
//            |((QApplication::platformName() == "linuxfb")&(e->type()==QEvent::Leave)))
    if(e->type()==QEvent::HideToParent)
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
//    qDebug()<<e->type();
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

void GeneralSettingDialog::manualShow()
{
    ui->spinBoxHeadsCount->setVisible(MachineSettings::getServiceWidgEn());
    ui->labelH1->setVisible(MachineSettings::getServiceWidgEn());
    ui->pButtonUseUnload->setVisible(MachineSettings::getServiceWidgEn());
    ui->tabWidget->setTabEnabled(3, MachineSettings::getServiceWidgEn());
//    ui->tabWidget->setTabEnabled(2, MachineSettings::getServiceWidgEn());
    ui->pButtonServiceState->setChecked(MachineSettings::getServiceWidgEn());
    ui->widgetServiceSettings->setVisible(MachineSettings::getServiceWidgEn());

    this->manualResize();
    this->show();
}

void GeneralSettingDialog::setRegisterPointer(Register *reg)
{
    this->registers = reg;
}

void GeneralSettingDialog::showEvent(QShowEvent *ev)
{
    acceptOnDeactilationEn = true;
    acceptEnable = true;
    ev->accept();
}

void GeneralSettingDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void GeneralSettingDialog::on_pButtonHeadsActivation_clicked()
{
    emit this->headActivationRequest();
}

void GeneralSettingDialog::on_checkBoxUseBackgr_clicked()
{
    ui->pButtonSelectImg->setEnabled(ui->checkBoxUseBackgr->isChecked());
    emit this->imageRequest(ui->checkBoxUseBackgr->isChecked(), false);
}

void GeneralSettingDialog::on_pButtonSelectImg_clicked()
{
    emit this->imageRequest(ui->checkBoxUseBackgr->isChecked(), true);
}

void GeneralSettingDialog::on_pButtonWarming_clicked()
{
    this->machineParamGl.lastRevWarm.field.warm = ui->pButtonWarming->isChecked();
    this->machineParamGl.lastRevWarm.field.warm = ui->pButtonWarming->isChecked();

    if(ui->pButtonWarming->isChecked())
    {
        ui->pButtonWarming->setText(tr("Disable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOff.png"));
    }
    else
    {
        ui->pButtonWarming->setText(tr("Enable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOn.png"));
        QByteArray cmdArr;
        int data;
        cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
        cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexLastRevolvWarm&0x00FF));
        data = this->machineParamGl.lastRevWarm.all;
        cmdArr.append(static_cast<char>(data>>8));
        cmdArr.append(static_cast<char>(data&0x00FF));
        data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
        cmdArr.append(static_cast<char>(data>>8));
        cmdArr.append(static_cast<char>(data&0x00FF));
        emit this->sendCommand(cmdArr);
    }
    emit this->warmingStateChanged(ui->pButtonWarming->isChecked());

}

void GeneralSettingDialog::on_pButtonCounters_clicked()
{
    emit this->countersDialogRequest();
    this->accept();
}

void GeneralSettingDialog::manualResize()
{
    if(ui->widgetServiceSettings->isHidden())
    {
        this->setMaximumSize(500, 703);
    }
    else
    {
        this->setMaximumSize(700, 703);
    }
    this->resize(this->maximumSize());
}

void GeneralSettingDialog::on_pButtonShowSkip_clicked()
{
    MachineSettings::setSoftwartSkipEn(ui->pButtonShowSkip->isChecked());
    if(ui->pButtonShowSkip->isChecked())
    {
        ui->pButtonShowSkip->setText(tr("Hide skip\nbutton"));
    }
    else
        ui->pButtonShowSkip->setText(tr("Show skip\nbutton"));
    emit this->skipStateChanged();
}
