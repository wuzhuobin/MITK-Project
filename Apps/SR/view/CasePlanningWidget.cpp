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

#include "CasePlanningSettingsWidget.h"
#include "PathSettingsWidget.h"
#include "SRStdMultiWidget.h"
#include "ScrewSettingsWidget.h"
#include "ui_CasePlanningWidget.h"

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkDrawPaintbrushTool.h>
#include <mitkLogMacros.h>
#include <mitkPointSet.h>
#include <mitkPointSetDataInteractor.h>
#include <mitkPointSetShapeProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <mitkToolManager.h>
#include <mitkToolManagerProvider.h>
#include <usModuleRegistry.h>

// qt
#include <QButtonGroup>
#include <QRadioButton>

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(std::make_unique<Ui::CasePlanningWidget>()),
    mButtonGroupScrew(new QButtonGroup(this)),
    mButtonGroupPath(new QButtonGroup(this)),
    mButtonGroupPlate(new QButtonGroup(this)),
    mButtonGroupInterval(new QButtonGroup(this))
{
  mUi->setupUi(this);
  connect(mUi->spinBoxIntervalSize,
          QOverload<int>::of(&QSpinBox::valueChanged),
          mUi->horizontalSliderIntervalSize,
          &QSlider::setValue);
  connect(mUi->horizontalSliderIntervalSize,
          &QSlider::valueChanged,
          mUi->spinBoxIntervalSize,
          &QSpinBox::setValue);

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
  connect(mButtonGroupPlate,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupPlateButtonToggled);
  connect(mButtonGroupInterval,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupIntervalButtonToggled);

  auto toolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  toolManager->InitializeTools();
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

void CasePlanningWidget::onActionPlatePlanningTriggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->platePlanning);
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

void CasePlanningWidget::on_CasePlanningWidget_currentChanged(int index)
{
  Q_UNUSED(index);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* multiWidget = SRStdMultiWidget::getInstance();
  multiWidget->enableGroupBox(false);
  switch (index)
  {
    case 0:
      break;
    case 1: {
      break;
    }
    case 2: {
      break;
    }
    case 3: {
      break;
    }
    case 4: {
      break;
    }
    case 5: {
      break;
    }
    default:
      break;
  }
}

void CasePlanningWidget::on_pushButtonScrewNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupScrew->buttons().size();
  QString screwName("screw");
  auto newScrewName = QString(screwName + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  // auto* multiWidget = SRStdMultiWidget::getInstance();
  // auto pos = multiWidget->GetSelectedPosition("");
  auto* stdmultiWidget0Plane =
      ds->GetNamedObject<mitk::PlaneGeometryData>("stdmulti.widget0.plane");
  auto* stdmultiWidget1Plane =
      ds->GetNamedObject<mitk::PlaneGeometryData>("stdmulti.widget1.plane");
  auto* stdmultiWidget2Plane =
      ds->GetNamedObject<mitk::PlaneGeometryData>("stdmulti.widget2.plane");
  mitk::Vector3D translate;
  translate[0] = stdmultiWidget0Plane->GetPlaneGeometry()->GetCenter()[0];
  translate[1] = stdmultiWidget1Plane->GetPlaneGeometry()->GetCenter()[1];
  translate[2] = stdmultiWidget2Plane->GetPlaneGeometry()->GetCenter()[2];

  auto* screw = ds->GetNamedObject<mitk::Surface>(screwName.toStdString());
  auto newScrew = screw->Clone();
  newScrew->GetGeometry()->Translate(translate);
  auto screwNode = mitk::DataNode::New();
  screwNode->SetName(newScrewName.toStdString());
  screwNode->SetData(newScrew);
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

void CasePlanningWidget::on_pushButtonPlateNew_clicked(bool checked) {}

void CasePlanningWidget::on_pushButtonIntervalNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupInterval->buttons().size();
  QString intervalName("interval");
  auto newIntervalName = QString(intervalName + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* image = ds->GetNamedObject<mitk::Image>("image");

  auto toolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  toolManager->SetDataStorage(*ds);
  toolManager->RegisterClient();
  auto drawPaintBrushId =
      toolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  auto* drawPaintBrush = toolManager->GetToolById(drawPaintBrushId);

  float colorFloat[3] = {1.0f, 0.0f, 0.0f};
  auto intervalNode = drawPaintBrush->CreateEmptySegmentationNode(
      image, newIntervalName.toStdString(), colorFloat);
  ds->Add(intervalNode);
  toolManager->SetReferenceData(imageNode);
  toolManager->SetWorkingData(intervalNode);
  toolManager->ActivateTool(drawPaintBrushId);

  auto* intervalSettingsWidget =
      new CasePlanningSettingsWidget(newIntervalName, this);
  mButtonGroupInterval->addButton(intervalSettingsWidget->getRadioButton());
  mUi->groupBoxIntervals->layout()->addWidget(intervalSettingsWidget);

  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}
void CasePlanningWidget::on_spinBoxIntervalSize_valueChanged(int value)
{
  auto toolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  auto drawPaintBrushId =
      toolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  if (drawPaintBrushId == -1)
  {
    return;
  }

  auto* drawPaintBrush = static_cast<mitk::DrawPaintbrushTool*>(
      toolManager->GetToolById(drawPaintBrushId));
  drawPaintBrush->SetSize(value);
}

void CasePlanningWidget::onButtonGroupScrewButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
  auto screwName = button->text();
  auto* multiWidget = SRStdMultiWidget::getInstance();
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

void CasePlanningWidget::onButtonGroupPlateButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
}

void CasePlanningWidget::onButtonGroupIntervalButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
  auto intervalName = button->text();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* intervalNode = ds->GetNamedNode(intervalName.toStdString());
  auto* toolManager =
      mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  toolManager->SetDataStorage(*ds);
  toolManager->SetReferenceData(imageNode);
  toolManager->SetWorkingData(intervalNode);
}