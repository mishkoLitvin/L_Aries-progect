#include "maintancedialog.h"
#include "ui_maintancedialog.h"

MaintanceDialog::MaintanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaintanceDialog)
{
    ui->setupUi(this);
}

MaintanceDialog::~MaintanceDialog()
{
    delete ui;
}
