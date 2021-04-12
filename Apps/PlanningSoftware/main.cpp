#include "PlanningSoftware.h"
#include <QtWidgets/QApplication>
#include <QStackedWidget>
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include "account_manager/dbmanager.h"
#include "QtSql/QSqlDatabase"
#include "account_manager/account_manager.h"
#include "account_manager/account_manager_impl.h"
#include "login_view.h"
#include "account_manager/account_manager.h"
#include "account_manager/account_manager_impl.h"
#include "session.h"

void initDataBase()
{
    AccountManager::initialize(new AccountManagerImpl());//To be added and fixed
    DbManager *db = new DbManager("sovajoDatabase.db");
}

void initGui(QStackedWidget* topWidget)
{
    // Create top level three views: LoginView, CaseManageView, MainWindow

    auto* loginView = new LoginView;
    topWidget->addWidget(loginView);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initDataBase();

    Q_INIT_RESOURCE(resource);
    //add app logo
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QApplication::setWindowIcon(QIcon(":/svpk_lib/images/sw_logo.png"));

    QStackedWidget topView;
    initGui(&topView);
    topView.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    topView.setWindowState(Qt::WindowFullScreen);
    topView.show();

    return a.exec();
}
