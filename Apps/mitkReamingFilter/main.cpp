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

  mitk::ReamerSource::Pointer reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(25);
  reamerSource->SetLength(70);
  reamerSource->Update();
  mitk::Surface * shell = reamerSource->GetOutput();

  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateY(-90);
  transform->RotateX(0);
  transform->RotateZ(0);
  transform->Translate(
    -28.54936906586104,
    14.204643754424644,
    1578.9678664012258);
  transform->Update();

  vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(shell->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  shell->SetVtkPolyData(transformPolyData->GetOutput());

  auto reamingFilter = ReamingFilter::New();
  reamingFilter->SetInput(overlay);
  reamingFilter->SetReamer(shell);
  reamingFilter->SetShell(shell);
  reamingFilter->SetThreshold(2);
  reamingFilter->Update();

  IOUtil::Save(reamingFilter->GetOutput(), "output.vtp");



  return 0;
}