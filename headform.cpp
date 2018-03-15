#include "headform.h"
#include "ui_headform.h"

#include <QBitmap>
#include <QPalette>


HeadForm::HeadForm(QWidget *parent, HeadformType type) :
    QWidget(parent),
    ui(new Ui::HeadForm)
{
    ui->setupUi(this);

    headformType = type;

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
            this->headformType = HeadProcessing;
            break;
        case HeadPutingOn:
            this->headformType = HeadPutingOn; break;
        case HeadRemoving:
            this->headformType = HeadRemoving; break;
    };
}

HeadForm::HeadformType HeadForm::getHeadformType()
{
    return this->headformType;
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
            case shirtOff: this->setPixmap(shirtOn); break;
            case shirtOn: this->setPixmap(shirtProcessing); break;
            case shirtProcessing: this->setPixmap(shirtOff); break;
            }
        }
    }
}
