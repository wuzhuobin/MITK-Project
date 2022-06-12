/**
 * @file CasePlanningWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CasePlanningWidget.h"

#include "ScrewSettingsWidget.h"
#include "ui_CasePlanningWidget.h"

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>

// qt
#include <QButtonGroup>
#include <QRadioButton>

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(std::make_unique<Ui::CasePlanningWidget>()),
    mButtonGroupScrew(new QButtonGroup(this))
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

void CasePlanningWidget::on_pushButtonNewScrew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupScrew->buttons().size();
  QString screwName("screw");
  auto newScrewName = QString(screwName + "_%1").arg(size + 1);
  auto* screwSettingsWidget = new ScrewSettingsWidget(newScrewName, this);
  mButtonGroupScrew->addButton(screwSettingsWidget->getRadioButton());
  mUi->groupBoxScrews->layout()->addWidget(screwSettingsWidget);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* screw = ds->GetNamedObject<mitk::Surface>(screwName.toStdString());
  auto screwNode = mitk::DataNode::New();
  screwNode->SetName(newScrewName.toStdString());
  screwNode->SetData(screw->Clone());
  screwNode->SetVisibility(true);
  ds->Add(screwNode);
  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_pushButtonScrewConfirm_clicked(bool checked)
{
  Q_UNUSED(checked);
  auto* button = mButtonGroupScrew->checkedButton();

  if (button)
  {
    auto screwName = button->text();
    auto screwSettingsWidgetObjectName = "ScrewSettingsWidget_" + screwName;
    auto* screwSettingsWidget =
        findChild<ScrewSettingsWidget*>(screwSettingsWidgetObjectName);
    if (screwSettingsWidget)
    {
      screwSettingsWidget->setDiameter(
          mUi->doubleSpinBoxScrewDiameter->value());
      screwSettingsWidget->setLength(mUi->spinBoxScrewLength->value());
    }
  }
}

void CasePlanningWidget::onButtonGroupScrewButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
  auto screwName = button->text();
  auto screwSettingsWidgetObjectName = "ScrewSettingsWidget_" + screwName;
  auto* screwSettingsWidget =
      findChild<ScrewSettingsWidget*>(screwSettingsWidgetObjectName);
  if (screwSettingsWidget)
  {
    mUi->doubleSpinBoxScrewDiameter->setValue(
        screwSettingsWidget->getDiameter());
    mUi->spinBoxScrewLength->setValue(screwSettingsWidget->getLength());
  }
}