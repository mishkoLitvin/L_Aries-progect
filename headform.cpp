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

    pButtonSets = new QPushButton("", this);

    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(50,50);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    pButtonSets->setIcon(QIcon(":/new/icons/icons/settings.png"));

    labelIndex = new QLabel(this);
//    labelIndex->setFont(QFont("Noto Sans",20,4,true));
    labelIndex->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 20px bold italic large \"Times New Roman\"");

    labelIndex->resize(25,25);
    labelIndex->move(ui->label->width()-labelIndex->width(),this->height()-labelIndex->height()-10);



    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicedSlot()));
}

HeadForm::~HeadForm()
{
    delete ui;
}

void HeadForm::setIndex(int i)
{
    this->index = i;
    labelIndex->setText(QString::number(this->index+1));
    if(i+1<10)
        labelIndex->resize(15,25);

}

void HeadForm::setSettBtnPosition(HeadForm::SettBtnPos position)
{
    switch (position) {
    case AtRightUp:
        pButtonSets->move(this->width() - pButtonSets->width(), 0);
        labelIndex->move(25, 105-labelIndex->height());
        break;
    case AtRightDown:
        pButtonSets->move(this->width() - pButtonSets->width(), this->height()-pButtonSets->height());
        labelIndex->move(25, 25);

        break;
    case AtLeftUp:
        pButtonSets->move(0, 0);
        labelIndex->move(105-labelIndex->width(),105-labelIndex->height());

        break;
    case AtLeftDown:
        pButtonSets->move(0, this->height()-pButtonSets->height());
        labelIndex->move(105-labelIndex->width(), 25);

        break;
    }

}

void HeadForm::setPixmap(PixmapState state, QString stStr)
{
    switch(state){
    case pixmapHide:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtHide.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        pixmapShown = false;
        break;
    case pixmapShow:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtShow.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        pixmapShown = true;
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

void HeadForm::mousePressEvent(QMouseEvent *event)
{
    if(event->type() == QMouseEvent::MouseButtonDblClick)
    {
        if(pixmapShown)
            this->setPixmap(pixmapHide, ui->label->styleSheet());
        else
            this->setPixmap(pixmapShow, ui->label->styleSheet());
    }
}
