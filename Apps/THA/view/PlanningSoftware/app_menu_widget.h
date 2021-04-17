#ifndef apps_svpk_lib_app_menu_widget_h_
#define apps_svpk_lib_app_menu_widget_h_

#include <QWidget>
#include <QAbstractButton>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <vector>
#include <QPixmap>

class AppMenu : public QObject
{
    Q_OBJECT

public:
    void AppMenu::setAdminBtnMenu(QToolButton* adminBtn);
    void AppMenu::setUserBtnMenu(QToolButton* userBtn);
    void setScreenshotBtnConnect(QAbstractButton* screenshotBtn);
    void setShutdownBtnConnect(QWidget *parent, QAbstractButton* shutdownBtn);
    static AppMenu* instance();

protected:
    AppMenu() = default;
    void createAdminMenu();
    void createUserMenu();    

private:
    typedef void (AppMenu::*Funs)();   
    void bindingMenuActions(QMenu* menu,const std::vector<QString> &actionTitles, Funs funs[]);

    //system admin menu action items:
    void versionInfoView();
    void switchLanguage();
    void exportLog();
    void toSystemDesktop();
    void update();
    void systemStatus();
    void(*pFunc)();

    //user admin menu action items:
    void changeUserPassword();
    void accountManagement();
    void userLogout();

    void onScreenShot();
    void onShutdown(QWidget *parent);

    void closeApp();
    void computer_PowerOff();

    QMenu* m_adminMenu = nullptr;
    QMenu* m_userMenu = nullptr;
    QPixmap m_pixmap;
    QString m_fileName;
    
};

#endif // apps_svpk_lib_app_menu_widget_h_
