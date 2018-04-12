#ifndef NUMPADDIALOG_H
#define NUMPADDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDebug>
#include <QAction>
#include <QKeySequence>

class NumpadButton : public QPushButton{
    Q_OBJECT
public:
    NumpadButton(unsigned int inputNumber, QString name);
signals:
    void clicked(int index);
private:
    unsigned int index;
private slots:
    void thisClicked()
    {
        emit this->clicked(this->index);
        this->clearFocus();
    }
};

namespace Ui {
class NumpadDialog;
}

class NumpadDialog : public QDialog
{
    Q_OBJECT

signals:
    void valueSubmited(QString value);

public:
    explicit NumpadDialog(QWidget *parent = 0, QString windowTitle = "Numpad");
    ~NumpadDialog();

    static double getValue(QWidget *parent = 0, QString windowTitle = "Numpad");

private slots:
    void appendToLineEdit(int number);
    void submitValue();
    void changeSign();
    void appendZero();
    void addDot();
    void backspace();

private:
    Ui::NumpadDialog *ui;
    NumpadButton *buttons[10];

    const QString stSheet;

    QAction* actionSubmit;

    double value;
};

#endif // NUMPADDIALOG_H
