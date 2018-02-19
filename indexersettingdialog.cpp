#include "indexersettingdialog.h"
#include "ui_indexersettingdialog.h"

IndexerSettingDialog::IndexerSettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexerSettingDialog)
{
    ui->setupUi(this);

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

