#ifndef apps_svpk_lib_login_dialog_h_
#define apps_svpk_lib_login_dialog_h_

#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>
#include <QRegExpValidator>

namespace Ui { class LoginDialog; }


class LoginDialog : public QMainWindow
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();


signals:
    void signalLoginDialogResult(const QString& userName, const QString& password);

private slots:
    void onLogin();

private:
    QRegExpValidator *pRevalidotor;
    void initialize();
    void onUserVerify(bool nameVerifyResut, bool pswVerifyResut);
    void showLoginWrongInfo();
    Ui::LoginDialog *m_ui = nullptr;
    bool m_verifyUserName = false;
    bool m_verifyPassword = false;
};
#endif // apps_svpk_lib_login_dialog_h_
