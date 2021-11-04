#include "mitkReamingFilter.h"

// mitk
#include <mitkSurface.h>
#include <mitkProgressBar.h>
#include <mitkMorphologicalOperations.h>

// vtk
#include <vtkImageData.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkImageStencil.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageMathematics.h>

#include <vtkDecimatePro.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
//#include <vtkLinearTransform.h>
//#include <vtkMath.h>
//#include <vtkMatrix4x4.h>
#include <vtkQuadricDecimation.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
//#include <vtkSmartPointer.h>

const mitk::Surface * mitk::ReamingFilter::GetInput(unsigned int i)
{
  if (i + 1 >= this->GetNumberOfInputs()) {
    return nullptr;
  }
  const mitk::Surface * surface = static_cast<mitk::Surface*>(this->itk::ProcessObject::GetInput(i + 1));
  return surface;
}

void mitk::ReamingFilter::SetInput(unsigned int i, const mitk::Surface * input)
{
  this->SetNthInput(i + 1, const_cast<mitk::Surface*>(input));
}

void mitk::ReamingFilter::GenerateData()
{
  mitk::Surface *surface = this->GetOutput();

  auto *image = const_cast<mitk::Image*>(this->GetInput());
  if (image == nullptr || !image->IsInitialized())
    mitkThrow() << "No input image set, please set an valid input image!";

  auto * reamer = (this->GetInput(0));
  if (reamer == nullptr || !reamer->IsInitialized()) {
    mitkThrow() << "No input reamer set, please set an valid input reamer!";
  }

  auto * shell = (this->GetInput(1));
  if (shell == nullptr || !shell->IsInitialized()) {
    mitkThrow() << "No input shell set, please set an valid input shell!";
  }

  mitk::Image::RegionType outputRegion = image->GetRequestedRegion();

  int tstart = outputRegion.GetIndex(3);
  int tmax = tstart + outputRegion.GetSize(3); // GetSize()==1 - will aber 0 haben, wenn nicht zeitaufgeloest

  if ((tmax - tstart) > 0)
  {
    ProgressBar::GetInstance()->AddStepsToDo(4 * (tmax - tstart));
  }
 
  for (int t = tstart; t < tmax; ++t)
  {
    auto shellImage = this->Stencil3DImage(shell, t);

    auto replace = vtkSmartPointer<vtkImageMathematics>::New();
    replace->SetInputData(shellImage->GetVtkImageData(t));
    replace->SetConstantC(1);
    replace->SetConstantK(3);
    replace->SetOperationToReplaceCByK();
    replace->Update();

    auto sub = vtkSmartPointer<vtkImageMathematics>::New();
    sub->SetInput1Data(image->GetVtkImageData(t));
    sub->SetInput2Data(shellImage->GetVtkImageData(t));
    sub->SetOperationToSubtract();
    sub->Update();

    auto erodedImage = mitk::Image::New();
    erodedImage->Initialize(image);
    erodedImage->SetVolume(sub->GetOutput()->GetScalarPointer(), t);

    mitk::MorphologicalOperations::Erode(erodedImage, 3, mitk::MorphologicalOperations::Ball);

    auto add = vtkSmartPointer<vtkImageMathematics>::New();
    add->SetInput1Data(erodedImage->GetVtkImageData(t));
    add->SetInput2Data(sub->GetOutput());
    add->SetOperationToAdd();
    add->Update();

    add->SetInput1Data(add->GetOutput());
    add->SetInput2Data(replace->GetOutput());
    add->Update();

    auto reamerImage = this->Stencil3DImage(reamer, t, true);

    auto multiply = vtkSmartPointer<vtkImageMathematics>::New();
    multiply->SetInput1Data(add->GetOutput());
    multiply->SetInput2Data(reamerImage->GetVtkImageData());
    multiply->SetOperationToMultiply();
    multiply->Update();

    shellImage->SetVolume(multiply->GetOutput()->GetScalarPointer(), t);

    CreateSurface(t, shellImage->GetVtkImageData(t), surface, m_Threshold);
    ProgressBar::GetInstance()->Progress();
  }
}

