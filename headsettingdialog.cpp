#include "headsettingdialog.h"
#include "ui_headsettingdialog.h"

SettingDialog::SettingDialog(HeadSetting hSttg, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);


    ui->tabWidget->setStyleSheet("QTabBar::tab {color: white;}"
                                    "QTabBar::tab:first:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                                    "QTabBar::tab:first:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #2050A0, stop: 0.8 #203070,stop: 1.0 #202030);}"
                                    "QTabBar::tab:last:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);}"
                                    "QTabBar::tab:last:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #AE385B, stop: 0.8 #693359,stop: 1.0 #681030);}"
                                    "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #962AAD, stop: 0.8 #8C04A8,stop: 1.0 #6C0382);}"
                                    "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #77168B, stop: 0.8 #610A73,stop: 1.0 #4F025F);}"
                                 );

    ui->widgetQuartzWithoutTempSensor->setStyleSheet(".QWidget {border-style: none; background-color: 0x00000000}");
    ui->widgetQuartzWithTempSensor->setStyleSheet(".QWidget {border-style: none; background-color: 0x00000000}");

    this->index = index;

    ui->tabWidget->setCurrentIndex(hSttg.headParam.headType);
    ui->lcdNumberHeadNo->display(index);

    ui->pButtonHeadOnOff->setChecked(hSttg.headParam.powerOn&0x01);

    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        ui->spinBoxRearSpeed->setValue(hSttg.headParam.speedRear);
        ui->spinBoxFrontSpeed->setValue(hSttg.headParam.speedFront);
        ui->dSpinBoxFrontRange->setValue(hSttg.headParam.limitFront/10.);
        ui->dSpinBoxRearRange->setValue(hSttg.headParam.limitRear/10.);
        ui->spinBoxStrokCount->setValue(hSttg.headParam.stroksCount);
        break;
    }
    case 1:
    {
        ui->dSpinBoxHeatTime1Q->setValue(hSttg.headParam.heatTime1/10.);
        ui->dSpinBoxHeatTime2Q->setValue(hSttg.headParam.heatTime2/10.);
        ui->spinBoxDryPowerQ->setValue(hSttg.headParam.heatPower);
        break;
    }
    case 2:
    {
        ui->dSpinBoxHeatTime1IR->setValue(hSttg.headParam.heatTime1/10.);
        ui->dSpinBoxHeatTime2IR->setValue(hSttg.headParam.heatTime2/10.);
        ui->dSpinBoxDryingRangeIR->setValue(hSttg.headParam.limitFront/10.);
        break;
    }
    }
//    this->installEventFilter(this);

    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pButtonHeadNoInc, SIGNAL(clicked(bool)), this, SLOT(pButtonIncClkd()));
    connect(ui->pButtonHeadNoDec, SIGNAL(clicked(bool)), this, SLOT(pButtonDecClkd()));

    connect(ui->pushButtonCopyToAll, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonCopyToAll_clicked()));
//    connect(ui->toolButtonPlast, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonPlast_clicked()));
//    connect(ui->toolButtonMoveFront, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonMoveFront_clicked()));
//    connect(ui->toolButtonMoveRear, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonMoveRear_clicked()));
//    connect(ui->toolButtonMoveTest, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonMoveTest_clicked()));
//    connect(ui->toolButtonFL, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonFL_clicked()));
//    connect(ui->toolButtonFL_SQ, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonFL_SQ_clicked()));
//    connect(ui->toolButtonSQ, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonSQ_clicked()));
//    connect(ui->toolButtonMTPMove, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonMTPMove_clicked()));
//    connect(ui->toolButtonPressure, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonPressure_clicked()));
//    connect(ui->toolButtonHoldOn, SIGNAL(clicked(bool)), this, SLOT(on_toolButtonHoldOn_clicked()));

    withTemperatureSensor = false;
    connect(ui->checkBoxQuartzTempSensor, SIGNAL(toggled(bool)), this, SLOT(temperatureSensoreChanged(bool)));

    this->eventFilterSetup();

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setHeadParams(HeadSetting hSttg, int index)
{
    this->disconnectAll();

    this->index = index;

    ui->tabWidget->setCurrentIndex(hSttg.headParam.headType);
    ui->lcdNumberHeadNo->display(index);

    ui->pButtonHeadOnOff->setChecked(hSttg.headParam.powerOn&0x01);

    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        ui->spinBoxRearSpeed->setValue(hSttg.headParam.speedRear);
        ui->spinBoxFrontSpeed->setValue(hSttg.headParam.speedFront);
        ui->dSpinBoxFrontRange->setValue(hSttg.headParam.limitFront/10.);
        ui->dSpinBoxRearRange->setValue(hSttg.headParam.limitRear/10.);
        ui->spinBoxStrokCount->setValue(hSttg.headParam.stroksCount);
        break;
    }
    case 1:
    {
        ui->dSpinBoxHeatTime1Q->setValue(hSttg.headParam.heatTime1/10.);
        ui->dSpinBoxHeatTime2Q->setValue(hSttg.headParam.heatTime2/10.);
        ui->spinBoxDryPowerQ->setValue(hSttg.headParam.heatPower);
        break;
    }
    case 2:
    {
        ui->dSpinBoxHeatTime1IR->setValue(hSttg.headParam.heatTime1/10.);
        ui->dSpinBoxHeatTime2IR->setValue(hSttg.headParam.heatTime2/10.);
        ui->dSpinBoxDryingRangeIR->setValue(hSttg.headParam.limitFront/10.);
        break;
    }
    }
    acceptOnDeactilationEn = true;
    this->connectAll();
}

