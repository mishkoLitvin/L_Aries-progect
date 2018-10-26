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

    settings = new QSettings("./settingsPATH.ini", QSettings::IniFormat);
    QString setPath = settings->value("LAST_PATH", "./settings.ini").toString();
    delete settings;
    settings = new QSettings(setPath, QSettings::IniFormat);

    this->getLangFile(settings->value("STYLE/LANG_SEL_INDEX", -1).toInt());
    this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                          +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt())), QStringList("Empty")).toString());
    ui->widgetHeads->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    headsCount = settings->value(QString("HEAD/HEADS_COUNT"), 4).toInt();

    comPort = new SerialPort(settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>(),this);
    comPort->setStyleSheet(this->styleSheet());
    connect(comPort, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));
    connect(comPort, SIGNAL(dataReady(ModData)), this, SLOT(getSerialData(ModData)));
    connect(comPort, SIGNAL(initFinihed()), this, SLOT(setHeadsPosition()));

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

    warming = new WarmingWidget(this);
    ui->layoutIndexer->addWidget(warming);
    warming->setVisible(false);
    connect(warming, SIGNAL(sendCommand(QByteArray)), this, SLOT(getWarmingCommand(QByteArray)));

    indexerLiftSetDialog = new IndexerSettingDialog();
    indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS", QVariant::fromValue(this->indexerLiftSettings.indexerParam.toByteArray())).value<QByteArray>(),
                                      settings->value("LIFT_PARAMS", QVariant::fromValue(this->indexerLiftSettings.liftParam.toByteArray())).value<QByteArray>());
    connect(indexerLiftSetDialog, SIGNAL(indexerParamChanged(QByteArray)), this, SLOT(getIndexerParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(liftParamChanged(QByteArray)), this, SLOT(getLiftParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getIndexLiftSettComm(QByteArray)));
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());

    generalSettingDialog = new GeneralSettingDialog();
    machineSettings.fromByteArray(settings->value("MACHINE_PARAMS", machineSettings.machineParam.toByteArray()).value<QByteArray>());
    generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS").value<EmailSettings>());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL", 35142).toInt(),
                                       settings->value("PASSWORDS/PASSWORD_LOCK_MAIL", 31046).toInt(),
                                       settings->value("PASSWORDS/PASSWORD_USERS", 30726).toInt());
    generalSettingDialog->setStyle(settings->value("STYLE/STYLE_LIST", QStringList("Empty")).value<QStringList>(),
                                       settings->value("STYLE/STYLE_SEL_INDEX", 0).toInt(),
                                       settings->value("STYLE/ICON_LIST", QStringList("Empty")).value<QStringList>(),
                                       settings->value("STYLE/ICON_SEL_INDEX", 0).toInt(),
                                       settings->value("STYLE/IMAGE_EN", false).toBool());
    generalSettingDialog->setLangList(settings->value("STYLE/LANG_LIST", QStringList("Empty")).value<QStringList>(),
                                      settings->value("STYLE/LANG_SEL_INDEX", 0).toInt());
    generalSettingDialog->showPortInfo(settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>());
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
    connect(generalSettingDialog, &GeneralSettingDialog::warmingStateChanged, this, &MainWindow::getWarmingEnable);
    connect(generalSettingDialog, &GeneralSettingDialog::skipStateChanged, this, &MainWindow::skipStateUpdate);
    MachineSettings::setSoftwartSkipEn(settings->value("PROGRAM/SOFT_SKIP", false).toBool());
    MachineSettings::setHeadMaxRange(machineSettings.machineParam.headMaxRange);
    MachineSettings::setHeadType(machineSettings.machineParam.headType.all);
    MachineSettings::setIndexLiftType(machineSettings.machineParam.indexeLiftType.all);
    indexerLiftSetDialog->setLiftGearRatio(this->machineSettings.machineParam.liftGearRatio);

    mailSender = new MailSender(this);
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS", QVariant::fromValue(EmailSettings())).value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS", QVariant::fromValue(EmailSettings())).value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS", QVariant::fromValue(EmailSettings())).value<EmailSettings>().receiverAdress);

    connect(ui->pButtonExit, SIGNAL(clicked(bool)), this, SLOT(exitProgram()));
    connect(ui->pButtonSaveJob, SIGNAL(clicked(bool)), this, SLOT(saveJob()));
    connect(ui->pButtonLoadJob, SIGNAL(clicked(bool)), this, SLOT(loadJob()));

    int i;
    QStringList stList;
    stList.append("background-color:#AAAAAA;");
    stList.append("background-color:#0000AA;"); stList.append("background-color:#AA00AA;"); stList.append("background-color:#AA0000;");
    stList.append("background-color:#8888AA;"); stList.append("background-color:#AA88AA;"); stList.append("background-color:#AA8888;");
    for(i = 0; i<7; i++)
        headStylesStr.append(settings->value("STYLE/HEAD_STYLES_"+QString::number(i), stList[i]).toString());
    this->headsInit();

    infoWidget = new InfoWidget(ui->widgetHeads);

    if((QApplication::platformName() != "eglfs")&(QApplication::platformName() != "linuxfb"))
        this->resize(QSize(1024, 768));
    else
        this->setWindowState(Qt::WindowMaximized);

    cycleDialog = new CyclesDialog(this->headsCount, this);
    connect(ui->pButtonCyclesSetup, SIGNAL(clicked(bool)), cycleDialog, SLOT(show()));
    connect(cycleDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getCyclesCommand(QByteArray)));

    this->setHeadsPosition();

    maintanceDialog = new MaintanceDialog(this);
    maintanceDialog->setStyleSheet(this->styleSheet());
    connect(maintanceDialog, SIGNAL(stopRequest()), indexer, SLOT(printFinish()));
    connect(maintanceDialog, SIGNAL(continueRequest()), indexer, SLOT(printStart()));
    connect(maintanceDialog, SIGNAL(maintanceWorkEnable(bool)), this, SLOT(maintanceWorkSlot(bool)));

    maintanceWidget = new MaintanceWidget(this);
    connect(ui->pButtonMaintance, SIGNAL(clicked(bool)), this, SLOT(showMaintananceWidget()));
    connect(maintanceWidget, SIGNAL(troubleSolved(int)), maintanceDialog, SLOT(solveItem(int)));

