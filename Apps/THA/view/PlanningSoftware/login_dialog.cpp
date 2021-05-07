#include "login_dialog.h"
#include "ui_login_dialog.h"
#include "theme.h"
#include "session.h"
#include "account_manager/account_manager.h"

struct LoginDaiglogResourceInit
{
    LoginDaiglogResourceInit()
    {
        Q_INIT_RESOURCE(resource);
    }
} static init;

LoginDialog::LoginDialog(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::LoginDialog)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    initialize();
}

LoginDialog::~LoginDialog()
{
    delete m_ui;
}

void LoginDialog::initialize()
{
    // Theme::instance()->setFlowViewStyle(this);
    this->setWindowModality(Qt::WindowModality::NonModal);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    m_ui->loginInformation->setPalette(pa);
    // Theme::instance()->setTitleWidgetStyle(m_ui->widget);

    pRevalidotor = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
    m_ui->lineEditUserName->setPlaceholderText(tr("User ID"));
    m_ui->lineEditUserName->setValidator(pRevalidotor); //(only english and numbers)
    // Theme::instance()->setLineEditStyle(m_ui->lineEditUserName);
    m_ui->lineEditPassword->setPlaceholderText(tr("Password"));
    // Theme::instance()->setLineEditStyle(m_ui->lineEditPassword);
    m_ui->lineEditPassword->setValidator(pRevalidotor);
    m_ui->lineEditPassword->setEchoMode(QLineEdit::Password); //password input model（show****）
    connect(m_ui->loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
}

//Login Button Function
void LoginDialog::onLogin()
{
    QString name = m_ui->lineEditUserName->text();
    QString password = m_ui->lineEditPassword->text();

    if (true)
    // if (AccountManager::instance()->login(name, password))
    {
        this->close(); //should close this page first and then show create case page. or this page will exist.
        Session::instance()->setAccount(AccountManager::instance()->getAccount(name));
        emit signalLoginDialogResult(name, password);
    }
    else
    {
        showLoginWrongInfo();
    }
}

void LoginDialog::onUserVerify(bool nameVerifyResut, bool pswVerifyResut)
{
    m_verifyUserName = nameVerifyResut;
    m_verifyPassword = pswVerifyResut;
}

void LoginDialog::showLoginWrongInfo()
{
    if (!(m_verifyPassword && m_verifyUserName))
    {
        QString s = tr("Sorry. Account is wrong!");
        m_ui->loginInformation->setText(s);
    }
}