#include "indexersettingdialog.h"
#include "ui_indexersettingdialog.h"

IndexerSettingDialog::IndexerSettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerSettingDialog)
{
    ui->setupUi(this);

//    setStyleSheet(QString(("* {color: #ABEFF6;"
//                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
//                           "selection-color: yellow;"
//                           "selection-background-color: blue;"
//                           "font: 14px bold italic large \"Times New Roman\"}"
//                           "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
//                           "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8080A0, stop: 0.8 #606070,stop: 1.0 #202030);}")));

    this->eventFilterSetup();

    connect(ui->pButtonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pButtonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    acceptOnDeactilationEn = true;

}

IndexerSettingDialog::~IndexerSettingDialog()
{
    delete ui;
}

void IndexerSettingDialog::setRegisters(Register *reg)
{
    this->registers = reg;
}

void IndexerSettingDialog::setIndexerSetting(bool disconnect)
{
    if(disconnect)
        this->disconnectAll();
    ui->dSpinBoxIndexAccel->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                        Register::indexerReg_ACC)/*/10.*/);
    ui->dSpinBoxIndexAccelRet->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                           Register::indexerReg_RACC)/*/10*/);
    ui->dSpinBoxIndexDistance->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                           Register::indexerReg_DIST)/10.);
    ui->spinBoxIndexDistanceOffcet->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                                Register::indexerReg_DIST_OFFSET));
    ui->spinBoxIndexHomeOffset->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                            Register::indexerReg_HOME_OFFSET));
    ui->spinBoxIndexSpeed->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                       Register::indexerReg_SPEED));
    ui->spinBoxindexSpeedRet->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                          Register::indexerReg_RSPEED));
    if(disconnect)
        this->connectAll();
    else
    {
        acceptEnable = true;
        this->accept();
    }
}

void IndexerSettingDialog::setLiftSetting(bool disconnect)
{
    if(disconnect)
        this->disconnectAll();
    ui->dSpinBoxLiftAccel->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                       Register::liftReg_ACC)/*/10.*/);
    ui->dSpinBoxLiftDownDelay->setValue(registers->readReg(IndexerLiftSettings::LiftDevice,
                                                           Register::liftReg_DOWN_DELAY)/10.);
    ui->dSpinBoxLiftUpDelay->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                         Register::indexerliftReg_UP_DELAY)/10.);
    ui->dSpinBoxLiftDistance->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                          Register::liftReg_DIST)/100.);
    ui->spinBoxLiftHomeOffset->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                           Register::liftReg_HOME_OFFSET));
    ui->spinBoxLiftSpeed->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice,
                                                      Register::liftReg_SPEED));
    if(disconnect)
        this->connectAll();
    else
    {
        acceptEnable = true;
        this->accept();
    }
}

void IndexerSettingDialog::setIndexerSetting(IndexerLiftSettings::IndexParameters indexParam, bool disconnect)
{
    if(disconnect)
        this->disconnectAll();
    ui->dSpinBoxIndexAccel->setValue(indexParam.acceleration/*/10.*/);
    ui->dSpinBoxIndexAccelRet->setValue(indexParam.accelerationRet/*/10*/);
    ui->dSpinBoxIndexDistance->setValue(indexParam.distance/10.);
    ui->spinBoxIndexDistanceOffcet->setValue(indexParam.distOffcet);
    ui->spinBoxIndexHomeOffset->setValue(indexParam.homeOffset);
    ui->spinBoxIndexSpeed->setValue(indexParam.speed);
    ui->spinBoxindexSpeedRet->setValue(indexParam.speedRet);
    if(disconnect)
        this->connectAll();
    else
    {
        acceptEnable = true;
        this->accept();
    }
}

void IndexerSettingDialog::setLiftSetting(IndexerLiftSettings::LiftParameters liftParam, bool disconnect)
{
    if(disconnect)
        this->disconnectAll();
    ui->dSpinBoxLiftAccel->setValue(liftParam.acceleration/*/10.*/);
    ui->dSpinBoxLiftDownDelay->setValue(liftParam.delayDown/10.);
    ui->dSpinBoxLiftUpDelay->setValue(liftParam.delayUp/10.);
    ui->dSpinBoxLiftDistance->setValue(liftParam.distance/100.);
    ui->spinBoxLiftHomeOffset->setValue(liftParam.homeOffcet);
    ui->spinBoxLiftSpeed->setValue(liftParam.speed);
    if(disconnect)
        this->connectAll();
    else
    {
        acceptEnable = true;
        this->accept();
    }
}

