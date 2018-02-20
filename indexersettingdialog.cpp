#include "indexersettingdialog.h"
#include "ui_indexersettingdialog.h"

IndexerSettingDialog::IndexerSettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerSettingDialog)
{
    ui->setupUi(this);

    setStyleSheet(QString(("color: #ABEFF6;"
                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
                           "selection-color: yellow;"
                           "selection-background-color: blue;"
                           "font: 14px bold italic large \"Times New Roman\"")));

    ui->tabWidget->setStyleSheet("QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                                 "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #2050A0, stop: 0.8 #203070,stop: 1.0 #202030);}"
                                 );




}

IndexerSettingDialog::~IndexerSettingDialog()
{
    delete ui;
}

void IndexerSettingDialog::setIndexerSetting(IndexerLiftSettings::IndexParameters indexParam)
{
    ui->dSpinBoxIndexAccel->setValue(indexParam.acceleration/10.);
    ui->dSpinBoxIndexAccelRet->setValue(indexParam.accelerationRet/10);
    ui->dSpinBoxIndexDistance->setValue(indexParam.distance/10.);
    ui->spinBoxIndexDistanceOffcet->setValue(indexParam.distOffcet);
    ui->spinBoxIndexHomeOffset->setValue(indexParam.homeOffset);
    ui->spinBoxIndexSpeed->setValue(indexParam.speed);
    ui->spinBoxindexSpeedRet->setValue(indexParam.speedRet);
}

void IndexerSettingDialog::setLiftSetting(IndexerLiftSettings::LiftParameters liftParam)
{
    ui->dSpinBoxLiftAccel->setValue(liftParam.acceleration/10.);
    ui->dSpinBoxLiftDownDelay->setValue(liftParam.delayDown/10.);
    ui->dSpinBoxLiftUpDelay->setValue(liftParam.delayUp/10.);
    ui->dSpinBoxLiftDistance->setValue(liftParam.distance/10.);
    ui->spinBoxLiftHomeOffset->setValue(liftParam.homeOffcet);
    ui->spinBoxLiftSpeed->setValue(liftParam.speed);
}

void IndexerSettingDialog::accepted()
{
    IndexerLiftSettings::IndexParameters indexParam;
    indexParam.acceleration = ui->dSpinBoxIndexAccel->value()*10.;
    indexParam.accelerationRet = ui->dSpinBoxIndexAccelRet->value()*10.;
    indexParam.distance = ui->dSpinBoxIndexDistance->value()*10.;
    indexParam.distOffcet = ui->spinBoxIndexDistanceOffcet->value();
    indexParam.homeOffset = ui->spinBoxIndexHomeOffset->value();
    indexParam.speed = ui->spinBoxIndexSpeed->value();
    indexParam.speedRet = ui->spinBoxindexSpeedRet->value();

    IndexerLiftSettings::LiftParameters liftParams;
    liftParams.acceleration = ui->dSpinBoxLiftAccel->value()*10.;
    liftParams.delayDown = ui->dSpinBoxLiftDownDelay->value()*10.;
    liftParams.delayUp = ui->dSpinBoxLiftUpDelay->value()*10.;
    liftParams.distance = ui->dSpinBoxLiftDistance->value()*10.;
    liftParams.homeOffcet = ui->spinBoxLiftHomeOffset->value();
    liftParams.speed = ui->spinBoxLiftSpeed->value();

    emit this->indexerParamChanged(indexParam.toByteArray());
    emit this->liftParamChanged(liftParams.toByteArray());

    this->hide();
}

void IndexerSettingDialog::rejected()
{
    this->hide();
}

bool IndexerSettingDialog::event(QEvent *e)
{
    if(e->type()==QEvent::WindowDeactivate)
    {
        this->accepted();
    }
    return QWidget::event(e);
}

