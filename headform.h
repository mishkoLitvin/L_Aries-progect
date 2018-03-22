#ifndef HEADFORM_H
#define HEADFORM_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

class HeadSettingButton : public QPushButton{
    Q_OBJECT
public:
    HeadSettingButton(unsigned int inputNumber, QWidget *parent = 0)
    {
        this->setParent(parent);
        this->index = inputNumber;
        this->setStyleSheet("background-color: rgb(100,250,100);");
        this->resize(40,40);
        connect(this, SIGNAL(clicked()), this, SLOT(thisClicked()));
        this->setIcon(QIcon(":/new/icons/icons/settings.png"));
    }

signals:
    void settingButtonCliced(int index);
private:
    unsigned int index;
private slots:
    void thisClicked()
    {
        emit this->settingButtonCliced(this->index);
    }
};


namespace Ui {
class HeadForm;
}

class HeadForm : public QWidget
{
    Q_OBJECT

public:
    typedef enum HeadformType_{HeadPutingOn, HeadRemoving, HeadProcessing}HeadformType;

    HeadForm(QWidget *parent = 0);
    ~HeadForm();

    typedef enum HeadformState_ {shirtOn, shirtOff, shirtProcessing} HeadformState;
    typedef enum SettBtnPos_ {AtLeftUp, AtLeftDown, AtRightUp, AtRightDown } SettBtnPos;

    void setIndex(int i);
    void setIndexLabelPosition(SettBtnPos position );
    void setPixmap(HeadformState state, QString stStr);
    void setPixmap(HeadformState state);

    HeadformState getRagState();
    void setHeadformType(HeadformType type);
    HeadformType getHeadformType();

    QSize getLabelSize();

signals:
    void settingButtonCliced(int index);

private:
    Ui::HeadForm *ui;
    QLabel *labelIndex;
    QImage pixShirtShow, pixShirtHide, pixShirtAnimate;

    int index;
    HeadformState headformState;
    HeadformType headformType;


protected:
    void mousePressEvent(QMouseEvent *event);
};



#endif // HEADFORM_H
