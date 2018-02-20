#include "numpaddialog.h"
#include "ui_numpaddialog.h"

NumpadDialog::NumpadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumpadDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("color: #ABEFF6;"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #80D0F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
//                              "background-color: qradialgradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop: 0 #80D0F0, stop: 1.0 #003070)"
                        "selection-color: yellow;"
                        "border-radius: 10px;"
                        "border-width: 2px;"
                        "border-style: outset;"
                        "border-color: #003070;"
                        "selection-background-color: blue;"
                        "font : 24pt");
    int k;
    buttons[0] = new NumpadButton(0, QString::number(0));
    ui->layoutButtons->addWidget(buttons[0],3,0);
    connect(buttons[0], SIGNAL(clicked(int)), this, SLOT(appendToLineEdit(int)));
    for(k = 1; k<10; k++){
        buttons[k] = new NumpadButton(k, QString::number(k));
        ui->layoutButtons->addWidget(buttons[k], 2-(k-1)/3, (k-1)%3);
        connect(buttons[k], SIGNAL(clicked(int)), this, SLOT(appendToLineEdit(int)));
    }
    QPushButton *comaButton = new QPushButton(".");
    ui->layoutButtons->addWidget(comaButton, 3, 1);
    connect(comaButton, SIGNAL(clicked(bool)), this, SLOT(addDot()));
    QPushButton *signButton = new QPushButton("+/-");
    ui->layoutButtons->addWidget(signButton, 3, 2);
    connect(signButton, SIGNAL(clicked(bool)), this, SLOT(changeSign()));
    QPushButton *enterButton = new QPushButton("Enter");
    ui->layoutButtons->addWidget(enterButton, 3, 3);
    connect(enterButton, SIGNAL(clicked(bool)), this, SLOT(submitValue()));
}

NumpadDialog::~NumpadDialog()
{
    delete ui;
}

QString NumpadDialog::getValue()
{
    NumpadDialog numpad;
    numpad.exec();
    return numpad.value;
}

void NumpadDialog::appendToLineEdit(int number)
{
    ui->lineValue->insert(QString::number(number));
}

void NumpadDialog::submitValue()
{
    this->value = ui->lineValue->text();
    ui->lineValue->clear();
    this->accept();
}

void NumpadDialog::changeSign()
{
    ui->lineValue->setText(QString::number((ui->lineValue->text().toDouble())*(-1)));
}

void NumpadDialog::addDot()
{
    if(!ui->lineValue->text().isEmpty() && !ui->lineValue->text().contains('.')){
        ui->lineValue->insert(".");
    }
}

NumpadButton::NumpadButton(unsigned int inputNumber, QString name)
{
    index = inputNumber;
    this->setText(name);
    this->setStyleSheet("QPushButton:pressed {background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0070FF, stop: 0.8 #3050A0,stop: 1.0 #103070);}");
    connect(this, SIGNAL(clicked()), this, SLOT(thisClicked()));
}