void mitk::ReamingFilter::CreateSurface(int time, vtkImageData * vtkimage, mitk::Surface * surface, const ScalarType threshold)
{
  auto indexCoordinatesImageFilter = vtkSmartPointer<vtkImageChangeInformation>::New();
  indexCoordinatesImageFilter->SetInputData(vtkimage);
  indexCoordinatesImageFilter->SetOutputOrigin(0.0, 0.0, 0.0);
  indexCoordinatesImageFilter->Update();
  

  // MarchingCube -->create Surface
  auto skinExtractor = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
  skinExtractor->SetInputConnection(indexCoordinatesImageFilter->GetOutputPort()); // RC++
  skinExtractor->ComputeScalarsOn();
  skinExtractor->GenerateValues(threshold, 1, threshold);

  vtkPolyData *polydata;
  skinExtractor->Update();
  polydata = skinExtractor->GetOutput();
  polydata->Register(nullptr); // RC++

  if (m_Smooth)
  {
    vtkSmoothPolyDataFilter *smoother = vtkSmoothPolyDataFilter::New();
    // read poly1 (poly1 can be the original polygon, or the decimated polygon)
    smoother->SetInputConnection(skinExtractor->GetOutputPort()); // RC++
    smoother->SetNumberOfIterations(m_SmoothIteration);
    smoother->SetRelaxationFactor(m_SmoothRelaxation);
    smoother->SetFeatureAngle(60);
    smoother->FeatureEdgeSmoothingOff();
    smoother->BoundarySmoothingOff();
    smoother->SetConvergence(0);
    smoother->Update();

    polydata->Delete(); // RC--
    polydata = smoother->GetOutput();
    polydata->Register(nullptr); // RC++
    smoother->Delete();
  }
  ProgressBar::GetInstance()->Progress();

  // decimate = to reduce number of polygons
  if (m_Decimate == DecimatePro)
  {
    vtkDecimatePro *decimate = vtkDecimatePro::New();
    decimate->SplittingOff();
    decimate->SetErrorIsAbsolute(5);
    decimate->SetFeatureAngle(30);
    decimate->PreserveTopologyOn();
    decimate->BoundaryVertexDeletionOff();
    decimate->SetDegree(10); // std-value is 25!

    decimate->SetInputData(polydata); // RC++
    decimate->SetTargetReduction(m_TargetReduction);
    decimate->SetMaximumError(0.002);
    decimate->Update();

    polydata->Delete(); // RC--
    polydata = decimate->GetOutput();
    polydata->Register(nullptr); // RC++
    decimate->Delete();
  }
  else if (m_Decimate == QuadricDecimation)
  {
    vtkQuadricDecimation *decimate = vtkQuadricDecimation::New();
    decimate->SetTargetReduction(m_TargetReduction);

    decimate->SetInputData(polydata);
    decimate->Update();
    polydata->Delete();
    polydata = decimate->GetOutput();
    polydata->Register(nullptr);
    decimate->Delete();
  }

  ProgressBar::GetInstance()->Progress();

  if (polydata->GetNumberOfPoints() > 0)
  {
    mitk::Vector3D spacing = GetInput()->GetGeometry(time)->GetSpacing();

    vtkPoints *points = polydata->GetPoints();
    vtkMatrix4x4 *vtkmatrix = vtkMatrix4x4::New();
    GetInput()->GetGeometry(time)->GetVtkTransform()->GetMatrix(vtkmatrix);
    double(*matrix)[4] = vtkmatrix->Element;

    unsigned int i, j;
    for (i = 0; i < 3; ++i)
      for (j = 0; j < 3; ++j)
        matrix[i][j] /= spacing[j];

    unsigned int n = points->GetNumberOfPoints();
    double point[3];

    for (i = 0; i < n; i++)
    {
      points->GetPoint(i, point);
      mitkVtkLinearTransformPoint(matrix, point, point);
      points->SetPoint(i, point);
    }
    vtkmatrix->Delete();
  }
  ProgressBar::GetInstance()->Progress();

  // determine point_data normals for the poly data points.
  vtkSmartPointer<vtkPolyDataNormals> normalsGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
  normalsGenerator->SetInputData(polydata);
  normalsGenerator->FlipNormalsOn();

  

  vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
  cleanPolyDataFilter->SetInputConnection(normalsGenerator->GetOutputPort());
  cleanPolyDataFilter->PieceInvariantOff();
  cleanPolyDataFilter->ConvertLinesToPointsOff();
  cleanPolyDataFilter->ConvertPolysToLinesOff();
  cleanPolyDataFilter->ConvertStripsToPolysOff();
  cleanPolyDataFilter->PointMergingOn();
  cleanPolyDataFilter->Update();

  surface->SetVtkPolyData(cleanPolyDataFilter->GetOutput(), time);
  polydata->UnRegister(nullptr);

}

