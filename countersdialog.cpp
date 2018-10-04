#include "countersdialog.h"
#include "ui_countersdialog.h"

CountersDialog::CountersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountersDialog)
{
    ui->setupUi(this);

    ui->doubleSpinBoxRemain->children()[0]->installEventFilter(this);
    connect(ui->doubleSpinBoxRemain, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxRemain_valueChanged(double)));

}

CountersDialog::~CountersDialog()
{
    delete ui;
}

void CountersDialog::setRemaining(int val)
{
    disconnect(ui->doubleSpinBoxRemain, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxRemain_valueChanged(double)));
    ui->doubleSpinBoxRemain->setValue(val);
    connect(ui->doubleSpinBoxRemain, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxRemain_valueChanged(double)));
}

bool CountersDialog::event(QEvent *e)
{
    if((e->type()==QEvent::WindowDeactivate)
            |((QApplication::platformName() == "eglfs")&(e->type()==QEvent::Leave))
            |((QApplication::platformName() == "linuxfb")&(e->type()==QEvent::Leave)))
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}

void CountersDialog::showEvent(QShowEvent *ev)
{
    acceptOnDeactilationEn = true;
    ev->accept();
}

bool CountersDialog::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease))
            |((QApplication::platformName() == "linuxfb")&(event->type()==QEvent::MouseButtonRelease)))
    {
        acceptOnDeactilationEn = false;
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
        acceptOnDeactilationEn = true;
    }
    return false;
}

void CountersDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}


void CountersDialog::on_pushButtonRemainReset_clicked()
{
    emit this->resetRemaining();
}

void CountersDialog::on_pushButtonSkippedReset_clicked()
{
    emit this->resetSkipped();
}

void CountersDialog::doubleSpinBoxRemain_valueChanged(double arg1)
{
    emit this->remainingValChanged(arg1);
}

void CountersDialog::on_pushButtonHide_clicked()
{
    this->hide();
}
