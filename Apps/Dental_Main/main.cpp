#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTranslator>

// me
#include "SCBDentalHub.h"
#include "SCBDentalMainWindow.h"
// #include "MACKeyconfirm.hpp"
// #include "SCBDentalConfiguration.h"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")

int main(int argc, char* argv[])
{
  // using namespace QsLogging;

  QApplication app(argc, argv);
  //   SCBSceneConfiguration();

  QTranslator qtTranslator;
  qtTranslator.load("lang_" + QLocale::system().name());
  // qtTranslator.load("lang_zh_CN.ts");
  app.installTranslator(&qtTranslator);

  // QFile styleFile(":/HyperBlue/hyperblue/styles.qss");
  // if (styleFile.open(QIODevice::ReadOnly)) {
  //	app.setStyleSheet(styleFile.readAll());
  // }
  SCBDentalHub hub;
  if (hub.mainWindow->slotMacChecking())
  {
    hub.mainWindow->show();
    // hub.mainWindow->showFullScreen();
    hub.mainWindow->on_tabWidgetCentral_currentChanged(0);
    hub.mainWindow->slotWelcomeWizard();
    // hub.slotInitializationImages(QStringList() <<
    // QCoreApplication::applicationDirPath() +
    //	"/HardCode/seg_resample.nii.gz");
  }

  ////QPixmap pixmap(":/HyperBlue/hyperblue/icon/product-splashScreen.png");
  // QPixmap pixmap(":/HyperBlue/hyperblue/icon/Orthopedics-splashscreen2.png");
  // QSplashScreen splash(pixmap);
  // splash.show();
  // app.processEvents();

  // splash.finish(NULL);
  // hub.slotImport("C:/Users/jieji/Desktop/mandible.xml", true);

  return app.exec();
}
