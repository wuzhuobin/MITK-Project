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

double CasePlanningSettingsWidget::getDiameter() const
{
  return mUi->doubleSpinBoxDiameter->value();
}

void CasePlanningSettingsWidget::setDiameter(double diameter)
{
  mUi->doubleSpinBoxDiameter->setValue(diameter);
}

void CasePlanningSettingsWidget::setDoubleSpinBoxDiameterVisible(bool visible)
{
  mUi->doubleSpinBoxDiameter->setVisible(visible);
}

bool CasePlanningSettingsWidget::getDoubleSpinBoxDiameterVisible() const
{
  return mUi->doubleSpinBoxDiameter->isVisible();
}

double CasePlanningSettingsWidget::getLength() const
{
  return mUi->doubleSpinBoxLength->value();
}

void CasePlanningSettingsWidget::setLength(double length)
{
  mUi->doubleSpinBoxLength->setValue(length);
}

void CasePlanningSettingsWidget::setDoubleSpinBoxLengthVisible(bool visible)
{
  mUi->doubleSpinBoxLength->setVisible(visible);
}

bool CasePlanningSettingsWidget::getDoubleSpinBoxLengthVisible() const
{
  return mUi->doubleSpinBoxLength->isVisible();
}

bool CasePlanningSettingsWidget::getDataNodeVisibility() const
{
  return !mUi->toolButtonHide->isChecked();
}