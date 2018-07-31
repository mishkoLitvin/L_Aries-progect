#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "math.h"
#include "crc16.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaTypeStreamOperators<EmailSettings>("EmailSettings");
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");

    usersSettingDialog = new UserSettingDialog(this);

    settings = new QSettings("./settings.ini", QSettings::IniFormat);

    this->getLangFile(settings->value("STYLE/LANG_SEL_INDEX", 0).toInt());
    this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                          +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt()))).toString());
    ui->widgetHeads->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    headsCount = settings->value(QString("HEAD/HEADS_COUNT"), 1).toInt();

    comPort = new SerialPort(settings->value("COM_SETTING").value<ComSettings>(),this);
    comPort->setStyleSheet(this->styleSheet());
    connect(comPort, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));
    connect(comPort, SIGNAL(dataReady(ModData)), this, SLOT(getSerialData(ModData)));

    timerMain = new QTimer(this);
    connect(timerMain, SIGNAL(timeout()), this, SLOT(indexerStepFinish()));

    headSettingDialog = new SettingDialog(headSettings);
    connect(headSettingDialog, SIGNAL(accept(int,QByteArray)), this, SLOT(getHeadParam(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(changeNumber(int)), this, SLOT(changeHeadNo(int)));
    connect(headSettingDialog, SIGNAL(setParamsToAll(int,QByteArray)), this, SLOT(getAllHeadParam(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(sendCommand(int,QByteArray)), this, SLOT(getHeadCommand(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(colorChanged(int, QColor)), this, SLOT(getHeadColor(int, QColor)));
    headSettingDialog->setStyleSheet(this->styleSheet());

    indexer = new IndexerWidget(this);
    connect(indexer, SIGNAL(settingButtonCliced()), this, SLOT(indexerLiftSettingRequest()));
    connect(indexer, SIGNAL(sendCommand(QByteArray)), this, SLOT(getIndexLiftCommand(QByteArray)));
    connect(indexer, SIGNAL(resetRequest()), this, SLOT(resetMachine()));
    ui->layoutIndexer->addWidget(indexer);
    connect(indexer, SIGNAL(startPrint(bool)), this, SLOT(startPrintProcess(bool)));
    connect(indexer, SIGNAL(stopPrint()), this, SLOT(stopPrintProcess()));

    indexerLiftSetDialog = new IndexerSettingDialog();
    indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                      settings->value("LIFT_PARAMS").value<QByteArray>());
    connect(indexerLiftSetDialog, SIGNAL(indexerParamChanged(QByteArray)), this, SLOT(getIndexerParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(liftParamChanged(QByteArray)), this, SLOT(getLiftParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getIndexLiftSettComm(QByteArray)));
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());

    generalSettingDialog = new GeneralSettingDialog();
    machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
    generalSettingDialog->setMachineSetting(machineSettings.machineParam);
    generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS").value<EmailSettings>());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL").toInt(),
                                       settings->value("PASSWORDS/PASSWORD_LOCK_MAIL").toInt(),
                                       settings->value("PASSWORDS/PASSWORD_USERS").toInt());
    generalSettingDialog->setStyle(settings->value("STYLE/STYLE_LIST").value<QStringList>(),
                                       settings->value("STYLE/STYLE_SEL_INDEX").toInt(),
                                       settings->value("STYLE/ICON_LIST").value<QStringList>(),
                                       settings->value("STYLE/ICON_SEL_INDEX").toInt(),
                                       settings->value("STYLE/IMAGE_EN").toBool());
    generalSettingDialog->setLangList(settings->value("STYLE/LANG_LIST").value<QStringList>(),
                                      settings->value("STYLE/LANG_SEL_INDEX").toInt());
    generalSettingDialog->showPortInfo(settings->value("COM_SETTING").value<ComSettings>());
    connect(ui->pButtonSetting, SIGNAL(clicked(bool)), this,  SLOT(generalSettingDialogRequest()));
    connect(generalSettingDialog, SIGNAL(machineParamChanged(QByteArray)), this, SLOT(getMachineParam(QByteArray)));
    connect(generalSettingDialog, SIGNAL(emailSettingsChanged(EmailSettings)), this, SLOT(getEmailSettings(EmailSettings)));
    connect(generalSettingDialog, SIGNAL(serialPortSettingsDialogRequested()), comPort, SLOT(setupPort()));
    connect(generalSettingDialog, SIGNAL(serviceSettingRequest()), this, SLOT(serviceStateChange()));
    connect(generalSettingDialog, SIGNAL(styleChangedIndex(int)), this, SLOT(getVeiwSettings(int)));
    connect(generalSettingDialog, SIGNAL(iconsChangedIndex(int)), this, SLOT(setIconFolder(int)));
    connect(generalSettingDialog, SIGNAL(langChangedIndex(int)), this, SLOT(getLangFile(int)));
    connect(generalSettingDialog, SIGNAL(usersSettingRequest()), usersSettingDialog, SLOT(show()));
    connect(generalSettingDialog, SIGNAL(directionChanged(int)), this, SLOT(getDirection(int)));
    connect(generalSettingDialog, SIGNAL(unloadStateChanged(bool)), this, SLOT(setUnloadState(bool)));
    connect(generalSettingDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getMachineCommand(QByteArray)));
    connect(generalSettingDialog, SIGNAL(imageRequest(bool, bool)), this, SLOT(setBackGround(bool,bool)));

    mailSender = new MailSender(this);
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().receiverAdress);

    connect(ui->pButtonExit, SIGNAL(clicked(bool)), this, SLOT(exitProgram()));
    connect(ui->pButtonSaveJob, SIGNAL(clicked(bool)), this, SLOT(saveJob()));
    connect(ui->pButtonLoadJob, SIGNAL(clicked(bool)), this, SLOT(loadJob()));

    int i;

    for(i = 0; i<7; i++)
        headStylesStr.append(settings->value("STYLE/HEAD_STYLES_"+QString::number(i)).toString());

    this->headsInit();

    infoWidget = new InfoWidget(ui->widgetHeads);

    if(QApplication::platformName() != "eglfs")
        this->resize(QSize(1024, 768));
    else
        this->setWindowState(Qt::WindowMaximized);

    this->setHeadsPosition();

    maintanceDialog = new MaintanceDialog(this);
    maintanceDialog->setStyleSheet(this->styleSheet());
    connect(maintanceDialog, SIGNAL(stopRequest()), indexer, SLOT(printFinish()));
    connect(maintanceDialog, SIGNAL(continueRequest()), indexer, SLOT(printStart()));
    connect(maintanceDialog, SIGNAL(maintanceWorkEnable(bool)), this, SLOT(maintanceWorkSlot(bool)));
    connect(ui->pButtonMaintance, SIGNAL(clicked(bool)), maintanceDialog, SLOT(openMaintanceList()));
    connect(indexer, SIGNAL(stopPrint()), maintanceDialog, SLOT(openDialog()));

    registers = new Register(headsCount);

    registers->setMasterReg(this->machineSettings);
    registers->setIndexLiftReg(this->indexerLiftSettings);
    comPort->setRegisterPointer(this->registers);
    infoWidget->setRegisterPointer(this->registers);
    indexerLiftSetDialog->setRegisters(this->registers);
    headSettingDialog->setRegisters(this->registers);

    for(i = 0; i < this->headsCount; i++)
    {
        headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
        registers->setHeadReg(i, headSettings);
    }

    udpHandler = new UdpSocket(this);
    udpHandler->startUdp(true);
    connect(udpHandler, SIGNAL(dataReady(QByteArray)), this, SLOT(getUdpData(QByteArray)));

    cycleDialog = new CyclesDialog(this->headsCount, this);
    connect(ui->pButtonCyclesSetup, SIGNAL(clicked(bool)), cycleDialog, SLOT(show()));
    connect(cycleDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getCyclesCommand(QByteArray)));

    ui->widgetHeads->installEventFilter(this);
    ui->dSpinBoxLiftOffcet->children()[0]->installEventFilter(this);
    connect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
    connect(indexerLiftSetDialog, SIGNAL(liftDistanceChanged(float)), this, SLOT(getLiftDistance(float)));
    ui->dSpinBoxStepDelay->children()[0]->installEventFilter(this);
    connect(ui->dSpinBoxStepDelay, SIGNAL(valueChanged(double)), this, SLOT(getStepDelayTime(double)));

    watchDog = new QTimer(this);
    watchDog->setInterval(1000);
    connect(watchDog, SIGNAL(timeout()), this, SLOT(watchDogTimeout()));
    connect(comPort, SIGNAL(working()), watchDog, SLOT(start()));

    this->zeroStart();

    reprogramDialog = new ReprogramDialog(this);
    connect(reprogramDialog, SIGNAL(programArrReady(ReprogramDialog::BoardType, QByteArray)), comPort, SLOT(sendProgram(ReprogramDialog::BoardType, QByteArray)));
    connect(comPort, SIGNAL(proramProgres(int)), reprogramDialog, SLOT(setProgress(int)));

}

