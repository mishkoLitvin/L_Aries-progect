#include "headform.h"
#include "ui_headform.h"

#include <QBitmap>
#include <QPalette>


HeadForm::HeadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeadForm)
{
    ui->setupUi(this);

    headformType = HeadProcessing;

    pixShirtShow.load(":/new/icons/icons/tt.png");
    pixShirtHide.load(":/new/icons/icons/blank.png");
    pixShirtAnimate.load(":/new/icons/icons/tt3.png");

    this->setPixmap(shirtOff);


    this->headformType = HeadProcessing;

    labelIndex = new QLabel(this);
    labelIndex->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 20px bold italic large \"Times New Roman\"");

    labelIndex->resize(25,25);
    labelIndex->move(ui->label->width()-labelIndex->width(),this->height()-labelIndex->height()-10);
}

HeadForm::~HeadForm()
{
    delete ui;
}

void HeadForm::setHeadformType(HeadForm::HeadformType type)
{
    switch (type)
    {
        case HeadProcessing:
            this->headformType = HeadProcessing; break;
        case HeadPutingOn:
            this->headformType = HeadPutingOn;
            labelIndex->hide();
            ui->label->setStyleSheet("* {background-color: #009900}");
            break;
        case HeadRemoving:
            this->headformType = HeadRemoving;
            labelIndex->hide();
            ui->label->setStyleSheet("background-color: #990000");
            break;
    };
}

HeadForm::HeadformType HeadForm::getHeadformType()
{
    return this->headformType;
}

void HeadForm::setIndex(int i)
{
    if(i<10)
        labelIndex->resize(15,25);
    labelIndex->setText(QString::number(i));
}




void HeadForm::setSettBtnPosition(HeadForm::SettBtnPos position)
{
    switch (position) {
    case AtRightUp:
        labelIndex->move(this->width()-11-labelIndex->width(), 11);
        break;
    case AtRightDown:
        labelIndex->move(this->width()-11-labelIndex->width(),ui->label->height()+11-labelIndex->height());
        break;
    case AtLeftUp:
        labelIndex->move(11,11);
        break;
    case AtLeftDown:
        labelIndex->move(11, ui->label->height()+11-labelIndex->height());
        break;
    }

}

void HeadForm::setPixmap(HeadformState state, QString stStr)
{
    switch(state){
    case shirtOff:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtHide.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        headformState = shirtOff;
        break;
    case shirtOn:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtShow.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        headformState = shirtOn;
        break;
    case shirtProcessing:
        ui->label->setPixmap(QPixmap::fromImage(pixShirtAnimate.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        ui->label->setStyleSheet(stStr);
        headformState = shirtProcessing;
        break;
    }
}

void HeadForm::setPixmap(HeadForm::HeadformState state)
{
    this->setPixmap(state, ui->label->styleSheet());
}

HeadForm::HeadformState HeadForm::getRagState()
{
    return this->headformState;
}

QSize HeadForm::getLabelSize()
{
    return ui->label->size();
}


void HeadForm::mousePressEvent(QMouseEvent *event)
{
    if(event->type() == QMouseEvent::MouseButtonPress)
    {
        if(this->headformType == HeadProcessing)
        {
            if(this->headformState == shirtOff)
                this->setPixmap(shirtOn, ui->label->styleSheet());
            else
                this->setPixmap(shirtOff, ui->label->styleSheet());
        }
        else
        {
            switch(headformState)
            {
            case shirtOff: this->setPixmap(shirtOn, ui->label->styleSheet()); break;
            case shirtOn: this->setPixmap(shirtProcessing, ui->label->styleSheet()); break;
            case shirtProcessing: this->setPixmap(shirtOff, ui->label->styleSheet()); break;
            }
        }
    }
}