void IndexerSettingDialog::setLiftDistance(float distance, int gearRatio)
{
    disconnect(ui->dSpinBoxLiftDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDistance_valueChanged(double)));
    ui->dSpinBoxLiftDistance->setValue(distance);
    connect(ui->dSpinBoxLiftDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDistance_valueChanged(double)));

    QByteArray cmdArr;
    int data = distance*100;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDistance&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

    uint32_t liftPulseDist = Register::calcLiftPulse(gearRatio, distance*100);

    cmdArr.clear();
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
    cmdArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_L));
    data = ((uint16_t)(liftPulseDist&0x0000FFFF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

    cmdArr.clear();
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
    cmdArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_H));
    data = ((uint16_t)((liftPulseDist>>16)&0x0000FFFF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

}

float IndexerSettingDialog::getLiftDistance()
{
    return ui->dSpinBoxLiftDistance->value();
}

void IndexerSettingDialog::setLiftGearRatio(uint32_t gearRatio)
{
    this->liftGearRatio = gearRatio;
}

void IndexerSettingDialog::connectAll()
{
    connect(ui->dSpinBoxIndexAccel, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexAccel_valueChanged(double)));
    connect(ui->dSpinBoxIndexAccelRet, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexAccelRet_valueChanged(double)));
    connect(ui->dSpinBoxIndexDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexDistance_valueChanged(double)));
    connect(ui->spinBoxIndexDistanceOffcet, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexDistanceOffcet_valueChanged(double)));
    connect(ui->spinBoxIndexHomeOffset, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexHomeOffset_valueChanged(double)));
    connect(ui->spinBoxIndexSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexSpeed_valueChanged(double)));
    connect(ui->spinBoxindexSpeedRet, SIGNAL(valueChanged(double)), this, SLOT(spinBoxindexSpeedRet_valueChanged(double)));

    connect(ui->dSpinBoxLiftAccel, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftAccel_valueChanged(double)));
    connect(ui->dSpinBoxLiftDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDistance_valueChanged(double)));
    connect(ui->dSpinBoxLiftDownDelay, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDownDelay_valueChanged(double)));
    connect(ui->dSpinBoxLiftUpDelay, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftUpDelay_valueChanged(double)));
    connect(ui->spinBoxLiftHomeOffset, SIGNAL(valueChanged(double)), this, SLOT(spinBoxLiftHomeOffset_valueChanged(double)));
    connect(ui->spinBoxLiftSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxLiftSpeed_valueChanged(double)));
}

void IndexerSettingDialog::disconnectAll()
{
    disconnect(ui->dSpinBoxIndexAccel, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexAccel_valueChanged(double)));
    disconnect(ui->dSpinBoxIndexAccelRet, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexAccelRet_valueChanged(double)));
    disconnect(ui->dSpinBoxIndexDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxIndexDistance_valueChanged(double)));
    disconnect(ui->spinBoxIndexDistanceOffcet, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexDistanceOffcet_valueChanged(double)));
    disconnect(ui->spinBoxIndexHomeOffset, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexHomeOffset_valueChanged(double)));
    disconnect(ui->spinBoxIndexSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxIndexSpeed_valueChanged(double)));
    disconnect(ui->spinBoxindexSpeedRet, SIGNAL(valueChanged(double)), this, SLOT(spinBoxindexSpeedRet_valueChanged(double)));

    disconnect(ui->dSpinBoxLiftAccel, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftAccel_valueChanged(double)));
    disconnect(ui->dSpinBoxLiftDistance, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDistance_valueChanged(double)));
    disconnect(ui->dSpinBoxLiftDownDelay, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftDownDelay_valueChanged(double)));
    disconnect(ui->dSpinBoxLiftUpDelay, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxLiftUpDelay_valueChanged(double)));
    disconnect(ui->spinBoxLiftHomeOffset, SIGNAL(valueChanged(double)), this, SLOT(spinBoxLiftHomeOffset_valueChanged(double)));
    disconnect(ui->spinBoxLiftSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxLiftSpeed_valueChanged(double)));
}


void IndexerSettingDialog::accept()
{
    if(acceptEnable)
    {
        IndexerLiftSettings::IndexParameters indexParam;
        indexParam.acceleration = ui->dSpinBoxIndexAccel->value();
        indexParam.accelerationRet = ui->dSpinBoxIndexAccelRet->value();
        indexParam.distance = ui->dSpinBoxIndexDistance->value()*10.;
        indexParam.distOffcet = ui->spinBoxIndexDistanceOffcet->value();
        indexParam.homeOffset = ui->spinBoxIndexHomeOffset->value();
        indexParam.speed = ui->spinBoxIndexSpeed->value();
        indexParam.speedRet = ui->spinBoxindexSpeedRet->value();

        IndexerLiftSettings::LiftParameters liftParams;
        liftParams.acceleration = ui->dSpinBoxLiftAccel->value()/**10.*/;
        liftParams.delayDown = ui->dSpinBoxLiftDownDelay->value()*10.;
        liftParams.delayUp = ui->dSpinBoxLiftUpDelay->value()*10.;
        liftParams.distance = ui->dSpinBoxLiftDistance->value()*100.;
        liftParams.homeOffcet = ui->spinBoxLiftHomeOffset->value();
        liftParams.speed = ui->spinBoxLiftSpeed->value();

        emit this->indexerParamChanged(indexParam.toByteArray());
        emit this->liftParamChanged(liftParams.toByteArray());

        this->hide();
    }
    else
        acceptEnable = true;
}

