#include "headactivationdialog.h"
#include "ui_headactivationdialog.h"

HeadActivationDialog::HeadActivationDialog(int headCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadActivationDialog)
{
    ui->setupUi(this);

    this->headCount = headCount-1;

    this->setStyleSheet(parent->styleSheet());
    this->needSendReset = false;

    int i;
    for(i = 0; i<this->headCount; i++)
    {
        checkBoxList.append(new CheckBoxIndexed(i+1,tr("Head ")+QString::number(i+1),this));
        ui->gridLayout->addWidget(checkBoxList[i],
                                  i-(headCount/2*(i/(headCount/2))),
                                  i/(headCount/2));

        checkBoxList[i]->setStyleSheet(this->styleSheet()+
                                       "QCheckBox{border: 1px solid; border-radius: 3px;}"
                                       "QCheckBox::indicator{width: 55px;height: 55px;}"
                                       "QCheckBox::indicator::unchecked {image: url(:/new/icons/icons/Base/uncheck.png);}"
                                       "QCheckBox::indicator::checked {image: url(:/new/icons/icons/Base/check.png);}");
    }

    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(pushButtonOK_clicked()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(pushButtonCancel_clicked()));
}

HeadActivationDialog::~HeadActivationDialog()
{
    delete ui;
}

void HeadActivationDialog::setHeadActivState(uint32_t state)
{
    qDebug()<<"setHeadActivState"<<state;

    int i;
    this->headActivState = state;
    for(i = 0; i< checkBoxList.length(); i++)
    {
        checkBoxList[i]->setChecked(!(static_cast<bool>(state&(1<<i))));
    }
}

uint32_t HeadActivationDialog::getHeadActivState()
{
    return this->headActivState;
}

bool HeadActivationDialog::getHeadActivAtIndex(int index)
{
    return (!static_cast<bool>(headActivState&(1<<(index-1))));
}

void HeadActivationDialog::pushButtonOK_clicked()
{
    int i;
    for(i = 0; i< checkBoxList.length(); i++)
        disconnect(checkBoxList[i], SIGNAL(clicked(int)), this, SLOT(headStateChanged(int)));

    int data;
    if(this->needSendReset)
    {
        QByteArray bArr;
        data = 1;
        bArr.append(MachineSettings::MasterDevice);
        bArr.append(MachineSettings::MasterLastButton);
        bArr[2] = ((char)(data>>8));
        bArr[3] = ((char)(data&0x00FF));
        data = CrcCalc::CalculateCRC16(bArr);
        bArr[4] = ((char)(data>>8));
        bArr[5] = ((char)(data&0x00FF));
        emit this->sendCommand(bArr);
    }
    this->needSendReset = false;
    this->hide();
}

void HeadActivationDialog::pushButtonCancel_clicked()
{
    int i;
    for(i = 0; i< checkBoxList.length(); i++)
        disconnect(checkBoxList[i], SIGNAL(clicked(int)), this, SLOT(headStateChanged(int)));
    this->needSendReset = false;
    this->hide();
}

void HeadActivationDialog::headStateChanged(int index)
{
    ui->pushButtonCancel->setEnabled(false);
    this->needSendReset = true;

    int i;
    qDebug()<<"headActivState index b"<<index;
    headActivState = 0xFFFFFFFF;
    for(i = 0; i<checkBoxList.length(); i++)
    {
        if(checkBoxList[i]->isChecked())
            headActivState &= (~(1<<(i)));
        else
            headActivState |= (1<<(i));
    }
    qDebug()<<"headActivState a"<<QString::number(headActivState, 2);

    QByteArray cmdArr;
    uint16_t data;
    cmdArr.append((char)MachineSettings::MasterDevice);
    cmdArr.append((char)Register::masterReg_ACTIVHEAD_L);
    data = ((uint16_t)(headActivState&0x0000FFFF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);

    cmdArr.clear();
    cmdArr.append((char)MachineSettings::MasterDevice);
    cmdArr.append((char)Register::masterReg_ACTIVHEAD_H);
    data = ((uint16_t)((headActivState>>16)&0x0000FFFF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);


    cmdArr.clear();
    data = index+500;
    cmdArr.append((char)((MachineSettings::MasterDevice)&0x00FF));
    cmdArr.append((char)(MachineSettings::MasterLastButton&0x00FF));
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    data = CrcCalc::CalculateCRC16(cmdArr);
    cmdArr.append((char)(data>>8));
    cmdArr.append((char)(data&0x00FF));
    emit this->sendCommand(cmdArr);

}

bool HeadActivationDialog::event(QEvent *e)
{
    if((e->type()==QEvent::WindowDeactivate)
            |((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave))
            |((QApplication::platformName() == "linuxfb")&(e->type()==QEvent::Leave)))
    {
        this->pushButtonOK_clicked();
    }
    return QWidget::event(e);
}

void HeadActivationDialog::showEvent(QShowEvent *ev)
{
    int i;
    for(i = 0; i< checkBoxList.length(); i++)
    {
        checkBoxList[i]->setChecked(!(static_cast<bool>(this->headActivState&(1<<i))));
        connect(checkBoxList[i], SIGNAL(clicked(int)), this, SLOT(headStateChanged(int)));
    }
    ui->pushButtonCancel->setEnabled(true);

    ev->accept();
}

void HeadActivationDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