//    connect(ui->pButtonMaintance, SIGNAL(clicked(bool)), maintanceDialog, SLOT(openMaintanceList()));
//    connect(indexer, SIGNAL(stopPrint()), maintanceDialog, SLOT(openDialog()));

    registers = new Register(headsCount);

    registers->setMasterReg(this->machineSettings);
    registers->setIndexLiftReg(this->indexerLiftSettings);
    comPort->setRegisterPointer(this->registers);
    generalSettingDialog->setRegisterPointer(this->registers);
    infoWidget->setRegisterPointer(this->registers);
    indexerLiftSetDialog->setRegisters(this->registers);
    headSettingDialog->setRegisters(this->registers);
    warming->setRegisters(this->registers);

    for(i = 0; i < this->headsCount; i++)
    {
        headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM"), HeadSetting().headParam.toByteArray()).value<QByteArray>());
        registers->setHeadReg(i, headSettings);
    }

    udpHandler = new UdpSocket(this);
    udpHandler->startUdp(true);
    connect(udpHandler, SIGNAL(dataReady(QByteArray)), this, SLOT(getUdpData(QByteArray)));

    ui->widgetHeads->installEventFilter(this);
    ui->dSpinBoxLiftOffcet->children()[0]->installEventFilter(this);
    connect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
    connect(indexerLiftSetDialog, SIGNAL(liftDistanceChanged(float)), this, SLOT(getLiftDistance(float)));
    ui->dSpinBoxStepDelay->children()[0]->installEventFilter(this);
    connect(ui->dSpinBoxStepDelay, SIGNAL(valueChanged(double)), this, SLOT(getStepDelayTime(double)));

    watchDog = new QTimer(this);
    watchDog->setInterval(10000);
    connect(watchDog, SIGNAL(timeout()), this, SLOT(watchDogTimeout()));
    connect(comPort, SIGNAL(working()), watchDog, SLOT(start()));

    updateTime = new QTimer(this);
    connect(updateTime, SIGNAL(timeout()), this, SLOT(updateTimeSlot()));

    this->zeroStart();

    reprogramDialog = new ReprogramDialog(this);
    connect(reprogramDialog, &ReprogramDialog::programArrReady, comPort, &SerialPort::sendProgram);
    connect(comPort, &SerialPort::proramProgres, reprogramDialog, &ReprogramDialog::setProgress);
    connect(reprogramDialog, &ReprogramDialog::reprogramDialogFinished, this, &MainWindow::watchDogTimeout);

    counterDialog = new CountersDialog(this);
    counterDialog->setRemaining(settings->value("COUNTERS/REMAINING", 0).toInt());
    infoWidget->setRemaining(0);
    connect(counterDialog, &CountersDialog::resetRemaining, this, &MainWindow::resetRemainingRequest);
    connect(counterDialog, &CountersDialog::resetSkipped, this, &MainWindow::resetSkippedRequest);
    connect(counterDialog, &CountersDialog::remainingValChanged, this, &MainWindow::remainingValChangedRequest);
    connect(generalSettingDialog, &GeneralSettingDialog::countersDialogRequest, this, &MainWindow::counterDialogRequest);
    counterDialog->hide();

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
            msgBox.setText(tr("You mast enter a master code number ")
                              +QString::number(masterCodes->value("LAST_INDEX").toInt())
                              +tr("\n Please contact manufacturer to get a next one."));
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
    headSettingDialog->setHeadParams(index);
    headSettingDialog->move(this->pos().x()+this->width()-headSettingDialog->width()-30,
                            this->pos().y()+10);
    headSettingDialog->show();
    this->lastOpened = headSettingDialog;
}

