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

#include "PathSettingsWidget.h"
#include "SRStdMultiWidget.h"
#include "ScrewSettingsWidget.h"
#include "ui_CasePlanningWidget.h"

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkPointSet.h>
#include <mitkPointSetDataInteractor.h>
#include <mitkPointSetShapeProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>

// qt
#include <QButtonGroup>
#include <QRadioButton>

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(std::make_unique<Ui::CasePlanningWidget>()),
    mButtonGroupScrew(new QButtonGroup(this)),
    mButtonGroupPath(new QButtonGroup(this))
{
  mUi->setupUi(this);

  connect(mButtonGroupScrew,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupScrewButtonToggled);
  connect(mButtonGroupPath,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupPathButtonToggled);
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

void CasePlanningWidget::on_pushButtonScrewNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupScrew->buttons().size();
  QString screwName("screw");
  auto newScrewName = QString(screwName + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* screw = ds->GetNamedObject<mitk::Surface>(screwName.toStdString());
  auto screwNode = mitk::DataNode::New();
  screwNode->SetName(newScrewName.toStdString());
  screwNode->SetData(screw->Clone());
  screwNode->SetVisibility(true);
  ds->Add(screwNode);

  auto* screwSettingsWidget = new ScrewSettingsWidget(newScrewName, this);
  screwSettingsWidget->setDiameter(5.0);
  screwSettingsWidget->setLength(10.0);
  mButtonGroupScrew->addButton(screwSettingsWidget->getRadioButton());
  mUi->groupBoxScrews->layout()->addWidget(screwSettingsWidget);

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

void CasePlanningWidget::on_pushButtonPathNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupPath->buttons().size();
  QString pathName("path");
  auto newPathName = QString(pathName + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto pathNode = mitk::DataNode::New();
  pathNode->SetName(newPathName.toStdString());
  pathNode->SetData(mitk::PointSet::New());
  pathNode->SetVisibility(true);
  pathNode->SetProperty(
      "Pointset.2D.shape",
      mitk::PointSetShapeProperty::New(mitk::PointSetShapeProperty::CIRCLE));
  pathNode->SetBoolProperty("show contour", true);
  pathNode->SetBoolProperty("show points", true);
  pathNode->SetBoolProperty("fill shape", true);
  ds->Add(pathNode);

  auto pointSetInteractor = mitk::PointSetDataInteractor::New();
  pointSetInteractor->LoadStateMachine("PointSet.xml");
  pointSetInteractor->SetEventConfig("PointSetConfig.xml");
  pointSetInteractor->SetDataNode(pathNode);
  pointSetInteractor->SetMaxPoints(2);

  auto* pathSettingsWidget = new PathSettingsWidget(newPathName, this);
  pathSettingsWidget->setDiameter(10);
  mButtonGroupPath->addButton(pathSettingsWidget->getRadioButton());
  mUi->groupBoxPaths->layout()->addWidget(pathSettingsWidget);

  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_pushButtonPathConfirm_clicked(bool checked)
{
  Q_UNUSED(checked);
  auto* button = mButtonGroupPath->checkedButton();

  if (button)
  {
    auto screwName = button->text();
    auto pathSettingsWidgetObjectName = "PathSettingsWidget_" + screwName;
    auto* pathSettingsWidget =
        findChild<PathSettingsWidget*>(pathSettingsWidgetObjectName);
    if (pathSettingsWidget)
    {
      pathSettingsWidget->setDiameter(mUi->doubleSpinBoxPathDiameter->value());
    }
  }
}

void CasePlanningWidget::onButtonGroupScrewButtonToggled(
    QAbstractButton* button, bool checked)
{
  auto* multiWidget = SRStdMultiWidget::getInstance();
  if (!checked)
  {
    return;
  }
  auto screwName = button->text();
  multiWidget->enableGroupBox(true);
  multiWidget->setTransformTarget(screwName);
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

void CasePlanningWidget::onButtonGroupPathButtonToggled(QAbstractButton* button,
                                                        bool checked)
{
  auto* multiWidget = SRStdMultiWidget::getInstance();
  if (!checked)
  {
    return;
  }
  auto pathName = button->text();
  auto pathSettingsWidgetObjectName = "PathSettingsWidget_" + pathName;
  auto* pathSettingsWidget =
      findChild<PathSettingsWidget*>(pathSettingsWidgetObjectName);
  if (pathSettingsWidget)
  {
    mUi->doubleSpinBoxPathDiameter->setValue(pathSettingsWidget->getDiameter());
  }
}
