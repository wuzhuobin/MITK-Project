#include "QTransformWidget.hpp"
#include "ReamingFilter3.hpp"

// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <QmitkRenderWindow.h>
#include <mitkDataNode.h>
#include <mitkIOUtil.h>
#include <mitkLogMacros.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkVtkRepresentationProperty.h>

// vtk
#include <vtkBox.h>
#include <vtkClipPolyData.h>
#include <vtkCubeSource.h>
#include <vtkNew.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QmitkRegisterClasses();

  auto dataStorage = mitk::StandaloneDataStorage::New();
  std::string fileName = "../../../Data/left (smoothed).vtp";
  auto left = mitk::IOUtil::Load<mitk::Surface>(fileName);

  auto box = vtkNew<vtkBox>();
  box->SetBounds(70, 70 + 100, -20, -20 + 100, 1137, 1137 + 116);

  auto clip = vtkNew<vtkClipPolyData>();
  clip->SetInputData(left->GetVtkPolyData());
  clip->SetClipFunction(box);
  clip->InsideOutOn();
  clip->Update();
  left->SetVtkPolyData(clip->GetOutput());

  auto leftNode = mitk::DataNode::New();
  leftNode->SetData(left);
  leftNode->SetName("left");
  leftNode->SetColor(0, 1, 0);
  leftNode->SetOpacity(0.5);
  leftNode->SetVisibility(false);
  dataStorage->Add(leftNode);

  auto cubeSource = vtkNew<vtkCubeSource>();
  cubeSource->SetCenter(134, 17, 1154);
  cubeSource->SetXLength(30);
  cubeSource->SetYLength(30);
  cubeSource->SetZLength(30);
  cubeSource->Update();

  auto transform = vtkNew<vtkTransform>();
  auto transformPolyDataFilter = vtkNew<vtkTransformPolyDataFilter>();
  transformPolyDataFilter->SetInputData(cubeSource->GetOutput());
  transformPolyDataFilter->SetTransform(transform);
  transformPolyDataFilter->Update();

  auto cube = mitk::Surface::New();
  cube->SetVtkPolyData(transformPolyDataFilter->GetOutput());
  auto cubeNode = mitk::DataNode::New();
  cubeNode->SetName("cube");
  cubeNode->SetData(cube);
  cubeNode->SetProperty("material.representation",
                        mitk::VtkRepresentationProperty::New("Wireframe"));
  dataStorage->Add(cubeNode);

  auto reamingFilter3 = vtkNew<ReamingFilter3>();
  reamingFilter3->SetInputData(left->GetVtkPolyData());
  // reamingFilter3->SetReamer(cube->GetVtkPolyData());
  reamingFilter3->SetInputConnection(1,
                                     transformPolyDataFilter->GetOutputPort());
  reamingFilter3->Update();

  auto reaming = mitk::Surface::New();
  reaming->SetVtkPolyData(reamingFilter3->GetOutput());

  auto reamingNode = mitk::DataNode::New();
  reamingNode->SetName("reaming");
  reamingNode->SetData(reaming);
  reamingNode->SetColor(1, 0, 0);
  // reamingNode->SetOpacity(0.5);
  dataStorage->Add(reamingNode);

  QmitkRenderWindow renderWindow;
  renderWindow.GetRenderer()->SetDataStorage(dataStorage);
  renderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  renderWindow.show();
  renderWindow.resize(800, 600);

  QTransformWidget transformWidget(transform, [&]() {
    transformPolyDataFilter->Update();
    reamingFilter3->Update();
    cube->SetVtkPolyData(transformPolyDataFilter->GetOutput());
    reaming->SetVtkPolyData(reamingFilter3->GetOutput());
    MITK_INFO << *transform << '\n';
    MITK_INFO << *reamingFilter3->GetInput() << '\n';
    MITK_INFO << *reamingFilter3->GetOutput() << '\n';
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  });
  transformWidget.show();

  // mitk::RenderingManager::GetInstance()->InitializeViews();
  mitk::RenderingManager::GetInstance()->InitializeViews(
      dataStorage->ComputeBoundingGeometry3D());
  // mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  return app.exec();
}