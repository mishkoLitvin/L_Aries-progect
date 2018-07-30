#ifndef HEADFORM_H
#define HEADFORM_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsEffect>

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
    }
    void setIconPath(QString path)
    {
        this->setIcon(QIcon(path));
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

typedef enum LoadState_
{
    LoadClean = 0x00,
    LoadOne = 0x01,
    LoadAuto = 0x02
}LoadState;

namespace Ui {
class HeadForm;
}

class HeadForm : public QWidget
{
    Q_OBJECT

public:
    typedef enum HeadformType_
    {
        HeadPutingOn,
        HeadRemoving,
        HeadProcessing}
    HeadformType;

    HeadForm(QWidget *parent = 0);
    ~HeadForm();

    typedef enum HeadformState_ {shirtOn, shirtOff, shirtProcessing} HeadformState;
    typedef enum SettBtnPos_ {AtLeftUp, AtLeftDown, AtRightUp, AtRightDown } SettBtnPos;

    void setIndex(int i);
    void setIndexLabelPosition(SettBtnPos position );
    void setPixmap(HeadformState state, QString stStr);
    void setPixmap(HeadformState state);
    void setRagOn(bool state);
    void setIconPath(QString path);
    void setRagColor(QColor color);

    HeadformState getRagState();
    void setHeadformType(HeadformType type);
    HeadformType getHeadformType();

    QSize getLabelSize();

signals:
    void settingButtonCliced(int index);
    void loadStateChanged(LoadState state);

private:
    Ui::HeadForm *ui;
    QLabel *labelIndex;
    QImage pixShirtShow, pixShirtHide, pixShirtAnimate;
    QGraphicsColorizeEffect *graphEffect;
    QString pathImage;

    int index;
    HeadformState headformState;
    HeadformType headformType;


protected:
    void mousePressEvent(QMouseEvent *event);
};



#endif // HEADFORM_H
