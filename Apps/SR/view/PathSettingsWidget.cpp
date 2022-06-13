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
  mUi->radioButtonScrew->setText(pathName);
}

PathSettingsWidget::~PathSettingsWidget() = default;

QRadioButton* PathSettingsWidget::getRadioButton() const
{
  return mUi->radioButtonScrew;
}

double PathSettingsWidget::getDiameter() const
{
  return mUi->doubleSpinBoxDiameter->value();
}

void PathSettingsWidget::setDiameter(double diameter)
{
  mUi->doubleSpinBoxDiameter->setValue(diameter);
}

void PathSettingsWidget::on_toolButtonDelete_clicked(bool checked)
{
  Q_UNUSED(checked);
  //   auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  //   ds->Remove(ds->GetNamedNode(getPathName().toStdString()));
  //   mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  //   mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  deleteLater();
}

void PathSettingsWidget::on_toolButtonHide_toggled(bool checked)
{
  //   auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  //   auto* screwNode = ds->GetNamedNode(getPathName().toStdString());
  //   screwNode->SetVisibility(!checked);

  //   mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  //   mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}