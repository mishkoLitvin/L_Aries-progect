#ifndef GENERALSETTINGDIALOG_H
#define GENERALSETTINGDIALOG_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <QInputDialog>

#include "settings.h"
#include "numpaddialog.h"
#include "serialsettingsdialog.h"

struct EmailSettings{
    QString senderAdress;
    QString senderPassword;
    QString receiverAdress;
    QString emailSubject;
};

Q_DECLARE_METATYPE(EmailSettings)

inline QDataStream& operator<<(QDataStream& out, const EmailSettings& st)
{
    out << st.senderAdress;
    out << st.senderPassword;
    out << st.receiverAdress;
    out << st.emailSubject;

    return out;
}
inline QDataStream& operator>>(QDataStream& in, EmailSettings& st)
{
    in >> st.senderAdress;
    in >> st.senderPassword;
    in >> st.receiverAdress;
    in >> st.emailSubject;

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
    void setEmailSettings(EmailSettings emailSett);
    void setMachineSetting(MachineSettings::MachineParameters machineParam);
    void setFocusLossAccept(bool flag);
    void setPasswords(uint16_t serialPass, uint16_t mailPass);
    void setStyleList(QStringList stList, int curSelect);
    void showPortInfo(ComSettings comSett);

signals:
    void emailSettingsChanged(EmailSettings);
    void machineParamChanged(QByteArray machinePararmArr);
    void serialPortSettingsDialogRequested();
    void styleChangedIndex(int index);

private:
    Ui::GeneralSettingDialog *ui;
    bool acceptOnDeactilationEn;
    bool logedInSerial = false;
    bool logedInMail = false;
    uint16_t serialPassword;
    uint16_t mailPassword;

private slots:
    void accept();
    void reject();
    void lockUnlockEmail();
    void hideShowPassword();
    void eventFilterSetup();
    void changeSerialPortSettingsClicked();
    void styleChanged(int index);

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // GENERALSETTINGDIALOG_H
