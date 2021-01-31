#include "mitkReamerSource.h"

// mitk
#include <mitkSurface.h>

// vtk
#include <vtkSphereSource.h>
#include <vtkLineSOurce.h>
#include <vtkTubeFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkFillHolesFilter.h>
#include <vtkNew.h>

namespace mitk
{
void ReamerSource::GenerateData()
{
  mitk::Surface * output = this->GetOutput();
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->SetCenter(0, 0, this->m_Radius);
  sphereSource->SetThetaResolution(this->m_ThetaResolution);
  sphereSource->SetPhiResolution(this->m_PhiResolution);
  sphereSource->SetStartPhi(90);
  sphereSource->SetEndPhi(180);
  sphereSource->SetRadius(this->m_Radius);
  sphereSource->Update();

  vtkNew<vtkFillHolesFilter> fillHole;
  fillHole->SetInputData(sphereSource->GetOutput());
  fillHole->SetHoleSize(this->m_Radius * this->m_Radius * vtkMath::Pi());
  fillHole->Update();

  vtkNew<vtkLineSource> lineSource;
  lineSource->SetPoint1(0, 0, this->m_Radius);
  lineSource->SetPoint2(0, 0, this->m_Radius + this->m_Length);
  lineSource->Update();

  vtkNew<vtkTubeFilter> tubeFilter;
  tubeFilter->SetInputData(lineSource->GetOutput());
  tubeFilter->SetRadius(this->m_Radius);
  tubeFilter->SetNumberOfSides(this->m_ThetaResolution);
  tubeFilter->SetCapping(true);
  tubeFilter->Update();

  vtkNew<vtkAppendPolyData> appendPolyData;
  appendPolyData->AddInputData(fillHole->GetOutput());
  appendPolyData->AddInputData(tubeFilter->GetOutput());
  appendPolyData->Update();

  output->SetVtkPolyData(appendPolyData->GetOutput());

}
}