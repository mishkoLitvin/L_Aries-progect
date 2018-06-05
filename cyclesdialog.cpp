#include "cyclesdialog.h"
#include "ui_cyclesdialog.h"

CyclesDialog::CyclesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CyclesDialog)
{
    ui->setupUi(this);
}

CyclesDialog::CyclesDialog(int headCount, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CyclesDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("QPushButton:!checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #A0A0C0, stop: 0.8 #90907F,stop: 1.0 #108020)}"
                        "QPushButton:checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #40D080, stop: 0.8 #10B07F,stop: 1.0 #30A040)}"
                        "QLabel{background-color: rgba(255, 255, 255, 200)}"
                        "QLabel{font: 14px bold italic large \"Serif\"}");
    ui->pButtonON->setStyleSheet("QToolButton:!checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #A0A0C0, stop: 0.8 #90907F,stop: 1.0 #108020)}"
                                 "QToolButton:checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #40D080, stop: 0.8 #10B07F,stop: 1.0 #30A040)}");


    this->headCount = headCount;

    cycleSettings = new QSettings("./cycles.ini", QSettings::IniFormat);

    int i, j;
    QObjectList objList;

    for(i = 0; i<headCount; i++)
    {
        this->spinBoxList.append(new QDoubleSpinBox(ui->widgetSpinPlace));
        spinBoxList[i]->setRange(0, 10);
        spinBoxList[i]->setDecimals(0);
        spinBoxList[i]->resize(80, 65);
        spinBoxList[i]->setStyleSheet("QDoubleSpinBox::up-button {"
                                      "width: 50px;"
                                      "height: 30px;"
                                      "subcontrol-origin: border;"
                                      "subcontrol-position: top right;}"
                                      "QDoubleSpinBox::down-button {"
                                      "width: 50px;"
                                      "height: 30px;"
                                      "subcontrol-origin: border;"
                                      "subcontrol-position: bottom right;}");
        spinBoxList[i]->children()[0]->installEventFilter(this);

        this->labelList.append(new QLabel(ui->widgetSpinPlace));
        labelList[i]->setText(QString::number(i)+":");
        if(i<10)
            labelList[i]->resize(15,25);
        else
            labelList[i]->resize(25,25);


    }

    for(i = 0; i<8; i++)
    {
        cycleValues.append(new int[headCount]);
        for(j = 0; j<headCount; j++)
            cycleValues[i][j] = cycleSettings->value("CYCLE_"+QString::number(i)+
                                                     "/HEAD_"+QString::number(j), 0).toInt();
    }
    cycleState = cycleSettings->value("CYCLE_STATE").toInt();
    ui->pButtonC1->setChecked(cycleState&(1<<0));
    ui->pButtonC2->setChecked(cycleState&(1<<1));
    ui->pButtonC3->setChecked(cycleState&(1<<2));
    ui->pButtonC4->setChecked(cycleState&(1<<3));
    ui->pButtonC5->setChecked(cycleState&(1<<4));
    ui->pButtonC6->setChecked(cycleState&(1<<5));
    ui->pButtonC7->setChecked(cycleState&(1<<6));
    ui->pButtonC8->setChecked(cycleState&(1<<7));
    ui->pButtonON->setChecked(cycleState&(1<<0));

    lastCycleSel = 0;
    this->loadValues(cycleValues[0]);
    ui->lcdNumber->display(1);


}

CyclesDialog::~CyclesDialog()
{
    delete ui;
}

