#include "indexerwidget.h"
#include "ui_indexerwidget.h"

IndexerWidget::IndexerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerWidget)
{
    ui->setupUi(this);

    pButtonSets = new QPushButton(this);

    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(50,50);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicSlot()));

    pButtonSets->setIcon(QIcon(":/new/icons/icons/settings.png"));

    isAutoPrintEnable = false;
    machineState.all = 0x0000;
}

IndexerWidget::~IndexerWidget()
{
    delete ui;
}

void IndexerWidget::printFinish()
{
    ui->pButtonPrint->setChecked(true);
    ui->pButtonPrint->click();
}

void IndexerWidget::printStart()
{
    ui->pButtonPrint->setChecked(false);
    ui->pButtonPrint->click();
}

bool IndexerWidget::getIsAutoPrint()
{
    return this->isAutoPrintEnable;
}

void IndexerWidget::on_pButtonLock_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.lockUnLock = ui->pButtonLock->isChecked();
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    bArr.append((char)((machineState.all)>>8));
    bArr.append((char)((machineState.all)&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonLock->isChecked())
    {
        ui->pButtonLock->setText("Lock");
        ui->pButtonLock->setIcon(QIcon(":/new/icons/icons/lock.png"));
        ui->pButtonPrint->setHidden(true);
        ui->pButtonAuto->setHidden(true);
        ui->pButtonMove->setHidden(true);
        ui->pButtonMoveLeft->setHidden(true);
        ui->pButtonMoveRight->setHidden(true);
        ui->pButtonMoveUp->setHidden(true);
        pButtonSets->setHidden(true);
    }
    else
    {
        ui->pButtonLock->setText("Unlock");
        ui->pButtonLock->setIcon(QIcon(":/new/icons/icons/unlock.png"));
        ui->pButtonPrint->setHidden(false);
        ui->pButtonAuto->setHidden(false);
        ui->pButtonMove->setHidden(false);
        ui->pButtonMoveLeft->setHidden(false);
        ui->pButtonMoveRight->setHidden(false);
        ui->pButtonMoveUp->setHidden(false);
        pButtonSets->setHidden(false);
    }
}

