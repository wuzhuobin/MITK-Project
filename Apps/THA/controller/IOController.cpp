#include "IOController.h"

#include "ReamingFilter2.h"
#include "mitkReamerSource.h"

// mitk
#include <mitkIOUtil.h>
#include <mitkLogMacros.h>
#include <mitkLookupTable.h>
#include <mitkLookupTableProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkSurface.h>
#include <mitkVtkRepresentationProperty.h>
#include <mitkVtkScalarModeProperty.h>

// vtk
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

// qt
#include <qapplication.h>
#include <qcoreapplication.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

#include <QSharedPointer>

QString IOController::getBaseProject()
{
  static QString gBaseProject(qApp->applicationDirPath() + "/THA.mitk");
  return gBaseProject;
}

IOController* IOController::getInstance()
{
  static IOController gInstance;
  return &gInstance;
}

IOController::IOController(QObject* parent) : QObject(parent)
{
  initCaseDataBase();
}

void IOController::loadScene(const QString& fileName) const
{
  auto sceneIO = mitk::SceneIO::New();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  // auto dataNodes = sceneIO->LoadScene(fileName.toStdString(), ds)->GetAll();
  // hardcoded for now
  auto dataNodes =
      sceneIO
          ->LoadScene((qApp->applicationDirPath() + "/cases/first_case.0")
                          .toStdString(),
                      ds)
          ->GetAll();
  MITK_INFO << "DataStorage, num of nodes: " << dataNodes->size();
  for (const auto& one : *dataNodes)
  {
    MITK_INFO << one->GetName();
  }

  addReamer();
  addReamerTrajectory();
  addReamingPelvis();
  addImpactingAcetabularShell();
  addImpactingAcetabularShellCor();
  Q_EMIT sceneLoaded();
}

bool IOController::createScene(const QString& filename,
                               const QStringList& dicoms) const
{
  auto sceneIO = mitk::SceneIO::New();
  auto ds = mitk::StandaloneDataStorage::New();
  sceneIO->LoadScene(getBaseProject().toStdString(), ds);
  ds->Remove(ds->GetNamedNode("image"));

  auto setObObjects = mitk::IOUtil::Load(dicoms.first().toStdString(), *ds);
  setObObjects->front()->SetName("image");

  return sceneIO->SaveScene(ds->GetAll(), ds, filename.toStdString());
}

void IOController::addReamerTrajectory() const
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(25);
  reamerSource->SetLength(70);
  reamerSource->Update();
  auto* reamer = reamerSource->GetOutput();

  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateY(-90);
  transform->RotateX(0);
  transform->RotateZ(0);
  transform->Translate(
      -58.54936906586104, 14.204643754424644, 1578.9678664012258);
  // transform->Translate(
  //     -28.54936906586104,
  //     14.204643754424644,
  //     1578.9678664012258);
  transform->Update();

  auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(reamer->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  reamer->SetVtkPolyData(transformPolyData->GetOutput());

  auto reamerNode = mitk::DataNode::New();
  reamerNode->SetData(reamer);
  reamerNode->SetName("reamer_trajectory");
  reamerNode->SetVisibility(false);

  ds->Add(reamerNode);
}

void IOController::addReamer() const
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto reamerSource = mitk::ReamerSource::New();
  reamerSource->SetSphereRadius(25);
  reamerSource->SetTubeRadius(2);
  reamerSource->SetLength(70);
  reamerSource->SetThetaResolution(10);
  reamerSource->SetPhiResolution(10);
  reamerSource->Update();
  auto* reamer = reamerSource->GetOutput();

  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateY(-90);
  transform->RotateX(0);
  transform->RotateZ(0);
  // transform->Translate(-58.54936906586104, 14.204643754424644,
  // 1578.9678664012258);
  transform->Translate(
      -108.54936906586104, 14.204643754424644, 1578.9678664012258);
  transform->Update();

  auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(reamer->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  reamer->SetVtkPolyData(transformPolyData->GetOutput());

  auto reamerNode = mitk::DataNode::New();
  reamerNode->SetData(reamer);
  reamerNode->SetName("reamer");
  reamerNode->SetVisibility(false);
  reamerNode->SetProperty("material.representation",
                          mitk::VtkRepresentationProperty::New("Wireframe"));

  ds->Add(reamerNode);
}

