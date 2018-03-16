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
}

IndexerWidget::~IndexerWidget()
{
    delete ui;
}

void IndexerWidget::manualPrintFinish()
{
    ui->pButtonPrint->click();
}

bool IndexerWidget::getIsAutoPrint()
{
    return this->isAutoPrintEnable;
}

void IndexerWidget::on_pButtonLock_clicked()
{
    if(ui->pButtonLock->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::IndUnLock>>8));
        bArr.append((char)(IndexerLiftSettings::IndUnLock&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::IndLock>>8));
        bArr.append((char)(IndexerLiftSettings::IndLock&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
    if(ui->pButtonMove->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::MoveHalf>>8));
        bArr.append((char)(IndexerLiftSettings::MoveHalf&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::MoveFull>>8));
        bArr.append((char)(IndexerLiftSettings::MoveFull&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
    int data;
    if(ui->pButtonAuto->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::Auto>>8));
        bArr.append((char)(IndexerLiftSettings::Auto&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

        ui->pButtonAuto->setText("Manual");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/playP.png"));
        ui->pButtonPrint->setText("Print Auto");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/play.png"));
        isAutoPrintEnable = true;
    }
    else
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::Manual>>8));
        bArr.append((char)(IndexerLiftSettings::Manual&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

        ui->pButtonAuto->setText("Auto");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/play.png"));
        ui->pButtonPrint->setText("Print Manual");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/playP.png"));
        isAutoPrintEnable = false;
    }
}

void IndexerWidget::on_pButtonPrint_clicked()
{
    if(ui->pButtonPrint->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        if(ui->pButtonAuto->isChecked())
        {
            bArr.append((char)(IndexerLiftSettings::PrintAuto>>8));
            bArr.append((char)(IndexerLiftSettings::PrintAuto&0x00FF));
        }
        else
        {
            bArr.append((char)(IndexerLiftSettings::PrintManual>>8));
            bArr.append((char)(IndexerLiftSettings::PrintManual&0x00FF));
        }
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
        bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::PrintStop>>8));
        bArr.append((char)(IndexerLiftSettings::PrintStop&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
    bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
    bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
    if(ui->pButtonMove->isChecked())
    {
        bArr.append((char)(IndexerLiftSettings::MoveLeft>>8));
        bArr.append((char)(IndexerLiftSettings::MoveLeft&0x00FF));
        halfCounter--;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        bArr.append((char)(IndexerLiftSettings::MoveLeft>>8));
        bArr.append((char)(IndexerLiftSettings::MoveLeft&0x00FF));
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
    if(ui->pButtonMoveUp->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::LiftDevice>>8));
        bArr.append((char)(IndexerLiftSettings::LiftDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::MoveUp>>8));
        bArr.append((char)(IndexerLiftSettings::MoveUp&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
        QByteArray bArr;
        bArr.append((char)(IndexerLiftSettings::LiftDevice>>8));
        bArr.append((char)(IndexerLiftSettings::LiftDevice&0x00FF));
        bArr.append((char)(IndexerLiftSettings::MoveDown>>8));
        bArr.append((char)(IndexerLiftSettings::MoveDown&0x00FF));
        bArr.append((char)(1>>8));
        bArr.append((char)(1&0x00FF));
        int data = CrcCalc::CalculateCRC16(0xFFFF, bArr);
        bArr.append((char)(data>>8));
        bArr.append((char)(data&0x00FF));
        emit this->sendCommand(bArr);

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
    bArr.append((char)(IndexerLiftSettings::IndexerDevice>>8));
    bArr.append((char)(IndexerLiftSettings::IndexerDevice&0x00FF));
    if(ui->pButtonMove->isChecked())
    {
        bArr.append((char)(IndexerLiftSettings::MoveRightHalf>>8));
        bArr.append((char)(IndexerLiftSettings::MoveRightHalf&0x00FF));
        halfCounter++;
        ui->pButtonMove->setHidden(halfCounter%2);
    }
    else
    {
        bArr.append((char)(IndexerLiftSettings::MoveRight>>8));
        bArr.append((char)(IndexerLiftSettings::MoveRight&0x00FF));
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
