#ifndef GENERALSETTINGDIALOG_H
#define GENERALSETTINGDIALOG_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <QInputDialog>
#include <QShowEvent>

#include "settings.h"
#include "numpaddialog.h"
#include "keyboarddialog.h"
#include "serialsettingsdialog.h"

struct EmailSettings{
    QString senderAdress;
    QString senderPassword;
    QString receiverAdress;
    QString emailSubject;
    bool mailEnable;
};

Q_DECLARE_METATYPE(EmailSettings)

inline QDataStream& operator<<(QDataStream& out, const EmailSettings& st)
{
    out << st.senderAdress;
    out << st.senderPassword;
    out << st.receiverAdress;
    out << st.emailSubject;
    out << st.mailEnable;
    return out;
}
inline QDataStream& operator>>(QDataStream& in, EmailSettings& st)
{
    in >> st.senderAdress;
    in >> st.senderPassword;
    in >> st.receiverAdress;
    in >> st.emailSubject;
    in >> st.mailEnable;
    return in;
}

namespace Ui {
class GeneralSettingDialog;
}

class GeneralSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralSettingDialog(QWidget *parent = 0);
    ~GeneralSettingDialog();
    void setIconFolder(QString path);
    void setEmailSettings(EmailSettings emailSett);
    void setMachineSetting(MachineSettings::MachineParameters machineParam);
    void setFocusLossAccept(bool flag);
    void setPasswords(uint16_t serialPass, uint16_t mailPass, uint16_t userPass);
    void setStyle(QStringList stList, int curSelect, QStringList iconList, int iconSel, bool backGrEn);
    void setLangList(QStringList langList, int curSelect);
    void showPortInfo(ComSettings comSett);

signals:
    void emailSettingsChanged(EmailSettings);
    void machineParamChanged(QByteArray machinePararmArr);
    void serialPortSettingsDialogRequested();
    void styleChangedIndex(int index);
    void iconsChangedIndex(int index);
    void langChangedIndex(int index);
    void serviceSettingRequest();
    void headActivationRequest();
    void usersSettingRequest();
    void directionChanged(int dir);
    void unloadStateChanged(bool state);
    void sendCommand(QByteArray command);
    void imageRequest(bool enable, bool req = false);

private:
    Ui::GeneralSettingDialog *ui;

    MachineSettings::MachineParameters machineParamGl;

    bool acceptOnDeactilationEn;
    bool acceptEnable;
    bool logedInSerial;
    bool logedInMail;
    bool logedInUserSett;

    uint16_t serialPassword;
    uint16_t mailPassword;
    uint16_t usersPassword;

    QIcon directionIcon;
    QString pathIcon;

private slots:
    void accept();
    void reject();
    void lockUnlockEmail();
    void hideShowPassword();
    void eventFilterSetup();
    void changeSerialPortSettingsClicked();
    void changeServiceStateClicked();
    void userSettingClicked();
    void styleChanged(int index);
    void iconChanged(int index);
    void langChanged(int index);
    void changeDirection();
    void changeCyclesState();
    void useUnloadStateChanged();
    void headCountChanged(double arg1);
    void machineTypeChanget(int index);
    void warningTimeChanged(double arg1);
    void on_pButtonHeadsActivation_clicked();
    void on_checkBoxUseBackgr_clicked();
    void on_pButtonSelectImg_clicked();

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *ev);
    void changeEvent(QEvent *event);
};

#endif // GENERALSETTINGDIALOG_H