MainWindow::~MainWindow()
{
    comPort->closeSerialPort();

    headButton.clear();
    headSettButton.clear();

    delete ui;
}

void MainWindow::masterCodeCheck()
{
    if(settings->value("MASTER_CODE/ENABLE").toBool())
    {
        masterCodes = new QSettings("./masterCodes.ini", QSettings::IniFormat);
        QDate dateNext = masterCodes->value("LAST_DATE").toDate().addMonths(1);
        if(dateNext<QDate::currentDate())
        {
            bool stayOnFlag = true, exitFlag = false;
            QMessageBox msgBox;
            msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
            msgBox.setText(tr("You mast enter a master code!\n Please contact manufacturer to get a next one."));
            msgBox.setInformativeText(tr("To enter code press \"Yes\" \n"
                                      "To exit from program press \"No\"."));
            msgBox.setWindowTitle(tr("Master code"));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            int ret = msgBox.exec();
            switch (ret)
            {
            case QMessageBox::Yes:
                stayOnFlag = true;
                exitFlag = false;
                break;
            case QMessageBox::No:
                this->close();
                settings->setValue("PROGRAM/FINISH", true);
                exit(0);
                break;
            }

            for(;stayOnFlag;)
            {
                QString code = KeyboardDialog::getText(this, "Master Code");
                QByteArray codeArr;
                codeArr.append(code);
                if(codeArr.toHex() != masterCodes->value("CODES/INDEX_"
                                                         +QString::number(masterCodes->value("LAST_INDEX").toInt())))
                {
                    QMessageBox msgBox;
                    msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
                    msgBox.setText(tr("Wrong master code!\n Please contact manufacturer to get a next one."));
                    msgBox.setInformativeText(tr("To try typing again press \"Yes\" \n"
                                              "To exit press \"No\"."));
                    msgBox.setWindowTitle(tr("Master code"));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret)
                    {
                    case QMessageBox::Yes:
                        stayOnFlag = true;
                        exitFlag = false;
                        break;
                    case QMessageBox::No:
                        stayOnFlag = false;
                        exitFlag = true;
                        break;
                    }
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
                    msgBox.setText("Master code is correct!\n "
                                   "You mast enter next one at "
                                   +QDate::currentDate().addMonths(1).toString("dd MMMM yyyy"));

                    msgBox.setWindowTitle("Master code");
                    msgBox.setIcon(QMessageBox::Information);
                    msgBox.setStandardButtons(QMessageBox::Ok );
                    msgBox.exec();
                    masterCodes->setValue("LAST_DATE", QDate::currentDate());
                    masterCodes->setValue("LAST_INDEX", masterCodes->value("LAST_INDEX").toInt()+1);
                    if(masterCodes->value("LAST_INDEX").toInt() ==
                            masterCodes->value("ALL_COUNT").toInt())
                    {
                        settings->setValue("MASTER_CODE/ENABLE", false);
                    }
                    stayOnFlag = false;
                    exitFlag = false;
                }
            }

            if(exitFlag)
            {
                settings->setValue("PROGRAM/FINISH", true);
                this->close();
                exit(0);
            }
        }
    }
}

void MainWindow::headSettingRequest(int index)
{
    headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>());
    headSettingDialog->setHeadParams(index);
    headSettingDialog->move(this->pos().x()+this->width()-headSettingDialog->width()-30,
                            this->pos().y()+10);
    headSettingDialog->show();
}

void MainWindow::indexerLiftSettingRequest()
{
    indexerLiftSetDialog->setIndexerSetting();
    indexerLiftSetDialog->setLiftSetting();
    indexerLiftSetDialog->show();
    indexerLiftSetDialog->move(this->pos().x()+this->width()-indexerLiftSetDialog->width(),
                               this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
}

void MainWindow::generalSettingDialogRequest()
{
        machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
        generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS").value<EmailSettings>());
        generalSettingDialog->setStyleSheet(this->styleSheet());
        generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL").toInt(),
                                           settings->value("PASSWORDS/PASSWORD_LOCK_MAIL").toInt(),
                                           settings->value("PASSWORDS/PASSWORD_USERS").toInt());
        generalSettingDialog->showPortInfo(settings->value("COM_SETTING").value<ComSettings>());
        generalSettingDialog->setMachineSetting(machineSettings.machineParam);
        generalSettingDialog->show();
        generalSettingDialog->move(this->pos().x()+this->width()-generalSettingDialog->width(),
                                   this->pos().y()+10);
}

void MainWindow::changeHeadNo(int index)
{
    if(index<1)
    {
        if(this->machineSettings.machineParam.useUnloadHead)
            index = headsCount - 2;
        else
            index = headsCount - 1;
    }
    else
    {
        if(this->machineSettings.machineParam.useUnloadHead)
        {
            if(index > headsCount-2)
                index = 1;
        }
        else
        {
            if(index > headsCount-1)
                index = 1;
        }
    }
    this->headSettingRequest(index);
}

void MainWindow::resetMachine()
{
    ComSettings comSett = settings->value("COM_SETTING").value<ComSettings>();
    QByteArray bArr;

    if(needCompleteReset)
    {
        comSett.stringBaudRate = "19200";
        comSett.stringParity = "None";
        comSett.stringStopBits = "1";
        comSett.stringDataBits = "5";

        comPort->openSerialPort(comSett);

        bArr.append(static_cast<char>(0x75));//"u"
        bArr.append(static_cast<char>(0x78));//"x"
        bArr.append(static_cast<char>(0x75));//"u"
        bArr.append(static_cast<char>(0x72));//"r"
        comPort->sendData(bArr, false, true);
        QThread::msleep(500);
        bArr.clear();
        bArr.append(static_cast<char>(0x75));//"u"
        bArr.append(static_cast<char>(0x72));//"r"
        comPort->sendData(bArr, false, true);
        QThread::msleep(500);

        comPort->closeSerialPort();

        comPort->openSerialPort(settings->value("COM_SETTING").value<ComSettings>());

        needCompleteReset = false;
    }
    else
    {
        int data;
        bArr.clear();
        data = 1;
        bArr.append(MachineSettings::MasterDevice);
        bArr.append(MachineSettings::MasterLastButton);
        bArr[2] = (static_cast<char>(data>>8));
        bArr[3] = (static_cast<char>(data&0x00FF));
        data = CrcCalc::CalculateCRC16(bArr);
        bArr[4] = (static_cast<char>(data>>8));
        bArr[5] = (static_cast<char>(data&0x00FF));
        comPort->sendData(bArr);
        bArr.clear();
        bArr.append(static_cast<char>(MachineSettings::MasterDevice));
        bArr.append(static_cast<char>(MachineSettings::MasterMachineType));
    }
}

