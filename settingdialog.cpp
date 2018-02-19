#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(HeadSetting hSttg, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    setStyleSheet(QString(("color: #ABEFF6;"
                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
                           "selection-color: yellow;"
                           "selection-background-color: blue;")));
    ui->tabWidget->setStyleSheet("QTabBar::tab:first:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                                 "QTabBar::tab:first:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #2050A0, stop: 0.8 #203070,stop: 1.0 #202030);}"
                                 "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);}"
                                 "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #AE385B, stop: 0.8 #693359,stop: 1.0 #681030);}");

    this->index = index;

    ui->tabWidget->setCurrentIndex(hSttg.headParameters.headType);
    ui->lcdNumberHeadNo->display(index);

    ui->pButtonHeadOnOff->setChecked(hSttg.headParameters.powerOn&0x01);

    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        ui->spinBoxRearSpeed->setValue(hSttg.headParameters.speedRear);
        ui->spinBoxFrontSpeed->setValue(hSttg.headParameters.speedFront);
        ui->dSpinBoxFrontRange->setValue(hSttg.headParameters.limitFront/10.);
        ui->dSpinBoxRearRange->setValue(hSttg.headParameters.limitRear/10.);
        ui->spinBoxStrokCount->setValue(hSttg.headParameters.stroksCount);
        break;
    }
    case 1:
    {
        ui->dSpinBoxHeatTime1Q->setValue(hSttg.headParameters.heatTime1/10.);
        ui->dSpinBoxHeatTime2Q->setValue(hSttg.headParameters.heatTime2/10.);
        ui->spinBoxDryPowerQ->setValue(hSttg.headParameters.heatPower);
        break;
    }
    case 2:
    {
        ui->dSpinBoxHeatTime1IR->setValue(hSttg.headParameters.heatTime1/10.);
        ui->dSpinBoxHeatTime2IR->setValue(hSttg.headParameters.heatTime2/10.);
        ui->dSpinBoxDryingRangeIR->setValue(hSttg.headParameters.limitFront/10.);
        break;
    }
    }
//    this->installEventFilter(this);

    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(accepted()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pButtonHeadNoInc, SIGNAL(clicked(bool)), this, SLOT(pButtonIncClkd()));
    connect(ui->pButtonHeadNoDec, SIGNAL(clicked(bool)), this, SLOT(pButtonDecClkd()));

    connect(ui->pushButtonCopyToAll, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonCopyToAll_clicked()));
    connect(ui->pushButtonPlast, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonPlast_clicked()));
    connect(ui->pushButtonMoveFront, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonMoveFront_clicked()));
    connect(ui->pushButtonMoveRear, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonMoveRear_clicked()));
    connect(ui->pushButtonMoveTest, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonMoveTest_clicked()));
    connect(ui->pushButtonFL, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonFL_clicked()));
    connect(ui->pushButtonFL_SQ, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonFL_SQ_clicked()));
    connect(ui->pushButtonSQ, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonSQ_clicked()));
    connect(ui->pushButtonMTPMove, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonMTPMove_clicked()));
    connect(ui->pushButtonPressure, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonPressure_clicked()));
    connect(ui->pushButtonHoldOn, SIGNAL(clicked(bool)), this, SLOT(on_pushButtonHoldOn_clicked()));

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setHeadParams(HeadSetting hSttg, int index)
{
    this->index = index;

    ui->tabWidget->setCurrentIndex(hSttg.headParameters.headType);
    ui->lcdNumberHeadNo->display(index);

    ui->pButtonHeadOnOff->setChecked(hSttg.headParameters.powerOn&0x01);

    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        ui->spinBoxRearSpeed->setValue(hSttg.headParameters.speedRear);
        ui->spinBoxFrontSpeed->setValue(hSttg.headParameters.speedFront);
        ui->dSpinBoxFrontRange->setValue(hSttg.headParameters.limitFront/10.);
        ui->dSpinBoxRearRange->setValue(hSttg.headParameters.limitRear/10.);
        ui->spinBoxStrokCount->setValue(hSttg.headParameters.stroksCount);
        break;
    }
    case 1:
    {
        ui->dSpinBoxHeatTime1Q->setValue(hSttg.headParameters.heatTime1/10.);
        ui->dSpinBoxHeatTime2Q->setValue(hSttg.headParameters.heatTime2/10.);
        ui->spinBoxDryPowerQ->setValue(hSttg.headParameters.heatPower);
        break;
    }
    case 2:
    {
        ui->dSpinBoxHeatTime1IR->setValue(hSttg.headParameters.heatTime1/10.);
        ui->dSpinBoxHeatTime2IR->setValue(hSttg.headParameters.heatTime2/10.);
        ui->dSpinBoxDryingRangeIR->setValue(hSttg.headParameters.limitFront/10.);
        break;
    }
    }
}

