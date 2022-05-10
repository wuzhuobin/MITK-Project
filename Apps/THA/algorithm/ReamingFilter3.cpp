#include "ReamingFilter3.hpp"

// vtk
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkDecimatePro.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTriangleFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>

vtkStandardNewMacro(ReamingFilter3);

void ReamingFilter3::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size: " << this->Size << "\n";
  os << indent << "UseSmooth: " << this->UseSmooth << "\n";
  os << indent << "Reset: " << this->Reset << "\n";
}

ReamingFilter3::ReamingFilter3() :
    IntermediatePolyData(Ptr<vtkPolyData>::New()),
    BooleanOperationPolyDataFilter(
        Ptr<vtkBooleanOperationPolyDataFilter>::New()),
    TriangleFilter1(Ptr<vtkTriangleFilter>::New()),
    CleanPolyData1(Ptr<vtkCleanPolyData>::New()),
    DecimatePro(Ptr<vtkDecimatePro>::New()),
    TriangleFilter2(Ptr<vtkTriangleFilter>::New()),
    CleanPolyData2(Ptr<vtkCleanPolyData>::New())

{
  SetNumberOfInputPorts(3);
  SetNumberOfOutputPorts(1);
}

void ReamingFilter3::SetReamer(vtkPolyData* reamer)
{
  SetInputData(1, reamer);
}

vtkPolyData* ReamingFilter3::GetReamer()
{
  return vtkPolyData::SafeDownCast(GetInput(1));
}

void ReamingFilter3::SetReamerTrajectory(vtkPolyData* trajectory)
{
  SetInputData(2, trajectory);
}

vtkPolyData* ReamingFilter3::GetReamerTrajectory()
{
  return vtkPolyData::SafeDownCast(GetInput(2));
}

int ReamingFilter3::RequestData(vtkInformation* info,
                                vtkInformationVector** input,
                                vtkInformationVector* output)
{
  auto* input0 = vtkPolyData::GetData(input[0]);

  auto* reamer = vtkPolyData::GetData(input[1]);
  // auto* trajectory = vtkPolyData::GetData(input[2]);
  auto* output0 = vtkPolyData::GetData(output);

  if (GetReset())
  {
    TriangleFilter1->SetInputData(input0);
    TriangleFilter1->Update();

    if (GetDecimate())
    {
      DecimatePro->SetInputConnection(TriangleFilter1->GetOutputPort());
      DecimatePro->SetTargetReduction(0.9);
      DecimatePro->Update();
      CleanPolyData1->SetInputConnection(DecimatePro->GetOutputPort());
    }
    else
    {
      CleanPolyData1->SetInputConnection(TriangleFilter1->GetOutputPort());
    }
    CleanPolyData1->Update();
    BooleanOperationPolyDataFilter->SetInputConnection(
        0, CleanPolyData1->GetOutputPort());

    SetReset(false);
  }
  else
  {
    BooleanOperationPolyDataFilter->SetInputData(0, IntermediatePolyData);
  }
  TriangleFilter2->SetInputData(reamer);
  TriangleFilter2->Update();

  CleanPolyData2->SetInputConnection(TriangleFilter2->GetOutputPort());
  CleanPolyData2->Update();
  BooleanOperationPolyDataFilter->SetInputConnection(
      1, CleanPolyData2->GetOutputPort());

  BooleanOperationPolyDataFilter->SetOperation(
      vtkBooleanOperationPolyDataFilter::VTK_DIFFERENCE);
  BooleanOperationPolyDataFilter->Update();

  output0->ShallowCopy(BooleanOperationPolyDataFilter->GetOutput());
  IntermediatePolyData->DeepCopy(output0);

  return 1;
}

int ReamingFilter3::FillInputPortInformation(int port, vtkInformation* info)
{
  if (port == 2)
  {
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    return 1;
  }
  return Superclass::FillInputPortInformation(port, info);
}