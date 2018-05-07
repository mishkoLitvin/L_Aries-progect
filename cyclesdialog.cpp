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
    int i;
    for(i = 0; i<headCount; i++)
    {
        this->spinBoxList.append(new QDoubleSpinBox(ui->widgetSpinPlace));
        spinBoxList[i]->setRange(0, 10);
        spinBoxList[i]->setDecimals(0);

    }
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


}