void MainWindow::indexerLiftSettingRequest()
{
    indexerLiftSetDialog->setIndexerSetting();
    indexerLiftSetDialog->setLiftSetting();
    indexerLiftSetDialog->show();
    indexerLiftSetDialog->move(this->pos().x()+this->width()-indexerLiftSetDialog->width(),
                               this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
    this->lastOpened = indexerLiftSetDialog;
}

void MainWindow::generalSettingDialogRequest()
{
    machineSettings.fromByteArray(settings->value("MACHINE_PARAMS", this->machineSettings.machineParam.toByteArray()).value<QByteArray>());
    generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS", QVariant::fromValue(EmailSettings())).value<EmailSettings>());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL", 35142).toInt(),
                                       settings->value("PASSWORDS/PASSWORD_LOCK_MAIL", 31046).toInt(),
                                       settings->value("PASSWORDS/PASSWORD_USERS", 30726).toInt());
    generalSettingDialog->showPortInfo(settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>());
    generalSettingDialog->setMachineSetting(machineSettings.machineParam);

    generalSettingDialog->manualShow();
    generalSettingDialog->move(this->pos().x()+this->width()-generalSettingDialog->size().width(),
                               this->pos().y()+10);
    this->lastOpened = generalSettingDialog;
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
    ComSettings comSett = settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>();
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

        comPort->openSerialPort(settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>());

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
    uint8_t hb;
    bool idle;

    watchDog->start();
    if(modData.fileds.adress<=HeadSetting::HeadDeviceAdrOffcet)
    {
        switch (modData.fileds.adress) {
        case MachineSettings::MasterDevice:
            switch (modData.fileds.registerNo) {
            case Register::masterReg_ACTIVHEAD_L:
                headActDialog->setHeadActivState((headActDialog->getHeadActivState()|0x0000FFFF)
                                                 &(modData.fileds.data|0xFFFF0000));
                break;
            case Register::masterReg_ACTIVHEAD_H:
                headActDialog->setHeadActivState(((uint32_t)registers->readReg(MachineSettings::MasterDevice, Register::masterReg_ACTIVHEAD_L))
                                                 |(((uint32_t)registers->readReg(MachineSettings::MasterDevice, Register::masterReg_ACTIVHEAD_H)<<16)&0xFFFF0000));
                settings->setValue("HEAD/ACTIVATION", headActDialog->getHeadActivState());
//                qDebug()<<"Heads activation:"<<headActDialog->getHeadActivState();
                break;
            case Register::masterReg_EKR:
                infoWidget->setIndicatorState(modData.fileds.data);
                indexer->setState(modData.fileds.data);
                hb = ((modData.fileds.data>>8)&0x00FF);
                if((((hb == 0)|(hb == 17)|(hb == 18)|(hb == 2)))&(ui->pButtonWarming->isChecked()))
                    ui->pButtonWarming->click();
                if((hb == 7)&(ui->pButtonWarming->isChecked()))
                    warming->resetWidget();
                idle = MachineSettings::getMachineIdle();
                ui->pButtonLoadJob->setEnabled(idle);
                ui->pButtonSaveJob->setEnabled(idle);
                ui->pButtonExit->setEnabled(idle);
                ui->pButtonSetting->setEnabled(idle);
                ui->pButtonWarming->setEnabled(idle);
                if(modData.fileds.data == 0x0800)
                    this->indexerStepFinish();
                break;
            case Register::masterReg_paletStLow:
                MachineSettings::setHeadPalStateLo(registers->readReg(MachineSettings::MasterDevice, Register::masterReg_paletStLow));
                if(machineSettings.machineParam.direction == 1)
                {
                    for(i = 1; (i<32)&(i<headsCount); i++)
                        if((static_cast<bool>(MachineSettings::getHeadPalState()&(1<<i))) !=
                                static_cast<bool>(headButton[headsCount-i-machineSettings.machineParam.useUnloadHead]->getRagState() == HeadForm::shirtOn))
                            headButton[headsCount-i-machineSettings.machineParam.useUnloadHead]->setRagOn(MachineSettings::getHeadPalState()&(1<<i));
                }
                else
                {
                    for(i = 1; (i<32)&(i<headsCount); i++)
                        if( (static_cast<bool>(MachineSettings::getHeadPalState()&(1<<i))) !=
                                (headButton[i]->getRagState() == HeadForm::shirtOn))
                            headButton[i]->setRagOn(MachineSettings::getHeadPalState()&(1<<i));
                }
                break;
            case Register::masterReg_paletStHigh:
                MachineSettings::setHeadPalStateHi(registers->readReg(MachineSettings::MasterDevice, Register::masterReg_paletStHigh));
                if(machineSettings.machineParam.direction == 1)
                {
                    for(i = 1; (i<32)&(i<headsCount); i++)
                        if( (static_cast<bool>(MachineSettings::getHeadPalState()&(1<<i))) !=
                                (headButton[headsCount-i-machineSettings.machineParam.useUnloadHead]->getRagState() == HeadForm::shirtOn))
                            headButton[headsCount-i-machineSettings.machineParam.useUnloadHead]->setRagOn(MachineSettings::getHeadPalState()&(1<<i));
                }
                else
                {
                    for(i = 1; (i<32)&(i<headsCount); i++)
                        if( (static_cast<bool>(MachineSettings::getHeadPalState()&(1<<i))) !=
                                (headButton[i]->getRagState() == HeadForm::shirtOn))
                            headButton[i]->setRagOn(MachineSettings::getHeadPalState()&(1<<i));
                }
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
                indexerLiftSettings.indexerParam.distOffcet = static_cast<int16_t>(modData.fileds.data);
                break;
            case Register::indexerReg_HOME_OFFSET:
                indexerLiftSettings.indexerParam.homeOffset = static_cast<int16_t>(modData.fileds.data);
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
            case Register::indexerReg_DIR:
                if(modData.fileds.data == 0)
                    machineSettings.machineParam.direction = -1;
                else
                    machineSettings.machineParam.direction = 1;
                settings->setValue("MACHINE_PARAMS", machineSettings.machineParam.toByteArray());
                break;
            case Register::liftReg_ACC:
                indexerLiftSettings.liftParam.acceleration = modData.fileds.data;
                break;
            case Register::liftReg_DIST:
                indexerLiftSettings.liftParam.distance = modData.fileds.data;
                disconnect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
                ui->dSpinBoxLiftOffcet->setValue(1.18-modData.fileds.data/100.);
                connect(ui->dSpinBoxLiftOffcet, SIGNAL(valueChanged(double)), this, SLOT(getLiftOffcet(double)));
                break;
            case Register::liftReg_HOME_OFFSET:
                indexerLiftSettings.liftParam.homeOffcet = static_cast<int16_t>(modData.fileds.data);
                break;
            case Register::liftReg_SPEED:
                indexerLiftSettings.liftParam.speed = modData.fileds.data;
                break;
            default:
                break;
            }
            break;

        case IndexerLiftSettings::LiftDevice:
            switch (modData.fileds.registerNo) {

            case Register::liftReg_DOWN_DELAY:
                indexerLiftSettings.liftParam.delayDown = modData.fileds.data;
                break;
            case Register::REG_SKIPC_H:
                infoWidget->setSkipped((static_cast<uint32_t>(registers->readReg(IndexerLiftSettings::LiftDevice, Register::REG_SKIPC_L))&0x0000FFFF)|
                                       ((static_cast<uint32_t>(registers->readReg(IndexerLiftSettings::LiftDevice, Register::REG_SKIPC_H))<<16)&0xFFFF0000));
                break;
            case Register::REG_SKIPC_L:
                infoWidget->setSkipped((static_cast<uint32_t>(registers->readReg(IndexerLiftSettings::LiftDevice, Register::REG_SKIPC_L))&0x0000FFFF)|
                                       ((static_cast<uint32_t>(registers->readReg(IndexerLiftSettings::LiftDevice, Register::REG_SKIPC_H))<<16)&0xFFFF0000));
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
        if(machineSettings.machineParam.direction == 1)
            i = headsCount-machineSettings.machineParam.useUnloadHead - i;
        switch (modData.fileds.registerNo) {
        case Register::REG_RW_POWER:
            headSettings.headParam.dryPowerQ = modData.fileds.data;
            headButton[i]->setDryPower(headSettings.headParam.dryPowerQ);
            headButton[i]->setStepBkStrCnt(0);
            break;
        case Register::REG_SENSOR_TIME:
            headSettings.headParam.dryTimeQ = modData.fileds.data;
            break;
        case Register::headReg_FLDWE:
            headSettings.headParam.dwellFLTime = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_SQDWE:
            headSettings.headParam.dwellSQTime = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_ON:
            headSettings.headParam.headOnType = (HeadSetting::HeadOnType)modData.fileds.data;
            headSettings.headParam.powerOn = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
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
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_RANGE_2:
            headSettings.headParam.limitRear = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_SPD_FRONT:
            headSettings.headParam.speedFront = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_SPD_REAR:
            headSettings.headParam.speedRear = modData.fileds.data;
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::REG_STANDBY_POWER:
            headSettings.headParam.standbyPowerQ = modData.fileds.data;
            break;
        case Register::REG_STANDBY_TIME:
            headSettings.headParam.standbyTimeQ = modData.fileds.data;
            break;
        case Register::headReg_STR_COUNT:
            headSettings.headParam.stroksCount = modData.fileds.data;
            headButton[i]->setStrokCount(headSettings.headParam.stroksCount);
            if(this->lastOpened == headSettingDialog)
                headSettingDialog->setHeadParams(i);
            break;
        case Register::headReg_StBk_STR_COUNT:
            headSettings.headParam.stroksSBCount = modData.fileds.data;
            headButton[i]->setStepBkStrCnt(headSettings.headParam.stroksSBCount);
            break;
        case Register::REG_TEMP_MEAS:
            headButton[i]->setTemperature(modData.fileds.data);
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
                    headButton[i]->setStrokCount(headSettings.headParam.stroksCount);
                    headButton[i]->setStepBkStrCnt(headSettings.headParam.stroksSBCount);
                    break;
                case HeadSetting::QuartzHeadOn:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[2]);
                    headButton[i]->setDryPower(headSettings.headParam.dryPowerQ);
                    break;
                case HeadSetting::InfraRedHeadOn:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[3]);
                    break;
                case HeadSetting::PrintHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[4]);
                    headButton[i]->setOff();
                    break;
                case HeadSetting::QuartzHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[5]);
                    headButton[i]->setOff();
                    break;
                case HeadSetting::InfraRedHeadOff:
                    headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[6]);
                    headButton[i]->setOff();
                    break;
                }
            else
                headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[0]);
            if((i<headSettButton.length()))
                headSettButton[i]->setEnabled(headActDialog->getHeadActivAtIndex(i));
        }
    }
}

