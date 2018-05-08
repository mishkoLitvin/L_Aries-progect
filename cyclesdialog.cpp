#include "cyclesdialog.h"
#include "ui_cyclesdialog.h"

CyclesDialog::CyclesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CyclesDialog)
{
    ui->setupUi(this);
}

CyclesDialog::CyclesDialog(int headCount, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CyclesDialog)
{
    ui->setupUi(this);

    this->headCount = headCount;
    int i, j;
    QObjectList objList;

    for(i = 0; i<headCount; i++)
    {
        this->spinBoxList.append(new QDoubleSpinBox(ui->widgetSpinPlace));
        spinBoxList[i]->setRange(0, 10);
        spinBoxList[i]->setDecimals(0);
        spinBoxList[i]->resize(80, 65);
        spinBoxList[i]->setStyleSheet("QDoubleSpinBox::up-button {"
                                      "width: 50px;"
                                      "height: 30px;"
                                      "subcontrol-origin: border;"
                                      "subcontrol-position: top right;}"
                                      "QDoubleSpinBox::down-button {"
                                      "width: 50px;"
                                      "height: 30px;"
                                      "subcontrol-origin: border;"
                                      "subcontrol-position: bottom right;}");
        spinBoxList[i]->children()[0]->installEventFilter(this);
    }

    for(i = 0; i<8; i++)
    {
        cycleValues.append(new int[headCount]);
        for(j = 0; j<headCount; j++)
            cycleValues[i][j] = 0;
    }
    cycleValues[0][0] = 10;
    cycleValues[0][1] = 9;
    cycleValues[0][2] = 8;
    cycleValues[0][3] = 7;
    cycleValues[0][8] = 10;

    this->setStyleSheet("QPushButton:!checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #A0A0C0, stop: 0.8 #90907F,stop: 1.0 #108020)}"
                        "QPushButton:checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #40D080, stop: 0.8 #10B07F,stop: 1.0 #30A040)}");
    ui->pButtonON->setStyleSheet("QToolButton:!checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #A0A0C0, stop: 0.8 #90907F,stop: 1.0 #108020)}"
                                 "QToolButton:checked {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #40D080, stop: 0.8 #10B07F,stop: 1.0 #30A040)}");


    lastCycleSel = 0;
    this->loadValues(cycleValues[0]);
    ui->lcdNumber->display(1);


}

CyclesDialog::~CyclesDialog()
{
    delete ui;
}

void CyclesDialog::showEvent(QShowEvent *ev)
{
    float sinCoef, cosCoef;
    float x0_hb, y0_hb, R;
    x0_hb = ui->widgetSpinPlace->width()/2-spinBoxList[0]->width()/2;
    y0_hb = ui->widgetSpinPlace->height()/2-spinBoxList[0]->height()/2;

    if(ui->widgetSpinPlace->height()<ui->widgetSpinPlace->width())
        R = ui->widgetSpinPlace->height()/2-spinBoxList[0]->height()/2-10;
    else
        R = ui->widgetSpinPlace->width()/2-spinBoxList[0]->width()/2-10;

    int i;
    for(i = 0; i<headCount; i++)
    {
        sinCoef = sin(2.*3.1415926*i/headCount+3.1415926/2.+3.1415926/headCount);
        cosCoef = cos(2.*3.1415926*i/headCount+3.1415926/2.+3.1415926/headCount);

        spinBoxList[i]->move(x0_hb+(R)*cosCoef, y0_hb+(R)*sinCoef);
    }
    if(QApplication::platformName() != "eglfs")
        this->resize(QSize(1024, 768));
    else
        this->setWindowState(Qt::WindowMaximized);
    ev->accept();
}

bool CyclesDialog::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease)))
    {
        qobject_cast<QDoubleSpinBox*>(watched->parent())->setValue(NumpadDialog::getValue(this));
        qobject_cast<QDoubleSpinBox*>(watched->parent())->clearFocus();
    }
    return false;
}

void CyclesDialog::on_pButtonOK_clicked()
{
    this->accept();
}

