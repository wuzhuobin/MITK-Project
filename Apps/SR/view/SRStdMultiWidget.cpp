/**
 * @file SRStdMultiWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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

static SRStdMultiWidget* gInstance = nullptr;
SRStdMultiWidget* SRStdMultiWidget::getInstance()
{
  return gInstance;
}

SRStdMultiWidget::SRStdMultiWidget(QWidget* parent,
                                   Qt::WindowFlags f,
                                   const QString& name) :
    QmitkStdMultiWidget(parent, f, name),
    // planarFigureInteractor(mitk::PlanarFigureInteractor::New()),
    mGroupBoxGadget{
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
  gInstance = this;
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
  // SetCrosshairVisibility(false);
  SetWidgetPlanesVisibility(true,
                            mitk::BaseRenderer::GetInstance(
                                GetRenderWindow1()->GetVtkRenderWindow()));
  SetWidgetPlanesVisibility(true,
                            mitk::BaseRenderer::GetInstance(
                                GetRenderWindow2()->GetVtkRenderWindow()));
  SetWidgetPlanesVisibility(true,
                            mitk::BaseRenderer::GetInstance(
                                GetRenderWindow3()->GetVtkRenderWindow()));
  SetWidgetPlanesVisibility(false,
                            mitk::BaseRenderer::GetInstance(
                                GetRenderWindow4()->GetVtkRenderWindow()));

  for (int i = 0; i < 4; ++i)
  {
    auto* renderWindow = GetRenderWindow(i);
    auto* pRenderWindow1Layout = new QGridLayout(renderWindow);
    pRenderWindow1Layout->addWidget(
        mGroupBoxGadget[i], 0, 1, Qt::AlignRight | Qt::AlignTop);
    renderWindow->setLayout(pRenderWindow1Layout);
  }

  updateViewMode();
}

void SRStdMultiWidget::setTransformTarget(const QString& transformTarget)
{
  for (auto i = 0; i < 4; ++i)
  {
    mGroupBoxGadget[i]->setTarget(transformTarget);
  }
}

void SRStdMultiWidget::enableGroupBox(bool flag)
{
  for (auto i = 0; i < 4; ++i)
  {
    mGroupBoxGadget[i]->setVisible(flag);
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
    auto isHelperObject = false;
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