void MainWindow::getUdpData(QByteArray data)
{
    indexer->clickButton(data);
}

void MainWindow::getHeadColor(int index, QColor color)
{
    if(machineSettings.machineParam.direction == 1)
        index = headsCount-machineSettings.machineParam.useUnloadHead - index;
    headButton[index]->setRagColor(color);
}

void MainWindow::getHeadParam(int index, QByteArray hParamArr)
{
    headSettings.fromByteArray(hParamArr);
    HeadSetting::setHeadOn_OffStateInd(index, static_cast<bool>(hParamArr[2]));
    settings->setValue(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
    if(machineSettings.machineParam.direction == 1)
        index = headsCount-machineSettings.machineParam.useUnloadHead - index;
//    qDebug()<<machineSettings.machineParam.direction<<index<<headsCount<<machineSettings.machineParam.useUnloadHead;

    if(headActDialog->getHeadActivAtIndex(index))
        switch ((HeadSetting::HeadOnType)(static_cast<uint8_t>(hParamArr[0])))
        {
        case HeadSetting::PrintHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[1]);
            headButton[index]->setStrokCount(headSettings.headParam.stroksCount);
            headButton[index]->setStepBkStrCnt(headSettings.headParam.stroksSBCount);
            break;
        case HeadSetting::QuartzHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[2]);
            headButton[index]->setDryPower(headSettings.headParam.dryPowerQ);
            break;
        case HeadSetting::InfraRedHeadOn:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[3]);
            break;
        case HeadSetting::PrintHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[4]);
            headButton[index]->setOff();
            break;
        case HeadSetting::QuartzHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[5]);
            headButton[index]->setOff();
            break;
        case HeadSetting::InfraRedHeadOff:
            headButton[index]->setPixmap(headButton[index]->getRagState(),headStylesStr[6]);
            headButton[index]->setOff();
            break;
        }
    else
        headButton[index]->setPixmap(HeadForm::shirtOff,headStylesStr[0]);
    headSettButton[index-1]->setEnabled(headActDialog->getHeadActivAtIndex(index));

}

void MainWindow::getAllHeadParam(int index, QByteArray hParamArr)
{
    headSettings.fromByteArray(hParamArr);            

    uint8_t dev;
    for(dev = 1; dev<headsCount; dev++)
    {
        if(((dev<headsCount-1)|(!this->machineSettings.machineParam.useUnloadHead))&(dev != index))
        {
            if((headSettings.headParam.headOnType == HeadSetting::PrintHeadOn)|
                    (headSettings.headParam.headOnType == HeadSetting::PrintHeadOff))
            {
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadSpeedRear);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadRangeLimit1);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadSpeedFront);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadRangeLimit2);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadStroksCount);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadSBStroksCount);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlDwellTime);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadSqDwellTime);
            }
            if((headSettings.headParam.headOnType == HeadSetting::QuartzHeadOn)|
                    (headSettings.headParam.headOnType == HeadSetting::QuartzHeadOff))
            {
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashTime1Q);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashTime2Q);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashPowerWtoutIR);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashTimeStBy);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadHeatTemper);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashTime1Q);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashPowerStBy);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadFlashWarmTime);
            }
            if((headSettings.headParam.headOnType == HeadSetting::InfraRedHeadOn)|
                    (headSettings.headParam.headOnType == HeadSetting::InfraRedHeadOff))
            {
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadHeatTime1IR);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadHeatTime2IR);
                comPort->sendReg(HeadSetting::HeadDeviceAdrOffcet+dev, HeadSetting::HeadHeatDryRange);
            }
            settings->setValue(QString("HEAD/HEAD_"+QString::number(dev)+"_PARAM"), hParamArr);
        }
    }
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

void MainWindow::getHeadActiveState(int index, bool state)
{
    headSettButton[index-1]->setEnabled(state);
    headSettButton[index-1]->setEnabled(headActDialog->getHeadActivAtIndex(index));
}

void MainWindow::getCyclesCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getLoadState(int index, LoadState state)
{
    QByteArray cmdArr;
    int data;
    if(index == 0)
    {
        cmdArr.clear();
        cmdArr.append(static_cast<char>((IndexerLiftSettings::IndexerDevice)&0x00FF));
        cmdArr.append(static_cast<char>(IndexerLiftSettings::LoadHeadState&0x00FF));
        cmdArr.append(static_cast<char>(state>>8));
        cmdArr.append(static_cast<char>(state&0x00FF));
        data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
        cmdArr.append(static_cast<char>(data>>8));
        cmdArr.append(static_cast<char>(data&0x00FF));
        comPort->sendData(cmdArr);
    }
    else
    {
        qDebug()<<index
               <<headsCount+1-index
              <<this->machineSettings.machineParam.useUnloadHead
             <<headsCount-this->machineSettings.machineParam.useUnloadHead-index;
        MachineSettings::setHeadPalStateIndex(index, static_cast<bool>(state == LoadOne));
        cmdArr.clear();
        if(machineSettings.machineParam.direction == 1)
            data = (headsCount-index)+HeadSetting::ChangeRagState;
        else
            data = index+HeadSetting::ChangeRagState;
        qDebug()<<"sends"<<data-HeadSetting::ChangeRagState;

        cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
        cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
        cmdArr.append(static_cast<char>(data>>8));
        cmdArr.append(static_cast<char>(data&0x00FF));
        data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
        cmdArr.append(static_cast<char>(data>>8));
        cmdArr.append(static_cast<char>(data&0x00FF));
        comPort->sendData(cmdArr);

//        cmdArr.clear();
//        cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
//        cmdArr.append(static_cast<char>(MachineSettings::MasterPaletStateLo&0x00FF));
//        cmdArr.append(static_cast<char>(MachineSettings::getHeadPalStateLo()>>8));
//        cmdArr.append(static_cast<char>(MachineSettings::getHeadPalStateLo()&0x00FF));
//        data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
//        cmdArr.append(static_cast<char>(data>>8));
//        cmdArr.append(static_cast<char>(data&0x00FF));
//        comPort->sendData(cmdArr);

//        cmdArr.clear();
//        cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
//        cmdArr.append(static_cast<char>(MachineSettings::MasterPaletStateHi&0x00FF));
//        cmdArr.append(static_cast<char>(MachineSettings::getHeadPalStateHi()>>8));
//        cmdArr.append(static_cast<char>(MachineSettings::getHeadPalStateHi()&0x00FF));
//        data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
//        cmdArr.append(static_cast<char>(data>>8));
//        cmdArr.append(static_cast<char>(data&0x00FF));
//        comPort->sendData(cmdArr);
    }
}

