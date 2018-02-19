#include "indexerwidget.h"
#include "ui_indexerwidget.h"

IndexerWidget::IndexerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerWidget)
{
    ui->setupUi(this);

    ui->pButtonPrint->setStyleSheet(QString(("QPushButton:checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0070FF, stop: 0.8 #3050A0,stop: 1.0 #103070)};")));
    setStyleSheet(QString(("QPushButton:pressed {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0070FF, stop: 0.8 #3050A0,stop: 1.0 #103070)};")));

    pButtonSets = new QPushButton(this);

    pButtonSets->setStyleSheet("background-color: rgb(100,250,100);");
    pButtonSets->resize(30,30);
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    connect(pButtonSets, SIGNAL(clicked(bool)), this, SLOT(settingPButtonClicSlot()));

    pButtonSets->setIcon(QIcon(":/new/icons/icons/settings.png"));
}

IndexerWidget::~IndexerWidget()
{
    delete ui;
}

void IndexerWidget::on_pButtonLock_clicked()
{

}

void IndexerWidget::on_pButtonMove_clicked()
{

}

void IndexerWidget::on_pButtonAuto_clicked()
{

}

void IndexerWidget::on_pButtonPrint_clicked()
{

}

void IndexerWidget::on_pButtonMoveLeft_clicked()
{

}

void IndexerWidget::on_pButtonMoveUp_clicked()
{

}

void IndexerWidget::on_pButtonRight_clicked()
{

}

void IndexerWidget::settingPButtonClicSlot()
{
    emit this->settingButtonCliced();
}

void IndexerWidget::resizeEvent(QResizeEvent *e)
{
    pButtonSets->move(this->width() - pButtonSets->width(), 0);

    e->accept();
}
