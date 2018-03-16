#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    this->setStyleSheet("*{}");
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setPrinted(int val)
{
    ui->labelPrinted->setText(QString::number(val));
}

void InfoWidget::setTotal(int val)
{
    ui->labelTotal->setText(QString::number(val));
}
