#include "ReamingFilter2.h"

// vtk
#include <vtkAbstractTransform.h>
#include <vtkDiscreteFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkImageDilateErode3D.h>
#include <vtkImageMathematics.h>
#include <vtkImageStencil.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>

vtkStandardNewMacro(ReamingFilter2);
vtkCxxSetObjectMacro(ReamingFilter2, ImageTransform, vtkAbstractTransform);

void ReamingFilter2::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size: " << this->Size << "\n";
  os << indent << "UseSmooth: " << this->UseSmooth << "\n";
  os << indent << "Reset: " << this->Reset << "\n";
  os << indent << "ImageTransform: ";
  ImageTransform->PrintSelf(os, indent.GetNextIndent());
}

ReamingFilter2::ReamingFilter2() :
    IntermediateImage(Ptr<vtkImageData>::New()),
    TransformPolyDataFilter1(Ptr<vtkTransformPolyDataFilter>::New()),
    TransformPolyDataFilter2(Ptr<vtkTransformPolyDataFilter>::New()),
    PolyDataToImageStencil1(Ptr<vtkPolyDataToImageStencil>::New()),
    PolyDataToImageStencil2(Ptr<vtkPolyDataToImageStencil>::New()),
    ImageStencil1(Ptr<vtkImageStencil>::New()),
    ImageStencil2(Ptr<vtkImageStencil>::New()),
    Erode(Ptr<vtkImageDilateErode3D>::New()),
    ImageSubstractTrajectory(Ptr<vtkImageMathematics>::New()),
    ImageMultiplyBy2(Ptr<vtkImageMathematics>::New()),
    ImageAddTrajectory(Ptr<vtkImageMathematics>::New()),
    ImageAddErode(Ptr<vtkImageMathematics>::New()),
    DiscreteFlyingEdges(Ptr<vtkDiscreteFlyingEdges3D>::New()),
    TransformPolyDataFilter3(Ptr<vtkTransformPolyDataFilter>::New()),
    WindowedSincPolyDataFilter(Ptr<vtkWindowedSincPolyDataFilter>::New())
{
  SetNumberOfInputPorts(3);
  SetNumberOfOutputPorts(1);
}

void ReamingFilter2::SetPelvis(vtkImageData* pelvis)
{
  SetInputData(0, pelvis);
}

vtkImageData* ReamingFilter2::GetPelvis()
{
  return vtkImageData::SafeDownCast(GetInput(0));
}

void ReamingFilter2::SetReamer(vtkPolyData* reamer)
{
  SetInputData(1, reamer);
}

vtkPolyData* ReamingFilter2::GetReamer()
{
  return vtkPolyData::SafeDownCast(GetInput(1));
}

void ReamingFilter2::SetReamerTrajectory(vtkPolyData* trajectory)
{
  SetInputData(2, trajectory);
}

vtkPolyData* ReamingFilter2::GetReamerTrajectory()
{
  return vtkPolyData::SafeDownCast(GetInput(2));
}

