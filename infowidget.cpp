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

//    ui->labelHome->pixmap()->setMask();

}

InfoWidget::~InfoWidget()
{
    delete ui;
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
    effect[0]->setOpacity(1);

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