void CyclesDialog::showEvent(QShowEvent *ev)
{
    float sinCoef, cosCoef;
    float x0_hb, y0_hb, R;
    x0_hb = ui->widgetSpinPlace->width()/2-spinBoxList[0]->width()/2;
    y0_hb = ui->widgetSpinPlace->height()/2-spinBoxList[0]->height()/2;

    if(ui->widgetSpinPlace->height()<ui->widgetSpinPlace->width())
        R = ui->widgetSpinPlace->height()/2-spinBoxList[0]->height()/2-10;
    else
        R = ui->widgetSpinPlace->width()/2-spinBoxList[0]->width()/2-10;

    int i;
    for(i = 0; i<headCount; i++)
    {
        sinCoef = sin(2.*3.1415926*i/headCount+3.1415926/2.+3.1415926/headCount);
        cosCoef = cos(2.*3.1415926*i/headCount+3.1415926/2.+3.1415926/headCount);

        spinBoxList[i]->move(x0_hb+(R)*cosCoef, y0_hb+(R)*sinCoef);
        labelList[i]->move(x0_hb+(R)*cosCoef, y0_hb-3+(R)*sinCoef);
    }
    if(QApplication::platformName() != "eglfs")
        this->resize(QSize(1024, 768));
    else
        this->setWindowState(Qt::WindowMaximized);
    ev->accept();
}

bool CyclesDialog::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease))
            |((QApplication::platformName() == "linuxfb")&(event->type()==QEvent::MouseButtonRelease)))
    {
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
    }
    return false;
}

void CyclesDialog::on_pButtonOK_clicked()
{
    headStateList.clear();
    int i, j;
    QByteArray cmdArr;
    uint16_t data;
    for(i = 0; i < cycleValues.length(); i++)
    {
        headStateList << 0;
        for(j = 0; j<headCount; j++)
        {
            headStateList[i] |= (((uint32_t)((bool)(cycleValues[i][j]>0)))<<j);
        }
        cmdArr.clear();
        data = (headStateList[i]&0x0000FFFF);
        cmdArr.append(IndexerLiftSettings::LiftDevice);
        cmdArr.append(Register::liftReg_SEQU1_L+i*2);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        data = CrcCalc::CalculateCRC16(cmdArr);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        emit this->sendCommand(cmdArr);

        cmdArr.clear();
        data = ((headStateList[i]>>16)&0x0000FFFF);
        cmdArr.append(IndexerLiftSettings::LiftDevice);
        cmdArr.append(Register::liftReg_SEQU1_H+i*2);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        data = CrcCalc::CalculateCRC16(cmdArr);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        emit this->sendCommand(cmdArr);
    }
    qDebug()<<headStateList;

    headStrokList.clear();
    for(j = 1; j<headCount; j++)
    {
        headStrokList << 0;
        for(i = 0; i < cycleValues.length(); i++)
        {
            headStrokList[j] |= (((uint32_t)((uint8_t)(cycleValues[i][j]))&0x0F)<<i*4);
        }
        cmdArr.clear();
        data = (headStrokList[j]&0x0000FFFF);
        cmdArr.append(HeadSetting::HeadDeviceAdrOffcet+j);
        cmdArr.append(Register::headReg_REVOLVER_STR_L);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        data = CrcCalc::CalculateCRC16(cmdArr);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        emit this->sendCommand(cmdArr);

        cmdArr.clear();
        data = ((headStrokList[j]>>16)&0x0000FFFF);
        cmdArr.append(HeadSetting::HeadDeviceAdrOffcet+j);
        cmdArr.append(Register::headReg_REVOLVER_STR_H);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        data = CrcCalc::CalculateCRC16(cmdArr);
        cmdArr.append((char)(data>>8));
        cmdArr.append((char)(data&0x00FF));
        emit this->sendCommand(cmdArr);
    }
    qDebug()<<headStrokList;

    this->accept();
}

