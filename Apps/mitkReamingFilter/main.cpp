#include "mitkReamingFilter.h"
#include "mitkReamerSource.h"

// mitk
#include <mitkSceneIO.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkSurface.h>
#include <mitkImage.h>
#include <mitkSurfaceToImageFilter.h>
#include <mitkIOUtil.h>

// vtk
#include <vtkTransformPolyDataFilter.h>


using namespace mitk;
int main(int argc, char *argv[])
{
  //auto ds = StandaloneDataStorage::New();
  auto io = SceneIO::New();
  auto ds = io->LoadScene("THA.mitk");
  //auto shell = ds->GetNamedObject<Surface>("acetabular_shell");
  auto overlay = ds->GetNamedObject<Image>("overlay");

  mitk::ReamerSource::Pointer shellSource = mitk::ReamerSource::New();
  shellSource->SetSphereRadius(25);
  shellSource->SetTubeRadius(25);
  shellSource->SetLength(70);
  shellSource->Update();

  mitk::Surface * shell = shellSource->GetOutput();

  vtkSmartPointer<vtkTransform> transformShell =
    vtkSmartPointer<vtkTransform>::New();
  transformShell->PostMultiply();
  transformShell->RotateY(-90);
  transformShell->RotateX(0);
  transformShell->RotateZ(0);
  transformShell->Translate(
    -28.54936906586104,
    14.204643754424644,
    1578.9678664012258);
  transformShell->Update();

  vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataShell =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyDataShell->SetInputData(shell->GetVtkPolyData());
  transformPolyDataShell->SetTransform(transformShell);
  transformPolyDataShell->Update();
  shell->SetVtkPolyData(transformPolyDataShell->GetOutput());

  
  auto reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(25);
  reamerSource->SetLength(70);
  reamerSource->Update();
  mitk::Surface * reamer = reamerSource->GetOutput();

  vtkSmartPointer<vtkTransform> transformReamer =
    vtkSmartPointer<vtkTransform>::New();
  transformReamer->PostMultiply();
  transformReamer->RotateY(-90);
  transformReamer->RotateX(0);
  transformReamer->RotateZ(90);
  transformReamer->Translate(
    -58.54936906586104,
    14.204643754424644,
    1578.9678664012258);
  transformReamer->Update();

  vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataReamer =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyDataReamer->SetInputData(reamer->GetVtkPolyData());
  transformPolyDataReamer->SetTransform(transformReamer);
  transformPolyDataReamer->Update();
  reamer->SetVtkPolyData(transformPolyDataReamer->GetOutput());

  auto reamingFilter = ReamingFilter::New();
  reamingFilter->SetInput(overlay);
  reamingFilter->SetReamer(reamer);
  reamingFilter->SetShell(shell);
  reamingFilter->SetThreshold(3);
  reamingFilter->Update();

  IOUtil::Save(reamingFilter->GetOutput(), "output.vtp");

  return 0;
}