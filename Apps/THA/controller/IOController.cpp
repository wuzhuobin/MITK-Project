#include "IOController.h"
#include "mitkReamerSource.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkSurface.h>

// vtk
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>

// qt
#include <QSharedPointer>

QSharedPointer<IOController> instance = QSharedPointer<IOController>(new IOController);

IOController * IOController::GetInstance()
{
  return instance.get();
}

void IOController::LoadScene(QString fileName)
{
  mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataStorage::SetOfObjects::ConstPointer dataNodes =
      sceneIO->LoadScene(fileName.toStdString(), ds)->GetAll();
  MITK_INFO << "DataStorage, num of nodes: " << dataNodes->size();
  for (const mitk::DataNode *one : *dataNodes)
  {
    MITK_INFO << one->GetName();
  }

  this->AddReamer();
  this->AddReamerTrajectory();
  Q_EMIT this->SceneLoaded();
}

void IOController::AddReamerTrajectory()
{
  mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::ReamerSource::Pointer reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(25);
  reamerSource->SetLength(70);
  reamerSource->Update();
  mitk::Surface * reamer = reamerSource->GetOutput();

  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateY(-90);
  transform->RotateX(0);
  transform->RotateZ(0);
  transform->Translate(
      -58.54936906586104,
      14.204643754424644,
      1578.9678664012258);
  // transform->Translate(
  //     -28.54936906586104,
  //     14.204643754424644,
  //     1578.9678664012258);
  transform->Update();

  vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(reamer->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  reamer->SetVtkPolyData(transformPolyData->GetOutput());
  
  mitk::DataNode::Pointer reamerNode = mitk::DataNode::New();
  reamerNode->SetData(reamer);
  reamerNode->SetName("reamer_trajectory");
  reamerNode->SetVisibility(false);
  
  ds->Add(reamerNode);
}

void IOController::AddReamer()
{
  mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::ReamerSource::Pointer reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(2);
  reamerSource->SetLength(70);
  reamerSource->Update();
  mitk::Surface * reamer = reamerSource->GetOutput();

  vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateY(-90);
  transform->RotateX(0);
  transform->RotateZ(0);
  transform->Translate(
      -58.54936906586104,
      14.204643754424644,
      1578.9678664012258);
  transform->Update();

  vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData =
    vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(reamer->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  reamer->SetVtkPolyData(transformPolyData->GetOutput());
  
  mitk::DataNode::Pointer reamerNode = mitk::DataNode::New();
  reamerNode->SetData(reamer);
  reamerNode->SetName("reamer");
  reamerNode->SetVisibility(false);
  
  ds->Add(reamerNode);
}

void IOController::AddReamerCuter()
{

}

IOController::IOController(QObject *parent): QObject(parent)
{
}