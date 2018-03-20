#include "keyboarddialog.h"
#include "ui_keyboarddialog.h"

KeyboardDialog::KeyboardDialog(QWidget *parent, QString windowTitle) :
    QDialog(parent),
    ui(new Ui::KeyboardDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(windowTitle);

    this->setStyleSheet("KeyboardButton {font: 20px; min-width: 30px; min-height: 30px;}"
                        "QLineEdit {font: 20px; min-height: 30px;}");

    capsLockFlag = false;
    shiftFlag = false;

    QString qwerty = "qwertyuiopasdfghjkl:zxcvbnm.,/";
    characters = qwerty.split("", QString::SkipEmptyParts);

    int k = 0;
    foreach(QString s, characters){
        buttons.append(new KeyboardButton(s));
        ui->buttonsLayout->addWidget(buttons[k], k/10, k%10);
        connect(buttons[k], SIGNAL(clicked(QString)), this, SLOT(appendToLineEdit(QString)));
        connect(buttons[k], SIGNAL(clicked(QString)), this, SLOT(deshift()));
        k++;
    }

    connect(ui->pbBackspace, SIGNAL(clicked(bool)), this, SLOT(backspace()));
    connect(ui->pbEnter, SIGNAL(clicked(bool)), this, SLOT(submitText()));
    connect(ui->pbCaps, SIGNAL(clicked(bool)), this, SLOT(capsLock()));
    connect(ui->pbShift, SIGNAL(clicked(bool)), this, SLOT(shift()));
}

KeyboardDialog::~KeyboardDialog()
{
    delete ui;
}

QString KeyboardDialog::getText(QWidget *parent, QString windowTitle)
{
    KeyboardDialog keyboard(parent, windowTitle);
    keyboard.exec();
    return keyboard.text;
}

void KeyboardDialog::buttonsToUpper()
{
    foreach(KeyboardButton* butt, buttons){
        butt->setText(butt->text().toUpper());
        butt->setCharacter(butt->getCharacter().toUpper());
    }
}

void KeyboardDialog::buttonsToLower()
{
    foreach(KeyboardButton* butt, buttons){
        butt->setText(butt->text().toLower());
        butt->setCharacter(butt->getCharacter().toLower());
    }
}

void KeyboardDialog::appendToLineEdit(QString str)
{
    ui->lineEdit->insert(str);
}

void KeyboardDialog::backspace()
{
    ui->lineEdit->setText(ui->lineEdit->text().mid(0, ui->lineEdit->text().length() - 1));
}

void KeyboardDialog::submitText()
{
    this->text = ui->lineEdit->text();
    this->accept();
}

void KeyboardDialog::shift()
{
    if(!shiftFlag)
    {
        if(!capsLockFlag) this->buttonsToUpper();
        else this->buttonsToLower();
        shiftFlag = true;
    }
    else deshift();
}

void KeyboardDialog::deshift()
{
    if(shiftFlag)
    {
        if(!capsLockFlag) this->buttonsToLower();
        else this->buttonsToUpper();
        shiftFlag = false;
    }
}

void KeyboardDialog::capsLock()
{
    if (!capsLockFlag)
    {
        this->buttonsToUpper();
        capsLockFlag = true;
    }
    else
    {
        this->buttonsToLower();
        capsLockFlag = false;
    }

}

void KeyboardDialog::someKeyClicked()
{
    emit this->keyClicked();
}

KeyboardButton::KeyboardButton(QString character)
{
    this->character = character;
    this->setText(character);
    connect(this, SIGNAL(clicked()), this, SLOT(thisClicked()));
}

QString KeyboardButton::getCharacter()
{
    return this->character;
}

void KeyboardButton::setCharacter(QString str)
{
    this->character = str;
}
