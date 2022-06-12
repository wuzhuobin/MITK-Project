/**
 * @file PathSettingsWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com.com)
 * @brief
 * @version 0.1
 * @date 2022-06-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "PathSettingsWidget.h"

#include "ui_PathSettingsWidget.h"

PathSettingsWidget::PathSettingsWidget(const QString& pathName,
                                       QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::PathSettingsWidget>())
{
  mUi->setupUi(this);
}

PathSettingsWidget::~PathSettingsWidget() = default;