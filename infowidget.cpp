#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
//    this->setStyleSheet("QFrame#InfoWidget{margin : 0px; border: 2px; padding: 0px; border-style: outset; border-radius: 4px;}"
//                        "QLabel#labelTotal {font-size: 9px; border: 2px outset black; border-radius: 2px;}"
//                        "QLabel#labelDZH {font-size: 9px; border: 2px outset black; border-radius: 2px;}"
//                        "QLabel#labelPrinted {border: 2px outset black; border-radius: 2px;}"
//                        "QLabel#labelRemain {border: 2px outset black; border-radius: 2px;}"
//                        "QLabel#labelSkipedShirts {border: 2px outset black; border-radius: 2px;}");

    this->setStyleSheet("*{background-color: #50303030; color: white}"
                        "QLabel{background-color: #00000000; padding: 1px;}"
//                        "QLabel#labelTotal {background-color: #50303030; font-size: 9px}"
//                        "QLabel#labelDZH {background-color: #50303030; font-size: 9px}"
//                        "QLabel#labelPrinted {background-color: #50303030;}"
//                        "QLabel#labelRemain {background-color: #50303030;}"
//                        "QLabel#labelSkipedShirts {background-color: #50303030;}"
                        );

    imageArrows.load(":/arrows/icons/Base/arrows/arrowLR.png");
    ui->labelIndexerHalf->setPixmap(QPixmap::fromImage(imageArrows.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageHome.load(":/new/icons/icons/Base/home.png");
    ui->labelHome->setPixmap(QPixmap::fromImage(imageHome.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageLock.load(":/new/icons/icons/Base/unlock.png");
    ui->labelLock->setPixmap(QPixmap::fromImage(imageLock.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageUp.load(":/arrows/icons/Base/arrows/arrowUPart.png");
    ui->labelLiftUp->setPixmap(QPixmap::fromImage(imageUp.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageEmerg.load(":/new/icons/icons/Base/stopEmerg.png");
    ui->labelEmergency->setPixmap(QPixmap::fromImage(imageEmerg.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageWarning.load(":/new/icons/icons/Base/warning.png");
    ui->labelWarning->setPixmap(QPixmap::fromImage(imageWarning.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageStopHand.load(":/new/icons/icons/Base/stopHand.png");
    ui->labelStopHand->setPixmap(QPixmap::fromImage(imageStopHand.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));

    int i;
    for(i = 0; i<7; i++)
    {
        effect[i] = new QGraphicsOpacityEffect();
        effect[i]->setOpacityMask(QBrush(Qt::black, Qt::SolidPattern));
        effect[i]->setOpacity(0.1);
    }

    errMasages = new QSettings("messages.ini", QSettings::IniFormat, this);

    errMasages->setValue("HEAD/err"+QString::number(0), QString("err"));
//    ui->labelHome->pixmap()->setMask();

}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setRegisterPointer(Register *regPtr)
{
    this->localRegisters = regPtr;
}

void InfoWidget::setPrinted(int val)
{
    ui->labelPrinted->setText("Printed: " + QString::number(val));

    QTime curTime = QTime::currentTime();
    if((val>1)&((lastTime.secsTo(curTime))>0))
        ui->labelDZH->setText("R/H:"+QString::number(3600000/lastTime.msecsTo(curTime)));
    lastTime = curTime;

}

void InfoWidget::setTotal(int val)
{
    ui->labelTotal->setText("Total: " + QString::number(val));
}

void InfoWidget::setIconFolder(QString path)
{
    pathIcon = path;

    imageArrows.load(pathIcon+"/arrows/arrowLR.png");
    imageHome.load(pathIcon+"/home.png");
    imageLock.load(pathIcon+"/unlock.png");
    imageUp.load(pathIcon+"/arrows/arrowUPart.png");
    imageEmerg.load(pathIcon+"/stopEmerg.png");
    imageWarning.load(pathIcon+"/warning.png");
    imageStopHand.load(pathIcon+"/stopHand.png");

    ui->labelHome->setPixmap(QPixmap::fromImage(imageHome.scaled(ui->labelIndexerHalf->size(),
                                                                 Qt::KeepAspectRatio,
                                                                 Qt::SmoothTransformation)));
    ui->labelLock->setPixmap(QPixmap::fromImage(imageLock.scaled(ui->labelIndexerHalf->size(),
                                                                 Qt::KeepAspectRatio,
                                                                 Qt::SmoothTransformation)));
    ui->labelLiftUp->setPixmap(QPixmap::fromImage(imageUp.scaled(ui->labelIndexerHalf->size(),
                                                                 Qt::KeepAspectRatio,
                                                                 Qt::SmoothTransformation)));
    ui->labelIndexerHalf->setPixmap(QPixmap::fromImage(imageArrows.scaled(ui->labelIndexerHalf->size(),
                                                                          Qt::KeepAspectRatio,
                                                                          Qt::SmoothTransformation)));
    ui->labelWarning->setPixmap(QPixmap::fromImage(imageWarning.scaled(ui->labelIndexerHalf->size(),
                                                                       Qt::KeepAspectRatio,
                                                                       Qt::SmoothTransformation)));
    ui->labelEmergency->setPixmap(QPixmap::fromImage(imageEmerg.scaled(ui->labelIndexerHalf->size(),
                                                                       Qt::KeepAspectRatio,
                                                                       Qt::SmoothTransformation)));
    ui->labelStopHand->setPixmap(QPixmap::fromImage(imageStopHand.scaled(ui->labelIndexerHalf->size(),
                                                                         Qt::KeepAspectRatio,
                                                                         Qt::SmoothTransformation)));
    ui->labelHome->setGraphicsEffect(effect[0]);
    ui->labelLock->setGraphicsEffect(effect[1]);
    ui->labelLiftUp->setGraphicsEffect(effect[2]);
    ui->labelIndexerHalf->setGraphicsEffect(effect[3]);
    ui->labelWarning->setGraphicsEffect(effect[4]);
    ui->labelEmergency->setGraphicsEffect(effect[5]);
    ui->labelStopHand->setGraphicsEffect(effect[6]);
}

void InfoWidget::setIndicatorState(u_int16_t state)
{
//    effect[0]->setOpacity(1);

    switch ((u_int8_t)(state>>8)) {
    case 0:
        effect[0]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[0]->setOpacity(0.9);
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 1:
        effect[0]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[0]->setOpacity(0.9);
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 2:
        effect[0]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
        effect[0]->setOpacity(0.9);
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 3:
        effect[0]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[0]->setOpacity(0.3);
        break;
    case 4:
        effect[0]->setOpacity(0.1);
        effect[1]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 5:
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 6:
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.9);
        break;
    case 7:
        effect[0]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[0]->setOpacity(0.1);
        effect[1]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[1]->setOpacity(0.1);
        effect[2]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[2]->setOpacity(0.1);
        effect[3]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[3]->setOpacity(0.1);
        break;
    case 8:
        break;
    case 9:
        effect[2]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
        effect[2]->setOpacity(0.9);
        break;
    case 10:
        effect[3]->setOpacityMask(QBrush(Qt::blue, Qt::SolidPattern));
        effect[3]->setOpacity(0.9);
        break;
    case 11:
        effect[3]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
        effect[3]->setOpacity(0.9);
        break;
    default:
        break;
    }
    effect[4]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
    effect[4]->setOpacity(((state&0x4)>>2)+0.1);
    effect[5]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
    effect[5]->setOpacity((state&0x1)+0.1);
    effect[6]->setOpacityMask(QBrush(Qt::red, Qt::SolidPattern));
    effect[6]->setOpacity(((state&0x2)>>1)+0.1);
}

void InfoWidget::setErrorText(MachineSettings::MachineParameters machineParameters, uint16_t val)
{
    val++;
    uint16_t errDev;
    errDev = this->localRegisters->readReg(MachineSettings::MasterDevice,
                                      Register::masterReg_DEVERR);
    qDebug()<<"ERRRRR";

    uint16_t errMessage;
    errMessage = this->localRegisters->readReg(MachineSettings::MasterDevice,
                                          Register::masterReg_ERROR_MESSAGE);
    uint16_t err;
    err = this->localRegisters->readReg(MachineSettings::MasterDevice,
                                   Register::masterReg_ERR);


    QString msgStrRow1, msgStrRow2;

    msgStrRow1 = "Error! At ";
    switch (errDev) {
    case 1:
        msgStrRow1+="indexed";
        break;
    case 2:
        msgStrRow1+="lift";
        break;
    default:
        if(errDev>2)
        {
            msgStrRow1+="head["+QString::number(errDev-2)+"]";
        }
        break;
    }
    msgStrRow1+= " - cmd:"+QString::number((err>>8)&0xFF)
            +" st:"+QString::number((err)&0xFF);


    if(errMessage<16)
    {
        qDebug()<<"Head servo";
        msgStrRow2 = "Head servo drive "+errMasages->value("HEAD_SERVO/err"+QString::number(errMessage),"").toString();
    }
    else
        if((errMessage>=16)&(errMessage<32))
        {
            qDebug()<<"SQ_FL";
            msgStrRow2 = "Head SQ/FL drive "+errMasages->value("HEAD_SERVO/err"+QString::number(errMessage-16),"").toString();
        }
        else
            if(errMessage == 32)
                msgStrRow2 = "Head "+errMasages->value("HEAD_SERVO/err"+QString::number(errMessage),"").toString();
            else
                if((errMessage>=100)&(errMessage<1001))
                {
                    qDebug()<<"Main servo";
                    if(machineParameters.indexeLiftType.field.indexerType == 0)
                        msgStrRow2 = errMasages->value("MAIN_ESTUN/err"+QString::number(errMessage-100),"").toString();
                    if(machineParameters.indexeLiftType.field.indexerType == 1)
                        msgStrRow2 = errMasages->value("MAIN_DELTA/err"+QString::number(errMessage-100),"").toString();
                }
                else
                    if((errMessage>=1001)&(errMessage<2001))
                    {
                        qDebug()<<"Machine error";
                        msgStrRow2 = errMasages->value("MACHINE/err"+QString::number(errMessage-1001),"").toString();
                    }
                    else
                        if(errMessage>=2001)
                        {
                            qDebug()<<"Head error";
                            msgStrRow2 = "Head "+errMasages->value("HEAD/err"+QString::number(errMessage-2001),"").toString();
                        }

    ui->labelInfo->setText(msgStrRow1+"\n"+msgStrRow2+"\nPlease press Reset button.");
}
