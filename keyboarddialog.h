#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDebug>

class KeyboardButton : public QPushButton{
    Q_OBJECT
public:
    KeyboardButton(QString character);
    QString getCharacter();
    void setCharacter(QString str);
signals:
    void clicked(QString character);
private:
    QString character;
private slots:
    void thisClicked()
    {
        emit this->clicked(this->character);
    }
};

namespace Ui {
class KeyboardDialog;
}

class KeyboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyboardDialog(QWidget *parent = 0, QString windowTitle = "Keyboard");
    ~KeyboardDialog();
    static QString getText(QWidget *parent = 0, QString windowTitle = "Keyboard");
    void buttonsToUpper();
    void buttonsToLower();

private slots:
    void appendToLineEdit(QString str);
    void backspace();
    void submitText();
    void shift();
    void deshift();
    void capsLock();
    void someKeyClicked();

signals:
    void keyClicked();

private:
    QStringList characters;
    Ui::KeyboardDialog *ui;
    QList<KeyboardButton*> buttons;
    QString text;
    bool capsLockFlag;
    bool shiftFlag;
};

#endif // KEYBOARDDIALOG_H
