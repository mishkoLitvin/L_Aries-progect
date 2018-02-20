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
                           "selection-background-color: blue;"
                           "font: 14px bold italic large \"Times New Roman\"")));

    ui->tabWidget->setStyleSheet("QTabBar::tab {color: white;}"
                                    "QTabBar::tab:first:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                                    "QTabBar::tab:first:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #2050A0, stop: 0.8 #203070,stop: 1.0 #202030);}"
                                    "QTabBar::tab:last:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);}"
                                    "QTabBar::tab:last:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #AE385B, stop: 0.8 #693359,stop: 1.0 #681030);}"
                                    "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #962AAD, stop: 0.8 #8C04A8,stop: 1.0 #6C0382);}"
                                    "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #77168B, stop: 0.8 #610A73,stop: 1.0 #4F025F);}"
                                 );
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


    numpad = new NumpadDialog();

    ui->dSpinBoxFrontRange->installEventFilter(this);
    ui->dSpinBoxFrontRange->objectName();
    QObjectList o_list = ui->dSpinBoxFrontRange->children();
    qDebug()<<o_list;
    for(int i = 0; i < o_list.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(o_list[i]);
        if(cast)
            cast->installEventFilter(this);
    }

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setHeadParams(HeadSetting hSttg, int index)
{
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
}

void SettingDialog::accepted()
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
        44;
//        this->accepted();
    }
    return QWidget::event(e);
}

bool SettingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        qDebug()<<"REQ"<<watched->objectName();
        numpad->show();
        connect(numpad, SIGNAL(valueSubmited(QString)), qobject_cast<QLineEdit*>(watched), SLOT(setText(QString)));
//        showNumpadDialog();
    }
    return false;
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
