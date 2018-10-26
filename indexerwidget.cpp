#include "indexerwidget.h"
#include "ui_indexerwidget.h"

IndexerWidget::IndexerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerWidget)
{
    ui->setupUi(this);

    pButtonSets = new QPushButton(this);

//    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(50,50);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);
    pButtonSets->setIconSize(QSize(40,40));
    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicSlot()));


    pButtonSets->setIcon(QIcon(pathIcon+"/settings.png"));
    pButtonSets->setVisible(true);

    isAutoPrintEnable = false;
    machineState.all = 0x0000;

    this->setState(0);
}

IndexerWidget::~IndexerWidget()
{
    delete ui;
}

void IndexerWidget::printFinish()
{
//    ui->pButtonPrint->setChecked(true);
//    ui->pButtonPrint->click();
}

void IndexerWidget::printStart()
{
//    ui->pButtonPrint->setChecked(false);
//    ui->pButtonPrint->click();
}

bool IndexerWidget::getIsAutoPrint()
{
    return this->isAutoPrintEnable;
}

void IndexerWidget::setIconFolder(QString path)
{
    pathIcon = path;
    ui->pButtonHome->setIcon(QIcon(pathIcon+"/home.png"));
    ui->pButtonReset->setIcon(QIcon(pathIcon+"/reset.png"));
    if(ui->pButtonLock->isChecked())
        ui->pButtonLock->setIcon(QIcon(pathIcon+"/unlock.png"));
    else
        ui->pButtonLock->setIcon(QIcon(pathIcon+"/lock.png"));

    if(ui->pButtonMove->isChecked())
    {
        ui->pButtonMove->setIcon(QIcon(pathIcon+"/arrows/arrowRPart2Round.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(pathIcon+"/arrows/arrowLPart.png"));
        ui->pButtonMoveRight->setIcon(QIcon(pathIcon+"/arrows/arrowRPart.png"));
    }
    else
    {
        ui->pButtonMove->setIcon(QIcon(pathIcon+"/arrows/arrowRPartRound.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(pathIcon+"/arrows/arrowL.png"));
        ui->pButtonMoveRight->setIcon(QIcon(pathIcon+"/arrows/arrowR.png"));
    }

    if(ui->pButtonAuto->isChecked())
    {
        ui->pButtonAuto->setIcon(QIcon(pathIcon+"/playP.png"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/play.png"));
    }
    else
    {
        ui->pButtonAuto->setIcon(QIcon(pathIcon+"/play.png"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/playP.png"));
    }
    if(ui->pButtonMoveUp->isChecked())
        ui->pButtonMoveUp->setIcon(QIcon(pathIcon+"/arrows/arrowDPart.png"));
    else
        ui->pButtonMoveUp->setIcon(QIcon(pathIcon+"/arrows/arrowUPart.png"));
    pButtonSets->setIcon(QIcon(pathIcon+"/settings.png"));

    ui->pButtonSkip->setIcon(QIcon(pathIcon+"/handNO.png"));
}

void IndexerWidget::clickButton(QByteArray data)
{
    switch (data[3]) {
    case IndexerLiftSettings::Machine_Reset:
        ui->pButtonReset->click();
        break;
    case IndexerLiftSettings::Machine_Home:
        ui->pButtonHome->click();
        break;
    case IndexerLiftSettings::IndexLock:
        ui->pButtonLock->click();
        break;
    case IndexerLiftSettings::MoveUp_Down:
        ui->pButtonMoveUp->click();
        break;
    case IndexerLiftSettings::MoveLeft:
        ui->pButtonMoveLeft->click();
        break;
    case IndexerLiftSettings::MoveRight:
        ui->pButtonMoveRight->click();
        break;
    case IndexerLiftSettings::MoveFull_Half:
        ui->pButtonMove->click();
        break;
    case IndexerLiftSettings::Auto_Manual:
        ui->pButtonAuto->click();
        break;
    case IndexerLiftSettings::PrintStart:
        ui->pButtonPrint->click();
        break;

    default:
        break;
    }
}

void IndexerWidget::setSettingEnable(bool enable)
{
    pButtonSets->setVisible(enable);
}

void IndexerWidget::on_pButtonLock_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.lockUnLock = ui->pButtonLock->isChecked();
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::IndexLock;
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonLock->isChecked())
    {
        ui->pButtonLock->setText(tr("Unlock"));
        ui->pButtonLock->setIcon(QIcon(pathIcon+"/unlock.png"));
    }
    else
    {
        ui->pButtonLock->setText(tr("Lock"));
        ui->pButtonLock->setIcon(QIcon(pathIcon+"/lock.png"));
    }
}

void IndexerWidget::on_pButtonMove_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.halfFull = (ui->pButtonMove->isChecked());
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::MoveFull_Half;
    bArr.append(static_cast<char>((data)>>8));
    bArr.append(static_cast<char>((data)&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonMove->isChecked())
    {
        ui->pButtonMove->setText(tr("Move full"));
        ui->pButtonMove->setIcon(QIcon(pathIcon+"/arrows/arrowRPart2Round.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(pathIcon+"/arrows/arrowLPart.png"));
        ui->pButtonMoveRight->setIcon(QIcon(pathIcon+"/arrows/arrowRPart.png"));
        halfCounter = 0;
    }
    else
    {
        ui->pButtonMove->setText(tr("Move half"));
        ui->pButtonMove->setIcon(QIcon(pathIcon+"/arrows/arrowRPartRound.png"));
        ui->pButtonMoveLeft->setIcon(QIcon(pathIcon+"/arrows/arrowL.png"));
        ui->pButtonMoveRight->setIcon(QIcon(pathIcon+"/arrows/arrowR.png"));
    }
}

void IndexerWidget::on_pButtonAuto_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.manualAuto = ui->pButtonAuto->isChecked();
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::Auto_Manual;
    bArr.append(static_cast<char>((data)>>8));
    bArr.append(static_cast<char>((data)&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonAuto->isChecked())
    {
        ui->pButtonAuto->setText(tr("Manual"));
        ui->pButtonAuto->setIcon(QIcon(pathIcon+"/playP.png"));
        ui->pButtonPrint->setText(tr("Print Auto"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/play.png"));
        isAutoPrintEnable = true;
    }
    else
    {
        ui->pButtonAuto->setText(tr("Auto"));
        ui->pButtonAuto->setIcon(QIcon(pathIcon+"/play.png"));
        ui->pButtonPrint->setText(tr("Print Manual"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/playP.png"));
        isAutoPrintEnable = false;
    }
}

void IndexerWidget::on_pButtonPrint_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.printStop = ui->pButtonPrint->isChecked();
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));

    if(ui->pButtonPrint->isChecked())
    {
        data = IndexerLiftSettings::PrintStart;
        ui->pButtonPrint->setText(tr("Stop"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/stop.png"));
        emit this->startPrint(this->isAutoPrintEnable);
    }
    else
    {
        data = IndexerLiftSettings::PrintStop;
        if(ui->pButtonAuto->isChecked())
        {
            ui->pButtonPrint->setText(tr("Print Auto"));
            ui->pButtonPrint->setIcon(QIcon(pathIcon+"/play.png"));
        }
        else
        {
            ui->pButtonPrint->setText(tr("Print Manual"));
            ui->pButtonPrint->setIcon(QIcon(pathIcon+"/playP.png"));
        }
        emit this->stopPrint();
    }

    bArr.append(static_cast<char>((data)>>8));
    bArr.append(static_cast<char>((data)&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);
    if(ui->pButtonPrint->isChecked())
        emit this->startPrint(this->isAutoPrintEnable);
    else
        emit this->stopPrint();
}

void IndexerWidget::on_pButtonMoveLeft_clicked()
{
    QByteArray bArr;
    int data;
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    if(ui->pButtonMove->isChecked())
    {
        data = IndexerLiftSettings::MoveLeftHalf;
        halfCounter--;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        data = IndexerLiftSettings::MoveLeft;
    }
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);
}

void IndexerWidget::on_pButtonMoveUp_clicked()
{
    QByteArray bArr;
    int data;
    machineState.bit.upDown = ui->pButtonMoveUp->isChecked();
    bArr.append(static_cast<char>(MachineSettings::MasterDevice&0x00FF));
    bArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    data = IndexerLiftSettings::MoveUp_Down;
    bArr.append(static_cast<char>((data)>>8));
    bArr.append(static_cast<char>((data)&0x00FF));
    data = CrcCalc::CalculateCRC16(bArr);
    bArr.append(static_cast<char>(data>>8));
    bArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(bArr);

    if(ui->pButtonMoveUp->isChecked())
    {
        ui->pButtonMoveUp->setText(tr("Down"));
        ui->pButtonMoveUp->setIcon(QIcon(pathIcon+"/arrows/arrowDPart.png"));
    }
    else
    {
        ui->pButtonMoveUp->setText(tr("Up"));
        ui->pButtonMoveUp->setIcon(QIcon(pathIcon+"/arrows/arrowUPart.png"));
    }
}

void IndexerWidget::on_pButtonMoveRight_clicked()
{
    QByteArray cmdArr;
    int data;
    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));

    if(ui->pButtonMove->isChecked())
    {
        data = IndexerLiftSettings::MoveRightHalf;
        halfCounter++;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        data = IndexerLiftSettings::MoveRight;
    }
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerWidget::on_pButtonReset_clicked()
{
//    QByteArray cmdArr;
//    uint16_t data;
//    data = IndexerLiftSettings::Machine_Reset;
//    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
//    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
//    cmdArr.append(static_cast<char>(data>>8));
//    cmdArr.append(static_cast<char>(data&0x00FF));
//    data = CrcCalc::CalculateCRC16(cmdArr);
//    cmdArr.append(static_cast<char>(data>>8));
//    cmdArr.append(static_cast<char>(data&0x00FF));
//    emit this->sendCommand(cmdArr);
    emit this->resetRequest();
}

void IndexerWidget::on_pButtonHome_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    data = IndexerLiftSettings::Machine_Home;
    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerWidget::settingPButtonClicSlot()
{
    emit this->settingButtonCliced();
}

void IndexerWidget::setState(u_int16_t state)
{
    uint8_t hb = ((state>>8)&0x00FF);

    MachineSettings::setMachineIdle(hb != 8);

    ui->pButtonReset->setVisible((hb == 0)|(hb == 17)|(hb == 18));
    ui->pButtonHome->setVisible(hb == 2);

    ui->pButtonPrint->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonLock->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonAuto->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonMove->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonMoveLeft->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonMoveRight->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonMoveUp->setVisible((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18));
    ui->pButtonSkip->setVisible(((hb != 0)&(hb != 2)&(hb != 17)&(hb != 18))&MachineSettings::getSoftwartSkipEn());

    ui->pButtonReset->setEnabled((hb == 0)|(hb == 17)|(hb == 18));
    ui->pButtonPrint->setEnabled((hb == 7)|(hb == 8));
    ui->pButtonLock->setEnabled(((hb == 4)|(hb == 7)));
    ui->pButtonAuto->setEnabled(hb == 7);
    ui->pButtonMove->setEnabled((hb == 7)&(hb != 11));
    ui->pButtonMoveLeft->setEnabled((hb == 7)|(hb == 11)|(hb == 10));
    ui->pButtonMoveRight->setEnabled((hb == 7)|(hb == 11)|(hb == 10));
    ui->pButtonMoveUp->setEnabled((hb == 7)|(hb == 9));
    ui->pButtonHome->setEnabled(hb == 2);
    ui->pButtonAir->setEnabled((hb == 7));
    ui->pButtonSkip->setEnabled((hb == 8));

    if(hb == 4)
    {
        ui->pButtonLock->setChecked(false);
        ui->pButtonLock->setText(tr("Lock"));
        ui->pButtonLock->setIcon(QIcon(pathIcon+"/lock.png"));
    }
    if(hb == 8)
    {
        ui->pButtonPrint->setChecked(true);
        ui->pButtonPrint->setText(tr("Stop"));
        ui->pButtonPrint->setIcon(QIcon(pathIcon+"/stop.png"));
    }
    if(hb == 7)
    {
        ui->pButtonPrint->setChecked(false);
        if(ui->pButtonAuto->isChecked())
        {
            ui->pButtonPrint->setText(tr("Print Auto"));
            ui->pButtonPrint->setIcon(QIcon(pathIcon+"/play.png"));
        }
        else
        {
            ui->pButtonPrint->setText(tr("Print Manual"));
            ui->pButtonPrint->setIcon(QIcon(pathIcon+"/playP.png"));
        }
    }
}

void IndexerWidget::resetWidget()
{
    ui->pButtonPrint->setChecked(false);
    ui->pButtonPrint->setText(tr("Print Auto"));
    ui->pButtonPrint->setIcon(QIcon(pathIcon+"/play.png"));
    ui->pButtonMoveUp->setChecked(false);
    ui->pButtonMoveUp->setText(tr("Up"));
    ui->pButtonMoveUp->setIcon(QIcon(pathIcon+"/arrows/arrowUPart.png"));
    ui->pButtonAuto->setChecked(false);
    ui->pButtonAuto->setText(tr("Auto"));
    ui->pButtonAuto->setIcon(QIcon(pathIcon+"/play.png"));
}

void IndexerWidget::resizeEvent(QResizeEvent *e)
{
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    e->accept();
    ui->pButtonAir->setVisible(false);
//                (MachineSettings::machineTypeStat == MachineSettings::TitanAAA)|
//                (MachineSettings::machineTypeStat == MachineSettings::TitanASA)|
//                (MachineSettings::machineTypeStat == MachineSettings::TitanASE));

}

void IndexerWidget::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void IndexerWidget::on_pButtonAir_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    data = IndexerLiftSettings::AirRelease;
    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}

void IndexerWidget::on_pButtonSkip_clicked()
{
    QByteArray cmdArr;
    uint16_t data;
    data = IndexerLiftSettings::SkipOne;
    cmdArr.append(static_cast<char>((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append(static_cast<char>(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append(static_cast<char>(data>>8));
    cmdArr.append(static_cast<char>(data&0x00FF));
    emit this->sendCommand(cmdArr);
}
