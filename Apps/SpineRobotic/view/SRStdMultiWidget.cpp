#include "SRStdMultiWidget.h"
#include "GroupBoxGadget.h"

// qt
#include <itkCommand.h>
#include <mitkDataInteractor.h>
#include <mitkDataStorage.h>
#include <mitkRenderingManager.h>
#include <qgridlayout.h>

// mitk
#include <QmitkRenderWindow.h>
#include <mitkDataNode.h>
#include <mitkPlanarAngle.h>
#include <mitkPlanarFigureInteractor.h>
#include <mitkPlanarLine.h>
#include <usModuleRegistry.h>

SRStdMultiWidget::SRStdMultiWidget(QWidget *parent, Qt::WindowFlags f,
                                   const QString &name)
    : QmitkStdMultiWidget(parent, f, name),
      // planarFigureInteractor(mitk::PlanarFigureInteractor::New()),
      groupBoxGadget{
          new GroupBoxGadget(GroupBoxGadget::Orientation::AXIAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::SAGITTAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this),
          new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this)} {

  // this->planarFigureInteractor->LoadStateMachine(
  //     "PlanarFigureInteraction.xml",
  //     us::ModuleRegistry::GetModule("MitkPlanarFigure"));
  // this->planarFigureInteractor->SetEventConfig(
  //     "PlanarFigureConfig.xml",
  //     us::ModuleRegistry::GetModule("MitkPlanarFigure"));

  this->enableGroupBox(false);
}

void SRStdMultiWidget::InitializeMultiWidget() {

  this->SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());

  QmitkStdMultiWidget::InitializeMultiWidget();
  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  this->AddDisplayPlaneSubTree();
  // show image plane in viewer and data storage
  this->AddPlanesToDataStorage();
  // Disable the plane widget
  this->SetCrosshairVisibility(false);

  for (int i = 0; i < 4; ++i) {
    QmitkRenderWindow *renderWindow = this->GetRenderWindow(i);
    QGridLayout *pRenderWindow1Layout = new QGridLayout(renderWindow);
    pRenderWindow1Layout->addWidget(this->groupBoxGadget[i], 0, 1,
                                    Qt::AlignRight | Qt::AlignTop);
    renderWindow->setLayout(pRenderWindow1Layout);
  }

  this->ResetCrosshair();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SRStdMultiWidget::enableGroupBox(bool flag) {
  for (int i = 0; i < 4; ++i) {
    this->groupBoxGadget[i]->setVisible(flag);
  }
}

void SRStdMultiWidget::enableDisplay(bool flag) {
  if (this->scrollEnabled == flag) {
    return;
  }
  this->scrollEnabled = flag;
  if (this->scrollEnabled) {

    this->GetInteractionEventHandler()->SetEventConfig("DisplayConfigMITK.xml");

  } else {

    this->GetInteractionEventHandler()->SetEventConfig(
        "DisplayConfigMITKLimited.xml");
  }
}

void SRStdMultiWidget::enablePlanarLine(bool flag) {

  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode *imageNode = ds->GetNamedNode("image");
  mitk::DataNode::Pointer planarLineNode = ds->GetNamedNode("planar_line");
  if (planarLineNode) {
    ds->Remove(planarLineNode);
  }
  if (flag) {

    typedef itk::SimpleMemberCommand<SRStdMultiWidget> SimpleMemberCommand;
    SimpleMemberCommand::Pointer startInteractionCommand =
        SimpleMemberCommand::New();
    startInteractionCommand->SetCallbackFunction(
        this, &SRStdMultiWidget::disableDisplayPrivate);

    SimpleMemberCommand::Pointer endInteractionCommand =
        SimpleMemberCommand::New();
    endInteractionCommand->SetCallbackFunction(
        this, &SRStdMultiWidget::enableDisplayPrivate);

    planarLineNode = mitk::DataNode::New();
    planarLineNode->SetName("planar_line");
    planarLineNode->SetData(mitk::PlanarLine::New());
    planarLineNode->SetVisibility(true);
    planarLineNode->SetSelected(true);
    planarLineNode->GetData()->AddObserver(
        mitk::StartInteractionPlanarFigureEvent(), startInteractionCommand);
    planarLineNode->GetData()->AddObserver(
        mitk::EndInteractionPlanarFigureEvent(), endInteractionCommand);
    ds->Add(planarLineNode);

    mitk::PlanarFigureInteractor::Pointer planarFigureInteractor =
        mitk::PlanarFigureInteractor::New();
    planarFigureInteractor->LoadStateMachine(
        "PlanarFigureInteraction.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->SetEventConfig(
        "PlanarFigureConfig.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->EnableInteraction(flag);
    planarFigureInteractor->SetDataNode(flag ? planarLineNode : nullptr);
  }

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SRStdMultiWidget::enablePlanarAngle(bool flag) {
  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode *imageNode = ds->GetNamedNode("image");
  mitk::DataNode::Pointer planarAngleNode = ds->GetNamedNode("planar_angle");
  if (planarAngleNode) {
    ds->Remove(planarAngleNode);
  }
  if (flag) {

    typedef itk::SimpleMemberCommand<SRStdMultiWidget> SimpleMemberCommand;
    SimpleMemberCommand::Pointer startInteractionCommand =
        SimpleMemberCommand::New();
    startInteractionCommand->SetCallbackFunction(
        this, &SRStdMultiWidget::disableDisplayPrivate);

    SimpleMemberCommand::Pointer endInteractionCommand =
        SimpleMemberCommand::New();
    endInteractionCommand->SetCallbackFunction(
        this, &SRStdMultiWidget::enableDisplayPrivate);

    planarAngleNode = mitk::DataNode::New();
    planarAngleNode->SetName("planar_angle");
    planarAngleNode->SetData(mitk::PlanarAngle::New());
    planarAngleNode->SetVisibility(true);
    planarAngleNode->SetSelected(true);
    planarAngleNode->GetData()->AddObserver(
        mitk::StartInteractionPlanarFigureEvent(), startInteractionCommand);
    planarAngleNode->GetData()->AddObserver(
        mitk::EndInteractionPlanarFigureEvent(), endInteractionCommand);
    ds->Add(planarAngleNode);

    mitk::PlanarFigureInteractor::Pointer planarFigureInteractor =
        mitk::PlanarFigureInteractor::New();
    planarFigureInteractor->LoadStateMachine(
        "PlanarFigureInteraction.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->SetEventConfig(
        "PlanarFigureConfig.xml",
        us::ModuleRegistry::GetModule("MitkPlanarFigure"));
    planarFigureInteractor->EnableInteraction(flag);
    planarFigureInteractor->SetDataNode(flag ? planarAngleNode : nullptr);
  }

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}