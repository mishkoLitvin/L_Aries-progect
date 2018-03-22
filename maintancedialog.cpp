#include "maintancedialog.h"
#include "ui_maintancedialog.h"

MaintanceDialog::MaintanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaintanceDialog)
{
    ui->setupUi(this);

    qRegisterMetaTypeStreamOperators<MaintanceElement>("MaintanceElement");
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");

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
        maintanceList.append(MaintanceElement(settings->value("ELEMENT_"+QString::number(i)+"/TROUBLE_INDEX", i).toInt(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/NAME").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/TROUBLE_INFO").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/MACHINE_INFO").toString(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/LAST_COUNT").toInt(),
                                              settings->value("ELEMENT_"+QString::number(i)+"/REPEAT_COUNT").toInt()));
    }

    doItNow = false;
    maintanceHaveWarning = false;
    maintanceHaveWork = false;

    maintanceWidget = new MaintanceWidget(this);

    unsolvedList.clear();
    unsolvedListIndex.clear();

    unsolvedListIndex.append(settings->value("UNSOLVED_ELEMENTS_LIST").value<QList<int> >());

    for(i = 0; i<settings->value("UNSOLVED_ELEMENTS_COUNT", 0).toInt(); i++)
    {
        unsolvedList.append(MaintanceElement(settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/TROUBLE_INDEX").toInt(),
                                             settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/NAME").toString(),
                                             settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/TROUBLE_INFO").toString(),
                                             settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/MACHINE_INFO").toString(),
                                             settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/LAST_COUNT").toInt(),
                                             settings->value("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[i])+"/REPEAT_COUNT").toInt()));
    }
    connect(maintanceWidget, SIGNAL(troubleSolved(int)), this, SLOT(solveItem(int)));

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

    maintanceWidget = new MaintanceWidget(this);

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

void MaintanceDialog::solveItem(int index)
{
    unsolvedList.removeAt(index);
    settings->setValue("UNSOLVED_ELEMENTS_COUNT", unsolvedList.length());

    settings->remove("UNSLOVED/ELEMENT_"+QString::number(unsolvedListIndex[index]));
    settings->setValue("ELEMENT_"+QString::number(unsolvedListIndex[index])+"/LAST_COUNT", this->lastCyclesCount);
    maintanceList[unsolvedListIndex[index]].lastCount = this->lastCyclesCount;
    unsolvedListIndex.removeAt(index);


    settings->setValue("UNSOLVED_ELEMENTS_LIST", QVariant::fromValue(unsolvedListIndex));

    if(unsolvedList.length() == 0)
        emit this->maintanceWorkEnable(false);
}

bool MaintanceDialog::execute(QWidget *parent,
                              MaintanceDialog::MaintanceType mType,
                              QString name,
                              QString trInfo,
                              QString machInfo)
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
    this->lastCyclesCount = cyclesCount;
    int i;
    for(i = 0; i<maintanceList.length(); i++)
    {
        if((cyclesCount==(maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount
                          -settings->value("WARNING_CYCLES_COUNT", 50).toInt()))
                &(cyclesCount<(maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount-1)))
        {
//            emit this->stopRequest();
            maintanceList[i].troubleType = (int)Warning;
            this->maintanceHaveWarning = true;
            bool service = false;
//            service = MaintanceDialog::execute(this,
//                                               (MaintanceType)maintanceList[i].troubleType,
//                                               maintanceList[i].troubleName,
//                                               maintanceList[i].troubleInfo
//                                               +"\nYou must do this after "
//                                               +QString::number(settings->value("WARNING_CYCLES_COUNT", 50).toInt())
//                                               +" cycles.",
//                                               maintanceList[i].machineInfo);
            if(service)
            {
                emit this->stopRequest();
                settings->setValue("ELEMENT_"+QString::number(i)+"/LAST_COUNT", cyclesCount);
                maintanceList[i].lastCount = cyclesCount;
            }
//            else
//                emit this->continueRequest();
        }
        else
            if(cyclesCount == (maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount))
            {
                maintanceList[i].troubleType = (int)Critical;
                bool service = false;
//                MaintanceDialog::execute(maintanceList[i], this);
                this->maintanceHaveWork = true;
                if(service)
                {
                    emit this->stopRequest();
                    settings->setValue("ELEMENT_"+QString::number(i)+"/LAST_COUNT", cyclesCount);
                    maintanceList[i].lastCount = cyclesCount;
                }
                else
                {
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/TROUBLE_INDEX", maintanceList[i].troubleIndex);
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/NAME", maintanceList[i].troubleName);
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/TROUBLE_INFO", maintanceList[i].troubleInfo);
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/MACHINE_INFO", maintanceList[i].machineInfo);
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/LAST_COUNT", maintanceList[i].lastCount);
                    settings->setValue("UNSLOVED/ELEMENT_"+QString::number(unsolvedList.length())+"/REPEAT_COUNT", maintanceList[i].repeatCyclesCount);
                    unsolvedList.append(maintanceList[i]);
                    unsolvedListIndex.append(i);
                    settings->setValue("UNSOLVED_ELEMENTS_COUNT", unsolvedList.length());
                    settings->setValue("UNSOLVED_ELEMENTS_LIST", QVariant::fromValue(unsolvedListIndex));
                    emit this->maintanceWorkEnable(true);
                    qDebug()<<unsolvedListIndex;
                }

            }
        if((cyclesCount > (maintanceList[i].lastCount+maintanceList[i].repeatCyclesCount))|
                (unsolvedList.length() > 0))
        {
            emit this->maintanceWorkEnable(true);
            this->maintanceHaveWork = true;
        }
        else
            emit this->maintanceWorkEnable(false);
    }
}

void MaintanceDialog::openMaintanceList()
{
    maintanceWidget->setElemets(this->unsolvedList);
    maintanceWidget->show();
}

void MaintanceDialog::openDialog()
{
    if(maintanceHaveWork)
    {
        for(;unsolvedList.length()>0;)
            if(MaintanceDialog::execute(unsolvedList[0], this))
                this->solveItem(0);
    }

}
