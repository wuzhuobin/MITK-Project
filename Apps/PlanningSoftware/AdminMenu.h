/**
 * @file AdminMenu.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2021-06-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef MENU_MENU
#define MENU_MENU

// qt
#include <qglobal.h>
#include <qmenu.h>

class AdminMenu : public QMenu {
  Q_OBJECT
public:
  explicit AdminMenu(QWidget *parent = nullptr);

private:
  Q_DISABLE_COPY(AdminMenu);
};

#endif //! MENU_MENU