void MainWindow::getIndexerParam(QByteArray indexerParamArr)
{
//    this->indexerLiftSettings.fromByteArray(indexerParamArr, this->indexerLiftSettings.liftParam.toByteArray());
    settings->setValue(QString("INDEXER_PARAMS"), indexerParamArr);
//    comPort->sendData(indexerParamArr);

}

void MainWindow::getLiftParam(QByteArray liftParamArr)
{
//    this->indexerLiftSettings.fromByteArray(this->indexerLiftSettings.indexerParam.toByteArray(), liftParamArr);
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

void MainWindow::getWarmingCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getWarmingEnable(bool enable)
{
    ui->pButtonWarming->setVisible(enable);
    if(!enable)
    {
        indexer->setVisible(true);
        warming->setVisible(false);
    }
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
            machineParamArr[0] = static_cast<char>(headsCount&0x00FF);
            machineParamArr[1] = static_cast<char>((headsCount>>8)&0x00FF);
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
    settings->setValue(QString("MACHINE_PARAMS"), this->machineSettings.machineParam.toByteArray());
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
        headButton[headsCount - 1]->setHeadformType(HeadForm::HeadUnload);

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
    counterDialog->setStyleSheet(this->styleSheet());
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
    ui->widgetHeads->setStyleSheet("#widgetHeads{"
                                   "background-color: rgba(255, 255, 255, 0);"
                                   "border-style: none;}");
    ui->widgetLiftOffcet->setStyleSheet("border-style: outset; background-color: rgba(255, 255, 255, 0);");
    ui->dSpinBoxStepDelay->setStyleSheet(ui->dSpinBoxLiftOffcet->styleSheet());
    ui->widgetStepDelay->setStyleSheet(ui->widgetLiftOffcet->styleSheet());

    ui->labelTime->setStyleSheet("font: 28px; font-style:italic; font-family: \"Courier\"");
    ui->labelUserName->setStyleSheet("font: 13px; font-style:italic; font-family:\"Linux Libertine Mono O\"");

    this->setBackGround(settings->value("STYLE/IMAGE_EN", false).toBool());
}

void MainWindow::getLangFile(int langIndex)
{
    if(langIndex != -1)
    {
        settings->setValue("STYLE/LANG_SEL_INDEX", langIndex);
        qApp->removeTranslator(&translator);
        translator.load(settings->value("STYLE/LANG_PATH").toStringList()[langIndex]);
        ui->retranslateUi(this);
    }
}

void MainWindow::serviceStateChange()
{
    if(!MachineSettings::getServiceWidgEn())
    {
        QByteArray passwordBArr;
        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));

        if(logedInService || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_GENERAL", 19442)))
        {
            MachineSettings::setServiceWidgEn(true);
            machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
            generalSettingDialog->setMachineSetting(machineSettings.machineParam);
        }
        else
        {
            MachineSettings::setServiceWidgEn(false);
            QMessageBox msgBox;
            msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
            msgBox.setText(tr("Wrong password!"));
            msgBox.setWindowTitle(tr("Password"));
            msgBox.exec();
        }
    }
    else
        MachineSettings::setServiceWidgEn(false);
    this->generalSettingDialogRequest();
    indexer->setSettingEnable(MachineSettings::getServiceWidgEn());
}

void MainWindow::warmingStateChange(bool state)
{
}

