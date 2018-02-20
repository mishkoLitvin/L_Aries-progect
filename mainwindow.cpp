#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "math.h"
#include "crc16.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    NumpadDialog *numpad = new NumpadDialog();
//    numpad->show();

    setStyleSheet(QString((
                              "color: #ABEFF6;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #80D0F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
                              "selection-color: yellow;"
                              "border-radius: 10px;"
                              "border-width: 2px;"
                              "border-style: outset;"
                              "border-color: #003070;"
                              "selection-background-color: blue;"
                              "font: 14px bold italic large \"Times New Roman\""
                              )));

    pix.load(":/new/icons/icons/tt.png");

    settings = new QSettings("settings.ini", QSettings::IniFormat);
//    QByteArray passwordBArr;
//    passwordBArr.append("2");
//    uint16_t temp =  CalculateCRC16(0xFFFF, passwordBArr);
//    settings->setValue("PASSWORD_INDEXER", temp);
//    qDebug() << temp;

    headSettingDialog = new SettingDialog(headSettings);
//    headSettingDialog->setStyleSheet(this->styleSheet());
    connect(headSettingDialog, SIGNAL(accept(int,QByteArray)), this, SLOT(headParamGet(int,QByteArray)));
    connect(headSettingDialog, SIGNAL(changeNumber(int)), this, SLOT(changeHeadNo(int)));
    connect(headSettingDialog, SIGNAL(setParamsToAll(int,QByteArray)), this, SLOT(allHeadParamGet(int,QByteArray)));

    indexer = new IndexerWidget(this);
    connect(indexer, SIGNAL(settingButtonCliced()), this, SLOT(indexerLiftSettingRequst()));

    indexerLiftSetDialog = new IndexerSettingDialog();
//    indexerLiftSetDialog->setStyleSheet(this->styleSheet());
    connect(indexerLiftSetDialog, SIGNAL(indexerParamChanged(QByteArray)), this, SLOT(indexerParamGet(QByteArray)));
    connect(indexerLiftSetDialog, SIGNAL(liftParamChanged(QByteArray)), this, SLOT(liftParamGet(QByteArray)));

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

        connect(headButton[i], SIGNAL(settingButtonCliced(int)), this, SLOT(headSettingRequest(int)));
    }
    ui->widgetHeads->resize(750, 570);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::headSettingRequest(int index)
{

    QByteArray passwordBArr;
    if(!logedInHeadSettings){
        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
    }
    if(logedInHeadSettings || (CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORD_HEAD_SETTING"))){
        logedInHeadSettings = true;
        headSettings.fromByteArray(settings->value(QString("HEAD_"+QString::number(index)+"_PARAM")).value<QByteArray>());
        headSettingDialog->setHeadParams(headSettings, index);
        headSettingDialog->move(this->pos().x()+300+200*cos(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.),this->pos().y()+300+200*sin(2.*3.1415926*index/HEAD_COUNT+3.1415926/2.));
    //    dial->move(this->pos().x()+this->size().width()*0.8,this->pos().y()+(this->size().height()-dial->size().width())*0.5);
        headSettingDialog->show();
    }
    else{
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet());
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }


}

void MainWindow::indexerLiftSettingRequst()
{
    QByteArray passwordBArr;
    if(!logedInIndexer){
        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
    }
    if(logedInIndexer || (CalculateCRC16(0xFFFF, passwordBArr) == settings->value("PASSWORD_INDEXER"))){
        logedInIndexer = true;
        indexerLiftSettings.fromByteArray(settings->value("INDEXER_PARAMS").value<QByteArray>(),
                                          settings->value("LIFT_PARAMS").value<QByteArray>());
        indexerLiftSetDialog->setIndexerSetting(indexerLiftSettings.indexerParam);
        indexerLiftSetDialog->setLiftSetting(indexerLiftSettings.liftParam);
        indexerLiftSetDialog->show();
        indexerLiftSetDialog->move(this->pos().x()+this->width()-indexerLiftSetDialog->width(),
                                   this->pos().y()/*+this->height()-indexerLiftSetDialog->height()*/);
    }
    else{
        QMessageBox msgBox;
        msgBox.setStyleSheet(this->styleSheet());
        msgBox.setText("Wrong password!");
        msgBox.setWindowTitle("Password");
        msgBox.exec();
    }
}

void MainWindow::changeHeadNo(int index)
{
    if(index<0)
        index = HEAD_COUNT-1;
    else
        if(index == HEAD_COUNT)
            index = 0;
    this->headSettingRequest(index);
}

void MainWindow::headParamGet(int index, QByteArray hParamArr)
{
    settings->setValue(QString("HEAD_"+QString::number(index)+"_PARAM"), hParamArr);
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
    index++;
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

void MainWindow::indexerParamGet(QByteArray indexerParamArr)
{
    settings->setValue(QString("INDEXER_PARAMS"), indexerParamArr);

}

void MainWindow::liftParamGet(QByteArray liftParamArr)
{
    settings->setValue(QString("LIFT_PARAMS"), liftParamArr);

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
