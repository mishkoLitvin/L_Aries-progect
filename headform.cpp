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

    this->headformType = HeadProcessing;

    pixShirtShow.load(":/new/icons/icons/Base/tt.png");
    pixShirtHide.load(":/new/icons/icons/Base/blank.png");
    pixShirtAnimate.load(":/new/icons/icons/Base/tt3.png");

    graphEffect = new QGraphicsColorizeEffect();

    labelIndex = new QLabel(this);
    labelIndex->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 20px bold italic large \"Times New Roman\"");

    labelIndex->resize(25,25);
    labelIndex->move(ui->graphicsView->width()-labelIndex->width(),this->height()-labelIndex->height()/*-10*/);

    grScene = new QGraphicsScene();
    grScene->setBackgroundBrush(QBrush(Qt::transparent));
    ui->graphicsView->setScene(grScene);
    ui->graphicsView->setStyleSheet("border-style: none;");

    grSize.setCoords(0,0,ui->graphicsView->width(), ui->graphicsView->height());
    QBrush grBrash;
    grRectBkgrItem = grScene->addRect(grSize, QPen(QColor(Qt::transparent),0),grBrash);

    grPixLogoItem = grScene->addPixmap(QPixmap::fromImage(pixShirtHide));
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
        this->headformType = HeadPutingOn;
        labelIndex->hide();
        ui->graphicsView->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DD00FF66, stop: 0.8 #DD008822,stop: 1.0 #DD003300); border-style: none;");
//        grRectBkgrItem->setBrush(QBrush("#009900"));
        break;
    case HeadRemoving:
        this->headformType = HeadRemoving;
        labelIndex->hide();
        ui->graphicsView->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DDCC3311, stop: 0.8 #DDAA220F,stop: 1.0 #DD331100); border-style: none;");
//        grRectBkgrItem->setBrush(QBrush("#990000"));
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

void HeadForm::setIndexLabelPosition(HeadForm::SettBtnPos position)
{
    switch (position) {
    case AtRightUp:
        labelIndex->move(this->width()-4-labelIndex->width(), 4);
        break;
    case AtRightDown:
        labelIndex->move(this->width()-4-labelIndex->width(),ui->graphicsView->height()+4-labelIndex->height());
        break;
    case AtLeftUp:
        labelIndex->move(4,4);
        break;
    case AtLeftDown:
        labelIndex->move(4, ui->graphicsView->height()+4-labelIndex->height());
        break;
    }

}

void HeadForm::setPixmap(HeadformState state, QString stStr)
{
    switch(state){
    case shirtOff:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(QPixmap::fromImage(pixShirtHide.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio)));
        headformState = shirtOff;
        break;
    case shirtOn:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(QPixmap::fromImage(pixShirtShow.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio)));
        headformState = shirtOn;
        break;
    case shirtProcessing:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(QPixmap::fromImage(pixShirtAnimate.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio)));
        headformState = shirtProcessing;
        break;
    }
}

void HeadForm::setPixmap(HeadForm::HeadformState state)
{
    this->setPixmap(state, ui->graphicsView->styleSheet());
}

void HeadForm::setRagOn(bool state)
{
    if(state)
        this->setPixmap(shirtOn);
    else
        this->setPixmap(shirtOff);
}


void HeadForm::setIconPath(QString path)
{
    pathImage = path;
    pixShirtShow.load(path+"/tt.png");
    pixShirtHide.load(path+"/blank.png");
    pixShirtAnimate.load(path+"/tt3.png");
    this->setPixmap(shirtOff);
}

void HeadForm::setRagColor(QColor color)
{
    grPixLogoItem->setGraphicsEffect(graphEffect);
    graphEffect->setColor(color);
}

HeadForm::HeadformState HeadForm::getRagState()
{
    return this->headformState;
}

QSize HeadForm::getLabelSize()
{
    return ui->graphicsView->size();
}


void HeadForm::mousePressEvent(QMouseEvent *event)
{
    if(event->type() == QMouseEvent::MouseButtonPress)
    {
        if(this->headformType == HeadProcessing)
        {
            if(this->headformState == shirtOff)
                this->setPixmap(shirtOn, ui->graphicsView->styleSheet());
            else
                this->setPixmap(shirtOff, ui->graphicsView->styleSheet());
        }
        else
        {
            switch(headformState)
            {
            case shirtOff: this->setPixmap(shirtOn, ui->graphicsView->styleSheet()); break;
            case shirtOn: this->setPixmap(shirtProcessing, ui->graphicsView->styleSheet()); break;
            case shirtProcessing: this->setPixmap(shirtOff, ui->graphicsView->styleSheet()); break;
            }
        }
        if(headformType == HeadPutingOn)
        {
            switch(headformState)
            {
            case shirtOff: emit this->loadStateChanged(LoadClean); break;
            case shirtOn: emit this->loadStateChanged(LoadOne); break;
            case shirtProcessing: emit this->loadStateChanged(LoadAuto); break;
            }
        }
    }

}
