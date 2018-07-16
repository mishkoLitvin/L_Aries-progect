#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDebug>
#include <QApplication>

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
        this->clearFocus();
    }
};

namespace Ui {
class KeyboardDialog;
}

class KeyboardDialog : public QDialog
{
    Q_OBJECT
private:
    QStringList characters;
    Ui::KeyboardDialog *ui;
    QList<KeyboardButton*> buttons;
    KeyboardButton* dashUnderscoreButton;
    QString text;
    bool capsLockFlag;
    bool shiftFlag;


public:
    typedef enum KeyboardPosition_{
        Bottom,
        Custom
    }KeyboardPosition;

    explicit KeyboardDialog(QWidget *parent = 0, QString windowTitle = "Keyboard");
    ~KeyboardDialog();
    static QString getText(QWidget *parent = 0, QString windowTitle = "Keyboard", KeyboardPosition position = Custom);
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
    void dashUnderscoreSwitch(bool shifted = false); //if(!shifted): dash to underscore and vice versa

signals:
    void keyClicked();
protected:
    void changeEvent(QEvent *event);
};

#endif // KEYBOARDDIALOG_H
