/**
 * @file CasePlanningWidget.cpp
 * @author wuzhuobin (zhuobin.wu@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CasePlanningWidget.h"

#include "ui_CasePlanningWidget.h"

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent), mUi(std::make_unique<Ui::CasePlanningWidget>())
{
  mUi->setupUi(this);
}

CasePlanningWidget::~CasePlanningWidget() = default;
