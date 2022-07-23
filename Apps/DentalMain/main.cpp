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
#include "SCBDentalMainWindow.h"

// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <mitkRenderingManager.h>
#include <mitkStandaloneDataStorage.h>

int main(int argc, char* argv[])
{
  QmitkRegisterClasses();
  mitk::RenderingManager::GetInstance()->SetDataStorage(
      mitk::StandaloneDataStorage::New());
  QApplication app(argc, argv);

  SCBDentalMainWindow w;
  w.show();
  w.test();
  w.on_tabWidgetCentral_currentChanged(0);
  w.slotWelcomeWizard();

  return app.exec();
}