void MainWindow::getSerialData(ModData modData)
{
    int i;
    QByteArray bArr;
    watchDog->start();
    if(modData.fileds.adress<=HeadSetting::HeadDeviceAdrOffcet)
    {
        switch (modData.fileds.adress) {
        case MachineSettings::MasterDevice:
            switch (modData.fileds.registerNo) {
            case Register::masterReg_ACTIVHEAD_L:
//                headActDialog->setHeadActivState((headActDialog->getHeadActivState()|0x0000FFFF)
//                                                 &(modData.fileds.data|0xFFFF0000));
                break;
            case Register::masterReg_ACTIVHEAD_H:
//                headActDialog->setHeadActivState(((uint32_t)registers->readReg(MachineSettings::MasterDevice, Register::masterReg_ACTIVHEAD_L))
//                                                 |(((uint32_t)registers->readReg(MachineSettings::MasterDevice, Register::masterReg_ACTIVHEAD_H)<<16)&0xFFFF0000));
//                settings->setValue("HEAD/ACTIVATION", headActDialog->getHeadActivState());
//                qDebug()<<"Heads activation:"<<headActDialog->getHeadActivState();
                break;
            case Register::masterReg_EKR:
                infoWidget->setIndicatorState(modData.fileds.data);
                indexer->setState(modData.fileds.data);
                bArr.append(static_cast<char>(MachineSettings::MasterDevice));
                bArr.append(static_cast<char>(Register::masterReg_EKR));
                bArr.append(static_cast<char>(modData.fileds.data>>8));
                bArr.append(static_cast<char>(modData.fileds.data&0x00FF));
                udpHandler->sendData(bArr);
                if(modData.fileds.data == 0x0800)
                    this->indexerStepFinish();
                break;
            case Register::masterReg_paletStLow:
                for(i = 1; (i<16)&(i<headsCount); i++)
                    headButton[i]->setRagOn(registers->readReg(MachineSettings::MasterDevice, Register::masterReg_paletStLow)&(1<<i));
            break;
            case Register::masterReg_paletStHigh:
                for(i = 16; i<headsCount; i++)
                    headButton[i]->setRagOn(registers->readReg(MachineSettings::MasterDevice, Register::masterReg_paletStHigh)&(1<<(i-16)));
            break;
            case Register::masterReg_ERROR_MESSAGE:
                infoWidget->setErrorText(this->machineSettings.machineParam, 0);
            break;
            default:
                break;
            }
            break;
        case IndexerLiftSettings::IndexerDevice:
            switch (modData.fileds.registerNo) {
            case Register::indexerReg_ACC:
                indexerLiftSettings.indexerParam.acceleration = modData.fileds.data;
                break;
            case Register::indexerReg_RACC:
                indexerLiftSettings.indexerParam.accelerationRet = modData.fileds.data;
                break;
            case Register::indexerReg_DIST:
                indexerLiftSettings.indexerParam.distance = modData.fileds.data;
                break;
            case Register::indexerReg_DIST_OFFSET:
                indexerLiftSettings.indexerParam.distOffcet = modData.fileds.data;
                break;
            case Register::indexerReg_HOME_OFFSET:
                indexerLiftSettings.indexerParam.homeOffset = modData.fileds.data;
                break;
            case Register::indexerReg_SPEED:
                indexerLiftSettings.indexerParam.speed = modData.fileds.data;
                break;
            case Register::indexerReg_RSPEED:
                indexerLiftSettings.indexerParam.speedRet = modData.fileds.data;
                break;
            case Register::indexerliftReg_UP_DELAY:
                indexerLiftSettings.liftParam.delayUp = modData.fileds.data; //FUCKING DISASTER!!!!!!!!
                break;
            default:
                break;
            }
            break;

        case IndexerLiftSettings::LiftDevice:
            switch (modData.fileds.registerNo) {
            case Register::liftReg_ACC:
                indexerLiftSettings.liftParam.acceleration = modData.fileds.data;
                break;
            case Register::liftReg_DOWN_DELAY:
                indexerLiftSettings.liftParam.delayDown = modData.fileds.data;
                break;
            case Register::liftReg_DIST:
                indexerLiftSettings.liftParam.distance = modData.fileds.data;
                break;
            case Register::liftReg_HOME_OFFSET:
                indexerLiftSettings.liftParam.homeOffcet = modData.fileds.data;
                break;
            case Register::liftReg_SPEED:
                indexerLiftSettings.liftParam.speed = modData.fileds.data;
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
    else
    {
        int i;
        i = modData.fileds.adress - HeadSetting::HeadDeviceAdrOffcet;
        switch (modData.fileds.registerNo) {
        case Register::REG_RW_POWER:
            headSettings.headParam.dryPowerQ = modData.fileds.data;
            break;
        case Register::REG_SENSOR_TIME:
            headSettings.headParam.dryTimeQ = modData.fileds.data;
            break;
        case Register::headReg_FLDWE:
            headSettings.headParam.dwellFLTime = modData.fileds.data;
            break;
        case Register::headReg_SQDWE:
            headSettings.headParam.dwellSQTime = modData.fileds.data;
            break;
        case Register::headReg_ON:
            headSettings.headParam.headOnType = (HeadSetting::HeadOnType)modData.fileds.data;
            headSettings.headParam.powerOn = modData.fileds.data;
            break;
        case Register::REG_SHUTTLE_REAR_POS:
            headSettings.headParam.heatLimit = modData.fileds.data;
            break;
        case Register::headReg_RW1_TIME:
            headSettings.headParam.heatTime1Q = modData.fileds.data;
            break;
        case Register::headReg_RW2_TIME:
            headSettings.headParam.heatTime2Q = modData.fileds.data;
            break;
        case Register::headReg_RANGE_1:
            headSettings.headParam.limitFront = modData.fileds.data;
            break;
        case Register::headReg_RANGE_2:
            headSettings.headParam.limitRear = modData.fileds.data;
            break;
        case Register::headReg_SPD_FRONT:
            headSettings.headParam.speedFront = modData.fileds.data;
            break;
        case Register::headReg_SPD_REAR:
            headSettings.headParam.speedRear = modData.fileds.data;
            break;
        case Register::REG_STANDBY_POWER:
            headSettings.headParam.standbyPowerQ = modData.fileds.data;
            break;
        case Register::REG_STANDBY_TIME:
            headSettings.headParam.standbyTimeQ = modData.fileds.data;
            break;
        case Register::headReg_STR_COUNT:
            headSettings.headParam.stroksCount = modData.fileds.data;
            break;
        case Register::headReg_StBk_STR_COUNT:
            headSettings.headParam.stroksSBCount = modData.fileds.data;
            break;
        case Register::REG_TEMP_SET:
            headSettings.headParam.temperatureSetQ = modData.fileds.data;
            break;
        case Register::headReg_WARM_FLASH_TIME:
            headSettings.headParam.warmFlashTimeQ = modData.fileds.data;
            break;
        default:
            break;
        }
        if((i<headsCount)&(modData.fileds.registerNo==Register::headReg_ON))
        {
            if(headActDialog->getHeadActivAtIndex(i))
                switch ((HeadSetting::HeadOnType)(headSettings.headParam.headOnType&0x0F))
                {
                case HeadSetting::PrintHeadOn:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[1]);
                    break;
                case HeadSetting::QuartzHeadOn:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[2]);
                    break;
                case HeadSetting::InfraRedHeadOn:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[3]);
                    break;
                case HeadSetting::PrintHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[4]);
                    break;
                case HeadSetting::QuartzHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[5]);
                    break;
                case HeadSetting::InfraRedHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[6]);
                    break;
                }
            else
                headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[0]);
        }
    }
}