void SettingDialog::setIconFolder(QString path)
{
    ui->toolButtonFL->setIcon(QIcon(path+"/sqfl_up.png"));
    ui->toolButtonFL_SQ->setIcon(QIcon(path+"/sqfl_middle.png"));
    ui->toolButtonSQ->setIcon(QIcon(path+"/sqfl_down.png"));
    ui->toolButtonHoldOn->setIcon(QIcon(path+"/handNO.png"));
    ui->toolButtonMoveFront->setIcon(QIcon(path+"/arrows/arrowDL.png"));
    ui->toolButtonMoveRear->setIcon(QIcon(path+"/arrows/arrowUR.png"));
    ui->toolButtonMoveTest->setIcon(QIcon(path+"/arrows/arrowLR2.png"));
    ui->toolButtonMTPMove->setIcon(QIcon(path+"/arrows/arrowUD.png"));
    ui->toolButtonPlast->setIcon(QIcon(path+"/Bucket.png"));

    ui->pushButtonOK->setIcon(QIcon(path+"/check.png"));
    ui->pushButtonCancel->setIcon(QIcon(path+"/multip.png"));
    ui->pushButtonCopyToAll->setIcon(QIcon(path+"/copy.png"));

    ui->tabWidget->setTabIcon(0, QIcon(path+"/brush.png"));
    ui->tabWidget->setTabIcon(1, QIcon(path+"/lamp.png"));
    ui->tabWidget->setTabIcon(2, QIcon(path+"/heat.png"));

}

void SettingDialog::accept()
{
    if(acceptEnable)
    {
        HeadSetting hSttg;
        hSttg.headParam.powerOn = ui->pButtonHeadOnOff->isChecked();
        switch(ui->tabWidget->currentIndex())
        {
        case 0:
        {
            hSttg.headParam.headType = HeadSetting::PrintHead;
            hSttg.headParam.speedRear = ui->spinBoxRearSpeed->value();
            hSttg.headParam.speedFront = ui->spinBoxFrontSpeed->value();
            hSttg.headParam.limitFront = ui->dSpinBoxFrontRange->value()*10;
            hSttg.headParam.limitRear = ui->dSpinBoxRearRange->value()*10;
            hSttg.headParam.stroksCount = ui->spinBoxStrokCount->value();
            break;
        }
        case 1:
        {
            hSttg.headParam.headType = HeadSetting::QuartzHead;
            hSttg.headParam.heatTime1 = ui->dSpinBoxHeatTime1Q->value()*10;
            hSttg.headParam.heatTime2 = ui->dSpinBoxHeatTime2Q->value()*10;
            hSttg.headParam.heatPower = ui->spinBoxDryPowerQ->value();
            break;
        }
        case 2:
        {
            hSttg.headParam.headType = HeadSetting::InfraRedHead;
            hSttg.headParam.heatTime1 = ui->dSpinBoxHeatTime1IR->value()*10;
            hSttg.headParam.heatTime2 = ui->dSpinBoxHeatTime2IR->value()*10;
            hSttg.headParam.limitFront = ui->dSpinBoxDryingRangeIR->value()*10;
            break;
        }
        }
        emit this->accept(this->index, hSttg.headParam.toByteArray());
        this->hide();
    }
    else
        acceptEnable = true;
}

void SettingDialog::reject()
{
    acceptEnable = false;
    this->hide();
}

void SettingDialog::pButtonIncClkd()
{
    this->accept();

    emit this->changeNumber(this->index+1);
}

