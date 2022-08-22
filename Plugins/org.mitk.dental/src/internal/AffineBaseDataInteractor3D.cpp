/**
 * @file AffineBaseDataInteractor3D.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "AffineBaseDataInteractor3D.h"

#include "DentalStdMultiWidget.h"

// mitk
#include <mitkBaseData.h>
#include <mitkDataNode.h>
#include <mitkLine.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSliceNavigationController.h>

void AffineBaseDataInteractor3D::SetDataNode(mitk::DataNode* dataNode)
{
  mitk::AffineBaseDataInteractor3D::SetDataNode(dataNode);
  if (dataNode != nullptr)
  {
    mCommand->SetCallbackFunction(
        this, &AffineBaseDataInteractor3D::selectSliceByPoint);
    dataNode->AddObserver(itk::ModifiedEvent(), mCommand);

    auto renderWindows =
        mitk::RenderingManager::GetInstance()->GetAllRegisteredRenderWindows();
    if (renderWindows.size() >= 3)
    {
      const auto* plane1 = mitk::BaseRenderer::GetInstance(renderWindows[0])
                               ->GetSliceNavigationController()
                               ->GetCurrentPlaneGeometry();
      const auto* plane2 = mitk::BaseRenderer::GetInstance(renderWindows[1])
                               ->GetSliceNavigationController()
                               ->GetCurrentPlaneGeometry();
      const auto* plane3 = mitk::BaseRenderer::GetInstance(renderWindows[2])
                               ->GetSliceNavigationController()
                               ->GetCurrentPlaneGeometry();

      mitk::Line3D line;
      if ((plane1 != nullptr) && (plane2 != nullptr) &&
          (plane1->IntersectionLine(plane2, line)))
      {
        mitk::Point3D point;
        if ((plane3 != nullptr) && (plane3->IntersectionPoint(line, point)))
        {
          mitk::Vector3D translate(point.GetVnlVector());
          dataNode->GetData()->GetGeometry()->Translate(translate);
        }
      }
    }
  }
}

void AffineBaseDataInteractor3D::selectSliceByPoint()
{
  auto* dataNode = GetDataNode();
  auto center = dataNode->GetData()->GetGeometry()->GetCenter();

  auto renderWindows =
      mitk::RenderingManager::GetInstance()->GetAllRegisteredRenderWindows();
  for (auto renderWindow : renderWindows)
  {
    mitk::BaseRenderer::GetInstance(renderWindow)
        ->GetSliceNavigationController()
        ->SelectSliceByPoint(center);
  }
}