void MainWindow::getUdpData(QByteArray data)
{
    indexer->clickButton(data);
}

void MainWindow::getHeadColor(int index, QColor color)
{
    headButton[index]->setRagColor(color);
}

void MainWindow::getHeadParam(int index, QByteArray hParamArr)
{
    HeadSetting::setHeadOn_OffStateInd(index, static_cast<bool>(hParamArr[2]));
    settings->setValue(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
    if(headActDialog->getHeadActivAtIndex(index))
        switch ((HeadSetting::HeadOnType)(((0x00FF&((uint16_t)hParamArr[1]))<<8)|(0x00FF&((uint16_t)hParamArr[0]))))
        {
        case HeadSetting::PrintHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[1]);
            break;
        case HeadSetting::QuartzHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[2]);
            break;
        case HeadSetting::InfraRedHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[3]);
            break;
        case HeadSetting::PrintHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[4]);
            break;
        case HeadSetting::QuartzHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[5]);
            break;
        case HeadSetting::InfraRedHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[6]);
            break;
        }
    else
        headButton[index]->setPixmap(HeadForm::shirtOff,headStylesStr[0]);


}

void MainWindow::getAllHeadParam(int index, QByteArray hParamArr)
{
    int cnt;
    headSettings.fromByteArray(hParamArr);

    for(cnt = 1; cnt<headsCount; cnt++)
    {
        if((cnt<headsCount-1)|(!this->machineSettings.machineParam.useUnloadHead))
        {
            settings->setValue(QString("HEAD/HEAD_"+QString::number(cnt)+"_PARAM"), hParamArr);

            HeadSetting::setHeadOn_OffStateInd(cnt, static_cast<bool>(hParamArr[2]&0x01));

            uint16_t t_headOn = ((0x00FF&((uint16_t)hParamArr[1]))<<8)|(0x00FF&((uint16_t)hParamArr[0]));

            if(t_headOn != registers->readReg((HeadSetting::HeadDeviceAdrOffcet+cnt)&0x00FF,
                                               Register::headReg_ON))
            {
                QByteArray cmdArr;
                uint16_t data = cnt+500;
                cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
                cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
                cmdArr.append(static_cast<char>(data>>8));
                cmdArr.append(static_cast<char>(data&0x00FF));
                data = CrcCalc::CalculateCRC16(cmdArr);
                cmdArr.append(static_cast<char>(data>>8));
                cmdArr.append(static_cast<char>(data&0x00FF));
                comPort->sendData(cmdArr);
            }

            settings->setValue(QString("HEAD/HEAD_"+QString::number(cnt)+"_PARAM"), hParamArr);
            if(static_cast<bool>(t_headOn)>0)
                switch (static_cast<HeadSetting::HeadOnType>(t_headOn))
                {
                case HeadSetting::PrintHeadOn:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[1]);
                    break;
                case HeadSetting::QuartzHeadOn:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[2]);
                    break;
                case HeadSetting::InfraRedHeadOn:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[3]);
                    break;
                case HeadSetting::PrintHeadOff:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[4]);
                    break;
                case HeadSetting::QuartzHeadOff:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[5]);
                    break;
                case HeadSetting::InfraRedHeadOff:
                    headButton[cnt]->setPixmap(headButton[index]->getRagState(),headStylesStr[6]);
                    break;
                }
            else
                headButton[cnt]->setPixmap(HeadForm::shirtOff,headStylesStr[0]);
        }
    }
    QByteArray cmdArr;
    int data;

    uint8_t dev, addr;
    for(dev = 1; dev<headsCount; dev++)
        for(addr = Register::headReg_REG_DEV_INF_H; addr<Register::headReg_REG_DEV_INF_L; addr++)
            if(((dev<headsCount-1)|(!this->machineSettings.machineParam.useUnloadHead))&(dev != index))
            {
                data = registers->readReg(HeadSetting::HeadDeviceAdrOffcet+index, addr);
                if(registers->readReg(HeadSetting::HeadDeviceAdrOffcet+dev, addr) != data)
                {
                    cmdArr.append(static_cast<char>((HeadSetting::HeadDeviceAdrOffcet+dev)&0x00FF));
                    cmdArr.append(static_cast<char>(addr&0x00FF));
                    cmdArr.append(static_cast<char>(data>>8));
                    cmdArr.append(static_cast<char>(data&0x00FF));
                    data = CrcCalc::CalculateCRC16(cmdArr);
                    cmdArr.append(static_cast<char>(data>>8));
                    cmdArr.append(static_cast<char>(data&0x00FF));
                    comPort->sendData(cmdArr);
                    cmdArr.clear();
                }
            }

    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterHeadStateLo&0x00FF));
    cmdArr.append(static_cast<char>(HeadSetting::getHeadStateLo()>>8));
    cmdArr.append(static_cast<char>(HeadSetting::getHeadStateLo()&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
    cmdArr.clear();

    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterHeadStateHi&0x00FF));
    cmdArr.append(static_cast<char>(HeadSetting::getHeadStateHi()>>8));
    cmdArr.append(static_cast<char>(HeadSetting::getHeadStateHi()&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::getHeadCommand(int index, QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getHeadActivCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
    settings->setValue("HEAD/ACTIVATION", headActDialog->getHeadActivState());
}

void MainWindow::getCyclesCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getLoadState(LoadState stase)
{
    QByteArray cmdArr;
    int data;
    cmdArr.append(static_cast<char>((IndexerLiftSettings::IndexerDevice)&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LoadHeadState&0x00FF));
    cmdArr.append(static_cast<char>(stase>>8));
    cmdArr.append(static_cast<char>(stase&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::getIndexerParam(QByteArray indexerParamArr)
{
    settings->setValue(QString("INDEXER_PARAMS"), indexerParamArr);
//    comPort->sendData(indexerParamArr);

}

void MainWindow::getLiftParam(QByteArray liftParamArr)
{
    settings->setValue(QString("LIFT_PARAMS"), liftParamArr);
    //    comPort->sendData(liftParamArr);
}

void MainWindow::getLiftOffcet(double arg1)
{
    indexerLiftSetDialog->setLiftDistance(1.18-arg1, this->machineSettings.machineParam.liftGearRatio);
    this->indexerLiftSettings.liftParam.distance = (1.18-arg1)*100;
}

void MainWindow::getLiftDistance(float distance)
{
    disconnect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
    ui->dSpinBoxLiftOffcet->setValue(1.18-distance);
    connect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
}

void MainWindow::getIndexLiftSettComm(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getIndexLiftCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getMachineCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getMachineParam(QByteArray machineParamArr)
{
    settings->setValue("MACHINE_PARAMS", machineParamArr);
    MachineSettings mSett;
    mSett.fromByteArray(machineParamArr);
    ui->pButtonCyclesSetup->setVisible(mSett.machineParam.lastRevWarm.field.revolver);
    if(((this->headsCount-2 != mSett.machineParam.headCount)&(mSett.machineParam.useUnloadHead))
            |((this->headsCount-1 != mSett.machineParam.headCount)&(!mSett.machineParam.useUnloadHead))
            |(mSett.machineParam.useUnloadHead != this->machineSettings.machineParam.useUnloadHead))
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText(tr("Heads count changed"));
        msgBox.setInformativeText(tr("To apply count changing please restart a program\n"
                                  "Press OK to exit"));
        msgBox.setWindowTitle(tr("Info"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Save:
            headsCount = ((0x00FF&((uint16_t)machineParamArr[1]))<<8)|(0x00FF&((uint16_t)machineParamArr[0]));
            settings->setValue(QString("HEAD/HEADS_COUNT"), headsCount);
            this->exitProgram();
//            this->setHeadsPosition();
            break;
        case QMessageBox::Discard:
            machineParamArr[0] = headsCount&0x00FF;
            machineParamArr[1] = (headsCount>>8)&0x00FF;
            settings->setValue("MACHINE_PARAMS", machineParamArr);
            break;
        }
    }

//    comPort->sendData(machineParamArr);
    this->setHeadsPosition();


}

void MainWindow::getStepDelayTime(double arg1)
{
    this->machineSettings.machineParam.stepTimeDelay = arg1*10;
    settings->setValue(QString("MACHINE_PARAMS"), this->machineSettings.machineParam.toByteArray());
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexStepTimeDelay&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::getDirection(int direction)
{
    machineSettings.machineParam.direction = direction;
    this->setHeadsPosition();
}

void MainWindow::setUnloadState(bool state)
{
    if(state)
    {
        headButton[headsCount - 1]->setHeadformType(HeadForm::HeadProcessing);
    }
    else
    {
        headButton[headsCount - 1]->setHeadformType(HeadForm::HeadRemoving);

    }
}

void MainWindow::getSerialSetting(ComSettings comSett)
{
    settings->setValue("COM_SETTING", QVariant::fromValue(comSett));
    generalSettingDialog->raise();
    generalSettingDialog->setFocusLossAccept(true);
    generalSettingDialog->showPortInfo(comSett);
}

void MainWindow::getEmailSettings(EmailSettings emailSett)
{
    settings->setValue("EMAIL_SETTINGS", QVariant::fromValue(emailSett));
    mailSender->setSenderMailAdress(emailSett.senderAdress);
    mailSender->setSenderPassword(emailSett.senderPassword);
    mailSender->setRecipientMailAdress(emailSett.receiverAdress);
    mailSender->setEmailSubject(emailSett.emailSubject);
}

void MainWindow::getVeiwSettings(int stSheetIndex)
{
    settings->setValue("STYLE/STYLE_SEL_INDEX", stSheetIndex);
    this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"+QString::number(stSheetIndex))).toString());
    headSettingDialog->setStyleSheet(this->styleSheet());
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    comPort->setStyleSheet(this->styleSheet());
    ui->labelPalet->setStyleSheet("QLabel{padding-bottom: 0px; font: 16px bold italic large \"Serif\"}");
    ui->dSpinBoxLiftOffcet->setStyleSheet("QDoubleSpinBox{min-height: 50px;"
                                          "border-style: none;"
                                          "padding-top: 0px;"
                                          "font: 20px bold italic large \"Serif\"}"
                                          "QDoubleSpinBox::up-button {"
                                          "width: 45px;"
                                          "height: 55px;"
                                          "subcontrol-origin: content;"
                                          "subcontrol-position: right;"
                                          "}"
                                          "QDoubleSpinBox::down-button {"
                                          "width: 45px;"
                                          "height: 55px;"
                                          "subcontrol-origin: content;"
                                          "subcontrol-position: left;"
                                          "}"
                                          );
    ui->widgetLiftOffcet->setStyleSheet("border-style: outset; background-color: rgba(255, 255, 255, 0);");
    ui->dSpinBoxStepDelay->setStyleSheet(ui->dSpinBoxLiftOffcet->styleSheet());
    ui->widgetStepDelay->setStyleSheet(ui->widgetLiftOffcet->styleSheet());

    this->setBackGround(settings->value("STYLE/IMAGE_EN", false).toBool());
}

void MainWindow::getLangFile(int langIndex)
{
    settings->setValue("STYLE/LANG_SEL_INDEX", langIndex);
    qApp->removeTranslator(&translator);
    translator.load(settings->value("STYLE/LANG_PATH").toStringList()[langIndex]);
    ui->retranslateUi(this);
}

void MainWindow::serviceStateChange()
{
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD
    if(!logedInService){
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
    }
    if(logedInService || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_GENERAL")))
#endif
    {
        MachineSettings::setServiceWidgEn(true);
        logedInService = true;
        machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
        generalSettingDialog->setMachineSetting(machineSettings.machineParam);
        generalSettingDialog->move(this->pos().x()+this->width()-generalSettingDialog->width(),
                                   this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
        generalSettingDialog->raise();
        generalSettingDialog->show();
        generalSettingDialog->setFocusLossAccept(true);

    }
#ifndef DEBUG_BUILD
    else
    {
        MachineSettings::setServiceWidgEn(false);
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText(tr("Wrong password!"));
        msgBox.setWindowTitle(tr("Password"));
        msgBox.exec();
        generalSettingDialog->show();
        generalSettingDialog->setFocusLossAccept(true);

    }
#endif
}

void MainWindow::exitProgram(bool restart)
{
    if(!restart)
        this->exitCode = ExitDialog::tryExit(this);
    else
        this->exitCode = ExitDialog::RestartProgram;

    timeProgramEnd = QTime::currentTime();
    timeWorking.setHMS(0,0,0);
    timeWorking = timeWorking.addMSecs(timeProgramStart.msecsTo(timeProgramEnd));

    settings->setValue("COUNTERS/RAG_ALL_CNT", ragAllCount);
    settings->setValue("COUNTERS/INDEXER_ALL_CNT", indexerCyclesAll);

    settings->sync();
    if((this->exitCode != ExitDialog::Continue)&(this->exitCode != ExitDialog::ReprogramMachine))
    {
        comPort->closeSerialPort();
        QProgressDialog *pDialog = new QProgressDialog("Wait for machine shutdown.", "Cancel", 0, 30);
        pDialog->setWindowModality(Qt::WindowModal);
        pDialog->setStyleSheet(this->styleSheet());
        pDialog->setAutoClose(true);

        switch (this->exitCode) {
        case ExitDialog::ExitFromProgram:
            settings->setValue("PROGRAM/FINISH", true);
            this->close();
            break;
        case ExitDialog::RestartProgram:
            pDialog->setValue(0);
            settings->setValue("PROGRAM/FINISH", true);
            for (int i = pDialog->minimum(); i <= pDialog->maximum(); i++)
            {
                pDialog->setValue(i);
                QThread::msleep(200);
                if(pDialog->wasCanceled())
                    break;
            }
            if(!pDialog->wasCanceled())
                this->close();
            break;
        case ExitDialog::Shutdown:
            settings->setValue("PROGRAM/FINISH", true);
            if(settings->value("EMAIL_SETTINGS").value<EmailSettings>().mailEnable)
                mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                        "Worker: " + this->userName + ".\n"
                                        "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                        "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                                                                                         "background-color: rgba(255, 255, 255, 0);"
                                                                                                                                                       "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                        "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                        " and " + QString::number(ragAllCount) + " items in total.\n"
                                        "\nHave a great day!" );

            this->close();
            break;
        case ExitDialog::RestartMachine:
            if(settings->value("EMAIL_SETTINGS").value<EmailSettings>().mailEnable)
                mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                        "Machine restarting\n"
                                        "Worker: " + this->userName + ".\n"
                                        "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                        "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                        "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                        "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                        " and " + QString::number(ragAllCount) + " items in total.\n"
                                        "\nHave a great day!" );
            this->close();
            break;
        case ExitDialog::ServiceMode:
            break;
        default:
            break;
        }
    }
    else
        if(exitCode == ExitDialog::ReprogramMachine)
        {
            ComSettings comSett = settings->value("COM_SETTING").value<ComSettings>();
            comSett.stringBaudRate = "19200";
            comSett.stringParity = "None";
            comSett.stringStopBits = "1";
            comSett.stringDataBits = "5";
            comPort->openSerialPort(comSett);
            reprogramDialog->show();
        }
}

void MainWindow::saveJob()
{
    QString saveFileName = KeyboardDialog::getText(this, "Enter job name");
    saveFileName = QDir::homePath()+"/jobs/"+saveFileName+".ini";
    //QFileDialog::getSaveFileName(this, "Save job...",".","Setting file(*.ini)");
    QFile::copy(settings->fileName(), saveFileName);
}

void MainWindow::loadJob()
{
    int t_INDEXER_ALL_CNT = settings->value("INDEXER_ALL_CNT", 0).toInt();
    int t_RAG_ALL_CNT = settings->value("RAG_ALL_CNT", 0).toInt();

    delete settings;
    QString openFileName = QFileDialog::getOpenFileName(this, "Open job...",QDir::homePath()+"/jobs/","Setting file(*.ini)");
    settings = new QSettings(openFileName, QSettings::IniFormat);

    settings->setValue("INDEXER_ALL_CNT", t_INDEXER_ALL_CNT);
    settings->setValue("RAG_ALL_CNT", t_RAG_ALL_CNT);

    this->getLangFile(settings->value("STYLE/LANG_SEL_INDEX", 0).toInt());
    this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                          +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt()))).toString());
    this->machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
    generalSettingDialog->setMachineSetting(this->machineSettings.machineParam);

    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::WarningTime);
    comPort->sendReg(MachineSettings::MasterDevice, MachineSettings::MasterHeadCount);

    indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                      settings->value("LIFT_PARAMS").value<QByteArray>());

    registers->setMasterReg(this->machineSettings);
    registers->setIndexLiftReg(this->indexerLiftSettings);
    indexerLiftSetDialog->setIndexerSetting(indexerLiftSettings.indexerParam, false);
    indexerLiftSetDialog->setLiftSetting(indexerLiftSettings.liftParam, false);

    indexerLiftSetDialog->setLiftDistance(indexerLiftSettings.indexerParam.distance/100.f,
                                          this->machineSettings.machineParam.liftGearRatio);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexDistance);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexHomeOffset);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexDistOffcet);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexSpeed);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexAcceleration);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexSpeedRet);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexAccelerationRet);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftDelayDown);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftDelayUp);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftHomeOffcet);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftSpeed);
    comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftAcceleration);

    uint8_t i;

    for(i = 0; i < this->headsCount; i++)
    {
        headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
        registers->setHeadReg(i, headSettings);
        headSettingDialog->setHeadParams(headSettings.headParam, i, false);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadSpeedRear);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadRangeLimit1);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadSpeedFront);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadRangeLimit2);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadStroksCount);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadSBStroksCount);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashTime1Q);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashTime2Q);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashPowerWtoutIR);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashTimeStBy);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadHeatTemper);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashTime1Q);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashPowerStBy);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlashWarmTime);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadHeatTime1IR);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadHeatTime2IR);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadHeatDryRange);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadFlDwellTime);
        comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+i, HeadSetting::HeadSqDwellTime);
    }

    for(i = 0; i<headsCount; i++)
        {
        if(i==0)
            headButton[i]->setHeadformType(HeadForm::HeadPutingOn);
        else
            if((i==headsCount - 1)&(machineSettings.machineParam.useUnloadHead))
                headButton[i]->setHeadformType(HeadForm::HeadRemoving);
            else
                if((registers->readReg(HeadSetting::HeadDeviceAdrOffcet+i, Register::headReg_ON)==2)|
                        (registers->readReg(HeadSetting::HeadDeviceAdrOffcet+i, Register::headReg_ON)==4)|
                        (registers->readReg(HeadSetting::HeadDeviceAdrOffcet+i, Register::headReg_ON)==6))
                {
                    headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
                    switch (headSettings.headParam.headOnType&0x00FF)
                    {
                    case HeadSetting::PrintHeadOn:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[1]);
                        break;
                    case HeadSetting::QuartzHeadOn:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[2]);
                        break;
                    case HeadSetting::InfraRedHeadOn:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[3]);
                        break;
                    case HeadSetting::PrintHeadOff:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[4]);
                        break;
                    case HeadSetting::QuartzHeadOff:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[5]);
                        break;
                    case HeadSetting::InfraRedHeadOff:
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[6]);
                        break;
                    }
                }
                else
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[0]);
    }

}

