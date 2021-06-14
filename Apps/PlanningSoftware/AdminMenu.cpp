/**
 * @file AdminMenu.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AdminMenu.h"

// qt
#include <qaction.h>
#include <qmenu.h>
#include <qwidget.h>

AdminMenu::AdminMenu(QWidget *parent) : QMenu(parent) {
  QStringList actionTitles = {tr("Version"),    tr("Switch Language"),
                              tr("Log Export"), tr("System Desktop"),
                              tr("Update"),     tr("System Status")};
  QStringList actionNames = {"action_Version",    "action_Switch_Language",
                             "action_Log_Export", "action_System_Desktop",
                             "action_Update",     "action_System_Status"};
  for (auto i = 0; i < actionTitles.size(); ++i) {
    QAction *action = new QAction(actionTitles[i]);
    action->setObjectName(actionNames[i]);
    this->addAction(action);
  }
}