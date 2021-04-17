#include "app_menu_widget.h"
#include<QVBoxLayout>
#include <QDesktopServices>
#include <QDesktopWidget>
#include<QDateTime>
#include <QProcess>
#include <QtWidgets/QApplication>
#include<QScreen>
#include<QMessagebox>
#include<QTimer>
// #include "apps/svpk_lib/translation.h"
// #include "utils/configuration/configuration.h"
// #include "apps/svpk_lib/shutdown_dialog.h"
// #include "apps/svpk_lib/theme.h"
// #include "apps/svpk_lib/session.h"
// #include "apps/svpk_lib/information_hint.h"
// #include "apps/svpk_lib/screenshot_info_dialog.h"
// #include "apps/svpk_lib/user_bar/user_manage_page.h"
// #include "apps/svpk_lib/user_bar/change_password_page.h"

namespace
{
    class AppMenuImpl final : public AppMenu
    {
    public:
        AppMenuImpl()
        {
            createAdminMenu();
            createUserMenu();
        }
    };
    Q_GLOBAL_STATIC(AppMenuImpl, g_instance);
};

void AppMenu::setAdminBtnMenu(QToolButton* adminBtn)
{
    adminBtn->setMenu(m_adminMenu);
}

void AppMenu::setUserBtnMenu(QToolButton* userBtn)
{
    userBtn->setMenu(m_userMenu);
}

void AppMenu::createAdminMenu()
{
    m_adminMenu = new QMenu; 
    std::vector<QString> actionTitles = { tr("Version"),tr("Switch Language"),tr("Log Export"),
       tr("System Desktop"),tr("Update"),tr("System Status") };
    Funs funs[] = {
    &AppMenu::versionInfoView,
    &AppMenu::switchLanguage,
    &AppMenu::exportLog,
    &AppMenu::toSystemDesktop,
    &AppMenu::update,
    &AppMenu::systemStatus
    };
    bindingMenuActions(m_adminMenu, actionTitles, funs);
}

void AppMenu::createUserMenu()
{   
    m_userMenu = new QMenu;
    std::vector<QString> actionTitles = { tr("Change Password"),tr("Account Management"),tr("User Logout") };
    Funs funs[] = {
    &AppMenu::changeUserPassword,
    &AppMenu::accountManagement,
    &AppMenu::userLogout
    };
    bindingMenuActions(m_userMenu, actionTitles, funs);
}

void AppMenu::bindingMenuActions(QMenu* menu, const std::vector<QString> &actionTitles, Funs funs[])
{
    // Theme::instance()->setMenuStyle(menu, 16);
    int count = actionTitles.size();
    for (int i = 0;i < count;i++)
    {
        QString actionTitle = QString("%1").arg(actionTitles[i]);
        auto* action = menu->addAction(actionTitle, this, funs[i]);
        action->setObjectName(actionTitle);
        action->setCheckable(true);
    }
}

void AppMenu::setScreenshotBtnConnect(QAbstractButton* screenshotBtn)
{
    connect(screenshotBtn, &QAbstractButton::clicked, this, &AppMenu::onScreenShot);
}
void AppMenu::setShutdownBtnConnect(QWidget *parent, QAbstractButton* shutdownBtn)
{    
    connect(shutdownBtn, &QAbstractButton::clicked, this, [=]() {onShutdown(parent);});
}
void AppMenu::versionInfoView()
{

}

void AppMenu::switchLanguage()
{
    // auto langVar = Configuration::user()->value("language", QLocale::English);
    // auto lang = langVar.value<QLocale::Language>();
    // lang = (lang == QLocale::English) ? QLocale::Chinese : QLocale::English;
    // Configuration::syncUpdateUserConfig("language", lang);

    QString app_fullName = qApp->applicationFilePath();
    QApplication::exit();
    QProcess restartProcess;
    restartProcess.startDetached(app_fullName);
    restartProcess.close();
}

void AppMenu::exportLog()
{

}

void AppMenu::toSystemDesktop()
{

}

void AppMenu::update()
{

}

void AppMenu::systemStatus()
{

}

//user admin menu action items:
void AppMenu::changeUserPassword()
{
    // ChangePasswordPage son;
    // son.setWindowTitle(tr("Change Password:"));
    // int m_ret = son.exec();
    // if (m_ret == QDialog::Accepted) {
    //     LOG_DEBUG("On Change password page, click Confirm Button");
    // }
    // else {
    //     LOG_DEBUG("Quit from ChangePassword Page");
    // }
}

void AppMenu::accountManagement()
{
    // UserManagePage son;
    // int m_ret = son.exec();
    // if (m_ret == QDialog::Accepted) {
    //     LOG_DEBUG("Quit from UserInfoPage");
    // }
    // else {
    //     LOG_DEBUG("Quit from UserInfoPage");
    // }
}

void AppMenu::userLogout()
{
    QString app_fullName = qApp->applicationFilePath();
    QApplication::exit();
    QProcess restartProcess;
    restartProcess.startDetached(app_fullName);
    restartProcess.close();

}

void AppMenu::onScreenShot()
{
    // if (Session::instance()->patientInfo()->patientName == nullptr) {
    //     InformationHint msg(tr("Please Choose a patient!"));
    //     msg.exec();
    // }
    // else
    // {
    //     this->m_pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    //     m_fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH`mm`ss") + ".bmp";

    //     if (m_pixmap.isNull())
    //     {
    //         InformationHint msg(tr("Screenshot is wrong !"));
    //         msg.exec();
    //     }
    //     else
    //     {
    //         QString curWorkBasePath = QApplication::applicationDirPath();
    //         QString m_path = QString("%1/user_folder/%2/%3/").arg(curWorkBasePath).arg(Session::instance()->account().userName).arg(Session::instance()->patientInfo()->patientName);
    //         if (!m_pixmap.save(m_path + m_fileName, "BMP"))
    //         {
    //             InformationHint msg(tr("There is a save problem!"));
    //             msg.exec();
    //         }
    //         else
    //         {
    //             ScreenshotInfoDialog son;
    //             son.exec();
    //         }
    //     }
    // }
}

void AppMenu::onShutdown(QWidget *parent)
{
    // ShutdownDialog* shutdownDialog = new ShutdownDialog(parent);
    // shutdownDialog->move(parent->x()+(parent->width() - shutdownDialog->width()) / 2, parent->y() + (parent->height() - shutdownDialog->height()) / 2);
    // shutdownDialog->show();
}

void AppMenu::closeApp()
{
	QApplication::exit();
}

void AppMenu::computer_PowerOff()
{
    //system("shutdown -s -t 1");
}

AppMenu* AppMenu::instance()
{
    return g_instance;
}