void MainWindow::setHeadsPosition()
{
    int areaW, areaH;
    areaH = ui->widgetHeads->height();
    areaW = ui->widgetHeads->width();

    int i;
    float sinCoef, cosCoef, R, x0_hb, y0_hb, x0_sb, y0_sb;
    if(areaH<areaW)
        R = areaH/2-headButton[0]->height()/2-headSettButton[0]->height()/2-10;
    else
        R = areaW/2-headButton[0]->width()/2-headSettButton[0]->width()/2-10;

    x0_hb = ui->widgetHeads->width()/2-headButton[0]->width()/2;
    y0_hb = ui->widgetHeads->height()/2-headButton[0]->height()/2+headSettButton[0]->width()/2;
    x0_sb = ui->widgetHeads->width()/2-headSettButton[0]->width()/2;
    y0_sb = ui->widgetHeads->height()/2-headSettButton[0]->height()/2+headSettButton[0]->width()/2;

    int direction = machineSettings.machineParam.direction;

    for(i = 0; i<headsCount; i++)
    {
        sinCoef = sin(direction*2.*3.1415926*i/headsCount+3.1415926/2.
                      +direction*3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        cosCoef = cos(direction*2.*3.1415926*i/headsCount+3.1415926/2.
                      +direction*3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        headButton[i]->move(x0_hb+(R)*cosCoef, y0_hb+(R)*sinCoef);

        if((i != 0)&(((i != headsCount-1)&(machineSettings.machineParam.useUnloadHead))
                     |(!machineSettings.machineParam.useUnloadHead)))
        {
            headSettButton[i-1]->move(x0_sb+(R+headButton[i]->width()/2+headSettButton[i-1]->width()/2)*cosCoef,
                    y0_sb+(R+headButton[i]->height()/2+headSettButton[i-1]->width()/2)*sinCoef);
            if(direction == 1)
            {
                if(i<(headsCount)/4)
                    headButton[i]->setIndexLabelPosition(HeadForm::AtRightUp);
                else
                    if(i<(headsCount)/2)
                        headButton[i]->setIndexLabelPosition(HeadForm::AtRightDown);
                    else
                        if(i<(3*headsCount)/4)
                            headButton[i]->setIndexLabelPosition(HeadForm::AtLeftDown);
                        else
                            if(i<(headsCount))
                                headButton[i]->setIndexLabelPosition(HeadForm::AtLeftUp);
            }
            else
            {
                if(i<(headsCount)/4)
                    headButton[i]->setIndexLabelPosition(HeadForm::AtLeftUp);
                else
                    if(i<(headsCount)/2)
                        headButton[i]->setIndexLabelPosition(HeadForm::AtLeftDown);
                    else
                        if(i<(3*headsCount)/4)
                            headButton[i]->setIndexLabelPosition(HeadForm::AtRightDown);
                        else
                            if(i<(headsCount))
                                headButton[i]->setIndexLabelPosition(HeadForm::AtRightUp);
            }
        }
    }

    ui->widgetTopMenu->move(ui->widgetHeads->pos());
    ui->widgetTopMenu->resize(ui->widgetHeads->width()-18, 65);

    infoWidget->move(ui->widgetHeads->width()/2-infoWidget->width()/2, ui->widgetHeads->height()/2+18-infoWidget->height()/2);

    ui->widgetLiftOffcet->move(infoWidget->pos().x()+infoWidget->width()/2-ui->widgetLiftOffcet->width()/2,
                                 infoWidget->pos().y()-ui->widgetLiftOffcet->height());
    ui->widgetStepDelay->move(infoWidget->pos().x()+infoWidget->width()/2-ui->widgetLiftOffcet->width()/2,
                                 infoWidget->pos().y()+infoWidget->height()+6);
}

void MainWindow::indexerStepFinish()
{
    indexerCyclesAll++;
    indexerCyclesSession++;

    if(headButton[headButton.length()-1]->getRagState() == HeadForm::shirtOn)
    {
       ragAllCount++;
       infoWidget->setTotal(ragAllCount);
       ragSessionCount++;
       infoWidget->setPrinted(ragSessionCount);
       ragAtHeadCount--;
    }
    if(headButton[0]->getRagState() != HeadForm::shirtOff)
        ragAtHeadCount++;

    if(!indexer->getIsAutoPrint())
    {
        if(ragAtHeadCount > 0)
            indexer->printFinish();
    }
    maintanceDialog->check(indexerCyclesAll);
    if(ragAtHeadCount == 0)
    {
        indexer->printFinish();
    }
    infoWidget->setPrinted(ragSessionCount);
    infoWidget->setTotal(ragAllCount);
    QByteArray bArr;
    bArr.resize(14);
    bArr[0] = 0;
    bArr[1] = 20;
    static QTime lastTime;
    QTime curTime = QTime::currentTime();
    uint32_t dph = 0;
    if(((lastTime.secsTo(curTime))>0))
        dph = 3600000/lastTime.msecsTo(curTime);
    lastTime = curTime;
    int i;
    for(i = 0; i<4; i++)
    {
        bArr[2+i] = static_cast<char>(ragSessionCount>>(i*8));
        bArr[6+i] = static_cast<char>(ragAtHeadCount>>(i*8));
        bArr[10+i] = static_cast<char>(dph>>(i*8));
    }
    udpHandler->sendData(bArr);
}

void MainWindow::startPrintProcess(bool autoPrint)
{
    this->autoPrintEnabled = autoPrint;
}

void MainWindow::stopPrintProcess()
{

}

void MainWindow::maintanceWorkSlot(bool enable)
{
    ui->pButtonMaintance->setVisible(enable);
}

void MainWindow::setIconFolder(int index)
{
    QStringList list = settings->value("STYLE/ICON_PATH").value<QStringList>();
    if(list.length()>index)
    {
        QString path = list[index];
        settings->setValue("STYLE/ICON_SEL_INDEX", index);

        ui->pButtonExit->setIcon(QIcon(path+"/exit.png"));
        ui->pButtonSetting->setIcon(QIcon(path+"/settings.png"));
        ui->pButtonLoadJob->setIcon(QIcon(path+"/load.png"));
        ui->pButtonSaveJob->setIcon(QIcon(path+"/save.png"));
        ui->pButtonMaintance->setIcon(QIcon(path+"/warning.png"));
        ui->pButtonCyclesSetup->setIcon(QIcon(path+"/cycles.png"));


        indexer->setIconFolder(path);
        headSettingDialog->setIconFolder(path);
        generalSettingDialog->setIconFolder(path);


        int i;
        for(i = 0; i<headButton.length(); i++)
            headButton[i]->setIconPath(path);
        for(i = 0; i<headSettButton.length(); i++)
            headSettButton[i]->setIconPath(path+"/settings.png");

        infoWidget->setIconFolder(path);
    }
}

void MainWindow::setBackGround(bool enable, bool request)
{
    settings->setValue("STYLE/IMAGE_EN", enable);
    if(!enable)
    {
        this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                                    +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt()))).toString());
    }
    else
    {
        ui->widgetHeads->setStyleSheet("#widgetHeads{"
                                       "background-color: rgba(255, 255, 255, 0);"
                                       "border-style: none;}");
        QString imgName;
        if(request)
        {
            imgName = QFileDialog::getOpenFileName(this, "Select background..", QDir::homePath(), "Images (*.png *.xpm)");
            settings->setValue("STYLE/IMAGE_PATH", imgName);
        }
        else
            imgName = settings->value("STYLE/IMAGE_PATH").toString();

        this->setStyleSheet(this->styleSheet()+
                            "#centralWidget {"
                            "background-image: url(\""+
                            imgName+
                            "\") 0 0 0 0 stretch stretch;}"
                            );
        ui->widgetLiftOffcet->setStyleSheet(ui->pButtonExit->styleSheet());
        ui->widgetStepDelay->setStyleSheet(ui->pButtonExit->styleSheet());
        ui->pButtonCyclesSetup->setStyleSheet(ui->pButtonExit->styleSheet());
        ui->pButtonMaintance->setStyleSheet(ui->pButtonExit->styleSheet());
//        ui->widgetLiftOffcet->setStyleSheet("border-style: none; background-color: rgba(255, 255, 255, 0);");
//        ui->widgetStepDelay->setStyleSheet("border-style: none; background-color: rgba(255, 255, 255, 0);");
    }
}