void IndexerWidget::on_pButtonMove_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.halfFull = (ui->pButtonMove->isChecked());
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    bArr.append((char)((machineState.all)>>8));
    bArr.append((char)((machineState.all)&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonMove->isChecked())
    {
        ui->pButtonMove->setText("Move full");
        ui->pButtonMove->setIcon(QIcon(":/arrows/icons/arrows/arrowRPart2Round.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(":/arrows/icons/arrows/arrowLPart.png"));
        ui->pButtonMoveRight->setIcon(QIcon(":/arrows/icons/arrows/arrowRPart.png"));
        ui->pButtonLock->setHidden(true);
        ui->pButtonAuto->setHidden(true);
        ui->pButtonMoveUp->setHidden(true);
        ui->pButtonPrint->setHidden(true);
        pButtonSets->setHidden(true);
        halfCounter = 0;
    }
    else
    {
        ui->pButtonMove->setText("Move half");
        ui->pButtonMove->setIcon(QIcon(":/arrows/icons/arrows/arrowRPartRound.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(":/arrows/icons/arrows/arrowL.png"));
        ui->pButtonMoveRight->setIcon(QIcon(":/arrows/icons/arrows/arrowR.png"));
        ui->pButtonLock->setHidden(false);
        ui->pButtonAuto->setHidden(false);
        ui->pButtonMoveUp->setHidden(false);
        ui->pButtonPrint->setHidden(false);
        pButtonSets->setHidden(false);
    }
}

void IndexerWidget::on_pButtonAuto_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.manualAuto = ui->pButtonAuto->isChecked();
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    bArr.append((char)((machineState.all)>>8));
    bArr.append((char)((machineState.all)&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonAuto->isChecked())
    {
        ui->pButtonAuto->setText("Manual");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/playP.png"));
        ui->pButtonPrint->setText("Print Auto");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/play.png"));
        isAutoPrintEnable = true;
    }
    else
    {
        ui->pButtonAuto->setText("Auto");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/play.png"));
        ui->pButtonPrint->setText("Print Manual");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/playP.png"));
        isAutoPrintEnable = false;
    }
}

void IndexerWidget::on_pButtonPrint_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.printStop = ui->pButtonPrint->isChecked();
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    bArr.append((char)((machineState.all)>>8));
    bArr.append((char)((machineState.all)&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonPrint->isChecked())
    {

        ui->pButtonPrint->setText("Stop");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/stop.png"));
        ui->pButtonLock->setHidden(true);
        ui->pButtonAuto->setHidden(true);
        ui->pButtonMove->setHidden(true);
        ui->pButtonMoveLeft->setHidden(true);
        ui->pButtonMoveRight->setHidden(true);
        ui->pButtonMoveUp->setHidden(true);
        pButtonSets->setHidden(true);
        emit this->startPrint(this->isAutoPrintEnable);
    }
    else
    {
        if(ui->pButtonAuto->isChecked())
        {
            ui->pButtonPrint->setText("Print Auto");
            ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/play.png"));
        }
        else
        {
            ui->pButtonPrint->setText("Print Manual");
            ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/playP.png"));
        }
        ui->pButtonLock->setHidden(false);
        ui->pButtonAuto->setHidden(false);
        ui->pButtonMove->setHidden(false);
        ui->pButtonMoveLeft->setHidden(false);
        ui->pButtonMoveRight->setHidden(false);
        ui->pButtonMoveUp->setHidden(false);
        pButtonSets->setHidden(false);
        emit this->stopPrint();
    }
}

void IndexerWidget::on_pButtonMoveLeft_clicked()
{
    QByteArray bArr;
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    if(ui->pButtonMove->isChecked())
    {
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
        halfCounter--;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    }
    bArr.append((char)(1>>8));
    bArr.append((char)(1&0x00FF));
    int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);
}

void IndexerWidget::on_pButtonMoveUp_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.upDown = ui->pButtonMoveUp->isChecked();
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
    bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    bArr.append((char)((machineState.all)>>8));
    bArr.append((char)((machineState.all)&0x00FF));
    data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonMoveUp->isChecked())
    {
        ui->pButtonMoveUp->setText("Down");
        ui->pButtonMoveUp->setIcon(QIcon(":/arrows/icons/arrows/arrowDPart.png"));
        ui->pButtonLock->setHidden(true);
        ui->pButtonAuto->setHidden(true);
        ui->pButtonMove->setHidden(true);
        ui->pButtonMoveLeft->setHidden(true);
        ui->pButtonMoveRight->setHidden(true);
        ui->pButtonPrint->setHidden(true);
        pButtonSets->setHidden(true);
    }
    else
    {
        ui->pButtonMoveUp->setText("Up");
        ui->pButtonMoveUp->setIcon(QIcon(":/arrows/icons/arrows/arrowUPart.png"));
        ui->pButtonLock->setHidden(false);
        ui->pButtonAuto->setHidden(false);
        ui->pButtonMove->setHidden(false);
        ui->pButtonMoveLeft->setHidden(false);
        ui->pButtonMoveRight->setHidden(false);
        ui->pButtonPrint->setHidden(false);
        pButtonSets->setHidden(false);
    }
}

void IndexerWidget::on_pButtonMoveRight_clicked()
{
    QByteArray bArr;
    bArr.append((char)(MachineSettings::MasterDevice>>8));
    bArr.append((char)(MachineSettings::MasterDevice&0x00FF));
    if(ui->pButtonMove->isChecked())
    {
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
        halfCounter++;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand>>8));
        bArr.append((char)(MachineSettings::MasterIndexLiftCommand&0x00FF));
    }
    bArr.append((char)(1>>8));
    bArr.append((char)(1&0x00FF));
    int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
    bArr.append((char)(data>>8));
    bArr.append((char)(data&0x00FF));
    emit this->sendCommand(bArr);
}

void IndexerWidget::settingPButtonClicSlot()
{
    emit this->settingButtonCliced();
}

void IndexerWidget::resizeEvent(QResizeEvent *e)
{
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    e->accept();
}
