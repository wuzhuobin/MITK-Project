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
// #include <mitkPlanarAngle.h>
// #include <mitkPlanarFigureInteractor.h>
// #include <mitkPlanarLine.h>
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
