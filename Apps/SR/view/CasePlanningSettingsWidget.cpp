/**
 * @file CasePlanningSettingsWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CasePlanningSettingsWidget.h"

#include "ui_CasePlanningSettingsWidget.h"

CasePlanningSettingsWidget::CasePlanningSettingsWidget(QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::CasePlanningSettingsWidget>())
{
  mUi->setupUi(this);
}

CasePlanningSettingsWidget::~CasePlanningSettingsWidget() = default;