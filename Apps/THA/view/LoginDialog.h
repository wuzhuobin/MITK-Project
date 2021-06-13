#ifndef LOGIN_DIALOG
#define LOGIN_DIALOG

// qt
#include <qdialog.h>

namespace Ui
{
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() override;

    // signals:
    //     void signalLoginDialogResult(const QString& userName, const QString& password);

    // private slots:
    //     void onLogin();

private:
    //     QRegExpValidator *pRevalidotor;
    //     void initialize();
    //     void onUserVerify(bool nameVerifyResut, bool pswVerifyResut);
    //     void showLoginWrongInfo();
    Ui::LoginDialog* mUi;
    Q_SLOT void on_pushButtonLogin_clicked(bool checked = false);
    //     bool m_verifyUserName = false;
    //     bool m_verifyPassword = false;
};
#endif  // LOGIN_DIALOG