void MainWindow::exitProgram(bool restart)
{
    if(!restart)
        this->exitCode = ExitDialog::tryExit(this, usersSettingDialog->getLoginWindowEnable());
    else
        this->exitCode = ExitDialog::RestartProgram;

    timeProgramEnd = QTime::currentTime();
    timeWorking.setHMS(0,0,0);
    timeWorking = timeWorking.addMSecs(timeProgramStart.msecsTo(timeProgramEnd));

    settings->setValue("COUNTERS/RAG_ALL_CNT", ragAllCount);
    settings->setValue("COUNTERS/INDEXER_ALL_CNT", indexerCyclesAll);

    settings->sync();
    if(this->exitCode == ExitDialog::RestartProgram)
    {
        if(settings->value("EMAIL_SETTINGS", false).value<EmailSettings>().mailEnable)
            mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                    "Machine restarting\n"
                                    "Worker: " + this->userName + ".\n"
                                    "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                    "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                    "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                    "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                    " and " + QString::number(ragAllCount) + " items in total.\n"
                                    "\nHave a great day!" );
        QProgressDialog *pDialog = new QProgressDialog("Wait for machine shutdown.", "Cancel", 0, 30);
        pDialog->setWindowModality(Qt::WindowModal);
        pDialog->setStyleSheet(this->styleSheet());
        pDialog->setAutoClose(true);
        pDialog->setValue(0);
        settings->setValue("PROGRAM/FINISH", true);
        for (int i = pDialog->minimum(); i <= pDialog->maximum(); i++)
        {
            pDialog->setValue(i);
            QThread::msleep(250);
            if(pDialog->wasCanceled())
                break;
        }
        if(!pDialog->wasCanceled())
            this->close();
    }
    else
        if((this->exitCode != ExitDialog::Continue)
                &(this->exitCode != ExitDialog::ReprogramMachine)
                &(this->exitCode != ExitDialog::LogOut))
        {
            comPort->closeSerialPort();

            watchDog->stop();

            switch (this->exitCode) {
            case ExitDialog::ExitFromProgram:
                settings->setValue("PROGRAM/FINISH", true);
                if(settings->value("EMAIL_SETTINGS", false).value<EmailSettings>().mailEnable)
                    mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                        "Worker: " + this->userName + ".\n"
                                        "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                        "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                        "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                        "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                        " and " + QString::number(ragAllCount) + " items in total.\n"
                                        "\nHave a great day!" );

                this->close();
                break;

            case ExitDialog::Shutdown:
                settings->setValue("PROGRAM/FINISH", true);
                if(settings->value("EMAIL_SETTINGS", false).value<EmailSettings>().mailEnable)
                    mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                        "Worker: " + this->userName + ".\n"
                                        "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                        "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                        "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                        "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                        " and " + QString::number(ragAllCount) + " items in total.\n"
                                        "\nHave a great day!" );

                this->close();
                break;
            case ExitDialog::RestartMachine:
                if(settings->value("EMAIL_SETTINGS", false).value<EmailSettings>().mailEnable)
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
            switch (exitCode) {
            case ExitDialog::ReprogramMachine:
            {
                watchDog->stop();
                indexer->setState(0x0);
                infoWidget->setIndicatorState(0x704);
                ComSettings comSett = settings->value("COM_SETTING", QVariant::fromValue(ComSettings())).value<ComSettings>();
                comSett.stringBaudRate = "19200";
                comSett.stringParity = "None";
                comSett.stringStopBits = "1";
                comSett.stringDataBits = "5";
                comPort->openSerialPort(comSett);
                reprogramDialog->show();
                break;
            }
            case ExitDialog::LogOut:
            {
                if(settings->value("EMAIL_SETTINGS", false).value<EmailSettings>().mailEnable)
                    mailSender->sendMessage("Hi!\nThis is LiQt Machine Interface\n"
                                            "User changed.\n"
                                            "Worker: " + this->userName + ".\n"
                                            "Program start time is " + timeProgramStart.toString("H:mm:ss") + ".\n"
                                            "Program finish time is " + timeProgramEnd.toString("H:mm:ss") + ".\n"
                                            "Total work time is " + timeWorking.toString("H:mm:ss") + ".\n"
                                            "Machine printed " + QString::number(ragSessionCount) + " items this session"
                                            " and " + QString::number(ragAllCount) + " items in total.\n"
                                            "\nHave a great day!", true);
                timeProgramStart = QTime::currentTime();
                ragSessionCount = 0;
                this->userLogin();
                break;
            }
            }
}

void MainWindow::saveJob()
{
    QString saveFileName = KeyboardDialog::getText(this, "Enter job name");
    saveFileName = "./jobs/"+saveFileName+".ini";
    //QFileDialog::getSaveFileName(this, "Save job...",".","Setting file(*.ini)");
    QFile::copy(settings->fileName(), saveFileName);
    delete settings;
    settings = new QSettings("./settingsPATH.ini", QSettings::IniFormat);
    settings->setValue("LAST_PATH", saveFileName);
    delete settings;
    settings = new QSettings(saveFileName, QSettings::IniFormat);
}

void MainWindow::loadJob()
{
    int t_INDEXER_ALL_CNT = settings->value("INDEXER_ALL_CNT", 0).toInt();
    int t_RAG_ALL_CNT = settings->value("RAG_ALL_CNT", 0).toInt();
    int t_HEAD_COUNT = this->machineSettings.machineParam.headCount;

    QString openFileName = QFileDialog::getOpenFileName(this, "Open job...","./jobs/","Setting file(*.ini)");
    if(!openFileName.isEmpty())
    {
        delete settings;
        settings = new QSettings("./settingsPATH.ini", QSettings::IniFormat);
        settings->setValue("LAST_PATH", openFileName);
        delete settings;
        settings = new QSettings(openFileName, QSettings::IniFormat);

        settings->setValue("INDEXER_ALL_CNT", t_INDEXER_ALL_CNT);
        settings->setValue("RAG_ALL_CNT", t_RAG_ALL_CNT);

        this->getLangFile(settings->value("STYLE/LANG_SEL_INDEX", 0).toInt());
        this->setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                                    +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt()))).toString());
        this->machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
        this->machineSettings.machineParam.headCount = t_HEAD_COUNT;
        generalSettingDialog->setMachineSetting(this->machineSettings.machineParam);

        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::WarningTime);

        indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                          settings->value("LIFT_PARAMS").value<QByteArray>());

        registers->setMasterReg(this->machineSettings);
        registers->setIndexLiftReg(this->indexerLiftSettings);
        indexerLiftSetDialog->setIndexerSetting(indexerLiftSettings.indexerParam, false);
        indexerLiftSetDialog->setLiftSetting(indexerLiftSettings.liftParam, false);

        indexerLiftSetDialog->setLiftDistance(indexerLiftSettings.indexerParam.distance/100.f,
                                              this->machineSettings.machineParam.liftGearRatio);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexSpeed);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexAcceleration);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexSpeedRet);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::IndexAccelerationRet);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftDelayDown);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftDelayUp);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftSpeed);
        comPort->sendReg(IndexerLiftSettings::IndexerDevice, IndexerLiftSettings::LiftAcceleration);

        uint8_t i;

        for(i = 1; i < this->headsCount-this->machineSettings.machineParam.useUnloadHead; i++)
        {
            headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
            registers->setHeadReg(i, headSettings);
            //        headSettingDialog->setHeadParams(headSettings.headParam, i, false);
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
                headButton[i]->setHeadformType(HeadForm::HeadLoad);
            else
                if((i==headsCount - 1)&(machineSettings.machineParam.useUnloadHead))
                    headButton[i]->setHeadformType(HeadForm::HeadUnload);
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
                            headButton[i]->setStrokCount(headSettings.headParam.stroksCount);
                            headButton[i]->setStepBkStrCnt(headSettings.headParam.stroksSBCount);
                            break;
                        case HeadSetting::QuartzHeadOn:
                            headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[2]);
                            headButton[i]->setDryPower(headSettings.headParam.dryPowerQ);
                            break;
                        case HeadSetting::InfraRedHeadOn:
                            headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[3]);
                            break;
                        case HeadSetting::PrintHeadOff:
                            headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[4]);
                            headButton[i]->setOff();
                            break;
                        case HeadSetting::QuartzHeadOff:
                            headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[5]);
                            headButton[i]->setOff();
                            break;
                        case HeadSetting::InfraRedHeadOff:
                            headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[6]);
                            headButton[i]->setOff();
                            break;
                        }
                    }
                    else
                        headButton[i]->setPixmap(headButton[i]->getRagState(),headStylesStr[0]);
        }
    }

}