void CyclesDialog::on_pButtonON_clicked()
{
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC1->setChecked(ui->pButtonON->isChecked());
        break;
    case 1:
        ui->pButtonC2->setChecked(ui->pButtonON->isChecked());
        break;
    case 2:
        ui->pButtonC3->setChecked(ui->pButtonON->isChecked());
        break;
    case 3:
        ui->pButtonC4->setChecked(ui->pButtonON->isChecked());
        break;
    case 4:
        ui->pButtonC5->setChecked(ui->pButtonON->isChecked());
        break;
    case 5:
        ui->pButtonC6->setChecked(ui->pButtonON->isChecked());
        break;
    case 6:
        ui->pButtonC7->setChecked(ui->pButtonON->isChecked());
        break;
    case 7:
        ui->pButtonC8->setChecked(ui->pButtonON->isChecked());
        break;
    default:
        break;
    }

}

void CyclesDialog::on_pButtonPrev_clicked()
{
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
        this->on_pButtonC8_clicked();
        break;
    case 1:
        ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
        this->on_pButtonC1_clicked();
        break;
    case 2:
        ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
        this->on_pButtonC2_clicked();
        break;
    case 3:
        ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
        this->on_pButtonC3_clicked();
        break;
    case 4:
        ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
        this->on_pButtonC4_clicked();
        break;
    case 5:
        ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
        this->on_pButtonC5_clicked();
        break;
    case 6:
        ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
        this->on_pButtonC6_clicked();
        break;
    case 7:
        ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
        this->on_pButtonC7_clicked();
        break;
    default:
        break;
    }
}

void CyclesDialog::on_pButtonNext_clicked()
{
    switch (lastCycleSel) {
    case 0:
        ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
        this->on_pButtonC2_clicked();
        break;
    case 1:
        ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
        this->on_pButtonC3_clicked();
        break;
    case 2:
        ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
        this->on_pButtonC4_clicked();
        break;
    case 3:
        ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
        this->on_pButtonC5_clicked();
        break;
    case 4:
        ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
        this->on_pButtonC6_clicked();
        break;
    case 5:
        ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
        this->on_pButtonC7_clicked();
        break;
    case 6:
        ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
        this->on_pButtonC8_clicked();
        break;
    case 7:
        ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
        this->on_pButtonC1_clicked();
        break;
    default:
        break;
    }
}

void CyclesDialog::on_pButtonC1_clicked()
{
    ui->pButtonC1->setChecked(!ui->pButtonC1->isChecked());
    this->saveValues();
    lastCycleSel = 0;
    ui->pButtonON->setChecked(ui->pButtonC1->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC2_clicked()
{
    ui->pButtonC2->setChecked(!ui->pButtonC2->isChecked());
    this->saveValues();
    lastCycleSel = 1;
    ui->pButtonON->setChecked(ui->pButtonC2->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC3_clicked()
{
    ui->pButtonC3->setChecked(!ui->pButtonC3->isChecked());
    this->saveValues();
    lastCycleSel = 2;
    ui->pButtonON->setChecked(ui->pButtonC3->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC4_clicked()
{
    ui->pButtonC4->setChecked(!ui->pButtonC4->isChecked());
    this->saveValues();
    lastCycleSel = 3;
    ui->pButtonON->setChecked(ui->pButtonC4->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC5_clicked()
{
    ui->pButtonC5->setChecked(!ui->pButtonC5->isChecked());
    this->saveValues();
    lastCycleSel = 4;
    ui->pButtonON->setChecked(ui->pButtonC5->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC6_clicked()
{
    ui->pButtonC6->setChecked(!ui->pButtonC6->isChecked());
    this->saveValues();
    lastCycleSel = 5;
    ui->pButtonON->setChecked(ui->pButtonC6->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);
}

void CyclesDialog::on_pButtonC7_clicked()
{
    ui->pButtonC7->setChecked(!ui->pButtonC7->isChecked());
    this->saveValues();
    lastCycleSel = 6;
    ui->pButtonON->setChecked(ui->pButtonC7->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::on_pButtonC8_clicked()
{
    ui->pButtonC8->setChecked(!ui->pButtonC8->isChecked());
    this->saveValues();
    lastCycleSel = 7;
    ui->pButtonON->setChecked(ui->pButtonC8->isChecked());
    ui->lcdNumber->display(lastCycleSel+1);
    this->loadValues(cycleValues[lastCycleSel]);

}

void CyclesDialog::loadValues(int *vals)
{
    int i;
    for(i = 0; i<spinBoxList.length(); i++)
        spinBoxList[i]->setValue(vals[i]);
}

void CyclesDialog::saveValues()
{
    int i;
    for(i = 0; i<spinBoxList.length(); i++)
        cycleValues[lastCycleSel][i] = spinBoxList[i]->value();
}


