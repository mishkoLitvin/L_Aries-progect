#include "maintancedialog.h"
#include "ui_maintancedialog.h"

MaintanceDialog::MaintanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaintanceDialog)
{
    ui->setupUi(this);

    qRegisterMetaTypeStreamOperators<MaintanceElement>("MaintanceElement");

    settings = new QSettings("./maintance.ini", QSettings::IniFormat);

    this->setTrableName();
    this->setTrableInfo();
    this->setMachineInfo();
    this->setMaintanceType();

    connect(ui->pushButtonOkNow, SIGNAL(clicked(bool)), this, SLOT(acceptSlot()));
    connect(ui->pushButtonOk, SIGNAL(clicked(bool)), this, SLOT(rejectSlot()));

    int i;
    for(i = 0; i<settings->value("ELEMENTS_COUNT", 0).toInt(); i++)
    {
        maintanceList.append(MaintanceElement(settings->value("ELEMENT_"+QString::number(i)+"/NAME").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/TROUBLE_INFO").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/MACHINE_INFO").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/LAST_COUNT").toInt(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/REPEAT_COUNT").toInt()));
    }

    doItNow = false;
}

MaintanceDialog::MaintanceDialog(MaintanceDialog::MaintanceType mType, QWidget *parent, QString name, QString trInfo, QString machInfo):
    QDialog(parent),
    ui(new Ui::MaintanceDialog)
{
    ui->setupUi(this);

    ui->labelTrName->setText(name);
    ui->labelTrInfo->setText(trInfo);
    ui->labelMachineInfo->setText(machInfo);
    QImage warningImg;
    warningImg.load(":/new/icons/icons/warning.png");
    ui->labelIcon->setPixmap(QPixmap::fromImage(warningImg.scaled(ui->labelIcon->size(),Qt::KeepAspectRatio)));
    if(mType == MaintanceDialog::Warning)
    {
        ui->labelIcon->setStyleSheet("background-color: yellow");
    }
    if(mType == MaintanceDialog::Critical)
    {
        ui->labelIcon->setStyleSheet("background-color: red");
    }
    connect(ui->pushButtonOkNow, SIGNAL(clicked(bool)), this, SLOT(acceptSlot()));
    connect(ui->pushButtonOk, SIGNAL(clicked(bool)), this, SLOT(rejectSlot()));

    doItNow = false;
}

MaintanceDialog::~MaintanceDialog()
{
    delete ui;
}

void MaintanceDialog::setTrableName(QString name)
{
    ui->labelTrName->setText(name);
}

void MaintanceDialog::setTrableInfo(QString info)
{
    ui->labelTrInfo->setText(info);
}

void MaintanceDialog::setMachineInfo(QString info)
{
    ui->labelMachineInfo->setText(info);
}

void MaintanceDialog::setMaintanceType(MaintanceDialog::MaintanceType mType)
{
    if(mType == MaintanceDialog::Warning)
        ui->labelIcon->setStyleSheet("background-color: yellow");
    if(mType == MaintanceDialog::Critical)
        ui->labelIcon->setStyleSheet("background-color: red");
}

void MaintanceDialog::acceptSlot()
{
    doItNow = true;
    this->accept();
}

void MaintanceDialog::rejectSlot()
{
    doItNow = false;
    this->reject();
}

bool MaintanceDialog::execute(QWidget *parent, MaintanceDialog::MaintanceType mType, QString name, QString trInfo, QString machInfo)
{
    MaintanceDialog mDialog(mType, parent, name, trInfo, machInfo);
    mDialog.exec();

    return mDialog.doItNow;
}

bool MaintanceDialog::execute(MaintanceElement maintance, QWidget *parent)
{
    MaintanceDialog mDialog((MaintanceDialog::MaintanceType)maintance.troubleType,
                            parent,
                            maintance.troubleName,
                            maintance.troubleInfo,
                            maintance.machineInfo);
    mDialog.exec();

    return mDialog.doItNow;
}

void MaintanceDialog::check(int cyclesCount)
{
    int i;
    for(i = 0; i<maintanceList.length(); i++)
    {
        if((cyclesCount==(maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount
                          -settings->value("WARNING_CYCLES_COUNT", 50).toInt()))
                &(cyclesCount<(maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount-1)))
        {
            emit this->stopRequest();
            maintanceList[i].troubleType = (int)Warning;
            bool service = MaintanceDialog::execute(maintanceList[i], this);
            if(service)
            {
                emit this->stopRequest();
                settings->setValue("ELEMENT_"+QString::number(i)+"/LAST_COUNT", cyclesCount);
            }
            else
                emit this->continueRequest();
        }
        else
            if(cyclesCount == (maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount))
            {
                maintanceList[i].troubleType = (int)Critical;
                bool service = MaintanceDialog::execute(maintanceList[i], this);
                if(service)
                {
                    emit this->stopRequest();
                    settings->setValue("ELEMENT_"+QString::number(i)+"/LAST_COUNT", cyclesCount);
                }
                else
                {
                    emit this->maintanceWorkEnable();
                }

            }
    }
}
