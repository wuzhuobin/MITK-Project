#include "LoginDialog.h"
#include "ui_LoginDialog.h"

// qt
#include <qregularexpression.h>
#include <qvalidator.h>


LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), mUi(new Ui::LoginDialog)
{
    mUi->setupUi(this);
    auto* validator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{25}"));
    mUi->lineEditPassword->setValidator(validator);
    // setWindowFlags(Qt::WindowStaysOnTopHint);
    // initialize();
}

LoginDialog::~LoginDialog()
{
    delete mUi;
}

// void LoginDialog::initialize()
// {
//     // Theme::instance()->setFlowViewStyle(this);
//     this->setWindowModality(Qt::WindowModality::NonModal);
//     QPalette pa;
//     pa.setColor(QPalette::WindowText, Qt::red);
//     m_ui->loginInformation->setPalette(pa);
//     // Theme::instance()->setTitleWidgetStyle(m_ui->widget);

//     pRevalidotor = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
//     m_ui->lineEditUserName->setPlaceholderText(tr("User ID"));
//     m_ui->lineEditUserName->setValidator(pRevalidotor); //(only english and numbers)
//     // Theme::instance()->setLineEditStyle(m_ui->lineEditUserName);
//     m_ui->lineEditPassword->setPlaceholderText(tr("Password"));
//     // Theme::instance()->setLineEditStyle(m_ui->lineEditPassword);
//     m_ui->lineEditPassword->setValidator(pRevalidotor);
//     m_ui->lineEditPassword->setEchoMode(QLineEdit::Password); //password input model（show****）
//     connect(m_ui->loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
// }

// //Login Button Function
// void LoginDialog::onLogin()
// {
//     QString name = m_ui->lineEditUserName->text();
//     QString password = m_ui->lineEditPassword->text();

//     if (true)
//     // if (AccountManager::instance()->login(name, password))
//     {
//         this->close(); //should close this page first and then show create case page. or this page will exist.
//         Session::instance()->setAccount(AccountManager::instance()->getAccount(name));
//         Q_EMIT signalLoginDialogResult(name, password);
//     }
//     else
//     {
//         showLoginWrongInfo();
//     }
// }

// void LoginDialog::onUserVerify(bool nameVerifyResut, bool pswVerifyResut)
// {
//     m_verifyUserName = nameVerifyResut;
//     m_verifyPassword = pswVerifyResut;
// }

// void LoginDialog::showLoginWrongInfo()
// {
//     if (!(m_verifyPassword && m_verifyUserName))
//     {
//         QString s = tr("Sorry. Account is wrong!");
//         m_ui->loginInformation->setText(s);
//     }
// }

void LoginDialog::on_pushButtonLogin_clicked(bool /*checked*/)
{
    accept();
}