void SettingDialog::pButtonDecClkd()
{
    this->accept();

    emit this->changeNumber(this->index-1);
}

void SettingDialog::connectAll()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_currentChanged(int )));
    connect(ui->spinBoxRearSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRearSpeed_valueChanged(double )));
    connect(ui->dSpinBoxRearRange, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxRearRange_valueChanged(double )));
    connect(ui->spinBoxFrontSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxFrontSpeed_valueChanged(double )));
    connect(ui->dSpinBoxFrontRange, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxFrontRange_valueChanged(double )));
    connect(ui->spinBoxStrokCount, SIGNAL(valueChanged(double)), this, SLOT(spinBoxStrokCount_valueChanged(double )));
    connect(ui->spinBoxSBStrokCount, SIGNAL(valueChanged(double)), this, SLOT(spinBoxSBStrokCount_valueChanged(double)));
    connect(ui->spinBoxDryPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxDryPowerQ_valueChanged(double )));
    connect(ui->dSpinBoxHeatTime1IR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime1IR_valueChanged(double )));
    connect(ui->dSpinBoxHeatTime2IR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime2IR_valueChanged(double )));
    connect(ui->dSpinBoxDryingRangeIR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxDryingRangeIR_valueChanged(double )));
    connect(ui->dSpinBoxFlDwellTime, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxFlDwellTime_valueChanged(double )));
    connect(ui->dSpinBoxSqDwellTime, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxSqDwellTime_valueChanged(double )));
    connect(ui->dSpinBoxHeatTime1Q, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime1Q_valueChanged(double )));
    connect(ui->dSpinBoxHeatTime2Q, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime2Q_valueChanged(double )));
    connect(ui->spinBoxDryPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxDryPowerQ_valueChanged(double)));
    connect(ui->dSpinBoxStepbackDryTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxStepbackDryTimeQ_valueChanged(double)));
    connect(ui->dSpinBoxTemperatureSetQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxTemperatureSetQ_valueChanged(double)));
    connect(ui->dSpinBoxDryTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxDryTimeQ_valueChanged(double)));
    connect(ui->dSpinBoxStandbyTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxStandbyTimeQ_valueChanged(double)));
    connect(ui->spinBoxStandbyPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxStandbyPowerQ_valueChanged(double)));
    connect(ui->dSpinBoxWarmFlashTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxWarmFlashTimeQ_valueChanged(double)));
}

void SettingDialog::disconnectAll()
{
    disconnect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_currentChanged(int )));
    disconnect(ui->spinBoxRearSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRearSpeed_valueChanged(double )));
    disconnect(ui->dSpinBoxRearRange, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxRearRange_valueChanged(double )));
    disconnect(ui->spinBoxFrontSpeed, SIGNAL(valueChanged(double)), this, SLOT(spinBoxFrontSpeed_valueChanged(double )));
    disconnect(ui->dSpinBoxFrontRange, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxFrontRange_valueChanged(double )));
    disconnect(ui->spinBoxStrokCount, SIGNAL(valueChanged(double)), this, SLOT(spinBoxStrokCount_valueChanged(double )));
    disconnect(ui->spinBoxSBStrokCount, SIGNAL(valueChanged(double)), this, SLOT(spinBoxSBStrokCount_valueChanged(double )));
    disconnect(ui->spinBoxDryPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxDryPowerQ_valueChanged(double )));
    disconnect(ui->dSpinBoxHeatTime1IR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime1IR_valueChanged(double )));
    disconnect(ui->dSpinBoxHeatTime2IR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime2IR_valueChanged(double )));
    disconnect(ui->dSpinBoxDryingRangeIR, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxDryingRangeIR_valueChanged(double )));
    disconnect(ui->dSpinBoxFlDwellTime, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxFlDwellTime_valueChanged(double )));
    disconnect(ui->dSpinBoxSqDwellTime, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxSqDwellTime_valueChanged(double )));
    disconnect(ui->dSpinBoxHeatTime1Q, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime1Q_valueChanged(double )));
    disconnect(ui->dSpinBoxHeatTime2Q, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxHeatTime2Q_valueChanged(double )));
    disconnect(ui->spinBoxDryPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxDryPowerQ_valueChanged(double)));
    disconnect(ui->dSpinBoxStepbackDryTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxStepbackDryTimeQ_valueChanged(double)));
    disconnect(ui->dSpinBoxTemperatureSetQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxTemperatureSetQ_valueChanged(double)));
    disconnect(ui->dSpinBoxDryTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxDryTimeQ_valueChanged(double)));
    disconnect(ui->dSpinBoxStandbyTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxStandbyTimeQ_valueChanged(double)));
    disconnect(ui->spinBoxStandbyPowerQ, SIGNAL(valueChanged(double)), this, SLOT(spinBoxStandbyPowerQ_valueChanged(double)));
    disconnect(ui->dSpinBoxWarmFlashTimeQ, SIGNAL(valueChanged(double)), this, SLOT(dSpinBoxWarmFlashTimeQ_valueChanged(double)));
}