void MainWindow::userLogin()
{
    if(usersSettingDialog->getLoginWindowEnable())
    {
        LoginDialog loginDialog;
        loginDialog.setUserNames(usersSettingDialog->getUserNames());
        loginDialog.setStyleSheet(this->styleSheet()
                                  +"*{color: white; font: 24px bold italic large}"
                                  +"QPushButton {min-width: 70px; min-height: 55px}");
        bool stayOnFlag = true, exitFlag = false;
        for(;stayOnFlag;)
        {
            loginDialog.exec();
            if(usersSettingDialog->isUser(loginDialog.userName, loginDialog.userPassword))
            {
                stayOnFlag = false;
                this->userName = loginDialog.userName;
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
                msgBox.setText(tr("Wrong user password"));
                msgBox.setInformativeText(tr("To try login again press \"Yes\" \n"
                                          "To exit press \"No\"."));
                msgBox.setWindowTitle(tr("Login"));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int ret = msgBox.exec();
                switch (ret)
                {
                case QMessageBox::Yes:
                    stayOnFlag = true;
                    exitFlag = false;
                    break;
                case QMessageBox::No:
                    stayOnFlag = false;
                    exitFlag = true;
                    break;
                }
            }
        }

        if(exitFlag)
        {
            this->close();
            exit(0);
        }
    }
    else
        this->userName = tr("Default_user");

}

