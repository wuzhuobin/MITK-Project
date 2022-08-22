/**
 * @file DentalStdMultiWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DentalStdMultiWidget.h"

// mitk
#include <QmitkRenderWindow.h>
#include <mitkRenderingManager.h>

static DentalStdMultiWidget* gInstance = nullptr;
DentalStdMultiWidget* DentalStdMultiWidget::getInstance()
{
  return gInstance;
}

DentalStdMultiWidget::DentalStdMultiWidget(QWidget* parent,
                                           Qt::WindowFlags f,
                                           const QString& name) :
    QmitkStdMultiWidget(parent, f, name)
{
  gInstance = this;
}

void DentalStdMultiWidget::InitializeMultiWidget()
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
  updateViewMode();
}

void DentalStdMultiWidget::updateViewMode()
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