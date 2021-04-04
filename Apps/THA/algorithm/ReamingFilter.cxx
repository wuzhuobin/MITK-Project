#include "ReamingFilter.h"
// vtk
#include <vtkExtractVOI.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkDiscreteFlyingEdges3D.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkImageMathematics.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
static constexpr char IN = 1;
static constexpr char COINCIDE = 2;
static constexpr char OUT = 0;
static constexpr unsigned int smoothingIterations = 15;
static constexpr double passBand = 0.001;
static constexpr double featureAngle = 120.0;
vtkStandardNewMacro(ReamingFilter);
// vtkCxxSetObjectMacro(ReamingFilter, Reamer, vtkPolyData);

void ReamingFilter::PrintSelf(ostream &os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

// void ReamingFilter::SetReamer(vtkPolyData *data)
// {
//   this->SetInputData(1, data);
// }

// vtkPolyData * ReamingFilter::GetReamer()
// {
//   return this->GetPolyDataInput(1);
// }

ReamingFilter::ReamingFilter():
  Extent{0,0,0,0,0,0},
  KeepTrace(false),
  FirstUpdate(true),
  Scale(1.0),
  extractVOI(Ptr<vtkExtractVOI>::New()),
  polyDataToImageStencil(Ptr<vtkPolyDataToImageStencil>::New()),
  imageStencil1(Ptr<vtkImageStencil>::New()),
  polyDataToImageStencil2(Ptr<vtkPolyDataToImageStencil>::New()),
  imageStencil2(Ptr<vtkImageStencil>::New()),
  polyDataToImageStencil3(Ptr<vtkPolyDataToImageStencil>::New()),
  imageStencil3(Ptr<vtkImageStencil>::New()),
  flyingEdges3D(Ptr<vtkDiscreteFlyingEdges3D>::New()),
  windowedSincPolyData(Ptr<vtkWindowedSincPolyDataFilter>::New()),
  imageMathematics(Ptr<vtkImageMathematics>::New()),
  imageMathematics2(Ptr<vtkImageMathematics>::New()),
  imageMathematics3(Ptr<vtkImageMathematics>::New()),
  transformPolyDataFilter(Ptr<vtkTransformPolyDataFilter>::New())
{
  this->SetNumberOfInputPorts(3);
}

int ReamingFilter::RequestData(vtkInformation *info, vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo0 = inputVector[0]->GetInformationObject(0);
  vtkInformation *inInfo1 = inputVector[1]->GetInformationObject(0);
  vtkInformation *inInfo2 = inputVector[2]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkImageData *input0 = vtkImageData::SafeDownCast(
    inInfo0->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *input1 = vtkPolyData::SafeDownCast(
    inInfo1->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *input2 = vtkPolyData::SafeDownCast(
    inInfo2->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  this->extractVOI->SetInputData(input0);
  this->extractVOI->SetVOI(this->Extent);
  this->polyDataToImageStencil->SetInputData(input1);
  this->polyDataToImageStencil->SetOutputOrigin(input0->GetOrigin());
  this->polyDataToImageStencil->SetOutputSpacing(input0->GetSpacing());
  this->polyDataToImageStencil->SetOutputWholeExtent(this->Extent);
  if (!this->FirstUpdate && this->KeepTrace)
  {
    vtkSmartPointer<vtkImageData> pImage = 
      vtkSmartPointer<vtkImageData>::New();
    pImage->ShallowCopy(this->imageStencil1->GetOutput());
    this->imageStencil1->SetInputData(pImage);
  }
  else
  {
    this->imageStencil1->SetInputConnection(this->extractVOI->GetOutputPort());
    this->FirstUpdate = false;
  }
  this->imageStencil1->SetStencilConnection(this->polyDataToImageStencil->GetOutputPort());
  this->imageStencil1->SetReverseStencil(true);
  this->imageStencil1->SetBackgroundValue(OUT);

  this->polyDataToImageStencil2->SetInputData(input2);
  this->polyDataToImageStencil2->SetOutputOrigin(input0->GetOrigin());
  this->polyDataToImageStencil2->SetOutputSpacing(input0->GetSpacing());
  this->polyDataToImageStencil2->SetOutputWholeExtent(this->Extent);
  this->imageStencil2->SetInputConnection(this->imageStencil1->GetOutputPort());
  this->imageStencil2->SetStencilConnection(this->polyDataToImageStencil2->GetOutputPort());
  this->imageStencil2->SetReverseStencil(false);
  this->imageStencil2->SetBackgroundValue(OUT);

  double *center = input2->GetCenter();
  Ptr<vtkTransform> transform =
    Ptr<vtkTransform>::New();
  transform->PostMultiply();
  transform->Translate(-center[0], -center[1], -center[2]);
  transform->Scale(this->Scale, this->Scale, this->Scale);
  transform->Translate(center);

  this->transformPolyDataFilter->SetInputData(input2);
  this->transformPolyDataFilter->SetTransform(transform);


  this->polyDataToImageStencil3->SetInputConnection(this->transformPolyDataFilter->GetOutputPort());
  this->polyDataToImageStencil3->SetOutputOrigin(input0->GetOrigin());
  this->polyDataToImageStencil3->SetOutputSpacing(input0->GetSpacing());
  this->polyDataToImageStencil3->SetOutputWholeExtent(this->Extent);
  this->imageStencil3->SetInputConnection(this->imageStencil1->GetOutputPort());
  this->imageStencil3->SetStencilConnection(this->polyDataToImageStencil3->GetOutputPort());
  this->imageStencil3->SetReverseStencil(false);
  this->imageStencil3->SetBackgroundValue(OUT);

  this->imageMathematics->SetInputConnection(0, this->imageStencil3->GetOutputPort());
  this->imageMathematics->SetConstantK(2);
  this->imageMathematics->SetOperationToMultiplyByK();

  this->imageMathematics2->SetInputConnection(0, this->imageMathematics->GetOutputPort());
  this->imageMathematics2->SetInputConnection(1, this->imageStencil2->GetOutputPort());
  this->imageMathematics2->SetOperationToMax();

  this->imageMathematics3->SetInputConnection(0, this->imageStencil1->GetOutputPort());
  this->imageMathematics3->SetInputConnection(1, this->imageMathematics2->GetOutputPort());
  this->imageMathematics3->SetOperationToAdd();




  this->flyingEdges3D->SetInputConnection(this->imageMathematics3->GetOutputPort());
  // this->flyingEdges3D->SetValue(0, 0);
  this->flyingEdges3D->SetValue(0, 1);
  this->flyingEdges3D->SetValue(1, 2);
  this->flyingEdges3D->SetValue(2, 3);
  this->flyingEdges3D->SetComputeGradients(false);
  this->flyingEdges3D->SetComputeNormals(false);
  this->flyingEdges3D->SetComputeScalars(true);
  this->windowedSincPolyData->SetInputConnection(this->flyingEdges3D->GetOutputPort());
  this->windowedSincPolyData->SetNumberOfIterations(smoothingIterations);
  this->windowedSincPolyData->SetFeatureEdgeSmoothing(false);
  this->windowedSincPolyData->SetBoundarySmoothing(false);
  this->windowedSincPolyData->SetFeatureAngle(featureAngle);
  this->windowedSincPolyData->SetPassBand(passBand);
  this->windowedSincPolyData->SetNonManifoldSmoothing(true);
  this->windowedSincPolyData->SetNormalizeCoordinates(true);
  this->windowedSincPolyData->Update();
  output->ShallowCopy(this->windowedSincPolyData->GetOutput());

  return 1;
}

int ReamingFilter::FillInputPortInformation(int port, vtkInformation *info)
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