void IOController::addReamingPelvis() const
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* overlay = ds->GetNamedObject<mitk::Image>("overlay");
  auto* overlayMatrix = overlay->GetGeometry()->GetVtkMatrix();
  auto overlaySpacing = overlay->GetGeometry()->GetSpacing();
  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->SetMatrix(overlayMatrix);
  transform->Scale(
      1 / overlaySpacing[0], 1 / overlaySpacing[1], 1 / overlaySpacing[2]);
  auto* reamerTrajectory =
      ds->GetNamedObject<mitk::Surface>("reamer_trajectory");

  auto reamingFilter = vtkSmartPointer<ReamingFilter2>::New();
  reamingFilter->SetImage(overlay->GetVtkImageData());
  reamingFilter->SetReamerTrajectory(reamerTrajectory->GetVtkPolyData());
  reamingFilter->SetSize(3);
  reamingFilter->SetImageTransform(transform);
  reamingFilter->SetExtent(100, 250, 200, 300, 1150, 1270);
  reamingFilter->SetUseSmooth(true);
  reamingFilter->Update();

  auto reamingPelvis = mitk::Surface::New();
  reamingPelvis->SetVtkPolyData(reamingFilter->GetOutput());

  auto vtkLut = vtkSmartPointer<vtkLookupTable>::New();
  vtkLut->SetTableRange(0, 3);
  vtkLut->SetNumberOfTableValues(4);
  vtkLut->SetTableValue(0, 0, 0, 0, 0);
  vtkLut->SetTableValue(1, 0, 1, 0, 1);
  vtkLut->SetTableValue(2, 1, 1, 1, 1);
  vtkLut->SetTableValue(3, 1, 0, 0, 1);
  vtkLut->Build();

  auto lut = mitk::LookupTable::New();
  lut->SetVtkLookupTable(vtkLut);

  auto lutProperty = mitk::LookupTableProperty::New(lut);

  auto reamingPelvisNode = mitk::DataNode::New();
  reamingPelvisNode->SetName("reaming_pelvis");
  reamingPelvisNode->SetData(reamingPelvis);
  reamingPelvisNode->SetProperty("LookupTable", lutProperty);
  reamingPelvisNode->SetProperty("scalar mode",
                                 mitk::VtkScalarModeProperty::New("Default"));
  reamingPelvisNode->SetBoolProperty("color mode", true);
  reamingPelvisNode->SetBoolProperty("scalar visibility", true);
  reamingPelvisNode->SetFloatProperty("ScalarsRangeMinimum",
                                      vtkLut->GetTableRange()[0]);
  reamingPelvisNode->SetFloatProperty("ScalarsRangeMaximum",
                                      vtkLut->GetTableRange()[1]);

  ds->Add(reamingPelvisNode);
}

void IOController::addImpactingAcetabularShell() const
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* acetabularShell = ds->GetNamedObject<mitk::Surface>("acetabular_shell");
  auto acetabularShellImpacting = acetabularShell->Clone();
  acetabularShellImpacting->GetGeometry()
      ->GetIndexToWorldTransform()
      ->SetIdentity();
  auto acetabularShellImpactingNode = mitk::DataNode::New();
  acetabularShellImpactingNode->SetName("impacting_acetabular_shell");
  acetabularShellImpactingNode->SetVisibility(true);
  acetabularShellImpactingNode->SetColor(0, 0, 1);
  acetabularShellImpactingNode->SetData(acetabularShellImpacting);

  ds->Add(acetabularShellImpactingNode);
}

void IOController::addImpactingAcetabularShellCor() const
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* cor = ds->GetNamedObject<mitk::PointSet>("cup_cor");
  auto impactingAcetabularShellCor = cor->Clone();
  impactingAcetabularShellCor->GetGeometry()
      ->GetIndexToWorldTransform()
      ->SetIdentity();

  auto corImpactingAcetabularShellCorNode = mitk::DataNode::New();
  corImpactingAcetabularShellCorNode->SetData(impactingAcetabularShellCor);
  corImpactingAcetabularShellCorNode->SetName("impacting_acetabular_shell_cor");
  corImpactingAcetabularShellCorNode->SetVisibility(true);

  ds->Add(corImpactingAcetabularShellCorNode);
}

void IOController::initCaseDataBase() const
{
  auto db = QSqlDatabase::addDatabase("QSQLITE");
  MITK_INFO << (qApp->applicationDirPath() + "/tha.sql").toStdString();
  db.setDatabaseName(qApp->applicationDirPath() + "/tha.sql");
  if (!db.open())
  {
    MITK_INFO << db.lastError().text().toStdString();
  }

  QSqlQuery createTable(
      "CREATE TABLE IF NOT EXISTS \"Cases\" ("
      "\"ID\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
      "\"Name\"	TEXT NOT NULL,"
      "\"DateTime\"	TEXT NOT NULL,"
      "\"Description\"	TEXT,"
      "\"File\"	TEXT"
      ")");
  if (!createTable.exec())
  {
    MITK_INFO << createTable.lastError().text().toStdString();
  }
  QSqlQuery insert("");

  if (insert.exec())
  {
    MITK_INFO << createTable.lastError().text().toStdString();
  }
}