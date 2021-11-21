#include "ToolsWidget.h"
#include "ui_ToolsWidget.h"

// qt
#include <itkMath.h>
#include <qcoreevent.h>
#include <qwidget.h>

// mitk
#include <mitkDataStorage.h>
#include <mitkPlanarAngle.h>
#include <mitkPlanarFigureInteractor.h>
#include <mitkPlanarLine.h>
#include <mitkRenderingManager.h>
#include <usModuleRegistry.h>

// itk
#include <itkCommand.h>

ToolsWidget::ToolsWidget(QWidget *parent)
    : QWidget(parent), mUi(new Ui::ToolsWidget) {

  this->mUi->setupUi(this);

  connect(this->mUi->stackedWidget, &QStackedWidget::currentChanged, this,
          &ToolsWidget::on_stackedWidget_currentChanged);
}

ToolsWidget::~ToolsWidget() { delete this->mUi; }

void ToolsWidget::setCurrentPage(ToolsWidget::Page page) {
  switch (page) {
  case Page::Line:
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->pageLine);
    break;
  case Page::Angle:
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->pageAngle);
    break;
  default:
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->pageDummy);
  }
}

void ToolsWidget::changeEvent(QEvent *event) {
  QWidget::changeEvent(event);
  if (event->type() == QEvent::EnabledChange && !this->isEnabled()) {
    this->setCurrentPage(Page::Dummy);
  }
}

void ToolsWidget::updateDistance(itk::Object *object,
                                 const itk::EventObject &event) {
  mitk::PlanarLine *planarLine = static_cast<mitk::PlanarLine *>(object);

  double distance = planarLine->GetQuantity(0);
  QString suffix = " " + QString(planarLine->GetFeatureUnit(0));
  this->mUi->doubleSpinBoxDistance->setValue(distance);
  this->mUi->doubleSpinBoxDistance->setSuffix(suffix);
}

void ToolsWidget::updateAngle(itk::Object *object,
                              const itk::EventObject &event) {
  mitk::PlanarAngle *planarAngle = static_cast<mitk::PlanarAngle *>(object);

  double angle = planarAngle->GetQuantity(planarAngle->FEATURE_ID_ANGLE);
  angle *= 180;
  angle /= itk::Math::pi;
  QString suffix =
      " " + QString(planarAngle->GetFeatureUnit(planarAngle->FEATURE_ID_ANGLE));
  this->mUi->doubleSpinBoxAngle->setValue(angle);
  this->mUi->doubleSpinBoxAngle->setSuffix(suffix);
}

void ToolsWidget::on_stackedWidget_currentChanged(int index) {

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer planarLineNode = ds->GetNamedNode("planar_line");
  mitk::DataNode::Pointer planarAngleNode = ds->GetNamedNode("planar_angle");

  if (planarLineNode) {
    ds->Remove(planarLineNode);
  }
  if (planarAngleNode) {
    ds->Remove(planarAngleNode);
  }

  if (this->mUi->stackedWidget->widget(index) == this->mUi->pageLine) {

    typedef itk::MemberCommand<ToolsWidget> MemberCommand;
    MemberCommand::Pointer memberCommand = MemberCommand::New();
    memberCommand->SetCallbackFunction(this, &ToolsWidget::updateDistance);

    planarLineNode = mitk::DataNode::New();
    planarLineNode->SetName("planar_line");
    planarLineNode->SetData(mitk::PlanarLine::New());
    planarLineNode->SetVisibility(true);
    planarLineNode->SetSelected(true);
    planarLineNode->GetData()->AddObserver(
        mitk::EndInteractionPlanarFigureEvent(), memberCommand);
    ds->Add(planarLineNode);

    mitk::PlanarFigureInteractor::Pointer planarFigureInteractor =
        mitk::PlanarFigureInteractor::New();
    planarFigureInteractor->LoadStateMachine(
        "PlanarFigureInteraction.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->SetEventConfig(
        "PlanarFigureConfig.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->EnableInteraction(true);
    planarFigureInteractor->SetDataNode(planarLineNode);

  } else if (this->mUi->stackedWidget->widget(index) == this->mUi->pageAngle) {

    typedef itk::MemberCommand<ToolsWidget> MemberCommand;
    MemberCommand::Pointer memberCommand = MemberCommand::New();
    memberCommand->SetCallbackFunction(this, &ToolsWidget::updateAngle);

    planarAngleNode = mitk::DataNode::New();
    planarAngleNode->SetName("planar_angle");
    planarAngleNode->SetData(mitk::PlanarAngle::New());
    planarAngleNode->SetVisibility(true);
    planarAngleNode->SetSelected(true);
    planarAngleNode->GetData()->AddObserver(
        mitk::EndInteractionPlanarFigureEvent(), memberCommand);
    ds->Add(planarAngleNode);

    mitk::PlanarFigureInteractor::Pointer planarFigureInteractor =
        mitk::PlanarFigureInteractor::New();
    planarFigureInteractor->LoadStateMachine(
        "PlanarFigureInteraction.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->SetEventConfig(
        "PlanarFigureConfig.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->EnableInteraction(true);
    planarFigureInteractor->SetDataNode(planarAngleNode);
  } else {
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}