void SettingDialog::eventFilterSetup()
{
    QObjectList objList = ui->dSpinBoxDryingRangeIR->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxFrontRange->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxHeatTime1IR->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxHeatTime1Q->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxHeatTime2IR->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxHeatTime2Q->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxRearRange->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxDryPowerQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxFrontSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxRearSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxStrokCount->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxSBStrokCount->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxFlDwellTime->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxSqDwellTime->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxDryTimeQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxStandbyTimeQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxStepbackDryTimeQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxTemperatureSetQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxWarmFlashTimeQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxStandbyPowerQ->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
}

void SettingDialog::temperatureSensoreChanged(bool tempSens)
{
    this->withTemperatureSensor = tempSens;
    this->hide();
    this->show();
}

bool SettingDialog::event(QEvent *e)
{
    if((e->type()==QEvent::WindowDeactivate)|((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave)))
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}

bool SettingDialog::eventFilter(QObject *watched, QEvent *event)
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

void SettingDialog::showEvent(QShowEvent *ev)
{
    acceptEnable = true;
    ui->labelSP1->setVisible(!((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASE)));
    ui->labelSP2->setVisible(!((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASE)));
    ui->spinBoxFrontSpeed->setVisible(!((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                                        |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                                        |(MachineSettings::getMachineType() == MachineSettings::TitanASE)));
    ui->spinBoxRearSpeed->setVisible(!((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                                       |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                                       |(MachineSettings::getMachineType() == MachineSettings::TitanASE)));

    ui->labelRL1->setVisible((MachineSettings::getMachineType() == MachineSettings::VoltServo));
    ui->labelRL2->setVisible((MachineSettings::getMachineType() == MachineSettings::VoltServo));
    ui->dSpinBoxFrontRange->setVisible((MachineSettings::getMachineType() == MachineSettings::VoltServo));
    ui->dSpinBoxRearRange->setVisible((MachineSettings::getMachineType() == MachineSettings::VoltServo));

    ui->labelDwT1->setVisible(((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASE)
                               |(MachineSettings::getMachineType() == MachineSettings::Vector)));
    ui->labelDwT2->setVisible(((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                               |(MachineSettings::getMachineType() == MachineSettings::TitanASE)
                               |(MachineSettings::getMachineType() == MachineSettings::Vector)));
    ui->dSpinBoxFlDwellTime->setVisible(((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                                        |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                                        |(MachineSettings::getMachineType() == MachineSettings::TitanASE)
                                         |(MachineSettings::getMachineType() == MachineSettings::Vector)));
    ui->dSpinBoxSqDwellTime->setVisible(((MachineSettings::getMachineType() == MachineSettings::TitanAAA)
                                       |(MachineSettings::getMachineType() == MachineSettings::TitanASA)
                                       |(MachineSettings::getMachineType() == MachineSettings::TitanASE)
                                         |(MachineSettings::getMachineType() == MachineSettings::Vector)));

    ui->widgetQuartzWithoutTempSensor->setVisible(!withTemperatureSensor);
    ui->widgetQuartzWithTempSensor->setVisible(withTemperatureSensor);

    ev->accept();
}


void SettingDialog::on_toolButtonPlast_clicked()
{

}

void SettingDialog::on_toolButtonFL_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::FL>>8));
    cmdArr.append((char)(HeadSetting::FL&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonMoveRear_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::MoveRear>>8));
    cmdArr.append((char)(HeadSetting::MoveRear&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonFL_SQ_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::SQ_FL>>8));
    cmdArr.append((char)(HeadSetting::SQ_FL&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonMoveFront_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::MoveFront>>8));
    cmdArr.append((char)(HeadSetting::MoveFront&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonMTPMove_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::MPT_Move>>8));
    cmdArr.append((char)(HeadSetting::MPT_Move&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonSQ_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::SQ>>8));
    cmdArr.append((char)(HeadSetting::SQ&0x00FF));
    cmdArr.append((char)(1>>8));
    cmdArr.append((char)(1&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonMoveTest_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)((int)HeadSetting::MoveTest>>8));
    cmdArr.append((char)((int)HeadSetting::MoveTest&0x00FF));
    cmdArr.append((char)(0));
    cmdArr.append((char)(1));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::on_toolButtonPressure_clicked()
{

}

void SettingDialog::on_toolButtonHoldOn_clicked()
{

}

void SettingDialog::on_pushButtonCopyToAll_clicked()
{
    HeadSetting hSttg;

    hSttg.headParam.powerOn = ui->pButtonHeadOnOff->isChecked();
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        hSttg.headParam.headType = HeadSetting::PrintHead;
        hSttg.headParam.speedRear = ui->spinBoxRearSpeed->value();
        hSttg.headParam.speedFront = ui->spinBoxFrontSpeed->value();
        hSttg.headParam.limitFront = ui->dSpinBoxFrontRange->value()*10;
        hSttg.headParam.limitRear = ui->dSpinBoxRearRange->value()*10;
        hSttg.headParam.stroksCount = ui->spinBoxStrokCount->value();
        break;
    }
    case 1:
    {
        hSttg.headParam.headType = HeadSetting::QuartzHead;
        hSttg.headParam.heatTime1 = ui->dSpinBoxHeatTime1Q->value()*10;
        hSttg.headParam.heatTime2 = ui->dSpinBoxHeatTime2Q->value()*10;
        hSttg.headParam.heatPower = ui->spinBoxDryPowerQ->value();
        break;
    }
    case 2:
    {
        hSttg.headParam.headType = HeadSetting::InfraRedHead;
        hSttg.headParam.heatTime1 = ui->dSpinBoxHeatTime1IR->value()*10;
        hSttg.headParam.heatTime2 = ui->dSpinBoxHeatTime2IR->value()*10;
        hSttg.headParam.limitFront = ui->dSpinBoxDryingRangeIR->value()*10;
        break;
    }
    }
    emit this->setParamsToAll(this->index, hSttg.headParam.toByteArray());
    this->hide();
}

