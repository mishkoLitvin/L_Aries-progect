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

    settings = new QSettings("./settings.ini", QSettings::IniFormat);

    setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"
                                          +QString::number(settings->value("STYLE/STYLE_SEL_INDEX").toInt()))).toString());
    ui->widgetHeads->setStyleSheet("background-color: rgba(255, 255, 255, 0);");

    headsCount = settings->value(QString("HEAD/HEADS_COUNT"), 1).toInt();
    headsCount+=2;

    comPort = new SerialPort(this);
    comPort->setComParams(settings->value("COM_SETTING").value<ComSettings>());
    comPort->setStyleSheet(this->styleSheet());
    connect(comPort, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));

//    QByteArray passwordBArr;
//    passwordBArr.append("998");
//    uint16_t temp =  CrcCalc::CalculateCRC16(0xFFFF, passwordBArr);
//    settings->setValue("PASSWORDS/PASSWORD_INDEXER", temp);
//    qDebug() << temp;
    int i;

//    QStringList stList;
//    stList.append("Blue");
//    stList.append("Red");
//    stList.append("Green");
//    settings->setValue("STYLE/STYLE_LIST", stList);
//    settings->setValue("STYLE/STYLE_SEL_INDEX", 0);


    headSettingDialog = new SettingDialog(headSettings);
    connect(headSettingDialog, SIGNAL(accept(int,QByteArray)), this, SLOT(getHeadParam(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(changeNumber(int)), this, SLOT(changeHeadNo(int)));
    connect(headSettingDialog, SIGNAL(setParamsToAll(int,QByteArray)), this, SLOT(getAllHeadParam(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(sendCommand(int,QByteArray)), this, SLOT(getHeadCommand(int,QByteArray)));
    headSettingDialog->setStyleSheet(this->styleSheet());

    indexer = new IndexerWidget(this);
    connect(indexer, SIGNAL(settingButtonCliced()), this, SLOT(indexerLiftSettingRequest()));
    connect(indexer, SIGNAL(sendCommand(QByteArray)), this, SLOT(getIndexLiftCommand(QByteArray)));
    ui->layoutIndexer->addWidget(indexer);

    indexerLiftSetDialog = new IndexerSettingDialog();
    connect(indexerLiftSetDialog, SIGNAL(indexerParamChanged(QByteArray)), this, SLOT(getIndexerParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(liftParamChanged(QByteArray)), this, SLOT(getLiftParam(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(sendCommand(QByteArray)), this, SLOT(getIndexLiftSettComm(QByteArray)));
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());

    generalSettingDialog = new GeneralSettingDialog();
    generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS").value<EmailSettings>());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL").toInt(), settings->value("PASSWORDS/PASSWORD_LOCK_MAIL").toInt());
    generalSettingDialog->setStyleList(settings->value("STYLE/STYLE_LIST").value<QStringList>(), settings->value("STYLE/STYLE_SEL_INDEX").toInt());
    generalSettingDialog->showPortInfo(settings->value("COM_SETTING").value<ComSettings>());
    connect(ui->pButtonSetting, SIGNAL(clicked(bool)), this,  SLOT(generalSettingDialogRequest()));
    connect(generalSettingDialog, SIGNAL(machineParamChanged(QByteArray)), this, SLOT(getMachineParam(QByteArray)));
    connect(generalSettingDialog, SIGNAL(emailSettingsChanged(EmailSettings)), this, SLOT(getEmailSettings(EmailSettings)));
    connect(generalSettingDialog, SIGNAL(serialPortSettingsDialogRequested()), comPort, SLOT(setupPort()));
    connect(generalSettingDialog, SIGNAL(serviceSettingRequest()), this, SLOT(serviceStateChange()));
    connect(generalSettingDialog, SIGNAL(styleChangedIndex(int)), this, SLOT(getVeiwSettings(int)));

    mailSender = new MailSender(this);
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().receiverAdress);

    connect(ui->pButtonExit, SIGNAL(clicked(bool)), this, SLOT(exitProgram()));
    connect(ui->pButtonSaveJob, SIGNAL(clicked(bool)), this, SLOT(saveJob()));
    connect(ui->pButtonLoadJob, SIGNAL(clicked(bool)), this, SLOT(loadJob()));


    for(i = 0; i<headsCount; i++)
    {
        headButton.append(new HeadForm(ui->widgetHeads));
        headButton[i]->setIndex(i);
        if(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[1]&0x01)
            switch (settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[0]) {
            case 0:
                headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                break;
            case 1:
                headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                break;
            case 2:
                headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                break;
            }
        else
            headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
        headSettButton[i] = new HeadSettingButton(i, ui->widgetHeads);
        if(i<(HEAD_COUNT)/4)
            headButton[i]->setSettBtnPosition(HeadForm::AtRightUp);
        if((i>=(headsCount)/4)&(i<(headsCount)/2))
            headButton[i]->setSettBtnPosition(HeadForm::AtRightDown);
        if((i>=(headsCount)/2)&(i<(3*headsCount)/4))
            headButton[i]->setSettBtnPosition(HeadForm::AtLeftDown);
        if((i>=(3*headsCount)/4)&(i<(headsCount)))
            headButton[i]->setSettBtnPosition(HeadForm::AtLeftUp);
        connect(headSettButton[i], SIGNAL(settingButtonCliced(int)), this, SLOT(headSettingRequest(int)));
    }


    if(QApplication::platformName() != "eglfs")
        this->resize(QSize(1024, 768));
    else
        this->setWindowState(Qt::WindowMaximized);
    this->setButtonPoss();

    logedInHeadSettings = false;
    logedInIndexer = false;
    logedInGeneral = false;
    logedInService = false;

//    this->setWindowState(Qt::WindowFullScreen);
//    this->setWindowOpacity(0.2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::headSettingRequest(int index)
{
        headSettings.fromByteArray(settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>());
        headSettingDialog->setHeadParams(headSettings, index);
//        headSettingDialog->move(this->pos().x()+300+200*cos(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.),this->pos().y()+300+200*sin(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.));
        headSettingDialog->move(this->pos().x()+this->width()-headSettingDialog->width()-30,
                                   this->pos().y()+10);
        qDebug()<<this->pos().x()+this->width()-headSettingDialog->width()-30<<this->pos().y()+10;
        headSettingDialog->show();
}

void MainWindow::indexerLiftSettingRequest()
{
//    QByteArray passwordBArr;
//#ifndef DEBUG_BUILD

//    if(!(logedInIndexer|logedInService)){
//        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
//    }
//    if(logedInIndexer|logedInService || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_INDEXER")))
//#endif
//    {
//        logedInIndexer = true;
        indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                          settings->value("LIFT_PARAMS").value<QByteArray>());
        indexerLiftSetDialog->setIndexerSetting(indexerLiftSettings.indexerParam);
        indexerLiftSetDialog->setLiftSetting(indexerLiftSettings.liftParam);
        indexerLiftSetDialog->show();
        indexerLiftSetDialog->move(this->pos().x()+this->width()-indexerLiftSetDialog->width(),
                                   this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
//    }
//#ifndef DEBUG_BUILD
//    else
//    {
//        QMessageBox msgBox;
//        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
//        msgBox.setText("Wrong password!");
//        msgBox.setWindowTitle("Password");
//        msgBox.exec();
//    }
//#endif
}

void MainWindow::generalSettingDialogRequest()
{
//    QByteArray passwordBArr;
//#ifndef DEBUG_BUILD
//    if(!(logedInGeneral|logedInService)){
//        passwordBArr.append(QString::number(NumpadDialog::getValue(this, "Password")));
//    }
//    if(logedInGeneral|logedInService || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_GENERAL")))
//#endif
//    {
//        logedInGeneral = true;

        machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
        generalSettingDialog->setMachineSetting(machineSettings.machineParam);
        generalSettingDialog->show();
        generalSettingDialog->move(this->pos().x()+this->width()-generalSettingDialog->width(),
                                   this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
//    }
//#ifndef DEBUG_BUILD
//    else
//    {
//        QMessageBox msgBox;
//        msgBox.setStyleSheet(this->styleSheet()+"QPushButton {min-width: 70px; min-height: 55px}");
//        msgBox.setText("Wrong password!");
//        msgBox.setWindowTitle("Password");
//        msgBox.exec();
//    }
//#endif
}

void MainWindow::changeHeadNo(int index)
{
    if(index<0)
        index = headsCount-1;
    else
        if(index == headsCount)
            index = 0;
    this->headSettingRequest(index);
}

void MainWindow::getHeadParam(int index, QByteArray hParamArr)
{
    settings->setValue(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
    if(settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[1]&0x01)

        switch (settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[0]) {
        case 0:
            headButton[index]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
            break;
        case 1:
            headButton[index]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
            break;
        case 2:
            headButton[index]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
            break;
        }
    else
        headButton[index]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
//    comPort->sendData(hParamArr);

}

void MainWindow::getAllHeadParam(int index, QByteArray hParamArr)
{
    int cnt;
    index++;
    for(cnt = 0; cnt<headsCount; cnt++)
    {
        comPort->sendData(hParamArr);
        settings->setValue(QString("HEAD/HEAD_"+QString::number(cnt)+"_PARAM"), hParamArr);
        if(hParamArr[1]&0x01)
            switch (hParamArr[0]) {
            case 0:
                headButton[cnt]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                break;
            case 1:
                headButton[cnt]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                break;
            case 2:
                headButton[cnt]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                break;
            }
        else
            headButton[cnt]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
    }
}

void MainWindow::getHeadCommand(int index, QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getIndexerParam(QByteArray indexerParamArr)
{
    settings->setValue(QString("INDEXER_PARAMS"), indexerParamArr);
    comPort->sendData(indexerParamArr);

}

void MainWindow::getLiftParam(QByteArray liftParamArr)
{
    settings->setValue(QString("LIFT_PARAMS"), liftParamArr);
    comPort->sendData(liftParamArr);

}

void MainWindow::getIndexLiftSettComm(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getIndexLiftCommand(QByteArray commandArr)
{
    comPort->sendData(commandArr);
}

void MainWindow::getMachineParam(QByteArray machineParamArr)
{
    settings->setValue("MACHINE_PARAMS", machineParamArr);
    if(this->headsCount-2 != (((0x00FF&((uint16_t)machineParamArr[1]))<<8)|(0x00FF&((uint16_t)machineParamArr[0]))))
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet()+"*{color: white; font: 16px bold italic large}"+"QPushButton {min-width: 70px; min-height: 55px}");
        msgBox.setText("Heads count changed");
        msgBox.setInformativeText("To apply count changing please restart a program");
        msgBox.setWindowTitle("Info");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Save:
            headsCount = ((0x00FF&((uint16_t)machineParamArr[1]))<<8)|(0x00FF&((uint16_t)machineParamArr[0]));
            settings->setValue(QString("HEAD/HEADS_COUNT"), headsCount);
            break;
        case QMessageBox::Discard:
            break;
        }
    }
    comPort->sendData(machineParamArr);
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
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().receiverAdress);
}

void MainWindow::getVeiwSettings(int stSheetIndex)
{
    settings->setValue("STYLE/STYLE_SEL_INDEX", stSheetIndex);
    setStyleSheet(settings->value(QString("STYLE/STYLE_SHEET_"+QString::number(stSheetIndex))).toString());
    headSettingDialog->setStyleSheet(this->styleSheet());
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    comPort->setStyleSheet(this->styleSheet());
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
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
        generalSettingDialog->show();
        generalSettingDialog->setFocusLossAccept(true);

    }
#endif
}

void MainWindow::exitProgram()
{
#ifndef DEBUG_BUILD
    mailSender->sendMessage();
#endif
    settings->sync();
    comPort->closeSerialPort();

    this->close();
}

void MainWindow::saveJob()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save job...",".","Setting file(*.ini)");
    QFile::copy(settings->fileName(), saveFileName);
}

void MainWindow::loadJob()
{
    delete settings;
    QString openFileName = QFileDialog::getOpenFileName(this, "Open job...",".","Setting file(*.ini)");
    settings = new QSettings(openFileName, QSettings::IniFormat);
    int i;
    for(i = 0; i<headsCount; i++)
        {
            if(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[1]&0x01)
                switch (settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[0]) {
                case 0:
                    headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                    break;
                case 1:
                    headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                    break;
                case 2:
                    headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                    break;
                }
            else
                headButton[i]->setPixmap(HeadForm::shirtOff,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
        }

}

void MainWindow::setButtonPoss()
{
    qDebug()<<this->size()<<ui->widgetHeads->size();

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
    y0_hb = ui->widgetHeads->height()/2-headButton[0]->height()/2;
    x0_sb = ui->widgetHeads->width()/2-headSettButton[0]->width()/2;
    y0_sb = ui->widgetHeads->height()/2-headSettButton[0]->height()/2;

    for(i = 0; i<headsCount; i++)
    {
        sinCoef = sin(2.*3.1415926*i/headsCount+3.1415926/2.+3.1415926/headsCount);
        cosCoef = cos(2.*3.1415926*i/headsCount+3.1415926/2.+3.1415926/headsCount);


            headButton[i]->move(x0_hb+(R)*cosCoef,
                                y0_hb+(R)*sinCoef);
            headSettButton[i]->move(x0_sb+(R+headButton[i]->width()/2+headSettButton[i]->width()/2)*cosCoef,
                                y0_sb+(R+headButton[i]->height()/2+headSettButton[i]->width()/2)*sinCoef);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    this->setButtonPoss();
    e->accept();
}

void MainWindow::showEvent(QShowEvent *ev)
{
    this->setButtonPoss();
}
