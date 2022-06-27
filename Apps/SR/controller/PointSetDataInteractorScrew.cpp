/**
 * @file PointSetDataInteractorScrew.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "PointSetDataInteractorScrew.h"

// mitk
#include <mitkInteractionEvent.h>
#include <mitkLogMacros.h>

using namespace mitk;

void PointSetDataInteractorScrew::AddPoint(
    mitk::StateMachineAction* action, mitk::InteractionEvent* interactionEvent)
{
  Superclass::AddPoint(action, interactionEvent);
  unsigned int timeStep =
      interactionEvent->GetSender()->GetTimeStep(GetDataNode()->GetData());

  // disallow adding of new points if maximum number of points is reached
  if (m_MaxNumberOfPoints > 1 &&
      m_PointSet->GetSize(timeStep) >= m_MaxNumberOfPoints)
  {
    updateScrew();
  }
}

void PointSetDataInteractorScrew::FinishMove(StateMachineAction* action,
                                             InteractionEvent* event)
{
  Superclass::FinishMove(action, event);
  updateScrew();
}

void PointSetDataInteractorScrew::updateScrew()
{
  auto* pointSet = static_cast<mitk::PointSet*>(GetDataNode()->GetData());
  auto point0 = pointSet->GetPoint(0);
  auto point1 = pointSet->GetPoint(1);
  auto* screw = GetScrew()->GetData();
  auto screwGeometry = screw->GetGeometry();

  mitk::AffineTransform3D::OutputVectorType newOffset;
  newOffset.SetVnlVector(point0.GetVnlVector() + point1.GetVnlVector());
  newOffset *= 0.5;

  mitk::AffineTransform3D::OutputVectorType newAxis;
  newAxis.SetVnlVector(point0.GetVnlVector() - point1.GetVnlVector());

  auto direction = 0;
  auto oldAxis = screwGeometry->GetAxisVector(0);
  oldAxis = oldAxis.GetNorm() > screwGeometry->GetAxisVector(1).GetNorm()
                ? oldAxis
                : screwGeometry->GetAxisVector(1);
  oldAxis = oldAxis.GetNorm() > screwGeometry->GetAxisVector(2).GetNorm()
                ? oldAxis
                : screwGeometry->GetAxisVector(2);

  auto transform = screwGeometry->GetIndexToWorldTransform()->Clone();
  transform->SetOffset(newOffset);
  screwGeometry->SetIndexToWorldTransform(transform);
}