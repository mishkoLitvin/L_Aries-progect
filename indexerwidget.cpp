#include "indexerwidget.h"
#include "ui_indexerwidget.h"

IndexerWidget::IndexerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerWidget)
{
    ui->setupUi(this);

    pButtonSets = new QPushButton(this);

    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(30,30);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicSlot()));

    pButtonSets->setIcon(QIcon(":/new/icons/icons/settings.png"));
}

IndexerWidget::~IndexerWidget()
{
    delete ui;
}

void IndexerWidget::on_pButtonLock_clicked()
{
    if(ui->pButtonLock->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::IndLock);
        bArr.append((char)0xFF);
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
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::IndLock);
        bArr.append((char)0xFF);
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
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::MoveFull);
        bArr.append((char)0xFF);
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
    }
    else
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::MoveHalf);
        bArr.append((char)0xFF);
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
    if(ui->pButtonAuto->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::Auto);
        bArr.append((char)0xFF);
        emit this->sendCommand(bArr);
        ui->pButtonAuto->setText("Auto");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/play.png"));
        ui->pButtonPrint->setText("Print Auto");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/play.png"));
    }
    else
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::Manual);
        bArr.append((char)0xFF);
        emit this->sendCommand(bArr);
        ui->pButtonAuto->setText("Manual");
        ui->pButtonAuto->setIcon(QIcon(":/new/icons/icons/playP.png"));
        ui->pButtonPrint->setText("Print Manual");
        ui->pButtonPrint->setIcon(QIcon(":/new/icons/icons/playP.png"));
    }
}

void IndexerWidget::on_pButtonPrint_clicked()
{
    if(ui->pButtonPrint->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        if(ui->pButtonAuto->isChecked())
            bArr.append((char)IndexerLiftSettings::PrintAuto);
        else
            bArr.append((char)IndexerLiftSettings::PrintManual);
        bArr.append((char)0xFF);
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
    }
    else
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x01);
        bArr.append((char)IndexerLiftSettings::PrintStop);
        bArr.append((char)0xFF);
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

    }
}

void IndexerWidget::on_pButtonMoveLeft_clicked()
{
    QByteArray bArr;
    bArr.append((char)0xEE);
    bArr.append((char)0x01);
    if(ui->pButtonMove->isChecked())
        bArr.append((char)IndexerLiftSettings::MoveLeft);
    else
        bArr.append((char)IndexerLiftSettings::MoveLeftHalf);
    bArr.append((char)0xFF);
    emit this->sendCommand(bArr);
}

void IndexerWidget::on_pButtonMoveUp_clicked()
{
    if(ui->pButtonMoveUp->isChecked())
    {
        QByteArray bArr;
        bArr.append((char)0xEE);
        bArr.append((char)0x02);
        bArr.append((char)IndexerLiftSettings::MoveUp);
        bArr.append((char)0xFF);
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
        bArr.append((char)0xEE);
        bArr.append((char)0x02);
        bArr.append((char)IndexerLiftSettings::MoveUp);
        bArr.append((char)0xFF);
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
    bArr.append((char)0xEE);
    bArr.append((char)0x01);
    if(ui->pButtonMove->isChecked())
        bArr.append((char)IndexerLiftSettings::MoveRight);
    else
        bArr.append((char)IndexerLiftSettings::MoveRightHalf);
    bArr.append((char)0xFF);
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
