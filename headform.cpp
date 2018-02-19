#include "headform.h"
#include "ui_headform.h"

#include <QBitmap>
#include <QPalette>


HeadForm::HeadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeadForm)
{
    ui->setupUi(this);

    QBitmap bitmap;
    bitmap.load("/home/mishko/Dropbox/SharedProgects/NewProjects/buildsLin/build-iconsManager_Release/fff.png");
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


    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonCliced()));
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

void HeadForm::setPixmap(QPixmap pix, QString stStr)
{
    ui->label->setPixmap(pix);
    ui->label->setStyleSheet(stStr);


}

QSize HeadForm::getLabelSize()
{
    return ui->label->size();
}

void HeadForm::settingPButtonCliced()
{
    emit this->settingButtonCliced(this->index);
}