mitk::Image::Pointer mitk::ReamingFilter::Stencil3DImage(const mitk::Surface *surface, int time, bool reverse)
{
  mitk::Image::Pointer output = mitk::Image::New();

  //unsigned int size = sizeof(unsigned char);
  //if (m_MakeOutputBinary)
  //{
  //  if (m_UShortBinaryPixelType)
  //  {
  //    binaryImage->Initialize(mitk::MakeScalarPixelType<unsigned short>(), *this->GetImage()->GetTimeGeometry(), 1, 1);
  //    size = sizeof(unsigned short);
  //  }
  //  else
  //  {
  //    binaryImage->Initialize(mitk::MakeScalarPixelType<unsigned char>(), *this->GetImage()->GetTimeGeometry(), 1, 1);
  //  }
  //}
  //else
  //{
  output->Initialize(this->GetInput()->GetPixelType(), *this->GetInput()->GetTimeGeometry(), 1, 1);
  //  size = this->GetImage()->GetPixelType().GetSize();
  //}

  //for (unsigned int i = 0; i < binaryImage->GetDimension(); ++i)
  //{
  //  size *= binaryImage->GetDimension(i);
  //}

  //mitk::ImageWriteAccessor accessor(binaryImage);
  //memset(accessor.GetData(), 1, size);

  const mitk::TimeGeometry *surfaceTimeGeometry = surface->GetTimeGeometry();
  const mitk::TimeGeometry *imageTimeGeometry = this->GetInput()->GetTimeGeometry();

  // Convert time step from image time-frame to surface time-frame
  mitk::TimePointType matchingTimePoint = imageTimeGeometry->TimeStepToTimePoint(time);
  mitk::TimeStepType surfaceTimeStep = surfaceTimeGeometry->TimePointToTimeStep(matchingTimePoint);

  vtkPolyData *polydata = surface->GetVtkPolyData(surfaceTimeStep);
  vtkSmartPointer<vtkTransformPolyDataFilter> move = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  move->SetInputData(polydata);
  move->ReleaseDataFlagOn();

  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  BaseGeometry *geometry = surfaceTimeGeometry->GetGeometryForTimeStep(surfaceTimeStep);
  if (!geometry)
  {
    geometry = GetInput()->GetGeometry();
  }
  transform->PostMultiply();
  transform->Concatenate(geometry->GetVtkTransform()->GetMatrix());
  // take image geometry into account. vtk-Image information will be changed to unit spacing and zero origin below.
  BaseGeometry *imageGeometry = imageTimeGeometry->GetGeometryForTimeStep(time);
  transform->Concatenate(imageGeometry->GetVtkTransform()->GetLinearInverse());
  move->SetTransform(transform);
  move->Update();

  //vtkSmartPointer<vtkPolyDataNormals> normalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
  //normalsFilter->SetFeatureAngle(50);
  //normalsFilter->SetConsistency(1);
  //normalsFilter->SetSplitting(1);
  //normalsFilter->SetFlipNormals(0);
  //normalsFilter->ReleaseDataFlagOn();

  //normalsFilter->SetInputConnection(move->GetOutputPort());

  vtkSmartPointer<vtkPolyDataToImageStencil> surfaceConverter = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
  surfaceConverter->SetTolerance(m_Tolerance);
  surfaceConverter->ReleaseDataFlagOn();

  surfaceConverter->SetInputConnection(move->GetOutputPort());
  surfaceConverter->Update();

  vtkImageData *image = const_cast<vtkImageData*>(this->GetInput()->GetVtkImageData());
  //m_MakeOutputBinary ? binaryImage->GetVtkImageData() :
  //const_cast<mitk::Image *>(this->GetImage())->GetVtkImageData(time);

// fill the image with foreground voxels:
//unsigned char inval = 1;
//vtkIdType count = image->GetNumberOfPoints();
//for (vtkIdType i = 0; i < count; ++i)
//{
//  image->GetPointData()->GetScalars()->SetTuple1(i, inval);
//}

// Create stencil and use numerical minimum of pixel type as background value
  vtkSmartPointer<vtkImageStencil> stencil = vtkSmartPointer<vtkImageStencil>::New();
  stencil->SetInputData(image);
  stencil->SetReverseStencil(reverse);
  stencil->ReleaseDataFlagOn();
  stencil->SetStencilConnection(surfaceConverter->GetOutputPort());

  stencil->SetBackgroundValue(0);
  stencil->Update();

  output->SetVolume(stencil->GetOutput()->GetScalarPointer(), time);
  MITK_INFO << "stencil ref count: " << stencil->GetReferenceCount() << std::endl;
  return output;
}
