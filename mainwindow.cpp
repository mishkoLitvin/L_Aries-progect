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

    setStyleSheet(settings->value("STYLE/STYLE_SHEET").toString());

    comPort = new SerialPort(this);
    comPort->setComParams(settings->value("COM_SETTING").value<ComSettings>());
    connect(comPort, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));

//    QByteArray passwordBArr;
//    passwordBArr.append("997");
//    uint16_t temp =  CrcCalc::CalculateCRC16(0xFFFF, passwordBArr);
//    settings->setValue("PASSWORDS/PASSWORD_GENERAL", temp);
//    qDebug() << temp;

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
    indexerLiftSetDialog->setStyleSheet(this->styleSheet());

    generalSettingDialog = new GeneralSettingDialog();
    generalSettingDialog->setEmailSettings(settings->value("EMAIL_SETTINGS").value<EmailSettings>());
    generalSettingDialog->setStyleSheet(this->styleSheet());
    generalSettingDialog->setPasswords(settings->value("PASSWORDS/PASSWORD_SERIAL").toInt(), settings->value("PASSWORDS/PASSWORD_LOCK_MAIL").toInt());
    connect(ui->pButtonSetting, SIGNAL(clicked(bool)), this,  SLOT(generalSettingDialogRequest()));
    connect(generalSettingDialog, SIGNAL(machineParamChanged(QByteArray)), this, SLOT(getMachineParam(QByteArray)));
    connect(generalSettingDialog, SIGNAL(emailSettingsChanged(EmailSettings)), this, SLOT(getEmailSettings(EmailSettings)));
    connect(generalSettingDialog, SIGNAL(serialPortSettingsDialogRequested()), comPort, SLOT(setupPort()));
//    qDebug()<<settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress;

    mailSender = new MailSender(this);
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().receiverAdress);


    connect(ui->pButtonExit, SIGNAL(clicked(bool)), this, SLOT(exitProgram()));
    connect(ui->pButtonSaveJob, SIGNAL(clicked(bool)), this, SLOT(saveJob()));
    connect(ui->pButtonLoadJob, SIGNAL(clicked(bool)), this, SLOT(loadJob()));

    int i;

    for(i = 0; i<HEAD_COUNT; i++)
    {
        headButton[i] = new HeadForm(ui->widgetHeads);
        headButton[i]->setIndex(i);
        if(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[1]&0x01)
            switch (settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[0]) {
            case 0:
                headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                break;
            case 1:
                headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                break;
            case 2:
                headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                break;
            }
        else
            headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");

        connect(headButton[i], SIGNAL(settingButtonCliced(int)), this, SLOT(headSettingRequest(int)));
    }

    if(QApplication::platformName() != "eglfs")
        this->resize(QSize(1024, 768));

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
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD

    if(!logedInIndexer){
        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
    }
    if(logedInIndexer || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_INDEXER")))
#endif
    {
        logedInIndexer = true;
        indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                          settings->value("LIFT_PARAMS").value<QByteArray>());
        indexerLiftSetDialog->setIndexerSetting(indexerLiftSettings.indexerParam);
        indexerLiftSetDialog->setLiftSetting(indexerLiftSettings.liftParam);
        indexerLiftSetDialog->show();
        indexerLiftSetDialog->move(this->pos().x()+this->width()-indexerLiftSetDialog->width(),
                                   this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
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
}

void MainWindow::generalSettingDialogRequest()
{
    QByteArray passwordBArr;
#ifndef DEBUG_BUILD
    if(!logedInGeneral){
        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
    }
    if(logedInGeneral || (CrcCalc::CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORDS/PASSWORD_GENERAL")))
#endif
    {
        logedInGeneral = true;

        machineSettings.fromByteArray(settings->value("MACHINE_PARAMS").value<QByteArray>());
        generalSettingDialog->setMachineSetting(machineSettings.machineParam);
        generalSettingDialog->show();
        generalSettingDialog->move(this->pos().x()+this->width()-generalSettingDialog->width(),
                                   this->pos().y()+10/*+this->height()-indexerLiftSetDialog->height()*/);
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
}

void MainWindow::changeHeadNo(int index)
{
    if(index<0)
        index = HEAD_COUNT-1;
    else
        if(index == HEAD_COUNT)
            index = 0;
    this->headSettingRequest(index);
}

void MainWindow::getHeadParam(int index, QByteArray hParamArr)
{
    settings->setValue(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
    if(settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[1]&0x01)

        switch (settings->value(QString("HEAD/HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[0]) {
        case 0:
            headButton[index]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
            break;
        case 1:
            headButton[index]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
            break;
        case 2:
            headButton[index]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
            break;
        }
    else
        headButton[index]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
    comPort->sendData(hParamArr);

}

void MainWindow::getAllHeadParam(int index, QByteArray hParamArr)
{
    int cnt;
    index++;
    for(cnt = 0; cnt<HEAD_COUNT; cnt++)
    {
        comPort->sendData(hParamArr);
        settings->setValue(QString("HEAD/HEAD_"+QString::number(cnt)+"_PARAM"), hParamArr);
        if(hParamArr[1]&0x01)
            switch (hParamArr[0]) {
            case 0:
                headButton[cnt]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                break;
            case 1:
                headButton[cnt]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                break;
            case 2:
                headButton[cnt]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                break;
            }
        else
            headButton[cnt]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
    }
}

void MainWindow::getHeadCommand(int index, QByteArray commandArr)
{
    qDebug()<<"headcommand:"<<index;
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

void MainWindow::getIndexLiftCommand(QByteArray commandArr)
{
    qDebug()<<"I-L command:";
    comPort->sendData(commandArr);
}

void MainWindow::getMachineParam(QByteArray machineParamArr)
{
    settings->setValue("MACHINE_PARAMS", machineParamArr);
    comPort->sendData(machineParamArr);
}

void MainWindow::getSerialSetting(ComSettings comSett)
{
    settings->setValue("COM_SETTING", QVariant::fromValue(comSett));
    generalSettingDialog->raise();
    generalSettingDialog->setFocusLossAccept(true);
}

void MainWindow::getEmailSettings(EmailSettings emailSett)
{
    settings->setValue("EMAIL_SETTINGS", QVariant::fromValue(emailSett));
    mailSender->setSenderMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderAdress);
    mailSender->setSenderPassword(settings->value("EMAIL_SETTINGS").value<EmailSettings>().senderPassword);
    mailSender->setRecipientMailAdress(settings->value("EMAIL_SETTINGS").value<EmailSettings>().receiverAdress);
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
    for(i = 0; i<HEAD_COUNT; i++)
        {
            if(settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[1]&0x01)
                switch (settings->value(QString("HEAD/HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[0]) {
                case 0:
                    headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                    break;
                case 1:
                    headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                    break;
                case 2:
                    headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                    break;
                }
            else
                headButton[i]->setPixmap(HeadForm::pixmapHide,"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");
        }

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    int areaW, areaH;
    areaH = ui->widgetHeads->height();
    areaW = ui->widgetHeads->width();

//    qDebug()<<this->size()<<ui->widgetHeads->size();

    int i;
    for(i = 0; i<HEAD_COUNT; i++)
    {
        if(areaH<areaW)
            headButton[i]->move(areaW/2+areaH/3*cos(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-73,
                                areaH/2+areaH/3*sin(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-60);
        else
            headButton[i]->move(areaW/2+areaW/3*cos(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-73,
                                areaH/2+areaW/3*sin(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-60);

    }
//    headButton[0]->move(areaW/2-73,
//                        areaH/2-60);
    e->accept();
}
