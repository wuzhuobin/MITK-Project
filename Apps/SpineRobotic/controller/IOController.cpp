#include "IOController.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>


// qt
#include <QSharedPointer>
#include <qobjectdefs.h>

QSharedPointer<IOController> instance = QSharedPointer<IOController>(new IOController);

IOController * IOController::getInstance()
{
  return instance.get();
}

void IOController::loadScene(QString fileName)
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
  Q_EMIT this->sceneLoaded();
}

void IOController::saveScene(QString fileName)
{
  mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  bool isSuccess = sceneIO->SaveScene(ds->GetAll(), ds, fileName.toStdString());
  if (isSuccess)
  {
    MITK_INFO << "Saved to " + fileName.toStdString() + ".";
  }
  else {
    MITK_INFO << "Failed to save to " + fileName.toStdString() + ".";
  }
}

// IOController::IOController(QObject *parent): QObject(parent)
// {
// }