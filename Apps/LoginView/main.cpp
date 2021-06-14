#include <QtWidgets/QApplication>
#include <QStackedWidget>
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include "PlanningSoftware/account_manager/dbmanager.h"
#include "QtSql/QSqlDatabase"
#include "PlanningSoftware/account_manager/account_manager.h"
#include "PlanningSoftware/account_manager/account_manager_impl.h"
#include "login_view.h"
#include "PlanningSoftware/account_manager/account_manager.h"
#include "PlanningSoftware/account_manager/account_manager_impl.h"
#include "PlanningSoftware/session.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //add app logo
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QApplication::setWindowIcon(QIcon(":/svpk_lib/images/sw_logo.png"));

    LoginView w;
    // topView.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    // topView.setWindowState(Qt::WindowFullScreen);
    w.show();

    return a.exec();
}
