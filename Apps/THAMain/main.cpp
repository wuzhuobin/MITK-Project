/**
 * @file main.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "MainWindow.h"

// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <mitkRenderingManager.h>
#include <mitkStandaloneDataStorage.h>

int main(int argc, char* argv[])
{
  // Register Qmitk-dependent global instances
  QmitkRegisterClasses();
  mitk::RenderingManager::GetInstance()->SetDataStorage(
      mitk::StandaloneDataStorage::New());
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  // w.test();
  return app.exec();
}