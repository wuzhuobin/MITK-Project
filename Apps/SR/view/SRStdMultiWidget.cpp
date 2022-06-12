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

SRStdMultiWidget::SRStdMultiWidget(QWidget* parent,
                                   Qt::WindowFlags f,
                                   const QString& name) :
    QmitkStdMultiWidget(parent, f, name),
    // planarFigureInteractor(mitk::PlanarFigureInteractor::New()),
    groupBoxGadget{
        new GroupBoxGadget(GroupBoxGadget::Orientation::AXIAL, this),
        new GroupBoxGadget(GroupBoxGadget::Orientation::SAGITTAL, this),
        new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this),
        new GroupBoxGadget(GroupBoxGadget::Orientation::CORONAL, this)}
{
  // planarFigureInteractor->LoadStateMachine(
  //     "PlanarFigureInteraction.xml",
  //     us::ModuleRegistry::GetModule("MitkPlanarFigure"));
  // planarFigureInteractor->SetEventConfig(
  //     "PlanarFigureConfig.xml",
  //     us::ModuleRegistry::GetModule("MitkPlanarFigure"));

  enableGroupBox(false);
}

void SRStdMultiWidget::InitializeMultiWidget()
{
  SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());

  QmitkStdMultiWidget::InitializeMultiWidget();
  // Add the displayed views to the DataStorage to see their positions in 2D and
  // 3D
  AddDisplayPlaneSubTree();
  // show image plane in viewer and data storage
  AddPlanesToDataStorage();
  // Disable the plane widget
  SetCrosshairVisibility(false);

  for (int i = 0; i < 4; ++i)
  {
    QmitkRenderWindow* renderWindow = GetRenderWindow(i);
    QGridLayout* pRenderWindow1Layout = new QGridLayout(renderWindow);
    pRenderWindow1Layout->addWidget(
        groupBoxGadget[i], 0, 1, Qt::AlignRight | Qt::AlignTop);
    renderWindow->setLayout(pRenderWindow1Layout);
  }

  updateViewMode();
}

void SRStdMultiWidget::enableGroupBox(bool flag)
{
  for (int i = 0; i < 4; ++i)
  {
    groupBoxGadget[i]->setVisible(flag);
  }
}

void SRStdMultiWidget::enableDisplay(bool flag)
{
  if (scrollEnabled == flag)
  {
    return;
  }
  scrollEnabled = flag;
  if (scrollEnabled)
  {
    GetInteractionEventHandler()->SetEventConfig("DisplayConfigMITK.xml");
  }
  else
  {
    GetInteractionEventHandler()->SetEventConfig(
        "DisplayConfigMITKLimited.xml");
  }
}

void SRStdMultiWidget::updateViewMode()
{
  auto all = GetDataStorage()->GetAll();
  for (const auto& one : *all)
  {
    bool isHelperObject = false;
    if (!one->GetBoolProperty("helper object", isHelperObject) ||
        !isHelperObject)
    {
      one->SetVisibility(false);
    }
  }
  auto* imageNode = GetDataStorage()->GetNamedNode("image");
  imageNode->SetVisibility(true);
  ResetCrosshair();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}