void SettingDialog::accepted()
{
    HeadSetting hSttg;

    hSttg.headParameters.powerOn = ui->pButtonHeadOnOff->isChecked();
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        hSttg.headParameters.headType = HeadSetting::PrintHead;
        hSttg.headParameters.speedRear = ui->spinBoxRearSpeed->value();
        hSttg.headParameters.speedFront = ui->spinBoxFrontSpeed->value();
        hSttg.headParameters.limitFront = ui->dSpinBoxFrontRange->value()*10;
        hSttg.headParameters.limitRear = ui->dSpinBoxRearRange->value()*10;
        hSttg.headParameters.stroksCount = ui->spinBoxStrokCount->value();
        break;
    }
    case 1:
    {
        hSttg.headParameters.headType = HeadSetting::QuartzHead;
        hSttg.headParameters.heatTime1 = ui->dSpinBoxHeatTime1Q->value()*10;
        hSttg.headParameters.heatTime2 = ui->dSpinBoxHeatTime2Q->value()*10;
        hSttg.headParameters.heatPower = ui->spinBoxDryPowerQ->value();
        break;
    }
    case 2:
    {
        hSttg.headParameters.headType = HeadSetting::InfraRedHead;
        hSttg.headParameters.heatTime1 = ui->dSpinBoxHeatTime1IR->value()*10;
        hSttg.headParameters.heatTime2 = ui->dSpinBoxHeatTime2IR->value()*10;
        hSttg.headParameters.limitFront = ui->dSpinBoxDryingRangeIR->value()*10;
        break;
    }
    }
    emit this->accept(this->index, hSttg.headParameters.toByteArray());
    this->hide();

}

void SettingDialog::pButtonIncClkd()
{
    this->accepted();

    emit this->changeNumber(this->index+1);
}

void SettingDialog::pButtonDecClkd()
{
    this->accepted();

    emit this->changeNumber(this->index-1);
}

bool SettingDialog::event(QEvent *e)
{
    if(e->type()==QEvent::WindowDeactivate)
    {
        this->accepted();
    }
    return QWidget::event(e);
}


void SettingDialog::on_pushButtonPlast_clicked()
{

}

void SettingDialog::on_pushButtonFL_clicked()
{

}

void SettingDialog::on_pushButtonMoveRear_clicked()
{

}

void SettingDialog::on_pushButtonFL_SQ_clicked()
{

}

void SettingDialog::on_pushButtonMoveFront_clicked()
{

}

void SettingDialog::on_pushButtonMTPMove_clicked()
{

}

void SettingDialog::on_pushButtonSQ_clicked()
{

}

void SettingDialog::on_pushButtonMoveTest_clicked()
{

}

void SettingDialog::on_pushButtonPressure_clicked()
{

}

void SettingDialog::on_pushButtonHoldOn_clicked()
{

}

void SettingDialog::on_pushButtonCopyToAll_clicked()
{
    HeadSetting hSttg;

    hSttg.headParameters.powerOn = ui->pButtonHeadOnOff->isChecked();
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
    {
        hSttg.headParameters.headType = HeadSetting::PrintHead;
        hSttg.headParameters.speedRear = ui->spinBoxRearSpeed->value();
        hSttg.headParameters.speedFront = ui->spinBoxFrontSpeed->value();
        hSttg.headParameters.limitFront = ui->dSpinBoxFrontRange->value()*10;
        hSttg.headParameters.limitRear = ui->dSpinBoxRearRange->value()*10;
        hSttg.headParameters.stroksCount = ui->spinBoxStrokCount->value();
        break;
    }
    case 1:
    {
        hSttg.headParameters.headType = HeadSetting::QuartzHead;
        hSttg.headParameters.heatTime1 = ui->dSpinBoxHeatTime1Q->value()*10;
        hSttg.headParameters.heatTime2 = ui->dSpinBoxHeatTime2Q->value()*10;
        hSttg.headParameters.heatPower = ui->spinBoxDryPowerQ->value();
        break;
    }
    case 2:
    {
        hSttg.headParameters.headType = HeadSetting::InfraRedHead;
        hSttg.headParameters.heatTime1 = ui->dSpinBoxHeatTime1IR->value()*10;
        hSttg.headParameters.heatTime2 = ui->dSpinBoxHeatTime2IR->value()*10;
        hSttg.headParameters.limitFront = ui->dSpinBoxDryingRangeIR->value()*10;
        break;
    }
    }
    emit this->setParamsToAll(this->index, hSttg.headParameters.toByteArray());
    this->hide();
}