void SettingDialog::tabWidget_currentChanged(int index)
{
    QByteArray cmdArr;
    int data = index;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeadType>>8));
    cmdArr.append((char)(HeadSetting::HeadHeadType&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::spinBoxRearSpeed_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadSpeedRear>>8));
    cmdArr.append((char)(HeadSetting::HeadSpeedRear&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxRearRange_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadRangeLimit1>>8));
    cmdArr.append((char)(HeadSetting::HeadRangeLimit1&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::spinBoxFrontSpeed_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadSpeedFront>>8));
    cmdArr.append((char)(HeadSetting::HeadSpeedFront&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxFrontRange_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadRangeLimit2>>8));
    cmdArr.append((char)(HeadSetting::HeadRangeLimit2&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::spinBoxStrokCount_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadStroksCount>>8));
    cmdArr.append((char)(HeadSetting::HeadStroksCount&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::spinBoxSBStrokCount_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadSBStroksCount>>8));
    cmdArr.append((char)(HeadSetting::HeadSBStroksCount&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxHeatTime1Q_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatTime1Q>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatTime1Q&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxHeatTime2Q_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatTime2Q>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatTime2Q&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::spinBoxDryPowerQ_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatPower>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatPower&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxStepbackDryTimeQ_valueChanged(double arg1)
{

}

void SettingDialog::dSpinBoxTemperatureSetQ_valueChanged(double arg1)
{

}

void SettingDialog::dSpinBoxDryTimeQ_valueChanged(double arg1)
{

}

void SettingDialog::spinBoxStandbyPowerQ_valueChanged(double arg1)
{

}

void SettingDialog::dSpinBoxStandbyTimeQ_valueChanged(double arg1)
{

}

void SettingDialog::dSpinBoxWarmFlashTimeQ_valueChanged(double arg1)
{

}

void SettingDialog::dSpinBoxHeatTime1IR_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatTime1IR>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatTime1IR&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxHeatTime2IR_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatTime2IR>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatTime2IR&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxDryingRangeIR_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadHeatDryRange>>8));
    cmdArr.append((char)(HeadSetting::HeadHeatDryRange&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxFlDwellTime_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadFlDwellTime>>8));
    cmdArr.append((char)(HeadSetting::HeadFlDwellTime&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}

void SettingDialog::dSpinBoxSqDwellTime_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)>>8));
    cmdArr.append((char)((HeadSetting::HeadDeviceAdrOffcet+this->index)&0x00FF));
    cmdArr.append((char)(HeadSetting::HeadSqDwellTime>>8));
    cmdArr.append((char)(HeadSetting::HeadSqDwellTime&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(this->index, cmdArr);
}
