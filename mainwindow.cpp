#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setStyleSheet(QString((
                              "color: #ABEFF6;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #80D0F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
//                              "background-color: qradialgradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop: 0 #80D0F0, stop: 1.0 #003070)"
                              "selection-color: yellow;"
                              "border-radius: 10px;"
                              "border-width: 2px;"
                              "border-style: outset;"
                              "border-color: #003070;"
                              "selection-background-color: blue;"
                              )));

    pix.load("/home/mishko/Dropbox/SharedProgects/NewProjects/buildsLin/build-iconsManager_Release/tt.png");

    settings = new QSettings("settings.ini", QSettings::IniFormat);

    dial = new SettingDialog(hStt);
    connect(dial, SIGNAL(accept(int,QByteArray)), this, SLOT(headParamGet(int,QByteArray)));
    connect(dial, SIGNAL(changeNumber(int)), this, SLOT(changeHeadNo(int)));
    connect(dial, SIGNAL(setParamsToAll(int,QByteArray)), this, SLOT(allHeadParamGet(int,QByteArray)));

    indexer = new IndexerWidget(this);
    ui->layoutIndexer->addWidget(indexer);

    int i;

    for(i = 0; i<HEAD_COUNT; i++)
    {
        headButton[i] = new HeadForm(ui->widgetHeads);
        headButton[i]->setIndex(i);

        headButton[i]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio)));

//        qDebug()<<headButton[i]->getLabelSize();
        if(settings->value(QString("HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[1]&0x01)
            switch (settings->value(QString("HEAD_"+QString::number(i)+"_PARAM")).value<QByteArray>()[0]) {
            case 0:
                headButton[i]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
                break;
            case 1:
                headButton[i]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
                break;
            case 2:
                headButton[i]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
                break;
            }
        else
            headButton[i]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[i]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");

        connect(headButton[i], SIGNAL(settingButtonCliced(int)), this, SLOT(someButtonClck(int)));
    }
    ui->widgetHeads->resize(750, 570);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::someButtonClck(int index)
{
    hStt.fromByteArray(settings->value(QString("HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>());

    dial->setHeadParams(hStt, index);
    dial->move(this->pos().x()+300+200*cos(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.),this->pos().y()+300+200*sin(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.));
//    dial->move(this->pos().x()+this->size().width()*0.8,this->pos().y()+(this->size().height()-dial->size().width())*0.5);
    dial->show();

}

void MainWindow::changeHeadNo(int index)
{
    if(index<0)
        index = HEAD_COUNT-1;
    else
        if(index == HEAD_COUNT)
            index = 0;
    this->someButtonClck(index);
}

void MainWindow::headParamGet(int index, QByteArray hParamArr)
{
    settings->setValue(QString("HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
//qDebug()<<headButton[index]->getLabelSize();
    if(settings->value(QString("HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[1]&0x01)

        switch (settings->value(QString("HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>()[0]) {
        case 0:
            headButton[index]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[index]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #046DC4, stop: 0.8 #04589D,stop: 1.0 #011D36);");
            break;
        case 1:
            headButton[index]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[index]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8826C4, stop: 0.8 #562773,stop: 1.0 #14043C);");
            break;
        case 2:
            headButton[index]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[index]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #DE083B, stop: 0.8 #A91349,stop: 1.0 #681030);");
            break;
        }
    else
        headButton[index]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[index]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #758491, stop: 0.8 #3E5468,stop: 1.0 #1D3D59);");

}

void MainWindow::allHeadParamGet(int index, QByteArray hParamArr)
{
    int cnt;
    for(cnt = 0; cnt<HEAD_COUNT; cnt++)
    {
        settings->setValue(QString("HEAD_"+QString::number(cnt)+"_PARAM"), hParamArr);
        if(hParamArr[1]&0x01)
            switch (hParamArr[0]) {
            case 0:
                headButton[cnt]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[cnt]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: rgb(0,255,0);");
                break;
            case 1:
                headButton[cnt]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[cnt]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: rgb(100,0,255);");
                break;
            case 2:
                headButton[cnt]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[cnt]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: rgb(255,0,0);");
                break;
            }
        else
            headButton[cnt]->setPixmap(QPixmap::fromImage(pix.scaled(headButton[cnt]->getLabelSize(),Qt::KeepAspectRatio)),"background-color: rgb(100,100,100);");
    }
}



void MainWindow::resizeEvent(QResizeEvent *e)
{
    int areaW, areaH;
    areaH = ui->widgetHeads->height();
    areaW = ui->widgetHeads->width();

    int i;
    for(i = 0; i<HEAD_COUNT; i++)
    {
        if(areaH<areaW)
            headButton[i]->move(areaW/2+areaH/3*cos(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-73,
                                areaH/2+areaH/3*sin(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-60);
        else
            headButton[i]->move(areaW/2+areaW/3*cos(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-73,
                                areaH/2+areaW/3*sin(2.*3.1415926*i/HEAD_COUNT+3.1415926/2.+3.1415926/HEAD_COUNT)-60);

    }
//    headButton[0]->move(areaW/2-73,
//                        areaH/2-60);
    e->accept();
}
