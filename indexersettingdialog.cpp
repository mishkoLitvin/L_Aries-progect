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
                                 "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8080A0, stop: 0.8 #606070,stop: 1.0 #202030);}"
                                 );

    this->eventFilterSetup();

    connect(ui->pButtonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->pButtonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    acceptOnDeactilationEn = true;

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

void IndexerSettingDialog::setMachineSetting(MachineSettings::MachineParameters machineParam)
{
    ui->spinBoxHeadsCount->setValue(machineParam.HeadCount);
    ui->dSpinBoxWarningTime->setValue(machineParam.WarningTime/10.);
}

void IndexerSettingDialog::accept()
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

    MachineSettings::MachineParameters machineParams;
    machineParams.HeadCount = ui->spinBoxHeadsCount->value();
    machineParams.WarningTime = ui->dSpinBoxWarningTime->value()*10;

    emit this->machineParamChanged(machineParams.toByteArray());
    emit this->indexerParamChanged(indexParam.toByteArray());
    emit this->liftParamChanged(liftParams.toByteArray());

    this->hide();
}

void IndexerSettingDialog::reject()
{
    this->hide();
}

void IndexerSettingDialog::eventFilterSetup()
{
    QObjectList objList = ui->dSpinBoxIndexAccel->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxIndexAccelRet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxIndexDistance->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxLiftAccel->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxLiftDistance->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxLiftDownDelay->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->dSpinBoxLiftUpDelay->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->dSpinBoxWarningTime->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxHeadsCount->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxIndexDistanceOffcet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxIndexHomeOffset->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxIndexSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxindexSpeedRet->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }

    objList = ui->spinBoxLiftHomeOffset->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
    objList = ui->spinBoxLiftSpeed->children();
    for(int i = 0; i < objList.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(objList[i]);
        if(cast)
            cast->installEventFilter(this);
    }
}

bool IndexerSettingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        acceptOnDeactilationEn = false;
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue());
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
        acceptOnDeactilationEn = true;
    }
    return false;
}

bool IndexerSettingDialog::event(QEvent *e)
{
    if(e->type()==QEvent::WindowDeactivate)
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}

