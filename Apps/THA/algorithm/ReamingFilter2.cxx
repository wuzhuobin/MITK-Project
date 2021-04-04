#include "ReamingFilter2.h"

// vtk
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>

vtkStandardNewMacro(ReamingFilter2);

void ReamingFilter2::PrintSelf(ostream &os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

ReamingFilter2::ReamingFilter2()
{
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(1);
}

int ReamingFilter2::RequestData(vtkInformation *info, vtkInformationVector **input,
                                vtkInformationVector *output)
{

  vtkPolyData *input0 = vtkPolyData::GetData(input[0]);
  vtkPolyData *input1 = vtkPolyData::GetData(input[1]);
  vtkPolyData *output0 = vtkPolyData::GetData(output);


  vtkSmartPointer<vtkSelectEnclosedPoints> selectedEnclosedPoints =
    vtkSmartPointer<vtkSelectEnclosedPoints>::New();
  selectedEnclosedPoints->SetInputData(input0);
  selectedEnclosedPoints->SetSurfaceData(input1);
  selectedEnclosedPoints->Update();

  vtkDataArray *selectedPoints =
    selectedEnclosedPoints->GetOutput()->GetPointData()->GetArray("SelectedPoints");

  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("colors");
  
  for (vtkIdType i = 0; i < selectedPoints->GetNumberOfTuples(); ++i) {

    selectedPoints->GetComponent(i, 0) == 1 ?
      colors->InsertNextTuple3(0, 255, 0) : colors->InsertNextTuple3(255, 255, 255);
  }

  selectedEnclosedPoints->GetOutput()->GetPointData()->SetScalars(colors);
  selectedEnclosedPoints->GetOutput()->GetPointData()->RemoveArray("SelectedPoints");

  output0->ShallowCopy(selectedEnclosedPoints->GetOutput());

  return 1;
}