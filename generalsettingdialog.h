#ifndef GENERALSETTINGDIALOG_H
#define GENERALSETTINGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QByteArray>

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

signals:
    void emailSettingsChanged(EmailSettings);

private:
    Ui::GeneralSettingDialog *ui;
    bool acceptOnDeactilationEn;

private slots:
    void accept();
    void reject();
    void lockUnlockEmail();
    void hideShowPassword();

protected:
    bool event(QEvent *e);
};

#endif // GENERALSETTINGDIALOG_H