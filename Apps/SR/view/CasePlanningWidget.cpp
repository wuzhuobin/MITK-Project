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
#include <mitkPointSetShapeProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <mitkToolManagerProvider.h>
#include <usModuleRegistry.h>

// qt
#include <QButtonGroup>
#include <QRadioButton>

class PointSetDataInteractorScrew : public mitk::PointSetDataInteractor
{
public:
  mitkClassMacro(PointSetDataInteractorScrew, mitk::PointSetDataInteractor);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);

  itkSetObjectMacro(Screw, mitk::DataNode);
  itkGetObjectMacro(Screw, mitk::DataNode);

protected:
  void FinishMove(mitk::StateMachineAction* action,
                  mitk::InteractionEvent* event) override
  {
    MITK_INFO << __func__;
    Superclass::FinishMove(action, event);
  }

private:
  mitk::DataNode::Pointer m_Screw;
};

CasePlanningWidget::CasePlanningWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(std::make_unique<Ui::CasePlanningWidget>()),
    mButtonGroupScrew(new QButtonGroup(this)),
    mPointSetDataInteractorScrew(PointSetDataInteractorScrew::New()),
    mButtonGroupPath(new QButtonGroup(this)),
    mPointSetDataInteractor(mitk::PointSetDataInteractor::New()),
    mButtonGroupPlate(new QButtonGroup(this)),
    mButtonGroupInterval(new QButtonGroup(this)),
    mToolManager(mitk::ToolManagerProvider::GetInstance()->GetToolManager())
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

  mPointSetDataInteractorScrew->LoadStateMachine("PointSet.xml");
  mPointSetDataInteractorScrew->SetEventConfig("PointSetConfig.xml");
  mPointSetDataInteractorScrew->SetMaxPoints(2);

  mPointSetDataInteractor->LoadStateMachine("PointSet.xml");
  mPointSetDataInteractor->SetEventConfig("PointSetConfig.xml");
  mPointSetDataInteractor->SetMaxPoints(2);

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

  auto screwSettingsWidgets =
      findChildren<ScrewSettingsWidget*>(QRegularExpression("screw_[0-9]?"));
  for (auto* screwSettingsWidget : screwSettingsWidgets)
  {
    ds->GetNamedNode(screwSettingsWidget->getScrewName().toStdString())
        ->SetVisibility(false);
  }

  auto pathSettingsWidgets = findChildren<PathSettingsWidget*>("path_[0-9]?");
  for (auto* pathSettingsWidget : pathSettingsWidgets)
  {
    ds->GetNamedNode(pathSettingsWidget->getPathName().toStdString())
        ->SetVisibility(false);
  }
  auto plateSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("plate_[0-9]?"));
  for (auto* plateSettingsWidget : plateSettingsWidgets)
  {
    ds->GetNamedNode(plateSettingsWidget->getCasePlanningName().toStdString())
        ->SetVisibility(false);
  }
  auto intervalSettingsWidgets = findChildren<CasePlanningSettingsWidget*>(
      QRegularExpression("interval_[0-9]?"));
  for (auto* intervalSettingsWidget : intervalSettingsWidgets)
  {
    ds->GetNamedNode(
          intervalSettingsWidget->getCasePlanningName().toStdString())
        ->SetVisibility(false);
  }

  // auto* multiWidget = SRStdMultiWidget::getInstance();
  // multiWidget->enableGroupBox(false);
  switch (index)
  {
    case 0: {  // dummy
      break;
    }
    case 1: {  // screw
      for (auto* widget : screwSettingsWidgets)
      {
        ds->GetNamedNode(widget->getScrewName().toStdString())
            ->SetVisibility(widget->getVisibility());
      }
      break;
    }
    case 2: {  // path
      for (auto* widget : pathSettingsWidgets)
      {
        ds->GetNamedNode(widget->getPathName().toStdString())
            ->SetVisibility(widget->getVisibility());
      }
      break;
    }
    case 3: {  // plate
      for (auto* widget : plateSettingsWidgets)
      {
        ds->GetNamedNode(widget->getCasePlanningName().toStdString())
            ->SetVisibility(widget->getVisibility());
      }
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
  screwSettingsWidget->setDiameter(5.0);
  screwSettingsWidget->setLength(10.0);
  mButtonGroupScrew->addButton(screwSettingsWidget->getRadioButton());
  screwSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxScrews->layout()->addWidget(screwSettingsWidget);

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
  pathSettingsWidget->setDiameter(10);
  mButtonGroupPath->addButton(pathSettingsWidget->getRadioButton());
  pathSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxPaths->layout()->addWidget(pathSettingsWidget);

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

void CasePlanningWidget::on_pushButtonPlateNew_clicked(bool checked)
{
  Q_UNUSED(checked);

  auto size = mButtonGroupPlate->buttons().size();
  QString plateNamePrefix("plate");
  auto plateName = QString(plateNamePrefix + "_%1").arg(size + 1);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* image = ds->GetNamedObject<mitk::Image>("image");

  mToolManager->SetDataStorage(*ds);
  mToolManager->RegisterClient();
  auto drawPaintBrushId =
      mToolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  auto* drawPaintBrush = mToolManager->GetToolById(drawPaintBrushId);

  float colorFloat[3] = {1.0f, 0.0f, 0.0f};
  auto plateNode = drawPaintBrush->CreateEmptySegmentationNode(
      image, plateName.toStdString(), colorFloat);
  ds->Add(plateNode);
  mToolManager->SetReferenceData(imageNode);
  mToolManager->SetWorkingData(plateNode);
  mToolManager->ActivateTool(drawPaintBrushId);

  auto* plateSettingsWidget = new CasePlanningSettingsWidget(plateName, this);
  mButtonGroupPlate->addButton(plateSettingsWidget->getRadioButton());
  plateSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxPlates->layout()->addWidget(plateSettingsWidget);

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

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageNode = ds->GetNamedNode("image");
  auto* image = ds->GetNamedObject<mitk::Image>("image");

  mToolManager->SetDataStorage(*ds);
  mToolManager->RegisterClient();
  auto drawPaintBrushId =
      mToolManager->GetToolIdByToolType<mitk::DrawPaintbrushTool>();
  auto* drawPaintBrush = mToolManager->GetToolById(drawPaintBrushId);

  float colorFloat[3] = {1.0f, 0.0f, 0.0f};
  auto intervalNode = drawPaintBrush->CreateEmptySegmentationNode(
      image, intervalName.toStdString(), colorFloat);
  ds->Add(intervalNode);
  mToolManager->SetReferenceData(imageNode);
  mToolManager->SetWorkingData(intervalNode);
  mToolManager->ActivateTool(drawPaintBrushId);

  auto* intervalSettingsWidget =
      new CasePlanningSettingsWidget(intervalName, this);
  mButtonGroupInterval->addButton(intervalSettingsWidget->getRadioButton());
  intervalSettingsWidget->getRadioButton()->setChecked(true);
  mUi->groupBoxIntervals->layout()->addWidget(intervalSettingsWidget);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}
void CasePlanningWidget::on_spinBoxIntervalSize_valueChanged(int value)
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
    // auto* multiWidget = SRStdMultiWidget::getInstance();
    // multiWidget->enableGroupBox(true);
    // multiWidget->setTransformTarget(screwName);
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto screwPointSetName = screwName.split("_").join("_point_set_");
    auto* screwNode = ds->GetNamedNode(screwName.toStdString());
    auto* screwPointSetNode = ds->GetNamedNode(screwPointSetName.toStdString());
    if (screwPointSetNode && screwNode)
    {
      mPointSetDataInteractorScrew->SetDataNode(screwPointSetNode);
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
  mToolManager->SetDataStorage(*ds);
  mToolManager->SetReferenceData(imageNode);
  mToolManager->SetWorkingData(plateNode);
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
  mToolManager->SetDataStorage(*ds);
  mToolManager->SetReferenceData(imageNode);
  mToolManager->SetWorkingData(intervalNode);
}