void MainWindow::zeroStart()
{
    if(!settings->value("PROGRAM/FINISH", false).toBool())
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText(tr("Exit incorrect"));
        msgBox.setInformativeText(tr("Machine shutdown during\n"
                                     " the last session is incorrect.\n"
                                     "Use the Exit menu to prevent errors.\n"));
        msgBox.setWindowTitle(tr("Exit error"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    settings->setValue("PROGRAM/FINISH", false);

    if(((!QSslSocket::supportsSsl())|
            (!(QSslSocket::sslLibraryBuildVersionString() == QSslSocket::sslLibraryVersionString())))
            &(settings->value("EMAIL_SETTINGS").value<EmailSettings>().mailEnable))
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText(tr("Wrong ssl library."));
        msgBox.setInformativeText(tr("E-mail will not be sended\n"
                                  "to continue press \"Yes\" \n"
                                  "To exit press \"No\"."));
        msgBox.setWindowTitle(tr("E-mail"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
            break;
        case QMessageBox::No:
            this->exitProgram(false);
            break;
        }
    }

    needCompleteReset = true;

    MachineSettings::serviceWidgetsEn = false;
    this->exitCode = ExitDialog::Continue;

    logedInHeadSettings = false;
    logedInIndexer = false;
    logedInGeneral = false;
    logedInService = false;

    ragSessionCount = 0;
    ragAllCount = settings->value("COUNTERS/RAG_ALL_CNT", 0).toInt();
    indexerCyclesSession = 0;
    indexerCyclesAll = settings->value("COUNTERS/INDEXER_ALL_CNT", 0).toInt();
    ragAtHeadCount = 0;

    infoWidget->setTotal(ragAllCount);

    infoWidget->setPrinted(ragSessionCount);

    serviceCounter = 0;

    ui->widgetTopMenu->setStyleSheet("border-style: none; background-color: rgba(255, 255, 255, 0);");

    ui->labelPalet->setStyleSheet("QLabel{font: 12px bold italic large \"Serif\"}");

    disconnect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
    ui->dSpinBoxLiftOffcet->setValue(1.18-this->indexerLiftSettings.liftParam.distance/100.);
    connect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
    ui->dSpinBoxStepDelay->setValue(this->machineSettings.machineParam.stepTimeDelay/10.);

    ui->dSpinBoxLiftOffcet->setStyleSheet("QDoubleSpinBox{min-height: 50px;"
                                          "padding-top: 0px;"
                                          "border-style: none;"
                                          "font: 16px bold italic large \"Serif\"}"
                                          "QDoubleSpinBox::up-button {"
                                          "width: 45px;"
                                          "height: 55px;"
                                          "subcontrol-origin: content;"
                                          "subcontrol-position: right;"
                                          "}"
                                          "QDoubleSpinBox::down-button {"
                                          "width: 45px;"
                                          "height: 50px;"
                                          "subcontrol-origin: content;"
                                          "subcontrol-position: left;"
                                          "}"
                                          );
    ui->widgetLiftOffcet->setStyleSheet("border-style: outset; background-color: rgba(255, 255, 255, 0);");

    ui->labelDelay->setStyleSheet("QLabel{padding-bottom: 0px; font: 12px bold italic large \"Serif\"}");
    ui->dSpinBoxStepDelay->setStyleSheet(ui->dSpinBoxLiftOffcet->styleSheet());
    ui->widgetStepDelay->setStyleSheet(ui->widgetLiftOffcet->styleSheet());

    timeProgramStart = QTime::currentTime();

    watchDog->start();

}

void MainWindow::headsInit()
{
    if(this->machineSettings.machineParam.useUnloadHead)
        headsCount+=2;
    else
        headsCount+=1;

    headActDialog = new HeadActivationDialog(headsCount, this);
    headActDialog->setHeadActivState(settings->value("HEAD/ACTIVATION", 0).toInt());
    connect(generalSettingDialog, SIGNAL(headActivationRequest()), headActDialog, SLOT(show()));
    connect(headActDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getHeadActivCommand(QByteArray)));

    int i;
    for(i = 0; i<headsCount; i++)
    {
        headButton.append(new HeadForm(ui->widgetHeads));
        headButton[i]->setIndex(i);
        if(i==0)
        {
            headButton[i]->setHeadformType(HeadForm::HeadPutingOn);
            connect(headButton[i], SIGNAL(loadStateChanged(LoadState)), this, SLOT(getLoadState(LoadState)));
        }
        else
            if((i==headsCount - 1)&(this->machineSettings.machineParam.useUnloadHead))
                headButton[i]->setHeadformType(HeadForm::HeadRemoving);
            else
            {
                headButton[i]->setPixmap(HeadForm::shirtOff,headStylesStr[0]);
                QColor col;
                headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
                col.setRed((headSettings.headParam.inkColor&0x000000FF)>>0);
                col.setGreen((headSettings.headParam.inkColor&0x0000FF00)>>8);
                col.setBlue((headSettings.headParam.inkColor&0x00FF0000)>>16);
                headButton[i]->setRagColor(col);
            }
        HeadSetting::setHeadOn_OffStateInd(i, static_cast<bool>(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[2]&0x01));

        if((i != 0)&(((i != headsCount-1)&(machineSettings.machineParam.useUnloadHead))
                     |((i != headsCount)&(!machineSettings.machineParam.useUnloadHead))))
        {
            headSettButton.append(new HeadSettingButton(i, ui->widgetHeads));
            if(i<(headsCount)/4)
                headButton[i]->setIndexLabelPosition(HeadForm::AtRightUp);
            if((i>=(headsCount)/4)&(i<(headsCount)/2))
                headButton[i]->setIndexLabelPosition(HeadForm::AtRightDown);
            if((i>=(headsCount)/2)&(i<(3*headsCount)/4))
                headButton[i]->setIndexLabelPosition(HeadForm::AtLeftDown);
            if((i>=(3*headsCount)/4)&(i<(headsCount)))
                headButton[i]->setIndexLabelPosition(HeadForm::AtLeftUp);
            connect(headSettButton[i-1], SIGNAL(settingButtonCliced(int)), this, SLOT(headSettingRequest(int)));
        }
    }
}

void MainWindow::watchDogTimeout()
{
//    qDebug()<<"WatchDog handler call";
//    needCompleteReset = true;
//    indexer->setState(0x0);
//    infoWidget->setIndicatorState(0x704);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    this->setHeadsPosition();
    e->accept();
}

void MainWindow::showEvent(QShowEvent *ev)
{
    this->setHeadsPosition();
    maintanceDialog->check(indexerCyclesAll);
    ui->pButtonCyclesSetup->setVisible(this->machineSettings.machineParam.lastRevWarm.field.revolver);
    this->setIconFolder(settings->value("STYLE/ICON_SEL_INDEX").toInt());
    this->setBackGround(settings->value("STYLE/IMAGE_EN", false).toBool());
    this->resetMachine();
    ev->accept();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if((ev->type() == QMouseEvent::MouseButtonRelease)&(obj==ui->widgetHeads))
    {
        serviceCounter++;
        if(serviceCounter>10)
            MachineSettings::setServiceWidgEn(true);
    }
    if(((ev->type() == QMouseEvent::MouseButtonDblClick)
        |((QApplication::platformName() == "eglfs")&(ev->type()==QEvent::MouseButtonRelease))
        |((QApplication::platformName() == "linuxfb")&(ev->type()==QEvent::MouseButtonRelease)))
            &((obj->parent()==ui->dSpinBoxLiftOffcet)|(obj->parent()==ui->dSpinBoxStepDelay)))
    {
        qobject_cast<QDoubleSpinBox*>(obj->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(obj->parent())->clearFocus();
    }
    return false;
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}
