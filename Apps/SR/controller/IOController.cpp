/**
 * @file IOController.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "IOController.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>

// qt
#include <QSharedPointer>

QSharedPointer<IOController> instance =
    QSharedPointer<IOController>(new IOController);

IOController* IOController::getInstance()
{
  return instance.get();
}

void IOController::loadScene(QString fileName)
{
  auto sceneIO = mitk::SceneIO::New();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto dataNodes = sceneIO->LoadScene(fileName.toStdString(), ds)->GetAll();
  MITK_INFO << "DataStorage, num of nodes: " << dataNodes->size();
  for (const mitk::DataNode* one : *dataNodes)
  {
    MITK_INFO << one->GetName();
  }
  Q_EMIT this->sceneLoaded();
}

void IOController::saveScene(QString fileName)
{
  auto sceneIO = mitk::SceneIO::New();
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  bool isSuccess = sceneIO->SaveScene(ds->GetAll(), ds, fileName.toStdString());
  if (isSuccess)
  {
    MITK_INFO << "Saved to " + fileName.toStdString() + ".";
  }
  else
  {
    MITK_INFO << "Failed to save to " + fileName.toStdString() + ".";
  }
}

// IOController::IOController(QObject *parent): QObject(parent)
// {
// }