int ReamingFilter2::RequestData(vtkInformation* info,
                                vtkInformationVector** input,
                                vtkInformationVector* output)
{
  auto* pelvis = vtkImageData::GetData(input[0]);
  auto* reamer = vtkPolyData::GetData(input[1]);
  auto* trajectory = vtkPolyData::GetData(input[2]);
  auto* output0 = vtkPolyData::GetData(output);

  if (GetReset())
  {
    TransformPolyDataFilter2->SetInputData(trajectory);
    TransformPolyDataFilter2->SetTransform(ImageTransform->GetInverse());
    TransformPolyDataFilter2->Update();

    PolyDataToImageStencil2->SetInputConnection(
        TransformPolyDataFilter2->GetOutputPort());
    PolyDataToImageStencil2->SetOutputOrigin(pelvis->GetOrigin());
    PolyDataToImageStencil2->SetOutputSpacing(pelvis->GetSpacing());
    PolyDataToImageStencil2->SetOutputWholeExtent(pelvis->GetExtent());
    PolyDataToImageStencil2->Update();

    ImageStencil2->SetInputData(pelvis);
    ImageStencil2->SetStencilConnection(
        PolyDataToImageStencil2->GetOutputPort());
    ImageStencil2->SetBackgroundValue(0);
    ImageStencil2->SetReverseStencil(false);
    ImageStencil2->Update();

    // ImageSubstractTrajectory->SetInputConnection(0, ->GetOutputPort());
    ImageSubstractTrajectory->SetInputData(0, pelvis);
    ImageSubstractTrajectory->SetInputConnection(
        1, ImageStencil2->GetOutputPort());
    ImageSubstractTrajectory->SetOperationToSubtract();
    ImageSubstractTrajectory->Update();

    ImageMultiplyBy2->SetInputConnection(
        0, ImageSubstractTrajectory->GetOutputPort());
    ImageMultiplyBy2->SetConstantK(2);
    ImageMultiplyBy2->SetOperationToMultiplyByK();
    ImageMultiplyBy2->Update();

    Erode->SetInputConnection(ImageSubstractTrajectory->GetOutputPort());
    Erode->SetKernelSize(Size, Size, Size);
    Erode->SetErodeValue(1);
    Erode->SetDilateValue(0);
    Erode->Update();

    ImageAddTrajectory->SetInputConnection(0, Erode->GetOutputPort());
    ImageAddTrajectory->SetInputConnection(1, ImageStencil2->GetOutputPort());
    ImageAddTrajectory->SetOperationToAdd();
    ImageAddTrajectory->Update();

    ImageAddErode->SetInputConnection(0, ImageAddTrajectory->GetOutputPort());
    ImageAddErode->SetInputConnection(1, ImageMultiplyBy2->GetOutputPort());
    ImageAddErode->SetOperationToAdd();
    ImageAddErode->Update();
    IntermediateImage->ShallowCopy(ImageAddErode->GetOutput());
    SetReset(false);
  }
  TransformPolyDataFilter1->SetInputData(reamer);
  TransformPolyDataFilter1->SetTransform(ImageTransform->GetInverse());
  TransformPolyDataFilter1->Update();

  PolyDataToImageStencil1->SetInputConnection(
      TransformPolyDataFilter1->GetOutputPort());
  PolyDataToImageStencil1->SetOutputOrigin(pelvis->GetOrigin());
  PolyDataToImageStencil1->SetOutputSpacing(pelvis->GetSpacing());
  PolyDataToImageStencil1->SetOutputWholeExtent(pelvis->GetExtent());
  PolyDataToImageStencil1->Update();

  // ImageStencil1->SetInputConnection(ImageAddErode->GetOutputPort());
  ImageStencil1->SetInputData(IntermediateImage);
  ImageStencil1->SetStencilConnection(PolyDataToImageStencil1->GetOutputPort());
  ImageStencil1->SetBackgroundValue(0);
  ImageStencil1->SetReverseStencil(true);
  ImageStencil1->Update();
  IntermediateImage->ShallowCopy(ImageStencil1->GetOutput());

  DiscreteFlyingEdges->SetInputConnection(ImageStencil1->GetOutputPort());
  DiscreteFlyingEdges->GenerateValues(3, 1, 3);
  DiscreteFlyingEdges->Update();

  if (UseSmooth)
  {
    WindowedSincPolyDataFilter->SetInputConnection(
        DiscreteFlyingEdges->GetOutputPort());
    WindowedSincPolyDataFilter->SetNumberOfIterations(15);
    WindowedSincPolyDataFilter->SetBoundarySmoothing(false);
    WindowedSincPolyDataFilter->SetFeatureEdgeSmoothing(false);
    WindowedSincPolyDataFilter->SetFeatureAngle(120);
    WindowedSincPolyDataFilter->SetPassBand(0.001);
    WindowedSincPolyDataFilter->SetNonManifoldSmoothing(true);
    WindowedSincPolyDataFilter->SetNormalizeCoordinates(true);
    WindowedSincPolyDataFilter->Update();

    TransformPolyDataFilter3->SetInputConnection(
        DiscreteFlyingEdges->GetOutputPort());
  }
  else
  {
    TransformPolyDataFilter3->SetInputConnection(
        DiscreteFlyingEdges->GetOutputPort());
  }

  TransformPolyDataFilter3->SetTransform(ImageTransform);
  TransformPolyDataFilter3->Update();

  output0->ShallowCopy(TransformPolyDataFilter3->GetOutput());

  return 1;
}

int ReamingFilter2::FillInputPortInformation(int port, vtkInformation* info)
{
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
    return 1;
  }
  else if (port == 1)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
  }
  else if (port == 2)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    return 1;
  }
  return 0;
}
