#include "IOController.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>

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
}

IOController::IOController(QObject *parent): QObject(parent)
{
}