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

// itk
#include <vnl/vnl_cross.h>
#include <vnl/vnl_vector.h>

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
  if (pointSet->GetSize() < 2)
  {
    return;
  }
  auto point0 = pointSet->GetPoint(0);
  auto point1 = pointSet->GetPoint(1);
  auto* screw = GetScrew()->GetData();
  auto screwGeometry = screw->GetGeometry();
  screwGeometry->SetIdentity();

  mitk::AffineTransform3D::OutputVectorType newMajorAxis;
  newMajorAxis.SetVnlVector(point0.GetVnlVector() - point1.GetVnlVector());

  auto majorAxisDirection = 0;  ///< 0: x, 1: y, 2: z
  auto majorAxis = screwGeometry->GetAxisVector(0);
  majorAxisDirection =
      majorAxis.GetNorm() > screwGeometry->GetAxisVector(1).GetNorm()
          ? majorAxisDirection
          : 1;
  majorAxis = majorAxis.GetNorm() > screwGeometry->GetAxisVector(1).GetNorm()
                  ? majorAxis
                  : screwGeometry->GetAxisVector(1);
  majorAxisDirection =
      majorAxis.GetNorm() > screwGeometry->GetAxisVector(2).GetNorm()
          ? majorAxisDirection
          : 2;
  majorAxis = majorAxis.GetNorm() > screwGeometry->GetAxisVector(2).GetNorm()
                  ? majorAxis
                  : screwGeometry->GetAxisVector(2);

  auto rotateAngle =
      angle(majorAxis.GetVnlVector(), newMajorAxis.GetVnlVector());
  auto rotateAxis = mitk::Vector3D(
      vnl_cross_3d(majorAxis.GetVnlVector(), newMajorAxis.GetVnlVector())
          .normalize());

  double minorAxisesLengthsMean = 0;
  for (auto i = 0; i < 3; ++i)
  {
    minorAxisesLengthsMean +=
        (i == majorAxisDirection ? 0
                                 : screwGeometry->GetAxisVector(i).GetNorm());
  }
  minorAxisesLengthsMean *= 0.5;

  auto scaleOfMinorAxisesDirection =
      GetScrewDiameter() == 0 ? 1
                              : GetScrewDiameter() * 2 / minorAxisesLengthsMean;
  auto scaleOfMajorAxisDirection = newMajorAxis.GetNorm() / majorAxis.GetNorm();

  mitk::Vector3D scale;
  scale.Fill(scaleOfMinorAxisesDirection);
  scale[majorAxisDirection] = scaleOfMajorAxisDirection;

  mitk::AffineTransform3D::OutputVectorType offset;
  offset.SetVnlVector((point0.GetVnlVector() + point1.GetVnlVector()) * 0.5);

  auto transform = screwGeometry->GetIndexToWorldTransform();
  transform->Scale(scale, false);
  transform->Rotate3D(rotateAxis, rotateAngle, false);
  transform->Translate(offset, false);
  screwGeometry->SetIndexToWorldTransform(transform);
}