void MainWindow::setHeadsPosition()
{
    int areaW, areaH;
    areaH = ui->widgetHeads->height();
    areaW = ui->widgetHeads->width();

    int i;
    float sinCoefHeads, cosCoefHeads, sinCoefSett, cosCoefSett, R, x0_hb, y0_hb, x0_sb, y0_sb;
    if(areaH<areaW)
        R = areaH/2-headButton[0]->height()/2-headSettButton[0]->height()/2-10;
    else
        R = areaW/2-headButton[0]->width()/2-headSettButton[0]->width()/2-10;

    x0_hb = ui->widgetHeads->width()/2-headButton[0]->width()/2;
    y0_hb = ui->widgetHeads->height()/2-headButton[0]->height()/2+headSettButton[0]->width()/2;
    x0_sb = ui->widgetHeads->width()/2-headSettButton[0]->width()/2;
    y0_sb = ui->widgetHeads->height()/2-headSettButton[0]->height()/2+headSettButton[0]->width()/2;

    if(x0_hb<y0_hb)
        y0_hb = x0_hb;
    else
        x0_hb = y0_hb;
    if(x0_sb<y0_sb)
        y0_sb = x0_sb;
    else
        x0_sb = y0_sb;

    int direction = machineSettings.machineParam.direction;
    cycleDialog->setDirection(direction);
    cycleDialog->setUnloadUseState(this->machineSettings.machineParam.useUnloadHead);

    for(i = 0; i<headsCount; i++)
    {
        sinCoefHeads = sin(direction*2.*3.1415926*i/headsCount+3.1415926/2.
                      +direction*3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        cosCoefHeads = cos(direction*2.*3.1415926*i/headsCount+3.1415926/2.
                      +direction*3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        sinCoefSett = sin(-2.*3.1415926*i/headsCount+3.1415926/2.
                      -3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        cosCoefSett = cos(-2.*3.1415926*i/headsCount+3.1415926/2.
                      -3.1415926/headsCount*this->machineSettings.machineParam.useUnloadHead);
        headButton[i]->move(x0_hb+(R)*cosCoefHeads, y0_hb+(R)*sinCoefHeads);

        if((i != 0)&(((i != headsCount-1)&(machineSettings.machineParam.useUnloadHead))
                     |(!machineSettings.machineParam.useUnloadHead)))
        {
            headSettButton[i-1]->move(x0_sb+(R+13+headButton[i]->width()/2+headSettButton[0]->width()/2)*cosCoefSett,
                    y0_sb+(R+13+headButton[i]->height()/2+headSettButton[0]->width()/2)*sinCoefSett);
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



    infoWidget->move(x0_sb-infoWidget->width()/2+headButton[0]->width()/3, y0_hb+18-infoWidget->height()/2);

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
    if(headButton[0]->getRagState() == HeadForm::shirtOn)
        headButton[0]->setPixmap(HeadForm::shirtOff);

    if(!static_cast<bool>(ragAllCount%100))
    {
        settings->setValue("COUNTERS/RAG_ALL_CNT", ragAllCount);
        settings->setValue("COUNTERS/INDEXER_ALL_CNT", indexerCyclesAll);
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

void MainWindow::showMaintananceWidget()
{
    maintanceWidget->setElemets(maintanceDialog->getUnsolvedList());
    maintanceWidget->show();
}

void MainWindow::setIconFolder(int index)
{
    QStringList list = settings->value("STYLE/ICON_PATH").value<QStringList>();
    if(list.length()>index)
    {
        QString path = list[index];
        this->pathIcon = path;
        settings->setValue("STYLE/ICON_SEL_INDEX", index);

        ui->pButtonExit->setIcon(QIcon(path+"/exit.png"));
        ui->pButtonSetting->setIcon(QIcon(path+"/settings.png"));
        ui->pButtonLoadJob->setIcon(QIcon(path+"/load.png"));
        ui->pButtonSaveJob->setIcon(QIcon(path+"/save.png"));
        ui->pButtonMaintance->setIcon(QIcon(path+"/warning.png"));
        ui->pButtonCyclesSetup->setIcon(QIcon(path+"/cycles.png"));

        indexer->setIconFolder(path);
        warming->setIconFolder(path);
        headSettingDialog->setIconFolder(path);
        generalSettingDialog->setIconFolder(path);


        int i;
        for(i = 0; i<headButton.length(); i++)
            headButton[i]->setIconPath(path);
        for(i = 0; i<headSettButton.length(); i++)
            headSettButton[i]->setIconPath(path+"/settings.png");

        infoWidget->setIconFolder(path);

        if(ui->pButtonWarming->isChecked())
        {
            ui->pButtonWarming->setText(tr("Disable\nwarming"));
            ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOff.png"));
        }
        else
        {
            ui->pButtonWarming->setText(tr("Enable\nwarming"));
            ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOn.png"));
        }
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

            imgName = QFileDialog::getOpenFileName(this, "Select background..", "/home/liqt/Pictures", "Images (*.png *.xpm)");
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

void MainWindow::updateTimeSlot()
{
    ui->labelTime->setText(QTime::currentTime().toString("HH:mm"));
    ui->labelDate->setText(QDate::currentDate().toString("ddd, dd/MM/yyyy"));
}

void MainWindow::resetSkippedRequest()
{
    QByteArray cmdArr;
    uint16_t data = 0;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::SkipCntLo&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);

    cmdArr.clear();
    data = 0;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::SkipCntHi&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::resetRemainingRequest()
{
    int val = settings->value("COUNTERS/REMAINING", 0).toInt();

    QByteArray cmdArr;
    uint16_t data = static_cast<uint16_t>(val&0x0000FFFF);
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterRemainLo&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);

    cmdArr.clear();
    data = static_cast<uint16_t>((val>>16)&0x0000FFFF);
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterRemainHi&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);

    cmdArr.clear();
    data = 0;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterPrintedLo&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);

    cmdArr.clear();
    data = 0;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterPrintedHi&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::remainingValChangedRequest(int val)
{
    settings->setValue("COUNTERS/REMAINING", val);
    infoWidget->setRemaining(val);
    QByteArray cmdArr;
    uint16_t data = static_cast<uint16_t>(val&0x0000FFFF);
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterRemainLo&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);

    cmdArr.clear();
    data = static_cast<uint16_t>((val>>16)&0x0000FFFF);
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterRemainHi&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}

void MainWindow::counterDialogRequest()
{
    counterDialog->show();
    counterDialog->move(ui->widgetHeads->width()-counterDialog->width(), 90);

    this->lastOpened = counterDialog;
}

void MainWindow::skipStateUpdate()
{
    settings->setValue("PROGRAM/SOFT_SKIP", MachineSettings::getSoftwartSkipEn());
}

void MainWindow::userLogin()
{
    if(usersSettingDialog->getLoginWindowEnable())
    {
        LoginDialog loginDialog;
        loginDialog.setUserNames(usersSettingDialog->getUserNames());
        loginDialog.setStyleSheet(this->styleSheet()
                                  +"*{color: black; font: 24px bold italic large}"
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

    ui->labelUserName->setText(this->userName);
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

    qDebug()<<(!QSslSocket::supportsSsl())<<QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
//    if(((!QSslSocket::supportsSsl())|
//            (!(QSslSocket::sslLibraryBuildVersionString() == QSslSocket::sslLibraryVersionString())))
//            &(settings->value("EMAIL_SETTINGS").value<EmailSettings>().mailEnable))
//    {
//        QMessageBox msgBox;
//        msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
//        msgBox.setText(tr("Wrong ssl library."));
//        msgBox.setInformativeText(tr("E-mail will not be sended\n"
//                                  "to continue press \"Yes\" \n"
//                                  "To exit press \"No\"."));
//        msgBox.setWindowTitle(tr("E-mail"));
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//        int ret = msgBox.exec();
//        switch (ret)
//        {
//        case QMessageBox::Yes:
//            break;
//        case QMessageBox::No:
//            this->exitProgram(false);
//            break;
//        }
//    }

    needCompleteReset = true;

    MachineSettings::setServiceWidgEn(false);
    this->exitCode = ExitDialog::Continue;
    indexer->setSettingEnable(false);

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
    ui->labelTime->setStyleSheet("font: 28px; font-style:italic; font-family:\"Linux Libertine Mono O\"");
    ui->labelDate->setStyleSheet("font: 13px; font-style:italic; font-family:\"Linux Libertine Mono O\"");
    ui->labelUserName->setStyleSheet("font: 13px; font-style:italic; font-family:\"Linux Libertine Mono O\"");

    ui->pButtonWarming->setVisible(machineSettings.machineParam.lastRevWarm.field.warm);
    ui->pButtonWarming->setChecked(false);
    ui->pButtonWarming->setText(tr("Enable\nwarming"));
    ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOn.png"));

    timeProgramStart = QTime::currentTime();
    ui->labelTime->setText(timeProgramStart.toString("HH:mm"));
    ui->labelDate->setText(QDate::currentDate().toString("ddd, dd/MM/yyyy"));
    watchDog->start();
    updateTime->start(30000);
}

void MainWindow::headsInit()
{
    if(this->machineSettings.machineParam.useUnloadHead)
        headsCount+=2;
    else
        headsCount+=1;

    headActDialog = new HeadActivationDialog(headsCount-this->machineSettings.machineParam.useUnloadHead, this);
    headActDialog->setHeadActivState(settings->value("HEAD/ACTIVATION", 0).toInt());
    connect(generalSettingDialog, SIGNAL(headActivationRequest()), headActDialog, SLOT(show()));
    connect(headActDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getHeadActivCommand(QByteArray)));
    connect(headActDialog, SIGNAL(headStateChangedSignal(int, bool)), this, SLOT(getHeadActiveState(int, bool)));

    int i;
    for(i = 0; i<headsCount; i++)
    {
        headButton.append(new HeadForm(ui->widgetHeads));
        headButton[i]->setIndex(i);
        connect(headButton[i], SIGNAL(loadStateChanged(int, LoadState)), this, SLOT(getLoadState(int, LoadState)));
        if(i==0)
        {
            headButton[i]->setHeadformType(HeadForm::HeadLoad);
        }
        else
            if((i==headsCount - 1)&(this->machineSettings.machineParam.useUnloadHead))
                headButton[i]->setHeadformType(HeadForm::HeadUnload);
            else
            {
                headButton[i]->setPixmap(HeadForm::shirtOff,headStylesStr[0]);
                QColor col;
                headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>());
                col.setRed((headSettings.headParam.inkColor&0x000000FF)>>0);
                col.setGreen((headSettings.headParam.inkColor&0x0000FF00)>>8);
                col.setBlue((headSettings.headParam.inkColor&0x00FF0000)>>16);
                headButton[i]->setRagColor(col);
                headButton[i]->setStrokCount(headSettings.headParam.stroksCount);
                headButton[i]->setStepBkStrCnt(headSettings.headParam.stroksSBCount);
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
            headSettButton[i-1]->setEnabled(headActDialog->getHeadActivAtIndex(i));
        }
    }
}

void MainWindow::watchDogTimeout()
{
    needCompleteReset = true;
    int i;
    for(i = 0; i<headButton.length(); i++)
        headButton[i]->setPixmap(HeadForm::shirtOff);
    indexer->resetWidget();
    indexer->setState(0x0);
    ui->pButtonExit->setEnabled(true);
    infoWidget->setIndicatorState(0x704);
    infoWidget->setText(tr("Lost communication.\nPlease press reset button."));
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

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(this->lastOpened != 0)
    {
        this->lastOpened->hide();
        this->lastOpened = 0;
    }

}

void MainWindow::on_pButtonWarming_clicked()
{
    if(ui->pButtonWarming->isChecked())
    {
        indexer->setVisible(false);
        warming->setVisible(true);
        ui->pButtonWarming->setText(tr("Disable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOff.png"));
        int i;
        for(i = 0; i<headSettButton.length(); i++)
            headSettButton[i]->setVisible(false);
    }
    else
    {
        warming->setVisible(false);
        indexer->setVisible(true);
        ui->pButtonWarming->setText(tr("Enable\nwarming"));
        ui->pButtonWarming->setIcon(QIcon(pathIcon+"/warmingOn.png"));
        int i;
        for(i = 0; i<headSettButton.length(); i++)
            headSettButton[i]->setVisible(true);
    }
    machineSettings.machineParam.lastRevWarm.field.warm = ui->pButtonWarming->isChecked();

    QByteArray cmdArr;
    int data;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexLastRevolvWarm&0x00FF));
    data = machineSettings.machineParam.lastRevWarm.all;
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    comPort->sendData(cmdArr);
}
