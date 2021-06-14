#include "MainWindow.h"

// qt
#include <QApplication>

// mitk
#include <QmitkRegisterClasses.h>
#include <mitkRenderingManager.h>
#include <mitkStandaloneDataStorage.h>

int main(int argc, char *argv[])
{
  // Register Qmitk-dependent global instances
  QmitkRegisterClasses();
  mitk::RenderingManager::GetInstance()->SetDataStorage(mitk::StandaloneDataStorage::New());
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  w.test();
  return app.exec();
}