#include "login_view.h"
#include "ui_login_view.h"
#include "theme.h"
#include "app_menu_widget.h"
#include "session.h"
#include "account_manager/account_manager.h"
#include "account_manager/account_manager_impl.h"
#include "account_manager/dbmanager.h"

LoginView::LoginView(QWidget *parent):QWidget(parent)
{
    m_ui = new Ui::LoginView();
    m_ui->setupUi(this);
    initialize();
    createloginDialog();

    connect(this, &LoginView::finished, this, &LoginView::hide);
}

LoginView::~LoginView()
{
    delete m_ui;
    delete m_loginDialog;
}

void initDataBase()
{
    AccountManager::initialize(new AccountManagerImpl());//To be added and fixed
    DbManager *db = new DbManager("sovajoDatabase.db");
}

void LoginView::initialize()
{
    initDataBase();
    Theme::instance()->setFlowViewStyle(this);
    Theme::instance()->setleftWidgetStyle(m_ui->widgetRight);
    Theme::instance()->setUserNameLabelStyle(m_ui->userNameLabel);

    AppMenu::instance()->setAdminBtnMenu(m_ui->menuBtn);
    //QObject::connect(Session::instance(), &Session::accountChanged, this, [=]() {
    //    m_ui->userNameLabel->setText(Session::instance()->account().userName);
    //    AppMenu::instance()->setUserBtnMenu(m_ui->userBtn);
    //});
    //AppMenu::instance()->setShutdownBtnConnect(this, m_ui->shutdownBtn);
}

void LoginView::createloginDialog()
{
    m_loginDialog = new LoginDialog(this);
    m_loginDialog->move(this->x() + (this->width() - m_loginDialog->width()) / 2, this->y() + (this->height() - m_loginDialog->height()) / 2);
    m_loginDialog->show();
    connect(m_loginDialog, &LoginDialog::signalLoginDialogResult, this, &LoginView::onUserLoginDialog);

}

void LoginView::onUserLoginDialog(const QString& userName, const QString& password)
{
    m_ui->userNameLabel->setText(userName);
    Account Account;
    Account.userName = userName;
    Account.passwordEncrypted = AccountManager::instance()->getEncryptDecrypt(password);
    Session::instance()->setAccount(Account);
    emit this->finished();
}
