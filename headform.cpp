#include "headform.h"
#include "ui_headform.h"

#include <QBitmap>
#include <QPalette>


HeadForm::HeadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeadForm)
{
    ui->setupUi(this);

    pixShirtShow.load(":/new/icons/icons/tt.png");
    pixShirtHide.load(":/new/icons/icons/blank.png");
    pixShirtAnimate.load(":/new/icons/icons/tt3.png");


    pButtonSets = new QPushButton("S", this);

    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(30,30);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    pButtonSets->setIcon(QIcon(":/new/icons/icons/settings.png"));

    labelIndex = new QLabel(this);
    labelIndex->setFont(QFont("Noto Sans",14,2,true));
    labelIndex->setStyleSheet("background-color: rgb(50,200,250);");

    labelIndex->resize(30,30);
    labelIndex->move(this->width()-labelIndex->width(),this->height()-labelIndex->height());


    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicedSlot()));
}

HeadForm::~HeadForm()
{
    delete ui;
}

void HeadForm::setIndex(int i)
{
    this->index = i;
    labelIndex->setText(QString::number(this->index));

}

void HeadForm::setPixmap(PixmapState state, QString stStr)
{
    switch(state){
    case pixmapHide:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtHide.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        break;
    case pixmapShow:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtShow.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        break;
    case pixmapAnimate:
//        QPixmap::fromImage(pixShirtShow.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(pixShirtAnimate.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        break;
    }
}

QSize HeadForm::getLabelSize()
{
    return ui->label->size();
}

void HeadForm::settingPButtonClicedSlot()
{
    emit this->settingButtonCliced(this->index);
}
