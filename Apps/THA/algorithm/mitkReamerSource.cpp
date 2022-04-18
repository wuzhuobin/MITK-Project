#include "mitkReamerSource.h"

// mitk
#include <mitkSurface.h>

// vtk
#include <vtkAppendPolyData.h>
#include <vtkFillHolesFilter.h>
#include <vtkLineSource.h>
#include <vtkNew.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>

namespace mitk
{
void ReamerSource::GenerateData()
{
  mitk::Surface* output = GetOutput();
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->SetCenter(0, 0, m_SphereRadius);
  sphereSource->SetThetaResolution(m_ThetaResolution);
  sphereSource->SetPhiResolution(m_PhiResolution);
  sphereSource->SetStartPhi(90);
  sphereSource->SetEndPhi(180);
  sphereSource->SetRadius(m_SphereRadius);
  sphereSource->Update();

  vtkNew<vtkFillHolesFilter> fillHole;
  fillHole->SetInputData(sphereSource->GetOutput());
  fillHole->SetHoleSize(m_SphereRadius * m_SphereRadius * vtkMath::Pi());
  fillHole->Update();

  vtkNew<vtkAppendPolyData> appendPolyData;
  appendPolyData->AddInputData(fillHole->GetOutput());
  if (m_HaveTube)
  {
    vtkNew<vtkLineSource> lineSource;
    lineSource->SetPoint1(0, 0, m_SphereRadius);
    lineSource->SetPoint2(0, 0, m_SphereRadius + m_Length);
    lineSource->Update();

    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputData(lineSource->GetOutput());
    tubeFilter->SetRadius(m_TubeRadius);
    tubeFilter->SetNumberOfSides(m_ThetaResolution);
    tubeFilter->SetCapping(true);
    tubeFilter->Update();

    appendPolyData->AddInputData(tubeFilter->GetOutput());
  }

  appendPolyData->Update();
  output->SetVtkPolyData(appendPolyData->GetOutput());
}
}  // namespace mitk