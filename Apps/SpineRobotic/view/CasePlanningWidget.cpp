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

void CasePlanningWidget::onActionScrewPlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->screwPlanning);
}

void CasePlanningWidget::onActionPathPlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->pathPlanning);
}
void CasePlanningWidget::onActionIntervalPlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->intervalPlanning);
}
void CasePlanningWidget::onActionLateralPlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->lateralPlanning);
}

void CasePlanningWidget::onActionPosteriorPlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->posteriorPlanning);
}
