#include "warmingwidget.h"
#include "ui_warmingwidget.h"

WarmingWidget::WarmingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarmingWidget)
{
    ui->setupUi(this);
}

WarmingWidget::~WarmingWidget()
{
    delete ui;
}

void WarmingWidget::setIconFolder(QString path)
{
    this->pathIcon = path;
    if(ui->toolButton->isChecked())
    {
        ui->toolButton->setIcon(QIcon(pathIcon+"/play.png"));
    }
    else
    {
        ui->toolButton->setIcon(QIcon(pathIcon+"/playP.png"));
    }
}

void WarmingWidget::setRegisters(Register *reg)
{
    this->registers = reg;
}

void WarmingWidget::on_toolButton_clicked()
{
    QByteArray bArr;
    int data;
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));

    data = IndexerLiftSettings::WarmStart_Stop;
    if(ui->toolButton->isChecked())
    {
        ui->toolButton->setText(tr("Stop"));
        ui->toolButton->setIcon(QIcon(pathIcon+"/stop.png"));
    }
    else
    {
        ui->toolButton->setText(tr("Start"));
        ui->toolButton->setIcon(QIcon(pathIcon+"/play.png"));
    }

    bArr.append(static_cast<char>((data)>>8));
    bArr.append(static_cast<char>((data)&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);
}

void WarmingWidget::on_dSpinBoxTime_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1*10;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::WarmingTime&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void WarmingWidget::on_dSpinBoxCycles_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1/**10*/;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::WarmingCycles&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void WarmingWidget::on_dSpinBoxTemperature_valueChanged(double arg1)
{
    QByteArray cmdArr;
    int data = arg1/**10*/;
    cmdArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice&0x00FF));
    cmdArr.append(static_cast<char>(IndexerLiftSettings::WarmingTemp&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void WarmingWidget::showEvent(QShowEvent *ev)
{
    ui->dSpinBoxTime->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_WARM_TIME)/10.);
    ui->dSpinBoxCycles->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_WARM_CYCLES));
    ui->dSpinBoxTemperature->setValue(registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_WARM_TEMP));
    bool sensor = false;
    int i;
    for(i = 1; i<registers->headRegListLen; i++)
    {
        sensor |= (registers->readReg(HeadSetting::HeadDeviceAdrOffcet+i, Register::headReg_CONFIG)&0x01);
    }
    ui->dSpinBoxCycles->setVisible(!sensor);
    ui->dSpinBoxTemperature->setVisible(sensor);
    ui->labelCycles->setVisible(!sensor);
    ui->labelCycles->setVisible(!sensor);
    ui->labelTemp->setVisible(sensor);
    ui->labelTemp->setVisible(sensor);
    ev->accept();
}
