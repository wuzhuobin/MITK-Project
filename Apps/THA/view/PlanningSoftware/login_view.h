#ifndef login_view_h_
#define login_view_h_

#include <QWidget>
#include "login_dialog.h"

namespace Ui { class LoginView; }

class LoginView :public QWidget 
{   
public:
    LoginView(QWidget *parent = nullptr);
    ~LoginView();

private:
    void initialize();
    void createloginDialog();
    void onUserLoginDialog(const QString& userName, const QString& password);
    Ui::LoginView *m_ui = nullptr;
    LoginDialog *m_loginDialog = nullptr;
};
#endif // apps_svpk_lib_login_view_h_