void CyclesDialog::on_pButtonON_clicked()
{
    cycleState &= (~(1<<lastCycleSel));
    cycleState |= ((ui->pButtonON->isChecked())<<lastCycleSel);
    cycleSettings->setValue("CYCLE_STATE", cycleState);
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC1->setChecked(ui->pButtonON->isChecked());
        break;
    case 1:
        ui->pButtonC2->setChecked(ui->pButtonON->isChecked());
        break;
    case 2:
        ui->pButtonC3->setChecked(ui->pButtonON->isChecked());
        break;
    case 3:
        ui->pButtonC4->setChecked(ui->pButtonON->isChecked());
        break;
    case 4:
        ui->pButtonC5->setChecked(ui->pButtonON->isChecked());
        break;
    case 5:
        ui->pButtonC6->setChecked(ui->pButtonON->isChecked());
        break;
    case 6:
        ui->pButtonC7->setChecked(ui->pButtonON->isChecked());
        break;
    case 7:
        ui->pButtonC8->setChecked(ui->pButtonON->isChecked());
        break;
    default:
        break;
    }
}

void CyclesDialog::on_pButtonPrev_clicked()
{
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
        this->on_pButtonC8_clicked();
        break;
    case 1:
        ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
        this->on_pButtonC1_clicked();
        break;
    case 2:
        ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
        this->on_pButtonC2_clicked();
        break;
    case 3:
        ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
        this->on_pButtonC3_clicked();
        break;
    case 4:
        ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
        this->on_pButtonC4_clicked();
        break;
    case 5:
        ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
        this->on_pButtonC5_clicked();
        break;
    case 6:
        ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
        this->on_pButtonC6_clicked();
        break;
    case 7:
        ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
        this->on_pButtonC7_clicked();
        break;
    default:
        break;
    }
}

void CyclesDialog::on_pButtonNext_clicked()
{
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
        this->on_pButtonC2_clicked();
        break;
    case 1:
        ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
        this->on_pButtonC3_clicked();
        break;
    case 2:
        ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
        this->on_pButtonC4_clicked();
        break;
    case 3:
        ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
        this->on_pButtonC5_clicked();
        break;
    case 4:
        ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
        this->on_pButtonC6_clicked();
        break;
    case 5:
        ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
        this->on_pButtonC7_clicked();
        break;
    case 6:
        ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
        this->on_pButtonC8_clicked();
        break;
    case 7:
        ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
        this->on_pButtonC1_clicked();
        break;
    default:
        break;
    }
}

void CyclesDialog::on_pButtonC1_clicked()
{
    ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
    this->saveValues();
    lastCycleSel = 0;
    ui->pButtonON->setChecked(ui->pButtonC1->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC2_clicked()
{
    ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
    this->saveValues();
    lastCycleSel = 1;
    ui->pButtonON->setChecked(ui->pButtonC2->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC3_clicked()
{
    ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
    this->saveValues();
    lastCycleSel = 2;
    ui->pButtonON->setChecked(ui->pButtonC3->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC4_clicked()
{
    ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
    this->saveValues();
    lastCycleSel = 3;
    ui->pButtonON->setChecked(ui->pButtonC4->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC5_clicked()
{
    ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
    this->saveValues();
    lastCycleSel = 4;
    ui->pButtonON->setChecked(ui->pButtonC5->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC6_clicked()
{
    ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
    this->saveValues();
    lastCycleSel = 5;
    ui->pButtonON->setChecked(ui->pButtonC6->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);
}

void CyclesDialog::on_pButtonC7_clicked()
{
    ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
    this->saveValues();
    lastCycleSel = 6;
    ui->pButtonON->setChecked(ui->pButtonC7->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC8_clicked()
{
    ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
    this->saveValues();
    lastCycleSel = 7;
    ui->pButtonON->setChecked(ui->pButtonC8->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::loadValues(int *vals)
{
    int i;
    for(i = 0; i<spinBoxList.length(); i++)
        spinBoxList[i]->setValue(vals[i]);
}

void CyclesDialog::saveValues()
{
    int i;
    for(i = 0; i<spinBoxList.length(); i++)
    {
        cycleValues[lastCycleSel][i] = spinBoxList[i]->value();
        cycleSettings->setValue("CYCLE_"+QString::number(lastCycleSel)+
                                "/HEAD_"+QString::number(i), spinBoxList[i]->value());

    }
}


