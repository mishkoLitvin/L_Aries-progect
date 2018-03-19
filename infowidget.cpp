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
                        "QLabel#labelTotal {background-color: #50303030; font-size: 9px}"
                        "QLabel#labelDZH {background-color: #50303030; font-size: 9px}"
                        "QLabel#labelPrinted {background-color: #50303030;}"
                        "QLabel#labelRemain {background-color: #50303030;}"
                        "QLabel#labelSkipedShirts {background-color: #50303030;}");

    imageArrows.load(":/arrows/icons/arrows/arrowLR.png");
    ui->labelIndexerHalf->setPixmap(QPixmap::fromImage(imageArrows.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageHome.load(":/new/icons/icons/home.png");
    ui->labelHome->setPixmap(QPixmap::fromImage(imageHome.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageLock.load(":/new/icons/icons/unlock.png");
    ui->labelLock->setPixmap(QPixmap::fromImage(imageLock.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageUp.load(":/arrows/icons/arrows/arrowUPart.png");
    ui->labelLiftUp->setPixmap(QPixmap::fromImage(imageUp.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageEmerg.load(":/new/icons/icons/stopEmerg.png");
    ui->labelEmergency->setPixmap(QPixmap::fromImage(imageEmerg.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageWarning.load(":/new/icons/icons/warning.png");
    ui->labelWarning->setPixmap(QPixmap::fromImage(imageWarning.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));
    imageStopHand.load(":/new/icons/icons/stopHand.png");
    ui->labelStopHand->setPixmap(QPixmap::fromImage(imageStopHand.scaled(ui->labelIndexerHalf->size(), Qt::KeepAspectRatio)));

//    ui->labelHome->pixmap()->setMask();

}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setPrinted(int val)
{
    ui->labelPrinted->setText("Printed: " + QString::number(val));
}

void InfoWidget::setTotal(int val)
{
    ui->labelTotal->setText("Total: " + QString::number(val));
}
