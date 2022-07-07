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
#include <mitkPlanarAngle.h>
#include <mitkPlane.h>
#include <mitkPointSet.h>
#include <mitkPointSetShapeProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <mitkToolManagerProvider.h>
#include <usModuleRegistry.h>

// vtk
#include <vtkCylinderSource.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

// qt
#include <QButtonGroup>
#include <QRadioButton>

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(std::make_unique<Ui::CasePlanningWidget>()),
    mButtonGroupScrew(new QButtonGroup(this)),
    mPointSetDataInteractorScrew(PointSetDataInteractorScrew::New()),
    mButtonGroupPath(new QButtonGroup(this)),
    mPointSetDataInteractor(mitk::PointSetDataInteractor::New()),
    mButtonGroupPlate(new QButtonGroup(this)),
    mButtonGroupInterval(new QButtonGroup(this)),
    mClippingPlaneInteractors{mitk::ClippingPlaneInteractor3D::New(),
                              mitk::ClippingPlaneInteractor3D::New()},
    mButtonGroupLateral(new QButtonGroup(this)),
    mButtonGroupPosterior(new QButtonGroup(this)),
    mPlanarFigureInteractor(mitk::PlanarFigureInteractor::New()),
    mToolManager(mitk::ToolManagerProvider::GetInstance()->GetToolManager())
{
  mUi->setupUi(this);
  connect(mUi->spinBoxPlateSize,
          QOverload<int>::of(&QSpinBox::valueChanged),
          mUi->horizontalSliderPlateSize,
          &QSlider::setValue);
  connect(mUi->horizontalSliderPlateSize,
          &QSlider::valueChanged,
          mUi->spinBoxPlateSize,
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
  connect(mButtonGroupLateral,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupLateralButtonToggled);
  connect(mButtonGroupPosterior,
          static_cast<void (QButtonGroup::*)(QAbstractButton*, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &CasePlanningWidget::onButtonGroupPosteriorButtonToggled);

  mPointSetDataInteractorScrew->LoadStateMachine("PointSet.xml");
  mPointSetDataInteractorScrew->SetEventConfig("PointSetConfig.xml");
  mPointSetDataInteractorScrew->SetMaxPoints(2);

  mPointSetDataInteractor->LoadStateMachine("PointSet.xml");
  mPointSetDataInteractor->SetEventConfig("PointSetConfig.xml");
  mPointSetDataInteractor->SetMaxPoints(2);

  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->LoadStateMachine(
        "ClippingPlaneInteraction3D.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
    clippingPlaneInteractor->SetEventConfig(
        "ClippingPlaneTranslationConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
  }

  mPlanarFigureInteractor->LoadStateMachine(
      "PlanarFigureInteraction.xml",
      us::ModuleRegistry::GetModule("MitkPlanarFigure"));
  mPlanarFigureInteractor->SetEventConfig(
      "PlanarFigureConfig.xml",
      us::ModuleRegistry::GetModule("MitkPlanarFigure"));

  mToolManager->InitializeTools();
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
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto screwSettingsWidgets = findChildren<ScrewSettingsWidget*>(
      QRegularExpression("ScrewSettingsWidget_screw_[0-9]?"));
  for (auto* screwSettingsWidget : screwSettingsWidgets)
  {
    ds->GetNamedNode(screwSettingsWidget->getScrewName().toStdString())
        ->SetVisibility(false);
    ds->GetNamedNode(screwSettingsWidget->getScrewName()
                         .split("_")
                         .join("_point_set_")
                         .toStdString())
        ->SetVisibility(false);
  }
  mPointSetDataInteractorScrew->SetDataNode(nullptr);

  auto pathSettingsWidgets = findChildren<PathSettingsWidget*>(
      QRegularExpression("PathSettingsWidget_path_[0-9]?"));
  for (auto* pathSettingsWidget : pathSettingsWidgets)
  {
    ds->GetNamedNode(pathSettingsWidget->getPathName().toStdString())
        ->SetVisibility(false);
  }
  mPointSetDataInteractor->SetDataNode(nullptr);

  auto plateSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("CasePlanningSettingsWidget_plate_[0-9]?"));
  for (auto* plateSettingsWidget : plateSettingsWidgets)
  {
    ds->GetNamedNode(plateSettingsWidget->getCasePlanningName().toStdString())
        ->SetVisibility(false);
  }
  mToolManager->SetWorkingData(nullptr);
  mToolManager->SetReferenceData(nullptr);
  mToolManager->UnregisterClient();

  auto intervalSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("CasePlanningSettingsWidget_interval_[0-9]?"));
  for (auto* intervalSettingsWidget : intervalSettingsWidgets)
  {
    ds->GetNamedNode(
          intervalSettingsWidget->getCasePlanningName().toStdString())
        ->SetVisibility(false);
    ds->GetNamedNode(intervalSettingsWidget->getCasePlanningName()
                         .split("_")
                         .join("_point_set_")
                         .toStdString())
        ->SetVisibility(false);
  }
  mToolManager->SetWorkingData(nullptr);
  mToolManager->SetReferenceData(nullptr);
  mToolManager->UnregisterClient();

  auto lateralSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("CasePlanningSettingsWidget_lateral_[0-9]?"));
  for (auto* lateralSettingsWidgets : lateralSettingsWidgets)
  {
    for (auto i = 0; i < 2; ++i)
    {
      ds->GetNamedNode((lateralSettingsWidgets->getCasePlanningName() + "_" +
                        QString::number(i))
                           .toStdString())
          ->SetVisibility(false);
    }
  }
  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetDataNode(nullptr);
  }

  auto posteriorSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("CasePlanningSettingsWidget_posterior_[0-9]?"));
  for (auto* posteriorSettingsWidgets : posteriorSettingsWidgets)
  {
    for (auto i = 0; i < 2; ++i)
    {
      ds->GetNamedNode((posteriorSettingsWidgets->getCasePlanningName() + "_" +
                        QString::number(i))
                           .toStdString())
          ->SetVisibility(false);
    }
  }

  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetDataNode(nullptr);
  }

  switch (index)
  {
    case 0: {  // dummy
      break;
    }
    case 1: {  // screw
      for (auto* widget : screwSettingsWidgets)
      {
        auto* screwNode =
            ds->GetNamedNode(widget->getScrewName().toStdString());
        auto* screwPointSetNode = ds->GetNamedNode(widget->getScrewName()
                                                       .split("_")
                                                       .join("_point_set_")
                                                       .toStdString());
        screwPointSetNode->SetVisibility(widget->getVisibility());
        screwNode->SetVisibility(widget->getVisibility());
        if (widget->getRadioButton()->isChecked())
        {
          onButtonGroupScrewButtonToggled(widget->getRadioButton(), true);
        }
      }
      break;
    }
    case 2: {  // path
      for (auto* widget : pathSettingsWidgets)
      {
        auto* pathNode = ds->GetNamedNode(widget->getPathName().toStdString());
        pathNode->SetVisibility(widget->getVisibility());
        if (widget->getRadioButton()->isChecked())
        {
          onButtonGroupPathButtonToggled(widget->getRadioButton(), true);
        }
      }
      break;
    }
    case 3: {  // plate
      mToolManager->RegisterClient();
      for (auto* widget : plateSettingsWidgets)
      {
        auto* plateNode =
            ds->GetNamedNode(widget->getCasePlanningName().toStdString());
        auto* imageNode = ds->GetNamedNode("image");
        plateNode->SetVisibility(widget->getDataNodeVisibility());
        if (widget->getRadioButton()->isChecked())
        {
          onButtonGroupPlateButtonToggled(widget->getRadioButton(), true);
        }
      }
      break;
    }
    case 4: {  // interval
      for (auto* widget : intervalSettingsWidgets)
      {
        auto* intervalNode =
            ds->GetNamedNode(widget->getCasePlanningName().toStdString());
        auto* intervalPointSetNode =
            ds->GetNamedNode(widget->getCasePlanningName()
                                 .split("_")
                                 .join("_point_set_")
                                 .toStdString());
        intervalPointSetNode->SetVisibility(widget->getDataNodeVisibility());
        intervalNode->SetVisibility(widget->getDataNodeVisibility());
        if (widget->getRadioButton()->isChecked())
        {
          onButtonGroupIntervalButtonToggled(widget->getRadioButton(), true);
        }
      }
      break;
    }
    case 5: {  // lateral
      for (auto* widget : lateralSettingsWidgets)
      {
        for (auto i = 0; i < 2; ++i)
        {
          auto* lateralNode = ds->GetNamedNode(
              (widget->getCasePlanningName() + "_" + QString::number(i))
                  .toStdString());
          lateralNode->SetVisibility(widget->getDataNodeVisibility());
          if (widget->getRadioButton()->isChecked())
          {
            onButtonGroupLateralButtonToggled(widget->getRadioButton(), true);
          }
        }
      }
      on_radioButtonLateralTranslate_toggled(
          mUi->radioButtonLateralTranslate->isChecked());
      on_radioButtonLateralRotate_toggled(
          mUi->radioButtonLateralRotate->isChecked());
      break;
    }
    case 6: {  // posterior
      for (auto* widget : posteriorSettingsWidgets)
      {
        for (auto i = 0; i < 2; ++i)
        {
          auto* posteriorNode = ds->GetNamedNode(
              (widget->getCasePlanningName() + "_" + QString::number(i))
                  .toStdString());
          posteriorNode->SetVisibility(widget->getDataNodeVisibility());
          if (widget->getRadioButton()->isChecked())
          {
            onButtonGroupPosteriorButtonToggled(widget->getRadioButton(), true);
          }
        }
      }

      on_radioButtonPosteriorTranslate_toggled(
          mUi->radioButtonPosteriorTranslate->isChecked());
      on_radioButtonPosteriorRotate_toggled(
          mUi->radioButtonPosteriorRotate->isChecked());
    }
    default:
      break;
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_pushButtonScrewNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupScrew->buttons().size();
  QString screwNamePrefix("screw");
  auto screwName = QString(screwNamePrefix + "_%1").arg(size + 1);
  auto screwPointSetName =
      QString(screwNamePrefix + "_point_set" + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* originalScrew =
      ds->GetNamedObject<mitk::Surface>(screwNamePrefix.toStdString());
  auto screwNode = mitk::DataNode::New();
  screwNode->SetName(screwName.toStdString());
  screwNode->SetData(originalScrew->Clone());
  screwNode->SetVisibility(true);
  ds->Add(screwNode);

  auto pointSetScrewNode = mitk::DataNode::New();
  pointSetScrewNode->SetName(screwPointSetName.toStdString());
  pointSetScrewNode->SetData(mitk::PointSet::New());
  pointSetScrewNode->SetVisibility(true);
  ds->Add(pointSetScrewNode);

  auto* screwSettingsWidget = new ScrewSettingsWidget(screwName, this);
  mButtonGroupScrew->addButton(screwSettingsWidget->getRadioButton());
  screwSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxScrews->layout()->addWidget(screwSettingsWidget);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_doubleSpinBoxScrewDiameter_valueChanged(
    double value)
{
  auto* button = mButtonGroupScrew->checkedButton();
  auto screwName = button->text();
  auto screwSettingsWidgetObjectName = "ScrewSettingsWidget_" + screwName;
  auto* screwSettingsWidget =
      findChild<ScrewSettingsWidget*>(screwSettingsWidgetObjectName);
  if (screwSettingsWidget)
  {
    screwSettingsWidget->setDiameter(value);
    mPointSetDataInteractorScrew->SetScrewDiameter(value);
    mPointSetDataInteractorScrew->updateScrew();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
}

void CasePlanningWidget::on_doubleSpinBoxScrewLength_valueChanged(double value)
{
  auto* button = mButtonGroupScrew->checkedButton();
  auto screwName = button->text();
  auto screwSettingsWidgetObjectName = "ScrewSettingsWidget_" + screwName;
  auto* screwSettingsWidget =
      findChild<ScrewSettingsWidget*>(screwSettingsWidgetObjectName);
  if (screwSettingsWidget)
  {
    screwSettingsWidget->setDiameter(value);
    mPointSetDataInteractorScrew->SetScrewLength(value);
    mPointSetDataInteractorScrew->updateScrew();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
}

void CasePlanningWidget::on_pushButtonPathNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupPath->buttons().size();
  QString pathNamePrefix("path");
  auto pathName = QString(pathNamePrefix + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto pathNode = mitk::DataNode::New();
  pathNode->SetName(pathName.toStdString());
  pathNode->SetData(mitk::PointSet::New());
  pathNode->SetVisibility(true);
  pathNode->SetProperty(
      "Pointset.2D.shape",
      mitk::PointSetShapeProperty::New(mitk::PointSetShapeProperty::CIRCLE));
  pathNode->SetBoolProperty("show contour", true);
  pathNode->SetBoolProperty("show points", true);
  pathNode->SetBoolProperty("fill shape", true);
  ds->Add(pathNode);

  auto* pathSettingsWidget = new PathSettingsWidget(pathName, this);
  mButtonGroupPath->addButton(pathSettingsWidget->getRadioButton());
  pathSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxPaths->layout()->addWidget(pathSettingsWidget);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

// void CasePlanningWidget::on_pushButtonPathConfirm_clicked(bool checked)
// {
//   Q_UNUSED(checked);
//   auto* button = mButtonGroupPath->checkedButton();

//   if (button)
//   {
//     auto screwName = button->text();
//     auto pathSettingsWidgetObjectName = "PathSettingsWidget_" + screwName;
//     auto* pathSettingsWidget =
//         findChild<PathSettingsWidget*>(pathSettingsWidgetObjectName);
//     if (pathSettingsWidget)
//     {
//       pathSettingsWidget->setDiameter(mUi->doubleSpinBoxPathDiameter->value());
//     }
//   }
// }

void CasePlanningWidget::on_pushButtonPlateNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupPlate->buttons().size();
  QString plateNamePrefix("plate");
  auto plateName = QString(plateNamePrefix + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* image = ds->GetNamedObject<mitk::Image>("image");

  auto drawPaintBrushId =
      mToolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  auto* drawPaintBrush = mToolManager->GetToolById(drawPaintBrushId);

  float colorFloat[3] = {1.0f, 0.0f, 0.0f};
  auto plateNode = drawPaintBrush->CreateEmptySegmentationNode(
      image, plateName.toStdString(), colorFloat);
  ds->Add(plateNode);

  auto* plateSettingsWidget = new CasePlanningSettingsWidget(plateName, this);
  plateSettingsWidget->setDoubleSpinBoxDiameterVisible(false);
  plateSettingsWidget->setDoubleSpinBoxLengthVisible(false);
  mButtonGroupPlate->addButton(plateSettingsWidget->getRadioButton());
  plateSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxPlates->layout()->addWidget(plateSettingsWidget);
  connect(plateSettingsWidget,
          &CasePlanningSettingsWidget::deleteClicked,
          this,
          &CasePlanningWidget::onPlateDeleteClicked);
  connect(plateSettingsWidget,
          &CasePlanningSettingsWidget::hideClicked,
          this,
          &CasePlanningWidget::onPlateHideClicked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_spinBoxPlateSize_valueChanged(int value)
{
  auto drawPaintBrushId =
      mToolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  if (drawPaintBrushId == -1)
  {
    return;
  }

  auto* drawPaintBrush = static_cast<mitk::DrawPaintbrushTool*>(
      mToolManager->GetToolById(drawPaintBrushId));
  drawPaintBrush->SetSize(value);
}

void CasePlanningWidget::on_pushButtonIntervalNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupInterval->buttons().size();
  QString intervalNamePrefix("interval");
  auto intervalName = QString(intervalNamePrefix + "_%1").arg(size + 1);
  auto intervalPointSetName =
      QString(intervalNamePrefix + "_point_set" + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
  cylinderSource->SetRadius(0.5);
  cylinderSource->SetHeight(2.0);
  cylinderSource->SetResolution(20);
  cylinderSource->SetCapping(true);
  cylinderSource->Update();

  auto interval = mitk::Surface::New();
  interval->SetVtkPolyData(cylinderSource->GetOutput());

  auto intervalNode = mitk::DataNode::New();
  intervalNode->SetName(intervalName.toStdString());
  intervalNode->SetData(interval);
  intervalNode->SetVisibility(true);
  ds->Add(intervalNode);

  auto pointSetIntervalNode = mitk::DataNode::New();
  pointSetIntervalNode->SetName(intervalPointSetName.toStdString());
  pointSetIntervalNode->SetData(mitk::PointSet::New());
  pointSetIntervalNode->SetVisibility(true);
  ds->Add(pointSetIntervalNode);

  auto* intervalSettingsWidget =
      new CasePlanningSettingsWidget(intervalName, this);
  mButtonGroupInterval->addButton(intervalSettingsWidget->getRadioButton());
  intervalSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxIntervals->layout()->addWidget(intervalSettingsWidget);

  connect(intervalSettingsWidget,
          &CasePlanningSettingsWidget::deleteClicked,
          this,
          &CasePlanningWidget::onIntervalDeleteClicked);
  connect(intervalSettingsWidget,
          &CasePlanningSettingsWidget::hideClicked,
          this,
          &CasePlanningWidget::onIntervalHideClicked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_doubleSpinBoxIntervalDiameter_valueChanged(
    double value)
{
  auto* button = mButtonGroupInterval->checkedButton();
  auto intervalName = button->text();
  auto casePlanningSettingsWidgetObjectName =
      "CasePlanningSettingsWidget_" + intervalName;
  auto* casePlanningSettingsWidget = findChild<CasePlanningSettingsWidget*>(
      casePlanningSettingsWidgetObjectName);
  if (casePlanningSettingsWidget)
  {
    casePlanningSettingsWidget->setDiameter(value);
    mPointSetDataInteractorScrew->SetScrewDiameter(value);
    mPointSetDataInteractorScrew->updateScrew();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
}

void CasePlanningWidget::on_doubleSpinBoxIntervalLength_valueChanged(
    double value)
{
  auto* button = mButtonGroupInterval->checkedButton();
  auto intervalName = button->text();
  auto casePlanningSettingsWidgetObjectName =
      "CasePlanningSettingsWidget_" + intervalName;
  auto* casePlanningSettingsWidget = findChild<CasePlanningSettingsWidget*>(
      casePlanningSettingsWidgetObjectName);
  if (casePlanningSettingsWidget)
  {
    casePlanningSettingsWidget->setLength(value);
    mPointSetDataInteractorScrew->SetScrewLength(value);
    mPointSetDataInteractorScrew->updateScrew();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
}

void CasePlanningWidget::on_pushButtonLateralNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupLateral->buttons().size();
  QString lateralNamePrefix("lateral");
  auto lateralName = QString(lateralNamePrefix + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* image = ds->GetNamedObject<mitk::Image>("image");

  auto extentX = image->GetGeometry()->GetExtentInMM(0);
  auto extentY = image->GetGeometry()->GetExtentInMM(1);

  for (auto i = 0; i < 2; ++i)
  {
    auto lateral0 = mitk::Plane::New();
    lateral0->SetOrigin(image->GetGeometry()->GetCenter());
    lateral0->SetExtent(extentX, extentY);

    auto scalars0 = vtkSmartPointer<vtkFloatArray>::New();
    scalars0->SetName("Distance");
    scalars0->SetNumberOfComponents(1);
    scalars0->SetNumberOfTuples(
        lateral0->GetVtkPolyData()->GetNumberOfPoints());
    lateral0->GetVtkPolyData()->GetPointData()->SetScalars(scalars0);

    auto lateralNode0 = mitk::DataNode::New();
    lateralNode0->SetName(
        (lateralName + "_" + QString::number(i)).toStdString());
    lateralNode0->SetData(lateral0);
    lateralNode0->SetVisibility(true);
    lateralNode0->SetBoolProperty("pickable", true);
    ds->Add(lateralNode0);
  }

  auto* lateralSettingsWidget =
      new CasePlanningSettingsWidget(lateralName, this);
  lateralSettingsWidget->setDoubleSpinBoxDiameterVisible(false);
  lateralSettingsWidget->setDoubleSpinBoxLengthVisible(false);
  mButtonGroupLateral->addButton(lateralSettingsWidget->getRadioButton());
  lateralSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxLaterals->layout()->addWidget(lateralSettingsWidget);
  connect(lateralSettingsWidget,
          &CasePlanningSettingsWidget::hideClicked,
          this,
          &CasePlanningWidget::onLateralHideClicked);
  connect(lateralSettingsWidget,
          &CasePlanningSettingsWidget::deleteClicked,
          this,
          &CasePlanningWidget::onLateralDeleteClicked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_radioButtonLateralTranslate_toggled(bool checked)
{
  if (!checked)
  {
    return;
  }
  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetEventConfig(
        "ClippingPlaneTranslationConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
  }
}

void CasePlanningWidget::on_radioButtonLateralRotate_toggled(bool checked)
{
  if (!checked)
  {
    return;
  }
  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetEventConfig(
        "ClippingPlaneRotationConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
  }
}

void CasePlanningWidget::on_pushButtonPosteriorNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupPosterior->buttons().size();
  QString posteriorNamePrefix("posterior");
  auto posteriorName = QString(posteriorNamePrefix + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto posteriorNode0 = mitk::DataNode::New();
  posteriorNode0->SetName(posteriorName.toStdString());
  posteriorNode0->SetData(mitk::PlanarAngle::New());
  posteriorNode0->SetVisibility(true);
  posteriorNode0->SetSelected(true);
  ds->Add(posteriorNode0);

  // auto* image = ds->GetNamedObject<mitk::Image>("image");

  // auto extentX = image->GetGeometry()->GetExtentInMM(0);
  // auto extentY = image->GetGeometry()->GetExtentInMM(1);

  // for (auto i = 0; i < 2; ++i)
  // {
  //   auto posterior0 = mitk::Plane::New();
  //   posterior0->SetOrigin(image->GetGeometry()->GetCenter());
  //   posterior0->SetExtent(extentX, extentY);

  //   auto scalars0 = vtkSmartPointer<vtkFloatArray>::New();
  //   scalars0->SetName("Distance");
  //   scalars0->SetNumberOfComponents(1);
  //   scalars0->SetNumberOfTuples(
  //       posterior0->GetVtkPolyData()->GetNumberOfPoints());
  //   posterior0->GetVtkPolyData()->GetPointData()->SetScalars(scalars0);

  //   auto posteriorNode0 = mitk::DataNode::New();
  //   posteriorNode0->SetName(
  //       (posteriorName + "_" + QString::number(i)).toStdString());
  //   posteriorNode0->SetData(posterior0);
  //   posteriorNode0->SetVisibility(true);
  //   posteriorNode0->SetBoolProperty("pickable", true);
  //   ds->Add(posteriorNode0);
  // }

  auto* posteriorSettingsWidget =
      new CasePlanningSettingsWidget(posteriorName, this);
  posteriorSettingsWidget->setDoubleSpinBoxDiameterVisible(false);
  posteriorSettingsWidget->setDoubleSpinBoxLengthVisible(false);
  mButtonGroupPosterior->addButton(posteriorSettingsWidget->getRadioButton());
  posteriorSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxPosteriors->layout()->addWidget(posteriorSettingsWidget);
  connect(posteriorSettingsWidget,
          &CasePlanningSettingsWidget::hideClicked,
          this,
          &CasePlanningWidget::onPosteriorHideClicked);
  connect(posteriorSettingsWidget,
          &CasePlanningSettingsWidget::deleteClicked,
          this,
          &CasePlanningWidget::onPosteriorDeleteClicked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::on_radioButtonPosteriorTranslate_toggled(bool checked)
{
  if (!checked)
  {
    return;
  }
  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetEventConfig(
        "ClippingPlaneTranslationConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
  }
}

void CasePlanningWidget::on_radioButtonPosteriorRotate_toggled(bool checked)
{
  if (!checked)
  {
    return;
  }
  for (auto clippingPlaneInteractor : mClippingPlaneInteractors)
  {
    clippingPlaneInteractor->SetEventConfig(
        "ClippingPlaneRotationConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
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
    mUi->doubleSpinBoxScrewLength->setValue(screwSettingsWidget->getLength());
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto screwPointSetName = screwName.split("_").join("_point_set_");
    auto* screwNode = ds->GetNamedNode(screwName.toStdString());
    auto* screwPointSetNode = ds->GetNamedNode(screwPointSetName.toStdString());
    if (screwPointSetNode && screwNode)
    {
      mPointSetDataInteractorScrew->SetDataNode(screwPointSetNode);
      mPointSetDataInteractorScrew->SetScrew(screwNode);
    }
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
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto* pathNode = ds->GetNamedNode(pathName.toStdString());
    if (pathNode)
    {
      mPointSetDataInteractor->SetDataNode(pathNode);
    }
  }
}

void CasePlanningWidget::onButtonGroupPlateButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
  auto plateName = button->text();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* plateNode = ds->GetNamedNode(plateName.toStdString());
  auto drawPaintBrushId =
      mToolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  mToolManager->SetDataStorage(*ds);
  mToolManager->SetReferenceData(imageNode);
  mToolManager->SetWorkingData(plateNode);
  mToolManager->ActivateTool(drawPaintBrushId);
}

void CasePlanningWidget::onButtonGroupIntervalButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }
  auto intervalName = button->text();
  auto casePlanningSettingsWidgetObjectName =
      "CasePlanningSettingsWidget_" + intervalName;
  auto* casePlanningSettingsWidget = findChild<CasePlanningSettingsWidget*>(
      casePlanningSettingsWidgetObjectName);
  if (casePlanningSettingsWidget)
  {
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto intervalPointSetName = intervalName.split("_").join("_point_set_");
    auto* intervalNode = ds->GetNamedNode(intervalName.toStdString());
    auto* intervalPointSetNode =
        ds->GetNamedNode(intervalPointSetName.toStdString());
    if (intervalPointSetNode && intervalNode)
    {
      mPointSetDataInteractorScrew->SetDataNode(intervalPointSetNode);
      mPointSetDataInteractorScrew->SetScrew(intervalNode);
    }

    mUi->doubleSpinBoxIntervalDiameter->setValue(
        casePlanningSettingsWidget->getDiameter());
    mUi->doubleSpinBoxIntervalLength->setValue(
        casePlanningSettingsWidget->getLength());
  }
}

void CasePlanningWidget::onButtonGroupLateralButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }

  auto lateralName = button->text();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  for (auto i = 0; i < 2; ++i)
  {
    auto* lateralNode0 = ds->GetNamedNode(
        (lateralName + "_" + QString::number(i)).toStdString());
    mClippingPlaneInteractors[i]->SetDataNode(lateralNode0);
  }
}

void CasePlanningWidget::onButtonGroupPosteriorButtonToggled(
    QAbstractButton* button, bool checked)
{
  if (!checked)
  {
    return;
  }

  auto posteriorName = button->text();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* posteriorNode0 = ds->GetNamedNode(posteriorName.toStdString());
  mPlanarFigureInteractor->SetDataNode(posteriorNode0);
  // for (auto i = 0; i < 2; ++i)
  // {
  //   auto* posteriorNode0 = ds->GetNamedNode(
  //       (posteriorName + "_" + QString::number(i)).toStdString());
  //   mClippingPlaneInteractors[i]->SetDataNode(posteriorNode0);
  // }
}

void CasePlanningWidget::onPlateDeleteClicked(bool checked)
{
  Q_UNUSED(checked);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  ds->Remove(ds->GetNamedNode(widget->getCasePlanningName().toStdString()));

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  widget->deleteLater();
}

void CasePlanningWidget::onPlateHideClicked(bool checked)
{
  Q_UNUSED(checked);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  auto* plateNode =
      ds->GetNamedNode(widget->getCasePlanningName().toStdString());
  plateNode->SetVisibility(!checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::onIntervalDeleteClicked(bool checked)
{
  Q_UNUSED(checked);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  auto* intervalNode =
      ds->GetNamedNode(widget->getCasePlanningName().toStdString());
  auto* intervalPointSetNode = ds->GetNamedNode(widget->getCasePlanningName()
                                                    .split("_")
                                                    .join("_point_set_")
                                                    .toStdString());
  ds->Remove(intervalNode);
  ds->Remove(intervalPointSetNode);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  widget->deleteLater();
}

void CasePlanningWidget::onIntervalHideClicked(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  auto* intervalNode =
      ds->GetNamedNode(widget->getCasePlanningName().toStdString());
  auto* intervalPointSetNode = ds->GetNamedNode(widget->getCasePlanningName()
                                                    .split("_")
                                                    .join("_point_set_")
                                                    .toStdString());
  intervalNode->SetVisibility(!checked);
  intervalPointSetNode->SetVisibility(!checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::onLateralDeleteClicked(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  for (auto i = 0; i < 2; ++i)
  {
    ds->Remove(ds->GetNamedNode(
        (widget->getCasePlanningName() + "_" + QString::number(i))
            .toStdString()));
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  widget->deleteLater();
}

void CasePlanningWidget::onLateralHideClicked(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  for (auto i = 0; i < 2; ++i)
  {
    auto* lateralNode = ds->GetNamedNode(
        (widget->getCasePlanningName() + "_" + QString::number(i))
            .toStdString());
    lateralNode->SetVisibility(!checked);
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CasePlanningWidget::onPosteriorDeleteClicked(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  for (auto i = 0; i < 2; ++i)
  {
    ds->Remove(ds->GetNamedNode(
        (widget->getCasePlanningName() + "_" + QString::number(i))
            .toStdString()));
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  widget->deleteLater();
}

void CasePlanningWidget::onPosteriorHideClicked(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* widget = static_cast<CasePlanningSettingsWidget*>(sender());
  for (auto i = 0; i < 2; ++i)
  {
    auto* posteriorNode = ds->GetNamedNode(
        (widget->getCasePlanningName() + "_" + QString::number(i))
            .toStdString());
    posteriorNode->SetVisibility(!checked);
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}