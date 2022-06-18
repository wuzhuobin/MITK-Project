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

// mitk
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>

PathSettingsWidget::PathSettingsWidget(const QString& pathName,
                                       QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::PathSettingsWidget>())
{
  mUi->setupUi(this);
  setObjectName("PathSettingsWidget_" + pathName);
  mUi->radioButtonScrew->setText(pathName);
}

PathSettingsWidget::~PathSettingsWidget() = default;

QRadioButton* PathSettingsWidget::getRadioButton() const
{
  return mUi->radioButtonScrew;
}

const QString PathSettingsWidget::getPathName() const
{
  return mUi->radioButtonScrew->text();
}

double PathSettingsWidget::getDiameter() const
{
  return mUi->doubleSpinBoxDiameter->value();
}

void PathSettingsWidget::setDiameter(double diameter)
{
  mUi->doubleSpinBoxDiameter->setValue(diameter);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* pathNode = ds->GetNamedNode(getPathName().toStdString());

  pathNode->SetIntProperty("line width", diameter);
  pathNode->SetFloatProperty("point 2D size", diameter);
  pathNode->SetFloatProperty("pointsize", diameter);
  pathNode->SetFloatProperty("contoursize", diameter * 0.5);
}

void PathSettingsWidget::on_toolButtonDelete_clicked(bool checked)
{
  Q_UNUSED(checked);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  ds->Remove(ds->GetNamedNode(getPathName().toStdString()));
  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  deleteLater();
}

void PathSettingsWidget::on_toolButtonHide_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* pathNode = ds->GetNamedNode(getPathName().toStdString());
  pathNode->SetVisibility(!checked);

  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}