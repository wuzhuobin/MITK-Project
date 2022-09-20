/**
 * @file main.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "BoneSegmentationWidget.h"

// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <QmitkStdMultiWidget.h>
#include <mitkIOUtil.h>
#include <mitkRenderingManager.h>
#include <mitkStandaloneDataStorage.h>

int main(int argc, char* argv[])
{
  QmitkRegisterClasses();
  mitk::RenderingManager::GetInstance()->SetDataStorage(
      mitk::StandaloneDataStorage::New());
  QApplication app(argc, argv);

  auto imageFileName = QApplication::applicationDirPath() + "/dental.nii.gz";
  auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName.toStdString());
  auto imageNode = mitk::DataNode::New();
  imageNode->SetName("image");
  imageNode->SetData(image);

  mitk::RenderingManager::GetInstance()->GetDataStorage()->Add(imageNode);

  QmitkStdMultiWidget multiWidget;
  multiWidget.SetDataStorage(
      mitk::RenderingManager::GetInstance()->GetDataStorage());
  multiWidget.InitializeMultiWidget();
  multiWidget.AddPlanesToDataStorage();
  multiWidget.SetCrosshairVisibility(true);
  multiWidget.ResetCrosshair();
  multiWidget.resize(1000, 1000);
  multiWidget.show();

  BoneSegmentationWidget segWidget;
  segWidget.show();

  return app.exec();
}