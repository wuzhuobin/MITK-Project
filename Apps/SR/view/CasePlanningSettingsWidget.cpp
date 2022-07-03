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

// qt
#include <QRadioButton>

// mitk
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>

CasePlanningSettingsWidget::CasePlanningSettingsWidget(
    const QString& objectName, QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::CasePlanningSettingsWidget>())
{
  mUi->setupUi(this);
  setObjectName("CasePlanningSettingsWidget_" + objectName);
  mUi->radioButton->setText(objectName);
  connect(mUi->toolButtonDelete,
          &QToolButton::clicked,
          this,
          &CasePlanningSettingsWidget::deleteClicked);
  connect(mUi->toolButtonHide,
          &QToolButton::clicked,
          this,
          &CasePlanningSettingsWidget::hideClicked);
}

CasePlanningSettingsWidget::~CasePlanningSettingsWidget() = default;

QRadioButton* CasePlanningSettingsWidget::getRadioButton() const
{
  return mUi->radioButton;
}

const QString CasePlanningSettingsWidget::getCasePlanningName() const
{
  return mUi->radioButton->text();
}

bool CasePlanningSettingsWidget::getVisibility() const
{
  return !mUi->toolButtonHide->isChecked();
}