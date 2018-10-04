#include "headform.h"
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

    imgShirtShow.load(":/new/icons/icons/Base/tt.png");
    imgShirtHide.load(":/new/icons/icons/Base/blank.png");
    imgShirtAnimate.load(":/new/icons/icons/Base/tt3.png");

    pixShirtShow = QPixmap::fromImage(imgShirtShow.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    pixShirtHide = QPixmap::fromImage(imgShirtHide.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    pixShirtAnimate = QPixmap::fromImage(imgShirtAnimate.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));

    graphEffect = new QGraphicsColorizeEffect();

    labelIndex = new QLabel(this);
    labelIndex->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 20px bold italic large \"Times New Roman\"");
    labelIndex->resize(25,25);
    labelIndex->move(ui->graphicsView->width()-labelIndex->width(),this->height()-labelIndex->height()/*-10*/);

    labelStrokCnt = new QLabel(this);
    labelStrokCnt->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 16px bold italic large \"Times New Roman\"");
    labelStrokCnt->resize(35,25);

    labelStBkStr = new QLabel(this);
    labelStBkStr->setStyleSheet("background-color: rgba(255, 255, 255, 0); color : white; font: 16px bold italic large \"Times New Roman\"");
    labelStBkStr->resize(35,25);

    grScene = new QGraphicsScene();
    grScene->setBackgroundBrush(QBrush(Qt::transparent));
    ui->graphicsView->setScene(grScene);
    ui->graphicsView->setStyleSheet("border-style: none;");

    grSize.setCoords(0,0,ui->graphicsView->width(), ui->graphicsView->height());
    QBrush grBrash;
    grRectBkgrItem = grScene->addRect(grSize, QPen(QColor(Qt::transparent),0),grBrash);

    grPixLogoItem = grScene->addPixmap(QPixmap::fromImage(imgShirtHide));
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
    case HeadLoad:
        this->headformType = HeadLoad;
        labelIndex->hide();
        ui->graphicsView->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DD00FF66, stop: 0.8 #DD008822,stop: 1.0 #DD003300); border-style: none;");
        labelStrokCnt->setVisible(false);
        labelStBkStr->setVisible(false);

        break;
    case HeadUnload:
        this->headformType = HeadUnload;
        labelIndex->hide();
        ui->graphicsView->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DDCC3311, stop: 0.8 #DDAA220F,stop: 1.0 #DD331100); border-style: none;");
        labelStrokCnt->setVisible(false);
        labelStBkStr->setVisible(false);

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
    this->index = i;
}

void HeadForm::setIndexLabelPosition(HeadForm::SettBtnPos position)
{
    this->labelPos = position;
    switch (position) {
    case AtRightUp:
        labelIndex->move(this->width()-4-labelIndex->width(), 4);
        labelStrokCnt->move(this->width()-4-labelStrokCnt->width(), 4+labelIndex->height());
        labelStBkStr->move(this->width()-4-labelStrokCnt->width(), 4+labelIndex->height()+labelStrokCnt->height());
        break;
    case AtRightDown:
        labelIndex->move(this->width()-4-labelIndex->width(),ui->graphicsView->height()+4-labelIndex->height());
        labelStrokCnt->move(this->width()-4-labelStrokCnt->width(),
                            ui->graphicsView->height()+4-labelIndex->height()-labelStrokCnt->height());
        labelStBkStr->move(this->width()-4-labelStrokCnt->width(),
                            ui->graphicsView->height()+4-labelIndex->height()-labelStrokCnt->height()-labelStrokCnt->height());
        break;
    case AtLeftUp:
        labelIndex->move(4,4);
        labelStrokCnt->move(4,4+labelIndex->height());
        labelStBkStr->move(4,4+labelIndex->height()+labelStrokCnt->height());
        break;
    case AtLeftDown:
        labelIndex->move(4, ui->graphicsView->height()+4-labelIndex->height());
        labelStrokCnt->move(4, ui->graphicsView->height()+4-labelIndex->height()-labelStrokCnt->height());
        labelStBkStr->move(4, ui->graphicsView->height()+4-labelIndex->height()-labelStrokCnt->height()-labelStrokCnt->height());
        break;
    }

}

void HeadForm::setPixmap(HeadformState state, QString stStr)
{
    switch(state){
    case shirtOff:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(pixShirtHide);
        headformState = shirtOff;
        break;
    case shirtOn:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(pixShirtShow);
        headformState = shirtOn;
        break;
    case shirtProcessing:
        ui->graphicsView->setStyleSheet(stStr+" border-style: none;");
        grPixLogoItem->setPixmap(pixShirtAnimate);
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
    imgShirtShow.load(path+"/tt.png");
    imgShirtHide.load(path+"/blank.png");
    imgShirtAnimate.load(path+"/tt3.png");

    pixShirtShow = QPixmap::fromImage(imgShirtShow.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    pixShirtHide = QPixmap::fromImage(imgShirtHide.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    pixShirtAnimate = QPixmap::fromImage(imgShirtAnimate.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));

    this->setPixmap(shirtOff);
}

void HeadForm::setRagColor(QColor color)
{
    grPixLogoItem->setGraphicsEffect(graphEffect);
    graphEffect->setColor(color);
}

void HeadForm::setStrokCount(int val)
{
    labelStrokCnt->resize(35,25);
    if(val != 0)
        labelStrokCnt->setText("St:"+QString::number(val));
    this->setIndexLabelPosition(this->labelPos);
}

void HeadForm::setDryPower(int val)
{
    if(val!=100)
        labelStrokCnt->resize(48,25);
    else
        labelStrokCnt->resize(58,25);
    labelStrokCnt->setText("UV:"+QString::number(val));
    this->setStepBkStrCnt(0);
    this->setIndexLabelPosition(this->labelPos);
}

void HeadForm::setOff()
{
    labelStrokCnt->setText("OFF");
    setStepBkStrCnt(0);
}

void HeadForm::setStepBkStrCnt(int val)
{
    if(val != 0)
    {
        labelStBkStr->setText("Bk:"+QString::number(val));
        labelStBkStr->setVisible(true);
    }
    else
        labelStBkStr->setVisible(false);
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
        if(this->headformType != HeadLoad)
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

        switch(headformState)
        {
        case shirtOff:          emit this->loadStateChanged(this->index, LoadClean); break;
        case shirtOn:           emit this->loadStateChanged(this->index, LoadOne); break;
        case shirtProcessing:   emit this->loadStateChanged(this->index, LoadAuto); break;
        }
    }

}