void IndexerSettingDialog::reject()
{
    acceptEnable = false;
    this->hide();
}

void IndexerSettingDialog::eventFilterSetup()
{
    QObjectList objList = ui->dSpinBoxIndexAccel->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxIndexAccelRet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxIndexDistance->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxLiftAccel->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxLiftDistance->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxLiftDownDelay->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxLiftUpDelay->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxIndexDistanceOffcet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxIndexHomeOffset->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxIndexSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxindexSpeedRet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxLiftHomeOffset->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxLiftSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
}

bool IndexerSettingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease))
            |((QApplication::platformName() == "linuxfb")&(event->type()==QEvent::MouseButtonRelease)))
    {
        acceptOnDeactilationEn = false;
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
        acceptOnDeactilationEn = true;
    }
    return false;
}

void IndexerSettingDialog::showEvent(QShowEvent *ev)
{
    acceptEnable = true;
    ui->dSpinBoxIndexDistance->setVisible(MachineSettings::getServiceWidgEn()
                                          &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->spinBoxIndexHomeOffset->setVisible(MachineSettings::getServiceWidgEn()
                                           &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->spinBoxIndexDistanceOffcet->setVisible(MachineSettings::getServiceWidgEn()
                                               &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->spinBoxIndexHomeOffset->setVisible(MachineSettings::getServiceWidgEn()
                                           &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->dSpinBoxLiftDistance->setVisible(MachineSettings::getServiceWidgEn()
                                         &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->spinBoxLiftHomeOffset->setVisible(MachineSettings::getServiceWidgEn()
                                          &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->labelH1->setVisible(MachineSettings::getServiceWidgEn()
                            &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->labelH2->setVisible(MachineSettings::getServiceWidgEn()
                            &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->labelH3->setVisible(MachineSettings::getServiceWidgEn()
                            &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->labelH4->setVisible(MachineSettings::getServiceWidgEn()
                            &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->labelH5->setVisible(MachineSettings::getServiceWidgEn()
                            &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->pButtonIndexHome->setVisible(MachineSettings::getServiceWidgEn()
                                     &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->pButtonIndexMove->setVisible(MachineSettings::getServiceWidgEn()
                                     &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->pButtonLiftHome->setVisible(MachineSettings::getServiceWidgEn()
                                    &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));
    ui->pButtonLiftMove->setVisible(MachineSettings::getServiceWidgEn()
                                    &(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA)));

    ui->labelS1->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->labelS2->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->labelS3->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->labelA1->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->labelA2->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->labelA3->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->dSpinBoxIndexAccel->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->dSpinBoxIndexAccelRet->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->spinBoxIndexSpeed->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->spinBoxindexSpeedRet->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->dSpinBoxLiftAccel->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));
    ui->spinBoxLiftSpeed->setVisible(!(MachineSettings::getMachineType() == MachineSettings::TitanAAA));

    ev->accept();
}

void IndexerSettingDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

bool IndexerSettingDialog::event(QEvent *e)
{
    if((e->type()==QEvent::WindowDeactivate)
            |((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave))
            |((QApplication::platformName() == "linuxfb")&(e->type()==QEvent::MouseButtonRelease)))
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}


void IndexerSettingDialog::dSpinBoxIndexDistance_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexDistance&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::spinBoxIndexHomeOffset_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexHomeOffset&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::spinBoxIndexDistanceOffcet_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexDistOffcet&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::spinBoxIndexSpeed_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexSpeed&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxIndexAccel_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1/**10*/;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexAcceleration&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::spinBoxindexSpeedRet_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexSpeedRet&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxIndexAccelRet_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1/**10*/;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexAccelerationRet&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxLiftDownDelay_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDelayDown&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxLiftUpDelay_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDelayUp&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxLiftDistance_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*100;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDistance&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

    uint32_t liftPulseDist = Register::calcLiftPulse(this->liftGearRatio, arg1*100);

    cmdArr.clear();
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
    cmdArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_L));
    data = ((uint16_t)(liftPulseDist&0x0000FFFF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

    cmdArr.clear();
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
    cmdArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_H));
    data = ((uint16_t)((liftPulseDist>>16)&0x0000FFFF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);

    emit this->liftDistanceChanged(arg1);
}

void IndexerSettingDialog::spinBoxLiftHomeOffset_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftHomeOffcet&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::spinBoxLiftSpeed_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftSpeed&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::dSpinBoxLiftAccel_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1/**10*/;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::LiftAcceleration&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::on_pButtonLiftMove_clicked()
{
    QByteArray cmdArr;
    int data = IndexerLiftSettings::LiftMoveEnd;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::on_pButtonLiftHome_clicked()
{
    QByteArray cmdArr;
    int data = IndexerLiftSettings::LiftMoveHome;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::on_pButtonIndexMove_clicked()
{
    QByteArray cmdArr;
    int data = IndexerLiftSettings::IndexMoveEnd;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerSettingDialog::on_pButtonIndexHome_clicked()
{
    QByteArray cmdArr;
    int data = IndexerLiftSettings::IndexMoveHome;
    cmdArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}
