/**
 * @file ScrewSettingsWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ScrewSettingsWidget.h"

#include "ui_ScrewSettingsWidget.h"

// qt
#include <QButtonGroup>

// mitk
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>

ScrewSettingsWidget::ScrewSettingsWidget(const QString& screwName,
                                         QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::ScrewSettingsWidget>())
{
  mUi->setupUi(this);
  setObjectName("ScrewSettingsWidget_" + screwName);
  mUi->radioButtonScrew->setText(screwName);
}

ScrewSettingsWidget::~ScrewSettingsWidget() = default;

QRadioButton* ScrewSettingsWidget::getRadioButton() const
{
  return mUi->radioButtonScrew;
}

const QString ScrewSettingsWidget::getScrewName() const
{
  return mUi->radioButtonScrew->text();
}

double ScrewSettingsWidget::getDiameter() const
{
  return mUi->doubleSpinBoxDiameter->value();
}

void ScrewSettingsWidget::setDiameter(double diameter)
{
  mUi->doubleSpinBoxDiameter->setValue(diameter);
}

int ScrewSettingsWidget::getLength() const
{
  return mUi->spinBoxLength->value();
}

void ScrewSettingsWidget::setLength(int length)
{
  mUi->spinBoxLength->setValue(length);
}

bool ScrewSettingsWidget::getVisibility() const
{
  return !mUi->toolButtonHide->isChecked();
}

void ScrewSettingsWidget::on_toolButtonDelete_clicked(bool checked)
{
  Q_UNUSED(checked);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  ds->Remove(ds->GetNamedNode(getScrewName().toStdString()));
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  deleteLater();
}

void ScrewSettingsWidget::on_toolButtonHide_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* screwNode = ds->GetNamedNode(getScrewName().toStdString());
  screwNode->SetVisibility(!checked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}